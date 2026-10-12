#!/usr/bin/env python3
"""Extract stock power profiles using the locally built aapt2 (no APK changes)."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import xml.etree.ElementTree as ET
from zipfile import ZipFile


def decode_tree(tree):
    root = None
    stack = []
    for line in tree.splitlines():
        indent = len(line) - len(line.lstrip())
        value = line.strip()
        match = re.fullmatch(r"E: (\w+) \(line=\d+\)", value)
        if match:
            while stack and stack[-1][0] >= indent:
                stack.pop()
            element = ET.Element(match[1])
            if stack:
                stack[-1][1].append(element)
            else:
                assert root is None, "Multiple roots"
                root = element
            stack.append((indent, element))
        elif value.startswith("A:"):
            match = re.fullmatch(r'A: name="([^"]*)" \(Raw: "([^"]*)"\)', value)
            assert match and match[1] == match[2], value
            stack[-1][1].set("name", match[1])
        elif value.startswith("T:"):
            match = re.fullmatch(r"T: '(.*)'", value)
            assert match, value
            while stack and stack[-1][0] >= indent:
                stack.pop()
            element = stack[-1][1]
            # Preserve the stock stray text between elements as XML tail text.
            if len(element):
                element[-1].tail = (element[-1].tail or "") + match[1]
            else:
                element.text = (element.text or "") + match[1]
        elif value:
            raise ValueError(value)
    assert root is not None and root.tag == "device"
    for element in root:
        assert element.tag in ("item", "array") and "name" in element.attrib
        values = [element.text] if element.tag == "item" else [v.text for v in element]
        for value in values:
            float(value)
    ET.indent(root, space="    ")
    return ET.tostring(root, encoding="utf-8", xml_declaration=True) + b"\n"


def main():
    repo = Path(__file__).resolve().parents[1]
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--aapt2", type=Path, default=Path.home() / "android3/lineage/out/host/linux-x86/bin/aapt2")
    parser.add_argument("--output", type=Path, default=repo / "extracted/metadata/stock-power-profiles")
    args = parser.parse_args()
    base = repo / "extracted/filesystems"
    apks = [base / "system/system/framework/framework-res.apk"]
    apks += sorted(base.glob("*/overlay/**/*.apk"))
    args.output.mkdir(parents=True, exist_ok=True)
    records = []
    for apk in apks:
        with ZipFile(apk) as archive:
            resources = [name for name in archive.namelist() if name.endswith("/power_profile.xml")]
            for resource in resources:
                label = apk.relative_to(base).parts[0] + "-" + apk.stem
                dump = subprocess.run([str(args.aapt2), "dump", "xmltree", str(apk), "--file", resource],
                                      check=True, capture_output=True, text=True).stdout
                xml = decode_tree(dump)
                (args.output / (label + ".xml")).write_bytes(xml)
                (args.output / (label + ".aapt2.txt")).write_text(dump)
                records.append({"apk": str(apk.relative_to(repo)), "resource": resource,
                                "apk_sha256": hashlib.sha256(apk.read_bytes()).hexdigest(),
                                "binary_xml_sha256": hashlib.sha256(archive.read(resource)).hexdigest(),
                                "decoded_xml": label + ".xml",
                                "decoded_xml_sha256": hashlib.sha256(xml).hexdigest()})
    (args.output / "sources.json").write_text(json.dumps(records, indent=2) + "\n")
    print(json.dumps(records, indent=2))


if __name__ == "__main__":
    main()
