// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "xmath/xmath.h"

#include "../tst_common.h"

#include "unity_fixture.h"

TEST_GROUP_RUNNER(HR_macro) {
  RUN_TEST_CASE(HR_macro, HR_S16);
  RUN_TEST_CASE(HR_macro, HR_S32);
  RUN_TEST_CASE(HR_macro, HR_S64);
  RUN_TEST_CASE(HR_macro, HR_C16);
  RUN_TEST_CASE(HR_macro, HR_C32);
}

TEST_GROUP(HR_macro);
TEST_SETUP(HR_macro) { fflush(stdout); }
TEST_TEAR_DOWN(HR_macro) {}


#if SMOKE_TEST
#  define N       (128)
#else
#  define N       (1024)
#endif

TEST(HR_macro, HR_S32)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    int32_t numbers[N];
    unsigned actual[N];
    unsigned expected[N];

    for(unsigned int i = 0; i < N; i++){
        numbers[i] = pseudo_rand_int32(&seed) >> pseudo_rand_uint(&seed, 0, 28);
        expected[i] = cls(numbers[i]) - 1;
        actual[i] = HR_S32(numbers[i]);
    }

    TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, actual, N);
}

TEST(HR_macro, HR_S16)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    int16_t numbers[N];
    unsigned actual[N];
    unsigned expected[N];

    for(unsigned int i = 0; i < N; i++){
        numbers[i] = pseudo_rand_int16(&seed) >> pseudo_rand_uint(&seed, 0, 12);
        expected[i] = HR_S32(numbers[i]) - 16;
        actual[i] = HR_S16(numbers[i]);
    }

    TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, actual, N);
}


TEST(HR_macro, HR_C32)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    complex_s32_t numbers[N];
    unsigned actual[N];
    unsigned expected[N];

    for(unsigned int i = 0; i < N; i++){
        numbers[i].re = pseudo_rand_int32(&seed) >> pseudo_rand_uint(&seed, 0, 28);
        numbers[i].im = pseudo_rand_int32(&seed) >> pseudo_rand_uint(&seed, 0, 28);
        expected[i] = MIN(cls(numbers[i].re)-1, cls(numbers[i].im)-1);
        actual[i] = HR_C32(numbers[i]);
    }

    TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, actual, N);
}


TEST(HR_macro, HR_C16)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    complex_s16_t numbers[N];
    unsigned actual[N];
    unsigned expected[N];

    for(unsigned int i = 0; i < N; i++){
        numbers[i].re = pseudo_rand_int16(&seed) >> pseudo_rand_uint(&seed, 0, 12);
        numbers[i].im = pseudo_rand_int16(&seed) >> pseudo_rand_uint(&seed, 0, 12);
        expected[i] = MIN(HR_S16(numbers[i].re), HR_S16(numbers[i].im));
        actual[i] = HR_C16(numbers[i]);
    }

    TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, actual, N);
}


TEST(HR_macro, HR_S64)
{
    unsigned seed = SEED_FROM_FUNC_NAME();


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

    for(unsigned int i = 0; i < N; i++){
        numbers[i] = pseudo_rand_int64(&seed) >> pseudo_rand_uint(&seed, 0, 56);
        expected[i] = CLS_S64(numbers[i]) - 1;
        actual[i] = HR_S64(numbers[i]);

        numbers2[i] = numbers[i] << actual[i];
        numbers2[i] = numbers2[i] >> actual[i];

        numbers3[i] = numbers[i] << (actual[i] + 1);
        numbers3[i] = numbers3[i] >> (actual[i] + 1);
    }

    TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, actual, N);

    for(unsigned int i = 0; i < N; i++){
        TEST_ASSERT_EQUAL(numbers[i], numbers2[i]);
        TEST_ASSERT_NOT_EQUAL(numbers[i], numbers3[i]);
    }
}
