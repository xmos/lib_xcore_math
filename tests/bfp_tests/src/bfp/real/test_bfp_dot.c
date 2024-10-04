// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xmath/xmath.h"
#include "../src/vect/vpu_helper.h"

#include "../../tst_common.h"

#include "unity_fixture.h"


TEST_GROUP_RUNNER(bfp_dot) {
  RUN_TEST_CASE(bfp_dot, bfp_s16_dot);
  RUN_TEST_CASE(bfp_dot, bfp_s32_dot);
  RUN_TEST_CASE(bfp_dot, bfp_s32_dot_2);
}

TEST_GROUP(bfp_dot);
TEST_SETUP(bfp_dot) { fflush(stdout); }
TEST_TEAR_DOWN(bfp_dot) {}

#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (128)
#else
#  define REPS       (1000)
#  define MAX_LEN    (512)
#endif


TEST(bfp_dot, bfp_s16_dot)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    int16_t dataB[MAX_LEN];
    int16_t dataC[MAX_LEN];
    bfp_s16_t B, C;

    B.data = dataB;
    C.data = dataC;

    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        bfp_s16_init(&B, dataB, pseudo_rand_int(&seed, -100, 100),
                            pseudo_rand_uint(&seed, 1, MAX_LEN+1), 0);

        bfp_s16_init(&C, dataC, pseudo_rand_int(&seed, -100, 100), B.length, 0);

        B.hr = pseudo_rand_uint(&seed, 1, 12);
        C.hr = pseudo_rand_uint(&seed, 1, 12);

        double expected = 0;

        for(unsigned int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_uint(&seed, 0, INT16_MAX) >> B.hr;
            C.data[i] = pseudo_rand_uint(&seed, 0, INT16_MAX) >> C.hr;

            expected += ldexp(B.data[i], B.exp) * ldexp(C.data[i], C.exp);
        }

        bfp_s16_headroom(&B);
        bfp_s16_headroom(&C);
        
        float_s64_t result = bfp_s16_dot(&B, &C);

        double diff = expected-ldexp((double) result.mant, result.exp);
        double error = fabs(diff/expected);

        double max_error = ldexp(1, u32_ceil_log2(B.length) - 8 - (B.hr+C.hr));

        TEST_ASSERT( error < max_error );
    }
}


TEST(bfp_dot, bfp_s32_dot)
{
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

    for(unsigned int v = start_case; v < N_cases; v++){
        setExtraInfo_R(v);

        test_case_t* casse = &casses[v];
        
        if(casse->length > MAX_LEN) continue;

        C.length = B.length = casse->length;
        B.exp = casse->b.exp;
        C.exp = casse->c.exp;

        for(unsigned int k = 0; k < casse->length; k++){
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
    unsigned seed = SEED_FROM_FUNC_NAME();

    int32_t dataB[MAX_LEN];
    int32_t dataC[MAX_LEN];
    bfp_s32_t B, C;

    B.data = dataB;
    C.data = dataC;

    for(int r = 0; r < REPS; r++){
        unsigned old_seed = seed;

        bfp_s32_init(&B, dataB, pseudo_rand_int(&seed, -100, 100),
                            pseudo_rand_uint(&seed, 1, MAX_LEN+1), 0);

        bfp_s32_init(&C, dataC, pseudo_rand_int(&seed, -100, 100), B.length, 0);

        B.hr = pseudo_rand_uint(&seed, 0, 28);
        C.hr = pseudo_rand_uint(&seed, 0, 28);
        
        setExtraInfo_RSL(r, old_seed, B.length);

        double expected = 0;

        for(unsigned int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int32(&seed) >> B.hr;
            C.data[i] = pseudo_rand_int32(&seed) >> C.hr;

            expected += ldexp(B.data[i], B.exp) * ldexp(C.data[i], C.exp);
        }

        bfp_s32_headroom(&B);
        bfp_s32_headroom(&C);
        
        float_s64_t result = bfp_s32_dot(&B, &C);

        double diff = expected - ldexp((double) result.mant, result.exp);
        double error = fabs(diff/expected);
        int extr = (int) floor(log2(B.length));
        // astew: 2022/06/30 -- if B.length == 1, log2(B.length) = 0, which means ldexp(extr,-20) = 0.0
        //        and any amount of error would cause a failure.
        if(extr == 0) extr = 1; // for when B.length == 0

        double thresh = ldexp(extr, -20);
        XTEST_ASSERT( error < thresh,
          "\n%e not less than %e\n"
          "B.hr = %u\n"
          "C.hr = %u\n",
          error, thresh, B.hr, C.hr );
    }
}

