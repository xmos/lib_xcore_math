// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "xmath/xmath.h"


headroom_t vect_s16_shr(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const right_shift_t shr)
{
    return vect_s16_shl(a, b, length, -shr);
}


headroom_t vect_s16_add_scalar(
    int16_t a[],
    const int16_t b[],
    const int16_t c,
    const unsigned length,
    const right_shift_t b_shr)
{
  int32_t cc = c;
  cc = (cc << 16) | (cc & 0x0000FFFF);
  return 15 - vect_sXX_add_scalar((int32_t*) a, (int32_t*)  b, 
                                      (length<<1), cc, cc, b_shr, 0x0100);
}

