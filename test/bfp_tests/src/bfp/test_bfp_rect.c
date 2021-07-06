// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "bfp_math.h"

#include "../tst_common.h"

#include "unity_fixture.h"


TEST_GROUP_RUNNER(bfp_rect) {
  RUN_TEST_CASE(bfp_rect, bfp_s16_rect);
  RUN_TEST_CASE(bfp_rect, bfp_s32_rect);
}
TEST_GROUP(bfp_rect);
TEST_SETUP(bfp_rect) {}
TEST_TEAR_DOWN(bfp_rect) {}


#if DEBUG_ON || 0
#undef DEBUG_ON
#define DEBUG_ON    (1)
#endif


#define REPS        1000
#define MAX_LEN     256


static unsigned seed = 666;
static char msg_buff[200];

#define TEST_ASSERT_EQUAL_MSG(EXPECTED, ACTUAL, EXTRA, LINE_NUM)   do{          \
    if((EXPECTED)!=(ACTUAL)) {                                                  \
      sprintf(msg_buff, "%s (test vector @ line %u)", (EXTRA), (LINE_NUM));     \
      TEST_ASSERT_EQUAL_MESSAGE((EXPECTED), (ACTUAL), msg_buff);                \
    }} while(0)



TEST(bfp_rect, bfp_s16_rect)
// static void test_bfp_s16_rect()
{
    PRINTF("%s...\n", __func__);

    seed = 9676;

    int16_t dataA[MAX_LEN];
    int16_t dataB[MAX_LEN];
    bfp_s16_t A, B;

    A.data = dataA;
    B.data = dataB;

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        test_random_bfp_s16(&B, MAX_LEN, &seed, &A, 0);
        bfp_s16_rect(&A, &B);

        TEST_ASSERT_EQUAL(B.length, B.length);
        TEST_ASSERT_EQUAL(B.exp, A.exp);
        TEST_ASSERT_EQUAL(xs3_vect_s16_headroom(A.data, A.length), A.hr);

        for(int i = 0; i < A.length; i++){
            int16_t expected = (B.data[i] < 0)? 0 : B.data[i];
            TEST_ASSERT_EQUAL(expected, A.data[i]);
        }
    }
}




TEST(bfp_rect, bfp_s32_rect)
// static void test_bfp_s32_rect()
{
    PRINTF("%s...\n", __func__);

    seed = 34573;

    int32_t dataA[MAX_LEN];
    int32_t dataB[MAX_LEN];
    bfp_s32_t A, B;

    A.data = dataA;
    B.data = dataB;

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        test_random_bfp_s32(&B, MAX_LEN, &seed, &A, 0);
        bfp_s32_rect(&A, &B);

        TEST_ASSERT_EQUAL(B.length, B.length);
        TEST_ASSERT_EQUAL(B.exp, A.exp);
        TEST_ASSERT_EQUAL(xs3_vect_s32_headroom(A.data, A.length), A.hr);

        for(int i = 0; i < A.length; i++){
            int32_t expected = (B.data[i] < 0)? 0 : B.data[i];
            TEST_ASSERT_EQUAL(expected, A.data[i]);
        }
    }
}

