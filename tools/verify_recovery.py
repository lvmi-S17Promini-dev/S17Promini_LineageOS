#!/usr/bin/env python3
"""Verify recovery against its source build (or an explicit stock baseline)."""
import argparse
import gzip
import hashlib
import json
import re
import shutil
from pathlib import Path
import subprocess
import struct
import tempfile

root = Path(__file__).resolve().parents[1]
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('lineage', nargs='?', type=Path,
                    default=Path('/home/dengxh/android3/lineage'))
parser.add_argument('--out', type=Path)
parser.add_argument('--image', type=Path)
parser.add_argument('--report', type=Path,
                    default=root / 'extracted/metadata/recovery-validation.json')
parser.add_argument('--kernel-mode', choices=['source', 'stock'], default='source')
parser.add_argument('--expected-release', default='4.19.127-s17-bringup-r11-connectivity')
parser.add_argument('--cpufreq-init', choices=['enabled', 'disabled'], default='enabled')
args = parser.parse_args()
lineage = args.lineage.resolve()
product = (args.out or lineage / 'out') / 'target/product/S17Promini'
image = args.image or product / 'recovery.img'
out = root / 'extracted/built-recovery'
out.mkdir(parents=True, exist_ok=True)
header = subprocess.check_output([
    'python3', str(lineage / 'system/tools/mkbootimg/unpack_bootimg.py'),
    '--boot_img', str(image), '--out', str(out)], text=True)
(root / 'extracted/metadata/built-recovery-header.txt').write_text(header)
device = lineage / 'device/lvmi/S17Promini'
assert (out / 'recovery_dtbo').read_bytes() == (device / 'prebuilt/recovery_dtbo.img').read_bytes()
kernel_identity = None
if args.kernel_mode == 'stock':
    for component in ['kernel', 'dtb']:
        assert (out / component).read_bytes() == (root / 'extracted/recovery' / component).read_bytes(), component
else:
    kernel_out = product / 'obj/KERNEL_OBJ'
    kernel = (out / 'kernel').read_bytes()
    assert kernel == (kernel_out / 'arch/arm64/boot/Image.gz').read_bytes(), 'Recovery kernel differs from source output'
    assert kernel != (root / 'extracted/recovery/kernel').read_bytes(), 'Stock recovery kernel still packaged'
    match = re.search(rb'Linux version [^\0]+', gzip.decompress(kernel))
    assert match is not None, 'Kernel identity missing'
    kernel_identity = match.group().decode(errors='replace').strip()
    assert args.expected_release in kernel_identity, 'Unexpected recovery kernel version'
    table = (out / 'dtb').read_bytes()
    assert table == (product / 'dtb.img').read_bytes(), 'Recovery DTB differs from generated DT table'
    table_header = struct.unpack_from('>8I', table)
    assert table_header[:6] == (0xD7B7AB1E, len(table), 32, 32, 1, 32), 'Invalid source DT table'
    entry = struct.unpack_from('>8I', table, 32)
    assert entry[1] == 64 and entry[0] + 64 == len(table), 'Invalid DT entry bounds'
    board_tree = table[64:]
    assert board_tree == (kernel_out / 'arch/arm64/boot/dts/mediatek/S17Promini.dtb').read_bytes(), 'Wrong board tree'
    config = (kernel_out / '.config').read_text().splitlines()
    for option in ['CONFIG_TOUCHSCREEN_MTK_AXS=y', 'CONFIG_MTK_CPU_FREQ=y']:
        assert option in config, option
    image_bytes = image.read_bytes()
    cmdline = image_bytes[64:576].split(b'\0')[0] + image_bytes[608:1632].split(b'\0')[0]
    blacklist = [name for arg in cmdline.split() if arg.startswith(b'initcall_blacklist=')
                 for name in arg.split(b'=', 1)[1].split(b',')]
    assert (b'_mt_cpufreq_pdrv_init' in blacklist) == (args.cpufreq_init == 'disabled'), 'Unexpected cpufreq init state'
    assert (out / 'ramdisk').read_bytes() == (product / 'ramdisk-recovery.img').read_bytes(), 'Recovery ramdisk was overridden'
    assert (out / 'ramdisk').read_bytes() != (device / 'prebuilt/boot-ramdisk.img').read_bytes(), 'Boot ramdisk used for recovery'
    with tempfile.TemporaryDirectory(prefix='s17-recovery-dtb-') as folder:
        raw = Path(folder) / 'board.dtb'
        merged = Path(folder) / 'merged.dtb'
        raw.write_bytes(board_tree)
        subprocess.run(['fdtoverlay', '-i', str(raw), '-o', str(merged),
                        str(root / 'extracted/metadata/stock-overlay-0.dtbo')], check=True)
        for tree in [raw, merged]:
            reg = subprocess.check_output(['fdtget', '-tx', str(tree),
                                           '/dvfsp@00110c00', 'reg'], text=True).split()
            assert reg == ['0', '110c00', '0', '1400'] * 2, 'Wrong DVFS SRAM resources'
            reg = subprocess.check_output(['fdtget', '-tx', str(tree),
                                           '/i2c0@11007000/axs15260@3b', 'reg'], text=True).strip()
            assert reg == '3b', 'Touch node missing after overlay'
for value in ['kernel load address: 0x40080000', 'ramdisk load address: 0x51b00000',
              'kernel tags load address: 0x47880000', 'page size: 2048',
              'boot image header version: 2', 'dtb address: 0x0000000047880000']:
    assert value in header, value
if args.kernel_mode == 'stock':
    assert 'androidboot.selinux=permissive' not in header
assert image.stat().st_size <= 33554432
ramdisk = out / 'ramdisk-root'
if ramdisk.exists():
    shutil.rmtree(ramdisk)
ramdisk.mkdir(exist_ok=True)
subprocess.run(['cpio', '-idm', '--no-absolute-filenames'],
               input=gzip.decompress((out / 'ramdisk').read_bytes()), cwd=ramdisk, check=True,
               stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
rc = (ramdisk / 'init.recovery.mt6765.rc').read_text()
assert 'setprop ro.adb.secure.recovery 0' in rc
assert 'setprop sys.usb.controller musb-hdrc' in rc
helper_source = lineage / 'device/lvmi/S17Promini/recovery/root/system/bin/s17-super-label.sh'
if helper_source.exists():
    helper = ramdisk / 'system/bin/s17-super-label.sh'
    assert helper.read_bytes() == helper_source.read_bytes()
    assert helper.stat().st_mode & 0o111
    assert 'service s17-super-label /system/bin/sh /system/bin/s17-super-label.sh' in rc
    assert 'exec_start s17-super-label' in rc
props = (ramdisk / 'prop.default').read_text()
assert 'ro.debuggable=1' in props
assert 'ro.minui.pixel_format=BGRA_8888' in props
fstab = (ramdisk / 'system/etc/recovery.fstab').read_text()
assert '/dev/block/by-name/md_udc /metadata' in fstab
assert 'system_ext /system_ext' not in fstab
device_contexts = lineage / 'device/lvmi/S17Promini/sepolicy/vendor/file_contexts'
if any(line.strip().startswith('BOARD_VENDOR_SEPOLICY_DIRS')
       for line in (lineage / 'device/lvmi/S17Promini/BoardConfig.mk').read_text().splitlines()):
    contexts = (ramdisk / 'vendor_file_contexts').read_text()
    for node in ['/dev/block/by-name/super', '/dev/block/mmcblk0p32']:
        assert any(line.split() == [node, 'u:object_r:super_block_device:s0']
                   for line in contexts.splitlines()), node
for resource in ['installing', 'installing_security', 'erasing', 'error', 'no_command', 'cancel_wipe_data', 'factory_data_reset',
                 'try_again', 'wipe_data_confirmation', 'wipe_data_menu_header']:
    name = resource + '_text.png'
    source = lineage / 'device/lvmi/S17Promini/recovery/res/images' / name
    if source.exists():
        packaged = (ramdisk / 'res/images' / name).read_bytes()
        assert packaged == source.read_bytes(), name
        assert struct.unpack('>I', packaged[16:20])[0] <= 374, name
for executable in ['adbd', 'recovery', 'fastbootd', 'sh']:
    binary = ramdisk / 'system/bin' / executable
    assert binary.exists(), executable
    if binary.is_symlink():
        continue
    dependencies = subprocess.check_output(['readelf', '-d', str(binary)], text=True)
    for line in dependencies.splitlines():
        if '(NEEDED)' in line:
            library = line.split('[')[1].split(']')[0]
            assert (ramdisk / 'system/lib64' / library).exists(), (executable, library)
report = dict(image=str(image), size=image.stat().st_size,
              sha256=hashlib.sha256(image.read_bytes()).hexdigest(),
              kernel_mode=args.kernel_mode, kernel_identity=kernel_identity,
              checks='PASS: selected kernel/DTB, load addresses, recovery DTBO, ramdisk, ADB config, super contexts, private narrow-screen text, executables and direct ELF dependencies',
              device_boot='NOT TESTED: no connected device')
args.report.write_text(json.dumps(report, indent=2)+'\n')
print(json.dumps(report, indent=2))
