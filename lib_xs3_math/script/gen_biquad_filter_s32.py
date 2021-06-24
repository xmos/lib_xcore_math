# Copyright 2021-2021 XMOS LIMITED.
# This Software is subject to the terms of the XMOS Public Licence: Version 1.
import numpy as np
import argparse
import os.path
import re

def main():
  
  parser = argparse.ArgumentParser()

  parser.add_argument("filter_name",
                      type=filter_name_type,
                      help="Name of the generated filter. This name will be used to invoke the filter from user code.")

  parser.add_argument("filter_coefficients",
                      type=existing_file,
                      help="File containing the filter coefficients. (Separated by whitespace and/or commas)")

  parser.add_argument("--filter-prefix",
                      type=filter_name_type,
                      default="filter_",
                      help="Prefix used for filenames, functions and variables related to this filter. (default: 'filter_')")


  args = parser.parse_args()
  run(args)

def run(args):
  
  coefs = load_coefficients_from_file(args.filter_coefficients)

  coefs_q30 = find_filter_parameters(coefs, args)

  write_header_file(args)
  write_source_file(coefs_q30, args)



def find_filter_parameters(coefs, args):

  assert(len(coefs) % 5 == 0)
  N_sections = len(coefs) // 5
  coefs = coefs.reshape((5,N_sections))
  coefs = coefs * 2**30
  return np.round(coefs).astype(np.int32)

def load_coefficients_from_file(fpath):
  with open(fpath) as file:
    lines = file.readlines()
    lines = " ".join(lines)
    lines = lines.replace(",", " ")
    coefs = lines.split()
    coefs = [float(x) for x in coefs]
    return np.array(coefs)
    

def write_header_file(args):
  filter = f"{args.filter_prefix}{args.filter_name}"
  with open(f"{filter}.h", "w+") as header_file:
    header_file.write(
f"""#pragma once

#include "xs3_math.h"

// Call to process an input sample and generate an output sample
C_API
int32_t {filter}(int32_t new_sample);
""")

def write_source_file(coefs, args):
  filter = f"{args.filter_prefix}{args.filter_name}"

  N_sections = coefs.shape[1]
  N_blocks = (N_sections+7)//8

  with open(f"{filter}.c", "w+") as source_file:

    source_file.write(
f"""
#include "{filter}.h"

xs3_biquad_filter_s32_t _{filter}[{N_blocks}] = {all_biquad_blocks(coefs)};

int32_t {filter}(int32_t new_sample)
{{
  return xs3_filter_biquads_s32(&_{filter}, new_sample);
}}
""")

filter_name_pattern = re.compile(r"^[A-Za-z_][A-Za-z0-9_]+$")


def all_biquad_blocks(block_coefs: np.ndarray):
  N_sections = block_coefs.shape[1]
  N_blocks = (N_sections+7) // 8

  blocks = [block_coefs[:, 8*k:8*(k+1)] for k in range(N_blocks)]


  blocks_str = ",".join([biquad_block_to_str(x) for x in blocks])
  return f"{{\n {blocks_str}  }}"



def biquad_block_to_str(block_coefs: np.ndarray):
  N_sections = block_coefs.shape[1]
  block_coefs = np.concatenate([block_coefs, np.zeros(shape=(5,8-N_sections),dtype=np.int32)], axis=1)

  return f"""{{
    .biquad_count = {N_sections},
    .state = {{{{0}}}},
    .coef = { array_to_string(block_coefs, hex) }
  }}"""
  

def array_to_string(arr, val_fmtr=str):
    if len(arr.shape) == 1:
        return f"{{{', '.join([val_fmtr(x) for x in arr])}}}"
    else:
        return (
            "{\n"
            + ",\n".join([array_to_string(arr[i], val_fmtr) for i in range(arr.shape[0])])
            + "}"
        )

def filter_name_type(fname):
  if filter_name_pattern.match(fname) is None: 
    raise ""
  return fname

def existing_file(fpath):
  if not os.path.isfile(fpath):
    raise ""
  return fpath

if __name__ == "__main__":
    main()