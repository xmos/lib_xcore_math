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

TEST_GROUP_RUNNER(float_sXX_div) {
  RUN_TEST_CASE(float_sXX_div, float_s32_div);
}

TEST_GROUP(float_sXX_div);
TEST_SETUP(float_sXX_div) { fflush(stdout); }
TEST_TEAR_DOWN(float_sXX_div) {}


#if SMOKE_TEST
#  define REPS       (100)
#else
#  define REPS       (1000)
#endif


TEST(float_sXX_div, float_s32_div)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  float_s32_t x, y;
  conv_error_e error = 0;
  
  for(unsigned int v = 0; v < REPS; v++){

    setExtraInfo_RS(v, seed);

    x.exp = pseudo_rand_int(&seed, -4, 4);
    y.exp = pseudo_rand_int(&seed, -4, 4);

    right_shift_t x_shr = pseudo_rand_uint(&seed, 0, 14);
    right_shift_t y_shr = pseudo_rand_uint(&seed, 0, 14);

    x.mant = pseudo_rand_int32(&seed) >> x_shr;
    y.mant = pseudo_rand_int32(&seed) >> y_shr;
    
    double expected_f = ldexp(x.mant, x.exp) / ldexp(y.mant, y.exp);

    float_s32_t actual = float_s32_div(x, y);
    
    int32_t expected = conv_double_to_s32(expected_f, actual.exp, &error);

    TEST_ASSERT_EQUAL_MESSAGE(0, error, "Conversion error.");

    TEST_ASSERT_INT32_WITHIN(4, expected, actual.mant);

  }
}

