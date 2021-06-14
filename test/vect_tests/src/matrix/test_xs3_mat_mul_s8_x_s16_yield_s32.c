// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xs3_math.h"

#include "testing.h"
#include "../tst_common.h"

#include "unity.h"


static unsigned seed = 546745;

#define MAX_OUT_GROUPS  (16)
#define MAX_IN_GROUPS   (16)
#define REPS            (1000)

#define MAX_ROWS        (MAX_OUT_GROUPS * VPU_INT8_ACC_PERIOD)
#define MAX_COLS        (MAX_IN_GROUPS * VPU_INT8_EPV)

static void xs3_mat_mul_s8_x_s16_yield_s32_ref (
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



static void test_xs3_mat_mul_s8_x_s16_yield_s32()
{
  printf("%s... \n", __func__);

  seed = 54654333;

  int32_t WORD_ALIGNED output[MAX_ROWS];
  int32_t WORD_ALIGNED output_ref[MAX_ROWS];
  int8_t WORD_ALIGNED matrix[MAX_ROWS*MAX_COLS];
  int16_t WORD_ALIGNED vector[MAX_COLS];

  int8_t WORD_ALIGNED scratch[MAX_COLS];

  float max_ratio = 0;

  for(int v = 0; v < REPS; v++){

      unsigned out_groups = pseudo_rand_uint(&seed, 1, MAX_OUT_GROUPS+1);
      unsigned in_groups = pseudo_rand_uint(&seed, 1, MAX_IN_GROUPS+1);

      if(v == 0){
        out_groups = 1;
        in_groups = 1;
      } else if(v == REPS-1){
        out_groups = MAX_OUT_GROUPS;
        in_groups = MAX_IN_GROUPS; 
      }
      
      const unsigned rows = out_groups * VPU_INT8_ACC_PERIOD;
      const unsigned cols = in_groups * VPU_INT8_EPV;

      // printf("\trep % 3d...\t(seed: 0x%08X) (rows, cols: %u, %u)\n", v, seed, rows, cols);

      for(int row = 0; row < rows; row++){
        for(int col = 0; col < cols; col++){
          matrix[row * cols + col] = pseudo_rand_int8(&seed);
        }
      }
      pseudo_rand_int8(&seed);

      for(int col = 0; col < cols; col++)
        vector[col] = pseudo_rand_int16(&seed);

      unsigned opt_start, opt_end;
      unsigned ref_start, ref_end;

      opt_start = getTimestamp();
      xs3_mat_mul_s8_x_s16_yield_s32(output, matrix, vector, rows, cols, scratch);
      opt_end = getTimestamp();
      ref_start = getTimestamp();
      xs3_mat_mul_s8_x_s16_yield_s32_ref(output_ref, matrix, vector, rows, cols);
      ref_end = getTimestamp();

      unsigned opt_ns = 10 * (opt_end - opt_start);
      unsigned ref_ns = 10 * (ref_end - ref_start);
      float ratio = (ref_ns + 0.0f) / opt_ns;

      max_ratio = (ratio > max_ratio)? ratio : max_ratio;

      printf("%d x %d:  %0.02f\n", rows, cols, ratio);

      for(int row = 0; row < rows; row++) {

        if(output[row] != output_ref[row]){
          printf("output[%d] = 0x%08X  (output_ref: 0x%08X)\n", 
                  row, (unsigned) output[row], (unsigned) output_ref[row]);
        }

        TEST_ASSERT_EQUAL_INT32(output_ref[row], output[row]);
      }
  }

  printf("Max speed-up: %0.02f\n", max_ratio);
  
}



void test_xs3_mat_mul_s8_x_s16()
{
    SET_TEST_FILE();

    RUN_TEST(test_xs3_mat_mul_s8_x_s16_yield_s32);

}
