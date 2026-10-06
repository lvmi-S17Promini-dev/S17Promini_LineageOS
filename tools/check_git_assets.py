#!/usr/bin/env python3
"""Reject staged files larger than 50 MiB and accidental embedded repositories."""
import subprocess
import sys

limit = 50 * 1024 * 1024
entries = subprocess.check_output(["git", "ls-files", "--stage", "-z"]).split(b"\0")
paths, objects, failures = [], [], []
for entry in entries:
    if not entry:
        continue
    header, path = entry.split(b"\t", 1)
    mode, oid, stage = header.split()
    name = path.decode("utf-8", errors="surrogateescape")
    if mode == b"160000":
        failures.append("Embedded repository: " + name)
        continue
    paths.append(name)
    objects.append(oid)
result = subprocess.run(["git", "cat-file", "--batch-check=%(objectsize)"],
                        input=b"\n".join(objects) + b"\n", stdout=subprocess.PIPE, check=True)
for name, size in zip(paths, result.stdout.splitlines()):
    if int(size) > limit:
        failures.append(f"Too large ({int(size) / 1024**2:.1f} MiB): {name}")
if failures:
    print("\n".join(failures), file=sys.stderr)
    sys.exit(1)
print(f"Checked {len(paths)} staged files: no file exceeds 50 MiB.")
