#!/usr/bin/env python3
"""Extract the S17 stock AXS15260 tables and replay its get_params stores.

Requires capstone. This is deliberately restricted to the verified stock image;
it is not a general ARM emulator. No device access or image modification.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct
import zlib

from capstone import Cs, CS_ARCH_ARM64, CS_MODE_ARM
from capstone.arm64 import ARM64_OP_IMM, ARM64_OP_REG

ROOT = Path(__file__).resolve().parents[1]
STOCK_HASH = "5851e16038244576d0a863d9f36b4ad602af68ea45477f5c6f44a9dfb0127286"
BASE = 0xFFFFFF8008080000
DRIVER = 0x176E8B0
FUNCTIONS = {
    "set_util_funcs": (0x6E77D4, 0x6E7808),
    "get_params": (0x6E7808, 0x6E78C4),
    "init": (0x6E78C4, 0x6E794C),
    "suspend": (0x6E794C, 0x6E79A8),
    "resume": (0x6E79A8, 0x6E79C0),
    "compare_id": (0x6E79C0, 0x6E7AEC),
    "bias_probe": (0x6E7AEC, 0x6E7B40),
    "bias_remove": (0x6E7B40, 0x6E7B80),
    "bias_enable": (0x6E7B80, 0x6E7BC8),
    "push_table": (0x6E7BC8, 0x6E7C4C),
    "bias_write": (0x6E7C4C, 0x6E7CE4),
    "axs_reset_proc": (0x9F8598, 0x9F8668),
}


def read_table(image, offset, count):
    result = []
    for i in range(count):
        pos = offset + i * 72
        cmd = struct.unpack_from("<I", image, pos)[0]
        size = image[pos + 4]
        item = {"offset": hex(pos), "cmd": hex(cmd), "count": size}
        if cmd == 0xFFE:
            item["delay_ms"] = size
        elif cmd == 0xFFF:
            item["end"] = True
        else:
            if size > 64:
                raise ValueError("Payload exceeds table entry")
            item["data"] = image[pos + 5:pos + 5 + size].hex()
        result.append(item)
    if not result[-1].get("end"):
        raise ValueError("Missing end marker")
    return result


def replay_params(disassembler, image):
    """Replay straight-line stores, intercepting only _mcount and memset."""
    params_address = 0x10000000
    regs = {"x0": params_address}
    result = bytearray(0x518)

    def key(reg):
        name = disassembler.reg_name(reg)
        return "x" + name[1:] if name.startswith("w") else name

    def value(operand):
        if operand.type == ARM64_OP_IMM:
            return operand.imm
        if operand.type == ARM64_OP_REG:
            return regs.get(key(operand.reg), 0)
        raise ValueError("Unexpected operand")

    start, end = FUNCTIONS["get_params"]
    for instruction in disassembler.disasm(image[start:end], BASE + start):
        op = instruction.operands
        name = instruction.mnemonic
        if name == "mov":
            regs[key(op[0].reg)] = value(op[1])
        elif name == "movk":
            shift = op[1].shift.value
            dst = key(op[0].reg)
            regs[dst] = (regs.get(dst, 0) & ~(0xFFFF << shift)) | (op[1].imm << shift)
        elif name == "add":
            regs[key(op[0].reg)] = value(op[1]) + value(op[2])
        elif name == "str":
            address = regs.get(key(op[1].mem.base), 0) + op[1].mem.disp
            if params_address <= address < params_address + len(result):
                size = 4 if disassembler.reg_name(op[0].reg).startswith("w") else 8
                pos = address - params_address
                result[pos:pos + size] = value(op[0]).to_bytes(size, "little")
        elif name == "bl":
            target = (op[0].imm & ((1 << 64) - 1)) - BASE
            if target == 0xF0F600:  # memset
                if (regs["x0"], regs["x1"], regs["x2"]) != (params_address, 0, len(result)):
                    raise ValueError("Unexpected memset arguments")
                result[:] = bytes(len(result))
            elif target != 0x1F01C:  # _mcount
                raise ValueError(f"Unexpected call {target:#x}")
        elif name not in {"stp", "ldr", "ldp", "ret"}:
            raise ValueError(f"Unexpected instruction {name}")
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--kernel", type=Path, default=ROOT / "extracted/boot/kernel")
    parser.add_argument("--out", type=Path, default=ROOT / "extracted/kernel-reverse")
    args = parser.parse_args()
    compressed = args.kernel.read_bytes()
    if hashlib.sha256(compressed).hexdigest() != STOCK_HASH:
        raise ValueError("Stock kernel hash mismatch; offsets are image-specific")
    image = zlib.decompress(compressed, 31)
    args.out.mkdir(parents=True, exist_ok=True)
    disassembler = Cs(CS_ARCH_ARM64, CS_MODE_ARM)
    disassembler.detail = True
    driver = struct.unpack_from("<41Q", image, DRIVER)
    name_offset = driver[0] - BASE
    name = image[name_offset:image.index(b"\0", name_offset)].decode()
    if name != "axs15260_dsi_6580_hd":
        raise ValueError("Unexpected driver name")
    for index, name_key in enumerate(list(FUNCTIONS)[:5], 1):
        if driver[index] - BASE != FUNCTIONS[name_key][0]:
            raise ValueError("Unexpected driver callback")
    if driver[10] - BASE != FUNCTIONS["compare_id"][0]:
        raise ValueError("Unexpected compare_id callback")
    params = replay_params(disassembler, image)
    report = {
        "stock_kernel_sha256": STOCK_HASH,
        "image_base": hex(BASE),
        "driver_offset": hex(DRIVER),
        "name": name,
        "callbacks": {name: hex(bounds[0]) for name, bounds in FUNCTIONS.items()},
        "init_table": read_table(image, 0x176E9F8, 8),
        "suspend_table": read_table(image, 0x176EC38, 5),
        "stock_params_size": len(params),
        "params_nonzero_words": {
            hex(i): struct.unpack_from("<I", params, i)[0]
            for i in range(0, len(params), 4)
            if struct.unpack_from("<I", params, i)[0]
        },
        "notes": [
            "Stock init/resume power callbacks are null; power is inside init/suspend.",
            "axs_reset_proc is a touch GPIO reset, not a 20ms delay function.",
            "get_params current header size is shorter; compare its full prefix.",
        ],
    }
    (args.out / "axs15260-stock.json").write_text(json.dumps(report, indent=2) + "\n")
    (args.out / "axs15260-params.bin").write_bytes(params)
    lines = []
    for name, (start, end) in FUNCTIONS.items():
        lines.append(f"\n{name} (Image {start:#x}, VA {BASE + start:#x})")
        lines.extend(
            f"{ins.address - BASE:08x}: {ins.mnemonic:8} {ins.op_str}"
            for ins in disassembler.disasm(image[start:end], BASE + start)
        )
    (args.out / "axs15260-disassembly.txt").write_text("\n".join(lines) + "\n")
    print(f"Recovered {name_offset:#x} driver name, {len(report['init_table'])} init entries")


if __name__ == "__main__":
    main()
