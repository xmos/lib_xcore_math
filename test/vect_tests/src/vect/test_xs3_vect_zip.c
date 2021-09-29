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

TEST_GROUP(xs3_vect_zip);
TEST_SETUP(xs3_vect_zip) { fflush(stdout); }
TEST_TEAR_DOWN(xs3_vect_zip) {}

TEST_GROUP_RUNNER(xs3_vect_zip) {
  RUN_TEST_CASE(xs3_vect_zip, xs3_vect_s32_zip);
  RUN_TEST_CASE(xs3_vect_zip, xs3_vect_s32_unzip);
}



#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (64)
#else
#  define REPS       (1000)
#  define MAX_LEN    (256)
#endif

TEST(xs3_vect_zip, xs3_vect_s32_zip)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    complex_s32_t DWORD_ALIGNED A[MAX_LEN];
    int32_t DWORD_ALIGNED B[MAX_LEN];
    int32_t DWORD_ALIGNED C[MAX_LEN];

    complex_s32_t expected[MAX_LEN];

    for(int v = 0; v < REPS; v++){
        unsigned old_seed = seed;
        unsigned len = pseudo_rand_uint(&seed, 0, MAX_LEN+1);

        setExtraInfo_RSL(v, old_seed, len);

        headroom_t b_hr = pseudo_rand_uint(&seed, 0, 8);
        headroom_t c_hr = pseudo_rand_uint(&seed, 0, 8);

        right_shift_t b_shr = pseudo_rand_int(&seed, -2, 3);
        right_shift_t c_shr = pseudo_rand_int(&seed, -2, 3);
        
        for(int i = 0; i < len; i++){
            B[i] = pseudo_rand_int32(&seed) >> b_hr;
            C[i] = pseudo_rand_int32(&seed) >> c_hr;

            expected[i].re = vlashr32(B[i], b_shr);
            expected[i].im = vlashr32(C[i], c_shr);
        }

        xs3_vect_s32_zip(A, B, C, len, b_shr, c_shr);

        for(int k = 0; k < len; k++){

          if(A[k].re != expected[k].re){
            printf("\nB[%d] = %ld;  b_shr = %d\n", k, B[k], b_shr);
          }
          if(A[k].im != expected[k].im){
            printf("\nC[%d] = %ld;  c_shr = %d\n", k, C[k], c_shr);
          }

          TEST_ASSERT_EQUAL_INT32_MESSAGE(expected[k].re, A[k].re, "");
          TEST_ASSERT_EQUAL_INT32_MESSAGE(expected[k].im, A[k].im, "");

        }
        
    }
}



TEST(xs3_vect_zip, xs3_vect_s32_unzip)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    int32_t DWORD_ALIGNED A[MAX_LEN];
    int32_t DWORD_ALIGNED B[MAX_LEN];
    complex_s32_t DWORD_ALIGNED C[MAX_LEN];

    int32_t expectedA[MAX_LEN];
    int32_t expectedB[MAX_LEN];

    for(int v = 0; v < REPS; v++){
        unsigned old_seed = seed;
        unsigned len = pseudo_rand_uint(&seed, 0, MAX_LEN+1);

        setExtraInfo_RSL(v, old_seed, len);

        headroom_t re_hr = pseudo_rand_uint(&seed, 0, 8);
        headroom_t im_hr = pseudo_rand_uint(&seed, 0, 8);
        
        for(int i = 0; i < len; i++){
          C[i].re = pseudo_rand_int32(&seed) >> re_hr;
          C[i].im = pseudo_rand_int32(&seed) >> im_hr;

          expectedA[i] = C[i].re;
          expectedB[i] = C[i].im;
        }

        xs3_vect_s32_unzip(A, B, C, len);

        for(int k = 0; k < len; k++){

          if(A[k] != expectedA[k])
            printf("\nC[%d].re = %ld\n", k, B[k]);
          
          if(B[k] != expectedB[k])
            printf("\nC[%d].im = %ld\n", k, C[k]);
          

          TEST_ASSERT_EQUAL_INT32_MESSAGE(expectedA[k], A[k], "");
          TEST_ASSERT_EQUAL_INT32_MESSAGE(expectedB[k], B[k], "");

        }
        
    }
}
