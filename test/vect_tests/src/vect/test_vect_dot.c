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


TEST_GROUP_RUNNER(vect_dot) {
  RUN_TEST_CASE(vect_dot, vect_s32_dot_prepare);
  RUN_TEST_CASE(vect_dot, vect_s16_dot);
  RUN_TEST_CASE(vect_dot, vect_s32_dot_basic);
}

TEST_GROUP(vect_dot);
TEST_SETUP(vect_dot) { fflush(stdout); }
TEST_TEAR_DOWN(vect_dot) {}


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


TEST(vect_dot, vect_s32_dot_prepare)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    int32_t B[MAX_LEN], C[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        const unsigned B_length = pseudo_rand_uint(&seed, 1, MAX_LEN);

        const exponent_t B_exp = pseudo_rand_int(&seed, -100, 100);
        const exponent_t C_exp = pseudo_rand_int(&seed, -100, 100);

        const headroom_t B_hr = pseudo_rand_uint(&seed, 0, 30);
        const headroom_t C_hr = pseudo_rand_uint(&seed, 0, 30);

        for(unsigned int i = 0; i < B_length; i++){
            B[i] = INT32_MIN >> B_hr;
            C[i] = INT32_MIN >> C_hr;
        }

        double expected = B_length * (ldexp(B[0], B_exp) * ldexp(C[0], C_exp));

        exponent_t A_exp;
        right_shift_t b_shr, c_shr;

        vect_s32_dot_prepare(&A_exp, &b_shr, &c_shr, B_exp, C_exp, B_hr, C_hr, B_length);

        int64_t result = vect_s32_dot(B, C, B_length, b_shr, c_shr);

        double got = ldexp((double) result, A_exp);

        TEST_ASSERT( fabs((expected-got)/expected) < ldexp(1, -25) );
    }
}
#undef MAX_LEN
#undef REPS



#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (64)
#else
#  define REPS       (1000)
#  define MAX_LEN    (256)
#endif

TEST(vect_dot, vect_s16_dot)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    int16_t WORD_ALIGNED B[MAX_LEN];
    int16_t WORD_ALIGNED C[MAX_LEN];
    

    for(unsigned int v = 0; v < REPS; v++){

        setExtraInfo_RS(v, seed);

        const unsigned len = pseudo_rand_uint(&seed, 1, MAX_LEN+1);


        headroom_t B_hr = pseudo_rand_uint(&seed, 0, 15);
        headroom_t C_hr = pseudo_rand_uint(&seed, 0, 15);

        int64_t expected = 0;
        
        for(unsigned int i = 0; i < len; i++){
            B[i] = pseudo_rand_int16(&seed) >> B_hr;
            C[i] = pseudo_rand_int16(&seed) >> C_hr;

            expected += ((int32_t)B[i]) * C[i];
        }

        int64_t result = vect_s16_dot(B, C, len);

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

TEST(vect_dot, vect_s32_dot_basic)
{
    

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
    for(unsigned int v = start_case; v < N_cases; v++){
        setExtraInfo_R(v);
        
        test_case_t* casse = &casses[v];

        unsigned len = casse->len;

        TEST_ASSERT(len <= MAX_LEN);

        int32_t WORD_ALIGNED B[MAX_LEN];
        int32_t WORD_ALIGNED C[MAX_LEN];
        int64_t result;

        for(unsigned int i = 0; i < len; i++){
            B[i] = casse->input.b;
            C[i] = casse->input.c;
        }

        result = vect_s32_dot(B, C, len, casse->shr.b, casse->shr.c);

        TEST_ASSERT_EQUAL_MSG(casse->expected, result, casse->line);
    }
}
#undef MAX_LEN

