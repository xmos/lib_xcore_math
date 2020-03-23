
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

    seed = 0xCA780526;

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

        // printf("B.exp     = %d\tB.hr  = %u\tB[0]  = %d\n", B.exp, B.hr, B.data[0]);

        // printf("bound_exp = %d\tlower = %d\tupper = %d\n", bound_exp, lower, upper);

        bfp_clip_vect_s16(&A, &B, lower, upper, bound_exp);


        TEST_ASSERT_EQUAL(B.length, B.length);
        // TEST_ASSERT_EQUAL(B.exp - B.hr + XS3_BFP_ALLOW_SATURATION, A.exp);
        TEST_ASSERT_EQUAL(xs3_headroom_vect_s16(A.data, A.length), A.hr);

        right_shift_t bound_shr = A.exp - bound_exp;
        int32_t upper32 = (bound_shr >= 0)? ((int32_t)upper) << (-bound_shr) : ((int32_t)upper) >> bound_shr; 
        int32_t lower32 = (bound_shr >= 0)? ((int32_t)lower) << (-bound_shr) : ((int32_t)lower) >> bound_shr;
        upper32 = (upper32 >= VPU_INT16_MAX)? VPU_INT16_MAX : (upper32 <= VPU_INT16_MIN)? VPU_INT16_MIN : upper32;
        lower32 = (lower32 >= VPU_INT16_MAX)? VPU_INT16_MAX : (lower32 <= VPU_INT16_MIN)? VPU_INT16_MIN : lower32;

        // printf("A.exp   = %d\tA.hr = %u\tA[0] = %d\n", A.exp, A.hr, A.data[0]);
        // printf("\t\tlower32 = %ld\t\tupper32 = %ld\n", lower32, upper32);

        for(int i = 0; i < A.length; i++){
            // if( A.data[i] > upper32 || A.data[i] < lower32){
            //     printf("!!\t%d\t%d\n", B.data[i], A.data[i]);
            // }


            TEST_ASSERT_GREATER_OR_EQUAL_INT32(lower32, A.data[i]);
            TEST_ASSERT_LESS_OR_EQUAL_INT32(upper32, A.data[i]);
        }

        // printf("\n\n\n");
    }
}

static void test_bfp_clip_vect_s32()
{
    PRINTF("%s...\t(random vectors)\n", __func__);
    TEST_IGNORE_MESSAGE("TEST NOT IMPLEMENTED");
}




void test_bfp_clip_vect()
{
    SET_TEST_FILE();

    RUN_TEST(test_bfp_clip_vect_s16);
    RUN_TEST(test_bfp_clip_vect_s32);
}