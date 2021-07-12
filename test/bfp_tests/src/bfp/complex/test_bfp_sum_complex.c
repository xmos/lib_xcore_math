// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "bfp_math.h"

#include "../../tst_common.h"

#include "unity_fixture.h"


TEST_GROUP_RUNNER(bfp_complex_sum) {
  RUN_TEST_CASE(bfp_complex_sum, bfp_complex_s16_sum);
  RUN_TEST_CASE(bfp_complex_sum, bfp_complex_s32_sum);
}

TEST_GROUP(bfp_complex_sum);
TEST_SETUP(bfp_complex_sum) {}
TEST_TEAR_DOWN(bfp_complex_sum) {}

#define REPS        (100)
#define MAX_LEN     1024 


TEST(bfp_complex_sum, bfp_complex_s16_sum)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    int16_t WORD_ALIGNED B_real[MAX_LEN];
    int16_t WORD_ALIGNED B_imag[MAX_LEN];
    
    bfp_complex_s16_t B;


    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        bfp_complex_s16_init(&B, B_real, B_imag, 
            pseudo_rand_int(&seed, -100, 100),
            pseudo_rand_uint(&seed, 0, MAX_LEN+1), 0);

        B.hr = pseudo_rand_uint(&seed, 0, 15);

        float_complex_s32_t expected = {
            {0, 0},
            B.exp 
        };

        for(int i = 0; i < B.length; i++){
            B.real[i] = pseudo_rand_int16(&seed) >> B.hr;
            B.imag[i] = pseudo_rand_int16(&seed) >> B.hr;

            expected.mant.re += B.real[i];
            expected.mant.im += B.imag[i];
        }

        bfp_complex_s16_headroom(&B);

        float_complex_s32_t result = bfp_complex_s16_sum(&B);

        TEST_ASSERT_EQUAL(expected.exp, result.exp);
        TEST_ASSERT_EQUAL_INT32(expected.mant.re, result.mant.re);
        TEST_ASSERT_EQUAL_INT32(expected.mant.im, result.mant.im);
    }
}


TEST(bfp_complex_sum, bfp_complex_s32_sum)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    complex_s32_t B_data[MAX_LEN];
    
    bfp_complex_s32_t B;


    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        bfp_complex_s32_init(&B, B_data, pseudo_rand_int(&seed, -100, 100),
            pseudo_rand_uint(&seed, 0, MAX_LEN+1), 0);

        B.hr = pseudo_rand_uint(&seed, 0, 6);

        complex_double_t expected = {0, 0};

        for(int i = 0; i < B.length; i++){

            // Use only positive values so that they tend not to cancel out (which they might in practice)
            B.data[i].re = pseudo_rand_int(&seed, 0, INT32_MAX) >> B.hr;
            B.data[i].im = pseudo_rand_int(&seed, 0, INT32_MAX) >> B.hr;

            expected.re += ldexp(B.data[i].re, B.exp);
            expected.im += ldexp(B.data[i].im, B.exp);
        }

        bfp_complex_s32_headroom(&B);

        float_complex_s64_t result = bfp_complex_s32_sum(&B);

        TEST_ASSERT_GREATER_OR_EQUAL(B.exp, result.exp);

        complex_double_t fl = { ldexp(result.mant.re, result.exp), ldexp(result.mant.im, result.exp) };

        complex_double_t ds = { 
            fabs((expected.re - fl.re) / expected.re),
            fabs((expected.im - fl.im) / expected.im) };

        TEST_ASSERT(ds.re <= ldexp(1, -20));
        TEST_ASSERT(ds.im <= ldexp(1, -20));
    }
}
