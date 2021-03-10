// Copyright 2020 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public License: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "bfp_math.h"

#include "../tst_common.h"
#include "xs3_vpu_scalar_ops.h"

#include "unity.h"

static unsigned seed = 2314567;

static char msg_buff[200];


#if DEBUG_ON || 0
#undef DEBUG_ON
#define DEBUG_ON    (1)
#endif


#define MAX_LEN     30
#define REPS        30




static void test_bfp_s16_inverse()
{

    PRINTF("%s...\n", __func__);
    seed = 0xF80C98BE;

    int16_t WORD_ALIGNED B_data[MAX_LEN];
    int16_t WORD_ALIGNED A_data[MAX_LEN];


    for(int v = 0; v < REPS; v++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", v, seed);

        bfp_s16_t A, B;

        bfp_s16_init(&B, B_data, 
                          pseudo_rand_int(&seed, -30, 30),
                          pseudo_rand_uint(&seed, 0, MAX_LEN-1), 0);
        bfp_s16_init(&A, A_data, 0, B.length, 0);

        B.hr = pseudo_rand_uint(&seed, 0, 12);

        for(int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int16(&seed) >> B.hr;
            if( B.data[i] == 0 )
                B.data[i] = 1;
        }

        bfp_s16_inverse(&A, &B);

        double expected_flt[MAX_LEN];

        for(int i = 0; i < B.length; i++){
            expected_flt[i] = 1.0 / ldexp(B.data[i], B.exp);
        }

        int16_t expected[MAX_LEN];

        test_s16_from_double(expected, expected_flt, A.length, A.exp);

        for(int i = 0; i < B.length; i++){
            TEST_ASSERT_INT16_WITHIN(2, expected[i], A.data[i]);
        }
    }
}



static void test_bfp_s32_inverse()
{
    PRINTF("%s...\n", __func__);
    seed = 47685;

    int32_t B_data[MAX_LEN];
    int32_t A_data[MAX_LEN];


    for(int v = 0; v < REPS; v++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", v, seed);

        bfp_s32_t A, B;

        bfp_s32_init(&B, B_data, 
                          pseudo_rand_int(&seed, -30, 30),
                          pseudo_rand_uint(&seed, 0, MAX_LEN-1), 0);
        bfp_s32_init(&A, A_data, 0, B.length, 0);

        B.hr = pseudo_rand_uint(&seed, 0, 28);

        for(int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int32(&seed) >> B.hr;
            if( B.data[i] == 0 )
                B.data[i] = 1;
        }

        bfp_s32_inverse(&A, &B);

        double expected_flt[MAX_LEN];

        for(int i = 0; i < B.length; i++){
            expected_flt[i] = 1.0 / ldexp(B.data[i], B.exp);
        }

        int32_t expected[MAX_LEN];

        test_s32_from_double(expected, expected_flt, A.length, A.exp);

        for(int i = 0; i < B.length; i++){
            TEST_ASSERT_INT32_WITHIN(2, expected[i], A.data[i]);
        }
    }
}




void test_bfp_inverse_vect()
{
    SET_TEST_FILE();
    
    RUN_TEST(test_bfp_s16_inverse);
    RUN_TEST(test_bfp_s32_inverse);

}