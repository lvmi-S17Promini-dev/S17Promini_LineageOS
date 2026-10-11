#!/usr/bin/env python3
"""Prepare targeted Ghidra bounds and audit the S17 stock PSC5425E callbacks."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import zlib

from reverse_axs15260 import BASE, ROOT, STOCK_HASH
from reverse_imx278 import symbols


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--symbols-only", action="store_true")
    args = parser.parse_args()
    compressed = (ROOT / "extracted/boot/kernel").read_bytes()
    assert hashlib.sha256(compressed).hexdigest() == STOCK_HASH
    image = zlib.decompress(compressed, 31)
    metadata = ROOT / "extracted/metadata"
    selected = []
    all_symbols = symbols(image)
    for (offset, name), (end, _) in zip(all_symbols, all_symbols[1:]):
        if "psc5425" in name.lower():
            selected.append(dict(offset=offset, end=end, name=name[1:], type=name[0]))
    assert any(e["name"] == "psc5425e_driver_probe" for e in selected)
    (metadata / "stock-psc5425e.Image").write_bytes(image)
    (metadata / "stock-psc5425e-symbols.json").write_text(json.dumps(selected, indent=2) + "\n")
    (metadata / "stock-psc5425e-bounds.tsv").write_text("".join(
        f'{e["offset"]:x}\t{e["end"]:x}\t{e["name"]}\n'
        for e in selected if e["type"].lower() == "t"))
    if args.symbols_only:
        return
    decompiled = metadata / "ghidra-psc5425e"
    writes = {}
    for name in ("driver_probe", "enable_charging", "set_current", "set_cv_voltage",
                 "set_input_current", "enable_otg"):
        text = (decompiled / f"psc5425e_{name}.c").read_text()
        writes[name] = [args.strip().split(",") for args in re.findall(
            r"psc5425e_config_interface\(([^()]*)\)", text)]
    assert writes["set_current"] == [["4", "uVar1", "7", "4"]]
    assert writes["set_cv_voltage"] == [["2", "uVar1", "0x3f", "2"]]
    assert writes["set_input_current"] == [["1", "uVar1", "3", "6"]]
    current = (decompiled / "psc5425e_set_current.c").read_text()
    thresholds = [int(x, 16) for x in re.findall(r"param_2 < (0x[0-9a-f]+)", current)]
    assert thresholds == [656001, 786001, 1048001, 1180001, 1442001, 1574001]
    assert "0x1c03e0 < param_2" in current  # 1836000
    report = dict(stock_kernel_sha256=STOCK_HASH, image_base=hex(BASE),
                  callbacks=writes, stock_current_ua=[656000, 786000, 1048000, 1180000,
                                                     1442000, 1574000, 1836000, 1966000],
                  evidence="Ghidra decompiled stock Image; no device access")
    (metadata / "stock-psc5425e-audit.json").write_text(json.dumps(report, indent=2) + "\n")
    print("Stock PSC5425E callback register masks and current thresholds: PASS")


if __name__ == "__main__":
    main()
