#!/usr/bin/env python3
"""Verify the built recovery structure against the original; no device writes."""
import gzip
import hashlib
import json
import shutil
from pathlib import Path
import subprocess
import struct
import sys

root = Path(__file__).resolve().parents[1]
lineage = Path(sys.argv[1] if len(sys.argv) > 1 else '/home/dengxh/android3/lineage')
image = lineage / 'out/target/product/S17Promini/recovery.img'
out = root / 'extracted/built-recovery'
out.mkdir(parents=True, exist_ok=True)
header = subprocess.check_output([
    'python3', str(lineage / 'system/tools/mkbootimg/unpack_bootimg.py'),
    '--boot_img', str(image), '--out', str(out)], text=True)
(root / 'extracted/metadata/built-recovery-header.txt').write_text(header)
for component in ['kernel', 'dtb', 'recovery_dtbo']:
    assert (out / component).read_bytes() == (root / 'extracted/recovery' / component).read_bytes(), component
for value in ['kernel load address: 0x40080000', 'ramdisk load address: 0x51b00000',
              'kernel tags load address: 0x47880000', 'page size: 2048',
              'boot image header version: 2', 'dtb address: 0x0000000047880000']:
    assert value in header, value
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
              checks='PASS: stock components/addresses, ramdisk, ADB config, super contexts, private narrow-screen text, executables and direct ELF dependencies',
              device_boot='NOT TESTED: no connected device')
(root / 'extracted/metadata/recovery-validation.json').write_text(json.dumps(report, indent=2)+'\n')
print(json.dumps(report, indent=2))
