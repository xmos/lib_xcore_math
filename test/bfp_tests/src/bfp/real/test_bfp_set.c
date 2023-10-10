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


TEST_GROUP_RUNNER(bfp_set) {
  RUN_TEST_CASE(bfp_set, bfp_s32_set);
  RUN_TEST_CASE(bfp_set, bfp_s16_set);
  RUN_TEST_CASE(bfp_set, bfp_complex_s32_set);
  RUN_TEST_CASE(bfp_set, bfp_complex_s16_set);
}

TEST_GROUP(bfp_set);
TEST_SETUP(bfp_set) { fflush(stdout); }
TEST_TEAR_DOWN(bfp_set) {}

#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (128)
#else
#  define REPS       (1000)
#  define MAX_LEN    (512)
#endif


TEST(bfp_set, bfp_s16_set)
{
    unsigned seed = SEED_FROM_FUNC_NAME();
    
    bfp_s16_t A;

    int16_t WORD_ALIGNED data[MAX_LEN];

    int16_t val;


    for(int r = 0; r < REPS; r++){
        setExtraInfo_R(r);

        memset(data, 0, sizeof(data));

        unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);

        bfp_s16_init(&A, data, 0, length, 0);

        exponent_t exponent = pseudo_rand_int(&seed, -20, 20);

        val = pseudo_rand_int16(&seed) >> (pseudo_rand_uint32(&seed) % 10);

        bfp_s16_set(&A, val, exponent);

        headroom_t exp_hr = HR_S16(val);

        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(exp_hr, A.hr);

        for(unsigned int i = 0; i < length; i++){
            TEST_ASSERT_EQUAL(val, A.data[i]);
        }
        for(int i = length; i < MAX_LEN; i++)
            TEST_ASSERT_EQUAL(0, A.data[i]);
    }
}

TEST(bfp_set, bfp_s32_set)
{
    unsigned seed = SEED_FROM_FUNC_NAME();
    
    bfp_s32_t A;

    int32_t WORD_ALIGNED data[MAX_LEN];

    int32_t val;


    for(int r = 0; r < REPS; r++){
        setExtraInfo_R(r);

        memset(data, 0, sizeof(data));

        unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);

        bfp_s32_init(&A, data, 0, length, 0);

        exponent_t exponent = pseudo_rand_int(&seed, -20, 20);

        val = pseudo_rand_int32(&seed) >> (pseudo_rand_uint32(&seed) % 10);

        bfp_s32_set(&A, val, exponent);

        headroom_t exp_hr = HR_S32(val);

        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(exp_hr, A.hr);

        for(unsigned int i = 0; i < length; i++)
            TEST_ASSERT_EQUAL(val, A.data[i]);
        for(int i = length; i < MAX_LEN; i++)
            TEST_ASSERT_EQUAL(0, A.data[i]);
    }
}


TEST(bfp_set, bfp_complex_s16_set)
{
    unsigned seed = SEED_FROM_FUNC_NAME();
    
    bfp_complex_s16_t A;

    int16_t WORD_ALIGNED real_data[MAX_LEN];
    int16_t WORD_ALIGNED imag_data[MAX_LEN];

    complex_s16_t val;


    for(int r = 0; r < REPS; r++){
        setExtraInfo_R(r);

        memset(real_data, 0, sizeof(real_data));
        memset(imag_data, 0, sizeof(imag_data));

        unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);

        bfp_complex_s16_init(&A, real_data, imag_data, 0, length, 0);

        exponent_t exponent = pseudo_rand_int(&seed, -20, 20);

        val.re = pseudo_rand_int16(&seed) >> (pseudo_rand_uint32(&seed) % 10);
        val.im = pseudo_rand_int16(&seed) >> (pseudo_rand_uint32(&seed) % 10);

        bfp_complex_s16_set(&A, val, exponent);

        headroom_t exp_hr = HR_S16(val.re) < HR_S16(val.im)?
                            HR_S16(val.re) : HR_S16(val.im);

        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(exp_hr, A.hr);

        for(unsigned int i = 0; i < length; i++){
            TEST_ASSERT_EQUAL(val.re, A.real[i]);
            TEST_ASSERT_EQUAL(val.im, A.imag[i]);
        }
        for(int i = length; i < MAX_LEN; i++){
            TEST_ASSERT_EQUAL(0, A.real[i]);
            TEST_ASSERT_EQUAL(0, A.imag[i]);
        }
    }
}


TEST(bfp_set, bfp_complex_s32_set)
{
    unsigned seed = SEED_FROM_FUNC_NAME();
    
    bfp_complex_s32_t A;

    complex_s32_t WORD_ALIGNED data[MAX_LEN];

    complex_s32_t val;


    for(int r = 0; r < REPS; r++){
        setExtraInfo_R(r);

        memset(data, 0, sizeof(data));

        unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);

        bfp_complex_s32_init(&A, data, 0, length, 0);

        exponent_t exponent = pseudo_rand_int(&seed, -20, 20);

        val.re = pseudo_rand_int32(&seed) >> (pseudo_rand_uint32(&seed) % 10);
        val.im = pseudo_rand_int32(&seed) >> (pseudo_rand_uint32(&seed) % 10);

        bfp_complex_s32_set(&A, val, exponent);

        headroom_t exp_hr = HR_S32(val.re) < HR_S32(val.im)?
                            HR_S32(val.re) : HR_S32(val.im);

        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(exp_hr, A.hr);

        for(unsigned int i = 0; i < length; i++){
            TEST_ASSERT_EQUAL(val.re, A.data[i].re);
            TEST_ASSERT_EQUAL(val.im, A.data[i].im);
        }
        for(int i = length; i < MAX_LEN; i++){
            TEST_ASSERT_EQUAL(0, A.data[i].re);
            TEST_ASSERT_EQUAL(0, A.data[i].im);
        }
    }
}