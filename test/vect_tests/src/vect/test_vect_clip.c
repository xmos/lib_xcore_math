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


TEST_GROUP_RUNNER(vect_clip) {
  RUN_TEST_CASE(vect_clip, vect_s16_clip_basic);
  RUN_TEST_CASE(vect_clip, vect_s32_clip_basic);
}

TEST_GROUP(vect_clip);
TEST_SETUP(vect_clip) { fflush(stdout); }
TEST_TEAR_DOWN(vect_clip) {}

static char msg_buff[200];

#define TEST_ASSERT_EQUAL_MSG(EXPECTED, ACTUAL, LINE_NUM)   do{       \
    if((EXPECTED)!=(ACTUAL)) {                                        \
      sprintf(msg_buff, "(test vector @ line %u)", (LINE_NUM));       \
      TEST_ASSERT_EQUAL_MESSAGE((EXPECTED), (ACTUAL), msg_buff);      \
    }} while(0)



#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (64)
#else
#  define REPS       (1000)
#  define MAX_LEN    (256)
#endif


TEST(vect_clip, vect_s16_clip_basic) //
{
    

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

        for(unsigned int v = start_case; v < N_cases; v++){
            setExtraInfo_R(v);
            
            test_case_t* casse = &casses[v];
            
            headroom_t hr;
            int16_t A[40];
            int16_t B[40];

            for(unsigned int i = 0; i < len; i++){
                A[i] = 0xCC;
                B[i] = casse->b;
            }


            hr = vect_s16_clip(A, B, len, casse->lower, casse->upper, casse->b_shr);

            for(unsigned int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(vect_s16_headroom(A, len), hr, casse->line);
            }

            memcpy(A, B, sizeof(A));
            hr = vect_s16_clip(A, A, len, casse->lower, casse->upper, casse->b_shr);

            for(unsigned int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(vect_s16_headroom(A, len), hr, casse->line);
            }
        }
    }
}


TEST(vect_clip, vect_s32_clip_basic)
{
    

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

        for(unsigned int v = start_case; v < N_cases; v++){
            setExtraInfo_R(v);
            
            test_case_t* casse = &casses[v];
            
            headroom_t hr;
            int32_t A[40];
            int32_t B[40];

            for(unsigned int i = 0; i < len; i++){
                A[i] = 0xCCCCCC;
                B[i] = casse->b;
            }


            hr = vect_s32_clip(A, B, len, casse->lower, casse->upper, casse->b_shr);

            for(unsigned int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(vect_s32_headroom(A, len), hr, casse->line);
            }

            memcpy(A, B, sizeof(A));
            hr = vect_s32_clip(A, A, len, casse->lower, casse->upper, casse->b_shr);

            for(unsigned int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(vect_s32_headroom(A, len), hr, casse->line);
            }
        }
    }
}

