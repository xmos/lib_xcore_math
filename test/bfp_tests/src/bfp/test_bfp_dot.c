// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "bfp_math.h"
#include "../src/vect/vpu_helper.h"

#include "../tst_common.h"

#include "unity_fixture.h"


TEST_GROUP_RUNNER(bfp_dot) {
  RUN_TEST_CASE(bfp_dot, bfp_s16_dot);
  RUN_TEST_CASE(bfp_dot, bfp_s32_dot);
  RUN_TEST_CASE(bfp_dot, bfp_s32_dot_2);
}

TEST_GROUP(bfp_dot);
TEST_SETUP(bfp_dot) {}
TEST_TEAR_DOWN(bfp_dot) {}

#if DEBUG_ON || 0
#undef DEBUG_ON
#define DEBUG_ON    (1)
#endif


#define REPS        (1000)
#define MAX_LEN     1024

static char msg_buff[200];

#define TEST_ASSERT_EQUAL_MSG(EXPECTED, ACTUAL, EXTRA, LINE_NUM)   do{          \
    if((EXPECTED)!=(ACTUAL)) {                                                  \
      sprintf(msg_buff, "%s (line %u)", (EXTRA), (LINE_NUM));                   \
      TEST_ASSERT_EQUAL_MESSAGE((EXPECTED), (ACTUAL), msg_buff);                \
    }} while(0)


TEST(bfp_dot, bfp_s16_dot)
{
    PRINTF("%s...\t(random vectors)\n", __func__);

    unsigned seed = 0x7C385C55;

    int16_t dataB[MAX_LEN];
    int16_t dataC[MAX_LEN];
    bfp_s16_t B, C;

    B.data = dataB;
    C.data = dataC;

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        bfp_s16_init(&B, dataB, pseudo_rand_int(&seed, -100, 100),
                            pseudo_rand_uint(&seed, 1, MAX_LEN+1), 0);

        bfp_s16_init(&C, dataC, pseudo_rand_int(&seed, -100, 100), B.length, 0);

        B.hr = pseudo_rand_uint(&seed, 1, 12);
        C.hr = pseudo_rand_uint(&seed, 1, 12);

        double expected = 0;

        for(int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_uint(&seed, 0, INT16_MAX) >> B.hr;
            C.data[i] = pseudo_rand_uint(&seed, 0, INT16_MAX) >> C.hr;

            expected += ldexp(B.data[i], B.exp) * ldexp(C.data[i], C.exp);
        }

        bfp_s16_headroom(&B);
        bfp_s16_headroom(&C);
        
        float_s64_t result = bfp_s16_dot(&B, &C);

        double diff = expected-ldexp(result.mant, result.exp);
        double error = fabs(diff/expected);

        double max_error = ldexp(1, ceil_log2(B.length) - 8 - (B.hr+C.hr));

        TEST_ASSERT( error < max_error );
    }
}





TEST(bfp_dot, bfp_s32_dot)
{
    PRINTF("%s...\t(random vectors)\n", __func__);


    int32_t dataB[MAX_LEN];
    int32_t dataC[MAX_LEN];
    bfp_s32_t B, C;

    B.data = dataB;
    C.data = dataC;
    
    typedef struct {

        struct {    int32_t value;  exponent_t exp;     } b;
        struct {    int32_t value;  exponent_t exp;     } c;
        struct {    int64_t value;  exponent_t exp;     } expected;

        unsigned length;
        unsigned line;
    } test_case_t;

    test_case_t casses[] = {
        //    b{       value,  exp },      c{      value,  exp },   expected{       value,  exp },  length,  line
        {      {  0x00000000,    0 },       { 0x00000000,    0 },           {  0x00000000,    0 },       1,  __LINE__ },
        {      {  0x00000000,    0 },       { 0x00000000,    0 },           {  0x00000000,    0 },      16,  __LINE__ },
        {      {  0x00000000,    0 },       { 0x00000000,    0 },           {  0x00000000,    0 },      40,  __LINE__ },

        {      {  0x00000001,    0 },       { 0x00000001,    0 },           {  0x00000001,    0 },       1,  __LINE__ },
        {      {  0x00000001,    0 },       { 0x00000001,    0 },           {  0x00000010,    0 },      16,  __LINE__ },
        {      {  0x00000001,    0 },       { 0x00000001,    0 },           {  0x00000028,    0 },      40,  __LINE__ },
        
        {      {  0x00000002,    0 },       { 0x00000001,    0 },           {  0x00000002,    0 },       1,  __LINE__ },
        {      {  0x00000002,    0 },       { 0x00000001,    0 },           {  0x00000020,    0 },      16,  __LINE__ },
        {      {  0x00000002,    0 },       { 0x00000001,    0 },           {  0x00000050,    0 },      40,  __LINE__ },

        {      {  0x00000002,    0 },       { 0x00000003,    0 },           {  0x00000006,    0 },       1,  __LINE__ },
        {      {  0x00000002,    0 },       { 0x00000003,    0 },           {  0x00000060,    0 },      16,  __LINE__ },
        {      {  0x00000002,    0 },       { 0x00000003,    0 },           {  0x00000600,    0 },     256,  __LINE__ },
        
        {      {  0x00000001,    1 },       { 0x00000001,    2 },           {  0x00000001,    3 },       1,  __LINE__ },
        {      {  0x00000001,    2 },       { 0x00000001,    1 },           {  0x00000010,    3 },      16,  __LINE__ },
        {      {  0x00000001,    1 },       { 0x00000001,    2 },           {  0x00000028,    3 },      40,  __LINE__ },
        
        {      {  0x01000000,  -20 },       { 0x000F0000,  -15 },           {  0x0000001e,    4 },       1,  __LINE__ },
        {      {  0x01000000,  -20 },       { 0x000F0000,  -15 },           {  0x0000001e,    8 },      16,  __LINE__ },
        {      {  0x01000000,  -20 },       { 0x000F0000,  -15 },           {  0x00005dc0,    0 },      50,  __LINE__ },
    };


    const unsigned N_cases = sizeof(casses)/sizeof(test_case_t);
    const unsigned start_case = 0;

    for(int v = start_case; v < N_cases; v++){
        PRINTF("\ttest vector %d..\n", v);

        test_case_t* casse = &casses[v];

        C.length = B.length = casse->length;
        B.exp = casse->b.exp;
        C.exp = casse->c.exp;

        for(int k = 0; k < casse->length; k++){
            B.data[k] = casse->b.value;
            C.data[k] = casse->c.value;
        }

        B.hr = HR_S32(casse->b.value);
        C.hr = HR_S32(casse->c.value);

        float_s64_t result = bfp_s32_dot(&B, &C);

        right_shift_t shr = result.exp - casse->expected.exp;

        int64_t exp_mod = (shr >= 0)? (casse->expected.value >> shr) : (casse->expected.value << -shr);

        TEST_ASSERT(exp_mod == result.mant);
    } 
}

TEST(bfp_dot, bfp_s32_dot_2)
{
    PRINTF("%s...\t(random vectors)\n", __func__);

    unsigned seed = 34563;

    int32_t dataB[MAX_LEN];
    int32_t dataC[MAX_LEN];
    bfp_s32_t B, C;

    B.data = dataB;
    C.data = dataC;

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        bfp_s32_init(&B, dataB, pseudo_rand_int(&seed, -100, 100),
                            pseudo_rand_uint(&seed, 1, MAX_LEN+1), 0);

        bfp_s32_init(&C, dataC, pseudo_rand_int(&seed, -100, 100), B.length, 0);

        B.hr = pseudo_rand_uint(&seed, 0, 28);
        C.hr = pseudo_rand_uint(&seed, 0, 28);

        double expected = 0;

        for(int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int32(&seed) >> B.hr;
            C.data[i] = pseudo_rand_int32(&seed) >> C.hr;

            expected += ldexp(B.data[i], B.exp) * ldexp(C.data[i], C.exp);
        }

        bfp_s32_headroom(&B);
        bfp_s32_headroom(&C);
        
        float_s64_t result = bfp_s32_dot(&B, &C);

        double diff = expected - ldexp(result.mant, result.exp);
        double error = fabs(diff/expected);
        TEST_ASSERT( error < ldexp(1,-20) );
    }
}

