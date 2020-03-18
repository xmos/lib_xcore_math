
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "xs3_math.h"

#include "../tst_common.h"

#include "unity.h"

static unsigned seed = 2314567;

static char msg_buff[200];


#if DEBUG_ON || 0
#undef DEBUG_ON
#define DEBUG_ON    (1)
#endif


#define N        IF_QUICK_TEST(100, 1000)


static void test_HR_S32()
{
    PRINTF("%s...\n", __func__);
    seed = 343446;

    int32_t numbers[N];
    unsigned actual[N];
    unsigned expected[N];

    for(int i = 0; i < N; i++){
        numbers[i] = pseudo_rand_int32(&seed);
        expected[i] = cls(numbers[i]) - 1;
        actual[i] = HR_S32(numbers[i]);
    }

    TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, actual, N);
}


static void test_HR_S16()
{
    PRINTF("%s...\n", __func__);
    seed = 343446;

    int16_t numbers[N];
    unsigned actual[N];
    unsigned expected[N];

    for(int i = 0; i < N; i++){
        numbers[i] = pseudo_rand_int16(&seed);
        expected[i] = HR_S32(numbers[i]) - 16;
        actual[i] = HR_S16(numbers[i]);
    }

    TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, actual, N);
}




static void test_HR_C32()
{
    PRINTF("%s...\n", __func__);
    seed = 343446;

    complex_s32_t numbers[N];
    unsigned actual[N];
    unsigned expected[N];

    for(int i = 0; i < N; i++){
        numbers[i].re = pseudo_rand_int32(&seed);
        numbers[i].im = pseudo_rand_int32(&seed);
        expected[i] = XS3_MIN(cls(numbers[i].re)-1, cls(numbers[i].im)-1);
        actual[i] = HR_C32(numbers[i]);
    }

    TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, actual, N);
}




static void test_HR_C16()
{
    PRINTF("%s...\n", __func__);
    seed = 343446;

    complex_s16_t numbers[N];
    unsigned actual[N];
    unsigned expected[N];

    for(int i = 0; i < N; i++){
        numbers[i].re = pseudo_rand_int16(&seed);
        numbers[i].im = pseudo_rand_int16(&seed);
        expected[i] = XS3_MIN(HR_S16(numbers[i].re), HR_S16(numbers[i].im));
        actual[i] = HR_C16(numbers[i]);
    }

    TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, actual, N);
}




int test_HR_funcs()
{
    int retval = 0;
    
    UnityBegin(__FILE__);

    RUN_TEST(test_HR_S32);
    RUN_TEST(test_HR_S16);
    RUN_TEST(test_HR_C32);
    RUN_TEST(test_HR_C16);

    retval = UnityEnd();
    

    return retval;
}