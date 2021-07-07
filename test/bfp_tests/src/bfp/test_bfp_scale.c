// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "bfp_math.h"

#include "../tst_common.h"

#include "unity_fixture.h"


TEST_GROUP_RUNNER(bfp_scale) {
  RUN_TEST_CASE(bfp_scale, bfp_s32_scale);
  RUN_TEST_CASE(bfp_scale, bfp_s16_scale);
}

TEST_GROUP(bfp_scale);
TEST_SETUP(bfp_scale) {}
TEST_TEAR_DOWN(bfp_scale) {}

#define REPS        1000
#define MAX_LEN     256


static char msg_buff[200];

#define TEST_ASSERT_EQUAL_MSG(EXPECTED, ACTUAL, EXTRA, LINE_NUM)   do{          \
    if((EXPECTED)!=(ACTUAL)) {                                                  \
      sprintf(msg_buff, "%s (test vector @ line %u)", (EXTRA), (LINE_NUM));     \
      TEST_ASSERT_EQUAL_MESSAGE((EXPECTED), (ACTUAL), msg_buff);                \
    }} while(0)


TEST(bfp_scale, bfp_s16_scale)
{


    unsigned seed = SEED_FROM_FUNC_NAME();

    int16_t WORD_ALIGNED dataA[MAX_LEN];
    int16_t WORD_ALIGNED dataB[MAX_LEN];
    int16_t WORD_ALIGNED expA[MAX_LEN];
    bfp_s16_t A, B;

    A.data = dataA;
    B.data = dataB;

    double Af[MAX_LEN];
    double Bf[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        test_random_bfp_s16(&B, MAX_LEN, &seed, &A, 0);
        int16_t alpha_mant = pseudo_rand_int16(&seed);
        exponent_t alpha_exp = pseudo_rand_int16(&seed) % 30;

        test_double_from_s16(Bf, &B);
        double alpha_f = ldexp(alpha_mant, alpha_exp);

        for(int i = 0; i < B.length; i++){
            Af[i] = Bf[i] * alpha_f;
        }

        float_s16_t alpha = {alpha_mant, alpha_exp};
        
        bfp_s16_scale(&A, &B, alpha);

        test_s16_from_double(expA, Af, MAX_LEN, A.exp);

        for(int i = 0; i < A.length; i++){
            TEST_ASSERT_INT16_WITHIN(1, expA[i], A.data[i]);
        }
    }
}


TEST(bfp_scale, bfp_s32_scale)
{


    unsigned seed = SEED_FROM_FUNC_NAME();

    int32_t dataA[MAX_LEN];
    int32_t dataB[MAX_LEN];
    int32_t expA[MAX_LEN];
    bfp_s32_t A, B;

    A.data = dataA;
    B.data = dataB;

    double Af[MAX_LEN];
    double Bf[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        test_random_bfp_s32(&B, MAX_LEN, &seed, &A, 0);
        int32_t alpha_mant = pseudo_rand_int32(&seed);
        exponent_t alpha_exp = pseudo_rand_int32(&seed) % 30;

        test_double_from_s32(Bf, &B);
        double alpha_f = ldexp(alpha_mant, alpha_exp);

        for(int i = 0; i < B.length; i++){
            Af[i] = Bf[i] * alpha_f;
        }

        float_s32_t alpha = {alpha_mant, alpha_exp};

        bfp_s32_scale(&A, &B, alpha);

        test_s32_from_double(expA, Af, MAX_LEN, A.exp);

        for(int i = 0; i < A.length; i++){
            TEST_ASSERT_INT32_WITHIN(1, expA[i], A.data[i]);
        }
    }
}


