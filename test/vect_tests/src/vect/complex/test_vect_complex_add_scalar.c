// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xmath/xmath.h"
#include "xmath/xs3/vpu_scalar_ops.h"

#include "../../tst_common.h"


#include "unity_fixture.h"


TEST_GROUP_RUNNER(vect_complex_add_scalar) {
  RUN_TEST_CASE(vect_complex_add_scalar, vect_complex_s16_add_scalar);
  RUN_TEST_CASE(vect_complex_add_scalar, vect_complex_s32_add_scalar);
}

TEST_GROUP(vect_complex_add_scalar);
TEST_SETUP(vect_complex_add_scalar) { fflush(stdout); }
TEST_TEAR_DOWN(vect_complex_add_scalar) {}



#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (64)
#else
#  define REPS       (1000)
#  define MAX_LEN    (256)
#endif



TEST(vect_complex_add_scalar, vect_complex_s16_add_scalar)
{
    
  unsigned seed = SEED_FROM_FUNC_NAME();

  headroom_t hr;
  int16_t WORD_ALIGNED A_real[MAX_LEN];
  int16_t WORD_ALIGNED B_real[MAX_LEN];

  int16_t WORD_ALIGNED A_imag[MAX_LEN];
  int16_t WORD_ALIGNED B_imag[MAX_LEN];

  int16_t expected_real[MAX_LEN];
  int16_t expected_imag[MAX_LEN];

  for(unsigned int v = 0; v < REPS; v++){

    const unsigned old_seed = seed;
    unsigned len = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
    setExtraInfo_RSL(v, old_seed, len);
    
    for(unsigned int i = 0; i < len; i++){
        unsigned shr = pseudo_rand_uint(&seed, 0, 8);
        B_real[i] = pseudo_rand_int16(&seed) >> shr;
        B_imag[i] = pseudo_rand_int16(&seed) >> shr;
    }

    complex_s16_t C = {
      .re = pseudo_rand_int16(&seed) >> 1,
      .im = pseudo_rand_int16(&seed) >> 1,
    };

    int b_shr = pseudo_rand_uint(&seed, -2, 3);


    for(unsigned int k = 0; k < len; k++){
      expected_real[k] = vladd16(vlashr16(B_real[k], b_shr), C.re);
      expected_imag[k] = vladd16(vlashr16(B_imag[k], b_shr), C.im);
    }
    
    hr = vect_complex_s16_add_scalar(A_real, A_imag, B_real, B_imag, C, len, b_shr);

    TEST_ASSERT_EQUAL_MESSAGE( vect_complex_s16_headroom(A_real, A_imag, len), hr, "Headroom is wrong.");

    TEST_ASSERT_EQUAL_INT16_ARRAY_MESSAGE(expected_real, A_real, len, "");
    TEST_ASSERT_EQUAL_INT16_ARRAY_MESSAGE(expected_imag, A_imag, len, "");
    
  }
}

TEST(vect_complex_add_scalar, vect_complex_s32_add_scalar)
{
    
  unsigned seed = SEED_FROM_FUNC_NAME();

  headroom_t hr;
  complex_s32_t WORD_ALIGNED A[MAX_LEN];
  complex_s32_t WORD_ALIGNED B[MAX_LEN];

  complex_s32_t expected[MAX_LEN];

  for(unsigned int v = 0; v < REPS; v++){

    const unsigned old_seed = seed;
    unsigned len = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
    setExtraInfo_RSL(v, old_seed, len);
    
    for(unsigned int i = 0; i < len; i++){
        unsigned shr = pseudo_rand_uint(&seed, 0, 8);
        B[i].re = pseudo_rand_int32(&seed) >> shr;
        B[i].im = pseudo_rand_int32(&seed) >> shr;
    }

    complex_s32_t C = {
      .re = pseudo_rand_int32(&seed) >> 1,
      .im = pseudo_rand_int32(&seed) >> 1,
    };

    int b_shr = pseudo_rand_uint(&seed, -2, 3);


    for(unsigned int k = 0; k < len; k++){
      expected[k].re = vladd32(vlashr32(B[k].re, b_shr), C.re);
      expected[k].im = vladd32(vlashr32(B[k].im, b_shr), C.im);
    }
    
    hr = vect_complex_s32_add_scalar(A, B, C, len, b_shr);

    TEST_ASSERT_EQUAL_MESSAGE( vect_complex_s32_headroom(A, len), hr, "Headroom is wrong.");

    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE( (int32_t*)expected, (int32_t*) A, 2*len, "");
    
  }
}


