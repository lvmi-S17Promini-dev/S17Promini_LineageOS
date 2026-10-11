#!/usr/bin/env python3
"""Prepare image-specific stock MIR3DA/proximity bounds for targeted Ghidra."""
import hashlib
import json
import zlib
from capstone import Cs, CS_ARCH_ARM64, CS_MODE_ARM
from reverse_axs15260 import ROOT, STOCK_HASH, BASE
from reverse_imx278 import symbols


def main():
    compressed = (ROOT / 'extracted/boot/kernel').read_bytes()
    if hashlib.sha256(compressed).hexdigest() != STOCK_HASH:
        raise ValueError('Stock kernel hash mismatch')
    image = zlib.decompress(compressed, 31)
    all_symbols = symbols(image)
    selected = []
    extra = {'alsps_driver_add', 'ps_register_control_path',
             'ps_register_data_path', 'ps_data_report', 'alsps_init',
             'acc_init', 'mir3da_init', 'ps_report_interrupt_data',
             'acc_work_func', 'acc_enable_and_batch', 'accenablenodata_store',
             'accbatch_store', 'accactive_store', 'gsensor_probe',
             'acc_register_control_path', 'acc_register_data_path',
             'ps_enable_and_batch', 'psactive_store', 'psbatch_store'}
    for (offset, name), (end, _) in zip(all_symbols, all_symbols[1:]):
        if name[0] in 'tT' and (0x630B58 <= offset < 0x6346D8 or name[1:] in extra):
            selected.append(dict(offset=offset, end=end, name=name[1:], type=name[0]))
    metadata = ROOT / 'extracted/metadata'
    (metadata / 'stock-sensor-symbols.json').write_text(
        json.dumps({'kernel_sha256': STOCK_HASH, 'functions': selected}, indent=2) + '\n')
    (metadata / 'stock-sensor-bounds.tsv').write_text(''.join(
        f'{e["offset"]:x}\t{e["end"]:x}\tsensor_{e["name"]}\n' for e in selected))
    names = {BASE + offset: name[1:] for offset, name in all_symbols}
    disassembler = Cs(CS_ARCH_ARM64, CS_MODE_ARM)
    lines = []
    for entry in selected:
        if entry['name'] not in {'mir3da_get_data', 'tpd_enable_ps', 'ps_enable_nodata'}:
            continue
        lines.append(f'\n{entry["name"]}:\n')
        for instruction in disassembler.disasm(
                image[entry['offset']:entry['end']], BASE + entry['offset']):
            target = (int(instruction.op_str[1:], 16)
                      if instruction.mnemonic == 'bl' else None)
            label = ' ; ' + names[target] if target in names else ''
            lines.append(f'{instruction.address - BASE:08x}: '
                         f'{instruction.mnemonic} {instruction.op_str}{label}\n')
    (metadata / 'stock-sensor-proximity-disassembly.txt').write_text(''.join(lines))
    print(f'Prepared {len(selected)} stock functions; verified SHA256 {STOCK_HASH}')


if __name__ == '__main__':
    main()
