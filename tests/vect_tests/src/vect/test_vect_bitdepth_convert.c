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


TEST_GROUP_RUNNER(vect_bitdepth_convert) {
  RUN_TEST_CASE(vect_bitdepth_convert, vect_s16_to_vect_s32_basic);
  RUN_TEST_CASE(vect_bitdepth_convert, vect_s16_to_vect_s32_random);
  RUN_TEST_CASE(vect_bitdepth_convert, vect_s32_to_vect_s16_basic);
}

TEST_GROUP(vect_bitdepth_convert);
TEST_SETUP(vect_bitdepth_convert) { fflush(stdout); }
TEST_TEAR_DOWN(vect_bitdepth_convert) {}

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


TEST(vect_bitdepth_convert, vect_s16_to_vect_s32_basic)
{
    

    typedef struct {
        int16_t input;
        unsigned line;
    } test_case_t;

    test_case_t casses[] = {
        //   input      line #
        {   0x0000,     __LINE__},
        {   0x0001,     __LINE__},
        {   0x0100,     __LINE__},
        {   0x0080,     __LINE__},
        {   0x8000,     __LINE__},
        {   0x7FFF,     __LINE__},
        {   0x8000,     __LINE__},
        {   0x0080,     __LINE__},
        {   0x8080,     __LINE__},
        {   0x00FF,     __LINE__},
        {   0xFF00,     __LINE__},
    };

    const unsigned N_cases = sizeof(casses)/sizeof(test_case_t);

    const unsigned start_case = 0;

    for(unsigned int v = start_case; v < N_cases; v++){
        setExtraInfo_R(v);
        
        test_case_t* casse = &casses[v];

        unsigned lengths[] = {1, 4, 16, 32, 40 };
        int32_t WORD_ALIGNED A[MAX_LEN] = { 0 };
        int16_t WORD_ALIGNED B[MAX_LEN] = { 0 };

        for(int i = 0; i < sizeof(lengths)/sizeof(lengths[0]); i++){
            unsigned len = lengths[i];

            for(unsigned int i = 0; i < MAX_LEN; i++){
                B[i] = i < len? casse->input : 0xBBBB;
                A[i] = 0xCCCCCCCC;
            }

            vect_s16_to_vect_s32(A, B, len);

            for(unsigned int k = 0; k < MAX_LEN; k++){
                int32_t exp = k < len? ((int32_t)casse->input) << 8 : 0xCCCCCCCC;
                TEST_ASSERT_EQUAL_MSG(exp, A[k], casse->line);
            }

        }
    }
}


TEST(vect_bitdepth_convert, vect_s16_to_vect_s32_random)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();

    
    int32_t DWORD_ALIGNED A[MAX_LEN];
    int16_t DWORD_ALIGNED B[MAX_LEN];

    for(unsigned int v = 0; v < REPS; v++){

        setExtraInfo_R(v);

        const unsigned len = pseudo_rand_uint32(&seed) % MAX_LEN;

        for(unsigned int i = 0; i < len; i++){
            unsigned shr = pseudo_rand_uint32(&seed) % 10;
            B[i] = pseudo_rand_int16(&seed) >> shr;
        }

        memset(A, 0xCC, sizeof(A));
        vect_s16_to_vect_s32(A, B, len);

        for(unsigned int k = 0; k < MAX_LEN; k++){
            int32_t exp = k < len? ((int32_t)B[k]) << 8 : 0xCCCCCCCC;
            TEST_ASSERT_EQUAL_MSG(exp, A[k], v);
        }
    }
}


TEST(vect_bitdepth_convert, vect_s32_to_vect_s16_basic)
{
    

    typedef struct {
        int32_t input;
        int b_shr;
        int16_t expected;
        unsigned line;
    } test_case_t;

    test_case_t casses[] = {
        //       input     shr     expected       line #
        {   0x00000000,      0,      0x0000,    __LINE__},
        {   0x00000001,      0,      0x0001,    __LINE__},
        {   0x00000010,      0,      0x0010,    __LINE__},
        {   0x00000100,      0,      0x0100,    __LINE__},
        {  -0x00000100,      0,     -0x0100,    __LINE__},
        {   0x00008000,      0,      0x7FFF,    __LINE__},
        {  -0x00008000,      0,     -0x7FFF,    __LINE__},
        {   0x00000001,      1,      0x0001,    __LINE__},
        {  -0x00000001,      1,      0x0000,    __LINE__}, //ties round towards positive infty
        {   0x00018000,     16,      0x0002,    __LINE__},
        {  -0x00018000,     16,     -0x0001,    __LINE__},
    };

    const unsigned N_cases = sizeof(casses)/sizeof(test_case_t);

    const unsigned start_case = 0;
    char buff[100];
    for(unsigned int v = start_case; v < N_cases; v++){
        setExtraInfo_R(v);
        
        test_case_t* casse = &casses[v];
        sprintf(buff, "(line %u)", casse->line);

        unsigned lengths[] = {1, 4, 16, 32, 40 };
        int32_t DWORD_ALIGNED B[MAX_LEN];
        int16_t DWORD_ALIGNED A[MAX_LEN];

        for(int i = 0; i < sizeof(lengths)/sizeof(lengths[0]); i++){
            unsigned len = lengths[i];

            for(unsigned int i = 0; i < MAX_LEN; i++){
                B[i] = casse->input;
                A[i] = 0xCCCC;
            }

            vect_s32_to_vect_s16(A, B, len, casse->b_shr);

            for(unsigned int k = 0; k < len; k++)
                TEST_ASSERT_EQUAL_MESSAGE(casse->expected, A[k], buff);
            for(int k = len; k < MAX_LEN; k++)
                TEST_ASSERT_EQUAL_MESSAGE((int16_t)0xCCCC, A[k], buff);

            vect_s32_to_vect_s16((int16_t*)B, B, len, casse->b_shr);

            for(unsigned int k = 0; k < len; k++)
                TEST_ASSERT_EQUAL_MESSAGE(casse->expected, ((int16_t*)B)[k], buff);
        }
    }
}

