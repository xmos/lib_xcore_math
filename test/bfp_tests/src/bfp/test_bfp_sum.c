// Copyright 2020 XMOS LIMITED. This Software is subject to the terms of the 
// XMOS Public License: Version 1

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "bfp_math.h"

#include "../tst_common.h"

#include "unity.h"

#if DEBUG_ON || 0
#undef DEBUG_ON
#define DEBUG_ON    (1)
#endif


#define REPS        (100)
#define MAX_LEN     256 


static unsigned seed = 666;





static void test_bfp_s16_sum()
{
    PRINTF("%s...\t(random vectors)\n", __func__);

    seed = 67765974;

    int16_t dataB[MAX_LEN];
    bfp_s16_t B;

    B.data = dataB;

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        test_random_bfp_s16(&B, MAX_LEN, &seed, NULL, 0);

        float_s32_t result = bfp_s16_sum(&B);

        float_s32_t expected = {
            .mant = 0,
            .exp = B.exp };

        for(int i = 0; i < B.length; i++)
            expected.mant += B.data[i];

        TEST_ASSERT_EQUAL(expected.exp, result.exp);
        TEST_ASSERT_EQUAL_INT32(expected.mant, result.mant);
    }
}

static void test_bfp_s32_sum()
{
    PRINTF("%s...\t(random vectors)\n", __func__);

    seed = 8789;

    int32_t dataB[MAX_LEN];
    bfp_s32_t B;

    B.data = dataB;

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        test_random_bfp_s32(&B, MAX_LEN, &seed, NULL, 0);

        float_s64_t result = bfp_s32_sum(&B);

        float_s64_t expected = {
            .mant = 0,
            .exp = B.exp };
            
        for(int i = 0; i < B.length; i++)
            expected.mant += B.data[i];

        TEST_ASSERT_EQUAL(expected.exp, result.exp);
        TEST_ASSERT(expected.mant == result.mant);
    }
}




void test_bfp_sum()
{
    SET_TEST_FILE();
    RUN_TEST(test_bfp_s16_sum);
    RUN_TEST(test_bfp_s32_sum);
}