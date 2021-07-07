// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "xs3_math.h"

#include "../../tst_common.h"

#include "unity_fixture.h"


TEST_GROUP_RUNNER(xs3_vect_abs_sum) {
  RUN_TEST_CASE(xs3_vect_abs_sum, xs3_vect_s16_abs_sum_basic);
  RUN_TEST_CASE(xs3_vect_abs_sum, xs3_vect_s16_abs_sum_random);
  RUN_TEST_CASE(xs3_vect_abs_sum, xs3_vect_s32_abs_sum_basic);
  RUN_TEST_CASE(xs3_vect_abs_sum, xs3_vect_s32_abs_sum_random);
}

TEST_GROUP(xs3_vect_abs_sum);
TEST_SETUP(xs3_vect_abs_sum) {}
TEST_TEAR_DOWN(xs3_vect_abs_sum) {}


static char msg_buff[200];


#define TEST_ASSERT_EQUAL_MSG(EXPECTED, ACTUAL, LINE_NUM)   do{       \
    if((EXPECTED)!=(ACTUAL)) {                                        \
      sprintf(msg_buff, "(test vector @ line %u)", (LINE_NUM));       \
      TEST_ASSERT_EQUAL_MESSAGE((EXPECTED), (ACTUAL), msg_buff);      \
    }} while(0)


TEST(xs3_vect_abs_sum, xs3_vect_s16_abs_sum_basic)
{
    

    typedef struct {
        int16_t b;
        unsigned line;
    } test_case_t;

    test_case_t casses[] = {
        //      b     line num
        {  0x0000,    __LINE__},
        {  0x0001,    __LINE__},
        { -0x0001,    __LINE__},
        {  0x0002,    __LINE__},
        {  0x7FFF,    __LINE__},
        { -0x8000,    __LINE__},
    };

    const unsigned N_cases = sizeof(casses)/sizeof(test_case_t);

    const unsigned start_case = 0;

    for(int v = start_case; v < N_cases; v++){
        setExtraInfo_R(v);
        
        test_case_t* casse = &casses[v];

        int16_t WORD_ALIGNED B[40];
        int32_t result;

        unsigned lengths[] = {1, 4, 16, 32, 40 };

        for( int l = 0; l < sizeof(lengths)/sizeof(lengths[0]); l++){
            unsigned len = lengths[l];

            for(int i = 0; i < len; i++){
                B[i] = casse->b;
            }

            result = xs3_vect_s16_abs_sum(B, len);

            int16_t t = casse->b;
            t = (t>=0)? t : -t;
            t = (t>=0)? t : 0x7FFF; // because -1*(-0x8000) = -0x8000
            int32_t exp = t * len;

            TEST_ASSERT_EQUAL_MSG(exp, result, casse->line);
        }
    }
}


#define MAX_LEN     200
#define REPS        (100)

TEST(xs3_vect_abs_sum, xs3_vect_s16_abs_sum_random)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    int32_t result;
    int16_t B[MAX_LEN];

    for(int v = 0; v < REPS; v++){

        unsigned len = (pseudo_rand_uint32(&seed) % MAX_LEN) + 1;
        setExtraInfo_RSL(v, seed, len);

        
        for(int i = 0; i < len; i++){
            B[i] = pseudo_rand_int16(&seed);
        }

        result = xs3_vect_s16_abs_sum(B, len);

        int32_t exp = 0;
        for(int i = 0; i < len; i++){
            int32_t b = B[i];
            b = (b>=0)? b : -b;
            exp += b;
        }

        TEST_ASSERT_EQUAL(exp, result);
        
    }
}
#undef MAX_LEN
#undef REPS


TEST(xs3_vect_abs_sum, xs3_vect_s32_abs_sum_basic)
{
    

    typedef struct {
        int32_t b;
        unsigned line;
    } test_case_t;

    test_case_t casses[] = {
        //      b     line num
        {  0x00000000,    __LINE__},
        {  0x00000001,    __LINE__},
        { -0x00000001,    __LINE__},
        {  0x00020000,    __LINE__},
        {  0x7FFFFFFF,    __LINE__},
        { -0x80000000,    __LINE__},
    };

    const unsigned N_cases = sizeof(casses)/sizeof(test_case_t);

    const unsigned start_case = 0;

    for(int v = start_case; v < N_cases; v++){
        setExtraInfo_R(v);
        
        test_case_t* casse = &casses[v];

        int32_t B[40];
        int64_t result;

        unsigned lengths[] = {1, 4, 16, 32, 40 };

        for( int l = 0; l < sizeof(lengths)/sizeof(lengths[0]); l++){
            unsigned len = lengths[l];

            for(int i = 0; i < len; i++){
                B[i] = casse->b;
            }

            result = xs3_vect_s32_abs_sum(B, len);

            int64_t t = casse->b;
            t = (t>=0)? t : -t;
            int64_t exp = t * len;

            if(exp != result){
                printf("Expected %lld was %lld\n", exp, result);
            }

            TEST_ASSERT(exp == result);
        }
    }
}


#define MAX_LEN     200
#define REPS        (100)

TEST(xs3_vect_abs_sum, xs3_vect_s32_abs_sum_random)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    int64_t result;
    int32_t B[MAX_LEN];

    for(int v = 0; v < REPS; v++){

        unsigned len = (pseudo_rand_uint32(&seed) % MAX_LEN) + 1;
        // len = 9;

        setExtraInfo_RSL(v, seed, len);

        for(int i = 0; i < len; i++){
            B[i] = pseudo_rand_int32(&seed);
        }

        result = xs3_vect_s32_abs_sum(B, len);

        int32_t exp = 0;
        for(int i = 0; i < len; i++){
            int64_t b = B[i];
            b = (b>=0)? b : -b;
            exp += b;
        }

        TEST_ASSERT_EQUAL(exp, result);
        
    }
}
#undef MAX_LEN
#undef REPS

