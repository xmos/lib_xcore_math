
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
#define MAX_LEN     256 


static unsigned seed = 666;





static void test_bfp_sum_s16()
{
    PRINTF("%s...\t(random vectors)\n", __func__);

    seed = 67765974;

    int16_t dataB[MAX_LEN];
    bfp_s16_t B;

    B.data = dataB;

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        test_random_bfp_s16(&B, MAX_LEN, &seed, NULL, 0);

        int32_t result = bfp_sum_s16(&B);

        int32_t exp = 0;
        for(int i = 0; i < B.length; i++)
            exp += B.data[i];

        TEST_ASSERT_EQUAL(exp, result);
    }
}

static void test_bfp_sum_s32()
{
    PRINTF("%s...\t(random vectors)\n", __func__);

    seed = 8789;

    int32_t dataB[MAX_LEN];
    bfp_s32_t B;

    B.data = dataB;

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        test_random_bfp_s32(&B, MAX_LEN, &seed, NULL, 0);

        int64_t result = bfp_sum_s32(&B);

        int64_t exp = 0;
        for(int i = 0; i < B.length; i++)
            exp += B.data[i];

        TEST_ASSERT_EQUAL(exp, result);
    }
}




int test_bfp_sum()
{
    int retval = 0;
    UnityBegin(__FILE__);

    RUN_TEST(test_bfp_sum_s16);
    RUN_TEST(test_bfp_sum_s32);

    retval = UnityEnd();
    return retval;
}