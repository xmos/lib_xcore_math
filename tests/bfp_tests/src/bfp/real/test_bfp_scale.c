// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xmath/xmath.h"

#include "../../tst_common.h"

#include "unity_fixture.h"


TEST_GROUP_RUNNER(bfp_scale) {
  RUN_TEST_CASE(bfp_scale, bfp_s32_scale);
  RUN_TEST_CASE(bfp_scale, bfp_s16_scale);
}

TEST_GROUP(bfp_scale);
TEST_SETUP(bfp_scale) { fflush(stdout); }
TEST_TEAR_DOWN(bfp_scale) {}

#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (128)
#else
#  define REPS       (1000)
#  define MAX_LEN    (512)
#endif


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
        unsigned old_seed = seed;

        test_random_bfp_s16(&B, MAX_LEN, &seed, &A, 0);
        int16_t alpha_mant = pseudo_rand_int16(&seed);
        exponent_t alpha_exp = pseudo_rand_int16(&seed) % 30;

        test_double_from_s16(Bf, &B);
        double alpha_f = ldexp(alpha_mant, alpha_exp);
      
        setExtraInfo_RSL(r, old_seed, B.length);

        for(unsigned int i = 0; i < B.length; i++){
            Af[i] = Bf[i] * alpha_f;
        }

        float alpha = s32_to_f32(alpha_mant, alpha_exp);
        
        bfp_s16_scale(&A, &B, alpha);

        test_s16_from_double(expA, Af, MAX_LEN, A.exp);

        XTEST_ASSERT_VECT_S16_WITHIN(1, expA, A.data, A.length,
            "Expected: %d  <-- %d * 2^(%d)  * %d * 2^(%d) \nActual: %d * 2^(%d)\n",
            expA[i], B.data[i], B.exp, alpha_mant, alpha_exp, A.data[i], A.exp);

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

        for(unsigned int i = 0; i < B.length; i++){
            Af[i] = Bf[i] * alpha_f;
        }

        float_s32_t alpha = {alpha_mant, alpha_exp};

        bfp_s32_scale(&A, &B, alpha);

        test_s32_from_double(expA, Af, MAX_LEN, A.exp);

        XTEST_ASSERT_VECT_S32_WITHIN(1, expA, A.data, A.length,
            "Expected: %ld  <-- %ld * 2^(%d)  * %ld * 2^(%d) \nActual: %ld * 2^(%d)\n",
            (long int) expA[i], (long int) B.data[i], B.exp, (long int) alpha.mant, alpha.exp, (long int) A.data[i], A.exp);
    }
}


