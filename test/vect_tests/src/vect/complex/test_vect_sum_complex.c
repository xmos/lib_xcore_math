// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "xmath/xmath.h"

#include "../../tst_common.h"
#include "../src/vect/vpu_helper.h"

#include "unity_fixture.h"

TEST_GROUP_RUNNER(vect_complex_sum) {
  RUN_TEST_CASE(vect_complex_sum, vect_complex_s32_sum_prepare);
  RUN_TEST_CASE(vect_complex_sum, vect_complex_s16_sum_basic);
  RUN_TEST_CASE(vect_complex_sum, vect_complex_s16_sum_random);
  RUN_TEST_CASE(vect_complex_sum, vect_complex_s32_sum_basic);
  RUN_TEST_CASE(vect_complex_sum, vect_complex_s32_sum_random);
}

TEST_GROUP(vect_complex_sum);
TEST_SETUP(vect_complex_sum) { fflush(stdout); }
TEST_TEAR_DOWN(vect_complex_sum) {}


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
#  define MAX_LEN    (500)
#endif


TEST(vect_complex_sum, vect_complex_s32_sum_prepare)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    
    complex_s32_t B[MAX_LEN];

    unsigned some_shr = 0;

    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);
        
        exponent_t b_exp = pseudo_rand_int(&seed, -30, 30);
        headroom_t b_hr  = pseudo_rand_uint(&seed, 0, 20);
        unsigned b_length = pseudo_rand_uint(&seed, 1, MAX_LEN-1);

        if(r == REPS-1)
          b_length = MAX_LEN;

        for(unsigned int i = 0; i < MAX_LEN; i++){
            B[i].re = 0x40000000 >> b_hr;
            B[i].im = INT32_MIN >> b_hr;
        }

        b_hr = vect_complex_s32_headroom(B, b_length);
 
        complex_s64_t A;
        exponent_t a_exp;
        right_shift_t b_shr;

        vect_complex_s32_sum_prepare(&a_exp, &b_shr, b_exp, b_hr, b_length);

        TEST_ASSERT_EQUAL(b_exp + b_shr, a_exp);

        TEST_ASSERT_GREATER_OR_EQUAL(0, b_shr);

        if(b_shr) some_shr = 1;

        vect_complex_s32_sum(&A, B, b_length, b_shr);

        complex_s64_t expected = {
            (b_length * ((int64_t)B[0].re)) >> b_shr,
            (b_length * ((int64_t)B[0].im)) >> b_shr };

        
        TEST_ASSERT( expected.re == A.re );
        TEST_ASSERT( expected.im == A.im );

    }

    if(!(SMOKE_TEST)){
      TEST_ASSERT_TRUE(some_shr);
    }

}
#undef REPS
#undef MAX_LEN


TEST(vect_complex_sum, vect_complex_s16_sum_basic)
{
    

    typedef struct {
        complex_s16_t b;
        unsigned line;
    } test_case_t;

    test_case_t casses[] = {
        // b(      re,      im ),    line num
        {   {  0x0000,  0x0000 },    __LINE__},
        {   {  0x0001,  0x0002 },    __LINE__},
        {   { -0x0001, -0x0002 },    __LINE__},
        {   {  0x0002,  0x0003 },    __LINE__},
        {   {  0x7FFF,  0x7FFF },    __LINE__},
        {   { -0x8000, -0x8000 },    __LINE__},
    };

    const unsigned N_cases = sizeof(casses)/sizeof(test_case_t);

    const unsigned start_case = 0;

    for(unsigned int v = start_case; v < N_cases; v++){
        setExtraInfo_R(v);
        
        test_case_t* casse = &casses[v];

        struct {
            int16_t real[40];
            int16_t imag[40];
        } B;
        complex_s32_t result;

        unsigned lengths[] = {1, 4, 16, 32, 40 };

        for( int l = 0; l < sizeof(lengths)/sizeof(lengths[0]); l++){

            unsigned len = lengths[l];

            for(unsigned int i = 0; i < len; i++){
                B.real[i] = casse->b.re;
                B.imag[i] = casse->b.im;
            }

            result = vect_complex_s16_sum(B.real, B.imag, len);

            complex_s32_t exp = {
                ((int32_t) casse->b.re) * len,
                ((int32_t) casse->b.im) * len};

            TEST_ASSERT_EQUAL_MSG(exp.re, result.re, casse->line);
            TEST_ASSERT_EQUAL_MSG(exp.im, result.im, casse->line);
        }
    }
}


#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (64)
#else
#  define REPS       (1000)
#  define MAX_LEN    (256)
#endif

TEST(vect_complex_sum, vect_complex_s16_sum_random)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    complex_s32_t result;
    struct {
        int16_t real[MAX_LEN];
        int16_t imag[MAX_LEN];
    } B;

    for(unsigned int v = 0; v < REPS; v++){

        setExtraInfo_RS(v, seed);

        unsigned len = (pseudo_rand_uint32(&seed) % MAX_LEN) + 1;
    
        complex_s32_t expected = {0,0};
        for(unsigned int i = 0; i < len; i++){
            B.real[i] = pseudo_rand_int16(&seed);
            B.imag[i] = pseudo_rand_int16(&seed);

            expected.re = expected.re + B.real[i];
            expected.im = expected.im + B.imag[i];
        }

        result = vect_complex_s16_sum(B.real, B.imag, len);

        TEST_ASSERT_EQUAL(expected.re, result.re);
        TEST_ASSERT_EQUAL(expected.im, result.im);
        
    }
}
#undef MAX_LEN
#undef REPS


TEST(vect_complex_sum, vect_complex_s32_sum_basic)
{
    

    typedef struct {
        complex_s32_t b;
        right_shift_t b_shr;
        unsigned line;
    } test_case_t;

    test_case_t casses[] = {
        // b{                   re,              im }, b_shr,  line num},
        {   {           0x00000000,     0x000000000 },     0,  __LINE__},
        {   {           0x00000001,     0x000000000 },     0,  __LINE__},
        {   {          -0x00000001,     0x000000000 },     0,  __LINE__},
        {   {           0x00020000,     0x000000000 },     0,  __LINE__},
        {   {           0x7FFFFFFF,     0x000000000 },     0,  __LINE__},
        {   { (int) (0-0x80000000),     0x000000000 },     0,  __LINE__},
    };

    const unsigned N_cases = sizeof(casses)/sizeof(test_case_t);

    const unsigned start_case = 0;

    for(unsigned int v = start_case; v < N_cases; v++){
        setExtraInfo_R(v);
        
        test_case_t* casse = &casses[v];

        complex_s32_t B[40];

        unsigned lengths[] = {1, 4, 16, 32, 40 };

        for( int l = 0; l < sizeof(lengths)/sizeof(lengths[0]); l++){
            unsigned len = lengths[l];

            for(unsigned int i = 0; i < len; i++){
                B[i] = casse->b;
            }

            complex_s64_t res = {31337, 31337};
            vect_complex_s32_sum(&res, B, len, casse->b_shr);

            complex_s64_t expected = {
                ((int64_t) casse->b.re) * len,
                ((int64_t) casse->b.im) * len, };

            TEST_ASSERT_EQUAL_MSG(expected.re, res.re, casse->line);
            TEST_ASSERT_EQUAL_MSG(expected.im, res.im, casse->line);
        }
    }
}


#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (64)
#else
#  define REPS       (1000)
#  define MAX_LEN    (256)
#endif

TEST(vect_complex_sum, vect_complex_s32_sum_random)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    complex_s64_t result;
    complex_s32_t B[MAX_LEN];

    for(unsigned int v = 0; v < REPS; v++){

        setExtraInfo_RS(v, seed);

        unsigned len = (pseudo_rand_uint32(&seed) % MAX_LEN) + 1;
        
        complex_s64_t expected = {0,0};
        for(unsigned int i = 0; i < len; i++){
            B[i].re = pseudo_rand_int32(&seed);
            B[i].im = pseudo_rand_int32(&seed);
        }

        headroom_t hr = vect_complex_s32_headroom(B, len);

        unsigned len_log2 = u32_ceil_log2(len);

        /*
            With a 40 bit accumulator we can add 2^(8+hr+b_shr) elements without saturating.
            We want b_shr to be the smallest (cannot be negative) number it can be.

            max_res = -(2**31) * 2**(len_log2-hr-b_shr)
            -(2**39) = -(2**31) * 2**(len_log2-hr-b_shr)  -->  2**8 = 2**(len_log2-hr-b_shr)
            8 = len_log2-hr-b_shr --> b_shr = len_log2-hr-8

        */

        right_shift_t b_shr = len_log2 - hr - 8;
        b_shr = MAX(0, b_shr);

        int64_t p = 0x80000000LL;
        p = p >> (hr+b_shr);
        p *= len;
        // printf("%d (>= 2**%d)\t%d\t%d\t0x%llX\n", len, len_log2, hr, b_shr, (uint64_t)p);
        // Shouldn't saturate 40-bit accumulator
        assert(p <= 0x8000000000LL);
        // If b_shr > 0, it should saturate a 39-bit accumulator.

        assert((b_shr == 0) || (p >= 0x4000000000LL));

        vect_complex_s32_sum(&result, B, len, b_shr);

        for(unsigned int i = 0; i < len; i++){
            expected.re += ROUND_SHR(B[i].re, b_shr);
            expected.im += ROUND_SHR(B[i].im, b_shr);
        }

        //Unity 64-bit support disabled...
        int32_t exp_re_hi = (expected.re >> 32) & 0xFFFFFFFF;
        uint32_t exp_re_lo = (expected.re & 0xFFFFFFFF);
        int32_t exp_im_hi = (expected.im >> 32) & 0xFFFFFFFF;
        uint32_t exp_im_lo = (expected.im & 0xFFFFFFFF);

        int32_t res_re_hi = (result.re >> 32) & 0xFFFFFFFF;
        uint32_t res_re_lo = (result.re & 0xFFFFFFFF);
        int32_t res_im_hi = (result.im >> 32) & 0xFFFFFFFF;
        uint32_t res_im_lo = (result.im & 0xFFFFFFFF);

        TEST_ASSERT_EQUAL_INT32(exp_re_hi, res_re_hi);
        TEST_ASSERT_EQUAL_UINT32(exp_re_lo, res_re_lo);
        TEST_ASSERT_EQUAL_INT32(exp_im_hi, res_im_hi);
        TEST_ASSERT_EQUAL_UINT32(exp_im_lo, res_im_lo);
        
    }
}
#undef MAX_LEN
#undef REPS

