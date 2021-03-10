// Copyright 2020 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public License: Version 1.

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
static char msg_buff[200];


#define TEST_ASSERT_EQUAL_MSG(EXPECTED, ACTUAL, LINE_NUM)   do{       \
    if((EXPECTED)!=(ACTUAL)) {                                        \
      sprintf(msg_buff, "(test vector @ line %u)", (LINE_NUM));       \
      TEST_ASSERT_EQUAL_MESSAGE((EXPECTED), (ACTUAL), msg_buff);      \
    }} while(0)

#if DEBUG_ON || 0
#undef DEBUG_ON
#define DEBUG_ON    (1)
#endif




#define REPS        10000

static void test_xs3_inverse_s32_A()
{
    PRINTF("%s...\n", __func__);
    seed = 0x92B7BD9A;
    
    for(int v = 0; v < REPS; v++){

        PRINTF("\trepetition %d.. (seed: 0x%08X)\n", v, seed);

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






void test_xs3_inverse_s32()
{
    SET_TEST_FILE();

    RUN_TEST(test_xs3_inverse_s32_A);
}