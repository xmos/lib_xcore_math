// Copyright 2020 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public License: Version 1.

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


#define N        (1000)

static void test_cls()
{
    PRINTF("%s...\n", __func__);
    seed = 3465;

    cls(-89319282);

    int32_t numbers[N];
    unsigned sign_bits[N];

    // Generate random values
    for(int i = 0; i < N; i++)
        numbers[i] = pseudo_rand_int32(&seed) >> (pseudo_rand_int32(&seed) % 32);

    // Count sign bits
    for(int i = 0; i < N; i++)
        sign_bits[i] = cls(numbers[i]);


    // Check each
    for(int i = 0; i < N; i++){ 

        if(sign_bits[i] == 32){
            TEST_ASSERT_TRUE((numbers[i] == 0 || numbers[i] == -1));
            continue;
        }

        unsigned check = 1;
        check = check && (numbers[i] << (sign_bits[i]-1) >> (sign_bits[i]-1)) == numbers[i];
        check = check && (numbers[i] << (sign_bits[i]  ) >> (sign_bits[i]  )) != numbers[i];

        if(!check){
            sprintf(msg_buff, "(index: %d; number: %ld (0x%08X); result: %u)", i, numbers[i], 
                        (unsigned) numbers[i], sign_bits[i]);
        }

        TEST_ASSERT_EQUAL_INT32_MESSAGE(numbers[i], (numbers[i] << (sign_bits[i]-1) >> (sign_bits[i]-1)), msg_buff);
        TEST_ASSERT_NOT_EQUAL_MESSAGE(  numbers[i], (numbers[i] << (sign_bits[i]  ) >> (sign_bits[i]  )), msg_buff);
    }
}


static void test_CLS_S32()
{
    PRINTF("%s...\n", __func__);
    seed = 764564;

    int32_t numbers[N];
    unsigned actual[N];
    unsigned expected[N];

    for(int i = 0; i < N; i++){
        numbers[i] = pseudo_rand_int32(&seed) >> (pseudo_rand_int32(&seed) % 32);
        expected[i] = cls(numbers[i]);
        actual[i] = CLS_S32(numbers[i]);
    }

    TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, actual, N);
}


static void test_CLS_S16()
{
    PRINTF("%s...\n", __func__);
    seed = 23234476;

    int16_t numbers[N];
    unsigned actual[N];
    unsigned expected[N];

    for(int i = 0; i < N; i++){
        numbers[i] = pseudo_rand_int16(&seed) >> (pseudo_rand_int32(&seed) % 16);
        expected[i] = cls(numbers[i]) - 16;
        actual[i] = CLS_S16(numbers[i]);
    }

    TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, actual, N);
}




static void test_CLS_C32()
{
    PRINTF("%s...\n", __func__);
    seed = 1256;

    complex_s32_t numbers[N];
    unsigned actual[N];
    unsigned expected[N];

    for(int i = 0; i < N; i++){
        numbers[i].re = pseudo_rand_int32(&seed) >> (pseudo_rand_int32(&seed) % 32);
        numbers[i].im = pseudo_rand_int32(&seed) >> (pseudo_rand_int32(&seed) % 32);
        expected[i] = MIN(cls(numbers[i].re), cls(numbers[i].im));
        actual[i] = CLS_C32(numbers[i]);
    }

    TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, actual, N);
}




static void test_CLS_C16()
{
    PRINTF("%s...\n", __func__);
    seed = 890786556;

    complex_s16_t numbers[N];
    unsigned actual[N];
    unsigned expected[N];

    for(int i = 0; i < N; i++){
        numbers[i].re = pseudo_rand_int16(&seed) >> (pseudo_rand_int32(&seed) % 16);
        numbers[i].im = pseudo_rand_int16(&seed) >> (pseudo_rand_int32(&seed) % 16);
        expected[i] = MIN(CLS_S16(numbers[i].re), CLS_S16(numbers[i].im));
        actual[i] = CLS_C16(numbers[i]);
    }

    TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, actual, N);
}




static void test_CLS_S64()
{
    PRINTF("%s...\n", __func__);
    seed = 4334;

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

    for(int i = 0; i < N; i++){

        numbers[i] = pseudo_rand_int64(&seed) >> pseudo_rand_uint(&seed, 0, 56);

        int32_t high = numbers[i] >> 32;
        int32_t mid = numbers[i] >> 16;
        int32_t low = numbers[i];

        expected[i] = (cls(high) == 32)? (cls(mid) == 32)? 32+cls(low) : 16+cls(mid) : cls(high);

        actual[i] = CLS_S64(numbers[i]);

    }

    TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, actual, N);
}


void test_CLS_funcs()
{
    SET_TEST_FILE();
    
    RUN_TEST(test_cls);
    RUN_TEST(test_CLS_S32);
    RUN_TEST(test_CLS_S16);
    RUN_TEST(test_CLS_C32);
    RUN_TEST(test_CLS_C16);
    RUN_TEST(test_CLS_S64);
}