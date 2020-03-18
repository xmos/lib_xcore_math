
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
        expected[i] = XS3_MIN(cls(numbers[i].re), cls(numbers[i].im));
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
        expected[i] = XS3_MIN(CLS_S16(numbers[i].re), CLS_S16(numbers[i].im));
        actual[i] = CLS_C16(numbers[i]);
    }

    TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, actual, N);
}




int test_CLS_funcs()
{
    int retval = 0;
    
    UnityBegin(__FILE__);

    RUN_TEST(test_cls);
    RUN_TEST(test_CLS_S32);
    RUN_TEST(test_CLS_S16);
    RUN_TEST(test_CLS_C32);
    RUN_TEST(test_CLS_C16);

    retval = UnityEnd();
    

    return retval;
}