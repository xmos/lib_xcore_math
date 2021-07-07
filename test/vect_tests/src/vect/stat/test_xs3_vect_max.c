// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "xs3_math.h"

#include "../../tst_common.h"

#include "unity_fixture.h"


TEST_GROUP_RUNNER(xs3_vect_max) {
  RUN_TEST_CASE(xs3_vect_max, xs3_vect_s16_max_random);
  RUN_TEST_CASE(xs3_vect_max, xs3_vect_s32_max_random);
}

TEST_GROUP(xs3_vect_max);
TEST_SETUP(xs3_vect_max) {}
TEST_TEAR_DOWN(xs3_vect_max) {}


#define MAX_LEN     256
#define REPS        1000


TEST(xs3_vect_max, xs3_vect_s16_max_random)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    int16_t result;
    int16_t B[MAX_LEN];

    for(int v = 0; v < REPS; v++){

        unsigned len = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        // len = 32;

        setExtraInfo_RSL(v, seed, len);

        int16_t max = -0x8000;

        for(int i = 0; i < len; i++){
            B[i] = pseudo_rand_int16(&seed);
            max = MAX(max, B[i]);
        }

        result = xs3_vect_s16_max(B, len);

        TEST_ASSERT_EQUAL(max, result);
    }
}


TEST(xs3_vect_max, xs3_vect_s32_max_random)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    int32_t result;
    int32_t B[MAX_LEN];

    for(int v = 0; v < REPS; v++){

        unsigned len = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        // len = 8;

        setExtraInfo_RSL(v, seed, len);

        int32_t max = -0x80000000;

        for(int i = 0; i < len; i++){
            B[i] = pseudo_rand_int32(&seed);
            max = MAX(max, B[i]);
        }

        result = xs3_vect_s32_max(B, len);

        TEST_ASSERT_EQUAL(max, result);
        
    }
}


