// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xmath/xmath.h"

#include "../../tst_common.h"

#include "testing.h"

#include "unity_fixture.h"


TEST_GROUP_RUNNER(bfp_depth_convert) {
  RUN_TEST_CASE(bfp_depth_convert, bfp_s32_to_bfp_s16);
  RUN_TEST_CASE(bfp_depth_convert, bfp_s16_to_bfp_s32);
}

TEST_GROUP(bfp_depth_convert);
TEST_SETUP(bfp_depth_convert) { fflush(stdout); }
TEST_TEAR_DOWN(bfp_depth_convert) {}

#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (128)
#else
#  define REPS       (1000)
#  define MAX_LEN    (512)
#endif


TEST(bfp_depth_convert, bfp_s32_to_bfp_s16) 
{


    unsigned seed = SEED_FROM_FUNC_NAME();

    int16_t WORD_ALIGNED dataA[MAX_LEN];
    int32_t WORD_ALIGNED dataB[MAX_LEN];
    bfp_s16_t A;
    bfp_s32_t B;

    A.data = dataA;
    B.data = dataB;

    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        bfp_s32_init(&B, dataB, 
            pseudo_rand_int(&seed, -50, 50),
            pseudo_rand_uint(&seed, 1, MAX_LEN+1), 0);
        
        bfp_s16_init(&A, dataA, 0, B.length, 0);

        B.hr = pseudo_rand_uint(&seed, 0, 28);

        for(unsigned int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int32(&seed) >> B.hr;
        }

        bfp_s32_headroom(&B);

        bfp_s32_to_bfp_s16(&A, &B);

        TEST_ASSERT_EQUAL(vect_s16_headroom(A.data, A.length), A.hr);

        conv_error_e err = 0;
        double Bf[MAX_LEN];

        conv_vect_s32_to_double(Bf, B.data, B.length, B.exp, &err);

        TEST_ASSERT_EQUAL_MESSAGE(0, err, "Conversion to doubles failed.");

        int16_t expected[MAX_LEN];
        
        test_s16_from_double(expected, Bf, B.length, A.exp);

        for(unsigned int i = 0; i < A.length; i++){
            TEST_ASSERT_INT16_WITHIN(1, expected[i], A.data[i]);
        }
    }
}


#undef MAX_LEN
#define MAX_LEN     18  //Smaller lengths mean larger variance w.r.t. individual element headroom


TEST(bfp_depth_convert, bfp_s16_to_bfp_s32) 
{


    unsigned seed = SEED_FROM_FUNC_NAME();

    int32_t dataA[MAX_LEN];
    int16_t dataB[MAX_LEN];
    bfp_s32_t A;
    bfp_s16_t B;

    A.data = dataA;
    B.data = dataB;

    for(int r = 0; r < REPS; r++){

        setExtraInfo_RS(r, seed);

        bfp_s16_init(&B, dataB,
        pseudo_rand_int(&seed, -50, 50),
        pseudo_rand_uint(&seed, 1, MAX_LEN+1), 0);

        bfp_s32_init(&A, dataA, 0, B.length, 0);

        test_random_bfp_s16(&B, MAX_LEN, &seed, NULL, 0);
        A.length = B.length;

        bfp_s16_to_bfp_s32(&A, &B);

        TEST_ASSERT_EQUAL(B.length, A.length);
        TEST_ASSERT_EQUAL(B.hr + 8, A.hr);
        TEST_ASSERT_EQUAL(vect_s32_headroom(A.data, A.length), A.hr);

        TEST_ASSERT_EQUAL(B.exp-8, A.exp);

        for(unsigned int i = 0; i < A.length; i++){
            TEST_ASSERT_EQUAL(((int32_t)B.data[i]) << 8, A.data[i]);
        }
    }
}
