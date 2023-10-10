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

#include "unity_fixture.h"


TEST_GROUP_RUNNER(vect_complex_s16_to_vect_complex_s32) {
  RUN_TEST_CASE(vect_complex_s16_to_vect_complex_s32, vect_complex_s16_to_vect_complex_s32_basic);
  RUN_TEST_CASE(vect_complex_s16_to_vect_complex_s32, vect_complex_s16_to_vect_complex_s32_random);
}

TEST_GROUP(vect_complex_s16_to_vect_complex_s32);
TEST_SETUP(vect_complex_s16_to_vect_complex_s32) { fflush(stdout); }
TEST_TEAR_DOWN(vect_complex_s16_to_vect_complex_s32) {}

static char msg_buff[200];

#define TEST_ASSERT_EQUAL_MSG(EXPECTED, ACTUAL, LINE_NUM)   do{       \
    if((EXPECTED)!=(ACTUAL)) {                                        \
      sprintf(msg_buff, "(test vector @ line %u)", (LINE_NUM));       \
      TEST_ASSERT_EQUAL_MESSAGE((EXPECTED), (ACTUAL), msg_buff);      \
    }} while(0)


#define MAX_LEN 50


TEST(vect_complex_s16_to_vect_complex_s32, vect_complex_s16_to_vect_complex_s32_basic)
{
    

    typedef struct {
        complex_s16_t b;
        unsigned line;
    } test_case_t;

    test_case_t casses[] = {
        // b{      re,      im },       line #
        {   {  0x0000,  0x0000 },     __LINE__},
        {   {  0x0001,  0x0000 },     __LINE__},
        {   {  0x0100,  0x0000 },     __LINE__},
        {   {  0x0080,  0x0000 },     __LINE__},
        {   {  0x8000,  0x0000 },     __LINE__},
        {   {  0x7FFF,  0x0000 },     __LINE__},
        {   {  0x8000,  0x0000 },     __LINE__},
        {   {  0x0080,  0x0000 },     __LINE__},
        {   {  0x8080,  0x0000 },     __LINE__},
        {   {  0x00FF,  0x0000 },     __LINE__},
        {   {  0xFF00,  0x0000 },     __LINE__},
    };

    const unsigned N_cases = sizeof(casses)/sizeof(test_case_t);

    const unsigned start_case = 0;

    for(unsigned int v = start_case; v < N_cases; v++){
        setExtraInfo_R(v);
        
        test_case_t* casse = &casses[v];

        unsigned lengths[] = {1, 4, 16, 32, 40 };
        complex_s32_t DWORD_ALIGNED A[MAX_LEN];
        struct {
            int16_t real[MAX_LEN];
            int16_t imag[MAX_LEN];
        } B;

        for(int i = 0; i < sizeof(lengths)/sizeof(lengths[0]); i++){
            unsigned len = lengths[i];

            for(unsigned int i = 0; i < MAX_LEN; i++){
                B.real[i] = (i < len)? casse->b.re : 0xBBBB;
                B.imag[i] = (i < len)? casse->b.im : 0xBBBB;
                A[i].re = 0xCCCCCCCC;
                A[i].im = 0xCCCCCCCC;
            }

            vect_complex_s16_to_vect_complex_s32(A, B.real, B.imag, len);

            for(unsigned int k = 0; k < MAX_LEN; k++){
                complex_s32_t exp = {
                    (k < len)? ((int32_t)casse->b.re) : 0xCCCCCCCC,
                    (k < len)? ((int32_t)casse->b.im) : 0xCCCCCCCC  };
                TEST_ASSERT_EQUAL_MSG(exp.re, A[k].re, casse->line);
                TEST_ASSERT_EQUAL_MSG(exp.im, A[k].im, casse->line);
            }

        }
    }
}
#undef MAX_LEN


#define MAX_LEN     68
#define REPS        (100)

TEST(vect_complex_s16_to_vect_complex_s32, vect_complex_s16_to_vect_complex_s32_random)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();

    
    complex_s32_t DWORD_ALIGNED A[MAX_LEN];
    struct {
        int16_t real[MAX_LEN];
        int16_t imag[MAX_LEN];
     } B;

    for(unsigned int v = 0; v < REPS; v++){

        setExtraInfo_R(v);

        const unsigned len = pseudo_rand_uint32(&seed) % MAX_LEN + 1;

        for(unsigned int i = 0; i < len; i++){
            unsigned shr = pseudo_rand_uint32(&seed) % 10;
            B.real[i] = pseudo_rand_int16(&seed) >> shr;
            B.imag[i] = pseudo_rand_int16(&seed) >> shr;
        }

        memset(A, 0xCC, sizeof(A));
        vect_complex_s16_to_vect_complex_s32(A, B.real, B.imag, len);

        for(unsigned int k = 0; k < MAX_LEN; k++){
                complex_s32_t exp = {
                    (k < len)? ((int32_t)B.real[k]) : 0xCCCCCCCC,
                    (k < len)? ((int32_t)B.imag[k]) : 0xCCCCCCCC  };

            TEST_ASSERT_EQUAL_MSG(exp.re, A[k].re, v);
            TEST_ASSERT_EQUAL_MSG(exp.im, A[k].im, v);
        }
    }
}
#undef MAX_LEN
#undef REPS

