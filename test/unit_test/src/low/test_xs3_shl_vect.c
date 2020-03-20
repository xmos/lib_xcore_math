
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


#if !defined(DEBUG_ON) || (0)
#undef DEBUG_ON
#define DEBUG_ON    (1)
#endif


static void test_xs3_shl_vect_s16_basic()
{
    PRINTF("%s...\n", __func__);

    typedef struct {
        int16_t value;
        int shl;
        int16_t expected;
        unsigned line;
    } test_case_t;

    test_case_t casses[] = {
        //    value     shl     expected    line #
        {        1,      0,           1,    __LINE__},
        {        0,      0,           0,    __LINE__},
        {       -1,      0,          -1,    __LINE__},
        {        2,      0,           2,    __LINE__},
        {        3,      0,           3,    __LINE__},
        {       -2,      0,          -2,    __LINE__},
        {       -3,      0,          -3,    __LINE__},
        {   0x00FF,      0,      0x00FF,    __LINE__},
        {        1,      1,           2,    __LINE__},
        {        1,      2,           4,    __LINE__},
        {        1,      3,           8,    __LINE__},
        {        1,     -1,           0,    __LINE__},
        {        1,     -2,           0,    __LINE__},
        {   0x07F0,      1,      0x0FE0,    __LINE__},
        {   0x07F0,     -1,      0x03F8,    __LINE__},
        {   0x07F0,      2,      0x1FC0,    __LINE__},
        {  -0x0008,      1,     -0x0010,    __LINE__},
        {  -0x0008,     -1,     -0x0004,    __LINE__},
        {  -0x0008,     -2,     -0x0002,    __LINE__},
        {  -0x0008,     -3,     -0x0001,    __LINE__},
        {  -0x0008,     -4,     -0x0001,    __LINE__},
        {   0x1000,      3,      0x7FFF,    __LINE__},
        {  -0x1000,      3,     -0x7FFF,    __LINE__},
        {  -0x8000,      0,     -0x7FFF,    __LINE__},
    };

    const unsigned N_cases = sizeof(casses)/sizeof(test_case_t);

    const unsigned start_case = 0;

    for(int v = 0; v < N_cases; v++){
        PRINTF("\ttest vector %d..\n", v);
        
        test_case_t* casse = &casses[v];

        int16_t B[1] = { casse->value };
        int16_t A[1] = { 0xCC };

        headroom_t hr = xs3_shl_vect_s16(A, B, 1, casse->shl);

        TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
        TEST_ASSERT_EQUAL_MSG(xs3_headroom_vect_s16(A, 1), hr, casse->line);

        hr = xs3_shl_vect_s16(B, B, 1, casse->shl);

        TEST_ASSERT_EQUAL_MSG(casse->expected, B[0], casse->line);
        TEST_ASSERT_EQUAL_MSG(xs3_headroom_vect_s16(B, 1), hr, casse->line);
    }
}


#define LEN     68
#define REPS        IF_QUICK_TEST(10, 100)
static void test_xs3_shl_vect_s16_random()
{
    PRINTF("%s...\n", __func__);
    seed = 362546234;

    for(int v = 0; v < REPS; v++){

        PRINTF("\trepetition %d..\n", v);
        
        int16_t A[LEN];
        int16_t B[LEN];

        for(int i = 0; i < LEN; i++){
            //without shr, the headroom would always be zero.
            unsigned shr = pseudo_rand_uint32(&seed) % 8;
            B[i] = pseudo_rand_int16(&seed) >> shr;
        }

        // shift left between -3 and 3 bits
        int shl = (pseudo_rand_int32(&seed) % 7) - 3;

        for(int in_place = 0; in_place < 2; in_place++){

            PRINTF("\t\t%s..\n", in_place? "in-place" : "not in-place");

            if(in_place)
                memcpy(A, B, sizeof(A));

            headroom_t hr = xs3_shl_vect_s16(A, (in_place)? A : B, LEN, shl);

            for(int i = 0; i < LEN; i++){
                int32_t b = B[i];
                int32_t exp;

                if(shl >= 0){
                    exp = b << shl;
                    exp = (exp > VPU_INT16_MAX)? VPU_INT16_MAX : exp;
                    exp = (exp < VPU_INT16_MIN)? VPU_INT16_MIN : exp;
                } else {
                    exp = b >> (-shl);
                    exp = (b >= 0)? exp : (exp == 0)? -1 : exp;
                }
                
                if(((int16_t)exp) != A[i]){
                    sprintf(msg_buff, "rep(%d)[%d]: %d <-- (%d << %d)     (A[i]=0x%04X; B[i]=0x%04X)\n",v, i, A[i], B[i], shl,  (uint16_t)A[i],  (uint16_t)B[i]);
                }
                TEST_ASSERT_EQUAL_MESSAGE((int16_t)exp, A[i], msg_buff);
            }

            TEST_ASSERT_EQUAL(xs3_headroom_vect_s16(A, LEN), hr);
        }
        
    }
}
#undef LEN
#undef REPS




static void test_xs3_shl_vect_s32_basic()
{
    PRINTF("%s...\n", __func__);

    typedef struct {
        int32_t value;
        int shl;
        int32_t expected;
        unsigned line;
    } test_case_t;

    test_case_t casses[] = {
        //    value     shl     expected    line #
        {            1,      0,               1,    __LINE__},
        {            0,      0,               0,    __LINE__},
        {           -1,      0,              -1,    __LINE__},
        {            2,      0,               2,    __LINE__},
        {            3,      0,               3,    __LINE__},
        {           -2,      0,              -2,    __LINE__},
        {           -3,      0,              -3,    __LINE__},
        {   0x00FF0000,      0,      0x00FF0000,    __LINE__},
        {            1,      1,               2,    __LINE__},
        {            1,      2,               4,    __LINE__},
        {            1,      3,               8,    __LINE__},
        {            1,     -1,               0,    __LINE__},
        {            1,     -2,               0,    __LINE__},
        {   0x07F00000,      1,      0x0FE00000,    __LINE__},
        {   0x07F00000,     -1,      0x03F80000,    __LINE__},
        {   0x07F00000,      2,      0x1FC00000,    __LINE__},
        {  -0x00080000,      1,     -0x00100000,    __LINE__},
        {  -0x00080000,     -1,     -0x00040000,    __LINE__},
        {  -0x00080000,     -2,     -0x00020000,    __LINE__},
        {  -0x00080000,     -3,     -0x00010000,    __LINE__},
        {  -0x00080000,     -4,     -0x00008000,    __LINE__},
        {   0x10000000,      3,      0x7FFFFFFF,    __LINE__},
        {  -0x10000000,      3,     -0x7FFFFFFF,    __LINE__},
        {  -0x80000000,      0,     -0x7FFFFFFF,    __LINE__},
    };

    const unsigned N_cases = sizeof(casses)/sizeof(test_case_t);

    const unsigned start_case = 0;

    for(int v = start_case; v < N_cases; v++){
        PRINTF("\ttest vector %d..\n", v);
        
        test_case_t* casse = &casses[v];

        int32_t B[1] = { casse->value };
        int32_t A[1] = { 0xCC };

        headroom_t hr = xs3_shl_vect_s32(A, B, 1, casse->shl);

        TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
        TEST_ASSERT_EQUAL_MSG(xs3_headroom_vect_s32(A, 1), hr, casse->line);

        hr = xs3_shl_vect_s32(B, B, 1, casse->shl);

        TEST_ASSERT_EQUAL_MSG(casse->expected, B[0], casse->line);
        TEST_ASSERT_EQUAL_MSG(xs3_headroom_vect_s32(B, 1), hr, casse->line);
    }
}





#define LEN     68
#define REPS        IF_QUICK_TEST(10, 100)
static void test_xs3_shl_vect_s32_random()
{
    PRINTF("%s...\n", __func__);
    seed = 362546234;

    for(int v = 0; v < REPS; v++){

        PRINTF("\trepetition %d..\n", v);
        
        int32_t A[LEN];
        int32_t B[LEN];

        for(int i = 0; i < LEN; i++){
            //without shr, the headroom would always be zero.
            unsigned shr = pseudo_rand_uint32(&seed) % 8;
            B[i] = pseudo_rand_int32(&seed) >> shr;
        }

        // shift left between -3 and 3 bits
        int shl = (pseudo_rand_int32(&seed) % 7) - 3;

        for(int in_place = 0; in_place < 2; in_place++){

            PRINTF("\t\t%s..\n", in_place? "in-place" : "not in-place");

            if(in_place)
                memcpy(A, B, sizeof(A));

            headroom_t hr = xs3_shl_vect_s32(A, (in_place)? A : B, LEN, shl);

            for(int i = 0; i < LEN; i++){
                int64_t b = B[i];
                int64_t exp;

                if(shl >= 0){
                    exp = b << shl;
                    exp = (exp > VPU_INT32_MAX)? VPU_INT32_MAX : exp;
                    exp = (exp < VPU_INT32_MIN)? VPU_INT32_MIN : exp;
                } else {
                    exp = b >> (-shl);
                    exp = (b >= 0)? exp : (exp == 0)? -1 : exp;
                }
                
                if(((int32_t)exp) != A[i]){
                    sprintf(msg_buff, "rep(%d)[%d]: %ld <-- (%ld << %d)     (A[i]=0x%04X; B[i]=0x%04X)\n",v, i, A[i], B[i], shl,  (unsigned)A[i],  (unsigned)B[i]);
                }
                TEST_ASSERT_EQUAL_MESSAGE((int32_t)exp, A[i], msg_buff);
            }

            TEST_ASSERT_EQUAL(xs3_headroom_vect_s32(A, LEN), hr);
        }
    }
}
#undef LEN
#undef REPS


void test_xs3_shl_vect()
{
    SET_TEST_FILE();

    RUN_TEST(test_xs3_shl_vect_s16_basic);
    RUN_TEST(test_xs3_shl_vect_s16_random);
    
    RUN_TEST(test_xs3_shl_vect_s32_basic);
    RUN_TEST(test_xs3_shl_vect_s32_random);
}