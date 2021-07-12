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


TEST_GROUP_RUNNER(xs3_vect_min) {
  RUN_TEST_CASE(xs3_vect_min, xs3_vect_s16_min_random);
  RUN_TEST_CASE(xs3_vect_min, xs3_vect_s32_min_random);
}

TEST_GROUP(xs3_vect_min);
TEST_SETUP(xs3_vect_min) {}
TEST_TEAR_DOWN(xs3_vect_min) {}


#define MAX_LEN     256
#define REPS        1000


TEST(xs3_vect_min, xs3_vect_s16_min_random)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    int16_t result;
    int16_t B[MAX_LEN];

    for(int v = 0; v < REPS; v++){

        unsigned len = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        // len = 32;

        setExtraInfo_RSL(v, seed, len);

        int16_t min = 0x7FFF;

        for(int i = 0; i < len; i++){
            B[i] = pseudo_rand_int16(&seed);
            min = MIN(min, B[i]);
        }

        result = xs3_vect_s16_min(B, len);

        TEST_ASSERT_EQUAL(min, result);
    }
}


TEST(xs3_vect_min, xs3_vect_s32_min_random)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    int32_t result;
    int32_t B[MAX_LEN];

    for(int v = 0; v < REPS; v++){

        unsigned len = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        // len = 1;

        setExtraInfo_RSL(v, seed, len);

        int32_t min = 0x7FFFFFFF;

        for(int i = 0; i < len; i++){
            // B[i] = pseudo_rand_int32(&seed);
            // B[i] = i;
            min = MIN(min, B[i]);
        }

        result = xs3_vect_s32_min(B, len);

        TEST_ASSERT_EQUAL(min, result);
        
    }
}


