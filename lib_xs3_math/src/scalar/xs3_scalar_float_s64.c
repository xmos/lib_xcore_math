// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"




float_s32_t float_s64_to_float_s32(
    const float_s64_t x)
{
  float_s32_t res = {
    .exp = x.exp,
    .mant = (int32_t) x.mant
  };

  headroom_t x_hr = HR_S64(x.mant);

  if(x_hr < 32) {
    right_shift_t shr = 32 - x_hr;
    res.mant = x.mant >> shr;
    res.exp = x.exp + shr;
  }

  return res;
}