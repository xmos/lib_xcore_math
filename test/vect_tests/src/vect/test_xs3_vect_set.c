// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xs3_math.h"

#include "../tst_common.h"

#include "unity.h"


static unsigned seed = 2314567;

#if DEBUG_ON || 0
#undef DEBUG_ON
#define DEBUG_ON    (1)
#endif

#define MAX_LEN     1024
#define REPS        (1000)




static void test_xs3_vect_s16_set()
{
    PRINTF("%s...\n", __func__);

    seed = 0x5F0BE930;

    int16_t WORD_ALIGNED A[MAX_LEN];

    for(int v = 0; v < REPS; v++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", v, seed);

        unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        
        memset(A, 0xCC, sizeof(A));

        const int16_t new_val = pseudo_rand_int16(&seed);
        
        xs3_vect_s16_set(A, new_val, length);

        for(int i = 0; i < length; i++)
            TEST_ASSERT_EQUAL_INT16(new_val, A[i]);
        for(int i = length; i < MAX_LEN; i++)
            TEST_ASSERT_EQUAL_INT16(0xCCCC, A[i]);
        
    }
}


static void test_xs3_vect_s32_set()
{
    PRINTF("%s...\n", __func__);

    seed = 45674;

    int32_t WORD_ALIGNED A[MAX_LEN];

    for(int v = 0; v < REPS; v++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", v, seed);

        unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        
        memset(A, 0xCC, sizeof(A));

        const int32_t new_val = pseudo_rand_int32(&seed);
        
        xs3_vect_s32_set(A, new_val, length);

        for(int i = 0; i < length; i++)
            TEST_ASSERT_EQUAL_INT32(new_val, A[i]);
        for(int i = length; i < MAX_LEN; i++)
            TEST_ASSERT_EQUAL_INT32(0xCCCCCCCC, A[i]);
        
    }
}





static void test_xs3_vect_complex_s16_set()
{
    PRINTF("%s...\n", __func__);

    seed = 6567888;

    int16_t WORD_ALIGNED A_real[MAX_LEN];
    int16_t WORD_ALIGNED A_imag[MAX_LEN];

    for(int v = 0; v < REPS; v++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", v, seed);

        unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        
        memset(A_real, 0xCC, sizeof(A_real));
        memset(A_imag, 0xCC, sizeof(A_imag));

        const int16_t new_real = pseudo_rand_int16(&seed);
        const int16_t new_imag = pseudo_rand_int16(&seed);
        
        xs3_vect_complex_s16_set(A_real, A_imag, new_real, new_imag, length);

        for(int i = 0; i < length; i++){
            TEST_ASSERT_EQUAL_INT16(new_real, A_real[i]);
            TEST_ASSERT_EQUAL_INT16(new_imag, A_imag[i]);
        }
        for(int i = length; i < MAX_LEN; i++){
            TEST_ASSERT_EQUAL_INT16(0xCCCC, A_real[i]);
            TEST_ASSERT_EQUAL_INT16(0xCCCC, A_imag[i]);
        }
        
    }
}





static void test_xs3_vect_complex_s32_set()
{
    PRINTF("%s...\n", __func__);

    seed = 73734;

    complex_s32_t WORD_ALIGNED A[MAX_LEN];

    for(int v = 0; v < REPS; v++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", v, seed);

        unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        
        memset(A, 0xCC, sizeof(A));

        const int32_t new_real = pseudo_rand_int32(&seed);
        const int32_t new_imag = pseudo_rand_int32(&seed);
        
        xs3_vect_complex_s32_set(A, new_real, new_imag, length);

        for(int i = 0; i < length; i++){
            TEST_ASSERT_EQUAL_INT32(new_real, A[i].re);
            TEST_ASSERT_EQUAL_INT32(new_imag, A[i].im);
        }
        for(int i = length; i < MAX_LEN; i++){
            TEST_ASSERT_EQUAL_INT32(0xCCCCCCCC, A[i].re);
            TEST_ASSERT_EQUAL_INT32(0xCCCCCCCC, A[i].im);
        }
        
    }
}






static void test_xs3_vect_ch_pair_s16_set()
{
    PRINTF("%s...\n", __func__);

    seed = 8808;

    ch_pair_s16_t WORD_ALIGNED A[MAX_LEN];

    for(int v = 0; v < REPS; v++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", v, seed);

        unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        
        memset(A, 0xCC, sizeof(A));

        const int16_t new_ch_a = pseudo_rand_int16(&seed);
        const int16_t new_ch_b = pseudo_rand_int16(&seed);
        
        xs3_vect_ch_pair_s16_set(A, new_ch_a, new_ch_b, length);

        for(int i = 0; i < length; i++){
            TEST_ASSERT_EQUAL_INT16(new_ch_a, A[i].ch_a);
            TEST_ASSERT_EQUAL_INT16(new_ch_b, A[i].ch_b);
        }
        for(int i = length; i < MAX_LEN; i++){
            TEST_ASSERT_EQUAL_INT16(0xCCCC, A[i].ch_a);
            TEST_ASSERT_EQUAL_INT16(0xCCCC, A[i].ch_b);
        }
        
    }
}






static void test_xs3_vect_ch_pair_s32_set()
{
    PRINTF("%s...\n", __func__);

    seed = 456678;

    ch_pair_s32_t WORD_ALIGNED A[MAX_LEN];

    for(int v = 0; v < REPS; v++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", v, seed);

        unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        
        memset(A, 0xCC, sizeof(A));

        const int32_t new_ch_a = pseudo_rand_int32(&seed);
        const int32_t new_ch_b = pseudo_rand_int32(&seed);
        
        xs3_vect_ch_pair_s32_set(A, new_ch_a, new_ch_b, length);

        for(int i = 0; i < length; i++){
            TEST_ASSERT_EQUAL_INT32(new_ch_a, A[i].ch_a);
            TEST_ASSERT_EQUAL_INT32(new_ch_b, A[i].ch_b);
        }
        for(int i = length; i < MAX_LEN; i++){
            TEST_ASSERT_EQUAL_INT32(0xCCCCCCCC, A[i].ch_a);
            TEST_ASSERT_EQUAL_INT32(0xCCCCCCCC, A[i].ch_b);
        }
        
    }
}


void test_xs3_set_vect()
{
    SET_TEST_FILE();

    RUN_TEST(test_xs3_vect_s16_set);
    RUN_TEST(test_xs3_vect_s32_set);
    RUN_TEST(test_xs3_vect_complex_s16_set);
    RUN_TEST(test_xs3_vect_complex_s32_set);
    RUN_TEST(test_xs3_vect_ch_pair_s16_set);
    RUN_TEST(test_xs3_vect_ch_pair_s32_set);
}