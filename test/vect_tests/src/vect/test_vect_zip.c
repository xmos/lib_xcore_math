// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xmath/xmath.h"
#include "xmath/xs3/vpu_scalar_ops.h"

#include "../tst_common.h"
#include "unity_fixture.h"

TEST_GROUP(vect_zip);
TEST_SETUP(vect_zip) { fflush(stdout); }
TEST_TEAR_DOWN(vect_zip) {}

TEST_GROUP_RUNNER(vect_zip) {
  RUN_TEST_CASE(vect_zip, vect_s32_zip);
  RUN_TEST_CASE(vect_zip, vect_s32_unzip);
}



#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (64)
#else
#  define REPS       (1000)
#  define MAX_LEN    (256)
#endif

TEST(vect_zip, vect_s32_zip)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    complex_s32_t DWORD_ALIGNED A[MAX_LEN];
    int32_t DWORD_ALIGNED B[MAX_LEN];
    int32_t DWORD_ALIGNED C[MAX_LEN];

    complex_s32_t expected[MAX_LEN];

    for(unsigned int v = 0; v < REPS; v++){
        unsigned old_seed = seed;
        unsigned len = pseudo_rand_uint(&seed, 1, MAX_LEN+1);

        setExtraInfo_RSL(v, old_seed, len);

        headroom_t b_hr = pseudo_rand_uint(&seed, 0, 8);
        headroom_t c_hr = pseudo_rand_uint(&seed, 0, 8);

        right_shift_t b_shr = pseudo_rand_int(&seed, -2, 3);
        right_shift_t c_shr = pseudo_rand_int(&seed, -2, 3);
        
        for(unsigned int i = 0; i < len; i++){
            B[i] = pseudo_rand_int32(&seed) >> b_hr;
            C[i] = pseudo_rand_int32(&seed) >> c_hr;

            expected[i].re = vlashr32(B[i], b_shr);
            expected[i].im = vlashr32(C[i], c_shr);
        }

        vect_s32_zip(A, B, C, len, b_shr, c_shr);

        for(unsigned int k = 0; k < len; k++){

          if(A[k].re != expected[k].re){
            printf("\nB[%d] = %ld;  b_shr = %d\n", k,  (long int) B[k], b_shr);
          }
          if(A[k].im != expected[k].im){
            printf("\nC[%d] = %ld;  c_shr = %d\n", k,  (long int) C[k], c_shr);
          }

          TEST_ASSERT_EQUAL_INT32_MESSAGE(expected[k].re, A[k].re, "");
          TEST_ASSERT_EQUAL_INT32_MESSAGE(expected[k].im, A[k].im, "");

        }
        
    }
}



TEST(vect_zip, vect_s32_unzip)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    int32_t DWORD_ALIGNED A[MAX_LEN];
    int32_t DWORD_ALIGNED B[MAX_LEN];
    complex_s32_t DWORD_ALIGNED C[MAX_LEN];

    int32_t expectedA[MAX_LEN];
    int32_t expectedB[MAX_LEN];

    for(unsigned int v = 0; v < REPS; v++){
        unsigned old_seed = seed;
        unsigned len = pseudo_rand_uint(&seed, 1, MAX_LEN+1);

        setExtraInfo_RSL(v, old_seed, len);

        headroom_t re_hr = pseudo_rand_uint(&seed, 0, 8);
        headroom_t im_hr = pseudo_rand_uint(&seed, 0, 8);
        
        for(unsigned int i = 0; i < len; i++){
          C[i].re = pseudo_rand_int32(&seed) >> re_hr;
          C[i].im = pseudo_rand_int32(&seed) >> im_hr;

          expectedA[i] = C[i].re;
          expectedB[i] = C[i].im;
        }

        vect_s32_unzip(A, B, C, len);

        for(unsigned int k = 0; k < len; k++){

          if(A[k] != expectedA[k])
            printf("\nC[%d].re = %ld\n", k,  (long int) C[k].re);
          
          if(B[k] != expectedB[k])
            printf("\nC[%d].im = %ld\n", k, (long int) C[k].im);
          

          TEST_ASSERT_EQUAL_INT32_MESSAGE(expectedA[k], A[k], "");
          TEST_ASSERT_EQUAL_INT32_MESSAGE(expectedB[k], B[k], "");

        }
        
    }
}
