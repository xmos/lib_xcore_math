
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


#define REPS        1000
#define MAX_LEN     1024 


static unsigned seed = 666;


static void test_bfp_max_s16()
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
        int16_t exp = INT16_MIN;
        for(int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int16(&seed) >> B.hr;
            exp = MAX(exp, B.data[i]);
        }
        bfp_headroom_vect_s16(&B);
        int16_t result = bfp_max_s16(&B);
        TEST_ASSERT_EQUAL_INT16(exp, result);
    }
}



static void test_bfp_max_s32()
{
    PRINTF("%s...\t(random vectors)\n", __func__);
    unsigned seed = 0x648E34A3;
    int32_t dataB[MAX_LEN];
    bfp_s32_t B;
    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);
        bfp_init_vect_s32(&B, dataB, pseudo_rand_int(&seed, -5, 5),
                            pseudo_rand_uint(&seed, 1, MAX_LEN+1), 0);
        B.hr = pseudo_rand_uint(&seed, 0, 28);
        int32_t exp = INT32_MIN;
        for(int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int32(&seed) >> B.hr;
            exp = MAX(exp, B.data[i]);
        }
        bfp_headroom_vect_s32(&B);
        int32_t result = bfp_max_s32(&B);
        TEST_ASSERT_EQUAL_INT32(exp, result);
    }
}









static void test_bfp_min_s16()
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
        int16_t exp = INT16_MAX;
        for(int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int16(&seed) >> B.hr;
            exp = MIN(exp, B.data[i]);
        }
        bfp_headroom_vect_s16(&B);
        int16_t result = bfp_min_s16(&B);
        TEST_ASSERT_EQUAL_INT16(exp, result);
    }
}



static void test_bfp_min_s32()
{
    PRINTF("%s...\t(random vectors)\n", __func__);
    unsigned seed = 0x648E34A3;
    int32_t dataB[MAX_LEN];
    bfp_s32_t B;
    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);
        bfp_init_vect_s32(&B, dataB, pseudo_rand_int(&seed, -5, 5),
                            pseudo_rand_uint(&seed, 1, MAX_LEN+1), 0);
        B.hr = pseudo_rand_uint(&seed, 0, 28);
        int32_t exp = INT32_MAX;
        for(int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int32(&seed) >> B.hr;
            exp = MIN(exp, B.data[i]);
        }
        bfp_headroom_vect_s32(&B);
        int32_t result = bfp_min_s32(&B);
        TEST_ASSERT_EQUAL_INT32(exp, result);
    }
}








static void test_bfp_argmax_s16()
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
        bfp_headroom_vect_s16(&B);
        unsigned result = bfp_argmax_s16(&B);

        if(result != exp){
            printf("B.data[%u] = %d //expected\n", exp, B.data[exp]);
            printf("B.data[%u] = %d //result\n", result, B.data[result]);
        }
        TEST_ASSERT_EQUAL(exp, result);
    }
}



static void test_bfp_argmax_s32()
{
    PRINTF("%s...\t(random vectors)\n", __func__);
    unsigned seed = 0x648E34A3;
    int32_t dataB[MAX_LEN];
    bfp_s32_t B;
    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);
        bfp_init_vect_s32(&B, dataB, pseudo_rand_int(&seed, -5, 5),
                            pseudo_rand_uint(&seed, 1, MAX_LEN+1), 0);
        B.hr = pseudo_rand_uint(&seed, 0, 28);
        unsigned exp = 0;
        for(int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int32(&seed) >> B.hr;
            exp = (B.data[exp] >= B.data[i])? exp : i;
        }
        bfp_headroom_vect_s32(&B);
        unsigned result = bfp_argmax_s32(&B);
        TEST_ASSERT_EQUAL(exp, result);
    }
}









static void test_bfp_argmin_s16()
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
        bfp_headroom_vect_s16(&B);
        unsigned result = bfp_argmin_s16(&B);
        TEST_ASSERT_EQUAL(exp, result);
    }
}



static void test_bfp_argmin_s32()
{
    PRINTF("%s...\t(random vectors)\n", __func__);
    unsigned seed = 0x648E34A3;
    int32_t dataB[MAX_LEN];
    bfp_s32_t B;
    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);
        bfp_init_vect_s32(&B, dataB, pseudo_rand_int(&seed, -5, 5),
                            pseudo_rand_uint(&seed, 1, MAX_LEN+1), 0);
        B.hr = pseudo_rand_uint(&seed, 0, 28);
        unsigned exp = 0;
        for(int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int32(&seed) >> B.hr;
            exp = (B.data[exp] <= B.data[i])? exp : i;
        }
        bfp_headroom_vect_s32(&B);
        unsigned result = bfp_argmin_s32(&B);
        TEST_ASSERT_EQUAL(exp, result);
    }
}






void test_bfp_max_min()
{
    SET_TEST_FILE();
    RUN_TEST(test_bfp_max_s16);
    RUN_TEST(test_bfp_max_s32);
    RUN_TEST(test_bfp_min_s16);
    RUN_TEST(test_bfp_min_s32);
    RUN_TEST(test_bfp_argmax_s16);
    RUN_TEST(test_bfp_argmax_s32);
    RUN_TEST(test_bfp_argmin_s16);
    RUN_TEST(test_bfp_argmin_s32);
}