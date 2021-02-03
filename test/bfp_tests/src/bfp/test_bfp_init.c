// Copyright 2020 XMOS LIMITED. This Software is subject to the terms of the 
// XMOS Public License: Version 1

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


#define REPS        (50)
#define MAX_LEN     (300)





static void test_bfp_s16_init()
{
    PRINTF("%s...\n", __func__);

    unsigned seed = 3724523;
    
    int16_t WORD_ALIGNED data[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep %d..\n", r);

        unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);

        exponent_t exponent = pseudo_rand_int(&seed, -20, 20);

        int shr = pseudo_rand_uint(&seed, 0, 10);

        for(int i = 0; i < length; i++){
            data[i] = pseudo_rand_int16(&seed) >> shr;
        }

        headroom_t exp_hr = xs3_vect_s16_headroom(data, length);  

        bfp_s16_t A;

        bfp_s16_init(&A, data, exponent, length, 0);

        TEST_ASSERT_EQUAL(data, A.data);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(0, A.hr);

        memset(&A, 0, sizeof(A));

        bfp_s16_init(&A, data, exponent, length, 1);

        TEST_ASSERT_EQUAL(data, A.data);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(exp_hr, A.hr);
    }
}



static void test_bfp_s32_init()
{
    PRINTF("%s...\n", __func__);

    unsigned seed = 2342;
    
    int32_t WORD_ALIGNED data[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep %d..\n", r);

        unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);

        exponent_t exponent = pseudo_rand_int(&seed, -20, 20);

        int shr = pseudo_rand_uint(&seed, 0, 10);

        for(int i = 0; i < length; i++){
            data[i] = pseudo_rand_int32(&seed) >> shr;
        }

        headroom_t exp_hr = xs3_vect_s32_headroom(data, length);

        bfp_s32_t A;

        bfp_s32_init(&A, data, exponent, length, 0);

        TEST_ASSERT_EQUAL(data, A.data);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(0, A.hr);

        memset(&A, 0, sizeof(A));

        bfp_s32_init(&A, data, exponent, length, 1);

        TEST_ASSERT_EQUAL(data, A.data);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(exp_hr, A.hr);
    }
}





static void test_bfp_complex_s16_init()
{
    PRINTF("%s...\n", __func__);

    unsigned seed = 233345;
    
    int16_t WORD_ALIGNED real_data[MAX_LEN];
    int16_t WORD_ALIGNED imag_data[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep %d..\n", r);

        unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);

        exponent_t exponent = pseudo_rand_int(&seed, -20, 20);

        int shr = pseudo_rand_uint(&seed, 0, 10);

        for(int i = 0; i < length; i++){
            real_data[i] = pseudo_rand_int16(&seed) >> (shr + (pseudo_rand_uint32(&seed) % 4));
            imag_data[i] = pseudo_rand_int16(&seed) >> (shr + (pseudo_rand_uint32(&seed) % 4));
        }

        headroom_t exp_hr_r = xs3_vect_s16_headroom(real_data, length);
        headroom_t exp_hr_i = xs3_vect_s16_headroom(imag_data, length);

        headroom_t exp_hr = (exp_hr_r < exp_hr_i)? exp_hr_r : exp_hr_i;

        bfp_complex_s16_t A;

        bfp_complex_s16_init(&A, real_data, imag_data, exponent, length, 0);

        TEST_ASSERT_EQUAL(real_data, A.real);
        TEST_ASSERT_EQUAL(imag_data, A.imag);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(0, A.hr);

        memset(&A, 0, sizeof(A));

        bfp_complex_s16_init(&A, real_data, imag_data, exponent, length, 1);

        TEST_ASSERT_EQUAL(real_data, A.real);
        TEST_ASSERT_EQUAL(imag_data, A.imag);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(exp_hr, A.hr);
    }
}





static void test_bfp_complex_s32_init()
{
    PRINTF("%s...\n", __func__);

    unsigned seed = 7646785;
    
    complex_s32_t WORD_ALIGNED data[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep %d..\n", r);

        unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);

        exponent_t exponent = pseudo_rand_int(&seed, -20, 20);

        int shr = pseudo_rand_uint(&seed, 0, 10);

        for(int i = 0; i < length; i++){
            data[i].re = pseudo_rand_int16(&seed) >> (shr + (pseudo_rand_uint32(&seed) % 7));
            data[i].im = pseudo_rand_int16(&seed) >> (shr + (pseudo_rand_uint32(&seed) % 7));
        }

        headroom_t exp_hr = xs3_vect_s32_headroom( (int32_t*)data, 2*length);

        bfp_complex_s32_t A;

        bfp_complex_s32_init(&A, data, exponent, length, 0);

        TEST_ASSERT_EQUAL(data, A.data);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(0, A.hr);

        memset(&A, 0, sizeof(A));

        bfp_complex_s32_init(&A, data, exponent, length, 1);

        TEST_ASSERT_EQUAL(data, A.data);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(exp_hr, A.hr);
    }
}





static void test_bfp_ch_pair_s16_init()
{
    PRINTF("%s...\n", __func__);

    unsigned seed = 2344;
    
    ch_pair_s16_t WORD_ALIGNED data[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep %d..\n", r);

        unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);

        exponent_t exponent = pseudo_rand_int(&seed, -20, 20);

        int shr = pseudo_rand_uint(&seed, 0, 10);

        for(int i = 0; i < length; i++){
            data[i].ch_a = pseudo_rand_int16(&seed) >> (shr + (pseudo_rand_uint32(&seed) % 4));
            data[i].ch_b = pseudo_rand_int16(&seed) >> (shr + (pseudo_rand_uint32(&seed) % 4));
        }

        headroom_t exp_hr = xs3_vect_ch_pair_s16_headroom( data, length);

        bfp_ch_pair_s16_t A;

        bfp_ch_pair_s16_init(&A, data, exponent, length, 0);

        TEST_ASSERT_EQUAL(data, A.data);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(0, A.hr);

        memset(&A, 0, sizeof(A));

        bfp_ch_pair_s16_init(&A, data, exponent, length, 1);

        TEST_ASSERT_EQUAL(data, A.data);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(exp_hr, A.hr);
    }
}





static void test_bfp_ch_pair_s32_init()
{
    PRINTF("%s...\n", __func__);

    unsigned seed = 33456;
    
    ch_pair_s32_t WORD_ALIGNED data[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep %d..\n", r);

        unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);

        exponent_t exponent = pseudo_rand_int(&seed, -20, 20);

        int shr = pseudo_rand_uint(&seed, 0, 10);

        for(int i = 0; i < length; i++){
            data[i].ch_a = pseudo_rand_int16(&seed) >> (shr + (pseudo_rand_uint32(&seed) % 7));
            data[i].ch_b = pseudo_rand_int16(&seed) >> (shr + (pseudo_rand_uint32(&seed) % 7));
        }

        headroom_t exp_hr = xs3_vect_ch_pair_s32_headroom(data, length);

        bfp_ch_pair_s32_t A;

        bfp_ch_pair_s32_init(&A, data, exponent, length, 0);

        TEST_ASSERT_EQUAL(data, A.data);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(0, A.hr);

        memset(&A, 0, sizeof(A));

        bfp_ch_pair_s32_init(&A, data, exponent, length, 1);

        TEST_ASSERT_EQUAL(data, A.data);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(exp_hr, A.hr);
    }
}




void test_bfp_init_vect()
{
    SET_TEST_FILE();

    RUN_TEST(test_bfp_s16_init);
    RUN_TEST(test_bfp_s32_init);
    RUN_TEST(test_bfp_complex_s16_init);
    RUN_TEST(test_bfp_complex_s32_init);
    RUN_TEST(test_bfp_ch_pair_s16_init);
    RUN_TEST(test_bfp_ch_pair_s32_init);
}