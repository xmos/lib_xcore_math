// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>

#include "xmath/xmath.h"




float_s32_t float_s64_to_float_s32(
    const float_s64_t x)
{
  float_s32_t res;
  res.mant = s64_to_s32(&res.exp, x.mant, x.exp);
  return res;
}