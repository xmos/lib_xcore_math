# Copyright 2021-2022 XMOS LIMITED.
# This Software is subject to the terms of the XMOS Public Licence: Version 1.
# This Software is subject to the terms of the XMOS Public Licence: Version 1.
import numpy as np
import argparse
import io
import os

import xmath_script as xms

def main():
  
  parser = argparse.ArgumentParser()

  parser.add_argument("filter_name",
                      type=xms.filter_id,
                      help=
"""Name of the generated filter. 
This name will be used to initialize and invoke the filter from user code.""")

  parser.add_argument("filter_coefficients",
                      type=xms.biquad_sections_file,
                      help=
"""File containing the filter coefficients. 

This is the path to a CSV file which contains the floating-point coefficients for the biquad
sections.  Each line of the CSV file stores the coefficients for one biquad section, with the first
biquad section beginning on the first line.

Each line must contain 5 command-separated coefficients, which are the coefficients b[0], b[1],
b[2], -a[1], -a[2], in that order.
""")

  parser.add_argument("--sections",
                      type=int,
                      default=-1,
                      help=
"""The number of biquad filter sections.

Default behavior is to derive this value from the number of rows found in the filter coefficients
CSV file. If this option is used, this script will verify that the number of sections found in the
file matches the number specified here.
""")

  parser.add_argument("--out-dir",
                      type=str,
                      default=".",
                      help=
"""
(optional) Output directory into which generated files are placed.
""")

  parser.add_argument("--scale", type=float, default=1.0, help=
  """
  Scale factor to be applied to all coefficients.

  In some cases this may be necessary if some filter input sequences result in overflows occurring 
  in the output of some filter sections.
  """)

  args = extra_process_args(parser.parse_args())

  coefs_q30 = convert_filter_coefficients(args)

  header_text = generate_header(args)
  source_text = generate_source(coefs_q30, args)

  dir = os.path.dirname(args.header_fpath)
  if not os.path.exists(dir):
    os.makedirs(dir)

  with open(args.header_fpath, "w+") as header_file:
    header_file.write(header_text.getvalue())

  with open(args.source_fpath, "w+") as source_file:
    source_file.write(source_text.getvalue())

### Process some extra stuff to put in args
def extra_process_args(args):

  if args.sections == -1:
    args.sections = args.filter_coefficients.shape[1]

  if args.sections != args.filter_coefficients.shape[1]:
    raise Exception(f"Loaded section count ({args.filter_coefficients.shape[1]}) doesn't match specified section count ({args.sections}).")

  print(f"Filter section count: {args.sections}")

  # header and source filenames
  args.header_filename = f"{args.filter_name}.h"
  args.source_filename = f"{args.filter_name}.c"
  args.header_fpath = os.path.join(args.out_dir, args.header_filename)
  args.source_fpath = os.path.join(args.out_dir, args.source_filename)

  print(f"Files to be written:")
  print(f"  {args.header_fpath}")
  print(f"  {args.source_fpath}")

  return args


### Convert filter coefficients to Q2.30
def convert_filter_coefficients(args):
  coefs = args.filter_coefficients * args.scale * 2**30
  return np.round(coefs).astype(np.int32)


### Generate C header file code using filter parameters
def generate_header(args):
  filter = args.filter_name

  header_text = io.StringIO()

  header_text.write(
f"""#pragma once

#include "xmath/xmath.h"

// Call to process an input sample and generate an output sample
C_API
int32_t {filter}(int32_t new_sample);
""")

  return header_text

### Generate C source file code using filter parameters
def generate_source(coefs, args):
  filter = args.filter_name

  source_text = io.StringIO()

  N_sections = coefs.shape[1]
  N_blocks = (N_sections+7)//8

  source_text.write(
f"""
#include "{filter}.h"

filter_biquad_s32_t _{filter}[{N_blocks}] = {all_biquad_blocks(coefs)};

int32_t {filter}(int32_t new_sample)
{{
  return filter_biquads_s32(_{filter}, {N_blocks}, new_sample);
}}
""")

  return source_text

###
def all_biquad_blocks(block_coefs: np.ndarray):
  N_sections = block_coefs.shape[1]
  N_blocks = (N_sections+7) // 8

  blocks = [block_coefs[:, 8*k:8*(k+1)] for k in range(N_blocks)]


  blocks_str = ",".join([biquad_block_to_str(x) for x in blocks])
  return f"{{\n {blocks_str}  }}"


###
def biquad_block_to_str(block_coefs: np.ndarray):
  N_sections = block_coefs.shape[1]
  block_coefs = np.concatenate([block_coefs, np.zeros(shape=(5,8-N_sections),dtype=np.int32)], axis=1)

  return f"""{{
    .biquad_count = {N_sections},
    .state = {{{{0}}}},
    .coef = { array_to_string(block_coefs, hex) }
  }}"""
  

###
def array_to_string(arr, val_fmtr=str):
    if len(arr.shape) == 1:
        return f"{{{', '.join([val_fmtr(x) for x in arr])}}}"
    else:
        return "{\n" + ",\n".join([array_to_string(arr[i], val_fmtr) for i in range(arr.shape[0])]) + "}"


### Execute script's main() function ###
if __name__ == "__main__":
    main()