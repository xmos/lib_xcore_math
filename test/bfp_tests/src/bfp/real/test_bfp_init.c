// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xmath/xmath.h"

#include "../../tst_common.h"

#include "unity_fixture.h"


TEST_GROUP_RUNNER(bfp_init) {
  RUN_TEST_CASE(bfp_init, bfp_s32_init);
  RUN_TEST_CASE(bfp_init, bfp_s16_init);
  RUN_TEST_CASE(bfp_init, bfp_complex_s32_init);
  RUN_TEST_CASE(bfp_init, bfp_complex_s16_init);
}

TEST_GROUP(bfp_init);
TEST_SETUP(bfp_init) { fflush(stdout); }
TEST_TEAR_DOWN(bfp_init) {}

#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (128)
#else
#  define REPS       (1000)
#  define MAX_LEN    (512)
#endif


TEST(bfp_init, bfp_s16_init) 
{


    unsigned seed = SEED_FROM_FUNC_NAME();
    
    int16_t WORD_ALIGNED data[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        setExtraInfo_R(r);

        unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);

        exponent_t exponent = pseudo_rand_int(&seed, -20, 20);

        int shr = pseudo_rand_uint(&seed, 0, 10);

        for(unsigned int i = 0; i < length; i++){
            data[i] = pseudo_rand_int16(&seed) >> shr;
        }

        headroom_t exp_hr = vect_s16_headroom(data, length);  

        bfp_s16_t A;

        bfp_s16_init(&A, data, exponent, length, 0);

        TEST_ASSERT_EQUAL(data, A.data);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(0, A.hr);

        memset(&A, 0, sizeof(A));

        bfp_s16_init(&A, data, exponent, length, 1);

        TEST_ASSERT_EQUAL(data, A.data);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(exp_hr, A.hr);
    }
}


TEST(bfp_init, bfp_s32_init) 
{


    unsigned seed = SEED_FROM_FUNC_NAME();
    
    int32_t WORD_ALIGNED data[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        setExtraInfo_R(r);

        unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);

        exponent_t exponent = pseudo_rand_int(&seed, -20, 20);

        int shr = pseudo_rand_uint(&seed, 0, 10);

        for(unsigned int i = 0; i < length; i++){
            data[i] = pseudo_rand_int32(&seed) >> shr;
        }

        headroom_t exp_hr = vect_s32_headroom(data, length);

        bfp_s32_t A;

        bfp_s32_init(&A, data, exponent, length, 0);

        TEST_ASSERT_EQUAL(data, A.data);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(0, A.hr);

        memset(&A, 0, sizeof(A));

        bfp_s32_init(&A, data, exponent, length, 1);

        TEST_ASSERT_EQUAL(data, A.data);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(exp_hr, A.hr);
    }
}


TEST(bfp_init, bfp_complex_s16_init) 
{


    unsigned seed = SEED_FROM_FUNC_NAME();
    
    int16_t WORD_ALIGNED real_data[MAX_LEN];
    int16_t WORD_ALIGNED imag_data[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        setExtraInfo_R(r);

        unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);

        exponent_t exponent = pseudo_rand_int(&seed, -20, 20);

        int shr = pseudo_rand_uint(&seed, 0, 10);

        for(unsigned int i = 0; i < length; i++){
            real_data[i] = pseudo_rand_int16(&seed) >> (shr + (pseudo_rand_uint32(&seed) % 4));
            imag_data[i] = pseudo_rand_int16(&seed) >> (shr + (pseudo_rand_uint32(&seed) % 4));
        }

        headroom_t exp_hr_r = vect_s16_headroom(real_data, length);
        headroom_t exp_hr_i = vect_s16_headroom(imag_data, length);

        headroom_t exp_hr = (exp_hr_r < exp_hr_i)? exp_hr_r : exp_hr_i;

        bfp_complex_s16_t A;

        bfp_complex_s16_init(&A, real_data, imag_data, exponent, length, 0);

        TEST_ASSERT_EQUAL(real_data, A.real);
        TEST_ASSERT_EQUAL(imag_data, A.imag);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(0, A.hr);

        memset(&A, 0, sizeof(A));

        bfp_complex_s16_init(&A, real_data, imag_data, exponent, length, 1);

        TEST_ASSERT_EQUAL(real_data, A.real);
        TEST_ASSERT_EQUAL(imag_data, A.imag);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(exp_hr, A.hr);
    }
}


TEST(bfp_init, bfp_complex_s32_init) 
{


    unsigned seed = SEED_FROM_FUNC_NAME();
    
    complex_s32_t WORD_ALIGNED data[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        setExtraInfo_R(r);

        unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);

        exponent_t exponent = pseudo_rand_int(&seed, -20, 20);

        int shr = pseudo_rand_uint(&seed, 0, 10);

        for(unsigned int i = 0; i < length; i++){
            data[i].re = pseudo_rand_int16(&seed) >> (shr + (pseudo_rand_uint32(&seed) % 7));
            data[i].im = pseudo_rand_int16(&seed) >> (shr + (pseudo_rand_uint32(&seed) % 7));
        }

        headroom_t exp_hr = vect_s32_headroom( (int32_t*)data, 2*length);

        bfp_complex_s32_t A;

        bfp_complex_s32_init(&A, data, exponent, length, 0);

        TEST_ASSERT_EQUAL(data, A.data);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(0, A.hr);

        memset(&A, 0, sizeof(A));

        bfp_complex_s32_init(&A, data, exponent, length, 1);

        TEST_ASSERT_EQUAL(data, A.data);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(exp_hr, A.hr);
    }
}