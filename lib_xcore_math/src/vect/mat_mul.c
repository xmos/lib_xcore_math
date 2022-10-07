// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "xmath/xmath.h"




void mat_mul_s8_x_s16_yield_s32_inner (
    int32_t output[],
    const int8_t weights[],
    const int16_t input[],
    const unsigned M_rows,
    const unsigned N_cols,
    int8_t scratch[])
{

  const int out_groups = (M_rows + VPU_INT8_ACC_PERIOD - 1) >> VPU_INT8_ACC_PERIOD_LOG2;
  const int in_groups = (N_cols + VPU_INT8_EPV - 1) >> VPU_INT8_EPV_LOG2;

  const unsigned N_ceil = in_groups << VPU_INT8_EPV_LOG2;

  split_acc_s32_t* accs = (split_acc_s32_t*) output;

  memset(output, 0, sizeof(split_acc_s32_t) * out_groups);
  memset(scratch, 0, sizeof(int8_t) * N_ceil);


  vect_s16_extract_high_byte(scratch, input, N_cols);
  mat_mul_s8_x_s8_yield_s32(accs, weights, scratch, M_rows, N_cols);

  vect_s16_extract_low_byte(scratch, input, N_cols);
  vect_s8_is_negative(scratch, scratch, N_cols);
  mat_mul_s8_x_s8_yield_s32(accs, weights, scratch, M_rows, N_cols);

  vect_s32_merge_accs(output, accs, M_rows);
  vect_s32_shl(output, output, M_rows, 8);
  vect_s32_split_accs(accs, output, M_rows);

  vect_s16_extract_low_byte(scratch, input, N_cols);
  mat_mul_s8_x_s8_yield_s32(accs, weights, scratch, M_rows, N_cols);

  vect_s32_merge_accs(output, accs, M_rows);
  
}




void mat_mul_s8_x_s16_yield_s32 (
    int32_t output[],
    const int8_t weights[],
    const int16_t input[],
    const unsigned M_rows,
    const unsigned N_cols,
    int8_t scratch[])
{

  const unsigned M_tail = M_rows % VPU_INT8_ACC_PERIOD;
  const unsigned M_body = M_rows - M_tail;

  if(M_body){
    mat_mul_s8_x_s16_yield_s32_inner(&output[0], weights, input, M_body, N_cols, scratch);
  }

  if(M_tail){
      // Here we need to write to a temp buffer and copy over as needed.
      int32_t WORD_ALIGNED tmp_buff[VPU_INT8_ACC_PERIOD];

    mat_mul_s8_x_s16_yield_s32_inner(tmp_buff, 
                                         &weights[M_body * N_cols], 
                                         input, M_tail, N_cols, scratch);

    memcpy(&output[M_body], tmp_buff, sizeof(int32_t) * M_tail);
  }



}