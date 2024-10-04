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


TEST_GROUP_RUNNER(bfp_headroom) {
  RUN_TEST_CASE(bfp_headroom, bfp_s32_headroom);
  RUN_TEST_CASE(bfp_headroom, bfp_s16_headroom);
  RUN_TEST_CASE(bfp_headroom, bfp_complex_s32_headroom);
  RUN_TEST_CASE(bfp_headroom, bfp_complex_s16_headroom);
}

TEST_GROUP(bfp_headroom);
TEST_SETUP(bfp_headroom) { fflush(stdout); }
TEST_TEAR_DOWN(bfp_headroom) {}

#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (128)
#else
#  define REPS       (1000)
#  define MAX_LEN    (512)
#endif


TEST(bfp_headroom, bfp_s16_headroom)
{


    unsigned seed = SEED_FROM_FUNC_NAME();
    
    int16_t WORD_ALIGNED data[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        setExtraInfo_R(r);

        unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        exponent_t exponent = pseudo_rand_int(&seed, -20, 20);
        int shr = pseudo_rand_uint32(&seed) % 10;

        for(unsigned int i = 0; i < length; i++)
            data[i] = pseudo_rand_int16(&seed) >> shr;
        
        headroom_t exp_hr = vect_s16_headroom(data, length);

        bfp_s16_t A;

        bfp_s16_init(&A, data, exponent, length, 0);

        TEST_ASSERT_EQUAL(data, A.data);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(0, A.hr);

        headroom_t got_hr = bfp_s16_headroom(&A);

        TEST_ASSERT_EQUAL(data, A.data);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(exp_hr, A.hr);
        TEST_ASSERT_EQUAL(exp_hr, got_hr);
    }
}


TEST(bfp_headroom, bfp_s32_headroom)
{


    unsigned seed = SEED_FROM_FUNC_NAME();
    
    int32_t WORD_ALIGNED data[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        setExtraInfo_R(r);

        unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        exponent_t exponent = pseudo_rand_int(&seed, -20, 20);
        int shr = pseudo_rand_uint32(&seed) % 10;

        for(unsigned int i = 0; i < length; i++)
            data[i] = pseudo_rand_int32(&seed) >> shr;
        
        headroom_t exp_hr = vect_s32_headroom(data, length);

        bfp_s32_t A;

        bfp_s32_init(&A, data, exponent, length, 0);

        TEST_ASSERT_EQUAL(data, A.data);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(0, A.hr);

        headroom_t got_hr = bfp_s32_headroom(&A);

        TEST_ASSERT_EQUAL(data, A.data);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(exp_hr, A.hr);
        TEST_ASSERT_EQUAL(exp_hr, got_hr);
    }
}


TEST(bfp_headroom, bfp_complex_s16_headroom)
{


    unsigned seed = SEED_FROM_FUNC_NAME();
    
    int16_t WORD_ALIGNED real[MAX_LEN];
    int16_t WORD_ALIGNED imag[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        setExtraInfo_R(r);

        unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        exponent_t exponent = pseudo_rand_int(&seed, -20, 20);
        int shr = pseudo_rand_uint32(&seed) % 10;

        for(unsigned int i = 0; i < length; i++){
            real[i] = pseudo_rand_int16(&seed) >> shr;
            imag[i] = pseudo_rand_int16(&seed) >> shr;
        }
        
        headroom_t exp_hr_a = vect_s16_headroom(real, length);
        headroom_t exp_hr_b = vect_s16_headroom(imag, length);
        headroom_t exp_hr = exp_hr_a < exp_hr_b?
                            exp_hr_a : exp_hr_b;

        bfp_complex_s16_t A;

        bfp_complex_s16_init(&A, real, imag, exponent, length, 0);

        TEST_ASSERT_EQUAL(real, A.real);
        TEST_ASSERT_EQUAL(imag, A.imag);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(0, A.hr);

        headroom_t got_hr = bfp_complex_s16_headroom(&A);

        TEST_ASSERT_EQUAL(real, A.real);
        TEST_ASSERT_EQUAL(imag, A.imag);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(exp_hr, A.hr);
        TEST_ASSERT_EQUAL(exp_hr, got_hr);
    }
}


TEST(bfp_headroom, bfp_complex_s32_headroom)
{


    unsigned seed = SEED_FROM_FUNC_NAME();
    
    complex_s32_t WORD_ALIGNED data[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        setExtraInfo_R(r);

        unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        exponent_t exponent = pseudo_rand_int(&seed, -20, 20);
        int shr = pseudo_rand_uint32(&seed) % 10;

        for(unsigned int i = 0; i < length; i++){
            data[i].re = pseudo_rand_int32(&seed) >> shr;
            data[i].im = pseudo_rand_int32(&seed) >> shr;
        }
        
        headroom_t exp_hr = vect_s32_headroom((int32_t*)data, length*2);

        bfp_complex_s32_t A;

        bfp_complex_s32_init(&A, data, exponent, length, 0);

        TEST_ASSERT_EQUAL(data, A.data);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(0, A.hr);

        headroom_t got_hr = bfp_complex_s32_headroom(&A);

        TEST_ASSERT_EQUAL(data, A.data);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(exp_hr, A.hr);
        TEST_ASSERT_EQUAL(exp_hr, got_hr);
    }
}