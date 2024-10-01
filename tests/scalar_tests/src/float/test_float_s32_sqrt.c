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

TEST_GROUP_RUNNER(float_sXX_sqrt) {
  // RUN_TEST_CASE(float_sXX_sqrt, float_s16_sqrt);
  RUN_TEST_CASE(float_sXX_sqrt, float_s32_sqrt);
}

TEST_GROUP(float_sXX_sqrt);
TEST_SETUP(float_sXX_sqrt) { fflush(stdout); }
TEST_TEAR_DOWN(float_sXX_sqrt) {}


#if SMOKE_TEST
#  define REPS       (100)
#else
#  define REPS       (1000)
#endif



TEST(float_sXX_sqrt, float_s32_sqrt)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  float_s32_t x;
  conv_error_e error = 0;
  
  for(unsigned int v = 0; v < REPS; v++){

    setExtraInfo_RS(v, seed);

    x.exp = pseudo_rand_int(&seed, -40, 40);

    right_shift_t x_shr = pseudo_rand_uint(&seed, 0, 14);

    // Mantissa must be positive for this to make sense
    x.mant = pseudo_rand_uint32(&seed) >> (1+x_shr);

    double expected_f = sqrt( ldexp(x.mant, x.exp) );

    float_s32_t actual = float_s32_sqrt(x);
    
    int32_t expected = conv_double_to_s32(expected_f, actual.exp, &error);

    TEST_ASSERT_EQUAL_MESSAGE(0, error, "Conversion error.");

    TEST_ASSERT_INT32_WITHIN(2, expected, actual.mant);

  }
}

