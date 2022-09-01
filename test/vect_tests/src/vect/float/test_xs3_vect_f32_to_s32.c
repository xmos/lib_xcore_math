// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <math.h>

#include "xs3_math.h"
#include "xs3_vpu_scalar_ops.h"
#include "../../tst_common.h"
#include "unity_fixture.h"



TEST_GROUP_RUNNER(xs3_vect_f32_to_s32) {
  RUN_TEST_CASE(xs3_vect_f32_to_s32, xs3_vect_f32_to_s32);
}

TEST_GROUP(xs3_vect_f32_to_s32);
TEST_SETUP(xs3_vect_f32_to_s32) { fflush(stdout); }
TEST_TEAR_DOWN(xs3_vect_f32_to_s32) {}



#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (64)
#else
#  define REPS       (1000)
#  define MAX_LEN    (256)
#endif



TEST(xs3_vect_f32_to_s32, xs3_vect_f32_to_s32)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  DWORD_ALIGNED
  float vec_in[MAX_LEN];
  DWORD_ALIGNED
  int32_t vec_out[MAX_LEN];
  int32_t expected[MAX_LEN];


  for(int v = 0; v < REPS; v++){
    const unsigned old_seed = seed;

    unsigned len = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
    setExtraInfo_RSL(v, old_seed, len);  

    exponent_t max_exponent = -0x80000000;

    // Create a bunch of random floats.
    for(int i = 0; i < len; i++){
      int32_t mant = pseudo_rand_int32(&seed);
      // modest range on exponents prevent us from ending up with lots of zeros.
      exponent_t exp = pseudo_rand_int(&seed, -10, 10); 
      float val = ldexpf(mant, exp);
      vec_in[i] = val;
    }

    exponent_t exp_out = xs3_vect_f32_max_exponent(vec_in, len);

    for(int k = 0; k < len; k++){
      float_s32_t f32 = xs3_f32_to_float_s32(vec_in[k]);
      expected[k] = vlashr32(f32.mant, exp_out - f32.exp);
    }

    xs3_vect_f32_to_s32(vec_out, vec_in, len, exp_out);

    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, vec_out, len, "");

    TEST_ASSERT_EQUAL(0, xs3_vect_s32_headroom(vec_out, len));
    
  }
}


