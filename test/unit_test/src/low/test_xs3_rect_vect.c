
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


#define MAX_LEN     68
#define REPS        IF_QUICK_TEST(10, 100)
static void test_xs3_rect_vect_s16()
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

        hr = xs3_rect_vect_s16(A, B, 2);
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
        hr = xs3_rect_vect_s16(A, B, len);

        for(int i = 0; i < len; i++)
            TEST_ASSERT_EQUAL(B[i] >= 0? B[i] : 0, A[i]);
        for(int i = len; i < MAX_LEN; i++)
            TEST_ASSERT_EQUAL((int16_t)0xCCCC, A[i]);

        memcpy(A, B, sizeof(A));
        hr = xs3_rect_vect_s16(A, A, len);

        for(int i = 0; i < len; i++)
            TEST_ASSERT_EQUAL(B[i] >= 0? B[i] : 0, A[i]);
    }
}
#undef MAX_LEN
#undef REPS


#define MAX_LEN     68
#define REPS        IF_QUICK_TEST(10, 100)
static void test_xs3_rect_vect_s32()
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

        hr = xs3_rect_vect_s32(A, B, 2);
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
        hr = xs3_rect_vect_s32(A, B, len);

        for(int i = 0; i < len; i++)
            TEST_ASSERT_EQUAL(B[i] >= 0? B[i] : 0, A[i]);
        for(int i = len; i < MAX_LEN; i++)
            TEST_ASSERT_EQUAL((int32_t)0xCCCCCCCC, A[i]);

        memcpy(A, B, sizeof(A));
        hr = xs3_rect_vect_s32(A, A, len);

        for(int i = 0; i < len; i++)
            TEST_ASSERT_EQUAL(B[i] >= 0? B[i] : 0, A[i]);
    }
}
#undef MAX_LEN
#undef REPS


void test_xs3_rect_vect()
{
    SET_TEST_FILE();

    RUN_TEST(test_xs3_rect_vect_s16);
    RUN_TEST(test_xs3_rect_vect_s32);
}