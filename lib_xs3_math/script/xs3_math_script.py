# Copyright 2021-2022 XMOS LIMITED.
# This Software is subject to the terms of the XMOS Public Licence: Version 1.
# This Software is subject to the terms of the XMOS Public Licence: Version 1.

import numpy as np
import os.path
import re
import io

# Use as argparse 'type'.
# Argument is given as a filepath. This opens the file, loads the coefficients
# and converts them to a numpy array
def fir_coefs_file(fpath):
  assert( os.path.isfile(fpath) )
  with open(fpath) as file:
    lines = file.readlines()
    lines = " ".join(lines)
    lines = lines.replace(",", " ")
    coefs = lines.split()
    coefs = np.array([float(x) for x in coefs], dtype=np.float)
    return coefs


# Use as argparse 'type'.
# Argument is given as a filepath. This opens the file, loads the coefficients
# and converts them to a numpy array
def biquad_sections_file(fpath):
  assert( os.path.isfile(fpath) )
  sections = []
  with open(fpath) as file:
    for line in file.readlines():
      line = line.strip()
      if not line.strip(): continue
      coefs = [float(x) for x in line.split(",")]
      assert(len(coefs) == 5)
      sections.append(coefs)

  sections = np.array(sections, dtype=np.float)
  return sections.transpose()

# Regex pattern for filter names (C identifier)
filter_name_pattern = re.compile(r"^[A-Za-z_][A-Za-z0-9_]+$")

# Use as argparse 'type' for filter name. Throws exception if it doesn't match
# the correct pattern
def filter_id(fname):
  assert( filter_name_pattern.match(fname) is not None )
  return fname


# Format a 1D list of integers as a string
def array_to_str(arr, element_fmt = hex, elements_per_line = 8):
  res = io.StringIO()
  N = len(arr)
  K = elements_per_line
  for i in range(N):
    res.write(f"{element_fmt(arr[i])}, ")
    if (i % K == (K-1)) and (i != N-1):
      res.write("\n\t")
  return res.getvalue()