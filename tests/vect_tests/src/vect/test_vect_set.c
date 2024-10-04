// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xmath/xmath.h"

#include "../tst_common.h"

#include "unity_fixture.h"


TEST_GROUP_RUNNER(vect_set) {
  RUN_TEST_CASE(vect_set, vect_s16_set);
  RUN_TEST_CASE(vect_set, vect_s32_set);
  RUN_TEST_CASE(vect_set, vect_complex_s16_set);
  RUN_TEST_CASE(vect_set, vect_complex_s32_set);
}

TEST_GROUP(vect_set);
TEST_SETUP(vect_set) { fflush(stdout); }
TEST_TEAR_DOWN(vect_set) {}


#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (128)
#else
#  define REPS       (1000)
#  define MAX_LEN    (1024)
#endif


TEST(vect_set, vect_s16_set)
{
    

    unsigned seed = SEED_FROM_FUNC_NAME();


    int16_t WORD_ALIGNED A[MAX_LEN];

    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        
        memset(A, 0xCC, sizeof(A));

        const int16_t new_val = pseudo_rand_int16(&seed);
        
        vect_s16_set(A, new_val, length);

        for(unsigned int i = 0; i < length; i++)
            TEST_ASSERT_EQUAL_INT16(new_val, A[i]);
        for(int i = length; i < MAX_LEN; i++)
            TEST_ASSERT_EQUAL_INT16(0xCCCC, A[i]);
        
    }
}


TEST(vect_set, vect_s32_set)
{
    

    unsigned seed = SEED_FROM_FUNC_NAME();


    int32_t WORD_ALIGNED A[MAX_LEN];

    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        
        memset(A, 0xCC, sizeof(A));

        const int32_t new_val = pseudo_rand_int32(&seed);
        
        vect_s32_set(A, new_val, length);

        for(unsigned int i = 0; i < length; i++)
            TEST_ASSERT_EQUAL_INT32(new_val, A[i]);
        for(int i = length; i < MAX_LEN; i++)
            TEST_ASSERT_EQUAL_INT32(0xCCCCCCCC, A[i]);
        
    }
}


TEST(vect_set, vect_complex_s16_set)
{
    

    unsigned seed = SEED_FROM_FUNC_NAME();


    int16_t WORD_ALIGNED A_real[MAX_LEN];
    int16_t WORD_ALIGNED A_imag[MAX_LEN];

    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        
        memset(A_real, 0xCC, sizeof(A_real));
        memset(A_imag, 0xCC, sizeof(A_imag));

        const int16_t new_real = pseudo_rand_int16(&seed);
        const int16_t new_imag = pseudo_rand_int16(&seed);
        
        vect_complex_s16_set(A_real, A_imag, new_real, new_imag, length);

        for(unsigned int i = 0; i < length; i++){
            TEST_ASSERT_EQUAL_INT16(new_real, A_real[i]);
            TEST_ASSERT_EQUAL_INT16(new_imag, A_imag[i]);
        }
        for(int i = length; i < MAX_LEN; i++){
            TEST_ASSERT_EQUAL_INT16(0xCCCC, A_real[i]);
            TEST_ASSERT_EQUAL_INT16(0xCCCC, A_imag[i]);
        }
        
    }
}


TEST(vect_set, vect_complex_s32_set)
{
    

    unsigned seed = SEED_FROM_FUNC_NAME();


    complex_s32_t WORD_ALIGNED A[MAX_LEN];

    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        
        memset(A, 0xCC, sizeof(A));

        const int32_t new_real = pseudo_rand_int32(&seed);
        const int32_t new_imag = pseudo_rand_int32(&seed);
        
        vect_complex_s32_set(A, new_real, new_imag, length);

        for(unsigned int i = 0; i < length; i++){
            TEST_ASSERT_EQUAL_INT32(new_real, A[i].re);
            TEST_ASSERT_EQUAL_INT32(new_imag, A[i].im);
        }
        for(int i = length; i < MAX_LEN; i++){
            TEST_ASSERT_EQUAL_INT32(0xCCCCCCCC, A[i].re);
            TEST_ASSERT_EQUAL_INT32(0xCCCCCCCC, A[i].im);
        }
        
    }
}