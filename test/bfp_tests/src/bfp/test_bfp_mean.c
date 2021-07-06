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


TEST_GROUP_RUNNER(bfp_mean) {
  RUN_TEST_CASE(bfp_mean, bfp_s16_mean);
  RUN_TEST_CASE(bfp_mean, bfp_s32_mean);
}

TEST_GROUP(bfp_mean);
TEST_SETUP(bfp_mean) {}
TEST_TEAR_DOWN(bfp_mean) {}

#if DEBUG_ON || 0
#undef DEBUG_ON
#define DEBUG_ON    (1)
#endif


#define REPS        1000
#define MAX_LEN     1024 


static unsigned seed = 666;


TEST(bfp_mean, bfp_s16_mean)
{
    PRINTF("%s...\t(random vectors)\n", __func__);

    seed = 0xE3AEA12B;

    int16_t dataB[MAX_LEN];
    bfp_s16_t B;

    B.data = dataB;

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        B.length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        B.exp = pseudo_rand_int(&seed, -100, 100);
        B.hr = pseudo_rand_uint(&seed, 0, 15);


        double sum = 0;

        for(int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int16(&seed) >> B.hr;

            sum += B.data[i];
        }

        bfp_s16_headroom(&B);

        float_s16_t result = bfp_s16_mean(&B);

        double meanf = ldexp(sum, B.exp) / B.length;
        
        double diff = meanf - ldexp(result.mant, result.exp);

        TEST_ASSERT( fabs(diff) <= ldexp(1, result.exp) );
    }
}

TEST(bfp_mean, bfp_s32_mean)
{
    PRINTF("%s...\t(random vectors)\n", __func__);

    seed = 44;

    int32_t dataB[MAX_LEN];
    bfp_s32_t B;

    B.data = dataB;

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        B.length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        B.exp = pseudo_rand_int(&seed, -100, 100);
        B.hr = pseudo_rand_uint(&seed, 0, 31);

        double sum = 0;

        for(int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int32(&seed) >> B.hr;

            sum += B.data[i];
        }

        bfp_s32_headroom(&B);

        float_s32_t result = bfp_s32_mean(&B);
        double meanf = ldexp(sum, B.exp) / B.length;
        double diff = meanf - ldexp(result.mant, result.exp);

        TEST_ASSERT( fabs(diff) <= ldexp(1, result.exp) );
    }
}
