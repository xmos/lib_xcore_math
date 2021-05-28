// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

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


#define REPS        (1000)
#define MAX_LEN     40 


static unsigned seed = 666;


static char msg_buff[200];

#define TEST_ASSERT_EQUAL_MSG(EXPECTED, ACTUAL, EXTRA, LINE_NUM)   do{          \
    if((EXPECTED)!=(ACTUAL)) {                                                  \
      sprintf(msg_buff, "%s (test vector @ line %u)", (EXTRA), (LINE_NUM));     \
      TEST_ASSERT_EQUAL_MESSAGE((EXPECTED), (ACTUAL), msg_buff);                \
    }} while(0)










static void test_bfp_complex_s16_add()
{
    PRINTF("%s...\n", __func__);

    seed = 5674555;

    struct {
        int16_t real[MAX_LEN];
        int16_t imag[MAX_LEN];
    } A_data, B_data, C_data;

    bfp_complex_s16_t A, B, C;

    A.real = A_data.real;
    A.imag = A_data.imag;
    B.real = B_data.real;
    B.imag = B_data.imag;
    C.real = C_data.real;
    C.imag = C_data.imag;

    struct {
        double real[MAX_LEN];
        double imag[MAX_LEN];
    } Af, Bf, Cf;

    struct {
        int16_t real[MAX_LEN];
        int16_t imag[MAX_LEN];
    } expA;


    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        test_random_bfp_complex_s16(&B, MAX_LEN, &seed, &A, 0);
        test_random_bfp_complex_s16(&C, MAX_LEN, &seed, &A, B.length);

        test_double_from_complex_s16(Bf.real, Bf.imag, &B);
        test_double_from_complex_s16(Cf.real, Cf.imag, &C);

        for(int i = 0; i < B.length; i++){
            Af.real[i] = Bf.real[i] + Cf.real[i];
            Af.imag[i] = Bf.imag[i] + Cf.imag[i];
        }

        bfp_complex_s16_add(&A, &B, &C);

        test_complex_s16_from_double(expA.real, expA.imag, Af.real, Af.imag, MAX_LEN, A.exp);

        for(int i = 0; i < A.length; i++){
            TEST_ASSERT_INT16_WITHIN(1, expA.real[i], A.real[i]);
            TEST_ASSERT_INT16_WITHIN(1, expA.imag[i], A.imag[i]);
        }
    }
}









static void test_bfp_complex_s32_add()
{
    PRINTF("%s...\n", __func__);

    seed = 478955;

    complex_s32_t dataA[MAX_LEN];
    complex_s32_t dataB[MAX_LEN];
    complex_s32_t dataC[MAX_LEN];
    complex_s32_t  expA[MAX_LEN];
    bfp_complex_s32_t A, B, C;

    A.data = dataA;
    B.data = dataB;
    C.data = dataC;

    struct {
        double real[MAX_LEN];
        double imag[MAX_LEN];
    } Af, Bf, Cf;

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        test_random_bfp_complex_s32(&B, MAX_LEN, &seed, &A, 0);
        test_random_bfp_complex_s32(&C, MAX_LEN, &seed, &A, B.length);

        //Just to make the test easier.
        for(int i = 0; i < B.length; i++){
            B.data[i].re = B.data[i].re & 0xFFFFFFFE;
            C.data[i].re = C.data[i].re & 0xFFFFFFFE;
            B.data[i].im = B.data[i].im & 0xFFFFFFFE;
            C.data[i].im = C.data[i].im & 0xFFFFFFFE;
        }

        test_double_from_complex_s32(Bf.real, Bf.imag, &B);
        test_double_from_complex_s32(Cf.real, Cf.imag, &C);

        for(int i = 0; i < B.length; i++){
            Af.real[i] = Bf.real[i] + Cf.real[i];
            Af.imag[i] = Bf.imag[i] + Cf.imag[i];
        }

        bfp_complex_s32_add(&A, &B, &C);

        test_complex_s32_from_double(expA, Af.real, Af.imag, MAX_LEN, A.exp);

        for(int i = 0; i < A.length; i++){
            TEST_ASSERT_INT32_WITHIN(1, expA[i].re, A.data[i].re);
            TEST_ASSERT_INT32_WITHIN(1, expA[i].im, A.data[i].im);
        }
    }
}




void test_bfp_add_vect_complex()
{
    SET_TEST_FILE();

    RUN_TEST(test_bfp_complex_s16_add);
    RUN_TEST(test_bfp_complex_s32_add);
}