// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "xmath/xmath.h"

#include "../tst_common.h"

#include "unity_fixture.h"

TEST_GROUP_RUNNER(vect_rect) {
  RUN_TEST_CASE(vect_rect, vect_s16_rect);
  RUN_TEST_CASE(vect_rect, vect_s32_rect);
}

TEST_GROUP(vect_rect);
TEST_SETUP(vect_rect) { fflush(stdout); }
TEST_TEAR_DOWN(vect_rect) {}

// static char msg_buff[200];

#define TEST_ASSERT_EQUAL_MSG(EXPECTED, ACTUAL, LINE_NUM)   do{       \
    if((EXPECTED)!=(ACTUAL)) {                                        \
      sprintf(msg_buff, "(test vector @ line %u)", (LINE_NUM));       \
      TEST_ASSERT_EQUAL_MESSAGE((EXPECTED), (ACTUAL), msg_buff);      \
    }} while(0)



#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (64)
#else
#  define REPS       (1000)
#  define MAX_LEN    (256)
#endif


TEST(vect_rect, vect_s16_rect) //
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();

    
    headroom_t hr;
    int16_t A[MAX_LEN];
    int16_t B[MAX_LEN];

    //If something's really wrong, debugging this first will be easiest.
    if(1){
        A[0] =  0xCCCC;
        A[1] =  0xCCCC;
        B[0] =  0x1234;
        B[1] = -0x1234;

        hr = vect_s16_rect(A, B, 2);
        TEST_ASSERT_EQUAL(0x1234, A[0]);
        TEST_ASSERT_EQUAL(0x0000, A[1]);
        TEST_ASSERT_EQUAL(2, hr);
    }

    for(unsigned int v = 0; v < REPS; v++){

        setExtraInfo_R(v);

        const unsigned len = pseudo_rand_uint32(&seed) % MAX_LEN;

        for(unsigned int i = 0; i < len; i++){
            unsigned shr = pseudo_rand_uint32(&seed) % 10;
            B[i] = pseudo_rand_int16(&seed) >> shr;
        }

        memset(A, 0xCC, sizeof(A));
        hr = vect_s16_rect(A, B, len);

        for(unsigned int i = 0; i < len; i++)
            TEST_ASSERT_EQUAL(B[i] >= 0? B[i] : 0, A[i]);
        for(int i = len; i < MAX_LEN; i++)
            TEST_ASSERT_EQUAL((int16_t)0xCCCC, A[i]);

        memcpy(A, B, sizeof(A));
        hr = vect_s16_rect(A, A, len);

        for(unsigned int i = 0; i < len; i++)
            TEST_ASSERT_EQUAL(B[i] >= 0? B[i] : 0, A[i]);
    }
}


TEST(vect_rect, vect_s32_rect)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();

    
    headroom_t hr;
    int32_t A[MAX_LEN];
    int32_t B[MAX_LEN];

    //If something's really wrong, debugging this first will be easiest.
    if(1){
        A[0] =  0xCCCCCCCC;
        A[1] =  0xCCCCCCCC;
        B[0] =  0x12345678;
        B[1] = -0x12345678;

        hr = vect_s32_rect(A, B, 2);
        TEST_ASSERT_EQUAL(0x12345678, A[0]);
        TEST_ASSERT_EQUAL(0x00000000, A[1]);
        TEST_ASSERT_EQUAL(2, hr);
    }

    for(unsigned int v = 0; v < REPS; v++){

        setExtraInfo_R(v);

        const unsigned len = pseudo_rand_uint32(&seed) % MAX_LEN;

        for(unsigned int i = 0; i < len; i++){
            unsigned shr = pseudo_rand_uint32(&seed) % 10;
            B[i] = pseudo_rand_int32(&seed) >> shr;
        }

        memset(A, 0xCC, sizeof(A));
        hr = vect_s32_rect(A, B, len);

        for(unsigned int i = 0; i < len; i++)
            TEST_ASSERT_EQUAL(B[i] >= 0? B[i] : 0, A[i]);
        for(int i = len; i < MAX_LEN; i++)
            TEST_ASSERT_EQUAL((int32_t)0xCCCCCCCC, A[i]);

        memcpy(A, B, sizeof(A));
        hr = vect_s32_rect(A, A, len);

        for(unsigned int i = 0; i < len; i++)
            TEST_ASSERT_EQUAL(B[i] >= 0? B[i] : 0, A[i]);
    }
}

