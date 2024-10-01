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

TEST_GROUP_RUNNER(s32_sqrt) {
  RUN_TEST_CASE(s32_sqrt, s32_sqrt);
}

TEST_GROUP(s32_sqrt);
TEST_SETUP(s32_sqrt) { fflush(stdout); }
TEST_TEAR_DOWN(s32_sqrt) {}



#if SMOKE_TEST
#  define N       (100)
#else
#  define N       (1000)
#endif

TEST(s32_sqrt, s32_sqrt)
{
    int32_t cases[] = { 0, 1, 4, 16, 100, 0x4000 };

    const unsigned N_cases = sizeof(cases) / sizeof(int32_t);

    for(unsigned int i = 0; i < N_cases; i++){

        exponent_t b_exp = 0, a_exp;

        int32_t res = s32_sqrt(&a_exp, cases[i], b_exp, 31);

        int64_t p = ((int64_t)res) * res;
        p = p >> 30;

        // printf("%d: sqrt( %ld   (0x%08X) ) -->  %ld    (0x%08X)\n", i, cases[i], (unsigned)cases[i], res, (unsigned)res);
        // printf("    a_exp: %d\n", a_exp);


        double B = ldexp(cases[i], b_exp);
        double A = ldexp(res, a_exp);

        double diff = B - pow(A, 2);

        // printf("%d:  (%f)^2  =  %f\n\n", i, A, B);

        TEST_ASSERT( fabs(diff) < ldexp(1, -10) );
    }


    
}

