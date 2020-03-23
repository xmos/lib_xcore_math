
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


#define REPS        IF_QUICK_TEST(10, 100)
#define MAX_LEN     18  //Smaller lengths mean larger variance w.r.t. individual element headroom


static unsigned seed = 666;


static void test_bfp_rect_vect_s16()
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
        bfp_rect_vect_s16(&A, &B);

        TEST_ASSERT_EQUAL(B.length, B.length);
        TEST_ASSERT_EQUAL(B.exp, A.exp);
        TEST_ASSERT_EQUAL(xs3_headroom_vect_s16(A.data, A.length), A.hr);

        for(int i = 0; i < A.length; i++){
            int16_t expected = (B.data[i] < 0)? 0 : B.data[i];
            TEST_ASSERT_EQUAL(expected, A.data[i]);
        }
    }
}




static void test_bfp_rect_vect_s32()
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
        bfp_rect_vect_s32(&A, &B);

        TEST_ASSERT_EQUAL(B.length, B.length);
        TEST_ASSERT_EQUAL(B.exp, A.exp);
        TEST_ASSERT_EQUAL(xs3_headroom_vect_s32(A.data, A.length), A.hr);

        for(int i = 0; i < A.length; i++){
            int32_t expected = (B.data[i] < 0)? 0 : B.data[i];
            TEST_ASSERT_EQUAL(expected, A.data[i]);
        }
    }
}





void test_bfp_rect_vect()
{
    SET_TEST_FILE();

    RUN_TEST(test_bfp_rect_vect_s16);
    RUN_TEST(test_bfp_rect_vect_s32);
}