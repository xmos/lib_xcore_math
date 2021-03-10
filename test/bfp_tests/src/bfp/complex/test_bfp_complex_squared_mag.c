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








void test_bfp_complex_s16_squared_mag()
{
    PRINTF("%s...\n", __func__);

    seed = 0x7E9C7154;

    int16_t A_data[MAX_LEN];

    struct {
        int16_t real[MAX_LEN];
        int16_t imag[MAX_LEN];
    } B_data;

    bfp_s16_t A;
    bfp_complex_s16_t B;

    double Af[MAX_LEN];

    struct {
        double real[MAX_LEN];
        double imag[MAX_LEN];
    } Bf;

    int16_t expA[MAX_LEN];



    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        bfp_complex_s16_init(&B, B_data.real, B_data.imag,
            pseudo_rand_int(&seed, -100, 100),
            pseudo_rand_int(&seed, 1, MAX_LEN+1), 0);

        bfp_s16_init(&A, A_data, 0, B.length, 0);

        B.hr = pseudo_rand_uint(&seed, 0, 12);

        for(int i = 0; i < B.length; i++){
            B.real[i] = pseudo_rand_int16(&seed) >> B.hr;
            B.imag[i] = pseudo_rand_int16(&seed) >> B.hr;

            Af[i] = ldexp(B.real[i], B.exp) * ldexp(B.real[i], B.exp) 
                  + ldexp(B.imag[i], B.exp) * ldexp(B.imag[i], B.exp);
        }

        bfp_complex_s16_headroom(&B);

        bfp_complex_s16_squared_mag(&A, &B);

        TEST_ASSERT_EQUAL_MESSAGE(xs3_vect_s16_headroom(A.data, A.length), A.hr, "[A.hr is wrong.]");

        // PRINTF("\t    B.length = %u\n", B.length);
        // PRINTF("\t    B.exp    = %d\n", B.exp);
        // PRINTF("\t    B.hr     = %u\n", B.hr);

        // for(int i = 0; i < B.length; i++){
        //     PRINTF("\t        B.real[% 3d] = % 10d    (0x%04X)\n", i, B.real[i], (unsigned) B.real[i]);
        //     PRINTF("\t        B.imag[% 3d] = % 10d    (0x%04X)\n", i, B.imag[i], (unsigned) B.imag[i]);
        // }

        // PRINTF("\t    C.exp = %d\n", c_exp);
        // PRINTF("\t        C  = % 10d    (0x%04X)\n", C, (unsigned) C);
        
        
        // PRINTF("\t    A.length = %u\n", A.length);
        // PRINTF("\t    A.exp    = %d\n", A.exp);
        // PRINTF("\t    A.hr     = %u\n", A.hr);

        // for(int i = 0; i < A.length; i++){
        //     PRINTF("\t        A.real[% 3d] = % 10d    (0x%04X)\n", i, A.real[i], (unsigned) A.real[i]);
        //     PRINTF("\t        A.imag[% 3d] = % 10d    (0x%04X)\n", i, A.imag[i], (unsigned) A.imag[i]);
        // }

        test_s16_from_double(expA, Af, MAX_LEN, A.exp);

        for(int i = 0; i < A.length; i++){
            TEST_ASSERT_INT16_WITHIN(1, expA[i], A.data[i]);
        }

        
    }
}






void test_bfp_complex_s32_squared_mag()
{
    PRINTF("%s...\n", __func__);

    seed = 0xB9A64595;

    int32_t A_data[MAX_LEN];
    bfp_s32_t A;
    double Af[MAX_LEN];
    int32_t expA[MAX_LEN];

    complex_s32_t B_data[MAX_LEN];
    bfp_complex_s32_t B;
    struct {
        double real[MAX_LEN];
        double imag[MAX_LEN];
    } Bf;

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        bfp_complex_s32_init(&B, B_data,
            pseudo_rand_int(&seed, -100, 100),
            pseudo_rand_int(&seed, 1, MAX_LEN+1), 0);

        bfp_s32_init(&A, A_data, 0, B.length, 0);

        B.hr = pseudo_rand_uint(&seed, 0, 28);

        for(int i = 0; i < B.length; i++){
            B.data[i].re = pseudo_rand_int32(&seed) >> B.hr;
            B.data[i].im = pseudo_rand_int32(&seed) >> B.hr;

            Af[i] = ldexp(B.data[i].re, B.exp) * ldexp(B.data[i].re, B.exp)
                  + ldexp(B.data[i].im, B.exp) * ldexp(B.data[i].im, B.exp);
        }

        bfp_complex_s32_headroom(&B);


        // PRINTF("\t    B.length = %u\n", B.length);
        // PRINTF("\t    B.exp    = %d\n", B.exp);
        // PRINTF("\t    B.hr     = %u\n", B.hr);

        // for(int i = 0; i < B.length; i++){
        //     PRINTF("\t        B.data[% 3d].re = % 15ld    (0x%08X)\n", i, B.data[i].re, (unsigned) B.data[i].re);
        //     PRINTF("\t        B.data[% 3d].im = % 15ld    (0x%08X)\n", i, B.data[i].im, (unsigned) B.data[i].im);
        // }

        bfp_complex_s32_squared_mag(&A, &B);
        
        TEST_ASSERT_EQUAL_MESSAGE(xs3_vect_s32_headroom(A.data, A.length), A.hr, "[A.hr is wrong.]");

        // PRINTF("\t    A.length = %u\n", A.length);
        // PRINTF("\t    A.exp    = %d\n", A.exp);
        // PRINTF("\t    A.hr     = %u\n", A.hr);

        // for(int i = 0; i < A.length; i++){
        //     PRINTF("\t        A.data[% 3d] = % 15ld    (0x%08X)\n", i, A.data[i], (unsigned) A.data[i]);
        // }
        
        TEST_ASSERT_LESS_OR_EQUAL_MESSAGE(3, A.hr, "[A.hr is too large.]");

        test_s32_from_double(expA, Af, MAX_LEN, A.exp);

        for(int i = 0; i < A.length; i++){
            TEST_ASSERT_INT32_WITHIN(2, expA[i], A.data[i]);
        }
    }
}




void test_bfp_squared_mag_vect_complex()
{
    SET_TEST_FILE();

    RUN_TEST(test_bfp_complex_s16_squared_mag);
    RUN_TEST(test_bfp_complex_s32_squared_mag);
}