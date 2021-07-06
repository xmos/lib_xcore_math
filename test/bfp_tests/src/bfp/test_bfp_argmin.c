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

TEST_GROUP_RUNNER(bfp_argmin) {
  RUN_TEST_CASE(bfp_argmin, bfp_s16_argmin);
  RUN_TEST_CASE(bfp_argmin, bfp_s32_argmin);
}
TEST_GROUP(bfp_argmin);
TEST_SETUP(bfp_argmin) {}
TEST_TEAR_DOWN(bfp_argmin) {}

#if DEBUG_ON || 0
#undef DEBUG_ON
#define DEBUG_ON    (1)
#endif


#define REPS        1000
#define MAX_LEN     1024 


static unsigned seed = 666;




TEST(bfp_argmin, bfp_s16_argmin)
{
    PRINTF("%s...\t(random vectors)\n", __func__);
    seed = 8566;
    int16_t dataB[MAX_LEN];
    bfp_s16_t B;
    B.data = dataB;
    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);
        B.length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        B.exp = pseudo_rand_int(&seed, -5, 5);
        B.hr = pseudo_rand_uint(&seed, 0, 15);
        unsigned exp = 0;
        for(int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int16(&seed) >> B.hr;
            exp = (B.data[exp] <= B.data[i])? exp : i;
        }
        bfp_s16_headroom(&B);
        unsigned result = bfp_s16_argmin(&B);
        TEST_ASSERT_EQUAL(exp, result);
    }
}



TEST(bfp_argmin, bfp_s32_argmin)
{
    PRINTF("%s...\t(random vectors)\n", __func__);
    unsigned seed = 0x648E34A3;
    int32_t dataB[MAX_LEN];
    bfp_s32_t B;
    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);
        bfp_s32_init(&B, dataB, pseudo_rand_int(&seed, -5, 5),
                            pseudo_rand_uint(&seed, 1, MAX_LEN+1), 0);
        B.hr = pseudo_rand_uint(&seed, 0, 28);
        unsigned exp = 0;
        for(int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int32(&seed) >> B.hr;
            exp = (B.data[exp] <= B.data[i])? exp : i;
        }
        bfp_s32_headroom(&B);
        unsigned result = bfp_s32_argmin(&B);
        TEST_ASSERT_EQUAL(exp, result);
    }
}
