// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// XMOS Public License: Version 1

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xmath/xmath.h"

#include "../../tst_common.h"

#include "unity_fixture.h"


TEST_GROUP_RUNNER(bfp_macc) {
  RUN_TEST_CASE(bfp_macc, bfp_s16_macc);
  RUN_TEST_CASE(bfp_macc, bfp_s16_nmacc);
  RUN_TEST_CASE(bfp_macc, bfp_s32_macc);
  RUN_TEST_CASE(bfp_macc, bfp_s32_nmacc);
}

TEST_GROUP(bfp_macc);
TEST_SETUP(bfp_macc) { fflush(stdout); }
TEST_TEAR_DOWN(bfp_macc) {}

#if SMOKE_TEST
#  define REPS       (100)
#  define LEN        (128)
#else
#  define REPS       (1000)
#  define LEN        (512)
#endif



TEST(bfp_macc, bfp_s16_macc)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    int16_t dataA[LEN];
    int16_t dataB[LEN];
    int16_t dataC[LEN];
    int16_t expA[LEN];
    bfp_s16_t A, B, C;

    double Af[LEN];

    bfp_s16_init(&A, dataA, -1024, LEN, 0);
    bfp_s16_init(&B, dataB, -1024, LEN, 0);
    bfp_s16_init(&C, dataC, -1024, LEN, 0);

    bfp_s16_set(&A, 0, -1024);

    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        B.exp = 10 + (pseudo_rand_int32(&seed) % 10);
        C.exp = 10 + (pseudo_rand_int32(&seed) % 10);

        for(int i = 0; i < LEN; i++){
            unsigned B_shr = pseudo_rand_uint16(&seed) % 8;
            unsigned C_shr = pseudo_rand_uint16(&seed) % 8;

            B.data[i] = pseudo_rand_int16(&seed) >> B_shr;
            C.data[i] = pseudo_rand_int16(&seed) >> C_shr;

            Af[i] = ldexp(A.data[i], A.exp);
        }

        bfp_s16_headroom(&B);
        bfp_s16_headroom(&C);

        for(unsigned int i = 0; i < B.length; i++){
            Af[i] = Af[i] + ldexp(B.data[i], B.exp) * ldexp(C.data[i], C.exp);
        }

        bfp_s16_macc(&A, &B, &C);

        test_s16_from_double(expA, Af, LEN, A.exp);

        for(unsigned int i = 0; i < A.length; i++){
            TEST_ASSERT_INT16_WITHIN(2, expA[i], A.data[i]);
        }
    }
}


TEST(bfp_macc, bfp_s16_nmacc)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    int16_t dataA[LEN];
    int16_t dataB[LEN];
    int16_t dataC[LEN];
    int16_t expA[LEN];
    bfp_s16_t A, B, C;

    double Af[LEN];

    bfp_s16_init(&A, dataA, -1024, LEN, 0);
    bfp_s16_init(&B, dataB, -1024, LEN, 0);
    bfp_s16_init(&C, dataC, -1024, LEN, 0);

    bfp_s16_set(&A, 0, -1024);

    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        B.exp = 10 + (pseudo_rand_int32(&seed) % 10);
        C.exp = 10 + (pseudo_rand_int32(&seed) % 10);

        for(int i = 0; i < LEN; i++){
            unsigned B_shr = pseudo_rand_uint16(&seed) % 8;
            unsigned C_shr = pseudo_rand_uint16(&seed) % 8;

            B.data[i] = pseudo_rand_int16(&seed) >> B_shr;
            C.data[i] = pseudo_rand_int16(&seed) >> C_shr;

            Af[i] = ldexp(A.data[i], A.exp);
        }

        bfp_s16_headroom(&B);
        bfp_s16_headroom(&C);

        for(unsigned int i = 0; i < B.length; i++){
            Af[i] = Af[i] - ldexp(B.data[i], B.exp) * ldexp(C.data[i], C.exp);
        }

        bfp_s16_nmacc(&A, &B, &C);

        test_s16_from_double(expA, Af, LEN, A.exp);

        for(unsigned int i = 0; i < A.length; i++){
            TEST_ASSERT_INT16_WITHIN(2, expA[i], A.data[i]);
        }
    }
}


TEST(bfp_macc, bfp_s32_macc)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    int32_t dataA[LEN];
    int32_t dataB[LEN];
    int32_t dataC[LEN];
    int32_t expA[LEN];
    bfp_s32_t A, B, C;

    double Af[LEN];

    bfp_s32_init(&A, dataA, -1024, LEN, 0);
    bfp_s32_init(&B, dataB, -1024, LEN, 0);
    bfp_s32_init(&C, dataC, -1024, LEN, 0);

    bfp_s32_set(&A, 0, -1024);

    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        B.exp = 10 + (pseudo_rand_int32(&seed) % 10);
        C.exp = 10 + (pseudo_rand_int32(&seed) % 10);

        for(int i = 0; i < LEN; i++){
            unsigned B_shr = pseudo_rand_uint16(&seed) % 8;
            unsigned C_shr = pseudo_rand_uint16(&seed) % 8;

            B.data[i] = pseudo_rand_int32(&seed) >> B_shr;
            C.data[i] = pseudo_rand_int32(&seed) >> C_shr;

            Af[i] = ldexp(A.data[i], A.exp);
        }

        bfp_s32_headroom(&B);
        bfp_s32_headroom(&C);

        for(unsigned int i = 0; i < B.length; i++){
            Af[i] = Af[i] + ldexp(B.data[i], B.exp) * ldexp(C.data[i], C.exp);
        }

        bfp_s32_macc(&A, &B, &C);

        test_s32_from_double(expA, Af, LEN, A.exp);

        for(unsigned int i = 0; i < A.length; i++){
            TEST_ASSERT_INT32_WITHIN(2, expA[i], A.data[i]);
        }
    }
}


TEST(bfp_macc, bfp_s32_nmacc)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    int32_t dataA[LEN];
    int32_t dataB[LEN];
    int32_t dataC[LEN];
    int32_t expA[LEN];
    bfp_s32_t A, B, C;

    double Af[LEN];

    bfp_s32_init(&A, dataA, -1024, LEN, 0);
    bfp_s32_init(&B, dataB, -1024, LEN, 0);
    bfp_s32_init(&C, dataC, -1024, LEN, 0);

    bfp_s32_set(&A, 0, -1024);

    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        B.exp = 10 + (pseudo_rand_int32(&seed) % 10);
        C.exp = 10 + (pseudo_rand_int32(&seed) % 10);

        for(int i = 0; i < LEN; i++){
            unsigned B_shr = pseudo_rand_uint16(&seed) % 8;
            unsigned C_shr = pseudo_rand_uint16(&seed) % 8;

            B.data[i] = pseudo_rand_int32(&seed) >> B_shr;
            C.data[i] = pseudo_rand_int32(&seed) >> C_shr;

            Af[i] = ldexp(A.data[i], A.exp);
        }

        bfp_s32_headroom(&B);
        bfp_s32_headroom(&C);

        for(unsigned int i = 0; i < B.length; i++){
            Af[i] = Af[i] - ldexp(B.data[i], B.exp) * ldexp(C.data[i], C.exp);
        }

        bfp_s32_nmacc(&A, &B, &C);

        test_s32_from_double(expA, Af, LEN, A.exp);

        for(unsigned int i = 0; i < A.length; i++){
            TEST_ASSERT_INT32_WITHIN(2, expA[i], A.data[i]);
        }
    }
}

