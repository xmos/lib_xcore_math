// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "xs3_math.h"
#include "../../vect/vpu_helper.h"
#include "xs3_vpu_scalar_ops.h"
#include "../../vect/vpu_const_vects.h"



void xs3_vect_s32_merge_accs(
    int32_t a[],
    const xs3_split_acc_s32_t b[],
    const unsigned length)
{
  // We have to copy b[] in case the merge is in-place.

  unsigned N = (length + VPU_INT8_ACC_PERIOD - 1) >> VPU_INT8_ACC_PERIOD_LOG2;

  for(int g = 0; g < N; g++){
    xs3_split_acc_s32_t tmp = b[g];

    for(int o = 0; o < VPU_INT8_ACC_PERIOD; o++){
      int k = g * VPU_INT8_ACC_PERIOD + o;
      
      int32_t  acc_hi = tmp.vD[o];
      uint32_t acc_lo = tmp.vR[o];

      a[k] = (acc_hi << 16) | (acc_lo & 0x0000FFFF);

    }
  }
}



void xs3_vect_s32_split_accs(
    xs3_split_acc_s32_t a[],
    const int32_t b[],
    const unsigned length)
{
  // We have to copy b[] in case the split is in-place.
  unsigned N = ((length + VPU_INT8_ACC_PERIOD - 1) >> VPU_INT8_ACC_PERIOD_LOG2);

  for(int g = 0; g < N; g++){
    xs3_split_acc_s32_t tmp;

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