// Copyright 2021 XMOS LIMITED. This Software is subject to the terms of the 
// XMOS Public License: Version 1

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "xs3_math.h"

#include "../tst_common.h"
#include "../src/vect/vpu_helper.h"

#include "unity.h"

static unsigned seed = 2314567;

#if DEBUG_ON || 0
#undef DEBUG_ON
#define DEBUG_ON    (1)
#endif

#define MAX_LEN     1024
#define REPS        (1000)


static void test_xs3_vect_s16_headroom()
{
    seed = 0x5F0BE930;

    int16_t WORD_ALIGNED A[MAX_LEN];

    for(int v = 0; v < REPS; v++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", v, seed);

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




static void test_xs3_vect_s32_headroom()
{
    seed = 567457;

    int32_t A[MAX_LEN];

    for(int v = 0; v < REPS; v++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", v, seed);

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


static void test_xs3_vect_ch_pair_s16_headroom()
{
    seed = 362546234;

    ch_pair_s16_t WORD_ALIGNED A[MAX_LEN];

    for(int v = 0; v < REPS; v++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", v, seed);

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




static void test_xs3_vect_ch_pair_s32_headroom()
{
    seed = 567457;

    ch_pair_s32_t A[MAX_LEN];

    for(int v = 0; v < REPS; v++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", v, seed);

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



static void test_xs3_vect_complex_s16_headroom()
{
    seed = 3;

    int16_t WORD_ALIGNED A_real[MAX_LEN];
    int16_t WORD_ALIGNED A_imag[MAX_LEN];

    for(int v = 0; v < REPS; v++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", v, seed);

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




static void test_xs3_vect_complex_s32_headroom()
{
    seed = 786;

    complex_s32_t A[MAX_LEN];

    for(int v = 0; v < REPS; v++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", v, seed);

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




void test_xs3_headroom_vect()
{
    SET_TEST_FILE();

    RUN_TEST(test_xs3_vect_s16_headroom);
    RUN_TEST(test_xs3_vect_s32_headroom);

    RUN_TEST(test_xs3_vect_ch_pair_s16_headroom);
    RUN_TEST(test_xs3_vect_ch_pair_s32_headroom);

    RUN_TEST(test_xs3_vect_complex_s16_headroom);
    RUN_TEST(test_xs3_vect_complex_s32_headroom);
}