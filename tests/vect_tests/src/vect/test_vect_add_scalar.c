// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xmath/xmath.h"

#include "../tst_common.h"

#include "xmath/xs3/vpu_scalar_ops.h"

#include "unity_fixture.h"


TEST_GROUP_RUNNER(vect_add_scalar) {
  RUN_TEST_CASE(vect_add_scalar, vect_s16_add_scalar);
  RUN_TEST_CASE(vect_add_scalar, vect_s32_add_scalar);
}

TEST_GROUP(vect_add_scalar);
TEST_SETUP(vect_add_scalar) { fflush(stdout); }
TEST_TEAR_DOWN(vect_add_scalar) {}



#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (64)
#else
#  define REPS       (1000)
#  define MAX_LEN    (256)
#endif

TEST(vect_add_scalar, vect_s16_add_scalar)
{
    
  unsigned seed = SEED_FROM_FUNC_NAME();

  headroom_t hr;
  int16_t WORD_ALIGNED A[MAX_LEN];
  int16_t WORD_ALIGNED B[MAX_LEN];

  int16_t expected[MAX_LEN];

  for(unsigned int v = 0; v < REPS; v++){

    const unsigned old_seed = seed;
    unsigned len = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
    setExtraInfo_RSL(v, old_seed, len);
    
    for(unsigned int i = 0; i < len; i++){
        unsigned shr = pseudo_rand_uint(&seed, 0, 8);
        B[i] = pseudo_rand_int16(&seed) >> shr;
    }

    int16_t C = pseudo_rand_int16(&seed) >> 1;
    int b_shr = pseudo_rand_uint(&seed, -2, 3);


    for(unsigned int k = 0; k < len; k++){
      expected[k] = vladd16(vlashr16(B[k], b_shr), C);
    }
    

    hr = vect_s16_add_scalar(A, B, C, len, b_shr);

    TEST_ASSERT_EQUAL_MESSAGE( vect_s16_headroom(A, len), hr, "Headroom is wrong.");

    TEST_ASSERT_EQUAL_INT16_ARRAY_MESSAGE(expected, A, len, "");
    
  }
}

TEST(vect_add_scalar, vect_s32_add_scalar)
{
    
  unsigned seed = SEED_FROM_FUNC_NAME();

  headroom_t hr;
  int32_t WORD_ALIGNED A[MAX_LEN];
  int32_t WORD_ALIGNED B[MAX_LEN];

  int32_t expected[MAX_LEN];

  for(unsigned int v = 0; v < REPS; v++){

    const unsigned old_seed = seed;
    unsigned len = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
    setExtraInfo_RSL(v, old_seed, len);
    
    for(unsigned int i = 0; i < len; i++){
        unsigned shr = pseudo_rand_uint(&seed, 0, 8);
        B[i] = pseudo_rand_int32(&seed) >> shr;
    }

    int32_t C = pseudo_rand_int32(&seed) >> 1;
    int b_shr = pseudo_rand_uint(&seed, -2, 3);


    for(unsigned int k = 0; k < len; k++){
      expected[k] = vladd32(vlashr32(B[k], b_shr), C);
    }
    

    hr = vect_s32_add_scalar(A, B, C, len, b_shr);

    TEST_ASSERT_EQUAL_MESSAGE( vect_s32_headroom(A, len), hr, "Headroom is wrong.");

    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, A, len, "");
    
  }
}


