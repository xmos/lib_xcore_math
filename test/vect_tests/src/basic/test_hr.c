
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


#define N        1024


static void test_HR_S32()
{
    PRINTF("%s...\n", __func__);
    seed = 999456;

    int32_t numbers[N];
    unsigned actual[N];
    unsigned expected[N];

    for(int i = 0; i < N; i++){
        numbers[i] = pseudo_rand_int32(&seed) >> pseudo_rand_uint(&seed, 0, 28);
        expected[i] = cls(numbers[i]) - 1;
        actual[i] = HR_S32(numbers[i]);
    }

    TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, actual, N);
}


static void test_HR_S16()
{
    PRINTF("%s...\n", __func__);
    seed = 2311;

    int16_t numbers[N];
    unsigned actual[N];
    unsigned expected[N];

    for(int i = 0; i < N; i++){
        numbers[i] = pseudo_rand_int16(&seed) >> pseudo_rand_uint(&seed, 0, 12);
        expected[i] = HR_S32(numbers[i]) - 16;
        actual[i] = HR_S16(numbers[i]);
    }

    TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, actual, N);
}




static void test_HR_C32()
{
    PRINTF("%s...\n", __func__);
    seed = 7889;

    complex_s32_t numbers[N];
    unsigned actual[N];
    unsigned expected[N];

    for(int i = 0; i < N; i++){
        numbers[i].re = pseudo_rand_int32(&seed) >> pseudo_rand_uint(&seed, 0, 28);
        numbers[i].im = pseudo_rand_int32(&seed) >> pseudo_rand_uint(&seed, 0, 28);
        expected[i] = MIN(cls(numbers[i].re)-1, cls(numbers[i].im)-1);
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
        numbers[i].re = pseudo_rand_int16(&seed) >> pseudo_rand_uint(&seed, 0, 12);
        numbers[i].im = pseudo_rand_int16(&seed) >> pseudo_rand_uint(&seed, 0, 12);
        expected[i] = MIN(HR_S16(numbers[i].re), HR_S16(numbers[i].im));
        actual[i] = HR_C16(numbers[i]);
    }

    TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, actual, N);
}





static void test_HR_S64()
{
    PRINTF("%s...\n", __func__);
    seed = 456456;


    TEST_ASSERT_EQUAL_UINT32(63, HR_S64( 0x0000000000000000LL));
    TEST_ASSERT_EQUAL_UINT32(63, HR_S64(-0x0000000000000001LL));
    TEST_ASSERT_EQUAL_UINT32( 0, HR_S64( 0x7FFFFFFFFFFFFFFFLL));
    TEST_ASSERT_EQUAL_UINT32( 0, HR_S64(-0x8000000000000000LL));
    TEST_ASSERT_EQUAL_UINT32(31, HR_S64( 0x00000000FFFFFFFFLL));
    TEST_ASSERT_EQUAL_UINT32(31, HR_S64( 0xFFFFFFFF00000000LL));
    TEST_ASSERT_EQUAL_UINT32(30, HR_S64( 0x0000000100000000LL));

    int64_t numbers[N], numbers2[N], numbers3[N];
    unsigned actual[N];
    unsigned expected[N];

    for(int i = 0; i < N; i++){
        numbers[i] = pseudo_rand_int64(&seed) >> pseudo_rand_uint(&seed, 0, 56);
        expected[i] = CLS_S64(numbers[i]) - 1;
        actual[i] = HR_S64(numbers[i]);

        numbers2[i] = numbers[i] << actual[i];
        numbers2[i] = numbers2[i] >> actual[i];

        numbers3[i] = numbers[i] << (actual[i] + 1);
        numbers3[i] = numbers3[i] >> (actual[i] + 1);
    }

    TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, actual, N);

    for(int i = 0; i < N; i++){
        TEST_ASSERT_EQUAL(numbers[i], numbers2[i]);
        TEST_ASSERT_NOT_EQUAL(numbers[i], numbers3[i]);
    }
}



void test_HR_funcs()
{
    SET_TEST_FILE();

    RUN_TEST(test_HR_S32);
    RUN_TEST(test_HR_S16);
    RUN_TEST(test_HR_C32);
    RUN_TEST(test_HR_C16);
    RUN_TEST(test_HR_S64);
}