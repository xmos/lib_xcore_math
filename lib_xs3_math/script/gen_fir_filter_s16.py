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
                      help="Name of the generated filter. This name will be used to initialize and invoke the filter from user code.")

  parser.add_argument("filter_coefficients",
                      type=existing_file,
                      help="File containing the filter coefficients. (Separated by whitespace and/or commas")

  parser.add_argument("--filter-prefix",
                      type=filter_name_type,
                      default="filter_",
                      help="Prefix used for filenames, functions and variables related to this filter. (default: 'filter_')")

  parser.add_argument("--input-headroom",
                      type=int,
                      default=0,
                      help="Minimum number of redundant sign bits known a priori to be present in the 16-bit input signal to be filtered.")

  parser.add_argument("--output-headroom",
                      type=int,
                      default=0,
                      help="Minimum number of redundant sign bits that the output should have.")

  args = parser.parse_args()
  run(args)

def run(args):
  
  coefs = load_coefficients_from_file(args.filter_coefficients)

  mantissas, shift, exponent = find_filter_parameters(coefs, args)

  write_header_file(len(mantissas), args)
  write_source_file(mantissas, shift, exponent, args)



def find_filter_parameters(coefs, args):
  N_taps = len(coefs)

  # Find least-headroom 16-bit BFP representation of coefs
  largest_coef = np.max(np.abs(coefs))
  scale = np.ldexp(1,15) / largest_coef
  scale_log2 = np.floor(np.log2(scale))
  scale = 2**scale_log2

  scaled_coefs = np.round(scale * coefs).astype(np.int64)
  exponent = -scale_log2
  
  # Find maximum possible dot-product of coefs with input (given known input headroom)
  dot_prod = max_dot_product(scaled_coefs, args.input_headroom)
  dot_prod_log2 = np.log2(dot_prod)

  # If that dot-product doesn't fit in 32 bits, add headroom to coefficients until it does.
  if dot_prod_log2 > 31.0:
    tmp = np.ceil(dot_prod_log2)-31
    scaled_coefs = np.round(scaled_coefs * 2**-tmp).astype(np.int64)
    exponent = exponent + tmp
    dot_prod = max_dot_product(scaled_coefs, args.input_headroom)
    dot_prod_log2 = np.log2(dot_prod)
  
  # Find the shift value that makes the output fit in 16 bits
  shift = 0
  if dot_prod_log2 > 15.0:
    shift = int(np.ceil(dot_prod_log2 - 15.0))

  # If the user wants guaranteed output headroom, include that as well.
  shift = shift + args.output_headroom

  return scaled_coefs.astype(np.int16), int(shift), int(exponent)

def max_dot_product(coefs, input_hr):
  max_pos_input = ((2**15)-1)>>input_hr
  max_neg_input = (-(2**15))>>input_hr

  inputs = [(max_pos_input if x else max_neg_input) for x in (coefs >= 0)]
  dot_prod = np.sum([np.round( (x*y) ) for x,y in zip(coefs, inputs)])
  return dot_prod

def load_coefficients_from_file(fpath):
  with open(fpath) as file:
    lines = file.readlines()
    lines = " ".join(lines)
    lines = lines.replace(",", " ")
    coefs = lines.split()
    coefs = [float(x) for x in coefs]
    return np.array(coefs)
    

def write_header_file(N_taps, args):
  filter = f"{args.filter_prefix}{args.filter_name}"
  with open(f"{filter}.h", "w+") as header_file:
    header_file.write(f"""
#pragma once
#include "xs3_math.h"

// Number of filter coefficients 
#define TAP_COUNT_{filter}\t({N_taps})

// Exponent associated with filter outputs
extern const exponent_t {filter}_exp;

// Call once to initialize the filter
C_API
void {filter}_init();

// Call to add a sample to the filter without computing an output sample
C_API
void {filter}_add_sample(int16_t new_sample);

// Call to process an input sample and generate an output sample
C_API
int16_t {filter}(int16_t new_sample);
    """)

def write_source_file(coefs, shift, exponent, args):
  filter = f"{args.filter_prefix}{args.filter_name}"
  N_taps = len(coefs)
  coef_string = ""
  for i in range(N_taps):
    coef_string += f"{hex(coefs[i])}, "
    if (i % 8 == 7) and (i != N_taps-1):
        coef_string += "\n\t"


  with open(f"{filter}.c", "w+") as source_file:

    source_file.write(f"""
#include "{filter}.h"

const right_shift_t {filter}_shift = {shift};
const exponent_t {filter}_exp = {exponent};
const int16_t WORD_ALIGNED {filter}_coefs[TAP_COUNT_{filter}] = {{
  {coef_string}
}};

int16_t WORD_ALIGNED {filter}_state[TAP_COUNT_{filter}] = {{0}};

xs3_filter_fir_s16_t _{filter};

void {filter}_init()
{{
  xs3_filter_fir_s16_init(&_{filter}, {filter}_state, TAP_COUNT_{filter},
                          {filter}_coefs, {filter}_shift);
}}

void {filter}_add_sample(int16_t new_sample)
{{
  xs3_filter_fir_s16_add_sample(&_{filter}, new_sample);
}}

int16_t {filter}(int16_t new_sample)
{{
  return xs3_filter_fir_s16(&_{filter}, new_sample);
}}
""")


filter_name_pattern = re.compile(r"^[A-Za-z_][A-Za-z0-9_]+$")

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