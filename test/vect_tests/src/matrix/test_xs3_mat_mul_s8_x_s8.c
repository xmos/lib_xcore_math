// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xs3_math.h"

#include "../tst_common.h"

#include "unity_fixture.h"

TEST_GROUP_RUNNER(xs3_mat_mul_s8_x_s8_yield_s32) {
  RUN_TEST_CASE(xs3_mat_mul_s8_x_s8_yield_s32, xs3_mat_mul_s8_x_s8_yield_s32);
}

TEST_GROUP(xs3_mat_mul_s8_x_s8_yield_s32);
TEST_SETUP(xs3_mat_mul_s8_x_s8_yield_s32) {}
TEST_TEAR_DOWN(xs3_mat_mul_s8_x_s8_yield_s32) {}


static char detail_buff[200];


#define MAX_OUT_GROUPS  (6)
#define MAX_IN_GROUPS   (6)
#define REPS            (1000)

#define MAX_ROWS        (MAX_OUT_GROUPS * VPU_INT8_ACC_PERIOD)
#define MAX_COLS        (MAX_IN_GROUPS * VPU_INT8_EPV)

TEST(xs3_mat_mul_s8_x_s8_yield_s32, xs3_mat_mul_s8_x_s8_yield_s32)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    xs3_split_acc_s32_t WORD_ALIGNED accumulators[MAX_OUT_GROUPS];
    int8_t WORD_ALIGNED matrix[MAX_ROWS*MAX_COLS];
    int8_t WORD_ALIGNED vector[MAX_COLS];

    for(int v = 0; v < REPS; v++){

        unsigned out_groups = pseudo_rand_uint(&seed, 1, MAX_OUT_GROUPS+1);
        unsigned in_groups = pseudo_rand_uint(&seed, 1, MAX_IN_GROUPS+1);

        if(v == 0){
          out_groups = 1;
          in_groups = 1;
        }
        
        const unsigned rows = out_groups * VPU_INT8_ACC_PERIOD;
        const unsigned cols = in_groups * VPU_INT8_EPV;

        sprintf(detail_buff, "( rep: %d; seed: 0x%08X; rows: %u; cols: %u )", v, seed, rows, cols);
        UNITY_SET_DETAIL(detail_buff);

        for(int row = 0; row < rows; row++){
          for(int col = 0; col < cols; col++){
            matrix[row * cols + col] = pseudo_rand_int8(&seed);
          }
        }
        pseudo_rand_int8(&seed);

        for(int col = 0; col < cols; col++)
          vector[col] = pseudo_rand_int8(&seed);

        memset(accumulators, 0xFF, sizeof(accumulators));

        xs3_mat_mul_s8_x_s8_yield_s32(accumulators, matrix, vector, rows, cols);

        for(int row = 0; row < rows; row++) {
          int grp = row / VPU_INT8_ACC_PERIOD;
          int offset = row % VPU_INT8_ACC_PERIOD;
          
          int32_t expected = -1; // They were initialized to -1

          for(int col = 0; col < cols; col++){
            expected += ((int32_t)matrix[row * cols + col]) * vector[col];
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

