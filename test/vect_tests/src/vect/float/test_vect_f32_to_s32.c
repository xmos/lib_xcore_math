// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <math.h>

#include "xmath/xmath.h"
#include "xmath/xs3/vpu_scalar_ops.h"
#include "../../tst_common.h"
#include "unity_fixture.h"



TEST_GROUP_RUNNER(vect_f32_to_vect_s32) {
  RUN_TEST_CASE(vect_f32_to_vect_s32, vect_f32_to_vect_s32);
}

TEST_GROUP(vect_f32_to_vect_s32);
TEST_SETUP(vect_f32_to_vect_s32) { fflush(stdout); }
TEST_TEAR_DOWN(vect_f32_to_vect_s32) {}



#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (64)
#else
#  define REPS       (1000)
#  define MAX_LEN    (256)
#endif



TEST(vect_f32_to_vect_s32, vect_f32_to_vect_s32)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  DWORD_ALIGNED
  float vec_in[MAX_LEN];
  DWORD_ALIGNED
  int32_t vec_out[MAX_LEN];
  int32_t expected[MAX_LEN];


  for(unsigned int v = 0; v < REPS; v++){
    const unsigned old_seed = seed;

    unsigned len = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
    setExtraInfo_RSL(v, old_seed, len);

    // Create a bunch of random floats.
    for(unsigned int i = 0; i < len; i++){
      int32_t mant = pseudo_rand_int32(&seed);
      // modest range on exponents prevent us from ending up with lots of zeros.
      exponent_t exp = pseudo_rand_int(&seed, -10, 10);
      float val = ldexpf((float) mant, exp);
      vec_in[i] = val;
    }

    exponent_t exp_out = vect_f32_max_exponent(vec_in, len);

    for(unsigned int k = 0; k < len; k++){
      float_s32_t f32 = f32_to_float_s32(vec_in[k]);
      expected[k] = vlashr32(f32.mant, exp_out - f32.exp);
    }

    vect_f32_to_vect_s32(vec_out, vec_in, len, exp_out);

    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, vec_out, len, "");

    TEST_ASSERT_EQUAL(0, vect_s32_headroom(vec_out, len));
    
  }
}


