// Copyright 2020 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public License: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "bfp_math.h"

#include "../../tst_common.h"

#include "unity.h"

#if DEBUG_ON || 0
#undef DEBUG_ON
#define DEBUG_ON    (1)
#endif


#define REPS        (100)
#define MAX_LEN     40 


static unsigned seed = 666;


static char msg_buff[200];





void test_bfp_complex_s16_real_scale()
{
    PRINTF("%s...\n", __func__);

    seed = 0x7E9C7154;

    struct {
        int16_t real[MAX_LEN];
        int16_t imag[MAX_LEN];
    } A_data, B_data;

    bfp_complex_s16_t A, B;

    struct {
        double real[MAX_LEN];
        double imag[MAX_LEN];
    } Af, Bf;

    struct {
        int16_t real[MAX_LEN];
        int16_t imag[MAX_LEN];
    } expA;


    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        bfp_complex_s16_init(&B, B_data.real, B_data.imag,
            pseudo_rand_int(&seed, -100, 100),
            pseudo_rand_int(&seed, 1, MAX_LEN+1), 0);

        bfp_complex_s16_init(&A, A_data.real, A_data.imag, 0, B.length, 0);

        B.hr = pseudo_rand_uint(&seed, 0, 12);

        headroom_t c_hr = pseudo_rand_uint(&seed, 0, 12);

        float_s16_t C = {
            pseudo_rand_int16(&seed) >> c_hr,
            pseudo_rand_int(&seed, -100, 100),
        };

        while(HR_S16(C.mant) != c_hr) C.mant = pseudo_rand_int16(&seed) >> c_hr;

        TEST_ASSERT_EQUAL_MESSAGE(c_hr, HR_S16(C.mant), "[C headroom is wrong]");

        for(int i = 0; i < B.length; i++){
            B.real[i] = pseudo_rand_int16(&seed) >> B.hr;
            B.imag[i] = pseudo_rand_int16(&seed) >> B.hr;

            Af.real[i] = ldexp(B.real[i], B.exp) * ldexp(C.mant, C.exp);
            Af.imag[i] = ldexp(B.imag[i], B.exp) * ldexp(C.mant, C.exp);
        }

        bfp_complex_s16_headroom(&B);

        bfp_complex_s16_real_scale(&A, &B, C);

        TEST_ASSERT_EQUAL_MESSAGE(xs3_vect_complex_s16_headroom(A.real, A.imag, A.length), A.hr, "[A.hr is wrong.]");

        test_complex_s16_from_double(expA.real, expA.imag, Af.real, Af.imag, MAX_LEN, A.exp);

        for(int i = 0; i < A.length; i++){
            TEST_ASSERT_INT16_WITHIN(1, expA.real[i], A.real[i]);
            TEST_ASSERT_INT16_WITHIN(1, expA.imag[i], A.imag[i]);
        }

        
    }
}







void test_bfp_complex_s32_real_scale()
{
    PRINTF("%s...\n", __func__);

    seed = 0xB9A64595;

    complex_s32_t A_data[MAX_LEN], B_data[MAX_LEN];

    bfp_complex_s32_t A, B;

    struct {
        double real[MAX_LEN];
        double imag[MAX_LEN];
    } Af;

    complex_s32_t expA[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        bfp_complex_s32_init(&B, B_data,
            pseudo_rand_int(&seed, -100, 100),
            pseudo_rand_int(&seed, 1, MAX_LEN+1), 0);

        bfp_complex_s32_init(&A, A_data, 0, B.length, 0);

        B.hr = pseudo_rand_uint(&seed, 0, 28);

        const headroom_t c_hr = pseudo_rand_uint(&seed, 0, 28);

        float_s32_t C = {
            pseudo_rand_int32(&seed) >> c_hr,
            pseudo_rand_int(&seed, -100, 100),
        };

        for(int i = 0; i < B.length; i++){
            B.data[i].re = pseudo_rand_int32(&seed) >> B.hr;
            B.data[i].im = pseudo_rand_int32(&seed) >> B.hr;

            Af.real[i] = ldexp(B.data[i].re, B.exp) * ldexp(C.mant, C.exp);
            Af.imag[i] = ldexp(B.data[i].im, B.exp) * ldexp(C.mant, C.exp);
        }

        bfp_complex_s32_headroom(&B);

        bfp_complex_s32_real_scale(&A, &B, C);
        
        TEST_ASSERT_EQUAL_MESSAGE(xs3_vect_complex_s32_headroom(A.data, A.length), A.hr, "[A.hr is wrong.]");
        
        TEST_ASSERT_LESS_OR_EQUAL_MESSAGE(2, A.hr, "[A.hr is too large.]");

        test_complex_s32_from_double(expA, Af.real, Af.imag, MAX_LEN, A.exp);

        for(int i = 0; i < A.length; i++){
            TEST_ASSERT_INT32_WITHIN(2, expA[i].re, A.data[i].re);
            TEST_ASSERT_INT32_WITHIN(2, expA[i].im, A.data[i].im);
        }
    }
}




void test_bfp_scalar_mul_vect_complex()
{
    SET_TEST_FILE();

    RUN_TEST(test_bfp_complex_s16_real_scale);
    RUN_TEST(test_bfp_complex_s32_real_scale);
}