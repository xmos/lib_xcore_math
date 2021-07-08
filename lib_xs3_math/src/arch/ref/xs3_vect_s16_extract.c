// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "../../vect/vpu_helper.h"
#include "xs3_vpu_scalar_ops.h"






void xs3_vect_s16_extract_high_byte(
    int8_t a[],
    const int16_t b[],
    const unsigned len)
{
  for(int k = 0; k < len; k++){
    a[k] = (b[k] & 0xFF00) >> 8;
  }
}

void xs3_vect_s16_extract_low_byte(
    int8_t a[],
    const int16_t b[],
    const unsigned len)
{
  for(int k = 0; k < len; k++){
    a[k] = (b[k] & 0x00FF);
  }
}



