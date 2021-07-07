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

TEST_GROUP_RUNNER(xs3_inverse_s32) {
  RUN_TEST_CASE(xs3_inverse_s32, xs3_inverse_s32);
}

TEST_GROUP(xs3_inverse_s32);
TEST_SETUP(xs3_inverse_s32) {}
TEST_TEAR_DOWN(xs3_inverse_s32) {}


#define REPS        10000

TEST(xs3_inverse_s32, xs3_inverse_s32)
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

        int32_t A = xs3_inverse_s32(&a_exp, B);


        double Bf = ldexp(B, 0);

        double Bf_inv = 1.0 / Bf;

        // We should not end up with much headroom
        TEST_ASSERT_LESS_OR_EQUAL(2, HR_S32(A));

        int32_t expected = floor( ldexp(Bf_inv, -a_exp) );

        TEST_ASSERT_INT32_WITHIN(2, expected, A);

    }
}

