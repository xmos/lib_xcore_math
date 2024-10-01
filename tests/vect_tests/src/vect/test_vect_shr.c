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
#include "../src/vect/vpu_helper.h"

#include "unity_fixture.h"

TEST_GROUP_RUNNER(vect_shr) {
  RUN_TEST_CASE(vect_shr, vect_s16_shr_basic);
  RUN_TEST_CASE(vect_shr, vect_s16_shr_random);
  RUN_TEST_CASE(vect_shr, vect_s32_shr_basic);
  RUN_TEST_CASE(vect_shr, vect_s32_shr_random);
  RUN_TEST_CASE(vect_shr, vect_complex_s16_shr);
  RUN_TEST_CASE(vect_shr, vect_complex_s32_shr);
  RUN_TEST_CASE(vect_shr, vect_split_acc_s32_shr);
}

TEST_GROUP(vect_shr);
TEST_SETUP(vect_shr) { fflush(stdout); }
TEST_TEAR_DOWN(vect_shr) {}


static char msg_buff[200];

#define TEST_ASSERT_EQUAL_MSG(EXPECTED, ACTUAL, LINE_NUM)   do{       \
    if((EXPECTED)!=(ACTUAL)) {                                        \
      sprintf(msg_buff, "(test vector @ line %u)", (LINE_NUM));       \
      TEST_ASSERT_EQUAL_MESSAGE((EXPECTED), (ACTUAL), msg_buff);      \
    }} while(0)


#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (128)
#else
#  define REPS       (1000)
#  define MAX_LEN    (512)
#endif


TEST(vect_shr, vect_s16_shr_basic)
{
    

    typedef struct {
        int16_t value;
        int shr;
        int16_t expected;
        unsigned line;
    } test_case_t;

    test_case_t cases[] = {
        //    value     shr     expected    line #
        {        1,      0,           1,    __LINE__},
        {        0,      0,           0,    __LINE__},
        {       -1,      0,          -1,    __LINE__},
        {        2,      0,           2,    __LINE__},
        {        3,      0,           3,    __LINE__},
        {       -2,      0,          -2,    __LINE__},
        {       -3,      0,          -3,    __LINE__},
        {   0x00FF,     -0,      0x00FF,    __LINE__},
        {        1,     -1,           2,    __LINE__},
        {        1,     -2,           4,    __LINE__},
        {        1,     -3,           8,    __LINE__},
        {        1,      1,           0,    __LINE__},
        {        1,      2,           0,    __LINE__},
        {   0x07F0,     -1,      0x0FE0,    __LINE__},
        {   0x07F0,      1,      0x03F8,    __LINE__},
        {   0x07F0,     -2,      0x1FC0,    __LINE__},
        {  -0x0008,     -1,     -0x0010,    __LINE__},
        {  -0x0008,      1,     -0x0004,    __LINE__},
        {  -0x0008,      2,     -0x0002,    __LINE__},
        {  -0x0008,      3,     -0x0001,    __LINE__},
        {  -0x0008,      4,     -0x0001,    __LINE__},
        {   0x1000,     -3,      0x7FFF,    __LINE__},
        {  -0x1000,     -3,     -0x7FFF,    __LINE__},
        {  -0x8000,      0,     -0x7FFF,    __LINE__},
    };

    const unsigned N_cases = sizeof(cases)/sizeof(test_case_t);

    const unsigned start_case = 0;

    for(unsigned int v = start_case; v < N_cases; v++){
        setExtraInfo_R(v);
        
        test_case_t* casse = &cases[v];

        int16_t B[1] = { casse->value };
        int16_t A[1] = { 0xCC };

        headroom_t hr = vect_s16_shr(A, B, 1, casse->shr);

        TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
        TEST_ASSERT_EQUAL_MSG(vect_s16_headroom(A, 1), hr, casse->line);

        hr = vect_s16_shr(B, B, 1, casse->shr);

        TEST_ASSERT_EQUAL_MSG(casse->expected, B[0], casse->line);
        TEST_ASSERT_EQUAL_MSG(vect_s16_headroom(B, 1), hr, casse->line);
    }
}


#define LEN     68

TEST(vect_shr, vect_s16_shr_random)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    for(unsigned int v = 0; v < REPS; v++){

        setExtraInfo_R(v);
        
        int16_t A[LEN];
        int16_t B[LEN];

        for(int i = 0; i < LEN; i++){
            //without shr, the headroom would always be zero.
            unsigned shr = pseudo_rand_uint32(&seed) % 8;
            B[i] = pseudo_rand_int16(&seed) >> shr;
        }

        // shift left between -3 and 3 bits
        int shr = (pseudo_rand_int32(&seed) % 7) - 3;

        for(int in_place = 0; in_place < 2; in_place++){

            //printf("\t\t%s..\n", in_place? "in-place" : "not in-place");

            if(in_place)
                memcpy(A, B, sizeof(A));

            headroom_t hr = vect_s16_shr(A, (in_place)? A : B, LEN, shr);

            for(int i = 0; i < LEN; i++){
                int32_t b = B[i];
                int32_t exp;

                if(shr < 0){
                    exp = b << -shr;
                    exp = (exp > VPU_INT16_MAX)? VPU_INT16_MAX : exp;
                    exp = (exp < VPU_INT16_MIN)? VPU_INT16_MIN : exp;
                } else {
                    exp = b >> shr;
                    exp = (b >= 0)? exp : (exp == 0)? -1 : exp;
                }
                
                if(((int16_t)exp) != A[i]){
                    sprintf(msg_buff, "rep(%d)[%d]: %d <-- (%d >> %d)     (A[i]=0x%04X; B[i]=0x%04X)\n",v, i, A[i], B[i], shr,  (uint16_t)A[i],  (uint16_t)B[i]);
                }
                TEST_ASSERT_EQUAL_MESSAGE((int16_t)exp, A[i], msg_buff);
            }

            TEST_ASSERT_EQUAL(vect_s16_headroom(A, LEN), hr);
        }
        
    }
}
#undef LEN


TEST(vect_shr, vect_s32_shr_basic)
{
    

    typedef struct {
        int32_t value;
        int shr;
        int32_t expected;
        unsigned line;
    } test_case_t;

    test_case_t cases[] = {
        //                value       shr         expected       line #
        {                     1,        0,               1,    __LINE__},
        {                     0,        0,               0,    __LINE__},
        {                    -1,        0,              -1,    __LINE__},
        {                     2,        0,               2,    __LINE__},
        {                     3,        0,               3,    __LINE__},
        {                    -2,        0,              -2,    __LINE__},
        {                    -3,        0,              -3,    __LINE__},
        {            0x00FF0000,        0,      0x00FF0000,    __LINE__},
        {                     1,       -1,               2,    __LINE__},
        {                     1,       -2,               4,    __LINE__},
        {                     1,       -3,               8,    __LINE__},
        {                     1,        1,               0,    __LINE__},
        {                     1,        2,               0,    __LINE__},
        {            0x07F00000,       -1,      0x0FE00000,    __LINE__},
        {            0x07F00000,        1,      0x03F80000,    __LINE__},
        {            0x07F00000,       -2,      0x1FC00000,    __LINE__},
        {           -0x00080000,       -1,     -0x00100000,    __LINE__},
        {           -0x00080000,        1,     -0x00040000,    __LINE__},
        {           -0x00080000,        2,     -0x00020000,    __LINE__},
        {           -0x00080000,        3,     -0x00010000,    __LINE__},
        {           -0x00080000,        4,     -0x00008000,    __LINE__},
        {            0x10000000,       -3,      0x7FFFFFFF,    __LINE__},
        {           -0x10000000,       -3,     -0x7FFFFFFF,    __LINE__},
        {  (int) (0-0x80000000),        0,     -0x7FFFFFFF,    __LINE__},
    };

    const unsigned N_cases = sizeof(cases)/sizeof(test_case_t);

    const unsigned start_case = 0;

    for(unsigned int v = start_case; v < N_cases; v++){
        setExtraInfo_R(v);
        
        test_case_t* casse = &cases[v];

        int32_t B[1] = { casse->value };
        int32_t A[1] = { 0xCC };

        headroom_t hr = vect_s32_shr(A, B, 1, casse->shr);

        TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
        TEST_ASSERT_EQUAL_MSG(vect_s32_headroom(A, 1), hr, casse->line);

        hr = vect_s32_shr(B, B, 1, casse->shr);

        TEST_ASSERT_EQUAL_MSG(casse->expected, B[0], casse->line);
        TEST_ASSERT_EQUAL_MSG(vect_s32_headroom(B, 1), hr, casse->line);
    }
}


#define LEN     68

TEST(vect_shr, vect_s32_shr_random)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    for(unsigned int v = 0; v < REPS; v++){

        setExtraInfo_R(v);
        
        int32_t A[LEN];
        int32_t B[LEN];

        for(int i = 0; i < LEN; i++){
            //without shr, the headroom would always be zero.
            unsigned shr = pseudo_rand_uint32(&seed) % 8;
            B[i] = pseudo_rand_int32(&seed) >> shr;
        }

        // shift left between -3 and 3 bits
        int shr = (pseudo_rand_int32(&seed) % 7) - 3;

        for(int in_place = 0; in_place < 2; in_place++){

            //printf("\t\t%s..\n", in_place? "in-place" : "not in-place");

            if(in_place)
                memcpy(A, B, sizeof(A));

            headroom_t hr = vect_s32_shr(A, (in_place)? A : B, LEN, shr);

            for(int i = 0; i < LEN; i++){
                int64_t b = B[i];
                int64_t exp;

                if(shr < 0){
                    exp = b << -shr;
                    exp = (exp > VPU_INT32_MAX)? VPU_INT32_MAX : exp;
                    exp = (exp < VPU_INT32_MIN)? VPU_INT32_MIN : exp;
                } else {
                    exp = b >> shr;
                    exp = (b >= 0)? exp : (exp == 0)? -1 : exp;
                }
                
                if(((int32_t)exp) != A[i]){
                    sprintf(msg_buff, "rep(%d)[%d]: %ld <-- (%ld >> %d)     (A[i]=0x%04X; B[i]=0x%04X)\n",v, i, (long int) A[i], (long int) B[i], shr,  (unsigned)A[i],  (unsigned)B[i]);
                }
                TEST_ASSERT_EQUAL_MESSAGE((int32_t)exp, A[i], msg_buff);
            }

            TEST_ASSERT_EQUAL(vect_s32_headroom(A, LEN), hr);
        }
    }
}
#undef LEN


TEST(vect_shr, vect_complex_s16_shr)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    int16_t WORD_ALIGNED A_real[MAX_LEN];
    int16_t WORD_ALIGNED A_imag[MAX_LEN];
    int16_t WORD_ALIGNED B_real[MAX_LEN];
    int16_t WORD_ALIGNED B_imag[MAX_LEN];

    for(unsigned int v = 0; v < REPS; v++){

        setExtraInfo_R(v);

        const unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);

        const headroom_t hr = pseudo_rand_uint(&seed, 0, 12);

        const left_shift_t shr = pseudo_rand_int(&seed, -4, 4);
        
        for(unsigned int i = 0; i < length; i++){
            B_real[i] = pseudo_rand_int16(&seed) >> hr;
            B_imag[i] = pseudo_rand_int16(&seed) >> hr;
        }

        vect_complex_s16_shr(A_real, A_imag, B_real, B_imag, length, shr);

        for(unsigned int i = 0; i < length; i++){
            TEST_ASSERT_EQUAL_HEX16( ASHR16(B_real[i], shr), A_real[i] );
            TEST_ASSERT_EQUAL_HEX16( ASHR16(B_imag[i], shr), A_imag[i] );
        }

        memcpy(A_real, B_real, sizeof(A_real));
        memcpy(A_imag, B_imag, sizeof(A_imag));

        vect_complex_s16_shr(A_real, A_imag, A_real, A_imag, length, shr);

        for(unsigned int i = 0; i < length; i++){
            TEST_ASSERT_EQUAL_HEX16( ASHR16(B_real[i], shr), A_real[i] );
            TEST_ASSERT_EQUAL_HEX16( ASHR16(B_imag[i], shr), A_imag[i] );
        }
    }
}


TEST(vect_shr, vect_complex_s32_shr)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    complex_s32_t A[MAX_LEN];
    complex_s32_t B[MAX_LEN];

    for(unsigned int v = 0; v < REPS; v++){

        setExtraInfo_R(v);

        const unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);

        const headroom_t hr = pseudo_rand_uint(&seed, 0, 28);

        const left_shift_t shr = pseudo_rand_int(&seed, -8, 8);
        
        for(unsigned int i = 0; i < length; i++){
            B[i].re = pseudo_rand_int32(&seed) >> hr;
            B[i].im = pseudo_rand_int32(&seed) >> hr;
        }

        vect_complex_s32_shr(A, B, length, shr);

        for(unsigned int i = 0; i < length; i++){
            TEST_ASSERT_EQUAL_HEX32( ASHR32(B[i].re, shr), A[i].re );
            TEST_ASSERT_EQUAL_HEX32( ASHR32(B[i].im, shr), A[i].im );
        }

        memcpy(A, B, sizeof(B));

        vect_complex_s32_shr(A, A, length, shr);
        
        for(unsigned int i = 0; i < length; i++){
            TEST_ASSERT_EQUAL_HEX32( ASHR32(B[i].re, shr), A[i].re );
            TEST_ASSERT_EQUAL_HEX32( ASHR32(B[i].im, shr), A[i].im );
        }
    }
}



TEST(vect_shr, vect_split_acc_s32_shr)
{
    
  unsigned seed = SEED_FROM_FUNC_NAME();

  int32_t DWORD_ALIGNED acc_s32[MAX_LEN];
  int32_t expected[MAX_LEN];
  split_acc_s32_t DWORD_ALIGNED acc[(MAX_LEN + (VPU_INT16_EPV-1)) / VPU_INT16_EPV];

  for(unsigned int v = 0; v < REPS; v++){

    setExtraInfo_R(v);

    const unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);

    headroom_t hr = pseudo_rand_uint(&seed, 0, 8);
    
    for(unsigned int i = 0; i < length; i++){
      acc_s32[i] = pseudo_rand_int32(&seed) >> hr;
    }

    vect_s32_split_accs(acc, acc_s32, length);

    hr = vect_s32_headroom(acc_s32, length);
    
    const right_shift_t shr = pseudo_rand_int(&seed, -(int)hr, (20-hr));

    for(unsigned int k = 0; k < length; k++){
      if(shr >= 0)
        expected[k] = acc_s32[k] >> shr;
      else
        expected[k] = acc_s32[k] << (-shr);
    }

    vect_split_acc_s32_shr(acc, length, shr);

    vect_s32_merge_accs(acc_s32, acc, length);

    TEST_ASSERT_EQUAL_INT32_ARRAY(expected, acc_s32, length);
  }
}


