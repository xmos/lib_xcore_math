// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xmath/xmath.h"
#include "xmath/xs3/vpu_scalar_ops.h"

#include "../src/vect/vpu_helper.h"

#include "../../tst_common.h"

#include "unity_fixture.h"


TEST_GROUP_RUNNER(vect_complex_real_scale) {
  RUN_TEST_CASE(vect_complex_real_scale, vect_s16_scale_prepare);
  RUN_TEST_CASE(vect_complex_real_scale, vect_complex_s16_real_scale);
  RUN_TEST_CASE(vect_complex_real_scale, vect_complex_s32_real_scale);
}

TEST_GROUP(vect_complex_real_scale);
TEST_SETUP(vect_complex_real_scale) { fflush(stdout); }
TEST_TEAR_DOWN(vect_complex_real_scale) {}


#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (64)
#else
#  define REPS       (1000)
#  define MAX_LEN    (256)
#endif


TEST(vect_complex_real_scale, vect_s16_scale_prepare)
{
    

    unsigned seed = SEED_FROM_FUNC_NAME();


    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);
        
        exponent_t b_exp = pseudo_rand_int(&seed, -30, 30);
        headroom_t b_hr  = pseudo_rand_uint(&seed, 0, 15);
        exponent_t c_exp = pseudo_rand_int(&seed, -30, 30);
        headroom_t c_hr  = pseudo_rand_uint(&seed, 0, 15);

        int16_t WORD_ALIGNED B_re = ((int16_t)-0x8000) >> b_hr;
        int16_t WORD_ALIGNED B_im = ((int16_t)-0x8000) >> b_hr;
        int16_t WORD_ALIGNED C = ((int16_t)-0x8000) >> c_hr;
        int16_t WORD_ALIGNED A_re;
        int16_t WORD_ALIGNED A_im;

        exponent_t a_exp;
        right_shift_t sat;

        vect_complex_s16_real_scale_prepare(&a_exp, &sat, b_exp, c_exp, b_hr, c_hr);

        vect_complex_s16_real_scale(&A_re, &A_im, &B_re, &B_im, C, 1, sat);

        const int32_t p = ((int32_t)B_re) * C;
        int16_t expected = p;

        if( p > VPU_INT16_MAX )
            expected = VPU_INT16_MAX;

        TEST_ASSERT_EQUAL_HEX16(expected, A_re);
        TEST_ASSERT_EQUAL_HEX16(expected, A_im);

        // Make sure the exponent was chosen correctly

        double fp  = ldexp(B_re, b_exp) * ldexp(C, c_exp);
        double fa  = ldexp(A_re, a_exp);

        // (A should be no more than 1 LSb off)
        TEST_ASSERT( fabs(fp - fa) <= ldexp(1,a_exp) );
        
    }
}


TEST(vect_complex_real_scale, vect_complex_s16_real_scale)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();

    
    struct {
        int16_t real[MAX_LEN];
        int16_t imag[MAX_LEN];
    } A, B;
    
    int16_t C;

    for(unsigned int v = 0; v < REPS; v++){

        setExtraInfo_RS(v, seed);

        unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);

        headroom_t b_hr = pseudo_rand_uint(&seed, 0, 15);
        headroom_t c_hr = pseudo_rand_uint(&seed, 0, 15);

        // Ideal sat would be  15 - (b_hr+c_hr).

        right_shift_t sat = 15 - (b_hr+c_hr) + pseudo_rand_int(&seed, -1, 3 );
        sat = MAX(sat, 0);
        
        for(unsigned int i = 0; i < length; i++){
            B.real[i] = pseudo_rand_int16(&seed) >> b_hr;
            B.imag[i] = pseudo_rand_int16(&seed) >> b_hr;
            C = pseudo_rand_int16(&seed) >> c_hr;
        }
        

        {
            headroom_t hr = vect_complex_s16_real_scale(A.real, A.imag,
                                                            B.real, B.imag,
                                                            C, length, sat);

            headroom_t exp_hr = vect_complex_s16_headroom(A.real, A.imag, length);

            TEST_ASSERT_EQUAL(exp_hr, hr);

            for(unsigned int i = 0; i < length; i++){
                int32_t exp = (int32_t) ldexp( (double) (((int32_t)B.real[i]) * C), -sat) + (int32_t) ldexp(1.0, -40);
                exp = MIN(exp, VPU_INT16_MAX);
                exp = MAX(exp, VPU_INT16_MIN);

                TEST_ASSERT_INT16_WITHIN(1, exp, A.real[i]);

                exp = lround(ldexp( ((int32_t)B.imag[i]) * C, -sat) + ldexp(1, -40));
                exp = MIN(exp, VPU_INT16_MAX);
                exp = MAX(exp, VPU_INT16_MIN);
                TEST_ASSERT_INT16_WITHIN(1, exp, A.imag[i]);
            }
        }

        {
            memcpy(A.real, B.real, sizeof(A.real));
            memcpy(A.imag, B.imag, sizeof(A.imag));


            headroom_t hr = vect_complex_s16_real_scale(A.real, A.imag,
                                                            A.real, A.imag,
                                                            C, length, sat);

            headroom_t exp_hr = vect_complex_s16_headroom(A.real, A.imag, length);

            TEST_ASSERT_EQUAL(exp_hr, hr);

            for(unsigned int i = 0; i < length; i++){
                int32_t exp = (int32_t) ldexp( (double) (((int32_t)B.real[i]) * C), -sat) + (int32_t) ldexp(1.0, -40);
                exp = MIN(exp, VPU_INT16_MAX);
                exp = MAX(exp, VPU_INT16_MIN);
                TEST_ASSERT_INT16_WITHIN(1, exp, A.real[i]);

                exp = lround(ldexp( ((int32_t)B.imag[i]) * C, -sat) + ldexp(1, -40));
                exp = MIN(exp, VPU_INT16_MAX);
                exp = MAX(exp, VPU_INT16_MIN);
                TEST_ASSERT_INT16_WITHIN(1, exp, A.imag[i]);
            }
        }

        
    }
}


TEST(vect_complex_real_scale, vect_complex_s32_real_scale)
{
    

    unsigned seed = SEED_FROM_FUNC_NAME();


    complex_s32_t A[MAX_LEN];
    complex_s32_t B[MAX_LEN];

    struct {
        double real[MAX_LEN];
        double imag[MAX_LEN];
    } Af;

    complex_s32_t expA[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        const unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);

        const exponent_t b_exp = 0;
        const exponent_t c_exp = 0;

        const headroom_t b_hr = pseudo_rand_uint(&seed, 0, 18);
        const headroom_t c_hr = pseudo_rand_uint(&seed, 0, 18);

        int32_t C = pseudo_rand_int32(&seed) >> c_hr;

        right_shift_t b_shr, c_shr;
        exponent_t a_exp;
        vect_complex_s32_real_scale_prepare(&a_exp, &b_shr, &c_shr, b_exp, c_exp, b_hr, c_hr);

        for(unsigned int i = 0; i < length; i++){
            B[i].re = pseudo_rand_int32(&seed) >> b_hr;
            B[i].im = pseudo_rand_int32(&seed) >> b_hr;

            Af.real[i] = ldexp(B[i].re, b_exp) * ldexp(C, c_exp);
            Af.imag[i] = ldexp(B[i].im, b_exp) * ldexp(C, c_exp);
        }

        headroom_t hr = vect_complex_s32_real_scale(&A[0], &B[0], C, length, b_shr, c_shr);
        
        TEST_ASSERT_EQUAL( vect_complex_s32_headroom(A, length), hr );

        test_complex_s32_from_double(expA, Af.real, Af.imag, length, a_exp);

        for(unsigned int i = 0; i < length; i++){
            TEST_ASSERT_INT32_WITHIN(2, expA[i].re, A[i].re);
            TEST_ASSERT_INT32_WITHIN(2, expA[i].im, A[i].im);
        }
    }
}

