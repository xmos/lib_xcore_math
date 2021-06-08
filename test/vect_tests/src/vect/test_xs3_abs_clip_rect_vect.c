// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "xs3_math.h"

#include "../tst_common.h"

#include "unity.h"

static char msg_buff[200];

#define TEST_ASSERT_EQUAL_MSG(EXPECTED, ACTUAL, LINE_NUM)   do{       \
    if((EXPECTED)!=(ACTUAL)) {                                        \
      sprintf(msg_buff, "(test vector @ line %u)", (LINE_NUM));       \
      TEST_ASSERT_EQUAL_MESSAGE((EXPECTED), (ACTUAL), msg_buff);      \
    }} while(0)


#if !defined(DEBUG_ON) || 0
#undef DEBUG_ON
#define DEBUG_ON    (1)
#endif





#define MAX_LEN     256
#define REPS        1000




static void test_xs3_vect_s16_abs()
{
    PRINTF("%s...\n", __func__);
    unsigned seed = 74754;
    
    headroom_t hr;
    int16_t A[MAX_LEN];
    int16_t B[MAX_LEN];

    //If something's really wrong, debugging this first will be easiest.
    if(1){
        A[0] =  0xCCCC;
        A[1] =  0xCCCC;
        B[0] =  0x1234;
        B[1] = -0x2480;

        hr = xs3_vect_s16_abs(A, B, 2);
        TEST_ASSERT_EQUAL(0x1234, A[0]);
        TEST_ASSERT_EQUAL(0x2480, A[1]);
        TEST_ASSERT_EQUAL(1, hr);
    }

    for(int v = 0; v < REPS; v++){

        PRINTF("\trepetition %d..\n", v);

        const unsigned len = pseudo_rand_uint32(&seed) % MAX_LEN;

        for(int i = 0; i < len; i++){
            unsigned shr = pseudo_rand_uint32(&seed) % 10;
            B[i] = pseudo_rand_int16(&seed) >> shr;
        }

        memset(A, 0xCC, sizeof(A));
        hr = xs3_vect_s16_abs(A, B, len);

        for(int i = 0; i < len; i++)
            TEST_ASSERT_EQUAL(B[i] >= 0? B[i] : -B[i], A[i]);
        for(int i = len; i < MAX_LEN; i++)
            TEST_ASSERT_EQUAL((int16_t)0xCCCC, A[i]);

        memcpy(A, B, sizeof(A));
        hr = xs3_vect_s16_abs(A, A, len);

        for(int i = 0; i < len; i++)
            TEST_ASSERT_EQUAL(B[i] >= 0? B[i] : -B[i], A[i]);
    }
}






static void test_xs3_vect_s32_abs()
{
    PRINTF("%s...\n", __func__);
    unsigned seed = 555226;
    
    headroom_t hr;
    int32_t A[MAX_LEN];
    int32_t B[MAX_LEN];

    //If something's really wrong, debugging this first will be easiest.
    if(1){
        A[0] =  0xCCCCCCCC;
        A[1] =  0xCCCCCCCC;
        B[0] =  0x12345678;
        B[1] = -0x22345678;

        hr = xs3_vect_s32_abs(A, B, 2);
        TEST_ASSERT_EQUAL(0x12345678, A[0]);
        TEST_ASSERT_EQUAL(0x22345678, A[1]);
        TEST_ASSERT_EQUAL(1, hr);
    }

    for(int v = 0; v < REPS; v++){

        PRINTF("\trepetition %d..\n", v);

        const unsigned len = pseudo_rand_uint32(&seed) % MAX_LEN;

        for(int i = 0; i < len; i++){
            unsigned shr = pseudo_rand_uint32(&seed) % 10;
            B[i] = pseudo_rand_int32(&seed) >> shr;
        }

        memset(A, 0xCC, sizeof(A));
        hr = xs3_vect_s32_abs(A, B, len);

        for(int i = 0; i < len; i++)
            TEST_ASSERT_EQUAL(B[i] >= 0? B[i] : -B[i], A[i]);
        for(int i = len; i < MAX_LEN; i++)
            TEST_ASSERT_EQUAL((int32_t)0xCCCCCCCC, A[i]);

        memcpy(A, B, sizeof(A));
        hr = xs3_vect_s32_abs(A, A, len);

        for(int i = 0; i < len; i++)
            TEST_ASSERT_EQUAL(B[i] >= 0? B[i] : -B[i], A[i]);
    }
}






static void test_xs3_vect_s16_clip_basic()
{
    PRINTF("%s...\n", __func__);

    typedef struct {
        int16_t b;
        int b_shr;
        int16_t lower;
        int16_t upper;
        int16_t expected;
        unsigned line;
    } test_case_t;

    test_case_t casses[] = {
        //       b   b_shr      lower      upper      exp        line num
        {   0x0000,     0,     -0x7FFF,    0x7FFF,    0x0000,       __LINE__},
        {   0x0001,     0,     -0x7FFF,    0x7FFF,    0x0001,       __LINE__},
        {   0x000A,     0,     -0x7FFF,    0x7FFF,    0x000A,       __LINE__},
        {   0x0F00,     0,     -0x7FFF,    0x7FFF,    0x0F00,       __LINE__},
        {  -0x0FFF,     0,     -0x7FFF,    0x7FFF,   -0x0FFF,       __LINE__},
        {  -0x8000,     0,     -0x7FFF,    0x7FFF,   -0x7FFF,       __LINE__},

        {   0x0000,     2,     -0x7FFF,    0x7FFF,    0x0000,       __LINE__},
        {   0x0002,     1,     -0x7FFF,    0x7FFF,    0x0001,       __LINE__},
        {   0x0005,    -1,     -0x7FFF,    0x7FFF,    0x000A,       __LINE__},
        {   0x0F00,     4,     -0x7FFF,    0x7FFF,    0x00F0,       __LINE__},
        {  -0x0FFF,    -1,     -0x7FFF,    0x7FFF,   -0x1FFE,       __LINE__},
        
        {   0x0000,     0,     -0x1000,    0x1000,    0x0000,       __LINE__},
        {   0x0100,     0,     -0x1000,    0x1000,    0x0100,       __LINE__},
        {   0x1100,     0,     -0x1000,    0x1000,    0x1000,       __LINE__},
        {  -0x1100,     0,     -0x1000,    0x1000,   -0x1000,       __LINE__},
        {   0x2300,     1,     -0x1000,    0x1000,    0x1000,       __LINE__},
        {  -0x7800,     2,     -0x1000,    0x1000,   -0x1000,       __LINE__},

        {   0x0000,     0,      0x1000,    0x2000,    0x1000,       __LINE__},
        {   0x0100,     0,      0x1000,    0x2000,    0x1000,       __LINE__},
        {  -0x0100,     0,      0x1000,    0x2000,    0x1000,       __LINE__},
        {   0x2100,     0,      0x1000,    0x2000,    0x2000,       __LINE__},
        {  -0x2100,     0,      0x1000,    0x2000,    0x1000,       __LINE__},
        {   0x7FFF,     0,      0x1000,    0x2000,    0x2000,       __LINE__},
        {  -0x7FFF,     0,      0x1000,    0x2000,    0x1000,       __LINE__},
        {   0x7000,     0,      0x1000,    0x2000,    0x2000,       __LINE__},
        {  -0x7000,     0,      0x1000,    0x2000,    0x1000,       __LINE__},
        {   0x7000,     1,      0x1000,    0x2000,    0x2000,       __LINE__},
        {  -0x7000,     1,      0x1000,    0x2000,    0x1000,       __LINE__},

        {   0x0000,     0,     -0x2000,   -0x1000,   -0x1000,       __LINE__},
        {  -0x0100,     0,     -0x2000,   -0x1000,   -0x1000,       __LINE__},
        {   0x0100,     0,     -0x2000,   -0x1000,   -0x1000,       __LINE__},
        {  -0x2100,     0,     -0x2000,   -0x1000,   -0x2000,       __LINE__},
        {   0x2100,     0,     -0x2000,   -0x1000,   -0x1000,       __LINE__},
        {  -0x7FFF,     0,     -0x2000,   -0x1000,   -0x2000,       __LINE__},
        {   0x7FFF,     0,     -0x2000,   -0x1000,   -0x1000,       __LINE__},
        {  -0x7000,     0,     -0x2000,   -0x1000,   -0x2000,       __LINE__},
        {   0x7000,     0,     -0x2000,   -0x1000,   -0x1000,       __LINE__},
        {  -0x7000,     1,     -0x2000,   -0x1000,   -0x2000,       __LINE__},
        {   0x7000,     1,     -0x2000,   -0x1000,   -0x1000,       __LINE__},
    };

    const unsigned N_cases = sizeof(casses)/sizeof(test_case_t);

    const unsigned start_case = 0;

    unsigned lengths[] = {1, 4, 16, 32, 40 };
    for( int l = 0; l < sizeof(lengths)/sizeof(lengths[0]); l++){
        unsigned len = lengths[l];
        PRINTF("\tlength %u\n", len);

        for(int v = start_case; v < N_cases; v++){
            PRINTF("\t\ttest vector %d..\n", v);
            
            test_case_t* casse = &casses[v];
            
            headroom_t hr;
            int16_t A[40];
            int16_t B[40];

            for(int i = 0; i < len; i++){
                A[i] = 0xCC;
                B[i] = casse->b;
            }


            hr = xs3_vect_s16_clip(A, B, len, casse->lower, casse->upper, casse->b_shr);

            for(int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(xs3_vect_s16_headroom(A, len), hr, casse->line);
            }

            memcpy(A, B, sizeof(A));
            hr = xs3_vect_s16_clip(A, A, len, casse->lower, casse->upper, casse->b_shr);

            for(int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(xs3_vect_s16_headroom(A, len), hr, casse->line);
            }
        }
    }
}



static void test_xs3_vect_s32_clip_basic()
{
    PRINTF("%s...\n", __func__);

    typedef struct {
        int32_t b;
        int b_shr;
        int32_t lower;
        int32_t upper;
        int32_t expected;
        unsigned line;
    } test_case_t;

    test_case_t casses[] = {
        //       b   b_shr      lower      upper      exp        line num
        {   0x0000,     0,     -0x7FFF,    0x7FFF,    0x0000,       __LINE__},
        {   0x0001,     0,     -0x7FFF,    0x7FFF,    0x0001,       __LINE__},
        {   0x000A,     0,     -0x7FFF,    0x7FFF,    0x000A,       __LINE__},
        {   0x0F00,     0,     -0x7FFF,    0x7FFF,    0x0F00,       __LINE__},
        {  -0x0FFF,     0,     -0x7FFF,    0x7FFF,   -0x0FFF,       __LINE__},
        {  -0x8000,     0,     -0x7FFF,    0x7FFF,   -0x7FFF,       __LINE__},

        {   0x0000,     2,     -0x7FFF,    0x7FFF,    0x0000,       __LINE__},
        {   0x0002,     1,     -0x7FFF,    0x7FFF,    0x0001,       __LINE__},
        {   0x0005,    -1,     -0x7FFF,    0x7FFF,    0x000A,       __LINE__},
        {   0x0F00,     4,     -0x7FFF,    0x7FFF,    0x00F0,       __LINE__},
        {  -0x0FFF,    -1,     -0x7FFF,    0x7FFF,   -0x1FFE,       __LINE__},
        
        {   0x0000,     0,     -0x1000,    0x1000,    0x0000,       __LINE__},
        {   0x0100,     0,     -0x1000,    0x1000,    0x0100,       __LINE__},
        {   0x1100,     0,     -0x1000,    0x1000,    0x1000,       __LINE__},
        {  -0x1100,     0,     -0x1000,    0x1000,   -0x1000,       __LINE__},
        {   0x2300,     1,     -0x1000,    0x1000,    0x1000,       __LINE__},
        {  -0x7800,     2,     -0x1000,    0x1000,   -0x1000,       __LINE__},

        {   0x0000,     0,      0x1000,    0x2000,    0x1000,       __LINE__},
        {   0x0100,     0,      0x1000,    0x2000,    0x1000,       __LINE__},
        {  -0x0100,     0,      0x1000,    0x2000,    0x1000,       __LINE__},
        {   0x2100,     0,      0x1000,    0x2000,    0x2000,       __LINE__},
        {  -0x2100,     0,      0x1000,    0x2000,    0x1000,       __LINE__},
        {   0x7FFF,     0,      0x1000,    0x2000,    0x2000,       __LINE__},
        {  -0x7FFF,     0,      0x1000,    0x2000,    0x1000,       __LINE__},
        {   0x7000,     0,      0x1000,    0x2000,    0x2000,       __LINE__},
        {  -0x7000,     0,      0x1000,    0x2000,    0x1000,       __LINE__},
        {   0x7000,     1,      0x1000,    0x2000,    0x2000,       __LINE__},
        {  -0x7000,     1,      0x1000,    0x2000,    0x1000,       __LINE__},

        {   0x0000,     0,     -0x2000,   -0x1000,   -0x1000,       __LINE__},
        {  -0x0100,     0,     -0x2000,   -0x1000,   -0x1000,       __LINE__},
        {   0x0100,     0,     -0x2000,   -0x1000,   -0x1000,       __LINE__},
        {  -0x2100,     0,     -0x2000,   -0x1000,   -0x2000,       __LINE__},
        {   0x2100,     0,     -0x2000,   -0x1000,   -0x1000,       __LINE__},
        {  -0x7FFF,     0,     -0x2000,   -0x1000,   -0x2000,       __LINE__},
        {   0x7FFF,     0,     -0x2000,   -0x1000,   -0x1000,       __LINE__},
        {  -0x7000,     0,     -0x2000,   -0x1000,   -0x2000,       __LINE__},
        {   0x7000,     0,     -0x2000,   -0x1000,   -0x1000,       __LINE__},
        {  -0x7000,     1,     -0x2000,   -0x1000,   -0x2000,       __LINE__},
        {   0x7000,     1,     -0x2000,   -0x1000,   -0x1000,       __LINE__},
    };

    const unsigned N_cases = sizeof(casses)/sizeof(test_case_t);

    const unsigned start_case = 0;

    unsigned lengths[] = {1, 4, 16, 32, 40 };
    for( int l = 0; l < sizeof(lengths)/sizeof(lengths[0]); l++){
        unsigned len = lengths[l];
        PRINTF("\tlength %u\n", len);

        for(int v = start_case; v < N_cases; v++){
            PRINTF("\t\ttest vector %d..\n", v);
            
            test_case_t* casse = &casses[v];
            
            headroom_t hr;
            int32_t A[40];
            int32_t B[40];

            for(int i = 0; i < len; i++){
                A[i] = 0xCCCCCC;
                B[i] = casse->b;
            }


            hr = xs3_vect_s32_clip(A, B, len, casse->lower, casse->upper, casse->b_shr);

            for(int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(xs3_vect_s32_headroom(A, len), hr, casse->line);
            }

            memcpy(A, B, sizeof(A));
            hr = xs3_vect_s32_clip(A, A, len, casse->lower, casse->upper, casse->b_shr);

            for(int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(xs3_vect_s32_headroom(A, len), hr, casse->line);
            }
        }
    }
}




static void test_xs3_vect_s16_rect()
{
    PRINTF("%s...\n", __func__);
    unsigned seed = 235426;
    
    headroom_t hr;
    int16_t A[MAX_LEN];
    int16_t B[MAX_LEN];

    //If something's really wrong, debugging this first will be easiest.
    if(1){
        A[0] =  0xCCCC;
        A[1] =  0xCCCC;
        B[0] =  0x1234;
        B[1] = -0x1234;

        hr = xs3_vect_s16_rect(A, B, 2);
        TEST_ASSERT_EQUAL(0x1234, A[0]);
        TEST_ASSERT_EQUAL(0x0000, A[1]);
        TEST_ASSERT_EQUAL(2, hr);
    }

    for(int v = 0; v < REPS; v++){

        PRINTF("\trepetition %d..\n", v);

        const unsigned len = pseudo_rand_uint32(&seed) % MAX_LEN;

        for(int i = 0; i < len; i++){
            unsigned shr = pseudo_rand_uint32(&seed) % 10;
            B[i] = pseudo_rand_int16(&seed) >> shr;
        }

        memset(A, 0xCC, sizeof(A));
        hr = xs3_vect_s16_rect(A, B, len);

        for(int i = 0; i < len; i++)
            TEST_ASSERT_EQUAL(B[i] >= 0? B[i] : 0, A[i]);
        for(int i = len; i < MAX_LEN; i++)
            TEST_ASSERT_EQUAL((int16_t)0xCCCC, A[i]);

        memcpy(A, B, sizeof(A));
        hr = xs3_vect_s16_rect(A, A, len);

        for(int i = 0; i < len; i++)
            TEST_ASSERT_EQUAL(B[i] >= 0? B[i] : 0, A[i]);
    }
}





static void test_xs3_vect_s32_rect()
{
    PRINTF("%s...\n", __func__);
    unsigned seed = 1212445;
    
    headroom_t hr;
    int32_t A[MAX_LEN];
    int32_t B[MAX_LEN];

    //If something's really wrong, debugging this first will be easiest.
    if(1){
        A[0] =  0xCCCCCCCC;
        A[1] =  0xCCCCCCCC;
        B[0] =  0x12345678;
        B[1] = -0x12345678;

        hr = xs3_vect_s32_rect(A, B, 2);
        TEST_ASSERT_EQUAL(0x12345678, A[0]);
        TEST_ASSERT_EQUAL(0x00000000, A[1]);
        TEST_ASSERT_EQUAL(2, hr);
    }

    for(int v = 0; v < REPS; v++){

        PRINTF("\trepetition %d..\n", v);

        const unsigned len = pseudo_rand_uint32(&seed) % MAX_LEN;

        for(int i = 0; i < len; i++){
            unsigned shr = pseudo_rand_uint32(&seed) % 10;
            B[i] = pseudo_rand_int32(&seed) >> shr;
        }

        memset(A, 0xCC, sizeof(A));
        hr = xs3_vect_s32_rect(A, B, len);

        for(int i = 0; i < len; i++)
            TEST_ASSERT_EQUAL(B[i] >= 0? B[i] : 0, A[i]);
        for(int i = len; i < MAX_LEN; i++)
            TEST_ASSERT_EQUAL((int32_t)0xCCCCCCCC, A[i]);

        memcpy(A, B, sizeof(A));
        hr = xs3_vect_s32_rect(A, A, len);

        for(int i = 0; i < len; i++)
            TEST_ASSERT_EQUAL(B[i] >= 0? B[i] : 0, A[i]);
    }
}


void test_xs3_abs_clip_rect_vect()
{
    SET_TEST_FILE();

    RUN_TEST(test_xs3_vect_s16_abs);
    RUN_TEST(test_xs3_vect_s32_abs);

    RUN_TEST(test_xs3_vect_s16_clip_basic);
    RUN_TEST(test_xs3_vect_s32_clip_basic);

    RUN_TEST(test_xs3_vect_s16_rect);
    RUN_TEST(test_xs3_vect_s32_rect);
}
