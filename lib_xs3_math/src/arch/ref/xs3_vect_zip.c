// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "../../vect/vpu_helper.h"
#include "xs3_vpu_scalar_ops.h"
#include "../../vect/vpu_const_vects.h"


void xs3_vect_s32_zip(
  complex_s32_t a[],
  const int32_t b[],
  const int32_t c[],
  const unsigned length,
  const right_shift_t b_shr,
  const right_shift_t c_shr)
{
  for(int i = 0; i < length; i++){
    const int32_t B = b[i];
    const int32_t C = c[i];
    a[i].re = (b_shr >= 0)? (B >> b_shr) : (B << -b_shr);
    a[i].im = (c_shr >= 0)? (C >> c_shr) : (C << -c_shr);
  }
}


void xs3_vect_s32_unzip(
  int32_t a[],
  int32_t b[],
  const complex_s32_t c[],
  const unsigned length)
{
  for(int i = 0; i < length; i++){
    const int32_t A = c[i].re;
    const int32_t B = c[i].im;
    a[i] = A;
    b[i] = B;
  }
}