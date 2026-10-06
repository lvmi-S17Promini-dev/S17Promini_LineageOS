#!/usr/bin/env bash
set -euo pipefail
project_root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
lineage_root=${1:-/home/dengxh/android3/lineage}
cd "$project_root"
mkdir -p extracted/metadata extracted/super extracted/filesystems
for part in boot recovery; do
    mkdir -p "extracted/$part/ramdisk-root"
    python3 "$lineage_root/system/tools/mkbootimg/unpack_bootimg.py" \
        --boot_img "stock_imgs/$part.bin" --out "extracted/$part" \
        > "extracted/metadata/$part-header.txt"
    gzip -dc "extracted/$part/ramdisk" | \
        (cd "extracted/$part/ramdisk-root" && cpio -idm --no-absolute-filenames)
done
"$lineage_root/prebuilts/extract-tools/linux-x86/bin/lpunpack" stock_imgs/super.bin extracted/super
for part in system vendor product; do
    mkdir -p "extracted/filesystems/$part"
    debugfs -R "rdump / extracted/filesystems/$part" "extracted/super/$part.img" \
        > "extracted/metadata/$part-extract.log" 2>&1
done
python3 tools/inspect_stock.py
