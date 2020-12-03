
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





static void test_bfp_s32_to_s16_()
{
    PRINTF("%s...\t(random vectors)\n", __func__);

    seed = 54516;

    int16_t dataA[MAX_LEN];
    int32_t dataB[MAX_LEN];
    bfp_s16_t A;
    bfp_s32_t B;

    A.data = dataA;
    B.data = dataB;

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        test_random_bfp_s32(&B, MAX_LEN, &seed, NULL, 0);
        A.length = B.length;

        if(r == 0){
            B.data[0] = 0x80000000;
            B.hr = HR_S32(B.data[0]);
        } else if(r == 1){
            B.data[0] = 0xC0000000;
            B.hr = HR_S32(B.data[0]);
        }

        bfp_s32_to_s16(&A, &B);

        TEST_ASSERT_EQUAL(B.length, A.length);
        TEST_ASSERT_EQUAL(xs3_vect_s16_headroom(A.data, A.length), A.hr);

        unsigned cc = !XS3_BFP_ALLOW_SATURATION;
        
        TEST_ASSERT_EQUAL(B.exp+cc,  A.exp );

        int tmp = (16 - B.hr) + cc;

        for(int i = 0; i < A.length; i++){
            int16_t expected = (B.data[i] + (1 << (tmp-1))) >> tmp;
            TEST_ASSERT_EQUAL(expected, A.data[i]);
        }
    }
}




void test_bfp_s32_to_s16()
{
    SET_TEST_FILE();

    RUN_TEST(test_bfp_s32_to_s16_);
}