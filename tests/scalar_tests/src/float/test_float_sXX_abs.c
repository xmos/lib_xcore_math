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

TEST_GROUP_RUNNER(float_sXX_abs) {
  RUN_TEST_CASE(float_sXX_abs, float_s32_abs);
}

TEST_GROUP(float_sXX_abs);
TEST_SETUP(float_sXX_abs) { fflush(stdout); }
TEST_TEAR_DOWN(float_sXX_abs) {}


#if SMOKE_TEST
#  define REPS       (100)
#else
#  define REPS       (1000)
#endif



TEST(float_sXX_abs, float_s32_abs)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  float_s32_t x;
  
  for(unsigned int v = 0; v < REPS; v++){

    setExtraInfo_RS(v, seed);

    x.exp = pseudo_rand_int(&seed, -40, 40);

    right_shift_t x_shr = pseudo_rand_uint(&seed, 0, 14);

    x.mant = pseudo_rand_int32(&seed) >> x_shr;

    int32_t expected_mant = (x.mant >= 0)? x.mant : -x.mant;

    float_s32_t actual = float_s32_abs(x);

    TEST_ASSERT_EQUAL_MESSAGE(x.exp, actual.exp, "");
    TEST_ASSERT_EQUAL_MESSAGE(expected_mant, actual.mant, "");

  }
}

