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


TEST_GROUP_RUNNER(vect_complex_mag) {
  RUN_TEST_CASE(vect_complex_mag, vect_complex_s32_mag_prepare);
  RUN_TEST_CASE(vect_complex_mag, vect_complex_s16_mag_basic);
  RUN_TEST_CASE(vect_complex_mag, vect_complex_s16_mag_random);
  RUN_TEST_CASE(vect_complex_mag, vect_complex_s32_mag_basic);
  RUN_TEST_CASE(vect_complex_mag, vect_complex_s32_mag_random);
}

TEST_GROUP(vect_complex_mag);
TEST_SETUP(vect_complex_mag) { fflush(stdout); }
TEST_TEAR_DOWN(vect_complex_mag) {}


static char msg_buff[300];


const extern unsigned rot_table32_rows;
const extern complex_s32_t rot_table32[30][4];

#define TEST_ASSERT_EQUAL_MSG(EXPECTED, ACTUAL, LINE_NUM)   do{       \
    if((EXPECTED)!=(ACTUAL)) {                                        \
      sprintf(msg_buff, "(test vector @ line %u)", (LINE_NUM));       \
      TEST_ASSERT_EQUAL_MESSAGE((EXPECTED), (ACTUAL), msg_buff);      \
    }} while(0)

#define TEST_ASSERT_INT32_WITHIN_MSG(THRESHOLD, EXPECTED, ACTUAL, FMT, ...)   do{       \
    if(abs((ACTUAL)-(EXPECTED)) > THRESHOLD) {                                           \
      sprintf(msg_buff, (FMT), __VA_ARGS__);                                            \
      TEST_ASSERT_INT32_WITHIN_MESSAGE((THRESHOLD), (EXPECTED), (ACTUAL), msg_buff);    \
    }} while(0)


const extern unsigned rot_table32_rows;
const extern complex_s32_t rot_table32[30][4];

const extern unsigned rot_table16_rows;
const extern int16_t rot_table16[14][2][16];


static int16_t mag_complex_s16(complex_s16_t b, right_shift_t b_shr)
{
    int32_t bp_re = ASHR(16)(b.re, b_shr);
    int32_t bp_im = ASHR(16)(b.im, b_shr);


    int32_t sqr_mag = bp_re*bp_re + bp_im*bp_im;

    assert(sqr_mag >= 0);

    double mag = sqrt(sqr_mag);

    int16_t a = (int16_t) lround(mag);
    
    return SAT(16)(a);
}


static int32_t mag_complex_s32(complex_s32_t b, right_shift_t b_shr)
{
    int64_t bp_re = b.re;
    int64_t bp_im = b.im;

    bp_re = ASHR(32)(bp_re, b_shr);
    bp_im = ASHR(32)(bp_im, b_shr);

    bp_re = SAT(32)(bp_re);
    bp_im = SAT(32)(bp_im);

    int64_t sqr_mag = bp_re*bp_re + bp_im*bp_im;

    double mag = sqrt((double) sqr_mag);

    int32_t a = lround(mag);
    
    return SAT(32)(a);
}


#define REPS        ((SMOKE_TEST)?100:1000)
TEST(vect_complex_mag, vect_complex_s32_mag_prepare)
{
    

    unsigned seed = SEED_FROM_FUNC_NAME();


    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);
        
        exponent_t b_exp = pseudo_rand_int(&seed, -30, 30);
        headroom_t b_hr  = pseudo_rand_uint(&seed, 0, 31);

        complex_s32_t B = { 0, INT32_MIN >> b_hr };
        int32_t A_mag;

        exponent_t a_exp;
        right_shift_t b_shr;

        vect_complex_s32_mag_prepare(&a_exp, &b_shr, b_exp, b_hr);

        vect_complex_s32_mag(&A_mag, &B, 1, b_shr, (complex_s32_t*) rot_table32, rot_table32_rows);

        TEST_ASSERT_INT32_WITHIN(1, 0x40000000, A_mag);
        
        TEST_ASSERT_TRUE(  ( fabs( ldexp(A_mag, a_exp) + ldexp( B.im, b_exp) ) / B.im ) <= 0.00001  );
    }
}
#undef REPS


#define THRESHOLD   5

TEST(vect_complex_mag, vect_complex_s16_mag_basic)
{
    

    typedef struct {
        complex_s16_t b;

        right_shift_t b_shr;

        int16_t expected;
        unsigned line;
    } test_case_t;

    unsigned max_abs_delta = 0;

    test_case_t casses[] = {
        //{ {    b.re,     b.im}, b_shr,  expected,      line num
          { {  0x0000,   0x0000},     0,    0x0000,      __LINE__},
          { {  0x0000,   0x0000},     0,    0x0000,      __LINE__},
          { {  0x4000,   0x0000},     0,    0x4000,      __LINE__},
          { { -0x4000,   0x0000},     0,    0x4000,      __LINE__},
          { {  0x0000,   0x4000},     0,    0x4000,      __LINE__},
          { {  0x0000,  -0x4000},     0,    0x4000,      __LINE__},
          { {  0x0400,   0x0000},     0,    0x0400,      __LINE__},
          { { -0x0400,   0x0000},     0,    0x0400,      __LINE__},
          { {  0x0000,   0x0400},     0,    0x0400,      __LINE__},
          { {  0x0000,  -0x0400},     0,    0x0400,      __LINE__},
          { {  0x2000,   0x2000},     0,    0x2D41,      __LINE__},
          { { -0x1000,  -0x1000},     0,    0x16A1,      __LINE__},
          
          { {  0x4000,   0x0000},     4,    0x0400,      __LINE__},
          { { -0x4000,   0x0000},     4,    0x0400,      __LINE__},
          { {  0x0000,   0x4000},     4,    0x0400,      __LINE__},
          { {  0x0000,  -0x4000},     4,    0x0400,      __LINE__},
          { {  0x0400,   0x0000},     4,    0x0040,      __LINE__},
          { { -0x0400,   0x0000},     4,    0x0040,      __LINE__},
          { {  0x0000,   0x0400},     4,    0x0040,      __LINE__},
          { {  0x0000,  -0x0400},     4,    0x0040,      __LINE__},
          { {  0x2000,   0x2000},     4,    0x02D4,      __LINE__},
          { { -0x1000,  -0x1000},     4,    0x016A,      __LINE__},
          
          { {  0x0400,   0x0000},    -4,    0x4000,      __LINE__},
          { { -0x0400,   0x0000},    -4,    0x4000,      __LINE__},
          { {  0x0000,   0x0400},    -4,    0x4000,      __LINE__},
          { {  0x0000,  -0x0400},    -4,    0x4000,      __LINE__},
          { {  0x2000,   0x2000},    -1,    0x2D41<<1,   __LINE__},
          { { -0x1000,  -0x1000},    -2,    0x2D41<<1,   __LINE__},
    };

    const unsigned N_cases = sizeof(casses)/sizeof(test_case_t);

    const unsigned start_case = 0;

    for(unsigned int v = start_case; v < N_cases; v++){
        // setExtraInfo_R(v);
        
        test_case_t* casse = &casses[v];

        unsigned lengths[] = {1, 4, 16, 32, 34 };

        for( int l = 0; l < sizeof(lengths)/sizeof(lengths[0]); l++){
            unsigned len = lengths[l];
            // printf("! %u\n", len);
            headroom_t hr;
            int16_t A[40];

            struct {
                int16_t real[40];
                int16_t imag[40];
            } B;

            for(unsigned int i = 0; i < len; i++){
                A[i] = 0xCC;
                B.real[i] = casse->b.re;
                B.imag[i] = casse->b.im;
            }

            hr = vect_complex_s16_mag(A, B.real, B.imag, len, casse->b_shr, (int16_t*) rot_table16, rot_table16_rows);

            for(unsigned int i = 0; i < len; i++){
                
                unsigned abs_delta = abs(A[i] - casse->expected);

                TEST_ASSERT_INT32_WITHIN_MSG(THRESHOLD, casse->expected, A[i],
                    "(test vector %d @ line %u) (length: %u) (index %d): mag( (%d + i*%d) >> %d )   (delta = %d)",
                        v, casse->line, len, i, (int) B.real[i], (int) B.imag[i], casse->b_shr, (int) (A[i] - casse->expected) );

                max_abs_delta = MAX(max_abs_delta, abs_delta);
            }
            TEST_ASSERT_EQUAL_MSG(vect_s16_headroom((int16_t*) A, len), hr, casse->line);
        }
    }

    // printf("\tMaximum absolute delta: %u\n", max_abs_delta);
}
#undef THRESHOLD



#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (64)
#else
#  define REPS       (1000)
#  define MAX_LEN    (256)
#endif

#define THRESHOLD   10

TEST(vect_complex_mag, vect_complex_s16_mag_random)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    headroom_t hr;
    
    int16_t A[MAX_LEN];

    struct {
        int16_t real[MAX_LEN];
        int16_t imag[MAX_LEN];
    } B;

    unsigned max_abs_delta = 0;

    for(unsigned int v = 0; v < REPS; v++){

        unsigned rep_seed = seed;

        // setExtraInfo_RS(v, seed);

        unsigned len = (pseudo_rand_uint32(&seed) % MAX_LEN) + 1;
        
        headroom_t B_hr = pseudo_rand_uint32(&seed) % 4;

        for(unsigned int i = 0; i < len; i++){
            B.real[i] = pseudo_rand_int16(&seed) >> B_hr;
            B.imag[i] = pseudo_rand_int16(&seed) >> B_hr;
        }

        B_hr = MIN( vect_s16_headroom(B.real, len), vect_s16_headroom(B.imag, len) );
// printf("! %d\t\t%d\n", B_hr, B.imag[0]);
        right_shift_t b_shr = -(int)B_hr + 1;
        
        hr = vect_complex_s16_mag(A, B.real, B.imag, len, b_shr, (int16_t*) rot_table16, rot_table16_rows);


        for(unsigned int i = 0; i < len; i++){
            complex_s16_t bbb = {B.real[i], B.imag[i]};
            int16_t expected = mag_complex_s16(bbb, b_shr);

            unsigned abs_delta = abs(A[i] - expected);

            TEST_ASSERT_INT32_WITHIN_MSG(THRESHOLD, expected, A[i],
                "(rep %d; seed: 0x%08X) A[%i]: mag( (%d + i*%d) >> %d ) (delta = %d) (B_hr: %d)",
                       v, rep_seed, i, (int) B.real[i], (int) B.imag[i], b_shr, (int) (A[i] - expected), B_hr );

            max_abs_delta = MAX(max_abs_delta, abs_delta);
        }
        TEST_ASSERT_EQUAL_MSG(vect_s16_headroom((int16_t*) A, len), hr, v);
    }

    // printf("\tMaximum absolute delta: %u\n", max_abs_delta);
}
#undef MAX_LEN
#undef REPS
#undef THRESHOLD


#define THRESHOLD   7

TEST(vect_complex_mag, vect_complex_s32_mag_basic)
{
    

    typedef struct {
        complex_s32_t b;
        right_shift_t b_shr;

        int32_t expected;
        unsigned line;
    } test_case_t;

    unsigned max_abs_delta = 0;

    test_case_t casses[] = {
        //{ {       b.re,       b.im}, b_shr,   expected,      line num
          { { 0x00000000, 0x00000000},     0, 0x00000000,      __LINE__},
          { { 0x00000000, 0x00000000},     1, 0x00000000,      __LINE__},
          { { 0x40000000, 0x00000000},     0, 0x40000000,      __LINE__},
          { {-0x40000000, 0x00000000},     0, 0x40000000,      __LINE__},
          { { 0x00000000, 0x40000000},     0, 0x40000000,      __LINE__},
          { { 0x00000000,-0x40000000},     0, 0x40000000,      __LINE__},
          { { 0x04000000, 0x00000000},     0, 0x04000000,      __LINE__},
          { {-0x04000000, 0x00000000},     0, 0x04000000,      __LINE__},
          { { 0x00000000, 0x04000000},     0, 0x04000000,      __LINE__},
          { { 0x00000000,-0x04000000},     0, 0x04000000,      __LINE__},
          { { 0x40000000, 0x00000000},     4, 0x04000000,      __LINE__},
          { {-0x40000000, 0x00000000},     4, 0x04000000,      __LINE__},
          { { 0x00000000, 0x40000000},     4, 0x04000000,      __LINE__},
          { { 0x00000000,-0x40000000},     4, 0x04000000,      __LINE__},
          { { 0x00001000, 0x00000000},     0, 0x00001000,      __LINE__},
          { { 0x00001000, 0x00000000},     4, 0x00000100,      __LINE__},
          { { 0x00001000, 0x00000000},     8, 0x00000010,      __LINE__},
          { { 0x00001000, 0x00000000},    12, 0x00000001,      __LINE__},
          { { 0x20000000, 0x20000000},     0, 0x2d413ccd,      __LINE__},
          { { 0x20000000,-0x20000000},     4, 0x02d413cd,      __LINE__},
          { {-0x40000000,-0x40000000},     0, 0x5A82799A,      __LINE__},
          { { 0x0186B400, 0x22E7EC80},     0, 0x22f07608,      __LINE__}, //worst case I've found, off by 7
        //   { { 0x20daf1a5, 0x7e8080b5},     0,
    };

    const unsigned N_cases = sizeof(casses)/sizeof(test_case_t);

    const unsigned start_case = 0;

    for(unsigned int v = start_case; v < N_cases; v++){
        // setExtraInfo_R(v);
        
        test_case_t* casse = &casses[v];

        unsigned lengths[] = {1, 4, 16, 32, 34 };

        for( int l = 0; l < sizeof(lengths)/sizeof(lengths[0]); l++){
            unsigned len = lengths[l];
            // printf("! %u\n", len);
            headroom_t hr;
            int32_t A[40];
            complex_s32_t B[40];
            // complex_s32_t C;

            for(unsigned int i = 0; i < len; i++){
                A[i] = 0xCC;
                B[i].re = casse->b.re;
                B[i].im = casse->b.im;
            }

            hr = vect_complex_s32_mag(A, B, len, casse->b_shr, (complex_s32_t*) rot_table32, rot_table32_rows);

            for(unsigned int i = 0; i < len; i++){
                
                unsigned abs_delta = abs(A[i] - casse->expected);

                TEST_ASSERT_INT32_WITHIN_MSG(THRESHOLD, casse->expected, A[i],
                    "(test vector %d @ line %u) (length: %u) (index %d): mag(( %d + i*%d) >> %d)   (delta = %d)",
                        v, casse->line, len, i, (int) B[i].re, (int) B[i].im, casse->b_shr, (int) (A[i] - casse->expected) );

                max_abs_delta = MAX(max_abs_delta, abs_delta);
            }
            TEST_ASSERT_EQUAL_MSG(vect_s32_headroom((int32_t*) A, len), hr, casse->line);
        }
    }

    // printf("\tMaximum absolute delta: %u\n", max_abs_delta);
}
#undef THRESHOLD



#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (64)
#else
#  define REPS       (1000)
#  define MAX_LEN    (256)
#endif

#define THRESHOLD   7

TEST(vect_complex_mag, vect_complex_s32_mag_random)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    headroom_t hr;
    
    int32_t A[MAX_LEN];
    complex_s32_t B[MAX_LEN];

    unsigned max_abs_delta = 0;

    for(unsigned int v = 0; v < REPS; v++){

        unsigned rep_seed = seed;

        // setExtraInfo_RS(v, seed);

        unsigned len = (pseudo_rand_uint32(&seed) % MAX_LEN) + 1;
        
        headroom_t B_hr = pseudo_rand_uint32(&seed) % 8;

        for(unsigned int i = 0; i < len; i++){
            //Bottom bits masked out because the expectation is calculated using double-precision floats, which
            // only have 53 mantissa bits. Masking out the bottom 8 bits of each means that their product will
            // have 16 zeros in the least significant bits, so it won't get truncated when converting to double.
            B[i].re = ( pseudo_rand_int32(&seed) & 0xFFFFFF00   ) >> (B_hr);
            B[i].im = ( pseudo_rand_int32(&seed) & 0xFFFFFF00   ) >> (B_hr);
        }
        
        B_hr = vect_s32_headroom((int32_t*) B, 2*len);

        right_shift_t b_shr = -(int)B_hr + 1;
        
        hr = vect_complex_s32_mag(A, B, len, b_shr, (complex_s32_t*) rot_table32, rot_table32_rows);


        for(unsigned int i = 0; i < len; i++){
            int32_t expected = mag_complex_s32(B[i], b_shr);

            unsigned abs_delta = abs(A[i] - expected);

            TEST_ASSERT_INT32_WITHIN_MSG(THRESHOLD, expected, A[i],
                "(rep %d; seed: 0x%08X) A[%i]: mag(( %d + i*%d) >> %d) (delta = %d) (B_hr: %d)",
                       v, rep_seed, i, (int) B[i].re, (int) B[i].im, b_shr, (int) (A[i] - expected), B_hr );

            max_abs_delta = MAX(max_abs_delta, abs_delta);
        }
        TEST_ASSERT_EQUAL_MSG(vect_s32_headroom((int32_t*) A, len), hr, v);
    }

    // printf("\tMaximum absolute delta: %u\n", max_abs_delta);
}
#undef MAX_LEN
#undef REPS
#undef THRESHOLD

