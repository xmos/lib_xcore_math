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

TEST_GROUP_RUNNER(vect_complex_squared_mag) {
  RUN_TEST_CASE(vect_complex_squared_mag, vect_complex_s16_squared_mag_prepare);
  RUN_TEST_CASE(vect_complex_squared_mag, vect_complex_s32_squared_mag_prepare);
  RUN_TEST_CASE(vect_complex_squared_mag, vect_complex_s16_squared_mag_basic);
  RUN_TEST_CASE(vect_complex_squared_mag, vect_complex_s16_squared_mag_random);
  RUN_TEST_CASE(vect_complex_squared_mag, vect_complex_s32_squared_mag_basic);
  RUN_TEST_CASE(vect_complex_squared_mag, vect_complex_s32_squared_mag_random);
}

TEST_GROUP(vect_complex_squared_mag);
TEST_SETUP(vect_complex_squared_mag) { fflush(stdout); }
TEST_TEAR_DOWN(vect_complex_squared_mag) {}


static char msg_buff[300];

#define TEST_ASSERT_EQUAL_MSG(EXPECTED, ACTUAL, LINE_NUM)   do{       \
    if((EXPECTED)!=(ACTUAL)) {                                        \
      sprintf(msg_buff, "(test vector @ line %u)", (LINE_NUM));       \
      TEST_ASSERT_EQUAL_MESSAGE((EXPECTED), (ACTUAL), msg_buff);      \
    }} while(0)
    
#define TEST_ASSERT_EQUAL_MSG_FMT(EXPECTED, ACTUAL, FMT, ...)   do{   \
    if((EXPECTED)!=(ACTUAL)) {                                        \
      sprintf(msg_buff, (FMT), __VA_ARGS__);                          \
      TEST_ASSERT_EQUAL_MESSAGE((EXPECTED), (ACTUAL), msg_buff);      \
    }} while(0)

#define TEST_ASSERT_INT32_WITHIN_MSG(THRESHOLD, EXPECTED, ACTUAL, FMT, ...)   do{       \
    if(abs((ACTUAL)-(EXPECTED)) > THRESHOLD) {                                           \
      sprintf(msg_buff, (FMT), __VA_ARGS__);                                            \
      TEST_ASSERT_INT32_WITHIN_MESSAGE((THRESHOLD), (EXPECTED), (ACTUAL), msg_buff);    \
    }} while(0)


static int16_t squared_mag_complex_s16(
    complex_s16_t b,
    right_shift_t sat)
{
    int32_t bp_re = b.re;
    int32_t bp_im = b.im;


    int32_t sqr_mag = bp_re*bp_re + bp_im*bp_im;

    return SAT(16)(ROUND_SHR(sqr_mag,sat));
}


static int32_t squared_mag_complex_s32(
    complex_s32_t b,
    right_shift_t b_shr)
{
    int64_t bp_re = b.re;
    int64_t bp_im = b.im;

    bp_re = SAT(32)(ASHR(32)(bp_re, b_shr));
    bp_im = SAT(32)(ASHR(32)(bp_im, b_shr));

    int64_t p_real = SAT(32)(ROUND_SHR(bp_re*bp_re, 30));
    int64_t p_imag = SAT(32)(ROUND_SHR(bp_im*bp_im, 30));

    return SAT(32)(p_real + p_imag);
}


#define REPS        1000


TEST(vect_complex_squared_mag, vect_complex_s16_squared_mag_prepare)
{
    

    unsigned seed = SEED_FROM_FUNC_NAME();


    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);
        
        exponent_t b_exp = pseudo_rand_int(&seed, -30, 30);
        headroom_t b_hr  = pseudo_rand_uint(&seed, 0, 15);

        int16_t WORD_ALIGNED B_re = INT16_MIN >> b_hr;
        int16_t WORD_ALIGNED B_im = B_re;
        int16_t WORD_ALIGNED A_mag;
            
        exponent_t a_exp;
        right_shift_t sat;

        vect_complex_s16_squared_mag_prepare(&a_exp, &sat, b_exp, b_hr);

        vect_complex_s16_squared_mag(&A_mag, &B_re, &B_im, 1, sat);

        uint32_t acc = ((int32_t)B_re) * B_re + ((int32_t)B_im) * B_im;

        double q = ldexp(B_re, b_exp) * ldexp(B_re, b_exp) + ldexp(B_im, b_exp) * ldexp(B_im, b_exp);
        double p = ldexp(A_mag, a_exp);

        // The only time p != q is if A_mag saturated from 0x8000 to 0x7FFF
        TEST_ASSERT_TRUE( (A_mag == 0x7FFF && 1) || (p == q) );

        if( acc < 0x8000 ){
            // Because sat cannot be negative, if the accumulator value would be less than 0x8000, the result is
            // just the direct squared magnitude.
            
            TEST_ASSERT_EQUAL_HEX16(acc, A_mag);
            TEST_ASSERT_EQUAL(2*b_exp, a_exp);
        } else {
            // Depends on whether saturation is allowed
            TEST_ASSERT_EQUAL( 1? 0x7FFF : 0x4000, A_mag );
        }
        
    }
}
#undef REPS


#define REPS        1000
TEST(vect_complex_squared_mag, vect_complex_s32_squared_mag_prepare)
{
    

    unsigned seed = SEED_FROM_FUNC_NAME();


    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);
        
        exponent_t b_exp = pseudo_rand_int(&seed, -30, 30);
        headroom_t b_hr  = pseudo_rand_uint(&seed, 0, 31);

        complex_s32_t B = { INT32_MIN >> b_hr, INT32_MIN >> b_hr };

        int32_t A;
            
        exponent_t a_exp;
        right_shift_t b_shr;

        vect_complex_s32_squared_mag_prepare(&a_exp, &b_shr, b_exp, b_hr);

        vect_complex_s32_squared_mag(&A, &B, 1, b_shr);

        double q = ldexp(B.re, b_exp) * ldexp(B.re, b_exp) + ldexp(B.im, b_exp) * ldexp(B.im, b_exp);
        double p = ldexp(A, a_exp);


        // The only time p != q is if A saturated from 0x80000000 to 0x7FFFFFFF
        TEST_ASSERT_TRUE( (A == 0x7FFFFFFF && 1) || (p == q) );

        TEST_ASSERT_EQUAL_INT32(0x7FFFFFFF, A);

        //Increasing b_shr by 1 should divide the pre-saturation result by 4
        vect_complex_s32_squared_mag(&A, &B, 1, b_shr+1);

        TEST_ASSERT_EQUAL_INT32(0x20000000, A);
    }
}
#undef REPS


#define THRESHOLD   5

TEST(vect_complex_squared_mag, vect_complex_s16_squared_mag_basic)
{
    

    typedef struct {
        complex_s16_t b;

        right_shift_t sat;

        int16_t expected;
        unsigned line;
    } test_case_t;

    test_case_t casses[] = {
        //{ {    b.re,     b.im},   sat,  expected,      line num
          { {  0x0000,   0x0000},     0,    0x0000,      __LINE__},
          { {  0x0001,   0x0000},     0,    0x0001,      __LINE__},
          { {  0x0000,   0x0001},     0,    0x0001,      __LINE__},
          { { -0x0001,   0x0000},     0,    0x0001,      __LINE__},
          { {  0x0000,  -0x0001},     0,    0x0001,      __LINE__},
          { {  0x0002,   0x0000},     0,    0x0004,      __LINE__},
          { {  0x0000,   0x0002},     0,    0x0004,      __LINE__},
          { { -0x0002,   0x0000},     0,    0x0004,      __LINE__},
          { {  0x0000,  -0x0002},     0,    0x0004,      __LINE__},
          { {  0x0001,   0x0001},     0,    0x0002,      __LINE__},
          { {  0x0001,  -0x0010},     0,    0x0101,      __LINE__},
          { {  0x0080,   0x0010},     0,    0x4100,      __LINE__},
          { {  0x0080,   0x0080},     0,    0x7FFF,      __LINE__},
          
          { {  0x0080,   0x0080},     1,    0x4000,      __LINE__},
          { {  0x0080,   0x0080},     5,    0x0400,      __LINE__},
          { {  0x0010,   0x0010},     0,    0x0200,      __LINE__},
          { {  0x0010,   0x0010},     1,    0x0100,      __LINE__},
    };

    const unsigned N_cases = sizeof(casses)/sizeof(test_case_t);

    const unsigned start_case = 0;

    for(unsigned int v = start_case; v < N_cases; v++){
        // setExtraInfo_R(v);
        
        test_case_t* casse = &casses[v];

        TEST_ASSERT_EQUAL_MSG(casse->expected,
                        squared_mag_complex_s16(casse->b, casse->sat), casse->line);

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

            hr = vect_complex_s16_squared_mag(A, B.real, B.imag, len, casse->sat);

            for(unsigned int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[i], casse->line);
            }

            TEST_ASSERT_EQUAL_MSG(vect_s16_headroom(A, len), hr, casse->line);
        }
    }
}
#undef THRESHOLD


#define MAX_LEN     100
#define REPS        1000
#define THRESHOLD   10

TEST(vect_complex_squared_mag, vect_complex_s16_squared_mag_random)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    headroom_t hr;
    
    int16_t A[MAX_LEN];

    struct {
        int16_t real[MAX_LEN];
        int16_t imag[MAX_LEN];
    } B;

    for(unsigned int v = 0; v < REPS; v++){

        // unsigned rep_seed = seed;

        unsigned len = (pseudo_rand_uint32(&seed) % MAX_LEN) + 1;
        
        headroom_t B_hr = pseudo_rand_uint32(&seed) % 4;

        for(unsigned int i = 0; i < len; i++){
            B.real[i] = pseudo_rand_int16(&seed) >> B_hr;
            B.imag[i] = pseudo_rand_int16(&seed) >> B_hr;
        }

        B_hr = vect_complex_s16_headroom(B.real, B.imag, len);

        //With 0 bits of headroom, the size of the result can be up to (0x8000**2 * 2)
        //  = (2**(15))**2 * 2 = 2**30 * 2 = 2**31
        //  which requires a sat of 16 to prevent saturation. Technically saturation will
        //  occur if the result is exactly 2**31  (2**31 >> 16 = 2**15 = 0x8000 -> 0x7FFF
        // Every bit of headroom decreases that maximum by a factor of 2**2
        right_shift_t sat_min = 16 - (2*B_hr);

        //For the sake of testing, add an extra bit of shift.
        right_shift_t sat = sat_min + (pseudo_rand_uint32(&seed) % 14);
        
        hr = vect_complex_s16_squared_mag(A, B.real, B.imag, len, sat);

        for(unsigned int i = 0; i < len; i++){
            complex_s16_t bbb = {B.real[i], B.imag[i]};
            int16_t expected = squared_mag_complex_s16(bbb, sat);

            TEST_ASSERT_EQUAL_MSG_FMT(expected, A[i],
                  "(test vect %d) (len: %u) (index %d): (mag(%d + i*%d))**2 >> %d",
                   v, len, i, B.real[i], B.imag[i], sat);
        }
        TEST_ASSERT_EQUAL_MSG(vect_s16_headroom(A, len), hr, v);
    }
}
#undef MAX_LEN
#undef REPS
#undef THRESHOLD


#define THRESHOLD   7

TEST(vect_complex_squared_mag, vect_complex_s32_squared_mag_basic)
{
    

    typedef struct {
        complex_s32_t b;
        right_shift_t b_shr;

        int32_t expected;
        unsigned line;
    } test_case_t;

    test_case_t casses[] = {
        //{ {       b.re,       b.im}, b_shr,   expected,      line num
          { { 0x00000000, 0x00000000},     0, 0x00000000,      __LINE__},
          { { 0x00000000, 0x00000000},     1, 0x00000000,      __LINE__},
          { { 0x40000000, 0x00000000},     0, 0x40000000,      __LINE__},
          { {-0x40000000, 0x00000000},     0, 0x40000000,      __LINE__},
          { { 0x00000000, 0x40000000},     0, 0x40000000,      __LINE__},
          { { 0x00000000,-0x40000000},     0, 0x40000000,      __LINE__},
          { { 0x04000000, 0x00000000},     0, 0x00400000,      __LINE__},
          { {-0x04000000, 0x00000000},     0, 0x00400000,      __LINE__},
          { { 0x00000000, 0x04000000},     0, 0x00400000,      __LINE__},
          { { 0x00000000,-0x04000000},     0, 0x00400000,      __LINE__},
          { { 0x40000000, 0x00000000},     4, 0x00400000,      __LINE__},
          { {-0x40000000, 0x00000000},     4, 0x00400000,      __LINE__},
          { { 0x00000000, 0x40000000},     4, 0x00400000,      __LINE__},
          { { 0x00000000,-0x40000000},     4, 0x00400000,      __LINE__},
          { { 0x00001000, 0x00000000},     0, 0x00000000,      __LINE__},
          { { 0x00001000, 0x00000000},   -15, 0x01000000,      __LINE__},
          { { 0x01000000, 0x00000000},     4, 0x00000400,      __LINE__},
          { { 0x00001000, 0x00001000},    -4, 0x00000008,      __LINE__},
          { { 0x00001000, 0x00002000},    -4, 0x00000014,      __LINE__},
    };

    const unsigned N_cases = sizeof(casses)/sizeof(test_case_t);

    const unsigned start_case = 0;

    for(unsigned int v = start_case; v < N_cases; v++){
        // setExtraInfo_R(v);
        
        test_case_t* casse = &casses[v];

        unsigned lengths[] = {1, 4, 16, 32, 34 };

        for( int l = 0; l < sizeof(lengths)/sizeof(lengths[0]); l++){
            unsigned len = lengths[l];
            
            headroom_t hr;
            int32_t DWORD_ALIGNED A[40];
            complex_s32_t DWORD_ALIGNED B[40];

            for(unsigned int i = 0; i < len; i++){
                A[i] = 0xCC;
                B[i].re = casse->b.re;
                B[i].im = casse->b.im;
            }

            hr = vect_complex_s32_squared_mag(A, B, len, casse->b_shr);

            for(unsigned int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG_FMT(casse->expected, A[i],
                  "(test vect %d @ line %u) (len: %u) (index %d): (mag(( %d + i*%d) >> %d))**2",
                   v, casse->line, len, i, (int) B[i].re, (int) B[i].im, casse->b_shr);
            }
            TEST_ASSERT_EQUAL_MSG(vect_s32_headroom((int32_t*) A, len), hr, casse->line);
        }
    }
}
#undef THRESHOLD


#define MAX_LEN     100
#define REPS        1000
#define THRESHOLD   7

TEST(vect_complex_squared_mag, vect_complex_s32_squared_mag_random)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    headroom_t hr;
    
    int32_t DWORD_ALIGNED A[MAX_LEN];
    complex_s32_t DWORD_ALIGNED B[MAX_LEN];

    for(unsigned int v = 0; v < REPS; v++){
        // setExtraInfo_RS(v, seed);

        unsigned len = (pseudo_rand_uint32(&seed) % MAX_LEN) + 1;
        
        headroom_t B_hr = pseudo_rand_uint32(&seed) % 8;

        for(unsigned int i = 0; i < len; i++){
            B[i].re = pseudo_rand_int32(&seed) >> B_hr;
            B[i].im = pseudo_rand_int32(&seed) >> B_hr;
        }
        
        B_hr = vect_complex_s32_headroom(B, len);

        // with 0 bits of headroom, B could be -0x8000000 + 0x8000000j = -(2**31) + -(2**31)j
        // Squared mag would be:   (-(2**31))**2 + (-(2**31))**2 = 2**62 + 2**62 = 2**63
        // But the multiplication shifts down by 30 --> 2**33. But we can't go above 2**30 if
        // we want to avoid saturation. Right-shifting the input by 1 bit will reduce the
        // products by a factor of 4, resulting in  2**60 >> 30 = 2**30.
        // So, ideally we shift to get exactly 1 bit of headroom.

        right_shift_t b_shr = (1-B_hr) + (pseudo_rand_uint32(&seed) % 4);
        
        hr = vect_complex_s32_squared_mag(A, B, len, b_shr);

        for(unsigned int i = 0; i < len; i++){
            int32_t expected = squared_mag_complex_s32(B[i], b_shr);
            TEST_ASSERT_EQUAL_MSG_FMT(expected, A[i],
                  "(test vect %d) (len: %u) (index %d): (mag(( %d + i*%d) >> %d))**2",
                   v, len, i, (int) B[i].re, (int) B[i].im, b_shr);
        }
        TEST_ASSERT_EQUAL_MSG(vect_s32_headroom(A, len), hr, v);
    }
}
#undef MAX_LEN
#undef REPS
#undef THRESHOLD

