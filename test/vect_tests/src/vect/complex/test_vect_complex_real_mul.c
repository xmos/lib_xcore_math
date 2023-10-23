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

TEST_GROUP_RUNNER(vect_complex_real_mul) {
  RUN_TEST_CASE(vect_complex_real_mul, vect_complex_s16_real_mul_prepare);
  RUN_TEST_CASE(vect_complex_real_mul, vect_complex_s16_real_mul_basic);
  RUN_TEST_CASE(vect_complex_real_mul, vect_complex_s16_real_mul_random);
  RUN_TEST_CASE(vect_complex_real_mul, vect_complex_s32_real_mul_basic);
  RUN_TEST_CASE(vect_complex_real_mul, vect_complex_s32_real_mul_random);
}

TEST_GROUP(vect_complex_real_mul);
TEST_SETUP(vect_complex_real_mul) { fflush(stdout); }
TEST_TEAR_DOWN(vect_complex_real_mul) {}


static char msg_buff[200];

#define TEST_ASSERT_EQUAL_MSG(EXPECTED, ACTUAL, LINE_NUM)   do{       \
    if((EXPECTED)!=(ACTUAL)) {                                        \
      sprintf(msg_buff, "(test vector @ line %u)", (LINE_NUM));       \
      TEST_ASSERT_EQUAL_MESSAGE((EXPECTED), (ACTUAL), msg_buff);      \
    }} while(0)


static complex_s16_t mul_complex_s16(
    int16_t b_re, int16_t b_im,
    int16_t c, right_shift_t sat)
{
    complex_s32_t p = {
        ((int32_t)b_re) * c,
        ((int32_t)b_im) * c,
    };

    complex_s16_t res = {
        SAT(16)(ROUND_SHR(p.re, sat)),
        SAT(16)(ROUND_SHR(p.im, sat)),
    };

    return res;
}


static complex_s32_t mul_complex_s32(
    complex_s32_t b,
    int32_t c,
    right_shift_t b_shr,
    right_shift_t c_shr)
{
    int64_t bp_re = b.re;
    int64_t bp_im = b.im;
    int64_t cp = c;

    bp_re = SAT(32)(ASHR(32)(bp_re, b_shr));
    bp_im = SAT(32)(ASHR(32)(bp_im, b_shr));

    cp = SAT(32)(ASHR(32)(cp, c_shr));

    complex_s32_t res = {
        SAT(32)(ROUND_SHR(bp_re * cp, 30)),
        SAT(32)(ROUND_SHR(bp_im * cp, 30)),
    };

    return res;
}


#define REPS        ((SMOKE_TEST)?100:1000)


TEST(vect_complex_real_mul, vect_complex_s16_real_mul_prepare)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    
    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        const exponent_t B_exp = pseudo_rand_int(&seed, -100, 100);
        const exponent_t C_exp = pseudo_rand_int(&seed, -100, 100);

        const headroom_t B_hr = pseudo_rand_uint(&seed, 0, 16);
        const headroom_t C_hr = pseudo_rand_uint(&seed, 0, 16);

        exponent_t A_exp;
        right_shift_t sat;

        // Allow saturation
        vect_complex_s16_real_mul_prepare(&A_exp, &sat, B_exp, C_exp, B_hr, C_hr);

        int32_t B = -0x8000 >> B_hr;
        int32_t C = -0x8000 >> C_hr;
        int32_t P = B*C;

        if(P <= 0x8000){

            TEST_ASSERT_EQUAL(0, sat);
            TEST_ASSERT_EQUAL(B_exp+C_exp, A_exp);

        } else {

            TEST_ASSERT_GREATER_THAN(0, sat);
            
            // double Pf = ldexp(B, B_exp) * ldexp(C, C_exp);
            double Rf = ldexp(P, -sat);

            TEST_ASSERT( Rf == 0x8000 );
        }
    }
}
#undef REPS


TEST(vect_complex_real_mul, vect_complex_s16_real_mul_basic)
{
    

    typedef struct {
        struct {    complex_s16_t b;
                    int16_t c;  } value;
        right_shift_t sat;
        complex_s16_t expected;
        unsigned line;
    } test_case_t;

    test_case_t cases[] = {
        // value{ b{     re,      im},       c},   sat,  exp{     re,      im},    line num
        {       {  { 0x0000,  0x0000},  0x0000},     0,     { 0x0000,  0x0000},    __LINE__},
        {       {  { 0x0001,  0x0001},  0x0000},     0,     { 0x0000,  0x0000},    __LINE__},
        {       {  { 0x0000,  0x0000},  0x0001},     0,     { 0x0000,  0x0000},    __LINE__},
        {       {  { 0x0001,  0x0000},  0x0001},     0,     { 0x0001,  0x0000},    __LINE__},
        {       {  { 0x0001,  0x0001},  0x0001},     0,     { 0x0001,  0x0001},    __LINE__},
        {       {  { 0x0001,  0x0001}, -0x0002},     0,     {-0x0002, -0x0002},    __LINE__},
        {       {  { 0x0010,  0x0020},  0x0030},     0,     { 0x0300,  0x0600},    __LINE__},
        {       {  { 0x0100, -0x0100},  0x0100},     0,     { 0x7FFF, -0x7FFF},    __LINE__},
        {       {  { 0x0100, -0x0100},  0x0100},     4,     { 0x1000, -0x1000},    __LINE__},
    };

    const unsigned N_cases = sizeof(cases)/sizeof(test_case_t);

    const unsigned start_case = 0;

    for(unsigned int v = start_case; v < N_cases; v++){
        setExtraInfo_R(v);
        
        test_case_t* casse = &cases[v];

        //Verify mul_complex_s16() is correct. It's used in other test cases.
        complex_s16_t tmp = mul_complex_s16(casse->value.b.re, casse->value.b.im,
                                            casse->value.c, casse->sat);
                                            
        TEST_ASSERT_EQUAL_MSG(casse->expected.re, tmp.re, casse->line);
        TEST_ASSERT_EQUAL_MSG(casse->expected.im, tmp.im, casse->line);

        unsigned lengths[] = {1, 4, 16, 32, 40 };
        
        for( int l = 0; l < sizeof(lengths)/sizeof(lengths[0]); l++){
            unsigned len = lengths[l];

            headroom_t hr;
            struct {
                int16_t real[40];
                int16_t imag[40];
            } A, B;

            int16_t C[40];

            vect_s16_set(A.real, 0xCC, len);
            vect_s16_set(A.imag, 0xCC, len);
            vect_s16_set(B.real, casse->value.b.re, len);
            vect_s16_set(B.imag, casse->value.b.im, len);
            vect_s16_set(C, casse->value.c, len);

            hr = vect_complex_s16_real_mul(A.real, A.imag,
                                          B.real, B.imag,
                                          C, len, casse->sat);

            for(unsigned int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected.re, A.real[i], casse->line);
                TEST_ASSERT_EQUAL_MSG(casse->expected.im, A.imag[i], casse->line);
            }

            headroom_t exp_hr = vect_complex_s16_headroom(A.real, A.imag, len);
            TEST_ASSERT_EQUAL_MSG(exp_hr, hr, casse->line);

            memcpy(&A, &B, sizeof(A));
            hr = vect_complex_s16_real_mul(A.real, A.imag,
                                          A.real, A.imag,
                                          C, len, casse->sat);

            for(unsigned int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected.re, A.real[i], casse->line);
                TEST_ASSERT_EQUAL_MSG(casse->expected.im, A.imag[i], casse->line);
            }

            exp_hr = vect_complex_s16_headroom(A.real, A.imag, len);
            TEST_ASSERT_EQUAL_MSG(exp_hr, hr, casse->line);
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

TEST(vect_complex_real_mul, vect_complex_s16_real_mul_random)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    headroom_t hr;
    
    struct {
        int16_t real[MAX_LEN];
        int16_t imag[MAX_LEN];
    } A, B;

    int16_t C[MAX_LEN];

    for(unsigned int v = 0; v < REPS; v++){

        setExtraInfo_RS(v, seed);

        unsigned len = (pseudo_rand_uint32(&seed) % MAX_LEN) + 1;
        
        for(unsigned int i = 0; i < len; i++){
            unsigned shr = pseudo_rand_uint32(&seed) % 8;
            B.real[i] = pseudo_rand_int16(&seed) >> shr;
            B.imag[i] = pseudo_rand_int16(&seed) >> shr;
            C[i] = pseudo_rand_int16(&seed) >> shr;
        }

        int sat = (pseudo_rand_uint32(&seed) % 10);
        
        hr = vect_complex_s16_real_mul(A.real, A.imag,
                                      B.real, B.imag,
                                      C, len, sat);

        // headroom_t hrre, hrim;

        for(unsigned int i = 0; i < len; i++){
            complex_s16_t expected = mul_complex_s16(B.real[i], B.imag[i], C[i], sat);
            
            TEST_ASSERT_EQUAL_MESSAGE(expected.re, A.real[i], msg_buff);
            TEST_ASSERT_EQUAL_MESSAGE(expected.im, A.imag[i], msg_buff);
        }

        TEST_ASSERT_EQUAL_MSG( vect_complex_s16_headroom(A.real,A.imag,len),  hr, v);
        
        memcpy(&A, &B, sizeof(A));
        hr = vect_complex_s16_real_mul(A.real, A.imag,
                                      A.real, A.imag,
                                      C, len, sat);

        for(unsigned int i = 0; i < len; i++){
            complex_s16_t expected = mul_complex_s16(B.real[i], B.imag[i], C[i], sat);
            TEST_ASSERT_EQUAL_MESSAGE(expected.re, A.real[i], msg_buff);
            TEST_ASSERT_EQUAL_MESSAGE(expected.im, A.imag[i], msg_buff);
        }

        TEST_ASSERT_EQUAL_MSG( vect_complex_s16_headroom(A.real,A.imag,len),  hr, v);
    }
}
#undef MAX_LEN
#undef REPS


TEST(vect_complex_real_mul, vect_complex_s32_real_mul_basic)
{
    

    typedef struct {
        struct {    complex_s32_t b;
                    int32_t c;  } value;
        struct {    right_shift_t b;
                    right_shift_t c; } shr;
        complex_s32_t expected;
        unsigned line;
    } test_case_t;

    test_case_t cases[] = {
        // value{ b{         re,          im},                     c}, shr{  b,  c},  exp{         re,             im},   line num},
        {       {  { 0x00000000,  0x00000000},            0x00000000},    {  0,  0},     { 0x00000000,     0x00000000},   __LINE__},
        {       {  { 0x00000001,  0x00000001},            0x00000000},    {  0,  0},     { 0x00000000,     0x00000000},   __LINE__},
        {       {  { 0x00000000,  0x00000000},            0x00000001},    {  0,  0},     { 0x00000000,     0x00000000},   __LINE__},
        {       {  { 0x00000100,  0x00000000},            0x40000000},    {  0,  0},     { 0x00000100,     0x00000000},   __LINE__},
        {       {  { 0x00000100,  0x00000100},            0x40000000},    {  0,  0},     { 0x00000100,     0x00000100},   __LINE__},
        {       {  { 0x00000100,  0x00000100},            0x20000000},    {  0,  0},     { 0x00000080,     0x00000080},   __LINE__},
        {       {  { 0x00000001,  0x00000001},  (int) (0-0x80000000)},    {  0,  0},     {-0x00000002,    -0x00000002},   __LINE__},
        {       {  { 0x00000001,  0x00000001},  (int) (0-0x80000000)},    {  0,  1},     {-0x00000001,    -0x00000001},   __LINE__},
        {       {  { 0x00000001,  0x00000001},  (int) (0-0x80000000)},    { -1,  0},     {-0x00000004,    -0x00000004},   __LINE__},
        {       {  { 0x05000000,  0x0A000000},            0x40000000},    {  8,  0},     { 0x00050000,     0x000A0000},   __LINE__},
        {       {  { 0x05000000,  0x0A000000},            0x40000000},    {  0,  8},     { 0x00050000,     0x000A0000},   __LINE__},
        {       {  { 0x05000000,  0x0A000000},            0x40000000},    {  4, 12},     { 0x00000500,     0x00000A00},   __LINE__},
    };

    const unsigned N_cases = sizeof(cases)/sizeof(test_case_t);

    const unsigned start_case = 0;

    for(unsigned int v = start_case; v < N_cases; v++){
        setExtraInfo_R(v);
        
        test_case_t* casse = &cases[v];

        //Verify mul_complex_s32() is correct. It's used in other test cases.
        complex_s32_t tmp = mul_complex_s32(casse->value.b, casse->value.c, casse->shr.b, casse->shr.c);
                                            
        TEST_ASSERT_EQUAL_MSG(casse->expected.re, tmp.re, casse->line);
        TEST_ASSERT_EQUAL_MSG(casse->expected.im, tmp.im, casse->line);

        unsigned lengths[] = {1, 4, 16, 32, 40 };
        
        for( int l = 0; l < sizeof(lengths)/sizeof(lengths[0]); l++){
            unsigned len = lengths[l];

            headroom_t hr;

            complex_s32_t A[40];
            complex_s32_t B[40];
            int32_t C[40];

            vect_complex_s32_set(A, 0xCC, 0xCC, len);
            vect_complex_s32_set(B, casse->value.b.re, casse->value.b.im, len);
            vect_s32_set(C, casse->value.c, len);

            hr = vect_complex_s32_real_mul(A, B, C, len, casse->shr.b, casse->shr.c);

            for(unsigned int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected.re, A[i].re, casse->line);
                TEST_ASSERT_EQUAL_MSG(casse->expected.im, A[i].im, casse->line);
            }

            headroom_t exp_hr = vect_complex_s32_headroom(A, len);
            TEST_ASSERT_EQUAL_MSG(exp_hr, hr, casse->line);

            memcpy(&A, &B, sizeof(A));
            hr = vect_complex_s32_real_mul(A, A, C, len, casse->shr.b, casse->shr.c);

            for(unsigned int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected.re, A[i].re, casse->line);
                TEST_ASSERT_EQUAL_MSG(casse->expected.im, A[i].im, casse->line);
            }

            exp_hr = vect_complex_s32_headroom(A, len);
            TEST_ASSERT_EQUAL_MSG(exp_hr, hr, casse->line);
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

TEST(vect_complex_real_mul, vect_complex_s32_real_mul_random)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    headroom_t hr;
    
    complex_s32_t A[MAX_LEN];
    complex_s32_t B[MAX_LEN];
    int32_t C[MAX_LEN];

    for(unsigned int v = 0; v < REPS; v++){

        setExtraInfo_RS(v, seed);

        unsigned len = (pseudo_rand_uint32(&seed) % MAX_LEN) + 1;
        
        for(unsigned int i = 0; i < len; i++){
            unsigned shr = pseudo_rand_uint32(&seed) % 8;
            B[i].re = pseudo_rand_int32(&seed) >> shr;
            B[i].im = pseudo_rand_int32(&seed) >> shr;
            C[i] = pseudo_rand_int32(&seed) >> shr;
        }

        int b_shr = (pseudo_rand_uint32(&seed) % 10);
        int c_shr = (pseudo_rand_uint32(&seed) % 10);
        
        hr = vect_complex_s32_real_mul(A, B, C, len, b_shr, c_shr);

        for(unsigned int i = 0; i < len; i++){
            complex_s32_t expected = mul_complex_s32(B[i], C[i], b_shr, c_shr);
            TEST_ASSERT_EQUAL_MESSAGE(expected.re, A[i].re, msg_buff);
            TEST_ASSERT_EQUAL_MESSAGE(expected.im, A[i].im, msg_buff);
        }
        TEST_ASSERT_EQUAL_MSG(  vect_complex_s32_headroom(A, len),  hr, v);
        
        memcpy(&A, &B, sizeof(A));
        hr = vect_complex_s32_real_mul(A, A, C, len, b_shr, c_shr);

        for(unsigned int i = 0; i < len; i++){
            complex_s32_t expected = mul_complex_s32(B[i], C[i], b_shr, c_shr);
            TEST_ASSERT_EQUAL_MESSAGE(expected.re, A[i].re, msg_buff);
            TEST_ASSERT_EQUAL_MESSAGE(expected.im, A[i].im, msg_buff);
        }
        TEST_ASSERT_EQUAL_MSG(  vect_complex_s32_headroom(A, len),  hr, v);
    }
}
#undef MAX_LEN
#undef REPS

