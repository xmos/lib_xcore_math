#!/usr/bin/env python3
# Copyright 2026 XMOS LIMITED.
# This Software is subject to the terms of the XMOS Public Licence: Version 1.
"""
replace_addi_sub.py

Walks lib_xcore_math/src/arch/vx4b and replaces patterns of the form:

    addi X, Y, -(Z)

with:

    sub X, Y, Z

Works on .S files, creates a .bak backup of each modified file and
prints a summary of changes.
"""
import re
from pathlib import Path
import argparse


PATTERN = re.compile(r"\baddi\s+([\w.]+)\s*,\s*([\w.]+)\s*,\s*-\(\s*([\w.]+)\s*\)")


def process_file(path: Path) -> int:
    text = path.read_text()
    new_text, count = PATTERN.subn(r"sub \1, \2, \3", text)
    if count:
        bak = path.with_suffix(path.suffix + '.bak')
        bak.write_text(text)
        path.write_text(new_text)
    return count


def main():
    p = argparse.ArgumentParser(description="Replace addi ..., -(...) with sub ... in vx4b .S files")
    p.add_argument("root", nargs="?", default="lib_xcore_math/src/arch/vx4b", help="root directory to search")
    args = p.parse_args()

    root = Path(args.root)
    if not root.exists():
        print(f"Root path does not exist: {root}")
        raise SystemExit(1)

    total_files = 0
    total_replacements = 0
    modified_files = []

    for f in root.rglob('*.S'):
        total_files += 1
        try:
            cnt = process_file(f)
        except Exception as e:
            print(f"Error processing {f}: {e}")
            continue
        if cnt:
            modified_files.append((f, cnt))
            total_replacements += cnt

    print(f"Scanned {total_files} .S files under {root}")
    if modified_files:
        for f, c in modified_files:
            print(f"Modified: {f} - replacements: {c}")
    else:
        print("No replacements made.")
    print(f"Total replacements: {total_replacements}")


if __name__ == '__main__':
    main()
