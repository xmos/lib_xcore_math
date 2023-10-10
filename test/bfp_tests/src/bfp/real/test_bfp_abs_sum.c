// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xmath/xmath.h"

#include "../../tst_common.h"

#include "unity_fixture.h"


TEST_GROUP_RUNNER(bfp_abs_sum) {
  RUN_TEST_CASE(bfp_abs_sum, bfp_s16_abs_sum);
  RUN_TEST_CASE(bfp_abs_sum, bfp_s32_abs_sum);
}

TEST_GROUP(bfp_abs_sum);
TEST_SETUP(bfp_abs_sum) { fflush(stdout); }
TEST_TEAR_DOWN(bfp_abs_sum) {}



#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (128)
#else
#  define REPS       (1000)
#  define MAX_LEN    (512)
#endif


TEST(bfp_abs_sum, bfp_s16_abs_sum)
{


    unsigned seed = SEED_FROM_FUNC_NAME();

    int16_t dataB[MAX_LEN];
    bfp_s16_t B;

    B.data = dataB;

    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        B.length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        B.exp = pseudo_rand_int(&seed, -100, 100);
        B.hr = pseudo_rand_uint(&seed, 0, 15);

        for(unsigned int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int16(&seed) >> B.hr;
        }

        bfp_s16_headroom(&B);

        float_s32_t result = bfp_s16_abs_sum(&B);

        float_s32_t expected = {
            .mant = 0,
            .exp = B.exp };

        for(unsigned int i = 0; i < B.length; i++)
            expected.mant += abs(B.data[i]);

        TEST_ASSERT_EQUAL(expected.exp, result.exp);
        TEST_ASSERT_EQUAL_INT32(expected.mant, result.mant);
    }
}

TEST(bfp_abs_sum, bfp_s32_abs_sum)
{


    unsigned seed = SEED_FROM_FUNC_NAME();

    int32_t dataB[MAX_LEN];
    bfp_s32_t B;

    B.data = dataB;

    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        B.length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        B.exp = pseudo_rand_int(&seed, -100, 100);
        B.hr = pseudo_rand_uint(&seed, 0, 31);
        
        float_s64_t expected = {
            .mant = 0,
            .exp = B.exp };

        for(unsigned int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int32(&seed) >> B.hr;
            expected.mant += abs( B.data[i] );
        }

        bfp_s32_headroom(&B);

        float_s64_t result = bfp_s32_abs_sum(&B);

        TEST_ASSERT_EQUAL(expected.exp, result.exp);
        TEST_ASSERT_EQUAL(expected.mant, result.mant);
    }
}
