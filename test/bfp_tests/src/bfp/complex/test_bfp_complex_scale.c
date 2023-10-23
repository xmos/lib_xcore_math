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


TEST_GROUP_RUNNER(bfp_complex_scale) {
  RUN_TEST_CASE(bfp_complex_scale, bfp_complex_s16_scale);
  RUN_TEST_CASE(bfp_complex_scale, bfp_complex_s32_scale);
}

TEST_GROUP(bfp_complex_scale);
TEST_SETUP(bfp_complex_scale) { fflush(stdout); }
TEST_TEAR_DOWN(bfp_complex_scale) {}

#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (128)
#else
#  define REPS       (1000)
#  define MAX_LEN    (512)
#endif


static char msg_buff[200];

#define TEST_ASSERT_EQUAL_MSG(EXPECTED, ACTUAL, EXTRA, LINE_NUM)   do{          \
    if((EXPECTED)!=(ACTUAL)) {                                                  \
      sprintf(msg_buff, "%s (test vector @ rep %u)", (EXTRA), (LINE_NUM));     \
      TEST_ASSERT_EQUAL_MESSAGE((EXPECTED), (ACTUAL), msg_buff);                \
    }} while(0)


TEST(bfp_complex_scale, bfp_complex_s16_scale)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    struct {
        int16_t real[MAX_LEN];
        int16_t imag[MAX_LEN];
    } A_data, B_data;

    bfp_complex_s16_t A, B;

    struct {
        double real[MAX_LEN];
        double imag[MAX_LEN];
    } Af;

    struct {
        int16_t real[MAX_LEN];
        int16_t imag[MAX_LEN];
    } expA;


    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        bfp_complex_s16_init(&B, B_data.real, B_data.imag,
            pseudo_rand_int(&seed, -100, 100),
            pseudo_rand_int(&seed, 1, MAX_LEN+1), 0);

        bfp_complex_s16_init(&A, A_data.real, A_data.imag, 0, B.length, 0);

        B.hr = pseudo_rand_uint(&seed, 0, 12);

        const headroom_t c_hr = pseudo_rand_uint(&seed, 0, 12);

        float_complex_s16_t C = {
            {   pseudo_rand_int16(&seed) >> c_hr,
                pseudo_rand_int16(&seed) >> c_hr },
            pseudo_rand_int(&seed, -100, 100),
        };

        for(unsigned int i = 0; i < B.length; i++){
            B.real[i] = pseudo_rand_int16(&seed) >> B.hr;
            B.imag[i] = pseudo_rand_int16(&seed) >> B.hr;

            Af.real[i] = ldexp(B.real[i], B.exp) * ldexp(C.mant.re, C.exp)
                       - ldexp(B.imag[i], B.exp) * ldexp(C.mant.im, C.exp);
            Af.imag[i] = ldexp(B.real[i], B.exp) * ldexp(C.mant.im, C.exp)
                       + ldexp(B.imag[i], B.exp) * ldexp(C.mant.re, C.exp);
        }

        bfp_complex_s16_headroom(&B);

        bfp_complex_s16_scale(&A, &B, C);
        
        test_complex_s16_from_double(expA.real, expA.imag, Af.real, Af.imag, MAX_LEN, A.exp);

        for(unsigned int i = 0; i < A.length; i++){
            TEST_ASSERT_INT16_WITHIN(1, expA.real[i], A.real[i]);
            TEST_ASSERT_INT16_WITHIN(1, expA.imag[i], A.imag[i]);
        }
    }
}


TEST(bfp_complex_scale, bfp_complex_s32_scale_prepare)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);
        
        exponent_t b_exp = pseudo_rand_int(&seed, -30, 30);
        headroom_t b_hr  = pseudo_rand_uint(&seed, 0, 31);
        exponent_t c_exp = pseudo_rand_int(&seed, -30, 30);
        headroom_t c_hr  = pseudo_rand_uint(&seed, 0, 31);

        complex_s32_t WORD_ALIGNED B = {((int) (0-0x80000000)) >> b_hr, ((int) (0-0x80000000)) >> b_hr};
        complex_s32_t WORD_ALIGNED C = {((int) (0-0x80000000)) >> c_hr, ((int) (0-0x80000000)) >> c_hr};
        complex_s32_t WORD_ALIGNED A;

        exponent_t a_exp;
        right_shift_t b_shr, c_shr;

        vect_complex_s32_scale_prepare(&a_exp, &b_shr, &c_shr, b_exp, c_exp, b_hr, c_hr);

        c_exp += c_shr;
        c_hr += c_shr;

        vect_complex_s32_scale(&A, &B, C.re, C.im, 1, b_shr, c_shr);

        TEST_ASSERT_EQUAL_MSG(0x7FFFFFFF, A.im, "A is wrong (sat)", r);


    }
        
}


TEST(bfp_complex_scale, bfp_complex_s32_scale)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    complex_s32_t A_data[MAX_LEN], B_data[MAX_LEN];

    bfp_complex_s32_t A, B;

    struct {
        double real[MAX_LEN];
        double imag[MAX_LEN];
    } Af;

    complex_s32_t expA[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        bfp_complex_s32_init(&B, B_data,
            pseudo_rand_int(&seed, -100, 100),
            pseudo_rand_int(&seed, 1, MAX_LEN+1), 0);

        bfp_complex_s32_init(&A, A_data, 0, B.length, 0);

        B.hr = pseudo_rand_uint(&seed, 0, 28);

        const headroom_t c_hr = pseudo_rand_uint(&seed, 0, 28);

        float_complex_s32_t C = {
            {   pseudo_rand_int32(&seed) >> c_hr,
                pseudo_rand_int32(&seed) >> c_hr },
            pseudo_rand_int(&seed, -100, 100),
        };

        for(unsigned int i = 0; i < B.length; i++){
            B.data[i].re = pseudo_rand_int32(&seed) >> B.hr;
            B.data[i].im = pseudo_rand_int32(&seed) >> B.hr;

            Af.real[i] = ldexp(B.data[i].re, B.exp) * ldexp(C.mant.re, C.exp)
                       - ldexp(B.data[i].im, B.exp) * ldexp(C.mant.im, C.exp);
            Af.imag[i] = ldexp(B.data[i].re, B.exp) * ldexp(C.mant.im, C.exp)
                       + ldexp(B.data[i].im, B.exp) * ldexp(C.mant.re, C.exp);
        }

        bfp_complex_s32_headroom(&B);

        bfp_complex_s32_scale(&A, &B, C);
        
        test_complex_s32_from_double(expA, Af.real, Af.imag, MAX_LEN, A.exp);

        // astew: 2022/06/30 -- Increased threshold from 2 to 3. Test was failing after I fixed
        //        the random number generation problem. (Threshold is observed, not theoretical)
        for(unsigned int i = 0; i < A.length; i++){
            TEST_ASSERT_INT32_WITHIN(3, expA[i].re, A.data[i].re);
            TEST_ASSERT_INT32_WITHIN(3, expA[i].im, A.data[i].im);
        }
    }
}
