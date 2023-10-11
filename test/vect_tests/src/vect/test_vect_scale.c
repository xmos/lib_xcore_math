// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xmath/xmath.h"
#include "xmath/xs3/vpu_scalar_ops.h"

#include "../tst_common.h"
#include "unity_fixture.h"


TEST_GROUP_RUNNER(vect_scale) {
  RUN_TEST_CASE(vect_scale, vect_s16_scale_prepare);
  RUN_TEST_CASE(vect_scale, vect_s16_scale_basic);
  RUN_TEST_CASE(vect_scale, vect_s16_scale_random);
  RUN_TEST_CASE(vect_scale, vect_s32_scale_basic);
  RUN_TEST_CASE(vect_scale, vect_s32_scale_random);
}

TEST_GROUP(vect_scale);
TEST_SETUP(vect_scale) { fflush(stdout); }
TEST_TEAR_DOWN(vect_scale) {}


static char msg_buff[200];

#define TEST_ASSERT_EQUAL_MSG(EXPECTED, ACTUAL, LINE_NUM)   do{       \
    if((EXPECTED)!=(ACTUAL)) {                                        \
      sprintf(msg_buff, "(test vector @ line %u)", (LINE_NUM));       \
      TEST_ASSERT_EQUAL_MESSAGE((EXPECTED), (ACTUAL), msg_buff);      \
    }} while(0)


static int16_t scalar_mul_s16(int16_t b, int16_t c, right_shift_t sat)
{
    return vlsat16( ((int32_t)b) * c  , sat);
}


static int32_t scalar_mul_s32(int32_t b, int32_t c, int b_shr, int c_shr)
{
    int32_t B = vlashr32(b, b_shr);
    int32_t C = vlashr32(c, c_shr);
    return vlmul32(B, C);
}



#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (64)
#else
#  define REPS       (1000)
#  define MAX_LEN    (256)
#endif


TEST(vect_scale, vect_s16_scale_prepare)
{
    

    unsigned seed = SEED_FROM_FUNC_NAME();


    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);
        
        exponent_t b_exp = pseudo_rand_int(&seed, -30, 30);
        headroom_t b_hr  = pseudo_rand_uint(&seed, 0, 15);
        exponent_t c_exp = pseudo_rand_int(&seed, -30, 30);
        headroom_t c_hr  = pseudo_rand_uint(&seed, 0, 15);

        int16_t WORD_ALIGNED B_re = ((int16_t)-0x8000) >> b_hr;
        int16_t WORD_ALIGNED B_im = ((int16_t)-0x8000) >> b_hr;
        int16_t WORD_ALIGNED C = ((int16_t)-0x8000) >> c_hr;
        int16_t WORD_ALIGNED A_re;
        int16_t WORD_ALIGNED A_im;

        exponent_t a_exp;
        right_shift_t sat;

        vect_s16_scale_prepare(&a_exp, &sat, b_exp, c_exp, b_hr, c_hr);

        vect_complex_s16_real_scale(&A_re, &A_im, &B_re, &B_im, C, 1, sat);

        const int32_t p = ((int32_t)B_re) * C;
        int16_t expected = p;

        if( p > VPU_INT16_MAX )
            expected = VPU_INT16_MAX;

        TEST_ASSERT_EQUAL_HEX16(expected, A_re);
        TEST_ASSERT_EQUAL_HEX16(expected, A_im);

        // Make sure the exponent was chosen correctly

        double fp  = ldexp(B_re, b_exp) * ldexp(C, c_exp);
        double fa  = ldexp(A_re, a_exp);

        // (A should be no more than 1 LSb off)
        TEST_ASSERT( fabs(fp - fa) <= ldexp(1,a_exp) );
        
    }
}


TEST(vect_scale, vect_s16_scale_basic)
{
    

    typedef struct {
        struct {    int16_t b;  int16_t alpha;  } value;
        right_shift_t sat;
        int16_t expected;
        unsigned line;
    } test_case_t;

    test_case_t cases[] = {
        // value{       b     alpha }   sat        exp        line num
        {       {  0x0000,   0x0000 },    0,    0x0000,       __LINE__},
        {       {  0x0001,   0x0000 },    0,    0x0000,       __LINE__},
        {       {  0x0000,   0x0001 },    0,    0x0000,       __LINE__},
        {       {  0x4000,   0x4000 },    0,    0x7FFF,       __LINE__},
        {       {  0x4000,   0x4000 },   14,    0x4000,       __LINE__},
        {       { -0x4000,   0x4000 },   14,   -0x4000,       __LINE__},
        {       { -0x4000,  -0x4000 },   14,    0x4000,       __LINE__},
        {       {  0x4000,   0x0001 },   14,    0x0001,       __LINE__},
        {       {  0x0001,   0x4000 },   14,    0x0001,       __LINE__},
        {       {  0x4000,   0x0123 },   14,    0x0123,       __LINE__},
        {       {  0x0123,   0x4000 },   14,    0x0123,       __LINE__},
        {       {  0x0040,   0x0200 },   14,    0x0002,       __LINE__},
        {       {  0x0040,   0x0100 },   14,    0x0001,       __LINE__},
        {       {  0x0040,   0x0080 },   14,    0x0001,       __LINE__},
        {       {  0x0040,   0x0040 },   14,    0x0000,       __LINE__},
        {       {  0x7f00,   0x7f00 },   14,    0x7fff,       __LINE__},
        {       {  0x7f00,  -0x7f00 },   14,   -0x7fff,       __LINE__},
        {       { -0x8000,   0x4000 },   14,   -0x7fff,       __LINE__},
        {       {  0x4000,   0x4000 },   15,    0x2000,       __LINE__},
        {       {  0x4000,   0x2000 },   14,    0x2000,       __LINE__},
        {       {  0x4000,   0x2000 },   15,    0x1000,       __LINE__},
        {       {  0x4000,   0x0200 },   17,    0x0040,       __LINE__},
        {       {  0x0800,   0x4000 },   13,    0x1000,       __LINE__},
        {       {  0x0800,   0x4000 },   12,    0x2000,       __LINE__},
        {       {  0x0800,   0x4000 },   11,    0x4000,       __LINE__},
        {       {  0x0800,   0x4000 },   10,    0x7fff,       __LINE__},
        {       {  0x0800,  -0x4000 },   10,   -0x7fff,       __LINE__},
        {       {  0x0800,   0x2000 },   10,    0x4000,       __LINE__},

        
    };

    const unsigned N_cases = sizeof(cases)/sizeof(test_case_t);

    const unsigned start_case = 3;

    for(unsigned int v = start_case; v < N_cases; v++){
        setExtraInfo_R(v);
        
        test_case_t* casse = &cases[v];

        unsigned lengths[] = {1, 4, 16, 32, 40 };

        for( int l = 0; l < sizeof(lengths)/sizeof(lengths[0]); l++){
            unsigned len = lengths[l];

            headroom_t hr;
            int16_t A[40];
            int16_t B[40];

            for(unsigned int i = 0; i < len; i++){
                A[i] = 0xCC;
                B[i] = casse->value.b;
            }


            hr = vect_s16_scale(A, B, len, casse->value.alpha, casse->sat);

            for(unsigned int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(vect_s16_headroom(A, len), hr, casse->line);
            }

            memcpy(A, B, sizeof(A));
            hr = vect_s16_scale(A, A, len, casse->value.alpha, casse->sat);

            for(unsigned int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(vect_s16_headroom(A, len), hr, casse->line);
            }
        }
    }
}


TEST(vect_scale, vect_s16_scale_random)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    headroom_t hr;
    int16_t A[MAX_LEN];
    int16_t B[MAX_LEN];
    int16_t alpha;

    for(unsigned int v = 0; v < REPS; v++){

        setExtraInfo_RS(v, seed);

        unsigned len = (pseudo_rand_uint32(&seed) % MAX_LEN) + 1;
        
        for(unsigned int i = 0; i < len; i++){
            unsigned shr = pseudo_rand_uint32(&seed) % 8;
            B[i] = pseudo_rand_int16(&seed) >> shr;
        }

        alpha = pseudo_rand_int16(&seed) >> (pseudo_rand_uint32(&seed) % 8);

        right_shift_t sat = (pseudo_rand_uint32(&seed) % 5) - 2;
        
        const char sprintpat[] = "rep(%d)[%d of %u]: %d <-- ((%d >> %d) * %d) >> 14     (A[i]=0x%04X; B[i]=0x%04X; alpha=0x%04X)";

        hr = vect_s16_scale(A, B, len, alpha, sat);

        for(unsigned int i = 0; i < len; i++){
            int16_t expected = scalar_mul_s16(B[i], alpha, sat);
            if(expected != A[i]) sprintf(msg_buff, sprintpat,v, i, len, A[i], B[i], sat, alpha, (uint16_t)A[i], (uint16_t)B[i],  (uint16_t)alpha);
            TEST_ASSERT_EQUAL_MESSAGE(expected, A[i], msg_buff);
        }
        TEST_ASSERT_EQUAL(vect_s16_headroom(A, len), hr);
        
        memcpy(A, B, sizeof(A[0])*len);
        hr = vect_s16_scale(A, A, len, alpha, sat);

        for(unsigned int i = 0; i < len; i++){
            int16_t expected = scalar_mul_s16(B[i], alpha, sat);
            if(expected != A[i]) sprintf(msg_buff, sprintpat,v, i, len, A[i], B[i], sat, alpha, (uint16_t)A[i], (uint16_t)B[i],  (uint16_t)alpha);
            TEST_ASSERT_EQUAL_MESSAGE(expected, A[i], msg_buff);
        }
        TEST_ASSERT_EQUAL(vect_s16_headroom(A, len), hr);
    }
}


TEST(vect_scale, vect_s32_scale_basic)
{
    

    typedef struct {
        struct {    int32_t b;       int32_t c;         } value;
        struct {    right_shift_t b; right_shift_t c;   } shr;
        int32_t expected;
        unsigned line;
    } test_case_t;

    test_case_t cases[] = {
        // value{                    b               c }   shr{   b    c }              exp        line num
        {       {           0x00000000,     0x00000000 },     {   0,   0 },      0x00000000,       __LINE__},
        {       {           0x00010000,     0x00000000 },     {   0,   0 },      0x00000000,       __LINE__},
        {       {           0x00000000,     0x00010000 },     {   0,   0 },      0x00000000,       __LINE__},
        {       {           0x40000000,     0x40000000 },     {   0,   0 },      0x40000000,       __LINE__},
        {       {          -0x40000000,     0x40000000 },     {   0,   0 },     -0x40000000,       __LINE__},
        {       {          -0x40000000,    -0x40000000 },     {   0,   0 },      0x40000000,       __LINE__},
        {       {           0x40000000,     0x00010000 },     {   0,   0 },      0x00010000,       __LINE__},
        {       {           0x00010000,     0x40000000 },     {   0,   0 },      0x00010000,       __LINE__},
        {       {           0x40000000,     0x01230000 },     {   0,   0 },      0x01230000,       __LINE__},
        {       {           0x01230000,     0x40000000 },     {   0,   0 },      0x01230000,       __LINE__},
        {       {           0x00400000,     0x02000000 },     {   0,   0 },      0x00020000,       __LINE__},
        {       {           0x00400000,     0x01000000 },     {   0,   0 },      0x00010000,       __LINE__},
        {       {           0x00004000,     0x00010000 },     {   0,   0 },      0x00000001,       __LINE__},
        {       {           0x00004000,     0x00008000 },     {   0,   0 },      0x00000001,       __LINE__},
        {       {           0x00000400,     0x00000400 },     {   0,   0 },      0x00000000,       __LINE__},
        {       {           0x7f000000,     0x7f000000 },     {   0,   0 },      0x7fffffff,       __LINE__},
        {       {           0x7f000000,    -0x7f000000 },     {   0,   0 },     -0x7fffffff,       __LINE__},
        {       { (int) (0-0x80000000),     0x40000000 },     {   0,   0 },     -0x7fffffff,       __LINE__},
        {       {           0x40000000,     0x40000000 },     {   1,   0 },      0x20000000,       __LINE__},
        {       {           0x40000000,     0x20000000 },     {   0,   0 },      0x20000000,       __LINE__},
        {       {           0x40000000,     0x20000000 },     {   1,   0 },      0x10000000,       __LINE__},
        {       {           0x40000000,     0x02000000 },     {   3,   0 },      0x00400000,       __LINE__},
        {       {           0x08000000,     0x40000000 },     { - 1,   0 },      0x10000000,       __LINE__},
        {       {           0x08000000,     0x40000000 },     { - 2,   0 },      0x20000000,       __LINE__},
        {       {           0x08000000,     0x40000000 },     { - 3,   0 },      0x40000000,       __LINE__},
        {       {           0x08000000,     0x40000000 },     { - 4,   0 },      0x7fffffff,       __LINE__},
        {       {           0x08000000,    -0x40000000 },     { - 4,   0 },     -0x7fffffff,       __LINE__},
        {       {           0x08000000,     0x20000000 },     { - 4,   0 },      0x40000000,       __LINE__},
    };

    const unsigned N_cases = sizeof(cases)/sizeof(test_case_t);

    const unsigned start_case = 0;

    for(unsigned int v = start_case; v < N_cases; v++){
        setExtraInfo_R(v);
        
        test_case_t* casse = &cases[v];

        //Verify mul_s32() is correct. It's used in other test cases.
        TEST_ASSERT_EQUAL_MSG(casse->expected, scalar_mul_s32(casse->value.b, casse->value.c,
                                casse->shr.b, casse->shr.c), casse->line);

        unsigned lengths[] = {1, 4, 16, 32, 40 };
        

        for( int l = 0; l < sizeof(lengths)/sizeof(lengths[0]); l++){
            unsigned len = lengths[l];

            headroom_t hr;
            int32_t A[40];
            int32_t B[40];

            for(unsigned int i = 0; i < len; i++){
                A[i] = 0xCC;
                B[i] = casse->value.b;
            }

            hr = vect_s32_scale(A, B, len, casse->value.c, casse->shr.b, casse->shr.c);

            for(unsigned int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(vect_s32_headroom(A, len), hr, casse->line);
            }

            memcpy(A, B, sizeof(A));
            hr = vect_s32_scale(A, A, len, casse->value.c, casse->shr.b, casse->shr.c);

            for(unsigned int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(vect_s32_headroom(A, len), hr, casse->line);
            }
        }
    }
}


TEST(vect_scale, vect_s32_scale_random)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    headroom_t hr;
    int32_t A[MAX_LEN];
    int32_t B[MAX_LEN];
    // int32_t alpha;

    for(unsigned int v = 0; v < REPS; v++){

        setExtraInfo_R(v);

        unsigned len = (pseudo_rand_uint32(&seed) % MAX_LEN) + 1;
        
        for(unsigned int i = 0; i < len; i++){
            unsigned shr = pseudo_rand_uint32(&seed) % 8;
            B[i] = pseudo_rand_int32(&seed) >> shr;
        }

        int32_t c = pseudo_rand_int32(&seed);

        right_shift_t b_shr = pseudo_rand_uint(&seed, -2, 2);
        right_shift_t c_shr = pseudo_rand_uint(&seed, 0, 8);
        
        // const char sprintpat[] = "rep(%d)[%d of %u]: %ld <-- ((%ld >> %d) * %ld) >> 30     (A[i]=0x%08X; B[i]=0x%08X; alpha=0x%08X)";

        hr = vect_s32_scale(A, B, len, c, b_shr, c_shr);

        for(unsigned int i = 0; i < len; i++){
            int32_t expected = scalar_mul_s32(B[i], c, b_shr, c_shr);
            TEST_ASSERT_EQUAL(expected, A[i]);
        }
        TEST_ASSERT_EQUAL(vect_s32_headroom(A, len), hr);
        
        memcpy(A, B, sizeof(A[0])*len);
        hr = vect_s32_scale(A, A, len, c, b_shr, c_shr);

        for(unsigned int i = 0; i < len; i++){
            int32_t expected = scalar_mul_s32(B[i], c, b_shr, c_shr);
            TEST_ASSERT_EQUAL(expected, A[i]);
        }
        TEST_ASSERT_EQUAL(vect_s32_headroom(A, len), hr);
    }
}

