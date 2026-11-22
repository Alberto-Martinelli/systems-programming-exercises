#!/usr/bin/env python3
"""Generate portable demonstration inputs; no third-party dependencies."""
from pathlib import Path
import struct
import sys

if len(sys.argv) != 2:
    raise SystemExit("Usage: python3 examples/make_sort_inputs.py OUTPUT_DIRECTORY")
directory = Path(sys.argv[1])
directory.mkdir(parents=True, exist_ok=True)
for index, values in enumerate(([23, 45, 67, 9, 23], [12, 0, -5, 90], []), 1):
    content = struct.pack("<I", len(values))
    content += b"".join(struct.pack("<i", value) for value in values)
    target = directory / f"input{index}.bin"
    target.write_bytes(content)
    print(target)
