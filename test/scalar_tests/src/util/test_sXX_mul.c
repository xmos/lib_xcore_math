// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "xmath/xmath.h"

#include "../tst_common.h"

#include "unity_fixture.h"

TEST_GROUP_RUNNER(sXX_mul) {
  RUN_TEST_CASE(sXX_mul, s16_mul);
  RUN_TEST_CASE(sXX_mul, s32_mul);
}

TEST_GROUP(sXX_mul);
TEST_SETUP(sXX_mul) { fflush(stdout); }
TEST_TEAR_DOWN(sXX_mul) {}


#if SMOKE_TEST
#  define REPS       (100)
#else
#  define REPS       (1000)
#endif

TEST(sXX_mul, s16_mul)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();

    
    for(unsigned int v = 0; v < REPS; v++){

        setExtraInfo_RS(v, seed);

        const exponent_t b_exp = pseudo_rand_int(&seed, -100, 100);
        const exponent_t c_exp = pseudo_rand_int(&seed, -100, 100);

        const headroom_t b_hr = pseudo_rand_uint(&seed, 0, 12);
        const headroom_t c_hr = pseudo_rand_uint(&seed, 0, 12);
        
        int16_t B = 0;
        int16_t C = 0;
        
        do { B = pseudo_rand_int16(&seed) >> b_hr; } while (B == 0);
        do { C = pseudo_rand_int16(&seed) >> c_hr; } while (C == 0);

        exponent_t a_exp;
        int16_t A = s16_mul(&a_exp, B, C, b_exp, c_exp);
        
        double Bf = ldexp(B, b_exp);
        double Cf = ldexp(C, c_exp);

        double Ef = Bf * Cf;

        int16_t expected = (int16_t) lround( ldexp(Ef, -a_exp) );


        TEST_ASSERT_INT16_WITHIN_MESSAGE(2, expected, A, "");

    }
}

TEST(sXX_mul, s32_mul)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();

    
    for(unsigned int v = 0; v < REPS; v++){

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
        int32_t A = s32_mul(&a_exp, B, C, b_exp, c_exp);
        
        double Bf = ldexp(B, b_exp);
        double Cf = ldexp(C, c_exp);

        double Ef = Bf * Cf;

        int32_t expected = lround( ldexp(Ef, -a_exp) );


        TEST_ASSERT_INT32_WITHIN_MESSAGE(2, expected, A, "");

    }
}

