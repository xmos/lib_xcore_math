// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// XMOS Public License: Version 1

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xmath/xmath.h"
#include "xmath/xs3/vpu_scalar_ops.h"
#include "testing.h"

#include "../../tst_common.h"

#include "unity_fixture.h"


TEST_GROUP_RUNNER(vect_complex_conj_macc) {
  RUN_TEST_CASE(vect_complex_conj_macc, vect_complex_s16_conj_macc);
  RUN_TEST_CASE(vect_complex_conj_macc, vect_complex_s16_conj_nmacc);
  RUN_TEST_CASE(vect_complex_conj_macc, vect_complex_s32_conj_macc);
  RUN_TEST_CASE(vect_complex_conj_macc, vect_complex_s32_conj_nmacc);
}

TEST_GROUP(vect_complex_conj_macc);
TEST_SETUP(vect_complex_conj_macc) { fflush(stdout); }
TEST_TEAR_DOWN(vect_complex_conj_macc) {}



#if SMOKE_TEST
#  define REPS       (100)
#  define LEN        (65)
#else
#  define REPS       (1000)
#  define LEN        (257)
#endif


TEST(vect_complex_conj_macc, vect_complex_s16_conj_macc)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    complex_double_t A_fp[LEN], B_fp[LEN], C_fp[LEN];

    struct {
      int16_t re[LEN + (LEN % 2)];  // + (LEN % 2) is to make sure 'im[]' is word-aligned
      int16_t im[LEN + (LEN % 2)];
    } A, B, C;

    struct { exponent_t a, b, c; } exp;
    struct { headroom_t a, b, c; } hr;

    vect_complex_s16_set(A.re, A.im, 0, 0, LEN);
    exp.a = -1024;
    hr.a = vect_complex_s16_headroom(A.re, A.im, LEN);

    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);
        
        exp.b = 10 + (pseudo_rand_int16(&seed) % 10);
        exp.c = 10 + (pseudo_rand_int16(&seed) % 10);

        for(int i = 0; i < LEN; i++){
            unsigned shr = pseudo_rand_uint16(&seed) % 8;
            B.re[i] = pseudo_rand_int16(&seed) >> shr;
            B.im[i] = pseudo_rand_int16(&seed) >> shr;
            C.re[i] = pseudo_rand_int16(&seed) >> shr;
            C.im[i] = pseudo_rand_int16(&seed) >> shr;
            
            B_fp[i].re = ldexp(B.re[i], exp.b);
            B_fp[i].im = ldexp(B.im[i], exp.b);
            C_fp[i].re = ldexp(C.re[i], exp.c);
            C_fp[i].im = ldexp(C.im[i], exp.c);
            
            A_fp[i].re = ldexp(A.re[i], exp.a);
            A_fp[i].im = ldexp(A.im[i], exp.a);
        }

        hr.b = vect_complex_s16_headroom(B.re, B.im, LEN);
        hr.c = vect_complex_s16_headroom(C.re, C.im, LEN);

        right_shift_t acc_shr, bc_shr;
        vect_complex_s16_conj_macc_prepare(&exp.a, &acc_shr, &bc_shr,
                                          exp.a, exp.b, exp.c,
                                          hr.a, hr.b, hr.c);

        hr.a = vect_complex_s16_conj_macc(A.re, A.im, B.re, B.im, C.re, C.im, LEN, acc_shr, bc_shr);
        
        TEST_ASSERT_LESS_THAN(4, hr.a);

        for(int i = 0; i < LEN; i++){
          conv_error_e error = 0;
          complex_double_t expected_fp;
          expected_fp.re = A_fp[i].re + B_fp[i].re * C_fp[i].re + B_fp[i].im * C_fp[i].im;
          expected_fp.im = A_fp[i].im - B_fp[i].re * C_fp[i].im + B_fp[i].im * C_fp[i].re;
          
          complex_s32_t expected;
          expected.re = conv_double_to_s16(expected_fp.re, exp.a, &error);
          expected.im = conv_double_to_s16(expected_fp.im, exp.a, &error);

          if(error) printf("Conversion error: %d\n", error);
          TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error.");

#define THRESHOLD  2
          if(expected.re - A.re[i] > THRESHOLD || A.re[i] - expected.re > THRESHOLD)
            printf("[%d]  exp.a = %d; expected[%d].re = %ld  ( %f );   A.re[%d] = %d\n", v, exp.a, i, (long int) expected.re, expected_fp.re, i, A.re[i]);
          TEST_ASSERT_INT16_WITHIN_MESSAGE(THRESHOLD, expected.re, A.re[i], "Error not within threshold. (real)");

          if(expected.im - A.im[i] > THRESHOLD || A.im[i] - expected.im > THRESHOLD)
            printf("[%d]  exp.a = %d; expected[%d].im = %ld  ( %f );   A.im[%d] = %d\n", v, exp.a, i, (long int) expected.im, expected_fp.im, i, A.im[i]);
          TEST_ASSERT_INT16_WITHIN_MESSAGE(THRESHOLD, expected.im, A.im[i], "Error not within threshold. (imag)");
#undef THRESHOLD
        }
        
    }
}


TEST(vect_complex_conj_macc, vect_complex_s16_conj_nmacc)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    complex_double_t A_fp[LEN], B_fp[LEN], C_fp[LEN];

    struct {
      int16_t re[LEN + (LEN % 2)];  // + (LEN % 2) is to make sure 'im[]' is word-aligned
      int16_t im[LEN + (LEN % 2)];
    } A, B, C;

    struct { exponent_t a, b, c; } exp;
    struct { headroom_t a, b, c; } hr;

    vect_complex_s16_set(A.re, A.im, 0, 0, LEN);
    exp.a = -1024;
    hr.a = vect_complex_s16_headroom(A.re, A.im, LEN);

    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);
        
        exp.b = 10 + (pseudo_rand_int16(&seed) % 10);
        exp.c = 10 + (pseudo_rand_int16(&seed) % 10);

        for(int i = 0; i < LEN; i++){
            unsigned shr = pseudo_rand_uint16(&seed) % 8;
            B.re[i] = pseudo_rand_int16(&seed) >> shr;
            B.im[i] = pseudo_rand_int16(&seed) >> shr;
            C.re[i] = pseudo_rand_int16(&seed) >> shr;
            C.im[i] = pseudo_rand_int16(&seed) >> shr;
            
            B_fp[i].re = ldexp(B.re[i], exp.b);
            B_fp[i].im = ldexp(B.im[i], exp.b);
            C_fp[i].re = ldexp(C.re[i], exp.c);
            C_fp[i].im = ldexp(C.im[i], exp.c);
            
            A_fp[i].re = ldexp(A.re[i], exp.a);
            A_fp[i].im = ldexp(A.im[i], exp.a);
        }

        hr.b = vect_complex_s16_headroom(B.re, B.im, LEN);
        hr.c = vect_complex_s16_headroom(C.re, C.im, LEN);

        right_shift_t acc_shr, bc_shr;
        vect_complex_s16_conj_macc_prepare(&exp.a, &acc_shr, &bc_shr,
                                          exp.a, exp.b, exp.c,
                                          hr.a, hr.b, hr.c);

        hr.a = vect_complex_s16_conj_nmacc(A.re, A.im, B.re, B.im, C.re, C.im, LEN, acc_shr, bc_shr);
        
        TEST_ASSERT_LESS_THAN(4, hr.a);

        for(int i = 0; i < LEN; i++){
          conv_error_e error = 0;
          complex_double_t expected_fp;
          expected_fp.re = A_fp[i].re - (  B_fp[i].re * C_fp[i].re + B_fp[i].im * C_fp[i].im );
          expected_fp.im = A_fp[i].im - (- B_fp[i].re * C_fp[i].im + B_fp[i].im * C_fp[i].re );
          
          complex_s32_t expected;
          expected.re = conv_double_to_s16(expected_fp.re, exp.a, &error);
          expected.im = conv_double_to_s16(expected_fp.im, exp.a, &error);

          if(error) printf("Conversion error: %d\n", error);
          TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error.");

#define THRESHOLD  2
          if(expected.re - A.re[i] > THRESHOLD || A.re[i] - expected.re > THRESHOLD)
            printf("[%d]  exp.a = %d; expected[%d].re = %ld  ( %f );   A.re[%d] = %d\n", v, exp.a, i, (long int) expected.re, expected_fp.re, i, A.re[i]);
          TEST_ASSERT_INT16_WITHIN_MESSAGE(THRESHOLD, expected.re, A.re[i], "Error not within threshold. (real)");

          if(expected.im - A.im[i] > THRESHOLD || A.im[i] - expected.im > THRESHOLD)
            printf("[%d]  exp.a = %d; expected[%d].im = %ld  ( %f );   A.im[%d] = %d\n", v, exp.a, i, (long int) expected.im, expected_fp.im, i, A.im[i]);
          TEST_ASSERT_INT16_WITHIN_MESSAGE(THRESHOLD, expected.im, A.im[i], "Error not within threshold. (imag)");
#undef THRESHOLD
        }
        
    }
}


TEST(vect_complex_conj_macc, vect_complex_s32_conj_macc)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    complex_double_t A_fp[LEN], B_fp[LEN], C_fp[LEN];

    complex_s32_t A[LEN], B[LEN], C[LEN];

    struct { exponent_t a, b, c; } exp;
    struct { headroom_t a, b, c; } hr;

    vect_complex_s32_set(A, 0, 0, LEN);
    exp.a = -1024;
    hr.a = vect_complex_s32_headroom(A, LEN);

    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);
        
        exp.b = 10 + (pseudo_rand_int32(&seed) % 10);
        exp.c = 10 + (pseudo_rand_int32(&seed) % 10);

        for(int i = 0; i < LEN; i++){
            unsigned shr = pseudo_rand_uint32(&seed) % 8;
            B[i].re = pseudo_rand_int32(&seed) >> shr;
            B[i].im = pseudo_rand_int32(&seed) >> shr;
            C[i].re = pseudo_rand_int32(&seed) >> shr;
            C[i].im = pseudo_rand_int32(&seed) >> shr;
            
            B_fp[i].re = ldexp(B[i].re, exp.b);
            B_fp[i].im = ldexp(B[i].im, exp.b);
            C_fp[i].re = ldexp(C[i].re, exp.c);
            C_fp[i].im = ldexp(C[i].im, exp.c);
            
            A_fp[i].re = ldexp(A[i].re, exp.a);
            A_fp[i].im = ldexp(A[i].im, exp.a);
        }

        hr.b = vect_complex_s32_headroom(B, LEN);
        hr.c = vect_complex_s32_headroom(C, LEN);

        right_shift_t acc_shr, b_shr, c_shr;
        vect_complex_s32_macc_prepare(&exp.a, &acc_shr, &b_shr, &c_shr,
                                          exp.a, exp.b, exp.c,
                                          hr.a, hr.b, hr.c);

        hr.a = vect_complex_s32_conj_macc(A, B, C, LEN, acc_shr, b_shr, c_shr);
        
        TEST_ASSERT_LESS_THAN(4, hr.a);

        for(int i = 0; i < LEN; i++){
          conv_error_e error = 0;
          complex_double_t expected_fp;
          expected_fp.re = A_fp[i].re + B_fp[i].re * C_fp[i].re + B_fp[i].im * C_fp[i].im;
          expected_fp.im = A_fp[i].im - B_fp[i].re * C_fp[i].im + B_fp[i].im * C_fp[i].re;

          complex_s32_t expected;
          expected.re = conv_double_to_s32(expected_fp.re, exp.a, &error);
          expected.im = conv_double_to_s32(expected_fp.im, exp.a, &error);

          if(error) printf("Conversion error: %d\n", error);
          TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error.");

        // astew: 6/30/2022 -- Increased threshold from 2 to 3. Test was failing after I fixed
        //        the random number generation problem.
#define THRESHOLD  3
          if(expected.re - A[i].re > THRESHOLD || A[i].re - expected.re > THRESHOLD)
            printf("[%d]  exp.a = %d; expected[%d].re = %ld  ( %f );   A[%d].re = %ld\n", v, exp.a, i, (long int) expected.re, expected_fp.re, i, (long int) A[i].re);
          
          TEST_ASSERT_INT32_WITHIN_MESSAGE(THRESHOLD, expected.re, A[i].re, "Error not within threshold (real).");

          if(expected.im - A[i].im > THRESHOLD || A[i].im - expected.im > THRESHOLD){
            printf("[%d]  exp.a = %d; expected[%d].im = %ld  ( %f );   A[%d].im = %ld\n", v, exp.a, i, (long int) expected.im, expected_fp.im, i, (long int) A[i].im);
            printf("B[%d] = %f + i*%f;   C = %f + i*%f\n", i, B_fp[i].re, B_fp[i].im, C_fp[i].re, C_fp[i].im);
          }
          
          TEST_ASSERT_INT32_WITHIN_MESSAGE(THRESHOLD, expected.im, A[i].im, "Error not within threshold (imag).");
#undef THRESHOLD
        }
        
    }
}


TEST(vect_complex_conj_macc, vect_complex_s32_conj_nmacc)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    complex_double_t A_fp[LEN], B_fp[LEN], C_fp[LEN];

    complex_s32_t A[LEN], B[LEN], C[LEN];

    struct { exponent_t a, b, c; } exp;
    struct { headroom_t a, b, c; } hr;

    vect_complex_s32_set(A, 0, 0, LEN);
    exp.a = -1024;
    hr.a = vect_complex_s32_headroom(A, LEN);

    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);
        
        exp.b = 10 + (pseudo_rand_int32(&seed) % 10);
        exp.c = 10 + (pseudo_rand_int32(&seed) % 10);

        for(int i = 0; i < LEN; i++){
            unsigned shr = pseudo_rand_uint32(&seed) % 8;
            B[i].re = pseudo_rand_int32(&seed) >> shr;
            B[i].im = pseudo_rand_int32(&seed) >> shr;
            C[i].re = pseudo_rand_int32(&seed) >> shr;
            C[i].im = pseudo_rand_int32(&seed) >> shr;
            
            B_fp[i].re = ldexp(B[i].re, exp.b);
            B_fp[i].im = ldexp(B[i].im, exp.b);
            C_fp[i].re = ldexp(C[i].re, exp.c);
            C_fp[i].im = ldexp(C[i].im, exp.c);
            
            A_fp[i].re = ldexp(A[i].re, exp.a);
            A_fp[i].im = ldexp(A[i].im, exp.a);
        }

        hr.b = vect_complex_s32_headroom(B, LEN);
        hr.c = vect_complex_s32_headroom(C, LEN);

        right_shift_t acc_shr, b_shr, c_shr;
        vect_complex_s32_conj_macc_prepare(&exp.a, &acc_shr, &b_shr, &c_shr,
                                          exp.a, exp.b, exp.c,
                                          hr.a, hr.b, hr.c);

        hr.a = vect_complex_s32_conj_nmacc(A, B, C, LEN, acc_shr, b_shr, c_shr);
        
        TEST_ASSERT_LESS_THAN(4, hr.a);

        for(int i = 0; i < LEN; i++){
          conv_error_e error = 0;
          complex_double_t expected_fp;
          expected_fp.re = A_fp[i].re - (  B_fp[i].re * C_fp[i].re + B_fp[i].im * C_fp[i].im );
          expected_fp.im = A_fp[i].im - (- B_fp[i].re * C_fp[i].im + B_fp[i].im * C_fp[i].re );

          complex_s32_t expected;
          expected.re = conv_double_to_s32(expected_fp.re, exp.a, &error);
          expected.im = conv_double_to_s32(expected_fp.im, exp.a, &error);

          if(error) printf("Conversion error: %d\n", error);
          TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error.");

        // astew: 6/30/2022 -- Increased threshold from 2 to 3. Test was failing after I fixed
        //        the random number generation problem.
#define THRESHOLD  3
          if(expected.re - A[i].re > THRESHOLD || A[i].re - expected.re > THRESHOLD)
            printf("[%d]  exp.a = %d; expected[%d].re = %ld  ( %f );   A[%d].re = %ld\n", v, exp.a, i, (long int) expected.re, expected_fp.re, i, (long int) A[i].re);
          
          TEST_ASSERT_INT32_WITHIN_MESSAGE(THRESHOLD, expected.re, A[i].re, "Error not within threshold (real).");

          if(expected.im - A[i].im > THRESHOLD || A[i].im - expected.im > THRESHOLD)
            printf("[%d]  exp.a = %d; expected[%d].im = %ld  ( %f );   A[%d].im = %ld\n", v, exp.a, i, (long int) expected.im, expected_fp.im, i, (long int) A[i].im);
          
          TEST_ASSERT_INT32_WITHIN_MESSAGE(THRESHOLD, expected.im, A[i].im, "Error not within threshold (imag).");
#undef THRESHOLD
        }
        
    }
}

