#!/usr/bin/env python3
"""Reproduce the S17 stock PV-table SRAM address from verified ARM64 code."""
import hashlib
from pathlib import Path
import zlib

from capstone import Cs, CS_ARCH_ARM64, CS_MODE_ARM
from reverse_axs15260 import BASE, ROOT, STOCK_HASH


def main():
    compressed = (ROOT / "extracted/boot/kernel").read_bytes()
    if hashlib.sha256(compressed).hexdigest() != STOCK_HASH:
        raise ValueError("Unexpected stock kernel")
    image = zlib.decompress(compressed, 31)
    cs = Cs(CS_ARCH_ARM64, CS_MODE_ARM)
    instructions = list(cs.disasm(image[0x153A32C:0x153A340], BASE + 0x153A32C))
    expected = [
        ("mov", "w0, #0xc10"),
        ("csel", "x2, x10, x9, ne"),
        ("movk", "w0, #0x11, lsl #16"),
        ("mov", "w1, #0x240"),
        ("bl", "#0xffffff80080a681c"),
    ]
    if [(i.mnemonic, i.op_str) for i in instructions] != expected:
        raise ValueError("Stock PV-table mapping instructions changed")
    lines = [
        f"Stock gzip kernel SHA256: {STOCK_HASH}",
        "PV-table mapping: physical 0x110c10, length 0x240",
        "Stock DT SRAM base: 0x110c00; table offset: 0x10",
        "Inherited source mapping: 0x110800 + 0x10 = 0x110810 (delta -0x400)",
        "Fixed mapping: reuse of_iomap(node, 1) + 0x10",
        "", "Stock Image instructions:",
    ]
    for i in cs.disasm(image[0x153A2DC:0x153A3B0], BASE + 0x153A2DC):
        lines.append(f"{i.address - BASE:08x}: {i.mnemonic} {i.op_str}")
    output = ROOT / "extracted/metadata/stock-dvfs-sram-address.txt"
    output.write_text("\n".join(lines) + "\n")
    print("Verified stock PV table at 0x110c10, length 0x240")


if __name__ == "__main__":
    main()
