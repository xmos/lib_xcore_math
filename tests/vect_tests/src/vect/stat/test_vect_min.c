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


TEST_GROUP_RUNNER(vect_min) {
  RUN_TEST_CASE(vect_min, vect_s16_min_random);
  RUN_TEST_CASE(vect_min, vect_s32_min_random);
}

TEST_GROUP(vect_min);
TEST_SETUP(vect_min) { fflush(stdout); }
TEST_TEAR_DOWN(vect_min) {}



#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (64)
#else
#  define REPS       (1000)
#  define MAX_LEN    (256)
#endif


TEST(vect_min, vect_s16_min_random)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    int16_t result;
    int16_t B[MAX_LEN];

    for(unsigned int v = 0; v < REPS; v++){

        unsigned len = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        // len = 32;

        setExtraInfo_RSL(v, seed, len);

        int16_t min = 0x7FFF;

        for(unsigned int i = 0; i < len; i++){
            B[i] = pseudo_rand_int16(&seed);
            min = MIN(min, B[i]);
        }

        result = vect_s16_min(B, len);

        TEST_ASSERT_EQUAL(min, result);
    }
}


TEST(vect_min, vect_s32_min_random)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    int32_t result;
    int32_t B[MAX_LEN];

    for(unsigned int v = 0; v < REPS; v++){

        unsigned len = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        // len = 1;

        setExtraInfo_RSL(v, seed, len);

        int32_t min = 0x7FFFFFFF;

        for(unsigned int i = 0; i < len; i++){
            // B[i] = pseudo_rand_int32(&seed);
            // B[i] = i;
            min = MIN(min, B[i]);
        }

        result = vect_s32_min(B, len);

        TEST_ASSERT_EQUAL(min, result);
        
    }
}


