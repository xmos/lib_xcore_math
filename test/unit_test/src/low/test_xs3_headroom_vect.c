
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


#define TEST_ASSERT_EQUAL_MSG(EXPECTED, ACTUAL, LINE_NUM)   do{       \
    if((EXPECTED)!=(ACTUAL)) {                                        \
      sprintf(msg_buff, "(test vector @ line %u)", (LINE_NUM));       \
      TEST_ASSERT_EQUAL_MESSAGE((EXPECTED), (ACTUAL), msg_buff);      \
    }} while(0)

#ifndef DEBUG_ON
#define DEBUG_ON    (1)
#endif

static void test_xs3_headroom_vect_s16_case0()
{
    typedef struct {
        int16_t value;
        headroom_t exp_hr;
        unsigned line;
    } test_case_t;

    test_case_t casses[] = {
        //    value          exp_hr          line #
        {        1,              14,         __LINE__},
        {        0,              15,         __LINE__},
        {       -1,              15,         __LINE__},
        {        2,              13,         __LINE__},
        {        3,              13,         __LINE__},
        {       -2,              14,         __LINE__},
        {       -3,              13,         __LINE__},
        {   0x7FFF,               0,         __LINE__},
        {  -0x8000,               0,         __LINE__},
        {  -0x7FFF,               0,         __LINE__},
        {   0x00FF,               7,         __LINE__},
    };

    const unsigned N_cases = sizeof(casses)/sizeof(test_case_t);

    const unsigned start_case = 0;

    for(int v = 0; v < N_cases; v++){
        
        test_case_t* casse = &casses[v];

        int16_t A[1] = { casse->value };

        headroom_t hr = xs3_headroom_vect_s16(A, 1);

        TEST_ASSERT_EQUAL_MSG(casse->exp_hr, hr, casse->line);

    }
}


#define LEN     64
#define REPS        IF_QUICK_TEST(10, 100)
static void test_xs3_headroom_vect_s16_case1()
{
    seed = 362546234;

    for(int v = 0; v < REPS; v++){
        
        //without shr, the headroom would always be zero.
        unsigned shr = pseudo_rand_uint32(&seed) % 8;
        int16_t A[LEN];
        for(int i = 0; i < LEN; i++){
            A[i] = pseudo_rand_int16(&seed) >> shr;
        }

        headroom_t hr = xs3_headroom_vect_s16(A, LEN);
        
        unsigned large_enough = 0;
        for(int i = 0; i < LEN; i++){
            int16_t a1 = A[i];
            int16_t a2 = (a1 << hr) >> hr;

            TEST_ASSERT_EQUAL(a1, a2);

            int16_t a3 = ((a1 << (hr+1)));
            a3 = a3 >> (hr+1);
            if(a3 != a1)
                large_enough = 1;
        }

        TEST_ASSERT(large_enough);

    }
}
#undef LEN
#undef REPS


static void test_xs3_headroom_vect_s32_case0()
{
    typedef struct {
        int32_t value;
        headroom_t exp_hr;
        unsigned line;
    } test_case_t;

    test_case_t casses[] = {
        //        value          exp_hr          line #
        {            1,              30,         __LINE__},
        {            0,              31,         __LINE__},
        {           -1,              31,         __LINE__},
        {            2,              29,         __LINE__},
        {            3,              29,         __LINE__},
        {           -2,              30,         __LINE__},
        {           -3,              29,         __LINE__},
        {   0x7FFFFFFF,               0,         __LINE__},
        {  -0x80000000,               0,         __LINE__},
        {  -0x7FFFFFFF,               0,         __LINE__},
        {   0x00FFFFFF,               7,         __LINE__},
    };

    const unsigned N_cases = sizeof(casses)/sizeof(test_case_t);

    const unsigned start_case = 0;

    for(int v = 0; v < N_cases; v++){
        
        test_case_t* casse = &casses[v];

        int32_t A[1] = { casse->value };

        headroom_t hr = xs3_headroom_vect_s32(A, 1);

        TEST_ASSERT_EQUAL_MSG(casse->exp_hr, hr, casse->line);

    }
}


#define LEN     64
#define REPS        IF_QUICK_TEST(10, 100)
static void test_xs3_headroom_vect_s32_case1()
{
    seed = 362546234;

    for(int v = 0; v < REPS; v++){
        
        //without shr, the headroom would always be zero.
        unsigned shr = pseudo_rand_uint32(&seed) % 8;
        int32_t A[LEN];
        for(int i = 0; i < LEN; i++){
            A[i] = pseudo_rand_int32(&seed) >> shr;
        }

        headroom_t hr = xs3_headroom_vect_s32(A, LEN);
        
        unsigned large_enough = 0;
        for(int i = 0; i < LEN; i++){
            int32_t a1 = A[i];
            int32_t a2 = (a1 << hr) >> hr;

            TEST_ASSERT_EQUAL(a1, a2);

            int32_t a3 = ((a1 << (hr+1)));
            a3 = a3 >> (hr+1);
            if(a3 != a1)
                large_enough = 1;
        }

        TEST_ASSERT(large_enough);

    }
}
#undef LEN
#undef REPS

#define RUN(TEST) do{UnityBegin(__FILE__); RUN_TEST(TEST); retval = UnityEnd(); printf("\n\n");}while(0);


int test_xs3_headroom_vect()
{
    int retval = 0;

    RUN(test_xs3_headroom_vect_s16_case0);
    RUN(test_xs3_headroom_vect_s16_case1);
    
    RUN(test_xs3_headroom_vect_s32_case0);
    RUN(test_xs3_headroom_vect_s32_case1);

    return retval;
}