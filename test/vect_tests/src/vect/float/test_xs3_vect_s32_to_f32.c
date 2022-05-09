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



TEST_GROUP_RUNNER(xs3_vect_s32_to_f32) {
  RUN_TEST_CASE(xs3_vect_s32_to_f32, xs3_vect_s32_to_f32);
}

TEST_GROUP(xs3_vect_s32_to_f32);
TEST_SETUP(xs3_vect_s32_to_f32) { fflush(stdout); }
TEST_TEAR_DOWN(xs3_vect_s32_to_f32) {}



#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (64)
#else
#  define REPS       (1000)
#  define MAX_LEN    (256)
#endif



TEST(xs3_vect_s32_to_f32, xs3_vect_s32_to_f32)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  DWORD_ALIGNED
  int32_t vec_in[MAX_LEN];
  DWORD_ALIGNED
  float vec_out[MAX_LEN];
  float expected[MAX_LEN];


  for(int v = 0; v < REPS; v++){
    const unsigned old_seed = seed;

    unsigned len = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
    setExtraInfo_RSL(v, old_seed, len);  

    exponent_t b_exp = pseudo_rand_int(&seed, -20, 20);

    for(int i = 0; i < len; i++){
      vec_in[i] = pseudo_rand_int32(&seed);
      expected[i] = ldexpf(vec_in[i], b_exp);
    }
    
    xs3_vect_s32_to_f32(vec_out, vec_in, len, b_exp);

    for(int k = 0; k < len; k++){
      TEST_ASSERT_EQUAL_MESSAGE(expected[k], vec_out[k], "");
    }
  }
}


