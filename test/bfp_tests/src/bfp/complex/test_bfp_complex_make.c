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


TEST_GROUP_RUNNER(bfp_complex_make) {
  RUN_TEST_CASE(bfp_complex_make, bfp_complex_s32_make);
  RUN_TEST_CASE(bfp_complex_make, bfp_complex_s32_real_part);
  RUN_TEST_CASE(bfp_complex_make, bfp_complex_s32_imag_part);
}

TEST_GROUP(bfp_complex_make);
TEST_SETUP(bfp_complex_make) { fflush(stdout); }
TEST_TEAR_DOWN(bfp_complex_make) {}

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




TEST(bfp_complex_make, bfp_complex_s32_make)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    complex_s32_t DWORD_ALIGNED dataA[MAX_LEN];
    int32_t dataB[MAX_LEN];
    int32_t dataC[MAX_LEN];

    complex_s32_t  expA[MAX_LEN];

    bfp_complex_s32_t A;
    bfp_s32_t B, C;

    A.data = dataA;
    B.data = dataB;
    C.data = dataC;

    struct {
        double real[MAX_LEN];
        double imag[MAX_LEN];
    } Af;

    double Bf[MAX_LEN];
    double Cf[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        test_random_bfp_s32(&B, MAX_LEN, &seed, NULL, 0);
        test_random_bfp_s32(&C, MAX_LEN, &seed, NULL, B.length);
        A.length = B.length;

        test_double_from_s32(Bf, &B);
        test_double_from_s32(Cf, &C);

        for(unsigned int i = 0; i < B.length; i++){
            Af.real[i] = Bf[i];
            Af.imag[i] = Cf[i];
        }

        bfp_s32_headroom(&B);
        bfp_s32_headroom(&C);

        bfp_complex_s32_make(&A, &B, &C);

        test_complex_s32_from_double(expA, Af.real, Af.imag, MAX_LEN, A.exp);

        for(unsigned int i = 0; i < A.length; i++){
            TEST_ASSERT_INT32_WITHIN_MESSAGE(1, expA[i].re, A.data[i].re, "");
            TEST_ASSERT_INT32_WITHIN_MESSAGE(1, expA[i].im, A.data[i].im, "");
        }
    }
}


TEST(bfp_complex_make, bfp_complex_s32_real_part)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    int32_t dataA[MAX_LEN];
    complex_s32_t DWORD_ALIGNED dataB[MAX_LEN];

    int32_t  expA[MAX_LEN];

    bfp_s32_t A;
    bfp_complex_s32_t B;

    A.data = dataA;
    B.data = dataB;

    double Af[MAX_LEN];
    struct {
        double real[MAX_LEN];
        double imag[MAX_LEN];
    } Bf;


    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        test_random_bfp_complex_s32(&B, MAX_LEN, &seed, NULL, 0);
        A.length = B.length;

        test_double_from_complex_s32(Bf.real, Bf.imag, &B);

        for(unsigned int i = 0; i < B.length; i++){
            Af[i] = Bf.real[i];
        }

        bfp_complex_s32_headroom(&B);

        bfp_complex_s32_real_part(&A, &B);

        test_s32_from_double(expA, Af, MAX_LEN, A.exp);

        for(unsigned int i = 0; i < A.length; i++){
            TEST_ASSERT_INT32_WITHIN_MESSAGE(1, expA[i], A.data[i], "");
        }
    }
}


TEST(bfp_complex_make, bfp_complex_s32_imag_part)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    int32_t dataA[MAX_LEN];
    complex_s32_t DWORD_ALIGNED dataB[MAX_LEN];

    int32_t  expA[MAX_LEN];

    bfp_s32_t A;
    bfp_complex_s32_t B;

    A.data = dataA;
    B.data = dataB;

    double Af[MAX_LEN];
    struct {
        double real[MAX_LEN];
        double imag[MAX_LEN];
    } Bf;


    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        test_random_bfp_complex_s32(&B, MAX_LEN, &seed, NULL, 0);
        A.length = B.length;

        test_double_from_complex_s32(Bf.real, Bf.imag, &B);

        for(unsigned int i = 0; i < B.length; i++){
            Af[i] = Bf.imag[i];
        }

        bfp_complex_s32_headroom(&B);

        bfp_complex_s32_imag_part(&A, &B);

        test_s32_from_double(expA, Af, MAX_LEN, A.exp);

        for(unsigned int i = 0; i < A.length; i++){
            TEST_ASSERT_INT32_WITHIN_MESSAGE(1, expA[i], A.data[i], "");
        }
    }
}

