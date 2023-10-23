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


TEST_GROUP_RUNNER(bfp_argmax) {
  RUN_TEST_CASE(bfp_argmax, bfp_s16_argmax);
  RUN_TEST_CASE(bfp_argmax, bfp_s32_argmax);
}
TEST_GROUP(bfp_argmax);
TEST_SETUP(bfp_argmax) { fflush(stdout); }
TEST_TEAR_DOWN(bfp_argmax) {}


#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (128)
#else
#  define REPS       (1000)
#  define MAX_LEN    (512)
#endif


TEST(bfp_argmax, bfp_s16_argmax)
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
        unsigned exp = 0;
        for(unsigned int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int16(&seed) >> B.hr;
            exp = (B.data[exp] >= B.data[i])? exp : i;
        }
        bfp_s16_headroom(&B);
        unsigned result = bfp_s16_argmax(&B);

        if(result != exp){
            printf("B.data[%u] = %d //expected\n", exp, B.data[exp]);
            printf("B.data[%u] = %d //result\n", result, B.data[result]);
        }
        TEST_ASSERT_EQUAL(exp, result);
    }
}

TEST(bfp_argmax, bfp_s32_argmax)
{

    unsigned seed = SEED_FROM_FUNC_NAME();
    int32_t dataB[MAX_LEN];
    bfp_s32_t B;
    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);
        bfp_s32_init(&B, dataB, pseudo_rand_int(&seed, -5, 5),
                            pseudo_rand_uint(&seed, 1, MAX_LEN+1), 0);
        B.hr = pseudo_rand_uint(&seed, 0, 28);
        unsigned exp = 0;
        for(unsigned int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int32(&seed) >> B.hr;
            exp = (B.data[exp] >= B.data[i])? exp : i;
        }
        bfp_s32_headroom(&B);
        unsigned result = bfp_s32_argmax(&B);
        TEST_ASSERT_EQUAL(exp, result);
    }
}