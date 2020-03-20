
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xs3_math.h"

#include "../tst_common.h"

#include "unity.h"

#if DEBUG_ON || 1
#undef DEBUG_ON
#define DEBUG_ON    (1)
#endif


#define REPS        IF_QUICK_TEST(10, 100)
#define MAX_LEN     18  //Smaller lengths mean larger variance w.r.t. individual element headroom


static unsigned seed = 666;





static void test_bfp_s16_to_s32_()
{
    PRINTF("%s...\t(random vectors)\n", __func__);

    seed = 645677;

    int32_t dataA[MAX_LEN];
    int16_t dataB[MAX_LEN];
    bfp_s32_t A;
    bfp_s16_t B;

    A.data = dataA;
    B.data = dataB;

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        test_random_bfp_s16(&B, MAX_LEN, &seed, NULL, 0);
        A.length = B.length;

        bfp_s16_to_s32(&A, &B);

        TEST_ASSERT_EQUAL(B.length, A.length);
        TEST_ASSERT_EQUAL(B.hr + 8, A.hr);
        TEST_ASSERT_EQUAL(xs3_headroom_vect_s32(A.data, A.length), A.hr);
        
        TEST_ASSERT_EQUAL(B.exp-8, A.exp);

        for(int i = 0; i < A.length; i++){
            TEST_ASSERT_EQUAL(((int32_t)B.data[i]) << 8, A.data[i]);
        }
    }
}




int test_bfp_s16_to_s32()
{
    int retval = 0;
    UnityBegin(__FILE__);

    RUN_TEST(test_bfp_s16_to_s32_);

    retval = UnityEnd();
    return retval;
}