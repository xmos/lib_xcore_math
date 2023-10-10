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


TEST_GROUP_RUNNER(bfp_rect) {
  RUN_TEST_CASE(bfp_rect, bfp_s16_rect);
  RUN_TEST_CASE(bfp_rect, bfp_s32_rect);
}
TEST_GROUP(bfp_rect);
TEST_SETUP(bfp_rect) { fflush(stdout); }
TEST_TEAR_DOWN(bfp_rect) {}

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


TEST(bfp_rect, bfp_s16_rect)
{


    unsigned seed = SEED_FROM_FUNC_NAME();

    int16_t dataA[MAX_LEN];
    int16_t dataB[MAX_LEN];
    bfp_s16_t A, B;

    A.data = dataA;
    B.data = dataB;

    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        test_random_bfp_s16(&B, MAX_LEN, &seed, &A, 0);
        bfp_s16_rect(&A, &B);

        TEST_ASSERT_EQUAL(B.length, B.length);
        TEST_ASSERT_EQUAL(B.exp, A.exp);
        TEST_ASSERT_EQUAL(vect_s16_headroom(A.data, A.length), A.hr);

        for(unsigned int i = 0; i < A.length; i++){
            int16_t expected = (B.data[i] < 0)? 0 : B.data[i];
            TEST_ASSERT_EQUAL(expected, A.data[i]);
        }
    }
}


TEST(bfp_rect, bfp_s32_rect)
{


    unsigned seed = SEED_FROM_FUNC_NAME();

    int32_t dataA[MAX_LEN];
    int32_t dataB[MAX_LEN];
    bfp_s32_t A, B;

    A.data = dataA;
    B.data = dataB;

    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        test_random_bfp_s32(&B, MAX_LEN, &seed, &A, 0);
        bfp_s32_rect(&A, &B);

        TEST_ASSERT_EQUAL(B.length, B.length);
        TEST_ASSERT_EQUAL(B.exp, A.exp);
        TEST_ASSERT_EQUAL(vect_s32_headroom(A.data, A.length), A.hr);

        for(unsigned int i = 0; i < A.length; i++){
            int32_t expected = (B.data[i] < 0)? 0 : B.data[i];
            TEST_ASSERT_EQUAL(expected, A.data[i]);
        }
    }
}

