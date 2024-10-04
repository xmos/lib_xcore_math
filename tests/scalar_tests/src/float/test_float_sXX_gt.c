// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "xmath/xmath.h"

#include "../tst_common.h"

#include "unity_fixture.h"

TEST_GROUP_RUNNER(float_sXX_gt) {
  RUN_TEST_CASE(float_sXX_gt, float_s32_gt);
  RUN_TEST_CASE(float_sXX_gt, float_s32_gte);
}

TEST_GROUP(float_sXX_gt);
TEST_SETUP(float_sXX_gt) { fflush(stdout); }
TEST_TEAR_DOWN(float_sXX_gt) {}


#if SMOKE_TEST
#  define REPS       (100)
#else
#  define REPS       (1000)
#endif


TEST(float_sXX_gt, float_s32_gt)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  float_s32_t x, y;
  
  for(unsigned int v = 0; v < REPS; v++){

    setExtraInfo_RS(v, seed);

    x.exp = pseudo_rand_int(&seed, -40, 40);
    y.exp = x.exp + pseudo_rand_int(&seed, -1, 2);

    right_shift_t x_shr = pseudo_rand_uint(&seed, 0, 14);
    right_shift_t y_shr = pseudo_rand_uint(&seed, 0, 14);

    x.mant = pseudo_rand_int32(&seed) >> x_shr;
    y.mant = pseudo_rand_int32(&seed) >> y_shr;
    
    unsigned actual = float_s32_gt(x, y);

    double dx = ldexp(x.mant, x.exp);
    double dy = ldexp(y.mant, y.exp);
    
    unsigned expected = dx > dy;

    TEST_ASSERT_EQUAL_MESSAGE(expected, actual, "");

  }
}


TEST(float_sXX_gt, float_s32_gte)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  float_s32_t x, y;
  
  for(unsigned int v = 0; v < REPS; v++){

    setExtraInfo_RS(v, seed);

    x.exp = pseudo_rand_int(&seed, -40, 40);
    y.exp = x.exp + pseudo_rand_int(&seed, -1, 2);

    right_shift_t x_shr = pseudo_rand_uint(&seed, 0, 14);
    right_shift_t y_shr = pseudo_rand_uint(&seed, 0, 14);

    x.mant = pseudo_rand_int32(&seed) >> x_shr;
    y.mant = pseudo_rand_int32(&seed) >> y_shr;
    
    unsigned actual = float_s32_gte(x, y);

    double dx = ldexp(x.mant, x.exp);
    double dy = ldexp(y.mant, y.exp);
    
    unsigned expected = dx >= dy;

    TEST_ASSERT_EQUAL_MESSAGE(expected, actual, "");

  }
}

