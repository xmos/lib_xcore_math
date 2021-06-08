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

#include "unity.h"

static unsigned seed = 2314567;



#if DEBUG_ON || 0
#undef DEBUG_ON
#define DEBUG_ON    (1)
#endif




#define REPS        1000

static void test_xs3_mul_s32_A()
{
    PRINTF("%s...\n", __func__);
    seed = 0x92B7BD9A;
    
    for(int v = 0; v < REPS; v++){

        PRINTF("\trepetition %d.. (seed: 0x%08X)\n", v, seed);

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






void test_xs3_mul_s32()
{
    SET_TEST_FILE();

    RUN_TEST(test_xs3_mul_s32_A);
}
