#!/usr/bin/env python3
"""Audit the S17 IMX278 port against its verified stock Image and Ghidra output.

No device access. Offsets deliberately apply only to this stock kernel.
Use --symbols-only before running S17CameraDecompile.java to generate bounds.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import struct
import subprocess
import zlib

from reverse_axs15260 import BASE, ROOT, STOCK_HASH


def symbols(image):
    count = struct.unpack_from('<Q', image, 0x11A9B00)[0]
    assert count == 59103
    indices = struct.unpack_from('<256H', image, 0x1264D00)
    tokens = [image[0x1264900 + i:image.index(b'\0', 0x1264900 + i)].decode()
              for i in indices]
    position = 0x11A9C00
    result = []
    for index in range(count):
        size = image[position]
        name = ''.join(tokens[token] for token in image[position + 1:position + 1 + size])
        position += size + 1
        address = struct.unpack_from('<Q', image, 0x1136400 + index * 8)[0]
        result.append((address - BASE, name))
    assert (0x877010, 'TIMX278_MIPI_RAW_SensorInit') in result
    return result


def ghidra_writes(text):
    """Replay constant stack-byte assignments, including shared block tails."""
    values = {}
    result = []
    pattern = (r'(uStack_[0-9a-f]+) = (0x[0-9a-f]+|[0-9]+);'
               r'|camera_iWriteRegI2C\(&(uStack_[0-9a-f]+),3,')
    for match in re.finditer(pattern, text):
        if match[1]:
            values[match[1]] = int(match[2], 0)
        else:
            name = match[3]
            encoded = values[name]
            data_name = 'uStack_' + format(int(name[7:], 16) - 2, 'x')
            result.append([((encoded & 255) << 8) | (encoded >> 8), values[data_name]])
    return result


def source_body(source, name):
    match = re.search(r'static [^;{}]*\b' + name + r'\([^)]*\)\s*\{', source)
    if not match:
        raise ValueError('Missing source function ' + name)
    start = match.end()
    depth, end = 1, start
    while depth:
        depth += (source[end] == '{') - (source[end] == '}')
        end += 1
    return source[start:end - 1]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--kernel-source', type=Path,
                        default=Path('/home/dengxh/android3/lineage/kernel/lvmi/mt6765'))
    parser.add_argument('--symbols-only', action='store_true')
    args = parser.parse_args()
    compressed = (ROOT / 'extracted/boot/kernel').read_bytes()
    assert hashlib.sha256(compressed).hexdigest() == STOCK_HASH
    image = zlib.decompress(compressed, 31)
    metadata = ROOT / 'extracted/metadata'
    all_symbols = symbols(image)
    selected = []
    for (offset, name), (end, _) in zip(all_symbols, all_symbols[1:]):
        if 0x877010 <= offset < 0x87C5A4 or name[1:] in (
                'iReadRegI2C', 'iWriteRegI2C', 'kdSetI2CSpeed'):
            selected.append(dict(offset=offset, end=end, name=name[1:], type=name[0]))
    (metadata / 'stock-camera-symbols.json').write_text(json.dumps(selected, indent=2) + '\n')
    (metadata / 'stock-camera-symbols.txt').write_text(''.join(
        f'{e["offset"]:08x} {e["type"]} {e["name"]}\n' for e in selected))
    (metadata / 'stock-camera-bounds.tsv').write_text(''.join(
        f'{e["offset"]:x}\t{e["end"]:x}\t' +
        ('imx278_' if e['offset'] >= 0x877010 else 'camera_') + e['name'] + '\n'
        for e in selected))
    if args.symbols_only:
        return
    decompiled = metadata / 'ghidra-camera'
    control = (decompiled / 'imx278_control.c').read_text()
    hs = control.split('  case 3:')[1].split('  case 4:')[0]
    slim = control.split('  case 4:')[1].split('  default:')[0]
    tail = slim.split('code_r0xffffff80088f9a28:')[1]
    tables = {
        'sensor_init': ghidra_writes((decompiled / 'imx278_sensor_init.c').read_text()),
        'preview_setting': ghidra_writes((decompiled / 'imx278_preview_setting.c').read_text()),
        'capture_setting': ghidra_writes(control.split('  case 1:')[1].split('  case 2:')[0]),
        'hs_video_setting': ghidra_writes(hs + tail),
        'slim_video_setting': ghidra_writes(slim),
    }
    assert [len(t) for t in tables.values()] == [249, 72, 72, 72, 72]
    driver = args.kernel_source / ('drivers/misc/mediatek/imgsensor/src/common/v1/'
                                  'imx278_mipi_raw/imx278mipiraw_Sensor.c')
    original = driver.read_text()
    stripped = re.sub(r'^\s*#include.*$', '', original, flags=re.M)
    source = subprocess.run(['cpp', '-P', '-'], input=stripped, text=True,
                            capture_output=True, check=True).stdout
    for name, expected in tables.items():
        body = source_body(source, name)
        actual = [[int(a, 0), int(v, 0)] for a, v in re.findall(
            r'write_cmos_sensor\((0x[0-9a-fA-F]+),\s*(0x[0-9a-fA-F]+|\d+)\)', body)]
        assert actual == expected, name + ' differs from stock'
    assert 'preview_setting();' in source_body(source, 'normal_video_setting')
    crop = [list(struct.unpack_from('<16H', image, 0x10A4F50 + i * 32)) for i in range(5)]
    gain = [list(struct.unpack_from('<2H', image, 0x10A5090 + i * 4)) for i in range(104)]
    source_gain = re.split(r'sensorGainMapping\[[^\]]+\]\[2\]\s*=', source)[1].split('};')[0]
    assert [[int(a), int(v)] for a, v in re.findall(r'\{\s*(\d+)\s*,\s*(\d+)\s*\}',
                                                  source_gain)] == gain
    source_crop = source.split('imgsensor_winsize_info[5] =')[1].split('};')[0]
    assert [[int(n) for n in row.split(',') if n.strip()] for row in
            re.findall(r'\{([^{}]+)\}', source_crop)] == crop
    power = {}
    for name, offset in [('gc5025sub_mipi_raw', 0x1785B88), ('imx278_mipi_raw', 0x1785C88)]:
        pointer = struct.unpack_from('<Q', image, offset)[0] - BASE
        assert image[pointer:image.index(b'\0', pointer)].decode() == name
        power[name] = [list(struct.unpack_from('<5I', image, offset + 8 + i * 20))
                       for i in range(9)]
    report = dict(stock_kernel_sha256=STOCK_HASH,
                  driver_sha256=hashlib.sha256(driver.read_bytes()).hexdigest(),
                  tables=tables, crop=crop, gain=gain, power=power,
                  i2c_addresses=list(image[0x10A4F47:0x10A4F4B]),
                  raw_format='RAW_B', mirror_register=[0x0101, 3],
                  validation='Register tables, crop and gain match stock; device test pending.')
    (metadata / 'stock-imx278-audit.json').write_text(json.dumps(report, indent=2) + '\n')
    print('Stock match: init 249, modes 72 each, crop 5, gain 104; device test pending')


if __name__ == '__main__':
    main()
