// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <math.h>

#include "xmath/xmath.h"
#include "../../tst_common.h"
#include "unity_fixture.h"



TEST_GROUP_RUNNER(vect_s32_to_vect_f32) {
  RUN_TEST_CASE(vect_s32_to_vect_f32, vect_s32_to_vect_f32);
  RUN_TEST_CASE(vect_s32_to_vect_f32, signed_boundaries);
}

TEST_GROUP(vect_s32_to_vect_f32);
TEST_SETUP(vect_s32_to_vect_f32) { fflush(stdout); }
TEST_TEAR_DOWN(vect_s32_to_vect_f32) {}



#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (64)
#else
#  define REPS       (1000)
#  define MAX_LEN    (256)
#endif



TEST(vect_s32_to_vect_f32, vect_s32_to_vect_f32)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  DWORD_ALIGNED
  int32_t vec_in[MAX_LEN];
  DWORD_ALIGNED
  float vec_out[MAX_LEN];
  float expected[MAX_LEN];


  for(unsigned int v = 0; v < REPS; v++){
    const unsigned old_seed = seed;

    unsigned len = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
    setExtraInfo_RSL(v, old_seed, len);

    exponent_t b_exp = pseudo_rand_int(&seed, -20, 20);

    for(unsigned int i = 0; i < len; i++){
      vec_in[i] = pseudo_rand_int32(&seed);
      expected[i] = ldexpf((float) vec_in[i], b_exp);
    }

    vect_s32_to_vect_f32(vec_out, vec_in, len, b_exp);

    for(unsigned int k = 0; k < len; k++){
      TEST_ASSERT_EQUAL_MESSAGE(expected[k], vec_out[k], "");
    }
  }
}

TEST(vect_s32_to_vect_f32, signed_boundaries)
{
  DWORD_ALIGNED
  const int32_t input[] = { 0, -1, 1, INT32_MIN, INT32_MAX, -2, 2, INT32_MIN + 1 };
  DWORD_ALIGNED
  float output[sizeof(input) / sizeof(input[0])];
  const unsigned lengths[] = { 1, 2, 3, 4, 5, 8 };

  for (unsigned n = 0; n < sizeof(lengths) / sizeof(lengths[0]); ++n) {
    const unsigned length = lengths[n];
    vect_s32_to_vect_f32(output, input, length, -3);
    for (unsigned k = 0; k < length; ++k)
      TEST_ASSERT_EQUAL_FLOAT(ldexpf((float) input[k], -3), output[k]);
  }
}
