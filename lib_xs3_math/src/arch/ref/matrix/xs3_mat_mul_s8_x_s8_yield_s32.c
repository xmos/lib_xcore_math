// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "../../vect/vpu_helper.h"
#include "xs3_vpu_scalar_ops.h"


static int32_t merge_acc(const int16_t high, const uint16_t low)
{
  return (((int32_t)high) << 16) + low;
}

static void split_acc(int16_t* high, uint16_t* low, const int32_t acc)
{
  *high = (acc & 0xFFFF0000) >> 16;
  *low = (acc & 0x0000FFFF);
}


void xs3_mat_mul_s8_x_s8_yield_s32 (
    xs3_split_acc_s32_t accumulators[],
    const int8_t matrix[],
    const int8_t input_vect[],
    const unsigned M_rows,
    const unsigned N_cols);
{
  const int acc_groups = M_rows >> VPU_INT8_ACC_PERIOD_LOG2;

  for(int ag = 0; ag < acc_groups; ag++){
    for(int o = 0; o < VPU_INT8_ACC_PERIOD; o++){
      const int row = (ag * VPU_INT8_ACC_PERIOD) + o;

      int32_t acc = merge_acc(accumulators[ag].vD[o], accumulators[ag].vR[o]);

      for(int k = 0; k < N_cols; k++){
        acc += ((int32_t)input_vect[k]) * matrix[row * N_cols + k];
      }

      split_acc(&accumulators[ag].vD[o], &accumulators[ag].vR[o], acc);
      
    }
  }
}