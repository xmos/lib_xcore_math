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

TEST_GROUP_RUNNER(xs3_mul_s32) {
  RUN_TEST_CASE(xs3_mul_s32, xs3_mul_s32);
}

TEST_GROUP(xs3_mul_s32);
TEST_SETUP(xs3_mul_s32) {}
TEST_TEAR_DOWN(xs3_mul_s32) {}


#define REPS        1000

TEST(xs3_mul_s32, xs3_mul_s32)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();

    
    for(int v = 0; v < REPS; v++){

        setExtraInfo_RS(v, seed);

        const exponent_t b_exp = pseudo_rand_int(&seed, -100, 100);
        const exponent_t c_exp = pseudo_rand_int(&seed, -100, 100);

        const headroom_t b_hr = pseudo_rand_uint(&seed, 0, 28);
        const headroom_t c_hr = pseudo_rand_uint(&seed, 0, 28);
        
        int32_t B = 0;
        int32_t C = 0;
        
        do { B = pseudo_rand_int32(&seed) >> b_hr; } while (B == 0);
        do { C = pseudo_rand_int32(&seed) >> c_hr; } while (C == 0);

        exponent_t a_exp;
        int32_t A = xs3_mul_s32(&a_exp, B, C, b_exp, c_exp);
        
        double Bf = ldexp(B, b_exp);
        double Cf = ldexp(C, c_exp);

        double Ef = Bf * Cf;

        int32_t expected = round( ldexp(Ef, -a_exp) );


        TEST_ASSERT_INT32_WITHIN(2, expected, A);

    }
}

