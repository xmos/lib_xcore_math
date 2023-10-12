// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xmath/xmath.h"

#include "testing.h"
#include "../tst_common.h"

#include "unity_fixture.h"

TEST_GROUP_RUNNER(mat_mul_s8_x_s16_yield_s32) {
  RUN_TEST_CASE(mat_mul_s8_x_s16_yield_s32, mat_mul_s8_x_s16_yield_s32);
}

TEST_GROUP(mat_mul_s8_x_s16_yield_s32);
TEST_SETUP(mat_mul_s8_x_s16_yield_s32) { fflush(stdout); }
TEST_TEAR_DOWN(mat_mul_s8_x_s16_yield_s32) {}

// static char detail_buff[200];


#define MAX_OUT_GROUPS  (16)
#define MAX_IN_GROUPS   (16)
#define REPS            ((SMOKE_TEST)?100:1000)

#define MAX_ROWS        (MAX_OUT_GROUPS * VPU_INT8_ACC_PERIOD)
#define MAX_COLS        (MAX_IN_GROUPS * VPU_INT8_EPV)

static void mat_mul_s8_x_s16_yield_s32_ref (
    int32_t output[],
    const int8_t weights[],
    const int16_t input[],
    const unsigned M_rows,
    const unsigned N_cols)
{
  for(unsigned int row = 0; row < M_rows; row++){

    output[row] = 0;
 
    for(unsigned int col = 0; col < N_cols; col++){
      output[row] += ((int32_t)weights[row * N_cols + col]) * ((int32_t)input[col]);
    }
  }
}


TEST(mat_mul_s8_x_s16_yield_s32, mat_mul_s8_x_s16_yield_s32)
{
  unsigned seed = SEED_FROM_FUNC_NAME();


  int32_t DWORD_ALIGNED output[MAX_ROWS];
  int32_t DWORD_ALIGNED output_ref[MAX_ROWS];
  int8_t DWORD_ALIGNED matrix[MAX_ROWS*MAX_COLS];
  int16_t DWORD_ALIGNED vector[MAX_COLS];

  int8_t DWORD_ALIGNED scratch[MAX_COLS];

  // float max_ratio = 0;

  // printf("\n\n\n");

  for(unsigned int v = 0; v < REPS; v++){

      unsigned M_rows = pseudo_rand_uint(&seed, 1, MAX_ROWS+1);
      unsigned N_cols = (pseudo_rand_uint(&seed, 4, MAX_COLS+1) >> 2) << 2;

      // printf("\trep % 3d...\t(seed: 0x%08X) (rows, cols: %u, %u)\n", v, seed, M_rows, N_cols);

      for(unsigned int row = 0; row < M_rows; row++){
        for(unsigned int col = 0; col < N_cols; col++){
          matrix[row * N_cols + col] = pseudo_rand_int8(&seed);
        }
      }
      pseudo_rand_int8(&seed);

      memset(vector, 0, sizeof(vector));
      for(unsigned int col = 0; col < N_cols; col++)
        vector[col] = pseudo_rand_int16(&seed);

      // unsigned opt_start, opt_end;
      // unsigned ref_start, ref_end;

      // opt_start = getTimestamp();
      mat_mul_s8_x_s16_yield_s32(output, matrix, vector, M_rows, N_cols, scratch);
      // opt_end = getTimestamp();
      // ref_start = getTimestamp();
      mat_mul_s8_x_s16_yield_s32_ref(output_ref, matrix, vector, M_rows, N_cols);
      // ref_end = getTimestamp();

      // unsigned opt_ns = 10 * (opt_end - opt_start);

      // unsigned ref_ns = 10 * (ref_end - ref_start);
      // float ratio = (ref_ns + 0.0f) / opt_ns;

      // max_ratio = (ratio > max_ratio)? ratio : max_ratio;

      // printf("%d x %d:  %0.02f\n", M_rows, N_cols, ratio);
      // printf("% 3d x % 3d:  %f us\n", M_rows, N_cols, opt_ns / 1000.0f);

      for(unsigned int row = 0; row < M_rows; row++) {

        if(output[row] != output_ref[row]){
          printf("\noutput[%d] = 0x%08X  (output_ref: 0x%08X)\n",
                  row, (unsigned) output[row], (unsigned) output_ref[row]);
        }

        TEST_ASSERT_EQUAL_INT32_MESSAGE(output_ref[row], output[row], "");
      }
  }

  // printf("Max speed-up: %0.02f\n", max_ratio);
  
}

