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


TEST_GROUP_RUNNER(bfp_complex_mul) {
  RUN_TEST_CASE(bfp_complex_mul, bfp_complex_s16_mul);
  RUN_TEST_CASE(bfp_complex_mul, bfp_complex_s32_mul);
}

TEST_GROUP(bfp_complex_mul);
TEST_SETUP(bfp_complex_mul) { fflush(stdout); }
TEST_TEAR_DOWN(bfp_complex_mul) {}

#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (128)
#else
#  define REPS       (1000)
#  define MAX_LEN    (512)
#endif

// static char msg_buff[200];

#define TEST_ASSERT_EQUAL_MSG(EXPECTED, ACTUAL, EXTRA, LINE_NUM)   do{          \
    if((EXPECTED)!=(ACTUAL)) {                                                  \
      sprintf(msg_buff, "%s (test vector @ line %u)", (EXTRA), (LINE_NUM));     \
      TEST_ASSERT_EQUAL_MESSAGE((EXPECTED), (ACTUAL), msg_buff);                \
    }} while(0)


TEST(bfp_complex_mul, bfp_complex_s16_mul)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

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
        setExtraInfo_RS(r, seed);

        test_random_bfp_complex_s16(&B, MAX_LEN, &seed, &A, 0);
        // B.length = 1;
        test_random_bfp_complex_s16(&C, MAX_LEN, &seed, &A, B.length);

        test_double_from_complex_s16(Bf.real, Bf.imag, &B);
        test_double_from_complex_s16(Cf.real, Cf.imag, &C);

        for(unsigned int i = 0; i < B.length; i++){
            Af.real[i] = Bf.real[i] * Cf.real[i] - Bf.imag[i] * Cf.imag[i];
            Af.imag[i] = Bf.real[i] * Cf.imag[i] + Bf.imag[i] * Cf.real[i];
        }

        bfp_complex_s16_mul(&A, &B, &C);

        test_complex_s16_from_double(expA.real, expA.imag, Af.real, Af.imag, MAX_LEN, A.exp);

        for(unsigned int i = 0; i < A.length; i++){
            TEST_ASSERT_INT16_WITHIN(1, expA.real[i], A.real[i]);
            TEST_ASSERT_INT16_WITHIN(1, expA.imag[i], A.imag[i]);
        }
    }
}


TEST(bfp_complex_mul, bfp_complex_s32_mul)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

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
        setExtraInfo_RS(r, seed);

        test_random_bfp_complex_s32(&B, MAX_LEN, &seed, &A, 0);
        test_random_bfp_complex_s32(&C, MAX_LEN, &seed, &A, B.length);

        //Just to make the test easier.
        for(unsigned int i = 0; i < B.length; i++){
            B.data[i].re = B.data[i].re & 0xFFFFFFFE;
            C.data[i].re = C.data[i].re & 0xFFFFFFFE;
            B.data[i].im = B.data[i].im & 0xFFFFFFFE;
            C.data[i].im = C.data[i].im & 0xFFFFFFFE;
        }

        test_double_from_complex_s32(Bf.real, Bf.imag, &B);
        test_double_from_complex_s32(Cf.real, Cf.imag, &C);

        for(unsigned int i = 0; i < B.length; i++){
            Af.real[i] = Bf.real[i] * Cf.real[i] - Bf.imag[i] * Cf.imag[i];
            Af.imag[i] = Bf.real[i] * Cf.imag[i] + Bf.imag[i] * Cf.real[i];
        }

        bfp_complex_s32_mul(&A, &B, &C);

        test_complex_s32_from_double(expA, Af.real, Af.imag, MAX_LEN, A.exp);

        for(unsigned int i = 0; i < A.length; i++){
            TEST_ASSERT_INT32_WITHIN(1, expA[i].re, A.data[i].re);
            TEST_ASSERT_INT32_WITHIN(1, expA[i].im, A.data[i].im);
        }
    }
}
