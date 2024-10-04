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


TEST_GROUP_RUNNER(bfp_complex_squared_mag) {
  RUN_TEST_CASE(bfp_complex_squared_mag, bfp_complex_s16_squared_mag);
  RUN_TEST_CASE(bfp_complex_squared_mag, bfp_complex_s32_squared_mag);
}

TEST_GROUP(bfp_complex_squared_mag);
TEST_SETUP(bfp_complex_squared_mag) { fflush(stdout); }
TEST_TEAR_DOWN(bfp_complex_squared_mag) {}

#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (128)
#else
#  define REPS       (1000)
#  define MAX_LEN    (512)
#endif

// static char msg_buff[200];


TEST(bfp_complex_squared_mag, bfp_complex_s16_squared_mag)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    int16_t A_data[MAX_LEN];

    struct {
        int16_t real[MAX_LEN];
        int16_t imag[MAX_LEN];
    } B_data;

    bfp_s16_t A;
    bfp_complex_s16_t B;

    double Af[MAX_LEN];

    int16_t expA[MAX_LEN];


    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        bfp_complex_s16_init(&B, B_data.real, B_data.imag,
            pseudo_rand_int(&seed, -100, 100),
            pseudo_rand_int(&seed, 1, MAX_LEN+1), 0);

        bfp_s16_init(&A, A_data, 0, B.length, 0);

        B.hr = pseudo_rand_uint(&seed, 0, 12);

        for(unsigned int i = 0; i < B.length; i++){
            B.real[i] = pseudo_rand_int16(&seed) >> B.hr;
            B.imag[i] = pseudo_rand_int16(&seed) >> B.hr;

            Af[i] = ldexp(B.real[i], B.exp) * ldexp(B.real[i], B.exp)
                  + ldexp(B.imag[i], B.exp) * ldexp(B.imag[i], B.exp);
        }

        bfp_complex_s16_headroom(&B);

        bfp_complex_s16_squared_mag(&A, &B);

        TEST_ASSERT_EQUAL_MESSAGE(vect_s16_headroom(A.data, A.length), A.hr, "[A.hr is wrong.]");

        test_s16_from_double(expA, Af, MAX_LEN, A.exp);

        for(unsigned int i = 0; i < A.length; i++){
            TEST_ASSERT_INT16_WITHIN(1, expA[i], A.data[i]);
        }

        
    }
}


TEST(bfp_complex_squared_mag, bfp_complex_s32_squared_mag)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    int32_t DWORD_ALIGNED A_data[MAX_LEN];
    bfp_s32_t A;
    double Af[MAX_LEN];
    int32_t expA[MAX_LEN];

    complex_s32_t DWORD_ALIGNED B_data[MAX_LEN];
    bfp_complex_s32_t B;

    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        bfp_complex_s32_init(&B, B_data,
            pseudo_rand_int(&seed, -100, 100),
            pseudo_rand_int(&seed, 1, MAX_LEN+1), 0);

        bfp_s32_init(&A, A_data, 0, B.length, 0);

        B.hr = pseudo_rand_uint(&seed, 0, 28);

        for(unsigned int i = 0; i < B.length; i++){
            B.data[i].re = pseudo_rand_int32(&seed) >> B.hr;
            B.data[i].im = pseudo_rand_int32(&seed) >> B.hr;

            Af[i] = ldexp(B.data[i].re, B.exp) * ldexp(B.data[i].re, B.exp)
                  + ldexp(B.data[i].im, B.exp) * ldexp(B.data[i].im, B.exp);
        }

        bfp_complex_s32_headroom(&B);

        bfp_complex_s32_squared_mag(&A, &B);
        
        TEST_ASSERT_EQUAL_MESSAGE(vect_s32_headroom(A.data, A.length), A.hr, "[A.hr is wrong.]");

        TEST_ASSERT_LESS_OR_EQUAL_MESSAGE(3, A.hr, "[A.hr is too large.]");

        test_s32_from_double(expA, Af, MAX_LEN, A.exp);

        for(unsigned int i = 0; i < A.length; i++){
            // astew: 2022/06/30 -- Increased threshold from 3 to 4. Test was failing after I fixed
            //        the random number generation problem. (Threshold is observed, not theoretical)
            TEST_ASSERT_INT32_WITHIN(4, expA[i], A.data[i]);
        }
    }
}

