// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "xs3_math.h"

static int32_t merge_acc(const int16_t high, const uint16_t low)
{
  return (((int32_t)high) << 16) + low;
}

static void split_acc(int16_t* high, uint16_t* low, const int32_t acc)
{
  *high = (acc & 0xFFFF0000) >> 16;
  *low = (acc & 0x0000FFFF);
}

void xs3_mat_mul_s8_x_s16_yield_s32_ref (
    int32_t output[],
    const int8_t weights[],
    const int16_t input[],
    const unsigned M_rows,
    const unsigned N_cols)
{
  for(int row = 0; row < M_rows; row++){

    output[row] = 0;
 
    for(int col = 0; col < N_cols; col++){
      output[row] += ((int32_t)weights[row * N_cols + col]) * ((int32_t)input[col]);
    }
  }
}

void xs3_mat_mul_s8_x_s16_yield_s32 (
    int32_t output[],
    const int8_t weights[],
    const int16_t input[],
    const unsigned M_rows,
    const unsigned N_cols,
    int8_t scratch[])
{
  const int out_groups = M_rows >> VPU_INT8_ACC_PERIOD_LOG2;
  xs3_split_acc_s32_t* accs = (xs3_split_acc_s32_t*) output;

  memset(output, 0, sizeof(int32_t) * M_rows);

  // Multiply-accumulate the high words onto the accumulators
  xs3_vect_s16_extract_high_byte(scratch, input, N_cols);
  xs3_mat_mul_s8_x_s8_yield_s32(accs, weights, scratch, M_rows, N_cols);

  // Shift accumulators left 8 bits.  (Should I just have the assembly convert them to not be split..?)
  for(int out_group = 0; out_group < out_groups; out_group++){
    for(int offset = 0; offset < VPU_INT8_ACC_PERIOD; offset++){
      int32_t acc32 = merge_acc(accs[out_group].vD[offset], accs[out_group].vR[offset]);
      acc32 = acc32 << 8;
      split_acc(&accs[out_group].vD[offset], &accs[out_group].vR[offset], acc32);
    }
  }

  // Now multiply-accumulate the low bytes onto the accumulators
  xs3_vect_s16_extract_low_byte(scratch, input, N_cols);
  xs3_mat_mul_s8_x_s8_yield_s32(accs, weights, scratch, M_rows, N_cols);

  // Next, correct for the negative values. We need to add 256 * weight for each low byte that is negative.
  xs3_vect_s8_is_negative(scratch, scratch, N_cols);

  for(int k = 0; k < N_cols; k++){
    scratch[k] *= 64;
  }

  xs3_mat_mul_s8_x_s8_yield_s32(accs, weights, scratch, M_rows, N_cols);
  xs3_mat_mul_s8_x_s8_yield_s32(accs, weights, scratch, M_rows, N_cols);
  xs3_mat_mul_s8_x_s8_yield_s32(accs, weights, scratch, M_rows, N_cols);
  xs3_mat_mul_s8_x_s8_yield_s32(accs, weights, scratch, M_rows, N_cols);

  // Finally, turn the split accumulators into proper int32s
  
  for(int out_group = 0; out_group < out_groups; out_group++){
    xs3_split_acc_s32_t tmp = accs[out_group];
    for(int offset = 0; offset < VPU_INT8_ACC_PERIOD; offset++){
      const int row = out_group * VPU_INT8_ACC_PERIOD + offset;
      output[row] = merge_acc(tmp.vD[offset], tmp.vR[offset]);
    }
  }
  
}