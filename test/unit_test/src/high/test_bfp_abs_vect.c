
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
static char msg_buff[200];

#define TEST_ASSERT_EQUAL_MSG(EXPECTED, ACTUAL, EXTRA, LINE_NUM)   do{          \
    if((EXPECTED)!=(ACTUAL)) {                                                  \
      sprintf(msg_buff, "%s (test vector @ line %u)", (EXTRA), (LINE_NUM));     \
      TEST_ASSERT_EQUAL_MESSAGE((EXPECTED), (ACTUAL), msg_buff);                \
    }} while(0)







static void test_bfp_abs_vect_s16()
{
    PRINTF("%s...\t(random vectors)\n", __func__);

    seed = 0x436EBC94;

    int16_t dataA[MAX_LEN];
    int16_t dataB[MAX_LEN];
    bfp_s16_t A, B;

    A.data = dataA;
    B.data = dataB;

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        test_random_bfp_s16(&B, MAX_LEN, &seed, &A, 0);
        bfp_abs_vect_s16(&A, &B);

        TEST_ASSERT_EQUAL(B.length, B.length);
        TEST_ASSERT_EQUAL(xs3_headroom_vect_s16(A.data, A.length), A.hr);

        unsigned cc = (B.hr == 0)  && (!XS3_BFP_ALLOW_SATURATION);
        
        TEST_ASSERT_EQUAL(B.exp+cc,  A.exp );

        for(int i = 0; i < A.length; i++){
            int16_t expected = abs(B.data[i] >> cc);
            TEST_ASSERT_EQUAL(expected, A.data[i]);
        }
    }
}




static void test_bfp_abs_vect_s32()
{
    PRINTF("%s...\t(random vectors)\n", __func__);

    seed = 453856;

    int32_t dataA[MAX_LEN];
    int32_t dataB[MAX_LEN];
    bfp_s32_t A, B;

    A.data = dataA;
    B.data = dataB;

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        test_random_bfp_s32(&B, MAX_LEN, &seed, &A, 0);
        bfp_abs_vect_s32(&A, &B);

        TEST_ASSERT_EQUAL(B.length, B.length);
        TEST_ASSERT_EQUAL(xs3_headroom_vect_s32(A.data, A.length), A.hr);

        unsigned cc = (B.hr == 0)  && (!XS3_BFP_ALLOW_SATURATION);
        
        TEST_ASSERT_EQUAL(B.exp+cc,  A.exp );

        for(int i = 0; i < A.length; i++){
            int32_t expected = abs(B.data[i] >> cc);
            TEST_ASSERT_EQUAL(expected, A.data[i]);
        }
    }
}





int test_bfp_abs_vect()
{
    int retval = 0;
    UnityBegin(__FILE__);

    RUN_TEST(test_bfp_abs_vect_s16);
    RUN_TEST(test_bfp_abs_vect_s32);

    retval = UnityEnd();
    return retval;
}