#!/usr/bin/env python
# Copyright 2017-2021 XMOS LIMITED.
# This Software is subject to the terms of the XMOS Public Licence: Version 1.
from builtins import str
import librosa as lr
import argparse

def get_def_val(filename:str) -> str:
    return "_" + filename.upper().replace(".", "_")

def make_hann(l=512, outfile=None):
    if outfile is None:
        outfile = f"hann_{l}.h"
    def_val = get_def_val(outfile)
    V = lr.filters.get_window('hann', l, fftbins=True) * (2**(32-1) - 1)
    with open(outfile, "w") as f:
        f.write(f"#ifndef {def_val}\n")
        f.write(f"#define {def_val}\n")
        f.write("#include <xmath/xmath.h>\n\n")
        f.write(f"static const q1_31 hanning_{l}[{l}] = {'{'}\n")
        for v in V:
            f.write(f"{int(v)},\n")
        f.write("};\n\n")
        f.write(f"#endif // {def_val}\n")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Script for generating Hann windows")
    parser.add_argument("-l", "--length", action="store", type=int, default=512)
    parser.add_argument("-o", "--output_file", action="store", default=None)

    args = parser.parse_args()
    make_hann(args.length, args.output_file)