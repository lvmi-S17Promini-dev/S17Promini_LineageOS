#!/usr/bin/env python3
"""Read stock GPT/LP metadata and inventory backups without mounting or root."""
import hashlib
import json
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / 'extracted/metadata'
OUT.mkdir(parents=True, exist_ok=True)


def save(name, value):
    (OUT / name).write_text(json.dumps(value, indent=2, ensure_ascii=False) + '\n')


gpt = (ROOT / 'stock_imgs/gpt.bin').read_bytes()
header = gpt.index(b'EFI PART')
entry_lba, count, size = struct.unpack_from('<QII', gpt, header + 72)
entries = []
for i in range(count):
    entry = gpt[entry_lba * 512 + i * size:entry_lba * 512 + (i + 1) * size]
    if len(entry) < 128 or entry[:16] == bytes(16):
        continue
    start, end = struct.unpack_from('<QQ', entry, 32)
    entries.append(dict(name=entry[56:128].decode('utf-16-le').rstrip('\0'),
                        start_sector=start, end_sector=end, size=(end-start+1)*512))
save('gpt.json', entries)

with (ROOT / 'stock_imgs/super.bin').open('rb') as f:
    f.seek(4096)
    geometry = f.read(52)
    assert struct.unpack_from('<I', geometry)[0] == 0x616c4467
    maximum, slots, block = struct.unpack_from('<III', geometry, 40)
    f.seek(12288)
    data = f.read(maximum)
magic, major, minor, header_size = struct.unpack_from('<IHHI', data)
assert magic == 0x414c5030
tables = data[header_size:]
result = dict(version=f'{major}.{minor}', geometry=dict(
    metadata_max_size=maximum, metadata_slot_count=slots, logical_block_size=block))
formats = [
    ('partitions', '<36sIIII', ['name', 'attributes', 'first_extent', 'num_extents', 'group_index']),
    ('extents', '<QIQI', ['num_sectors', 'target_type', 'target_data', 'target_source']),
    ('groups', '<36sIQ', ['name', 'flags', 'maximum_size']),
    ('block_devices', '<QIIQ36sI', ['first_logical_sector', 'alignment', 'alignment_offset', 'size', 'name', 'flags']),
]
for index, (kind, fmt, keys) in enumerate(formats):
    offset, count, size = struct.unpack_from('<III', data, 80 + index * 12)
    result[kind] = []
    for i in range(count):
        values = struct.unpack_from(fmt, tables, offset + i * size)
        values = [v.split(b'\0')[0].decode() if isinstance(v, bytes) else v for v in values]
        result[kind].append(dict(zip(keys, values)))
for partition in result['partitions']:
    first = partition['first_extent']
    partition['size'] = sum(e['num_sectors']*512 for e in result['extents'][first:first+partition['num_extents']])
save('lp.json', result)

categories = {
    'boot': ['boot', 'recovery', 'dtbo', 'vendor_boot'],
    'android': ['super', 'cache', 'metadata', 'md_udc', 'vbmeta', 'vbmeta_system', 'vbmeta_vendor'],
    'firmware': ['lk', 'lk2', 'tee1', 'tee2', 'gz1', 'gz2', 'sspm_1', 'sspm_2', 'spmfw', 'md1img', 'logo'],
    'device-unique': ['nvram', 'nvdata', 'nvcfg', 'protect1', 'protect2', 'proinfo', 'otp', 'frp', 'seccfg', 'sec1'],
    'layout-and-state': ['gpt', 'gpt_backup', 'para', 'boot_para', 'expdb', 'flashinfo'],
}
inventory = []
for image in sorted((ROOT / 'stock_imgs').glob('*.bin')):
    digest = hashlib.sha256()
    zero = True
    with image.open('rb') as f:
        while chunk := f.read(4*1024*1024):
            digest.update(chunk)
            if zero and chunk.count(0) != len(chunk):
                zero = False
    category = next((k for k, names in categories.items() if image.stem in names), 'other')
    inventory.append(dict(file=image.name, size=image.stat().st_size,
                          sha256=digest.hexdigest(), all_zero=zero, category=category))
    directory = ROOT / 'classified' / category
    directory.mkdir(parents=True, exist_ok=True)
    link = directory / (image.stem + '.img')
    if not link.exists():
        link.symlink_to(Path('../../stock_imgs') / image.name)
save('stock-inventory.json', inventory)
print(f'Inventoried {len(inventory)} images; GPT and LP saved in {OUT}')
