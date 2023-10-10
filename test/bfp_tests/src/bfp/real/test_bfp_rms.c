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


TEST_GROUP_RUNNER(bfp_rms) {
  RUN_TEST_CASE(bfp_rms, bfp_s16_rms);
  RUN_TEST_CASE(bfp_rms, bfp_s32_rms);
}

TEST_GROUP(bfp_rms);
TEST_SETUP(bfp_rms) { fflush(stdout); }
TEST_TEAR_DOWN(bfp_rms) {}

#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (128)
#else
#  define REPS       (1000)
#  define MAX_LEN    (512)
#endif

// static unsigned seed;

TEST(bfp_rms, bfp_s16_rms)
{


    unsigned seed = SEED_FROM_FUNC_NAME();

    int16_t WORD_ALIGNED dataB[MAX_LEN];
    bfp_s16_t B;

    B.data = dataB;

    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        bfp_s16_t B;

        bfp_s16_init(&B, dataB,
                          pseudo_rand_int(&seed, -30, 30),
                          pseudo_rand_uint(&seed, 1, MAX_LEN+1), 0);
                          
        B.hr = pseudo_rand_uint(&seed, 0, 12);

        for(unsigned int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int16(&seed) >> B.hr;
        }

        bfp_s16_headroom(&B);

        float_s32_t result = bfp_s16_rms(&B);

        double energy = 0;
        for(unsigned int i = 0; i < B.length; i++){
            energy += pow(ldexp(B.data[i], B.exp), 2);
        }

        const double mean_energy = energy / B.length;

        const double expectedF = sqrt(mean_energy);

        float_s32_t ideal_result = {
            .mant = lround( expectedF / ldexp((double) 1,result.exp) ),
            .exp = (exponent_t) floor( log2(expectedF) ) - 30 };


        TEST_ASSERT_INT32_WITHIN(3, ideal_result.exp, result.exp);
        TEST_ASSERT_INT32_WITHIN(4, ideal_result.mant, result.mant);
    }
}


TEST(bfp_rms, bfp_s32_rms)
{


    unsigned seed = SEED_FROM_FUNC_NAME();

    int32_t dataB[MAX_LEN];
    bfp_s32_t B;

    B.data = dataB;

    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        bfp_s32_t B;

        bfp_s32_init(&B, dataB,
                          pseudo_rand_int(&seed, -30, 30),
                          pseudo_rand_uint(&seed, 1, MAX_LEN+1), 0);
                          
        B.hr = pseudo_rand_uint(&seed, 0, 28);

        for(unsigned int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int32(&seed) >> B.hr;
        }

        bfp_s32_headroom(&B);

        float_s32_t result = bfp_s32_rms(&B);

        double energy = 0;
        for(unsigned int i = 0; i < B.length; i++){
            energy += pow(ldexp(B.data[i], B.exp), 2);
        }

        const double mean_energy = energy / B.length;

        const double expectedF = sqrt(mean_energy);

        float_s32_t ideal_result = {
            .mant = lround( expectedF / ldexp((double) 1,result.exp) ),
            .exp = (exponent_t) floor( log2(expectedF) ) - 30 };

        TEST_ASSERT_INT32_WITHIN(3, ideal_result.exp, result.exp);
        TEST_ASSERT_INT32_WITHIN(4, ideal_result.mant, result.mant);
    }
}

