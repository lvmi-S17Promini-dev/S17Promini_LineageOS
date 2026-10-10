#!/usr/bin/env python3
"""Verify an S17 source-kernel boot against its compiled inputs; no device IO."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import struct
import subprocess
import tempfile
import zlib

ROOT = Path(__file__).resolve().parents[1]


def sha(data):
    return hashlib.sha256(data).hexdigest()


def require(condition, message):
    if not condition:
        raise ValueError(message)


def newc_files(compressed):
    raw = zlib.decompress(compressed, 31)
    entries, offset = {}, 0
    while True:
        require(raw[offset:offset + 6] == b"070701", "Bad newc ramdisk")
        fields = [int(raw[offset + 6 + i * 8:offset + 14 + i * 8], 16)
                  for i in range(13)]
        name = raw[offset + 110:offset + 110 + fields[11] - 1].decode().removeprefix("./")
        begin = (offset + 110 + fields[11] + 3) & ~3
        data = raw[begin:begin + fields[6]]
        if name == "TRAILER!!!":
            return entries
        require(name not in entries, "Duplicate ramdisk entry")
        entries[name] = (fields[1], data)
        offset = (begin + fields[6] + 3) & ~3


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--lineage", type=Path, default=Path.home() / "android3/lineage")
    parser.add_argument("--out", type=Path)
    parser.add_argument("--image", type=Path)
    parser.add_argument("--expected-release", default="4.19.127-s17-bringup-r1")
    parser.add_argument("--ufdt-tool", type=Path)
    parser.add_argument("--expected-gpu-version", default="rgx doma m1.13ED5776728")
    parser.add_argument("--collector-tag", default="r8")
    parser.add_argument("--collector-wrapper", type=Path, help="Validate the explicit debug init wrapper")
    parser.add_argument("--report", type=Path,
                        default=ROOT / "extracted/metadata/source-boot-r1-validation.json")
    args = parser.parse_args()
    out = args.out or args.lineage / "out-s17-sourceboot"
    product = out / "target/product/S17Promini"
    kernel_out = product / "obj/KERNEL_OBJ"
    image_path = args.image or product / "boot.img"
    image = image_path.read_bytes()
    require(len(image) == 33554432, "Boot image must fit the 32 MiB partition")
    require(image[:8] == b"ANDROID!", "Bad Android boot magic")
    fields = struct.unpack_from("<10I", image, 8)
    kernel_size, kernel_addr, ramdisk_size, ramdisk_addr = fields[:4]
    second_size, _, tags_addr, page_size, version, _ = fields[4:]
    require((kernel_addr, ramdisk_addr, tags_addr, page_size, version) ==
            (0x40080000, 0x51B00000, 0x47880000, 2048, 2), "Load addresses/header mismatch")
    require(second_size == 0 and struct.unpack_from("<I", image, 1632)[0] == 0,
            "Unexpected second-stage/recovery DTBO payload")
    require(struct.unpack_from("<I", image, 1644)[0] == 1660, "Bad boot header size")
    dtb_size, dtb_addr = struct.unpack_from("<IQ", image, 1648)
    require(dtb_addr == 0x47880000, "DTB load address mismatch")
    align = lambda size: (size + page_size - 1) // page_size * page_size
    ramdisk_start = page_size + align(kernel_size)
    dtb_start = ramdisk_start + align(ramdisk_size)
    kernel = image[page_size:page_size + kernel_size]
    ramdisk = image[ramdisk_start:ramdisk_start + ramdisk_size]
    dtb_table = image[dtb_start:dtb_start + dtb_size]
    source_kernel = kernel_out / "arch/arm64/boot/Image.gz"
    require(kernel == source_kernel.read_bytes(), "Boot kernel differs from source build output")
    require(kernel != (ROOT / "extracted/boot/kernel").read_bytes(), "Still using stock kernel")
    stock_ramdisk = (args.lineage / "device/lvmi/S17Promini/prebuilt/boot-ramdisk.img").read_bytes()
    if args.collector_wrapper:
        expected = newc_files(stock_ramdisk)
        original_init = expected.pop("init")
        expected["init.stock"] = original_init
        expected["init"] = (original_init[0], args.collector_wrapper.read_bytes())
        require(newc_files(ramdisk) == expected, "Debug ramdisk differs beyond init wrapper")
        require(b"initcall_blacklist=_mt_cpufreq_pdrv_init" in image[64:1632],
                "Collector image lost cpufreq blacklist")
    else:
        require(ramdisk == stock_ramdisk, "Stock first-stage ramdisk changed")
    require(dtb_table == (product / "dtb.img").read_bytes(), "Boot DT table differs from built table")
    table_header = struct.unpack_from(">8I", dtb_table)
    require(table_header[:6] == (0xD7B7AB1E, len(dtb_table), 32, 32, 1, 32), "Bad MTK DT table")
    entry = struct.unpack_from(">8I", dtb_table, 32)
    require(entry[1] == 64 and entry[0] + 64 == len(dtb_table), "Bad DT entry bounds")
    dtb = dtb_table[entry[1]:entry[1] + entry[0]]
    require(dtb == (kernel_out / "arch/arm64/boot/dts/mediatek/S17Promini.dtb").read_bytes(),
            "DT table does not contain the compiled board tree")
    decoder = zlib.decompressobj(31)
    uncompressed = decoder.decompress(kernel) + decoder.flush()
    require(decoder.eof and not decoder.unused_data, "Kernel must be standalone gzip without an appended DTB")
    linux_version = re.search(rb"Linux version [^\0]+", uncompressed)
    require(linux_version is not None, "Missing kernel identity")
    identity = linux_version.group().decode(errors="replace")
    require(args.expected_release in identity, "Wrong test kernel release")
    cmdline = image[64:576].split(b"\0")[0] + image[608:1632].split(b"\0")[0]
    require(b"bootopt=64S3,32N2,64N2" in cmdline, "Missing stock bootopt")
    if args.collector_wrapper:
        require(f"s17.bootlog={args.collector_tag}".encode() in cmdline, "Missing cachelog build marker")
    config = (kernel_out / ".config").read_text()
    if b"androidboot.selinux=permissive" in cmdline:
        require("CONFIG_SECURITY_SELINUX_DEVELOP=y" in config,
                "Kernel cannot honor permissive cmdline without SELINUX_DEVELOP")
    for required in ['CONFIG_TOUCHSCREEN_MTK_AXS=y',
                     'CONFIG_CUSTOM_KERNEL_LCM="axs15260_dsi_6580_hd"',
                     f'CONFIG_MTK_GPU_VERSION="{args.expected_gpu_version}"']:
        require(required in config.splitlines(), "Missing config: " + required)
    avbtool = args.lineage / "external/avb/avbtool.py"
    avb_info = subprocess.check_output(["python3", str(avbtool), "info_image", "--image", str(image_path)], text=True)
    require("NONE" in avb_info and "boot" in avb_info, "Unexpected AVB boot metadata")
    with tempfile.TemporaryDirectory(prefix="s17-boot-dtb-") as folder:
        # avbtool resolves the boot hash descriptor through a file named boot.img.
        avb_image = Path(folder) / "boot.img"
        avb_image.symlink_to(image_path.resolve())
        subprocess.run(["python3", str(avbtool), "verify_image", "--image", str(avb_image)], check=True)
        raw = Path(folder) / "board.dtb"
        merged = Path(folder) / "with-stock-overlay.dtb"
        raw.write_bytes(dtb)
        subprocess.run(["fdtoverlay", "-i", str(raw), "-o", str(merged),
                        str(ROOT / "extracted/metadata/stock-overlay-0.dtbo")], check=True)
        for tree in [raw, merged]:
            node = "/i2c0@11007000/axs15260@3b"
            reg = subprocess.check_output(["fdtget", "-tx", str(tree), node, "reg"], text=True).strip()
            require(reg == "3b", "Touch node lost during bootloader overlay application")
        if args.ufdt_tool:
            ufdt_merged = Path(folder) / "with-android-ufdt-overlay.dtb"
            subprocess.run([str(args.ufdt_tool.resolve()), str(raw),
                            str(ROOT / "extracted/metadata/stock-overlay-0.dtbo"),
                            str(ufdt_merged)], check=True)
            touch = "/i2c0@11007000/axs15260@3b"
            group = "/pinctrl@1000b000/s17-axs-eint"
            for tree in [raw, merged, ufdt_merged]:
                reference = subprocess.check_output(["fdtget", "-tx", str(tree), touch, "pinctrl-0"], text=True).strip()
                phandle = subprocess.check_output(["fdtget", "-tx", str(tree), group, "phandle"], text=True).strip()
                require(reference == phandle, "Touch pinctrl reference broken by overlay")
    report = {
        "image": str(image_path), "size": len(image), "sha256": sha(image),
        "kernel_sha256": sha(kernel), "ramdisk_sha256": sha(ramdisk),
        "dtb_table_sha256": sha(dtb_table), "kernel_identity": identity,
        "cmdline": cmdline.decode(), "config_sha256": sha(config.encode()),
        "checks": "PASS: source kernel/DTB, verified ramdisk/addresses, gzip, AXS/GPU config, AVB and stock-overlay survival",
        "ramdisk_mode": "collector wrapper with original init preserved" if args.collector_wrapper else "stock",
        "device_boot": "NOT TESTED",
        "android_ufdt_pinctrl_check": "PASS" if args.ufdt_tool else "NOT RUN",
    }
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps(report, indent=2))


if __name__ == "__main__":
    main()
