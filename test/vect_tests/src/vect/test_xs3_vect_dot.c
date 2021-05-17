// Copyright 2020 XMOS LIMITED. This Software is subject to the terms of the 
// XMOS Public License: Version 1

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

#if DEBUG_ON || 0
#undef DEBUG_ON
#define DEBUG_ON    (1)
#endif




#define REPS        1000
#define MAX_LEN     1024



static void test_xs3_vect_s32_dot_prepare()
{
    PRINTF("%s...\n", __func__);
    seed = 0xEF70DEF9;

    int32_t B[MAX_LEN], C[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        const unsigned B_length = pseudo_rand_uint(&seed, 1, MAX_LEN);

        const exponent_t B_exp = pseudo_rand_int(&seed, -100, 100);
        const exponent_t C_exp = pseudo_rand_int(&seed, -100, 100);

        const headroom_t B_hr = pseudo_rand_uint(&seed, 0, 30);
        const headroom_t C_hr = pseudo_rand_uint(&seed, 0, 30);

        for(int i = 0; i < B_length; i++){
            B[i] = INT32_MIN >> B_hr;
            C[i] = INT32_MIN >> C_hr;
        }

        double expected = B_length * (ldexp(B[0], B_exp) * ldexp(C[0], C_exp));

        exponent_t A_exp;
        right_shift_t b_shr, c_shr;

        xs3_vect_s32_dot_prepare(&A_exp, &b_shr, &c_shr, B_exp, C_exp, B_hr, C_hr, B_length);

        int64_t result = xs3_vect_s32_dot(B, C, B_length, b_shr, c_shr);

        // PRINTF("\t        B_length  = %u\n", B_length);
        // PRINTF("\t        B_exp = %d\n", B_exp);
        // PRINTF("\t        C_exp = %d\n", C_exp);
        // PRINTF("\t        B_hr = %d     (-2^%d)\n", B_hr, 31-B_hr);
        // PRINTF("\t        C_hr = %d     (-2^%d)\n", C_hr, 31-C_hr);
        // PRINTF("\t        b_shr = %d\n", b_shr);
        // PRINTF("\t        c_shr = %d\n", c_shr);
        // PRINTF("\t            B[0] = %ld   (0x%08lX)\n", B[0], (uint32_t)B[0]);
        // PRINTF("\t            C[0] = %ld   (0x%08lX)\n", C[0], (uint32_t)C[0]);
        // PRINTF("\t        A_exp = %d\n", A_exp);
        // PRINTF("\t            result = %lld    (0x%08llX)     (%e)\n", result, (uint64_t) result, ldexp(result, A_exp));
        // PRINTF("\t            expected = %e\n", expected);


        double got = ldexp(result, A_exp);

        TEST_ASSERT( fabs((expected-got)/expected) < ldexp(1, -25) );
    }
}
#undef MAX_LEN
#undef REPS




#define MAX_LEN     4096
#define REPS        1000
static void test_xs3_vect_s16_dot()
{
    PRINTF("%s...\n", __func__);
    seed = 0x92B7BD9A;

    int16_t WORD_ALIGNED B[MAX_LEN];
    int16_t WORD_ALIGNED C[MAX_LEN];
    

    for(int v = 0; v < REPS; v++){

        PRINTF("\trepetition %d.. (seed: 0x%08X)\n", v, seed);

        const unsigned len = pseudo_rand_uint(&seed, 1, MAX_LEN+1);


        headroom_t B_hr = pseudo_rand_uint(&seed, 0, 15);
        headroom_t C_hr = pseudo_rand_uint(&seed, 0, 15);

        int64_t expected = 0;
        
        for(int i = 0; i < len; i++){
            B[i] = pseudo_rand_int16(&seed) >> B_hr;
            C[i] = pseudo_rand_int16(&seed) >> C_hr;

            expected += ((int32_t)B[i]) * C[i];
        }

        int64_t result = xs3_vect_s16_dot(B, C, len);

        // printf("============\n");
        // printf("Length: %u\n", len);
        // printf("Expected: %lld     (%012llX)\n", expected, (uint64_t) expected);
        // printf("Got:      %lld     (%012llX)\n", result,   (uint64_t) result);
        // printf("============\n");

        TEST_ASSERT_EQUAL(expected, result);
        
    }
}
#undef MAX_LEN
#undef REPS



#define MAX_LEN     40
static void test_xs3_vect_s32_dot_basic()
{
    PRINTF("%s...\n", __func__);

    typedef struct {
        struct{ int32_t b;  int32_t c;  } input;
        struct{ int b;      int c;      } shr;
        unsigned len;

        int64_t expected;

        unsigned line;
    } test_case_t;

    test_case_t casses[] = {
        //  input{           b,           c },  shr{   b,    c },   len,        expected,       line
        {        {  0x00000000,  0x00000000 },     {   0,    0 },     1,      0x00000000,     __LINE__ },
        {        {  0x00000000,  0x00000000 },     {   0,    0 },     2,      0x00000000,     __LINE__ },
        {        {  0x00000000,  0x00000000 },     {   0,    0 },    16,      0x00000000,     __LINE__ },
        {        {  0x00000000,  0x00000000 },     {   0,    0 },    32,      0x00000000,     __LINE__ },
        {        {  0x00000000,  0x00000000 },     {   0,    0 },    40,      0x00000000,     __LINE__ },
        {        {  0x00010000,  0x00000000 },     {   0,    0 },     1,      0x00000000,     __LINE__ },
        {        {  0x00000000,  0x00100000 },     {   0,    0 },     2,      0x00000000,     __LINE__ },
        {        {  0x22000000,  0x00000000 },     {   0,    0 },    16,      0x00000000,     __LINE__ },
        {        {  0x40000000,  0x00010000 },     {   0,    0 },     1,      0x00010000,     __LINE__ },
        {        {  0x40000000,  0x00010000 },     {   0,    0 },     2,      0x00020000,     __LINE__ },
        {        {  0x40000000,  0x00010000 },     {   0,    0 },     8,      0x00080000,     __LINE__ },
        {        {  0x40000000,  0x00010000 },     {   0,    0 },    16,      0x00100000,     __LINE__ },
        {        {  0x40000000,  0x00010000 },     {   0,    0 },    32,      0x00200000,     __LINE__ },
        {        {  0x40000000,  0x00010000 },     {   0,    0 },    40,      0x00280000,     __LINE__ },
        {        {  0x04000000,  0x01000000 },     {   0,    0 },     1,      0x00100000,     __LINE__ },
        {        {  0x04000000,  0x00100000 },     {   0,    0 },    25,      0x00190000,     __LINE__ },
        {        {  0x04000000,  0x00100000 },     {   0,    0 },    25,      0x00190000,     __LINE__ },
        {        {  0x04000000,  0x00100000 },     {   4,    4 },    25,      0x00001900,     __LINE__ },
        {        {  0x04000000,  0x00100000 },     {   2,    2 },    25,      0x00019000,     __LINE__ },
        {        {  0x04000000,  0x00100000 },     {  -1,    0 },    25,      0x00320000,     __LINE__ },
        {        {  0x04000000,  0x00100000 },     {  -1,   -1 },    25,      0x00640000,     __LINE__ },
    };

    const unsigned start_case = 0;

    const unsigned N_cases = sizeof(casses)/sizeof(test_case_t);
    for(int v = start_case; v < N_cases; v++){
        PRINTF("\ttest vector %d..\n", v);
        
        test_case_t* casse = &casses[v];

        unsigned len = casse->len;

        TEST_ASSERT(len <= MAX_LEN);

        int32_t WORD_ALIGNED B[MAX_LEN];
        int32_t WORD_ALIGNED C[MAX_LEN];
        int64_t result;

        for(int i = 0; i < len; i++){
            B[i] = casse->input.b;
            C[i] = casse->input.c;
        }

        result = xs3_vect_s32_dot(B, C, len, casse->shr.b, casse->shr.c);

        TEST_ASSERT_EQUAL_MSG(casse->expected, result, casse->line);
    }
}
#undef MAX_LEN




void test_xs3_dot()
{
    SET_TEST_FILE();
    RUN_TEST(test_xs3_vect_s32_dot_prepare);

    RUN_TEST(test_xs3_vect_s16_dot);
    RUN_TEST(test_xs3_vect_s32_dot_basic);
}
