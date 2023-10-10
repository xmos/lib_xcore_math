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


TEST_GROUP_RUNNER(bfp_clip) {
  RUN_TEST_CASE(bfp_clip, bfp_s16_clip);
  RUN_TEST_CASE(bfp_clip, bfp_s32_clip);
}
TEST_GROUP(bfp_clip);
TEST_SETUP(bfp_clip) { fflush(stdout); }
TEST_TEAR_DOWN(bfp_clip) {}

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


TEST(bfp_clip, bfp_s16_clip)
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

        int16_t lower = pseudo_rand_int16(&seed);
        int16_t upper = pseudo_rand_int16(&seed);

        if(lower > upper){
            int16_t t = lower;
            lower = upper;
            upper = t;
        }

        exponent_t bound_exp = B.exp + 1;

        bfp_s16_clip(&A, &B, lower, upper, bound_exp);

        TEST_ASSERT_EQUAL(vect_s16_headroom(A.data, A.length), A.hr);

        right_shift_t bound_shr = A.exp - bound_exp;
        assert(bound_shr > -16);

        int32_t upper32 = (bound_shr >= 0)? ((int32_t)upper) >> (bound_shr) : ((int32_t)upper) << -bound_shr;
        int32_t lower32 = (bound_shr >= 0)? ((int32_t)lower) >> (bound_shr) : ((int32_t)lower) << -bound_shr;
        upper32 = (upper32 >= VPU_INT16_MAX)? VPU_INT16_MAX : (upper32 <= VPU_INT16_MIN)? VPU_INT16_MIN : upper32;
        lower32 = (lower32 >= VPU_INT16_MAX)? VPU_INT16_MAX : (lower32 <= VPU_INT16_MIN)? VPU_INT16_MIN : lower32;

        for(unsigned int i = 0; i < A.length; i++){
            TEST_ASSERT_GREATER_OR_EQUAL_INT32(lower32, A.data[i]);
            TEST_ASSERT_LESS_OR_EQUAL_INT32(upper32, A.data[i]);
        }
    }
}

TEST(bfp_clip, bfp_s32_clip)
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

        int32_t lower = pseudo_rand_int32(&seed);
        int32_t upper = pseudo_rand_int32(&seed);

        if(lower > upper){
            int32_t t = lower;
            lower = upper;
            upper = t;
        }

        exponent_t bound_exp = B.exp + 1;

        bfp_s32_clip(&A, &B, lower, upper, bound_exp);

        TEST_ASSERT_EQUAL(vect_s32_headroom(A.data, A.length), A.hr);

        right_shift_t bound_shr = A.exp - bound_exp;
        assert(bound_shr > -32);

        int64_t upper64 = (bound_shr >= 0)? ((int64_t)upper) >> (bound_shr) : ((int64_t)upper) << -bound_shr;
        int64_t lower64 = (bound_shr >= 0)? ((int64_t)lower) >> (bound_shr) : ((int64_t)lower) << -bound_shr;
        upper64 = (upper64 >= VPU_INT32_MAX)? VPU_INT32_MAX : (upper64 <= VPU_INT32_MIN)? VPU_INT32_MIN : upper64;
        lower64 = (lower64 >= VPU_INT32_MAX)? VPU_INT32_MAX : (lower64 <= VPU_INT32_MIN)? VPU_INT32_MIN : lower64;

        for(unsigned int i = 0; i < A.length; i++){
            TEST_ASSERT_GREATER_OR_EQUAL_INT32(lower64, A.data[i]);
            TEST_ASSERT_LESS_OR_EQUAL_INT32(upper64, A.data[i]);
        }
    }
}
