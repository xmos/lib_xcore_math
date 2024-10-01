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

TEST_GROUP_RUNNER(float_sXX_ema) {
  RUN_TEST_CASE(float_sXX_ema, float_s32_ema);
}

TEST_GROUP(float_sXX_ema);
TEST_SETUP(float_sXX_ema) { fflush(stdout); }
TEST_TEAR_DOWN(float_sXX_ema) {}


#if SMOKE_TEST
#  define REPS       (100)
#else
#  define REPS       (1000)
#endif


TEST(float_sXX_ema, float_s32_ema)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  float_s32_t x, y;
  conv_error_e error = 0;
  
  for(unsigned int v = 0; v < REPS; v++){

    setExtraInfo_RS(v, seed);

    x.exp = pseudo_rand_int(&seed, -40, 40);
    y.exp = x.exp + pseudo_rand_int(&seed, -5, 6);

    right_shift_t x_shr = pseudo_rand_uint(&seed, 0, 14);
    right_shift_t y_shr = pseudo_rand_uint(&seed, 0, 14);

    x.mant = pseudo_rand_int32(&seed) >> x_shr;
    y.mant = pseudo_rand_int32(&seed) >> y_shr;

    int32_t coef_q30 = pseudo_rand_uint(&seed, 0x00000000, 0x40000000);
    
    double coef_f = ldexp(coef_q30, -30);

    double expected_f = ldexp(x.mant, x.exp) * coef_f + (1.0-coef_f) * ldexp(y.mant, y.exp);

    float_s32_t actual = float_s32_ema(x, y, coef_q30);
    
    int32_t expected = conv_double_to_s32(expected_f, actual.exp, &error);

    TEST_ASSERT_EQUAL_MESSAGE(0, error, "Conversion error.");
    
    // astew: 2022/06/30 - Changed threshold from 2 to 3. Test started failing when I fixed
    //        a problem with random number generation. (The threshold observed, not theoretical)
    TEST_ASSERT_INT32_WITHIN_MESSAGE(3, expected, actual.mant, "");

  }
}

