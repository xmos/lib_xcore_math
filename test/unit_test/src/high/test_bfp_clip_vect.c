
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xs3_math.h"

#include "../tst_common.h"

#include "unity.h"

#if DEBUG_ON || 0
#undef DEBUG_ON
#define DEBUG_ON    (1)
#endif


#define REPS        IF_QUICK_TEST(100, 100)
#define MAX_LEN     34 


static unsigned seed = 666;





static void test_bfp_clip_vect_s16()
{
    PRINTF("%s...\n", __func__);

    seed = 0xA148D19C;

    int16_t dataA[MAX_LEN];
    int16_t dataB[MAX_LEN];
    bfp_s16_t A, B;

    A.data = dataA;
    B.data = dataB;

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        test_random_bfp_s16(&B, MAX_LEN, &seed, &A, 0);

        int16_t lower = pseudo_rand_int16(&seed);
        int16_t upper = pseudo_rand_int16(&seed);

        if(lower > upper){
            int16_t t = lower;
            lower = upper;
            upper = t;
        }

        exponent_t bound_exp = B.exp + 1;

        bfp_clip_vect_s16(&A, &B, lower, upper, bound_exp);

        TEST_ASSERT_EQUAL(xs3_headroom_vect_s16(A.data, A.length), A.hr);

        right_shift_t bound_shr = A.exp - bound_exp;
        assert(bound_shr > -16);

        int32_t upper32 = (bound_shr >= 0)? ((int32_t)upper) >> (bound_shr) : ((int32_t)upper) << -bound_shr; 
        int32_t lower32 = (bound_shr >= 0)? ((int32_t)lower) >> (bound_shr) : ((int32_t)lower) << -bound_shr;
        upper32 = (upper32 >= VPU_INT16_MAX)? VPU_INT16_MAX : (upper32 <= VPU_INT16_MIN)? VPU_INT16_MIN : upper32;
        lower32 = (lower32 >= VPU_INT16_MAX)? VPU_INT16_MAX : (lower32 <= VPU_INT16_MIN)? VPU_INT16_MIN : lower32;

        for(int i = 0; i < A.length; i++){
            TEST_ASSERT_GREATER_OR_EQUAL_INT32(lower32, A.data[i]);
            TEST_ASSERT_LESS_OR_EQUAL_INT32(upper32, A.data[i]);
        }
    }
}

static void test_bfp_clip_vect_s32()
{
    PRINTF("%s...\n", __func__);

    seed = 0x2D3E3215;

    int32_t dataA[MAX_LEN];
    int32_t dataB[MAX_LEN];
    bfp_s32_t A, B;

    A.data = dataA;
    B.data = dataB;

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        test_random_bfp_s32(&B, MAX_LEN, &seed, &A, 0);

        int32_t lower = pseudo_rand_int32(&seed);
        int32_t upper = pseudo_rand_int32(&seed);

        if(lower > upper){
            int32_t t = lower;
            lower = upper;
            upper = t;
        }

        exponent_t bound_exp = B.exp + 1;

        bfp_clip_vect_s32(&A, &B, lower, upper, bound_exp);

        TEST_ASSERT_EQUAL(xs3_headroom_vect_s32(A.data, A.length), A.hr);

        right_shift_t bound_shr = A.exp - bound_exp;
        assert(bound_shr > -32);

        int64_t upper64 = (bound_shr >= 0)? ((int64_t)upper) >> (bound_shr) : ((int64_t)upper) << -bound_shr; 
        int64_t lower64 = (bound_shr >= 0)? ((int64_t)lower) >> (bound_shr) : ((int64_t)lower) << -bound_shr;
        upper64 = (upper64 >= VPU_INT32_MAX)? VPU_INT32_MAX : (upper64 <= VPU_INT32_MIN)? VPU_INT32_MIN : upper64;
        lower64 = (lower64 >= VPU_INT32_MAX)? VPU_INT32_MAX : (lower64 <= VPU_INT32_MIN)? VPU_INT32_MIN : lower64;

        for(int i = 0; i < A.length; i++){
            TEST_ASSERT_GREATER_OR_EQUAL_INT32(lower64, A.data[i]);
            TEST_ASSERT_LESS_OR_EQUAL_INT32(upper64, A.data[i]);
        }
    }
}




void test_bfp_clip_vect()
{
    SET_TEST_FILE();

    RUN_TEST(test_bfp_clip_vect_s16);
    RUN_TEST(test_bfp_clip_vect_s32);
}