// Copyright 2020 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "bfp_math.h"

#include "../tst_common.h"

#include "unity.h"

#if DEBUG_ON || 0
#undef DEBUG_ON
#define DEBUG_ON    (1)
#endif


#define REPS        1000
#define MAX_LEN     1024 


static unsigned seed = 666;


static void test_bfp_s16_max()
{
    PRINTF("%s...\t(random vectors)\n", __func__);
    seed = 8566;
    int16_t dataB[MAX_LEN];
    bfp_s16_t B;
    B.data = dataB;
    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);
        B.length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        B.exp = pseudo_rand_int(&seed, -5, 5);
        B.hr = pseudo_rand_uint(&seed, 0, 15);

        float_s16_t expected = {
            .mant = INT16_MIN,
            .exp = B.exp,
        };

        for(int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int16(&seed) >> B.hr;
            expected.mant = MAX(expected.mant, B.data[i]);
        }
        bfp_s16_headroom(&B);

        float_s16_t result = bfp_s16_max(&B);

        TEST_ASSERT_EQUAL(expected.exp, result.exp);
        TEST_ASSERT_EQUAL_INT16(expected.mant, result.mant);
    }
}



static void test_bfp_s32_max()
{
    PRINTF("%s...\t(random vectors)\n", __func__);
    unsigned seed = 0x648E34A3;
    int32_t dataB[MAX_LEN];
    bfp_s32_t B;
    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);
        bfp_s32_init(&B, dataB, pseudo_rand_int(&seed, -5, 5),
                            pseudo_rand_uint(&seed, 1, MAX_LEN+1), 0);
        B.hr = pseudo_rand_uint(&seed, 0, 28);

        float_s32_t expected = {
            .mant = INT32_MIN,
            .exp = B.exp,
        };
        
        for(int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int32(&seed) >> B.hr;
            expected.mant = MAX(expected.mant, B.data[i]);
        }
        bfp_s32_headroom(&B);

        float_s32_t result = bfp_s32_max(&B);

        TEST_ASSERT_EQUAL(expected.exp, result.exp);
        TEST_ASSERT_EQUAL_INT32(expected.mant, result.mant);
    }
}









static void test_bfp_s16_min()
{
    PRINTF("%s...\t(random vectors)\n", __func__);
    seed = 8566;
    int16_t dataB[MAX_LEN];
    bfp_s16_t B;
    B.data = dataB;
    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);
        B.length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        B.exp = pseudo_rand_int(&seed, -5, 5);
        B.hr = pseudo_rand_uint(&seed, 0, 15);
        
        float_s16_t expected = {
            .mant = INT16_MAX,
            .exp = B.exp,
        };

        for(int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int16(&seed) >> B.hr;
            expected.mant = MIN(expected.mant, B.data[i]);
        }
        bfp_s16_headroom(&B);

        float_s16_t result = bfp_s16_min(&B);

        TEST_ASSERT_EQUAL(expected.exp, result.exp);
        TEST_ASSERT_EQUAL_INT16(expected.mant, result.mant);
    }
}



static void test_bfp_s32_min()
{
    PRINTF("%s...\t(random vectors)\n", __func__);
    unsigned seed = 0x648E34A3;
    int32_t dataB[MAX_LEN];
    bfp_s32_t B;
    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);
        bfp_s32_init(&B, dataB, pseudo_rand_int(&seed, -5, 5),
                            pseudo_rand_uint(&seed, 1, MAX_LEN+1), 0);
        B.hr = pseudo_rand_uint(&seed, 0, 28);

        float_s32_t expected = {
            .mant = INT32_MAX,
            .exp = B.exp,
        };
        
        for(int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int32(&seed) >> B.hr;
            expected.mant = MIN(expected.mant, B.data[i]);
        }
        bfp_s32_headroom(&B);

        float_s32_t result = bfp_s32_min(&B);

        TEST_ASSERT_EQUAL(expected.exp, result.exp);
        TEST_ASSERT_EQUAL_INT32(expected.mant, result.mant);
    }
}








static void test_bfp_s16_argmax()
{
    PRINTF("%s...\t(random vectors)\n", __func__);


    seed = 8566;
    int16_t dataB[MAX_LEN];
    bfp_s16_t B;
    B.data = dataB;
    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);
        B.length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        B.exp = pseudo_rand_int(&seed, -5, 5);
        B.hr = pseudo_rand_uint(&seed, 0, 15);
        unsigned exp = 0;
        for(int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int16(&seed) >> B.hr;
            exp = (B.data[exp] >= B.data[i])? exp : i;
        }
        bfp_s16_headroom(&B);
        unsigned result = bfp_s16_argmax(&B);

        if(result != exp){
            printf("B.data[%u] = %d //expected\n", exp, B.data[exp]);
            printf("B.data[%u] = %d //result\n", result, B.data[result]);
        }
        TEST_ASSERT_EQUAL(exp, result);
    }
}



static void test_bfp_s32_argmax()
{
    PRINTF("%s...\t(random vectors)\n", __func__);
    unsigned seed = 0x648E34A3;
    int32_t dataB[MAX_LEN];
    bfp_s32_t B;
    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);
        bfp_s32_init(&B, dataB, pseudo_rand_int(&seed, -5, 5),
                            pseudo_rand_uint(&seed, 1, MAX_LEN+1), 0);
        B.hr = pseudo_rand_uint(&seed, 0, 28);
        unsigned exp = 0;
        for(int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int32(&seed) >> B.hr;
            exp = (B.data[exp] >= B.data[i])? exp : i;
        }
        bfp_s32_headroom(&B);
        unsigned result = bfp_s32_argmax(&B);
        TEST_ASSERT_EQUAL(exp, result);
    }
}









static void test_bfp_s16_argmin()
{
    PRINTF("%s...\t(random vectors)\n", __func__);
    seed = 8566;
    int16_t dataB[MAX_LEN];
    bfp_s16_t B;
    B.data = dataB;
    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);
        B.length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        B.exp = pseudo_rand_int(&seed, -5, 5);
        B.hr = pseudo_rand_uint(&seed, 0, 15);
        unsigned exp = 0;
        for(int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int16(&seed) >> B.hr;
            exp = (B.data[exp] <= B.data[i])? exp : i;
        }
        bfp_s16_headroom(&B);
        unsigned result = bfp_s16_argmin(&B);
        TEST_ASSERT_EQUAL(exp, result);
    }
}



static void test_bfp_s32_argmin()
{
    PRINTF("%s...\t(random vectors)\n", __func__);
    unsigned seed = 0x648E34A3;
    int32_t dataB[MAX_LEN];
    bfp_s32_t B;
    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);
        bfp_s32_init(&B, dataB, pseudo_rand_int(&seed, -5, 5),
                            pseudo_rand_uint(&seed, 1, MAX_LEN+1), 0);
        B.hr = pseudo_rand_uint(&seed, 0, 28);
        unsigned exp = 0;
        for(int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int32(&seed) >> B.hr;
            exp = (B.data[exp] <= B.data[i])? exp : i;
        }
        bfp_s32_headroom(&B);
        unsigned result = bfp_s32_argmin(&B);
        TEST_ASSERT_EQUAL(exp, result);
    }
}






void test_bfp_max_min()
{
    SET_TEST_FILE();
    RUN_TEST(test_bfp_s16_max);
    RUN_TEST(test_bfp_s32_max);
    RUN_TEST(test_bfp_s16_min);
    RUN_TEST(test_bfp_s32_min);
    RUN_TEST(test_bfp_s16_argmax);
    RUN_TEST(test_bfp_s32_argmax);
    RUN_TEST(test_bfp_s16_argmin);
    RUN_TEST(test_bfp_s32_argmin);
}