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

#include "../tst_common.h"

#include "unity_fixture.h"


TEST_GROUP_RUNNER(vect_macc) {
  RUN_TEST_CASE(vect_macc, vect_s16_macc);
  RUN_TEST_CASE(vect_macc, vect_s16_nmacc);
  RUN_TEST_CASE(vect_macc, vect_s32_macc);
  RUN_TEST_CASE(vect_macc, vect_s32_nmacc);
}

TEST_GROUP(vect_macc);
TEST_SETUP(vect_macc) { fflush(stdout); }
TEST_TEAR_DOWN(vect_macc) {}




#if SMOKE_TEST
#  define REPS       (100)
#  define LEN        (65)
#else
#  define REPS       (1000)
#  define LEN        (257)
#endif

TEST(vect_macc, vect_s16_macc)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    double A_fp[LEN], B_fp[LEN], C_fp[LEN];
    int16_t A[LEN], B[LEN], C[LEN];

    struct { exponent_t a, b, c; } exp;
    struct { headroom_t a, b, c; } hr;

    vect_s16_set(A, 0, LEN);
    exp.a = -1024;
    hr.a = vect_s16_headroom(A, LEN);

    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);
        
        exp.b = 10 + (pseudo_rand_int16(&seed) % 10);
        exp.c = 10 + (pseudo_rand_int16(&seed) % 10);

        for(int i = 0; i < LEN; i++){
            unsigned shr = pseudo_rand_uint16(&seed) % 8;
            B[i] = pseudo_rand_int16(&seed) >> shr;
            C[i] = pseudo_rand_int16(&seed) >> shr;
            
            B_fp[i] = ldexp(B[i], exp.b);
            C_fp[i] = ldexp(C[i], exp.c);
            A_fp[i] = ldexp(A[i], exp.a);
        }

        hr.b = vect_s16_headroom(B, LEN);
        hr.c = vect_s16_headroom(C, LEN);

        right_shift_t acc_shr, bc_shr;
        vect_s16_macc_prepare(&exp.a, &acc_shr, &bc_shr, 
                                  exp.a, exp.b, exp.c, 
                                  hr.a, hr.b, hr.c);

        hr.a = vect_s16_macc(A, B, C, LEN, acc_shr, bc_shr);
        
        TEST_ASSERT_LESS_THAN(4, hr.a);

        for(int i = 0; i < LEN; i++){
          conv_error_e error = 0;
          double expected_fp = A_fp[i] + B_fp[i] * C_fp[i];
          int16_t expected = conv_double_to_s16(expected_fp, exp.a, &error);
          if(error)
            printf("Conversion error: %d\n", error);
          TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error.");

#define THRESHOLD  3
          if(expected - A[i] > THRESHOLD || A[i] - expected > THRESHOLD){
            printf("[rep %d]  exp.a = %d; expected[%d] = %d  ( %f );   A[%d] = %d\n", 
              v, exp.a, i, expected, expected_fp, i, A[i]);
          }
          TEST_ASSERT_INT16_WITHIN_MESSAGE(THRESHOLD, expected, A[i], "Error not within threshold.");
#undef THRESHOLD
        }
        
    }
}

TEST(vect_macc, vect_s16_nmacc)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    double A_fp[LEN], B_fp[LEN], C_fp[LEN];
    int16_t A[LEN], B[LEN], C[LEN];

    struct { exponent_t a, b, c; } exp;
    struct { headroom_t a, b, c; } hr;

    vect_s16_set(A, 0, LEN);
    exp.a = -1024;
    hr.a = vect_s16_headroom(A, LEN);

    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);
        
        exp.b = 10 + (pseudo_rand_int16(&seed) % 10);
        exp.c = 10 + (pseudo_rand_int16(&seed) % 10);

        for(int i = 0; i < LEN; i++){
            unsigned shr = pseudo_rand_uint16(&seed) % 8;
            B[i] = pseudo_rand_int16(&seed) >> shr;
            C[i] = pseudo_rand_int16(&seed) >> shr;
            
            B_fp[i] = ldexp(B[i], exp.b);
            C_fp[i] = ldexp(C[i], exp.c);
            A_fp[i] = ldexp(A[i], exp.a);
        }

        hr.b = vect_s16_headroom(B, LEN);
        hr.c = vect_s16_headroom(C, LEN);

        right_shift_t acc_shr, bc_shr;
        vect_s16_nmacc_prepare(&exp.a, &acc_shr, &bc_shr, 
                                  exp.a, exp.b, exp.c, 
                                  hr.a, hr.b, hr.c);

        hr.a = vect_s16_nmacc(A, B, C, LEN, acc_shr, bc_shr);
        
        TEST_ASSERT_LESS_THAN(4, hr.a);

        for(int i = 0; i < LEN; i++){
          conv_error_e error = 0;
          double expected_fp = A_fp[i] - B_fp[i] * C_fp[i];
          int16_t expected = conv_double_to_s16(expected_fp, exp.a, &error);
          if(error)
            printf("Conversion error: %d\n", error);
          TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error.");

#define THRESHOLD  3
          if(expected - A[i] > THRESHOLD || A[i] - expected > THRESHOLD){
            printf("[%d]  exp.a = %d; expected[%d] = %d  ( %f );   A[%d] = %d\n", v, exp.a, i, expected, expected_fp, i, A[i]);
          }
          TEST_ASSERT_INT16_WITHIN_MESSAGE(THRESHOLD, expected, A[i], "Error not within threshold.");
#undef THRESHOLD
        }
        
    }
}

TEST(vect_macc, vect_s32_macc)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    double A_fp[LEN], B_fp[LEN], C_fp[LEN];
    int32_t A[LEN], B[LEN], C[LEN];

    struct { exponent_t a, b, c; } exp;
    struct { headroom_t a, b, c; } hr;

    vect_s32_set(A, 0, LEN);
    exp.a = -1024;
    hr.a = vect_s32_headroom(A, LEN);

    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);
        
        exp.b = 10 + (pseudo_rand_int32(&seed) % 10);
        exp.c = 10 + (pseudo_rand_int32(&seed) % 10);

        for(int i = 0; i < LEN; i++){
            unsigned shr = pseudo_rand_uint32(&seed) % 8;
            B[i] = pseudo_rand_int32(&seed) >> shr;
            C[i] = pseudo_rand_int32(&seed) >> shr;
            
            B_fp[i] = ldexp(B[i], exp.b);
            C_fp[i] = ldexp(C[i], exp.c);
            A_fp[i] = ldexp(A[i], exp.a);
        }

        hr.b = vect_s32_headroom(B, LEN);
        hr.c = vect_s32_headroom(C, LEN);

        right_shift_t acc_shr, b_shr, c_shr;
        vect_s32_macc_prepare(&exp.a, &acc_shr, &b_shr, &c_shr, 
                                              exp.a, exp.b, exp.c, 
                                              hr.a, hr.b, hr.c);

        hr.a = vect_s32_macc(A, B, C, LEN, acc_shr, b_shr, c_shr);
        
        TEST_ASSERT_LESS_THAN(4, hr.a);

        for(int i = 0; i < LEN; i++){
          conv_error_e error = 0;
          double expected_fp = A_fp[i] + B_fp[i] * C_fp[i];
          int32_t expected = conv_double_to_s32(expected_fp, exp.a, &error);
          if(error)
            printf("Conversion error: %d\n", error);
          TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error.");

#define THRESHOLD  3
          if(expected - A[i] > THRESHOLD || A[i] - expected > THRESHOLD){
            printf("[%d]  exp.a = %d; expected[%d] = %ld  ( %f );   A[%d] = %ld\n", v, exp.a, i, (long int) expected, expected_fp, i, (long int) A[i]);
          }
          TEST_ASSERT_INT32_WITHIN_MESSAGE(THRESHOLD, expected, A[i], "Error not within threshold.");
#undef THRESHOLD
        }
        
    }
}



TEST(vect_macc, vect_s32_nmacc)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    double A_fp[LEN], B_fp[LEN], C_fp[LEN];
    int32_t A[LEN], B[LEN], C[LEN];

    struct { exponent_t a, b, c; } exp;
    struct { headroom_t a, b, c; } hr;

    vect_s32_set(A, 0, LEN);
    exp.a = -1024;
    hr.a = vect_s32_headroom(A, LEN);

    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);
        
        exp.b = 10 + (pseudo_rand_int32(&seed) % 10);
        exp.c = 10 + (pseudo_rand_int32(&seed) % 10);

        for(int i = 0; i < LEN; i++){
            unsigned shr = pseudo_rand_uint32(&seed) % 8;
            B[i] = pseudo_rand_int32(&seed) >> shr;
            C[i] = pseudo_rand_int32(&seed) >> shr;
            
            B_fp[i] = ldexp(B[i], exp.b);
            C_fp[i] = ldexp(C[i], exp.c);
            A_fp[i] = ldexp(A[i], exp.a);
        }

        hr.b = vect_s32_headroom(B, LEN);
        hr.c = vect_s32_headroom(C, LEN);

        right_shift_t acc_shr, b_shr, c_shr;
        vect_s32_nmacc_prepare(&exp.a, &acc_shr, &b_shr, &c_shr, 
                                              exp.a, exp.b, exp.c, 
                                              hr.a, hr.b, hr.c);

        hr.a = vect_s32_nmacc(A, B, C, LEN, acc_shr, b_shr, c_shr);
        
        TEST_ASSERT_LESS_THAN(4, hr.a);

        for(int i = 0; i < LEN; i++){
          conv_error_e error = 0;
          double expected_fp = A_fp[i] - B_fp[i] * C_fp[i];
          int32_t expected = conv_double_to_s32(expected_fp, exp.a, &error);
          if(error)
            printf("Conversion error: %d\n", error);
          TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error.");

#define THRESHOLD  3
          if(expected - A[i] > THRESHOLD || A[i] - expected > THRESHOLD){
            printf("[%d]  exp.a = %d; expected[%d] = %ld  ( %f );   A[%d] = %ld\n", v, exp.a, i, (long int) expected, expected_fp, i, (long int) A[i]);
          }
          TEST_ASSERT_INT32_WITHIN_MESSAGE(THRESHOLD, expected, A[i], "Error not within threshold.");
#undef THRESHOLD
        }
        
    }
}
