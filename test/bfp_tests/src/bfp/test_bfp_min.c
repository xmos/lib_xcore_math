// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "bfp_math.h"

#include "../tst_common.h"

#include "unity_fixture.h"


TEST_GROUP_RUNNER(bfp_min) {
  RUN_TEST_CASE(bfp_min, bfp_s16_min);
  RUN_TEST_CASE(bfp_min, bfp_s32_min);
}
TEST_GROUP(bfp_min);
TEST_SETUP(bfp_min) {}
TEST_TEAR_DOWN(bfp_min) {}

#define REPS        1000
#define MAX_LEN     1024 


TEST(bfp_min, bfp_s16_min)
{

    unsigned seed = SEED_FROM_FUNC_NAME();
    int16_t dataB[MAX_LEN];
    bfp_s16_t B;
    B.data = dataB;
    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);
        B.length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        B.exp = pseudo_rand_int(&seed, -5, 5);
        B.hr = pseudo_rand_uint(&seed, 0, 15);
        
        float_s16_t expected = {
            .mant = INT16_MAX,
            .exp = B.exp,
        };

        for(int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int16(&seed) >> B.hr;
            expected.mant = MIN(expected.mant, B.data[i]);
        }
        bfp_s16_headroom(&B);

        float_s16_t result = bfp_s16_min(&B);

        TEST_ASSERT_EQUAL(expected.exp, result.exp);
        TEST_ASSERT_EQUAL_INT16(expected.mant, result.mant);
    }
}


TEST(bfp_min, bfp_s32_min)
{

    unsigned seed = SEED_FROM_FUNC_NAME();
    int32_t dataB[MAX_LEN];
    bfp_s32_t B;
    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);
        bfp_s32_init(&B, dataB, pseudo_rand_int(&seed, -5, 5),
                            pseudo_rand_uint(&seed, 1, MAX_LEN+1), 0);
        B.hr = pseudo_rand_uint(&seed, 0, 28);

        float_s32_t expected = {
            .mant = INT32_MAX,
            .exp = B.exp,
        };
        
        for(int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int32(&seed) >> B.hr;
            expected.mant = MIN(expected.mant, B.data[i]);
        }
        bfp_s32_headroom(&B);

        float_s32_t result = bfp_s32_min(&B);

        TEST_ASSERT_EQUAL(expected.exp, result.exp);
        TEST_ASSERT_EQUAL_INT32(expected.mant, result.mant);
    }
}


