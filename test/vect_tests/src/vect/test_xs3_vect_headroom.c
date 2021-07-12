// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "xs3_math.h"

#include "../tst_common.h"
#include "../src/vect/vpu_helper.h"

#include "unity_fixture.h"

TEST_GROUP_RUNNER(xs3_vect_headroom) {
  RUN_TEST_CASE(xs3_vect_headroom, xs3_vect_s16_headroom);
  RUN_TEST_CASE(xs3_vect_headroom, xs3_vect_s32_headroom);
  RUN_TEST_CASE(xs3_vect_headroom, xs3_vect_ch_pair_s16_headroom);
  RUN_TEST_CASE(xs3_vect_headroom, xs3_vect_ch_pair_s32_headroom);
  RUN_TEST_CASE(xs3_vect_headroom, xs3_vect_complex_s16_headroom);
  RUN_TEST_CASE(xs3_vect_headroom, xs3_vect_complex_s32_headroom);
}

TEST_GROUP(xs3_vect_headroom);
TEST_SETUP(xs3_vect_headroom) {}
TEST_TEAR_DOWN(xs3_vect_headroom) {}


#define MAX_LEN     1024
#define REPS        (1000)


TEST(xs3_vect_headroom, xs3_vect_s16_headroom)
{
    unsigned seed = SEED_FROM_FUNC_NAME();


    int16_t WORD_ALIGNED A[MAX_LEN];

    for(int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        unsigned length = pseudo_rand_uint(&seed, 100, MAX_LEN+1);

        const headroom_t shr = pseudo_rand_uint(&seed, 0, 15);
        
        for(int i = 0; i < length; i++){
            A[i] = pseudo_rand_int16(&seed) >> shr;
        }

        headroom_t hr = xs3_vect_s16_headroom(A, length);

        headroom_t min_hr = INT32_MAX;

        for(int i = 0; i < length; i++){
            min_hr = MIN( min_hr, HR_S16(A[i]) );
        }

        TEST_ASSERT_EQUAL( min_hr, hr );
    }
}


TEST(xs3_vect_headroom, xs3_vect_s32_headroom)
{
    unsigned seed = SEED_FROM_FUNC_NAME();


    int32_t A[MAX_LEN];

    for(int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        const unsigned length = pseudo_rand_uint(&seed, 100, MAX_LEN+1);

        const headroom_t shr = pseudo_rand_uint(&seed, 0, 31);
        
        for(int i = 0; i < length; i++){
            A[i] = pseudo_rand_int32(&seed) >> shr;
        }

        headroom_t hr = xs3_vect_s32_headroom(A, length);

        headroom_t min_hr = INT32_MAX;
        
        for(int i = 0; i < length; i++){
            min_hr = MIN( min_hr, HR_S32(A[i]) );
        }

        TEST_ASSERT_EQUAL( min_hr, hr );
    }
}


TEST(xs3_vect_headroom, xs3_vect_ch_pair_s16_headroom)
{
    unsigned seed = SEED_FROM_FUNC_NAME();


    ch_pair_s16_t WORD_ALIGNED A[MAX_LEN];

    for(int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        const unsigned length = pseudo_rand_uint(&seed, 100, MAX_LEN+1);

        const headroom_t shr = pseudo_rand_uint(&seed, 0, 15);
        
        for(int i = 0; i < length; i++){
            A[i].ch_a = pseudo_rand_int16(&seed) >> shr;
            A[i].ch_b = pseudo_rand_int16(&seed) >> shr;
        }

        headroom_t hr = xs3_vect_ch_pair_s16_headroom(A, length);

        headroom_t min_hr = INT32_MAX;
        
        for(int i = 0; i < length; i++){
            min_hr = MIN( min_hr, HR_S16(A[i].ch_a) );
            min_hr = MIN( min_hr, HR_S16(A[i].ch_b) );
        }

        TEST_ASSERT_EQUAL( min_hr, hr );
    }
}


TEST(xs3_vect_headroom, xs3_vect_ch_pair_s32_headroom)
{
    unsigned seed = SEED_FROM_FUNC_NAME();


    ch_pair_s32_t A[MAX_LEN];

    for(int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        const unsigned length = pseudo_rand_uint(&seed, 100, MAX_LEN+1);

        const headroom_t shr = pseudo_rand_uint(&seed, 0, 31);
        
        for(int i = 0; i < length; i++){
            A[i].ch_a = pseudo_rand_int32(&seed) >> shr;
            A[i].ch_b = pseudo_rand_int32(&seed) >> shr;
        }

        headroom_t hr = xs3_vect_ch_pair_s32_headroom(A, length);

        headroom_t min_hr = INT32_MAX;
        
        for(int i = 0; i < length; i++){
            min_hr = MIN( min_hr, HR_S32(A[i].ch_a) );
            min_hr = MIN( min_hr, HR_S32(A[i].ch_b) );
        }

        TEST_ASSERT_EQUAL( min_hr, hr );
    }
}


TEST(xs3_vect_headroom, xs3_vect_complex_s16_headroom)
{
    unsigned seed = SEED_FROM_FUNC_NAME();


    int16_t WORD_ALIGNED A_real[MAX_LEN];
    int16_t WORD_ALIGNED A_imag[MAX_LEN];

    for(int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        const unsigned length = pseudo_rand_uint(&seed, 100, MAX_LEN+1);

        const headroom_t shr = pseudo_rand_uint(&seed, 0, 15);
        
        for(int i = 0; i < length; i++){
            A_real[i] = pseudo_rand_int16(&seed) >> shr;
            A_imag[i] = pseudo_rand_int16(&seed) >> shr;
        }

        headroom_t hr = xs3_vect_complex_s16_headroom(A_real, A_imag, length);

        headroom_t min_hr = INT32_MAX;
        
        for(int i = 0; i < length; i++){
            min_hr = MIN( min_hr, HR_S16(A_real[i]) );
            min_hr = MIN( min_hr, HR_S16(A_imag[i]) );
        }

        TEST_ASSERT_EQUAL( min_hr, hr );
    }
}


TEST(xs3_vect_headroom, xs3_vect_complex_s32_headroom)
{
    unsigned seed = SEED_FROM_FUNC_NAME();


    complex_s32_t A[MAX_LEN];

    for(int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        const unsigned length = pseudo_rand_uint(&seed, 100, MAX_LEN+1);

        const headroom_t shr = pseudo_rand_uint(&seed, 0, 31);
        
        for(int i = 0; i < length; i++){
            A[i].re = pseudo_rand_int32(&seed) >> shr;
            A[i].im = pseudo_rand_int32(&seed) >> shr;
        }

        headroom_t hr = xs3_vect_complex_s32_headroom(A, length);

        headroom_t min_hr = INT32_MAX;
        
        for(int i = 0; i < length; i++){
            min_hr = MIN( min_hr, HR_S32(A[i].re) );
            min_hr = MIN( min_hr, HR_S32(A[i].im) );
        }

        TEST_ASSERT_EQUAL( min_hr, hr );
    }
}

