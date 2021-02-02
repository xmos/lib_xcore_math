// Copyright 2021 XMOS LIMITED. This Software is subject to the terms of the 
// XMOS Public License: Version 1

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "xs3_math.h"

#include "../tst_common.h"

#include "unity.h"

static unsigned seed = 2314567;

static char msg_buff[200];


#if DEBUG_ON || 0
#undef DEBUG_ON
#define DEBUG_ON    (1)
#endif


#define N        1000

static void test_xs3_sqrt_s32_A()
{

    PRINTF("%s...\n", __func__);
    seed = 3465;


    int32_t cases[] = { 0, 1, 4, 16, 100, 0x4000 };

    const unsigned N_cases = sizeof(cases) / sizeof(int32_t);

    for(int i = 0; i < N_cases; i++){

        exponent_t b_exp = 0, a_exp;

        int32_t res = xs3_sqrt_s32(&a_exp, cases[i], b_exp, 31);

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


void test_xs3_sqrt_s32()
{
    SET_TEST_FILE();
    
    RUN_TEST(test_xs3_sqrt_s32_A);
}