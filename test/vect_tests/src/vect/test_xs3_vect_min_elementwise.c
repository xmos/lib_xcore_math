// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xs3_math.h"
#include "xs3_vpu_scalar_ops.h"

#include "../tst_common.h"
#include "unity_fixture.h"

TEST_GROUP(xs3_vect_min_elementwise);
TEST_SETUP(xs3_vect_min_elementwise) { fflush(stdout); }
TEST_TEAR_DOWN(xs3_vect_min_elementwise) {}

TEST_GROUP_RUNNER(xs3_vect_min_elementwise) {
  RUN_TEST_CASE(xs3_vect_min_elementwise, xs3_vect_s16_min_elementwise);
  RUN_TEST_CASE(xs3_vect_min_elementwise, xs3_vect_s32_min_elementwise);
}

static int16_t min_s16(int16_t b, int16_t c, right_shift_t b_shr, right_shift_t c_shr)
{
  b = vlashr16(b, b_shr);
  c = vlashr16(c, c_shr);
  return (b < c)? b : c;
}

static int32_t min_s32(int32_t b, int32_t c, right_shift_t b_shr, right_shift_t c_shr)
{
  b = vlashr32(b, b_shr);
  c = vlashr32(c, c_shr);
  return (b < c)? b : c;
}


#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (64)
#else
#  define REPS       (1000)
#  define MAX_LEN    (256)
#endif



TEST(xs3_vect_min_elementwise, xs3_vect_s16_min_elementwise)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    headroom_t hr;
    int16_t A[MAX_LEN];
    int16_t B[MAX_LEN];
    int16_t C[MAX_LEN];

    int16_t expected[MAX_LEN];

    const char debug_fmt[] = "Expected: %d <-- min((%d >> %d), (%d >> %d)) \nActual: %d\n";

    for(int v = 0; v < REPS; v++){
        unsigned old_seed = seed;
        unsigned len = pseudo_rand_uint(&seed, 0, MAX_LEN+1);

        setExtraInfo_RSL(v, old_seed, len);

        headroom_t b_hr = pseudo_rand_uint(&seed, 0, 5);
        headroom_t c_hr = pseudo_rand_uint(&seed, 0, 5);
        
        for(int i = 0; i < len; i++){
            B[i] = pseudo_rand_int16(&seed) >> b_hr;
            C[i] = pseudo_rand_int16(&seed) >> c_hr;
        }

        b_hr = xs3_vect_s16_headroom(B, len);
        c_hr = xs3_vect_s16_headroom(C, len);

        right_shift_t b_shr = pseudo_rand_int(&seed, 1-b_hr, 5);
        right_shift_t c_shr = pseudo_rand_int(&seed, 1-c_hr, 5);
        
        // Determine expected outputs
        for(int i = 0; i < len; i++){
          expected[i] = min_s16(B[i], C[i], b_shr, c_shr);
        }
        
        // Not in-place
        hr = xs3_vect_s16_min_elementwise(A, B, C, len, b_shr, c_shr);
        XTEST_ASSERT_VECT_S16_EQUAL(expected, A, len, 
            debug_fmt, expected[i], B[i], b_shr, C[i], c_shr, A[i] );
        TEST_ASSERT_EQUAL(xs3_vect_s16_headroom(A, len), hr);
        
        // In-place arg-2
        memcpy(A, B, sizeof(A[0])*len);
        hr = xs3_vect_s16_min_elementwise(A, A, C, len, b_shr, c_shr);

        XTEST_ASSERT_VECT_S16_EQUAL(expected, A, len, 
            debug_fmt, expected[i], B[i], b_shr, C[i], c_shr, A[i] );
        TEST_ASSERT_EQUAL(xs3_vect_s16_headroom(A, len), hr);
        
        // In-place arg-3 NOT SUPPORTED
    }
}



TEST(xs3_vect_min_elementwise, xs3_vect_s32_min_elementwise)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    headroom_t hr;
    int32_t A[MAX_LEN];
    int32_t B[MAX_LEN];
    int32_t C[MAX_LEN];

    int32_t expected[MAX_LEN];

    const char debug_fmt[] = "Expected: %ld <-- min((%ld >> %d), (%ld >> %d)) \nActual: %ld\n";

    for(int v = 0; v < REPS; v++){
        unsigned old_seed = seed;
        unsigned len = pseudo_rand_uint(&seed, 0, MAX_LEN+1);

        setExtraInfo_RSL(v, old_seed, len);

        headroom_t b_hr = pseudo_rand_uint(&seed, 0, 5);
        headroom_t c_hr = pseudo_rand_uint(&seed, 0, 5);
        
        for(int i = 0; i < len; i++){
            B[i] = pseudo_rand_int32(&seed) >> b_hr;
            C[i] = pseudo_rand_int32(&seed) >> c_hr;
        }

        b_hr = xs3_vect_s32_headroom(B, len);
        c_hr = xs3_vect_s32_headroom(C, len);

        right_shift_t b_shr = pseudo_rand_int(&seed, 1-b_hr, 5);
        right_shift_t c_shr = pseudo_rand_int(&seed, 1-c_hr, 5);
        
        // Determine expected outputs
        for(int i = 0; i < len; i++){
          expected[i] = min_s32(B[i], C[i], b_shr, c_shr);
        }
        
        // Not in-place
        hr = xs3_vect_s32_min_elementwise(A, B, C, len, b_shr, c_shr);
        XTEST_ASSERT_VECT_S32_EQUAL(expected, A, len, 
            debug_fmt, expected[i], B[i], b_shr, C[i], c_shr, A[i] );
        TEST_ASSERT_EQUAL(xs3_vect_s32_headroom(A, len), hr);
        
        // In-place arg-2
        memcpy(A, B, sizeof(A[0])*len);
        hr = xs3_vect_s32_min_elementwise(A, A, C, len, b_shr, c_shr);

        XTEST_ASSERT_VECT_S32_EQUAL(expected, A, len, 
            debug_fmt, expected[i], B[i], b_shr, C[i], c_shr, A[i] );
        TEST_ASSERT_EQUAL(xs3_vect_s32_headroom(A, len), hr);
        
        // In-place arg-3 NOT SUPPORTED
    }
}

