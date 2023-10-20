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




TEST_GROUP_RUNNER(vect_f32_add) {
  RUN_TEST_CASE(vect_f32_add, vect_f32_add);
}

TEST_GROUP(vect_f32_add);
TEST_SETUP(vect_f32_add) { fflush(stdout); }
TEST_TEAR_DOWN(vect_f32_add) {}


#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (64)
#else
#  define REPS       (1000)
#  define MAX_LEN    (256)
#endif


TEST(vect_f32_add, vect_f32_add)
{
  unsigned seed = SEED_FROM_FUNC_NAME();
  // seed = 0x571AC451;

  float expected[MAX_LEN];

  DWORD_ALIGNED
  float a[MAX_LEN];
  DWORD_ALIGNED
  float b[MAX_LEN];
  DWORD_ALIGNED
  float c[MAX_LEN];

  for(unsigned int v = 0; v < REPS; v++){
    const unsigned old_seed = seed;

    unsigned len = pseudo_rand_uint(&seed, 1, MAX_LEN-1);
    setExtraInfo_RSL(v, old_seed, len);
    // printf("0x%08X\n", old_seed);

    for(unsigned int k = 0; k < len; k++){
      exponent_t exponent = pseudo_rand_int(&seed, -50, 2);
      b[k] = ldexpf((float) (pseudo_rand_int32(&seed) & 0xFFFFFF00),
                      exponent);
      c[k] = ldexpf((float) (pseudo_rand_int32(&seed) & 0xFFFFFF00),
                      exponent);
      expected[k] = b[k] + c[k];
    }
    
    vect_f32_add(a, b, c, len);

    for(unsigned int k = 0; k < len; k++){
      float delta = a[k] - expected[k];
      float ppo = delta/expected[k];
      // printf("Got: %e;\t Expected %e;\t Delta: %e\n", a[k], expected[k], delta);
      TEST_ASSERT_FLOAT_WITHIN_MESSAGE(ldexpf(1,-20), 0.0f, ppo, "");
    }
  }
}



