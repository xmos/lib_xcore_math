// Copyright 2020 XMOS LIMITED. This Software is subject to the terms of the 
// XMOS Public License: Version 1

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xs3_math.h"
#include "xs3_vpu_scalar_ops.h"
#include "testing.h"

#include "../tst_common.h"

#include "unity.h"



#define REPS    1000
#define LEN     257

static void test_xs3_vect_s16_macc()
{
    printf("%s...\n", __func__);
    unsigned seed = 764533;

    double A_fp[LEN], B_fp[LEN], C_fp[LEN];
    int16_t A[LEN], B[LEN], C[LEN];

    struct { exponent_t a, b, c; } exp;
    struct { headroom_t a, b, c; } hr;

    xs3_vect_s16_set(A, 0, LEN);
    exp.a = -1024;
    hr.a = xs3_vect_s16_headroom(A, LEN);

    for(int v = 0; v < REPS; v++){
        
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

        hr.b = xs3_vect_s16_headroom(B, LEN);
        hr.c = xs3_vect_s16_headroom(C, LEN);

        right_shift_t acc_shr, bc_shr;
        xs3_vect_s16_macc_prepare(&exp.a, &acc_shr, &bc_shr, 
                                  exp.a, exp.b, exp.c, 
                                  hr.a, hr.b, hr.c);

        hr.a = xs3_vect_s16_macc(A, B, C, LEN, acc_shr, bc_shr);
        
        TEST_ASSERT_LESS_THAN(4, hr.a);

        for(int i = 0; i < LEN; i++){
          conv_error_e error = 0;
          double expected_fp = A_fp[i] + B_fp[i] * C_fp[i];
          int16_t expected = conv_double_to_s16(expected_fp, exp.a, &error);
          if(error)
            printf("Conversion error: %d\n", error);
          TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error.");

#define THRESHOLD  2
          if(expected - A[i] > THRESHOLD || A[i] - expected > THRESHOLD){
            printf("[%d]  exp.a = %d; expected[%d] = %d  ( %f );   A[%d] = %d\n", v, exp.a, i, expected, expected_fp, i, A[i]);
          }
          TEST_ASSERT_INT16_WITHIN_MESSAGE(THRESHOLD, expected, A[i], "Error not within threshold.");
#undef THRESHOLD
        }
        
    }
}

static void test_xs3_vect_s16_nmacc()
{
    printf("%s...\n", __func__);
    unsigned seed = 764533;

    double A_fp[LEN], B_fp[LEN], C_fp[LEN];
    int16_t A[LEN], B[LEN], C[LEN];

    struct { exponent_t a, b, c; } exp;
    struct { headroom_t a, b, c; } hr;

    xs3_vect_s16_set(A, 0, LEN);
    exp.a = -1024;
    hr.a = xs3_vect_s16_headroom(A, LEN);

    for(int v = 0; v < REPS; v++){
        
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

        hr.b = xs3_vect_s16_headroom(B, LEN);
        hr.c = xs3_vect_s16_headroom(C, LEN);

        right_shift_t acc_shr, bc_shr;
        xs3_vect_s16_macc_prepare(&exp.a, &acc_shr, &bc_shr, 
                                  exp.a, exp.b, exp.c, 
                                  hr.a, hr.b, hr.c);

        hr.a = xs3_vect_s16_nmacc(A, B, C, LEN, acc_shr, bc_shr);
        
        TEST_ASSERT_LESS_THAN(4, hr.a);

        for(int i = 0; i < LEN; i++){
          conv_error_e error = 0;
          double expected_fp = A_fp[i] - B_fp[i] * C_fp[i];
          int16_t expected = conv_double_to_s16(expected_fp, exp.a, &error);
          if(error)
            printf("Conversion error: %d\n", error);
          TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error.");

#define THRESHOLD  2
          if(expected - A[i] > THRESHOLD || A[i] - expected > THRESHOLD){
            printf("[%d]  exp.a = %d; expected[%d] = %d  ( %f );   A[%d] = %d\n", v, exp.a, i, expected, expected_fp, i, A[i]);
          }
          TEST_ASSERT_INT16_WITHIN_MESSAGE(THRESHOLD, expected, A[i], "Error not within threshold.");
#undef THRESHOLD
        }
        
    }
}

static void test_xs3_vect_s32_macc()
{
    printf("%s...\n", __func__);
    unsigned seed = 764533;

    double A_fp[LEN], B_fp[LEN], C_fp[LEN];
    int32_t A[LEN], B[LEN], C[LEN];

    struct { exponent_t a, b, c; } exp;
    struct { headroom_t a, b, c; } hr;

    xs3_vect_s32_set(A, 0, LEN);
    exp.a = -1024;
    hr.a = xs3_vect_s32_headroom(A, LEN);

    for(int v = 0; v < REPS; v++){
        
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

        hr.b = xs3_vect_s32_headroom(B, LEN);
        hr.c = xs3_vect_s32_headroom(C, LEN);

        right_shift_t acc_shr, b_shr, c_shr;
        xs3_vect_s32_macc_prepare(&exp.a, &acc_shr, &b_shr, &c_shr, 
                                              exp.a, exp.b, exp.c, 
                                              hr.a, hr.b, hr.c);

        hr.a = xs3_vect_s32_macc(A, B, C, LEN, acc_shr, b_shr, c_shr);
        
        TEST_ASSERT_LESS_THAN(4, hr.a);

        for(int i = 0; i < LEN; i++){
          conv_error_e error = 0;
          double expected_fp = A_fp[i] + B_fp[i] * C_fp[i];
          int32_t expected = conv_double_to_s32(expected_fp, exp.a, &error);
          if(error)
            printf("Conversion error: %d\n", error);
          TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error.");

#define THRESHOLD  2
          if(expected - A[i] > THRESHOLD || A[i] - expected > THRESHOLD){
            printf("[%d]  exp.a = %d; expected[%d] = %ld  ( %f );   A[%d] = %ld\n", v, exp.a, i, expected, expected_fp, i, A[i]);
          }
          TEST_ASSERT_INT32_WITHIN_MESSAGE(THRESHOLD, expected, A[i], "Error not within threshold.");
#undef THRESHOLD
        }
        
    }
}



static void test_xs3_vect_s32_nmacc()
{
    printf("%s...\n", __func__);
    unsigned seed = 764533;

    double A_fp[LEN], B_fp[LEN], C_fp[LEN];
    int32_t A[LEN], B[LEN], C[LEN];

    struct { exponent_t a, b, c; } exp;
    struct { headroom_t a, b, c; } hr;

    xs3_vect_s32_set(A, 0, LEN);
    exp.a = -1024;
    hr.a = xs3_vect_s32_headroom(A, LEN);

    for(int v = 0; v < REPS; v++){
        
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

        hr.b = xs3_vect_s32_headroom(B, LEN);
        hr.c = xs3_vect_s32_headroom(C, LEN);

        right_shift_t acc_shr, b_shr, c_shr;
        xs3_vect_s32_macc_prepare(&exp.a, &acc_shr, &b_shr, &c_shr, 
                                              exp.a, exp.b, exp.c, 
                                              hr.a, hr.b, hr.c);

        hr.a = xs3_vect_s32_nmacc(A, B, C, LEN, acc_shr, b_shr, c_shr);
        
        TEST_ASSERT_LESS_THAN(4, hr.a);

        for(int i = 0; i < LEN; i++){
          conv_error_e error = 0;
          double expected_fp = A_fp[i] - B_fp[i] * C_fp[i];
          int32_t expected = conv_double_to_s32(expected_fp, exp.a, &error);
          if(error)
            printf("Conversion error: %d\n", error);
          TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error.");

#define THRESHOLD  2
          if(expected - A[i] > THRESHOLD || A[i] - expected > THRESHOLD){
            printf("[%d]  exp.a = %d; expected[%d] = %ld  ( %f );   A[%d] = %ld\n", v, exp.a, i, expected, expected_fp, i, A[i]);
          }
          TEST_ASSERT_INT32_WITHIN_MESSAGE(THRESHOLD, expected, A[i], "Error not within threshold.");
#undef THRESHOLD
        }
        
    }
}


void test_xs3_macc_vect()
{
    SET_TEST_FILE();

    RUN_TEST(test_xs3_vect_s16_macc);
    RUN_TEST(test_xs3_vect_s16_nmacc);

    RUN_TEST(test_xs3_vect_s32_macc);
    RUN_TEST(test_xs3_vect_s32_nmacc);
}
