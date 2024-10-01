// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "xmath/xmath.h"

#include "../../tst_common.h"
#include "xmath/xs3/vpu_scalar_ops.h"

#include "unity_fixture.h"


TEST_GROUP_RUNNER(bfp_sqrt) {
  RUN_TEST_CASE(bfp_sqrt, bfp_s16_sqrt);
  RUN_TEST_CASE(bfp_sqrt, bfp_s32_sqrt);
}

TEST_GROUP(bfp_sqrt);
TEST_SETUP(bfp_sqrt) { fflush(stdout); }
TEST_TEAR_DOWN(bfp_sqrt) {}

#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (128)
#else
#  define REPS       (1000)
#  define MAX_LEN    (512)
#endif

TEST(bfp_sqrt, bfp_s16_sqrt)
{
    unsigned seed = SEED_FROM_FUNC_NAME();


    int16_t WORD_ALIGNED A_data[MAX_LEN];
    int16_t WORD_ALIGNED B_data[MAX_LEN];

    bfp_s16_t A, B;

    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        bfp_s16_init(&B, B_data, 
            pseudo_rand_int(&seed, -30, 30),
            pseudo_rand_uint(&seed, 1, MAX_LEN-1), 0);

        bfp_s16_init(&A, A_data, 0, B.length, 0);

        B.hr = pseudo_rand_uint(&seed, 0, 4);

        for(unsigned int i = 0; i < B.length; i++){
            B_data[i] = pseudo_rand_uint(&seed, INT16_MAX / 10, INT16_MAX) >> B.hr;
        }

        bfp_s16_headroom(&B);

        bfp_s16_sqrt(&A, &B);

        TEST_ASSERT_EQUAL(bfp_s16_headroom(&A), A.hr);

        
        for(unsigned int i = 0; i < B.length; i++){

            double b_val = ldexp(B.data[i], B.exp);
            double a_val = ldexp(A.data[i], A.exp);

            double exp_val = sqrt(b_val);

            double diff = exp_val - a_val;

            TEST_ASSERT(  fabs(diff) <= ldexp(2, A.exp) );
        }
    }
}


TEST(bfp_sqrt, bfp_s32_sqrt)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    int32_t A_data[MAX_LEN];
    int32_t B_data[MAX_LEN];

    bfp_s32_t A, B;

    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        bfp_s32_init(&B, B_data, 
            pseudo_rand_int(&seed, -30, 30),
            pseudo_rand_uint(&seed, 1, MAX_LEN-1), 0);

        bfp_s32_init(&A, A_data, 0, B.length, 0);

        B.hr = pseudo_rand_uint(&seed, 0, 28);

        for(unsigned int i = 0; i < B.length; i++){
            B_data[i] = pseudo_rand_uint(&seed, INT32_MAX/10, INT32_MAX) >> B.hr;
        }

        bfp_s32_headroom(&B);

        bfp_s32_sqrt(&A, &B);

        TEST_ASSERT_EQUAL(bfp_s32_headroom(&A), A.hr);
        
        for(unsigned int i = 0; i < B.length; i++){
            double b_val = ldexp(B.data[i], B.exp);
            double a_val = ldexp(A.data[i], A.exp);

            double exp_val = sqrt(b_val);

            double diff = exp_val - a_val;

            TEST_ASSERT(  fabs(diff) <= ldexp(2, A.exp) );
        }
    }
}
