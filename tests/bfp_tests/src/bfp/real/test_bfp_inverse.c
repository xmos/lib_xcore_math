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


TEST_GROUP_RUNNER(bfp_inverse) {
  RUN_TEST_CASE(bfp_inverse, bfp_s16_inverse);
  RUN_TEST_CASE(bfp_inverse, bfp_s32_inverse);
}

TEST_GROUP(bfp_inverse);
TEST_SETUP(bfp_inverse) { fflush(stdout); }
TEST_TEAR_DOWN(bfp_inverse) {}

// static char msg_buff[200];

#define MAX_LEN     30
#define REPS        30


TEST(bfp_inverse, bfp_s16_inverse)
{


    unsigned seed = SEED_FROM_FUNC_NAME();

    int16_t WORD_ALIGNED B_data[MAX_LEN];
    int16_t WORD_ALIGNED A_data[MAX_LEN];


    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        bfp_s16_t A, B;

        bfp_s16_init(&B, B_data,
                          pseudo_rand_int(&seed, -30, 30),
                          pseudo_rand_uint(&seed, 0, MAX_LEN-1), 0);
        bfp_s16_init(&A, A_data, 0, B.length, 0);

        B.hr = pseudo_rand_uint(&seed, 0, 12);

        for(unsigned int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int16(&seed) >> B.hr;
            if( B.data[i] == 0 )
                B.data[i] = 1;
        }

        bfp_s16_inverse(&A, &B);

        double expected_flt[MAX_LEN];

        for(unsigned int i = 0; i < B.length; i++){
            expected_flt[i] = 1.0 / ldexp(B.data[i], B.exp);
        }

        int16_t expected[MAX_LEN];

        test_s16_from_double(expected, expected_flt, A.length, A.exp);

        for(unsigned int i = 0; i < B.length; i++){
            TEST_ASSERT_INT16_WITHIN(2, expected[i], A.data[i]);
        }
    }
}


TEST(bfp_inverse, bfp_s32_inverse)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    int32_t B_data[MAX_LEN];
    int32_t A_data[MAX_LEN];


    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        bfp_s32_t A, B;

        bfp_s32_init(&B, B_data,
                          pseudo_rand_int(&seed, -30, 30),
                          pseudo_rand_uint(&seed, 0, MAX_LEN-1), 0);
        bfp_s32_init(&A, A_data, 0, B.length, 0);

        B.hr = pseudo_rand_uint(&seed, 0, 28);

        for(unsigned int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int32(&seed) >> B.hr;
            if( B.data[i] == 0 )
                B.data[i] = 1;
        }

        bfp_s32_inverse(&A, &B);

        double expected_flt[MAX_LEN];

        for(unsigned int i = 0; i < B.length; i++){
            expected_flt[i] = 1.0 / ldexp(B.data[i], B.exp);
        }

        int32_t expected[MAX_LEN];

        test_s32_from_double(expected, expected_flt, A.length, A.exp);

        for(unsigned int i = 0; i < B.length; i++){
            TEST_ASSERT_INT32_WITHIN(2, expected[i], A.data[i]);
        }
    }
}
