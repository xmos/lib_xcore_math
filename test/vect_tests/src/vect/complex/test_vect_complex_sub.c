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

TEST_GROUP_RUNNER(vect_complex_sub) {
  RUN_TEST_CASE(vect_complex_sub, vect_complex_s32_sub);
  RUN_TEST_CASE(vect_complex_sub, vect_complex_s16_sub);
}

TEST_GROUP(vect_complex_sub);
TEST_SETUP(vect_complex_sub) { fflush(stdout); }
TEST_TEAR_DOWN(vect_complex_sub) {}


#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (64)
#else
#  define REPS       (1000)
#  define MAX_LEN    (256)
#endif


TEST(vect_complex_sub, vect_complex_s16_sub)
{
    

    unsigned seed = SEED_FROM_FUNC_NAME();


    struct {
        int16_t real[MAX_LEN];
        int16_t imag[MAX_LEN];
    } A, B, C, expA;

    struct {
        double real[MAX_LEN];
        double imag[MAX_LEN];
    } Af;

    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        const unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);

        const exponent_t b_exp = pseudo_rand_int(&seed, -2, 3 );
        const exponent_t c_exp = pseudo_rand_int(&seed, -2, 3 );

        const headroom_t b_hr = pseudo_rand_uint(&seed, 0, 15);
        const headroom_t c_hr = pseudo_rand_uint(&seed, 0, 15);

        right_shift_t b_shr, c_shr;
        exponent_t a_exp;

        vect_complex_s16_sub_prepare(&a_exp, &b_shr, &c_shr, b_exp, c_exp, b_hr, c_hr);

        for(unsigned int i = 0; i < length; i++){
            B.real[i] = pseudo_rand_int16(&seed) >> b_hr;
            B.imag[i] = pseudo_rand_int16(&seed) >> b_hr;

            C.real[i] = pseudo_rand_int16(&seed) >> c_hr;
            C.imag[i] = pseudo_rand_int16(&seed) >> c_hr;

            Af.real[i] = ldexp(B.real[i], b_exp) - ldexp(C.real[i], c_exp);
            Af.imag[i] = ldexp(B.imag[i], b_exp) - ldexp(C.imag[i], c_exp);
        }

        headroom_t hr = vect_complex_s16_sub(A.real, A.imag, B.real, B.imag, C.real, C.imag, length, b_shr, c_shr);
        
        TEST_ASSERT_EQUAL( vect_complex_s16_headroom(A.real, A.imag, length), hr );

        test_complex_s16_from_double(expA.real, expA.imag, Af.real, Af.imag, length, a_exp);

        for(unsigned int i = 0; i < length; i++){

            // if(abs(expA.real[i] - A.real[i]) > 2) 
            //     printf("((%d * 2^%d ) >> %d) - ((%d * 2^%d ) >> %d) = ( %d << %d)   // %d\n", B.real[i], b_exp, b_shr, C.real[i], c_exp, c_shr, A.real[i], a_exp, expA.real[i]);

            TEST_ASSERT_INT16_WITHIN(2, expA.real[i], A.real[i]);
            TEST_ASSERT_INT16_WITHIN(2, expA.imag[i], A.imag[i]);
        }
    }
}


TEST(vect_complex_sub, vect_complex_s32_sub)
{
    

    unsigned seed = SEED_FROM_FUNC_NAME();


    complex_s32_t A[MAX_LEN];
    complex_s32_t B[MAX_LEN];
    complex_s32_t C[MAX_LEN];

    struct {
        double real[MAX_LEN];
        double imag[MAX_LEN];
    } Af;

    complex_s32_t expA[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        const unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);

        const exponent_t b_exp = pseudo_rand_int(&seed, -2, 3 );
        const exponent_t c_exp = pseudo_rand_int(&seed, -2, 3 );

        const headroom_t b_hr = pseudo_rand_uint(&seed, 0, 18);
        const headroom_t c_hr = pseudo_rand_uint(&seed, 0, 18);

        right_shift_t b_shr, c_shr;
        exponent_t a_exp;
        vect_complex_s32_sub_prepare(&a_exp, &b_shr, &c_shr, b_exp, c_exp, b_hr, c_hr);

        for(unsigned int i = 0; i < length; i++){
            B[i].re = pseudo_rand_int32(&seed) >> b_hr;
            B[i].im = pseudo_rand_int32(&seed) >> b_hr;

            C[i].re = pseudo_rand_int32(&seed) >> c_hr;
            C[i].im = pseudo_rand_int32(&seed) >> c_hr;

            Af.real[i] = ldexp(B[i].re, b_exp) - ldexp(C[i].re, c_exp);
            Af.imag[i] = ldexp(B[i].im, b_exp) - ldexp(C[i].im, c_exp);
        }

        headroom_t hr = vect_complex_s32_sub(&A[0], &B[0], &C[0], length, b_shr, c_shr);
        
        TEST_ASSERT_EQUAL( vect_complex_s32_headroom(A, length), hr );

        test_complex_s32_from_double(expA, Af.real, Af.imag, length, a_exp);

        for(unsigned int i = 0; i < length; i++){

            // printf("(%ld >> %d) - (%ld >> %d) = ( %ld << %d)   // %ld\n", B[i].re, b_shr, C[i].re, c_shr, A[i].re, a_exp, expA[i].re);

            TEST_ASSERT_INT32_WITHIN(2, expA[i].re, A[i].re);
            TEST_ASSERT_INT32_WITHIN(2, expA[i].im, A[i].im);
        }
    }
}

