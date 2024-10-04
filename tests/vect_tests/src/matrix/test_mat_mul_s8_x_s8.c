// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xmath/xmath.h"
 
#include "../tst_common.h"

#include "unity_fixture.h"

TEST_GROUP_RUNNER(mat_mul_s8_x_s8_yield_s32) {
  RUN_TEST_CASE(mat_mul_s8_x_s8_yield_s32, mat_mul_s8_x_s8_yield_s32);
}

TEST_GROUP(mat_mul_s8_x_s8_yield_s32);
TEST_SETUP(mat_mul_s8_x_s8_yield_s32) { fflush(stdout); }
TEST_TEAR_DOWN(mat_mul_s8_x_s8_yield_s32) {}

static char detail_buff[200];

#define MAX_OUT_GROUPS  (16)
#define MAX_IN_GROUPS   (16)
#define REPS            ((SMOKE_TEST)?100:1000)

#define MAX_ROWS        (MAX_OUT_GROUPS * VPU_INT8_ACC_PERIOD)
#define MAX_COLS        (MAX_IN_GROUPS * VPU_INT8_EPV)

TEST(mat_mul_s8_x_s8_yield_s32, mat_mul_s8_x_s8_yield_s32)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    split_acc_s32_t WORD_ALIGNED accumulators[MAX_OUT_GROUPS];
    int8_t WORD_ALIGNED matrix[MAX_ROWS*MAX_COLS];
    int8_t WORD_ALIGNED vector[MAX_COLS];

    for(unsigned int v = 0; v < REPS; v++){

        unsigned M_rows = pseudo_rand_uint(&seed, 1, MAX_ROWS + 1);
        unsigned N_cols = 4 * pseudo_rand_uint(&seed, 1, (MAX_COLS/4) + 1);

        TEST_ASSERT(N_cols > 0);


        sprintf(detail_buff, "( rep: %d; seed: 0x%08X; rows: %u; cols: %u )", v, seed, M_rows, N_cols);
        UNITY_SET_DETAIL(detail_buff);

        for(unsigned int row = 0; row < M_rows; row++){
          for(unsigned int col = 0; col < N_cols; col++){
            matrix[row * N_cols + col] = pseudo_rand_int8(&seed);
          }
        }
        pseudo_rand_int8(&seed);

        memset(vector, 0, sizeof(vector));
        for(unsigned int col = 0; col < N_cols; col++)
          vector[col] = pseudo_rand_int8(&seed);

        memset(accumulators, 0xFF, sizeof(accumulators));

        mat_mul_s8_x_s8_yield_s32(accumulators, matrix, vector, M_rows, N_cols);


        for(unsigned int row = 0; row < M_rows; row++) {
          int grp = row / VPU_INT8_ACC_PERIOD;
          int offset = row % VPU_INT8_ACC_PERIOD;
          
          int32_t expected = -1; // They were initialized to -1

          for(unsigned int col = 0; col < N_cols; col++){
            expected += ((int32_t)matrix[row * N_cols + col]) * vector[col];
          }

          int32_t actual = (((int32_t)accumulators[grp].vD[offset]) << VPU_INT8_ACC_VR_BITS)
                          + accumulators[grp].vR[offset];

          if(actual != expected){
            printf("accumulator[%d] = 0x%08X  (expected: 0x%08X)\n", row, (unsigned) actual, (unsigned) expected);
          }

          TEST_ASSERT_EQUAL_INT32(expected, actual);
        }
    }
}

