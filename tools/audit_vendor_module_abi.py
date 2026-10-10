#!/usr/bin/env python3
"""Compare vendor module CRC requirements with a built kernel, without loading."""

import argparse
import json
from pathlib import Path
import subprocess


def module_symbols(option, path):
    result = subprocess.run(
        ["modprobe", option, str(path)], capture_output=True, text=True
    )
    if result.returncode:
        # libkmod uses ENODATA for a module with no exported symbols.
        if option == "--show-exports" and "No data available" in result.stderr:
            return {}
        raise RuntimeError(result.stderr.strip())
    return {name: int(crc, 16) for crc, name in
            (line.split() for line in result.stdout.splitlines())}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--symvers", required=True, type=Path)
    parser.add_argument("--modules", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    args = parser.parse_args()
    symbols = {}
    for line in args.symvers.read_text().splitlines():
        crc, name, provider, *_ = line.split()
        symbols[name] = (int(crc, 16), provider)
    paths = sorted(args.modules.glob("*.ko"))
    if not paths:
        parser.error("module directory contains no .ko files")
    exports = {}
    for path in paths:
        for name, crc in module_symbols("--show-exports", path).items():
            exports.setdefault(name, []).append((crc, path.name))
    report = {
        "symvers": str(args.symvers.resolve()),
        "note": "Offline CRC audit only. Matching CRCs do not prove binary "
                "compatibility. Candidate vendor providers may fail to load.",
        "modules": {},
    }
    for path in paths:
        imports = module_symbols("--dump-modversions", path)
        matching, mismatched, missing, dependencies = [], [], [], {}
        for name, crc in imports.items():
            if name in symbols:
                actual, provider = symbols[name]
                if crc == actual:
                    matching.append(name)
                else:
                    mismatched.append({"symbol": name,
                                       "module_crc": f"0x{crc:08x}",
                                       "kernel_crc": f"0x{actual:08x}",
                                       "provider": provider})
            elif name in exports:
                dependencies[name] = [
                    {"module": provider, "crc": f"0x{value:08x}",
                     "matches": value == crc}
                    for value, provider in exports[name]
                ]
            else:
                missing.append(name)
        report["modules"][path.name] = {
            "vermagic": subprocess.check_output(
                ["modinfo", "-F", "vermagic", str(path)], text=True
            ).strip(),
            "imports": len(imports),
            "matching_kernel_symbols": matching,
            "mismatched_kernel_symbols": mismatched,
            "not_exported_by_kernel_or_vendor_modules": missing,
            "candidate_vendor_providers": dependencies,
        }
        print(f"{path.name}: imports={len(imports)} "
              f"match={len(matching)} mismatch={len(mismatched)} "
              f"missing={len(missing)} vendor_imports={len(dependencies)}")
    args.output.write_text(json.dumps(report, indent=2) + "\n")


if __name__ == "__main__":
    main()
