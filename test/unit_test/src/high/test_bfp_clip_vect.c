
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
#define MAX_LEN     256 


static unsigned seed = 666;





static void test_bfp_clip_vect_s16()
{
    PRINTF("%s...\n", __func__);
    TEST_IGNORE_MESSAGE("TEST NOT IMPLEMENTED");
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