// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "xmath/xmath.h"

#include "../../tst_common.h"

#include "unity_fixture.h"


TEST_GROUP_RUNNER(vect_max) {
  RUN_TEST_CASE(vect_max, vect_s16_max_random);
  RUN_TEST_CASE(vect_max, vect_s32_max_random);
}

TEST_GROUP(vect_max);
TEST_SETUP(vect_max) { fflush(stdout); }
TEST_TEAR_DOWN(vect_max) {}



#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (64)
#else
#  define REPS       (1000)
#  define MAX_LEN    (256)
#endif


TEST(vect_max, vect_s16_max_random)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    int16_t result;
    int16_t B[MAX_LEN];

    for(unsigned int v = 0; v < REPS; v++){

        unsigned len = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        // len = 32;

        setExtraInfo_RSL(v, seed, len);

        int16_t max = -0x8000;

        for(unsigned int i = 0; i < len; i++){
            B[i] = pseudo_rand_int16(&seed);
            max = MAX(max, B[i]);
        }

        result = vect_s16_max(B, len);

        TEST_ASSERT_EQUAL(max, result);
    }
}


TEST(vect_max, vect_s32_max_random)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    int32_t result;
    int32_t B[MAX_LEN];

    for(unsigned int v = 0; v < REPS; v++){

        unsigned len = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        // len = 8;

        setExtraInfo_RSL(v, seed, len);

        int32_t max = (int) (0-0x80000000);

        for(unsigned int i = 0; i < len; i++){
            B[i] = pseudo_rand_int32(&seed);
            max = MAX(max, B[i]);
        }

        result = vect_s32_max(B, len);

        TEST_ASSERT_EQUAL(max, result);
        
    }
}


