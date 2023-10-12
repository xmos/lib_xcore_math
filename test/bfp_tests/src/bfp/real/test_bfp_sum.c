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


TEST_GROUP_RUNNER(bfp_sum) {
  RUN_TEST_CASE(bfp_sum, bfp_s16_sum);
  RUN_TEST_CASE(bfp_sum, bfp_s32_sum);
}

TEST_GROUP(bfp_sum);
TEST_SETUP(bfp_sum) { fflush(stdout); }
TEST_TEAR_DOWN(bfp_sum) {}

#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (128)
#else
#  define REPS       (1000)
#  define MAX_LEN    (512)
#endif


TEST(bfp_sum, bfp_s16_sum)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    int16_t dataB[MAX_LEN];
    bfp_s16_t B;

    B.data = dataB;

    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        test_random_bfp_s16(&B, MAX_LEN, &seed, NULL, 0);

        float_s32_t result = bfp_s16_sum(&B);

        float_s32_t expected = {
            .mant = 0,
            .exp = B.exp };

        for(unsigned int i = 0; i < B.length; i++)
            expected.mant += B.data[i];

        TEST_ASSERT_EQUAL(expected.exp, result.exp);
        TEST_ASSERT_EQUAL_INT32(expected.mant, result.mant);
    }
}

TEST(bfp_sum, bfp_s32_sum)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    int32_t dataB[MAX_LEN];
    bfp_s32_t B;

    B.data = dataB;

    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        test_random_bfp_s32(&B, MAX_LEN, &seed, NULL, 0);

        float_s64_t result = bfp_s32_sum(&B);

        float_s64_t expected = {
            .mant = 0,
            .exp = B.exp };
            
        for(unsigned int i = 0; i < B.length; i++)
            expected.mant += B.data[i];

        TEST_ASSERT_EQUAL(expected.exp, result.exp);
        TEST_ASSERT(expected.mant == result.mant);
    }
}
