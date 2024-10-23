// Copyright 2020-2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xmath/xmath.h"

#include "../../tst_common.h"

#include "unity_fixture.h"


TEST_GROUP_RUNNER(bfp_max_elementwise) {
  RUN_TEST_CASE(bfp_max_elementwise, bfp_s32_max_elementwise);
  RUN_TEST_CASE(bfp_max_elementwise, bfp_s16_max_elementwise);
}

TEST_GROUP(bfp_max_elementwise);
TEST_SETUP(bfp_max_elementwise) { fflush(stdout); }
TEST_TEAR_DOWN(bfp_max_elementwise) {}

#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (128)
#else
#  define REPS       (1000)
#  define MAX_LEN    (512)
#endif


TEST(bfp_max_elementwise, bfp_s16_max_elementwise)
{

    unsigned seed = SEED_FROM_FUNC_NAME();

    int16_t dataA[MAX_LEN];
    int16_t dataB[MAX_LEN];
    int16_t dataC[MAX_LEN];
    int16_t expA[MAX_LEN];
    bfp_s16_t A, B, C;

    A.data = dataA;
    B.data = dataB;
    C.data = dataC;

    double Af[MAX_LEN];
    double Bf[MAX_LEN];
    double Cf[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        test_random_bfp_s16(&B, MAX_LEN, &seed, &A, 0);
        test_random_bfp_s16(&C, MAX_LEN, &seed, &A, B.length);
        C.exp = B.exp + pseudo_rand_int(&seed, -10, 10);

        test_double_from_s16(Bf, &B);
        test_double_from_s16(Cf, &C);

        for(unsigned int i = 0; i < B.length; i++){
            Af[i] = (Bf[i] >= Cf[i])? Bf[i] : Cf[i];
        }

        bfp_s16_max_elementwise(&A, &B, &C);

        test_s16_from_double(expA, Af, MAX_LEN, A.exp);

        for(unsigned int i = 0; i < A.length; i++){
            TEST_ASSERT_INT16_WITHIN(1, expA[i], A.data[i]);
        }
    }
}


TEST(bfp_max_elementwise, bfp_s32_max_elementwise)
{


    unsigned seed = SEED_FROM_FUNC_NAME();

    int32_t dataA[MAX_LEN];
    int32_t dataB[MAX_LEN];
    int32_t dataC[MAX_LEN];
    int32_t expA[MAX_LEN];
    bfp_s32_t A, B, C;

    A.data = dataA;
    B.data = dataB;
    C.data = dataC;

    double Af[MAX_LEN];
    double Bf[MAX_LEN];
    double Cf[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        test_random_bfp_s32(&B, MAX_LEN, &seed, &A, 0);
        test_random_bfp_s32(&C, MAX_LEN, &seed, &A, B.length);
        C.exp = B.exp + pseudo_rand_int(&seed, -10, 10);

        //Just to make the test easier.
        for(unsigned int i = 0; i < B.length; i++){
            B.data[i] = B.data[i] & 0xFFFFFFFE;
            C.data[i] = C.data[i] & 0xFFFFFFFE;
        }

        bfp_s32_headroom(&B);
        bfp_s32_headroom(&C);

        // printf("B.hr = %u\n", B.hr);
        // printf("C.hr = %u\n", C.hr);

        test_double_from_s32(Bf, &B);
        test_double_from_s32(Cf, &C);

        for(unsigned int i = 0; i < B.length; i++){
            Af[i] = (Bf[i] >= Cf[i])? Bf[i] : Cf[i];
        }

        bfp_s32_max_elementwise(&A, &B, &C);

        test_s32_from_double(expA, Af, MAX_LEN, A.exp);

        for(unsigned int i = 0; i < A.length; i++){
            // printf("B[%d] = %e = %ld * 2**(%d)\n", i, ldexp(B.data[i], B.exp), B.data[i], B.exp);
            // printf("C[%d] = %e = %ld * 2**(%d)\n", i, ldexp(C.data[i], C.exp), C.data[i], C.exp);
            // printf("A[%d] = %e = %ld * 2**(%d)\n", i, ldexp(A.data[i], A.exp), A.data[i], A.exp);
            // printf("expA[%d] = %e = %ld * 2**(%d)\n", i, ldexp(expA[i], A.exp), expA[i], A.exp);
            TEST_ASSERT_INT32_WITHIN(1, expA[i], A.data[i]);
        }
    }
}