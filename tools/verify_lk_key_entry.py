#!/usr/bin/env python3
"""ARM/Thumb execution checks with mocked peripherals; no device access.

Runs the complete stock/patched boot_mode_select machine code. The stock key
detector also executes; only its configuration initializer, PMIC reads and
other platform services are mocked. This cannot test preloader acceptance.
"""
import argparse
import json
from pathlib import Path
import struct

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB
from unicorn import Uc, UC_ARCH_ARM, UC_MODE_THUMB, UC_HOOK_CODE
from unicorn.arm_const import (
    UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2, UC_ARM_REG_R3,
    UC_ARM_REG_R4, UC_ARM_REG_R5, UC_ARM_REG_R6, UC_ARM_REG_R7,
    UC_ARM_REG_R8, UC_ARM_REG_R9, UC_ARM_REG_R10, UC_ARM_REG_R11,
    UC_ARM_REG_SP, UC_ARM_REG_LR, UC_ARM_REG_PC,
)

ROOT = Path(__file__).resolve().parents[1]
BASE = 0x48000000
ENTRY, STOP = BASE + 0x37fc, BASE + 0x1ff000
STACK, BOOTARG, KEYCONFIG = 0x50020000, 0x50000000, 0x50010000
REGS = (UC_ARM_REG_R4, UC_ARM_REG_R5, UC_ARM_REG_R6, UC_ARM_REG_R7,
        UC_ARM_REG_R8, UC_ARM_REG_R9, UC_ARM_REG_R10, UC_ARM_REG_R11)


def word(data, offset):
    return struct.unpack_from('<I', data, offset)[0]


def run(image, case):
    uc = Uc(UC_ARCH_ARM, UC_MODE_THUMB)
    uc.mem_map(BASE, 0x200000)
    uc.mem_write(BASE, image[512:512 + word(image, 4)])
    uc.mem_map(0x50000000, 0x40000)
    uc.mem_map(0x10010000, 0x1000)
    uc.mem_map(0x10007000, 0x1000)
    uc.mem_write(0x10010004, bytes.fromhex('ffff0000ffff0000ffff0000ffff0000ffff0000'))
    read32 = lambda address: word(uc.mem_read(address, 4), 0)
    write32 = lambda address, value: uc.mem_write(address, struct.pack('<I', value))
    # Resolve original symbols from the stock image, independent of whether
    # the candidate replaces the original function's literal pool.
    original = (ROOT / 'stock_imgs/lk.bin').read_bytes()[512:]
    got = BASE + 0x3806 + word(original, 0x3958)
    mode_ptr = read32(got + word(original, 0x3968))
    bootarg_ptr = read32(got + word(original, 0x396c))
    keyconfig_var = BASE + 0x4afe + read32(BASE + 0x4b68)
    write32(bootarg_ptr, BOOTARG)
    write32(keyconfig_var, KEYCONFIG)
    write32(KEYCONFIG + 4, 8)  # Stock overlay: PMIC power key
    write32(KEYCONFIG + 12, 17)  # Stock overlay: PMIC reset/home key
    write32(KEYCONFIG + 0x138, 1)  # Stock overlay recovery key
    write32(KEYCONFIG + 0x13c, 0)  # Stock overlay factory key
    write32(mode_ptr, case.get('initial_mode', 0))
    if case.get('factory_forbidden'):
        write32(BOOTARG + 0x58ec, 0x4c4c4c4c)
        write32(BOOTARG + 0x58f0, 1)
    preserved = {reg: 0x12340000 + index for index, reg in enumerate(REGS)}
    for reg, value in preserved.items():
        uc.reg_write(reg, value)
    uc.reg_write(UC_ARM_REG_SP, STACK)
    uc.reg_write(UC_ARM_REG_LR, STOP | 1)
    calls = []
    env_writes = []
    clock_start = case.get('clock_start_ms', 1000)
    clock_ms = [clock_start]
    timer_reads = [0]
    stopped = [False]

    def return_value(value):
        # Caller-saved registers may be destroyed by a platform function.
        for reg in (UC_ARM_REG_R1, UC_ARM_REG_R2, UC_ARM_REG_R3):
            uc.reg_write(reg, 0xabcdef00)
        uc.reg_write(UC_ARM_REG_R0, value)
        uc.reg_write(UC_ARM_REG_PC, uc.reg_read(UC_ARM_REG_LR))

    def hook(_, address, size, __):
        if address == STOP:
            stopped[0] = True
            uc.emu_stop()
            return
        key_codes = case.get('key_codes', [17] if case.get('key') else [])
        elapsed = clock_ms[0] - clock_start
        active = (elapsed >= case.get('key_after_ms', 0)
                  and elapsed < case.get('key_release_ms', float('inf')))
        mocks = {
            BASE + 0x2174c: ('kedump', int(case.get('dump', False))),
            BASE + 0x1e288: ('mrdump_detection', int(case.get('dump_detected', False))),
            BASE + 0x14fa0: ('backlight_on', 0),
            BASE + 0x1e30c: ('mrdump_run', 0),
            BASE + 0x44670: ('arch_reset', 0),
            BASE + 0x17cfc: ('meta', int(case.get('meta', False))),
            BASE + 0x3efac: ('getenv', BOOTARG + 0x200 if 'env_value' in case else 0),
            BASE + 0x3f1c4: ('setenv', int(case.get('env_set_failed', False))),
            BASE + 0x831c: ('rtc_fastboot', int(case.get('rtc_fastboot', False))),
            BASE + 0x443ec: ('rgu_fastboot', int(case.get('rgu_fastboot', False))),
            BASE + 0x82d8: ('clear_rtc_fastboot', 0),
            BASE + 0x4442c: ('clear_rgu_fastboot', 0),
            BASE + 0x4b78: ('just_reset', 0),
            BASE + 0x8340: ('rtc_recovery', int(case.get('rtc_recovery', False))),
            BASE + 0x44224: ('rgu_mode', 2 if case.get('rgu_recovery') else 0),
            BASE + 0x17e38: ('misc_recovery', int(case.get('misc_recovery', False))),
            BASE + 0x27ff8: ('log', 0),
            BASE + 0x493c: ('key_config_init', 0),
            BASE + 0x788c: ('pmic_home_key', int(active and 17 in key_codes)),
            BASE + 0x7848: ('pmic_power_key', 1),
            BASE + 0x152a0: ('charging', int(case.get('charging', False))),
        }
        if address == BASE + 0x4aec:
            calls.append(['detect_key', uc.reg_read(UC_ARM_REG_R0)])
            for bank in range(5):
                value = 0xffff
                if active:
                    for key in key_codes:
                        if key not in (8, 17) and key // 16 == bank:
                            value &= ~(1 << (key % 16))
                uc.mem_write(0x10010004 + 4 * bank, struct.pack('<H', value))
        if address == BASE + 0x44280:
            # Execute the real watchdog restart instructions and MMIO store.
            calls.append(['wdt_restart', clock_ms[0] - clock_start])
        if address == BASE + 0x43da0:
            delay = uc.reg_read(UC_ARM_REG_R0)
            calls.append(['mdelay', delay])
            clock_ms[0] += delay
            return_value(0)
            return
        if address == BASE + 0x43cfc:
            calls.append(['timer', uc.reg_read(UC_ARM_REG_R0)])
            initial = uc.reg_read(UC_ARM_REG_R0)
            timer_reads[0] += 1
            if not case.get('timer_stuck'):
                clock_ms[0] += 1
            # Unsigned subtraction models the original timer wrap behavior.
            now = clock_start if case.get('timer_stuck') else clock_ms[0]
            return_value((now - initial) & 0xffffffff)
            return
        if address not in mocks:
            return
        name, value = mocks[address]
        calls.append([name, uc.reg_read(UC_ARM_REG_R0)])
        if name == 'setenv':
            key = bytes(uc.mem_read(uc.reg_read(UC_ARM_REG_R0), 64)).split(b'\0')[0].decode()
            val = bytes(uc.mem_read(uc.reg_read(UC_ARM_REG_R1), 64)).split(b'\0')[0].decode()
            env_writes.append([key, val])
        if name == 'meta' and value:
            write32(mode_ptr, 1)
        if name == 'getenv' and value:
            uc.mem_write(value, case['env_value'].encode() + b'\0')
        if name in ('backlight_on', 'mrdump_run') and case.get('dump_fastboot'):
            write32(mode_ptr, 99)
        if name == 'misc_recovery' and value:
            write32(mode_ptr, 2)
        if name == 'charging' and value:
            write32(mode_ptr, 8)
        return_value(value)

    uc.hook_add(UC_HOOK_CODE, hook)
    uc.emu_start(ENTRY | 1, STOP + 4, count=250000)
    if not stopped[0]:
        raise AssertionError('Function did not return: ' + str(case))
    if uc.reg_read(UC_ARM_REG_SP) != STACK:
        raise AssertionError('Stack was not restored')
    for reg, value in preserved.items():
        if uc.reg_read(reg) != value:
            raise AssertionError('Callee-saved register corrupted')
    return {'mode': read32(mode_ptr), 'calls': calls,
            'env_writes': env_writes,
            'elapsed_ms': clock_ms[0] - clock_start, 'timer_reads': timer_reads[0],
            'watchdog_restart_register': read32(0x10007008),
            'stack_and_saved_registers_ok': True}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--image', type=Path,
                        default=ROOT / 'artifacts/lk-volup-fastboot-r1.img')
    parser.add_argument('--report', type=Path,
                        default=ROOT / 'extracted/metadata/lk-key-entry-emulation.json')
    args = parser.parse_args()
    stock = (ROOT / 'stock_imgs/lk.bin').read_bytes()
    patched = args.image.read_bytes()
    cases = [
        ('normal_no_key', {}, 0, 0),
        ('volume_up', {'key': True}, 0, 99),
        ('misc_recovery_no_key', {'misc_recovery': True}, 2, 2),
        ('misc_recovery_volume_up', {'misc_recovery': True, 'key': True}, 2, 99),
        ('rtc_recovery_no_key', {'rtc_recovery': True}, 2, 2),
        ('rtc_recovery_volume_up', {'rtc_recovery': True, 'key': True}, 2, 99),
        ('rgu_recovery_no_key', {'rgu_recovery': True}, 2, 2),
        ('rgu_recovery_volume_up', {'rgu_recovery': True, 'key': True}, 2, 99),
        ('charging_no_key', {'charging': True}, 8, 8),
        ('charging_volume_up', {'charging': True, 'key': True}, 8, 99),
        ('factory_forbidden_key', {'factory_forbidden': True, 'key': True}, 0, 99),
        ('rtc_fastboot', {'rtc_fastboot': True}, 99, 99),
        ('rgu_fastboot', {'rgu_fastboot': True}, 99, 99),
        ('initial_fastboot', {'initial_mode': 99}, 99, 99),
        ('meta_no_key', {'meta': True}, 1, 1),
        ('meta_with_key', {'meta': True, 'key': True}, 1, 1),
    ]
    results = []
    for name, case, expected_stock, expected_patched in cases:
        old, new = run(stock, case), run(patched, case)
        if (old['mode'], new['mode']) != (expected_stock, expected_patched):
            raise AssertionError((name, old, new))
        if expected_patched == 99 and expected_stock != 99:
            names = [c[0] for c in new['calls']]
            if any(n in names for n in ('misc_recovery', 'rtc_recovery', 'rgu_mode', 'charging')):
                raise AssertionError('Key override happened too late: ' + name)
            if ['detect_key', 17] not in new['calls']:
                raise AssertionError('Wrong hardware key code')
        if not case.get('key'):
            platform_names = lambda result: [c[0] for c in result['calls'] if c[0] not in
                ('log', 'timer', 'detect_key', 'key_config_init', 'pmic_home_key')]
            if platform_names(old) != platform_names(new):
                raise AssertionError('No-key platform flow changed: ' + name)
        results.append({'name': name, 'stock': old, 'patched': new, 'passed': True})

    md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)
    disassembly = []
    for start, end in ((0x3a6a, 0x3a6e), (0x3af0, 0x3b08)):
        for instruction in md.disasm(patched[start:end], BASE + start - 512):
            disassembly.append(f'{instruction.address:08x} {instruction.bytes.hex():12s} '
                               f'{instruction.mnemonic} {instruction.op_str}')
    # Ensure the reused block has no unexpected direct branch from elsewhere.
    incoming = []
    code = stock[512:512 + 0x58000]
    for offset in range(0, len(code) - 4, 2):
        # Thumb branch prefixes only; avoid interpreting each half of every
        # instruction as a possible function entry.
        half = word(code + b'\0\0', offset) & 0xffff
        if (half & 0xf800) not in (0xf000, 0xe000) and (half & 0xf000) != 0xd000:
            continue
        instruction = next(md.disasm(code[offset:offset + 4], BASE + offset), None)
        if not instruction or not instruction.mnemonic.startswith(('b', 'cb')):
            continue
        target_text = instruction.op_str.split(',')[-1].strip()
        if not target_text.startswith('#0x'):
            continue
        target = int(target_text[1:], 16)
        source = instruction.address
        if BASE + 0x38f0 <= target < BASE + 0x3908 and not BASE + 0x38f0 <= source < BASE + 0x3908:
            incoming.append({'source': hex(source), 'target': hex(target)})
    if incoming:
        raise AssertionError('Unexpected direct entry into reused block: ' + str(incoming))
    report = {'passed': True, 'scenario_count': len(results),
              'peripherals_mocked': True, 'hardware_tested': False,
              'disassembly': disassembly, 'external_direct_entries_into_slot': incoming,
              'scenarios': results}
    target = args.report
    target.parent.mkdir(parents=True, exist_ok=True)
    target.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps({'passed': True, 'scenario_count': len(results),
                      'report': str(target), 'disassembly': disassembly}, indent=2))


if __name__ == '__main__':
    main()
