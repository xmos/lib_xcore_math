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

#include "../../tst_common.h"

#include "unity_fixture.h"


TEST_GROUP_RUNNER(vect_f32_max_exponent) {
  RUN_TEST_CASE(vect_f32_max_exponent, vect_f32_max_exponent);
}

TEST_GROUP(vect_f32_max_exponent);
TEST_SETUP(vect_f32_max_exponent) { fflush(stdout); }
TEST_TEAR_DOWN(vect_f32_max_exponent) {}



#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (64)
#else
#  define REPS       (1000)
#  define MAX_LEN    (256)
#endif



TEST(vect_f32_max_exponent, vect_f32_max_exponent)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  DWORD_ALIGNED
  float vector[MAX_LEN];

  for(unsigned int v = 0; v < REPS; v++){
    const unsigned old_seed = seed;

    unsigned len = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
    setExtraInfo_RSL(v, old_seed, len);

    // printf("seed = 0x%08X\n", old_seed);

    exponent_t max_exponent = (int) (0-0x80000000);

    // Create a bunch of random floats.
    for(unsigned int i = 0; i < len; i++){
      int32_t mant = pseudo_rand_int32(&seed);
      exponent_t exp = pseudo_rand_int(&seed, -95, 95);
      float val = ldexpf((float) mant, exp);
      vector[i] = val;
      
      int exp2;
      float vn = fabsf(frexpf(val, &exp2));
      int df = (int) ceilf(log2f(vn));
      assert(df == 0);
      exp2 -= 31;

      max_exponent = (exp2 > max_exponent)? exp2 : max_exponent;
    }

    exponent_t result = vect_f32_max_exponent(vector, len);

    TEST_ASSERT_EQUAL_MESSAGE(max_exponent, result, "");
    
  }
}


