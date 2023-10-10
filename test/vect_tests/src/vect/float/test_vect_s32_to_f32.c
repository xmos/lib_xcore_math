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



TEST_GROUP_RUNNER(vect_s32_to_vect_f32) {
  RUN_TEST_CASE(vect_s32_to_vect_f32, vect_s32_to_vect_f32);
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


