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

TEST_GROUP_RUNNER(float_convert) {
  RUN_TEST_CASE(float_convert, float_s64_to_float_s32);
  RUN_TEST_CASE(float_convert, float_s32_to_float_s64);

  RUN_TEST_CASE(float_convert, float_s32_to_double);
  RUN_TEST_CASE(float_convert, float_s32_to_float);

  RUN_TEST_CASE(float_convert, f32_to_float_s32);
  RUN_TEST_CASE(float_convert, f64_to_float_s32);
}

TEST_GROUP(float_convert);
TEST_SETUP(float_convert) { fflush(stdout); }
TEST_TEAR_DOWN(float_convert) {}


#if SMOKE_TEST
#  define REPS       (100)
#else
#  define REPS       (1000)
#endif


TEST(float_convert, float_s32_to_float_s64)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  float_s32_t x;
  conv_error_e error = 0;
  
  for(unsigned int v = 0; v < REPS; v++){

    setExtraInfo_RS(v, seed);

    x.exp = pseudo_rand_int(&seed, -40, 40);
    right_shift_t x_shr = pseudo_rand_uint(&seed, 0, 14);
    x.mant = pseudo_rand_int32(&seed) >> x_shr;

    float_s64_t actual = float_s32_to_float_s64(x);
    
    double expected_f = ldexp(x.mant, x.exp);

    int64_t expected_mant = conv_double_to_s64(expected_f, actual.exp, &error);

    TEST_ASSERT_EQUAL_MESSAGE(0, error, "Conversion error.");

    // There's no reason that any precision should be lost in this conversion
    TEST_ASSERT_LESS_OR_EQUAL_INT32(x.exp, actual.exp);

    TEST_ASSERT_INT64_WITHIN(2, expected_mant, actual.mant);

  }
}

TEST(float_convert, float_s64_to_float_s32)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  float_s64_t x;
  conv_error_e error = 0;
  
  for(unsigned int v = 0; v < REPS; v++){

    setExtraInfo_RS(v, seed);

    x.exp = pseudo_rand_int(&seed, -40, 40);
    right_shift_t x_shr = pseudo_rand_uint(&seed, 0, 14);
    x.mant = pseudo_rand_int64(&seed) >> x_shr;

    float_s32_t actual = float_s64_to_float_s32(x);
    
    double expected_f = ldexp((double) x.mant, x.exp);

    int32_t expected_mant = conv_double_to_s32(expected_f, actual.exp, &error);

    TEST_ASSERT_EQUAL_MESSAGE(0, error, "Conversion error.");

    // // There should be no headroom
    // TEST_ASSERT_EQUAL_MESSAGE(0, actual.exp, "Too much headroom");

    TEST_ASSERT_INT32_WITHIN_MESSAGE(2, expected_mant, actual.mant, "");

  }
}

TEST(float_convert, float_s32_to_float)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  float_s32_t x;
  
  for(unsigned int v = 0; v < REPS; v++){

    setExtraInfo_RS(v, seed) ;

    x.exp = pseudo_rand_int(&seed, -40, 40);
    right_shift_t x_shr = pseudo_rand_uint(&seed, 0, 14);
    x.mant = pseudo_rand_int32(&seed) >> x_shr;

    float actual = float_s32_to_float(x);
    
    float expected = ldexpf((float) x.mant, x.exp);

    // printf("\n\n\n");
    // printf("x.mant = 0x%08X  (%ld)\n", (unsigned) x.mant, x.mant);
    // printf("x.exp = %d\n", x.exp);
    // printf("x = %e\n", ldexpf(x.mant, x.exp));
    // printf("actual = %e\n", actual);
    // printf("expected = %e\n", expected);
    // printf("\n\n\n");

    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.0f, expected, actual, "");
  }
}

TEST(float_convert, float_s32_to_double)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  float_s32_t x;
  
  for(unsigned int v = 0; v < REPS; v++){

    setExtraInfo_RS(v, seed);

    x.exp = pseudo_rand_int(&seed, -40, 40);
    right_shift_t x_shr = pseudo_rand_uint(&seed, 0, 14);
    x.mant = pseudo_rand_int32(&seed) >> x_shr;

    double actual = float_s32_to_double(x);
    
    double expected = ldexp(x.mant, x.exp);

    TEST_ASSERT_DOUBLE_WITHIN(ldexp(1, x.exp), expected, actual);
  }
}

TEST(float_convert, f32_to_float_s32)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  float x;

  for(unsigned int v = 0; v < REPS; v++){

    setExtraInfo_RS(v, seed);

    x = ldexpf((float) pseudo_rand_int32(&seed), pseudo_rand_int(&seed, -40, 40));

    float_s32_t actual = f32_to_float_s32(x);

    float diff  = fabsf(x - ldexpf((float) (actual.mant + 0), actual.exp));
    float diffP = fabsf(x - ldexpf((float) (actual.mant + 1), actual.exp));
    float diffM = fabsf(x - ldexpf((float) (actual.mant - 1), actual.exp));

    TEST_ASSERT_GREATER_OR_EQUAL_MESSAGE(diff, diffP, "");
    TEST_ASSERT_GREATER_OR_EQUAL_MESSAGE(diff, diffM, "");

  }
}

TEST(float_convert, f64_to_float_s32)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  double x;
  
  for(unsigned int v = 0; v < REPS; v++){

    setExtraInfo_RS(v, seed);

    x = ldexp((double) pseudo_rand_int32(&seed), pseudo_rand_int(&seed, -40, 40));

    float_s32_t actual = f64_to_float_s32(x);

    if(x != 0.0)
      TEST_ASSERT_LESS_OR_EQUAL(1, HR_S32(actual.mant));

    double diff  = fabs(x - ldexp(actual.mant + 0, actual.exp));
    double diffP = fabs(x - ldexp(actual.mant + 2, actual.exp));
    double diffM = fabs(x - ldexp(actual.mant - 2, actual.exp));

    TEST_ASSERT_GREATER_OR_EQUAL_MESSAGE(diff, diffP, "");
    TEST_ASSERT_GREATER_OR_EQUAL_MESSAGE(diff, diffM, "");
  }
}
