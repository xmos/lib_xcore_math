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

TEST_GROUP_RUNNER(CLS_macro) {
  RUN_TEST_CASE(CLS_macro, cls);
  RUN_TEST_CASE(CLS_macro, CLS_S16);
  RUN_TEST_CASE(CLS_macro, CLS_S32);
  RUN_TEST_CASE(CLS_macro, CLS_S64);
  RUN_TEST_CASE(CLS_macro, CLS_C16);
  RUN_TEST_CASE(CLS_macro, CLS_C32);
}

TEST_GROUP(CLS_macro);
TEST_SETUP(CLS_macro) { fflush(stdout); }
TEST_TEAR_DOWN(CLS_macro) {}


static char msg_buff[200];

#if SMOKE_TEST
#  define N       (100)
#else
#  define N       (1000)
#endif

TEST(CLS_macro, cls)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    cls(-89319282);

    int32_t numbers[N];
    unsigned sign_bits[N];

    // Generate random values
    for(unsigned int i = 0; i < N; i++)
        numbers[i] = pseudo_rand_int32(&seed) >> (pseudo_rand_int32(&seed) % 32);

    // Count sign bits
    for(unsigned int i = 0; i < N; i++)
        sign_bits[i] = cls(numbers[i]);


    // Check each
    for(unsigned int i = 0; i < N; i++){

        if(sign_bits[i] == 32){
            TEST_ASSERT_TRUE((numbers[i] == 0 || numbers[i] == -1));
            continue;
        }

        unsigned check = 1;
        check = check && (numbers[i] << (sign_bits[i]-1) >> (sign_bits[i]-1)) == numbers[i];
        check = check && (numbers[i] << (sign_bits[i]  ) >> (sign_bits[i]  )) != numbers[i];

        if(!check){
            sprintf(msg_buff, "(index: %d; number: %ld (0x%08X); result: %u)", i, (long int) numbers[i],
                       (unsigned int) numbers[i], sign_bits[i]);
        }

        TEST_ASSERT_EQUAL_INT32_MESSAGE(numbers[i], (numbers[i] << (sign_bits[i]-1) >> (sign_bits[i]-1)), msg_buff);
        TEST_ASSERT_NOT_EQUAL_MESSAGE(  numbers[i], (numbers[i] << (sign_bits[i]  ) >> (sign_bits[i]  )), msg_buff);
    }
}

TEST(CLS_macro, CLS_S32)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    int32_t numbers[N];
    unsigned actual[N];
    unsigned expected[N];

    for(unsigned int i = 0; i < N; i++){
        numbers[i] = pseudo_rand_int32(&seed) >> (pseudo_rand_int32(&seed) % 32);
        expected[i] = cls(numbers[i]);
        actual[i] = CLS_S32(numbers[i]);
    }

    TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, actual, N);
}

TEST(CLS_macro, CLS_S16)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    int16_t numbers[N];
    unsigned actual[N];
    unsigned expected[N];

    for(unsigned int i = 0; i < N; i++){
        numbers[i] = pseudo_rand_int16(&seed) >> (pseudo_rand_int32(&seed) % 16);
        expected[i] = cls(numbers[i]) - 16;
        actual[i] = CLS_S16(numbers[i]);
    }

    TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, actual, N);
}


TEST(CLS_macro, CLS_C32)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    complex_s32_t numbers[N];
    unsigned actual[N];
    unsigned expected[N];

    for(unsigned int i = 0; i < N; i++){
        numbers[i].re = pseudo_rand_int32(&seed) >> (pseudo_rand_int32(&seed) % 32);
        numbers[i].im = pseudo_rand_int32(&seed) >> (pseudo_rand_int32(&seed) % 32);
        expected[i] = MIN(cls(numbers[i].re), cls(numbers[i].im));
        actual[i] = CLS_C32(numbers[i]);
    }

    TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, actual, N);
}


TEST(CLS_macro, CLS_C16)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    complex_s16_t numbers[N];
    unsigned actual[N];
    unsigned expected[N];

    for(unsigned int i = 0; i < N; i++){
        numbers[i].re = pseudo_rand_int16(&seed) >> (pseudo_rand_int32(&seed) % 16);
        numbers[i].im = pseudo_rand_int16(&seed) >> (pseudo_rand_int32(&seed) % 16);
        expected[i] = MIN(CLS_S16(numbers[i].re), CLS_S16(numbers[i].im));
        actual[i] = CLS_C16(numbers[i]);
    }

    TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, actual, N);
}


TEST(CLS_macro, CLS_S64)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    TEST_ASSERT_EQUAL_UINT32(64, CLS_S64( 0x0000000000000000LL));
    TEST_ASSERT_EQUAL_UINT32(64, CLS_S64(-0x0000000000000001LL));
    TEST_ASSERT_EQUAL_UINT32( 1, CLS_S64( 0x7FFFFFFFFFFFFFFFLL));
    TEST_ASSERT_EQUAL_UINT32( 1, CLS_S64(-0x8000000000000000LL));
    TEST_ASSERT_EQUAL_UINT32(32, CLS_S64( 0x00000000FFFFFFFFLL));
    TEST_ASSERT_EQUAL_UINT32(32, CLS_S64( 0xFFFFFFFF00000000LL));
    TEST_ASSERT_EQUAL_UINT32(31, CLS_S64( 0x0000000100000000LL));


    int64_t numbers[N];
    unsigned actual[N];
    unsigned expected[N];

    for(unsigned int i = 0; i < N; i++){

        numbers[i] = pseudo_rand_int64(&seed) >> pseudo_rand_uint(&seed, 0, 56);

        int32_t high = numbers[i] >> 32;
        int32_t mid = (int32_t) (numbers[i] >> 16);
        int32_t low = (int32_t) numbers[i];

        expected[i] = (cls(high) == 32)? (cls(mid) == 32)? 32+cls(low) : 16+cls(mid) : cls(high);

        actual[i] = CLS_S64(numbers[i]);

    }

    TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, actual, N);
}

