#!/usr/bin/env python3
"""Regression, timing, key-matrix and signature checks for S17 LK r2."""
import argparse
import json
from pathlib import Path
import re
import struct

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB
from cryptography.hazmat.primitives import serialization
from patch_lk_key_entry import (
    ROOT, BASE, STOCK_SHA256, sha, require, CERT1_START, CERT1_SIZE,
    CERT2_START, CERT2_SIZE, DATA_OID, HEADER_OID, certificate_public_key,
    verify_cert, hash_slot, payload_hash,
)
from verify_lk_key_entry import run


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--image', type=Path, default=ROOT / 'artifacts/lk-keys-fastboot-r2.img')
    parser.add_argument('--report', type=Path,
                        default=ROOT / 'extracted/metadata/lk-key-entry-r2-emulation.json')
    args = parser.parse_args()
    stock = (ROOT / 'stock_imgs/lk.bin').read_bytes()
    image = args.image.read_bytes()
    require(sha(stock) == STOCK_SHA256, 'Wrong stock image')
    require(len(image) == len(stock) == 1048576, 'Image size changed')
    cert2 = image[CERT2_START:CERT2_START + CERT2_SIZE]
    image_key = certificate_public_key(cert2)
    verify_cert(cert2, image_key)
    ds, de = hash_slot(cert2, DATA_OID)
    hs, he = hash_slot(cert2, HEADER_OID)
    require(cert2[ds:de] == payload_hash(image), 'Payload hash mismatch')
    require(cert2[hs:he].hex() == sha(image[:512]), 'Header hash mismatch')
    require(image[:512] == stock[:512], 'LK header changed')
    require(image[0xaef90:] == stock[0xaef90:], 'DTB and its certificates changed')
    require(image[CERT1_START:CERT1_START + CERT1_SIZE] ==
            stock[CERT1_START:CERT1_START + CERT1_SIZE], 'Original cert1 changed')
    require(b'Orange State\n' in image and b'S17-LK-r2: boot in 5 seconds\n' in image,
            'Missing visible version marker or original Orange State warning')
    overlay = (ROOT / 'extracted/metadata/stock-overlay-0.dts').read_text()
    values = re.search(r'mediatek,kpd-hw-init-map = <([^>]+)>', overlay).group(1).split()
    keymap = [int(v, 0) for v in values]
    require([(k, keymap[k]) for k in (0, 1, 19)] == [(0, 114), (1, 115), (19, 212)],
            'Stock keypad mapping differs')

    cases = [('normal', {}, 0, 0), ('power_only', {'key_codes': [8]}, 0, 0)]
    for key in (0, 1, 17, 19):
        cases.extend([
            (f'key_{key}_held', {'key_codes': [key]}, 0, 99),
            (f'key_{key}_late_2500ms', {'key_codes': [key], 'key_after_ms': 2500}, 0, 99),
            (f'key_{key}_after_timeout', {'key_codes': [key], 'key_after_ms': 3500}, 0, 0),
        ])
    for label in ('misc_recovery', 'rtc_recovery', 'rgu_recovery'):
        cases.append((label, {label: True}, 2, 2))
        cases.append((label + '_camera_override',
                      {label: True, 'key_codes': [19], 'key_after_ms': 1000}, 2, 99))
    cases.extend([
        ('charging', {'charging': True}, 8, 8),
        ('charging_keypad_up', {'charging': True, 'key_codes': [1]}, 8, 99),
        ('factory_forbidden', {'factory_forbidden': True}, 0, 0),
        ('factory_forbidden_key', {'factory_forbidden': True, 'key_codes': [0]}, 0, 99),
        ('rtc_fastboot', {'rtc_fastboot': True}, 99, 99),
        ('rgu_fastboot', {'rgu_fastboot': True}, 99, 99),
        ('initial_fastboot', {'initial_mode': 99}, 99, 99),
        ('meta', {'meta': True}, 1, 1),
        ('meta_preserved_with_key', {'meta': True, 'key_codes': [19]}, 1, 1),
        ('env_meta', {'env_value': 'meta'}, 1, 1),
        ('env_meta_set_failed', {'env_value': 'meta', 'env_set_failed': True}, 1, 1),
        ('env_meta_preserved_with_key', {'env_value': 'meta', 'key_codes': [0]}, 1, 1),
        ('env_other', {'env_value': 'normal'}, 0, 0),
        ('env_meta_prefix_only', {'env_value': 'metax'}, 0, 0),
        ('dump_reset', {'dump': True}, 0, 0),
        ('dump_run_then_reset', {'dump': True, 'dump_detected': True}, 0, 0),
        ('dump_fastboot', {'dump': True, 'dump_detected': True, 'dump_fastboot': True}, 99, 99),
        ('dump_already_fastboot', {'dump': True, 'initial_mode': 99}, 99, 99),
        ('both_volume_keys', {'key_codes': [0, 1]}, 0, 99),
        ('key_camera_short_press', {'key_codes': [19], 'key_after_ms': 500,
                                    'key_release_ms': 600}, 0, 99),
        ('unrelated_keys', {'key_codes': [3, 20, 21]}, 0, 0),
        ('timer_wrap_no_key', {'clock_start_ms': 0xfffffff0}, 0, 0),
        ('timer_wrap_camera', {'clock_start_ms': 0xfffffff0, 'key_codes': [19],
                               'key_after_ms': 2000}, 0, 99),
    ])
    results = []
    ignored = {'log', 'timer', 'detect_key', 'key_config_init', 'pmic_home_key',
               'wdt_restart', 'mdelay'}
    for name, case, old_expected, new_expected in cases:
        old, new = run(stock, case), run(image, case)
        require((old['mode'], new['mode']) == (old_expected, new_expected),
                f'{name}: unexpected modes {old["mode"]}, {new["mode"]}')
        require(old['env_writes'] == new['env_writes'], name + ': environment write differs')
        if case.get('env_value') == 'meta' and not case.get('meta'):
            require(new['env_writes'] == [['reboot_meta_flag', '0']], 'META flag not cleared correctly')
        names = [c[0] for c in new['calls']]
        if new_expected == 99 and old_expected != 99:
            require(not any(n in names for n in ('misc_recovery', 'rtc_recovery',
                                                 'rgu_mode', 'charging')),
                    name + ': key override came after recovery/charging')
        elif old_expected == new_expected:
            flow = lambda result: [c[0] for c in result['calls'] if c[0] not in ignored]
            require(flow(old) == flow(new), name + ': original platform flow changed')
        poll_count = names.count('wdt_restart')
        if poll_count:
            require(new['watchdog_restart_register'] == 0x1971, 'Watchdog restart not executed')
            require(all(c[1] == 10 for c in new['calls'] if c[0] == 'mdelay'), 'Wrong polling delay')
        require(poll_count <= 300, 'Polling exceeded iteration limit')
        if name in ('normal', 'power_only') or name.endswith('after_timeout'):
            require(3000 <= new['elapsed_ms'] <= 3020, 'No-key timeout outside expected bounds')
        if 'late_2500ms' in name:
            require(2500 <= new['elapsed_ms'] < 2520, 'Late key was missed')
        # Store summaries plus first/last calls, avoiding thousands of repeated
        # peripheral samples while preserving the flow and timing evidence.
        results.append({'name': name, 'passed': True,
                        'stock_mode': old['mode'], 'patched_mode': new['mode'],
                        'elapsed_ms': new['elapsed_ms'], 'poll_iterations': poll_count,
                        'stack_and_saved_registers_ok': new['stack_and_saved_registers_ok'],
                        'platform_flow': [c for c in new['calls'] if c[0] not in ignored],
                        'first_calls': new['calls'][:16], 'last_calls': new['calls'][-12:]})
    stuck = run(image, {'timer_stuck': True})
    require(stuck['mode'] == 0 and stuck['elapsed_ms'] == 3000 and
            sum(c[0] == 'wdt_restart' for c in stuck['calls']) == 300,
            'Timer failure did not terminate by hard cap')
    results.append({'name': 'timer_stuck_bounded', 'passed': True,
                    'elapsed_ms': stuck['elapsed_ms'], 'poll_iterations': 300,
                    'stack_and_saved_registers_ok': stuck['stack_and_saved_registers_ok']})

    md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)
    external_entries = []
    code = stock[512:512 + 0x58000]
    for offset in range(0, len(code) - 4, 2):
        half = struct.unpack_from('<H', code, offset)[0]
        if (half & 0xf800) not in (0xf000, 0xe000) and (half & 0xf000) != 0xd000:
            continue
        ins = next(md.disasm(code[offset:offset + 4], BASE + offset), None)
        if not ins or not ins.mnemonic.startswith(('b', 'cb')):
            continue
        target = ins.op_str.split(',')[-1].strip()
        if not target.startswith('#0x'):
            continue
        target = int(target[1:], 16)
        if BASE + 0x37fc < target < BASE + 0x3994 and not BASE + 0x37fc <= ins.address < BASE + 0x3994:
            external_entries.append({'source': hex(ins.address), 'target': hex(target)})
    require(not external_entries, 'External direct branch into replaced function interior')
    report = {'passed': True, 'revision': 'r2', 'image_sha256': sha(image),
              'scenario_count': len(results), 'hardware_tested': False,
              'peripherals_mocked': True, 'signature_and_hash_verified': True,
              'external_direct_entries_into_function_interior': external_entries,
              'keypad_map': {'volume_down': 0, 'volume_up': 1, 'camera': 19},
              'scenarios': results}
    out = args.report
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps({k: v for k, v in report.items() if k != 'scenarios'}, indent=2))


if __name__ == '__main__':
    main()
