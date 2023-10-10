// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>

#include "xmath/xmath.h"
#include "vpu_helper.h"
#include "xmath/xs3/vpu_scalar_ops.h"
#include "vpu_const_vects.h"


void vect_s32_zip(
  complex_s32_t a[],
  const int32_t b[],
  const int32_t c[],
  const unsigned length,
  const right_shift_t b_shr,
  const right_shift_t c_shr)
{
  for(unsigned i = 0; i < length; i++){
    const int32_t B = b[i];
    const int32_t C = c[i];
    a[i].re = vlashr32(B, b_shr);
    a[i].im = vlashr32(C, c_shr);
  }
}


void vect_s32_unzip(
  int32_t a[],
  int32_t b[],
  const complex_s32_t c[],
  const unsigned length)
{
  for(unsigned i = 0; i < length; i++){
    const int32_t A = c[i].re;
    const int32_t B = c[i].im;
    a[i] = A;
    b[i] = B;
  }
}
