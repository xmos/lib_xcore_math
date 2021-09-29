// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "xs3_math.h"

#include "../tst_common.h"

#include "unity_fixture.h"

TEST_GROUP_RUNNER(xs3_sXX_inverse) {
  RUN_TEST_CASE(xs3_sXX_inverse, xs3_s16_inverse);
  RUN_TEST_CASE(xs3_sXX_inverse, xs3_s32_inverse);
}

TEST_GROUP(xs3_sXX_inverse);
TEST_SETUP(xs3_sXX_inverse) { fflush(stdout); }
TEST_TEAR_DOWN(xs3_sXX_inverse) {}

#if SMOKE_TEST
#  define REPS       (100)
#else
#  define REPS       (1000)
#endif

TEST(xs3_sXX_inverse, xs3_s16_inverse)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();

    
    for(int v = 0; v < REPS; v++){

        setExtraInfo_RS(v, seed);

        headroom_t b_hr = pseudo_rand_uint(&seed, 0, 12);
        
        int16_t B = 0;
        
        do {
            B = pseudo_rand_int16(&seed) >> b_hr;
        } while (B == 0);

        exponent_t a_exp;

        int16_t A = xs3_s16_inverse(&a_exp, B);


        double Bf = ldexp(B, 0);

        double Bf_inv = 1.0 / Bf;

        // We should not end up with much headroom
        TEST_ASSERT_LESS_OR_EQUAL(2, HR_S16(A));

        int16_t expected = floor( ldexp(Bf_inv, -a_exp) );

        TEST_ASSERT_INT16_WITHIN(2, expected, A);

    }
}

TEST(xs3_sXX_inverse, xs3_s32_inverse)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();

    
    for(int v = 0; v < REPS; v++){

        setExtraInfo_RS(v, seed);

        headroom_t b_hr = pseudo_rand_uint(&seed, 0, 28);
        
        int32_t B = 0;
        
        do {
            B = pseudo_rand_int32(&seed) >> b_hr;
        } while (B == 0);

        exponent_t a_exp;

        int32_t A = xs3_s32_inverse(&a_exp, B);


        double Bf = ldexp(B, 0);

        double Bf_inv = 1.0 / Bf;

        // We should not end up with much headroom
        TEST_ASSERT_LESS_OR_EQUAL(2, HR_S32(A));

        int32_t expected = floor( ldexp(Bf_inv, -a_exp) );

        TEST_ASSERT_INT32_WITHIN(2, expected, A);

    }
}

