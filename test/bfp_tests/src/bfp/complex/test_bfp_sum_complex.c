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


TEST_GROUP_RUNNER(bfp_complex_sum) {
  RUN_TEST_CASE(bfp_complex_sum, bfp_complex_s16_sum);
  RUN_TEST_CASE(bfp_complex_sum, bfp_complex_s32_sum);
}

TEST_GROUP(bfp_complex_sum);
TEST_SETUP(bfp_complex_sum) { fflush(stdout); }
TEST_TEAR_DOWN(bfp_complex_sum) {}

#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (128)
#else
#  define REPS       (1000)
#  define MAX_LEN    (512)
#endif


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

        for(unsigned int i = 0; i < B.length; i++){
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
        const unsigned old_seed = seed;

        bfp_complex_s32_init(&B, B_data, pseudo_rand_int(&seed, -100, 100),
            pseudo_rand_uint(&seed, 1, MAX_LEN+1), 0);
            
        setExtraInfo_RSL(r, old_seed, B.length);

        B.hr = pseudo_rand_uint(&seed, 0, 6);

        for(unsigned int i = 0; i < B.length; i++){
            // Use only positive values so that they tend not to cancel out (which they might in practice)
            B.data[i].re = pseudo_rand_int(&seed, 0, INT32_MAX) >> B.hr;
            B.data[i].im = pseudo_rand_int(&seed, 0, INT32_MAX) >> B.hr;
        }

        bfp_complex_s32_headroom(&B);

        float_complex_s64_t result = bfp_complex_s32_sum(&B);
      
        TEST_ASSERT_GREATER_OR_EQUAL(B.exp, result.exp);

        right_shift_t shr = result.exp - B.exp;
        
        complex_s64_t expected = {0, 0};

        for(unsigned int i = 0; i < B.length; i++){
          expected.re += (B.data[i].re + (shr? (1<<(shr-1)) : 0)) >> shr;
          expected.im += (B.data[i].im + (shr? (1<<(shr-1)) : 0)) >> shr;
        }

        TEST_ASSERT_INT64_WITHIN_MESSAGE(1, expected.re, result.mant.re, "Real Part");
        TEST_ASSERT_INT64_WITHIN_MESSAGE(1, expected.im, result.mant.im, "Imag Part");
    }
}
