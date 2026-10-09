#!/usr/bin/env python3
"""Extract image-specific S17 AXS touch evidence and the embedded firmware.

Requires capstone. Does not access hardware or upload firmware.
"""
import argparse
import hashlib
import json
from pathlib import Path
import zlib

from capstone import Cs, CS_ARCH_ARM64, CS_MODE_ARM
from reverse_axs15260 import BASE, ROOT, STOCK_HASH


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--kernel", type=Path, default=ROOT / "extracted/boot/kernel")
    parser.add_argument("--out", type=Path, default=ROOT / "extracted/kernel-reverse/axs-touch")
    args = parser.parse_args()
    compressed = args.kernel.read_bytes()
    if hashlib.sha256(compressed).hexdigest() != STOCK_HASH:
        raise ValueError("Stock image hash mismatch")
    image = zlib.decompress(compressed, 31)
    firmware = image[0x10D0E89:0x10D0E89 + 0xC410]
    version_offset = 0x5034 + int.from_bytes(firmware[0x16:0x18], "big")
    if version_offset + 2 > len(firmware):
        raise ValueError("Firmware version outside image")
    args.out.mkdir(parents=True, exist_ok=True)
    report = {
        "stock_kernel_sha256": STOCK_HASH,
        "i2c_address_override": {
            "image_instruction": "0x9f88c4", "target_address": "0x3b",
            "stock_dt_address": "0x38",
        },
        "report_length": 32, "max_points": 5, "point_stride": 6,
        "raw_read_function_image": "0x9f9c9c",
        "report_parse_image": "0x9f92a4",
        "firmware_version_read_image": "0x9f84a8",
        "firmware_version_command": "0x0c",
        "firmware_blob": {
            "image_offset": "0x10d0e89", "length": len(firmware),
            "sha256": hashlib.sha256(firmware).hexdigest(),
            "version_offset": hex(version_offset),
            "version_be": firmware[version_offset:version_offset + 2].hex(),
        },
        "automatic_upgrade_work_image": "0x9fc40c",
        "automatic_upgrade_gate_image": "0x9fb644",
        "new_driver_scope": "existing firmware only; no upgrade/flash commands",
    }
    (args.out / "axs-touch-stock.json").write_text(json.dumps(report, indent=2) + "\n")
    (args.out / "axs-stock-firmware.bin").write_bytes(firmware)
    disassembler = Cs(CS_ARCH_ARM64, CS_MODE_ARM)
    lines = []
    for start, end in [(0x9F84A8, 0x9F8668), (0x9F886C, 0x9F926C),
                       (0x9F92A4, 0x9F98BC), (0x9F9B30, 0x9F9DCC),
                       (0x9FB644, 0x9FB968), (0x9FC2EC, 0x9FC580)]:
        lines.append(f"\nImage {start:#x}..{end:#x}")
        lines.extend(
            f"{ins.address - BASE:08x}: {ins.mnemonic:8} {ins.op_str}".rstrip()
            for ins in disassembler.disasm(image[start:end], BASE + start)
        )
    (args.out / "axs-touch-disassembly.txt").write_text("\n".join(lines) + "\n")
    print(f"AXS touch: I2C 0x3b, 32-byte reports; extracted {len(firmware)} firmware bytes, version {report['firmware_blob']['version_be']}")


if __name__ == "__main__":
    main()
