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


TEST_GROUP_RUNNER(bfp_complex_energy) {
  RUN_TEST_CASE(bfp_complex_energy, bfp_complex_s16_energy);
  RUN_TEST_CASE(bfp_complex_energy, bfp_complex_s32_energy);
}

TEST_GROUP(bfp_complex_energy);
TEST_SETUP(bfp_complex_energy) { fflush(stdout); }
TEST_TEAR_DOWN(bfp_complex_energy) {}

#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (128)
#else
#  define REPS       (1000)
#  define MAX_LEN    (512)
#endif


TEST(bfp_complex_energy, bfp_complex_s16_energy)
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

        double expected_f = 0.0;

        for(unsigned int i = 0; i < B.length; i++){
            B.real[i] = pseudo_rand_int16(&seed) >> B.hr;
            B.imag[i] = pseudo_rand_int16(&seed) >> B.hr;

            expected_f += pow(ldexp(B.real[i], B.exp), 2);
            expected_f += pow(ldexp(B.imag[i], B.exp), 2);
        }

        bfp_complex_s16_headroom(&B);

        float_s64_t result = bfp_complex_s16_energy(&B);

        conv_error_e conv_error = 0;
        int64_t expected = conv_double_to_s64(expected_f, result.exp, &conv_error);

        TEST_ASSERT_EQUAL(0, conv_error);

        TEST_ASSERT_EQUAL_INT64_MESSAGE(expected, result.mant, "");
    }
}


TEST(bfp_complex_energy, bfp_complex_s32_energy)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    complex_s32_t B_data[MAX_LEN];
    
    bfp_complex_s32_t B;

    int32_t worst_error = 0;

    for(int r = 0; r < REPS; r++){
        const unsigned old_seed = seed;

        bfp_complex_s32_init(&B, B_data, pseudo_rand_int(&seed, -100, 100),
            pseudo_rand_uint(&seed, 0, MAX_LEN+1), 0);

        setExtraInfo_RSL(r, old_seed, B.length);

        B.hr = pseudo_rand_uint(&seed, 0, 18);

        double expected_f = 0.0;

        for(unsigned int i = 0; i < B.length; i++){
            B.data[i].re = pseudo_rand_int32(&seed) >> B.hr;
            B.data[i].im = pseudo_rand_int32(&seed) >> B.hr;

            expected_f += pow(ldexp(B.data[i].re, B.exp), 2);
            expected_f += pow(ldexp(B.data[i].im, B.exp), 2);
        }

        // if(r == 11){
        //   printf("B.exp = %d\n", B.exp);
        //   for(unsigned int k = 0; k < B.length; k++){
        //     printf("(%ld, %ld), ", B.data[k].re, B.data[k].im);
        //   }
        //   printf("\n\n");
        // }

        bfp_complex_s32_headroom(&B);

        float_s64_t result = bfp_complex_s32_energy(&B);

        conv_error_e conv_error = 0;
        int64_t expected = conv_double_to_s64(expected_f, result.exp, &conv_error);

        TEST_ASSERT_EQUAL(0, conv_error);

        unsigned threshold;
        threshold = 10000;

        exponent_t tmp;
        right_shift_t b_shr;
        vect_s32_energy_prepare(&tmp, &b_shr, 2 * B.length, B.exp, B.hr);

        int32_t error = abs((int32_t)(expected - result.mant));

        // printf("!!! Rep: % 3d | Length: % 4u | HR: % 2u | Error: % 14ld | shr: % 4d !!!\n", r, B.length, B.hr, error, b_shr  );

        worst_error = MAX(worst_error, error);

        // printf("(%d, %u, %ld), ", B.length, B.hr, (int32_t)(expected - result.mant));

        // If left-shifting by at least 15 bits, the result should be exact (because loss only
        // happens when b_shr is positive, and on the implicit 30-bit right-shift, and if b_shr
        // is <= -15 the 30-bit right-shift can only shift out zeros).
        if( b_shr <= -15 )
          threshold = 0;

        TEST_ASSERT_INT64_WITHIN_MESSAGE(threshold, expected, result.mant, "");
    }

    // printf("worst_error: %ld\n", worst_error);
}
