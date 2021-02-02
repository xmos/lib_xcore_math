// Copyright 2021 XMOS LIMITED. This Software is subject to the terms of the 
// XMOS Public License: Version 1

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


#define MAX_LEN     100
#define REPS        1000




static void test_bfp_s16_sqrt()
{

    PRINTF("%s...\n", __func__);
    seed = 11;


    int16_t WORD_ALIGNED A_data[MAX_LEN];
    int16_t WORD_ALIGNED B_data[MAX_LEN];

    bfp_s16_t A, B;

    for(int v = 0; v < REPS; v++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", v, seed);

        bfp_s16_init(&B, B_data, 
            pseudo_rand_int(&seed, -30, 30),
            pseudo_rand_uint(&seed, 1, MAX_LEN-1), 0);

        bfp_s16_init(&A, A_data, 0, B.length, 0);

        B.hr = pseudo_rand_uint(&seed, 0, 4);

        for(int i = 0; i < B.length; i++){
            B_data[i] = pseudo_rand_uint(&seed, INT16_MAX / 10, INT16_MAX) >> B.hr;
        }

        bfp_s16_headroom(&B);

        bfp_s16_sqrt(&A, &B);

        TEST_ASSERT_EQUAL(bfp_s16_headroom(&A), A.hr);

        
        for(int i = 0; i < B.length; i++){

            double b_val = ldexp(B.data[i], B.exp);
            double a_val = ldexp(A.data[i], A.exp);

            double exp_val = sqrt(b_val);

            double diff = exp_val - a_val;

            TEST_ASSERT(  fabs(diff) <= ldexp(2, A.exp) );
        }
    }
}


static void test_bfp_s32_sqrt()
{

    PRINTF("%s...\n", __func__);
    seed = 3452;

    int32_t A_data[MAX_LEN];
    int32_t B_data[MAX_LEN];

    bfp_s32_t A, B;

    for(int v = 0; v < REPS; v++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", v, seed);

        bfp_s32_init(&B, B_data, 
            pseudo_rand_int(&seed, -30, 30),
            pseudo_rand_uint(&seed, 1, MAX_LEN-1), 0);

        bfp_s32_init(&A, A_data, 0, B.length, 0);

        B.hr = pseudo_rand_uint(&seed, 0, 28);

        for(int i = 0; i < B.length; i++){
            B_data[i] = pseudo_rand_uint(&seed, INT32_MAX/10, INT32_MAX) >> B.hr;
        }

        bfp_s32_headroom(&B);

        bfp_s32_sqrt(&A, &B);

        TEST_ASSERT_EQUAL(bfp_s32_headroom(&A), A.hr);
        
        for(int i = 0; i < B.length; i++){
            double b_val = ldexp(B.data[i], B.exp);
            double a_val = ldexp(A.data[i], A.exp);

            double exp_val = sqrt(b_val);

            double diff = exp_val - a_val;

            TEST_ASSERT(  fabs(diff) <= ldexp(2, A.exp) );
        }
    }
}





void test_bfp_sqrt_vect()
{
    SET_TEST_FILE();
    
    RUN_TEST(test_bfp_s16_sqrt);
    RUN_TEST(test_bfp_s32_sqrt);

}