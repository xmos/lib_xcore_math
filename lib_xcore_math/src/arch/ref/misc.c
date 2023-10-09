// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "xmath/xmath.h"
#include "vpu_helper.h"
#include "xmath/xs3/vpu_scalar_ops.h"
#include "vpu_const_vects.h"


void vect_s32_merge_accs(
    int32_t a[],
    const split_acc_s32_t b[],
    const unsigned length)
{
  // We have to copy b[] in case the merge is in-place.

  unsigned N = (length + VPU_INT8_ACC_PERIOD - 1) >> VPU_INT8_ACC_PERIOD_LOG2;

  for(unsigned g = 0; g < N; g++){
    split_acc_s32_t tmp = b[g];

    for(int o = 0; o < VPU_INT8_ACC_PERIOD; o++){
      int k = g * VPU_INT8_ACC_PERIOD + o;
      
      int32_t  acc_hi = tmp.vD[o];
      uint32_t acc_lo = tmp.vR[o];

      a[k] = (acc_hi << 16) | (acc_lo & 0x0000FFFF);

    }
  }
}

void vect_s32_split_accs(
    split_acc_s32_t a[],
    const int32_t b[],
    const unsigned length)
{
  // We have to copy b[] in case the split is in-place.
  unsigned N = ((length + VPU_INT8_ACC_PERIOD - 1) >> VPU_INT8_ACC_PERIOD_LOG2);

  for(unsigned g = 0; g < N; g++){
    split_acc_s32_t tmp;

    for(int o = 0; o < VPU_INT8_ACC_PERIOD; o++){
      int k = g * VPU_INT8_ACC_PERIOD + o;
      int32_t acc32 = b[k];

      int32_t acc_hi = (acc32 & 0xFFFF0000) >> 16;
      int32_t acc_lo = (acc32 & 0x0000FFFF);

      tmp.vD[o] = (int16_t) acc_hi;
      tmp.vR[o] = (int16_t) acc_lo;

    }

    a[g] = tmp;
  }
}

void f32_unpack(
    int32_t * mantissa,
    exponent_t * exp,
    const float input)
{
  *mantissa = lroundf(INT32_MAX * frexpf(input, exp));
  *exp -= 31;
}

float s32_to_f32(
    const int32_t mantissa,
    const exponent_t exp)
{
  return ldexpf((float) mantissa, exp);
}
