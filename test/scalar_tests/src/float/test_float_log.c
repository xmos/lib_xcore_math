// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "xmath/xmath.h"

#include "../tst_common.h"

#include "unity_fixture.h"

TEST_GROUP_RUNNER(float_log) {
  RUN_TEST_CASE(float_log, f32_normA);
  RUN_TEST_CASE(float_log, f32_log2);
}

TEST_GROUP(float_log);
TEST_SETUP(float_log) { fflush(stdout); }
TEST_TEAR_DOWN(float_log) {}


#if SMOKE_TEST
#  define REPS       (100)
#else
#  define REPS       (1000)
#endif

TEST(float_log, f32_normA)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  for(unsigned int v = 0; v < REPS; v++){
    setExtraInfo_RS(v, seed);

    float x = (float) ldexp((double) pseudo_rand_uint(&seed, 1, INT32_MAX),
                    pseudo_rand_int(&seed, -10, 10));

    int expected_exp;
    float expected_f = frexpf(x, &expected_exp);

    while(fabsf(expected_f) >= 1.0f){
      expected_f /= 2;
      expected_exp++;
    }

    while(fabsf(expected_f) < 0.5f){
      expected_f *= 2;
      expected_exp--;
    }

    exponent_t actual_exp;
    float actual_f = f32_normA(&actual_exp, x);
    

    double diff = fabs(expected_f - actual_f);
    double ratio = fabs(diff / expected_f);

    if( ratio > ldexp(1, -18)){
      printf("x = %f\n", x);
      printf("expected_exp = %d\n", expected_exp);
      printf("actual_exp = %d\n", actual_exp);
      printf("expected_f = %f\n", expected_f);
      printf("actual_f = %f\n", actual_f);
      printf("ratio = %e\n", ratio);
    }

    TEST_ASSERT_EQUAL_INT32(expected_exp, actual_exp);

    TEST_ASSERT_FLOAT_WITHIN(ldexpf(1,-18), x, ldexpf(expected_f, expected_exp));
    TEST_ASSERT_FLOAT_WITHIN(ldexpf(1,-18), x, ldexpf(actual_f, actual_exp));

  }
}



TEST(float_log, f32_log2)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  for(unsigned int v = 0; v < REPS; v++){
    setExtraInfo_RS(v, seed);

    float x = (float) ldexp((double) pseudo_rand_uint(&seed, 1, INT32_MAX),
      pseudo_rand_int(&seed, -10, 10));

    float actual, exp;

    TIME_STATEMENT(actual = f32_log2(x), delta_us);
    TIME_STATEMENT(exp = log2f(x), delta_us_ref);

    double diff = fabs(exp - actual);
    double ratio = fabs(diff / exp);


    if( ratio > ldexp(1, -11)){
      printf("x = %f\n", x);
      printf("exp = %f\n", exp);
      printf("actual = %f\n", actual);
      printf("ratio = %e\n", ratio);
    }

    TEST_ASSERT( ratio <= ldexp(1, -11) );
    
    // printf("time: % 7.02f us;\tref: % 7.02f us\n", delta_us, delta_us_ref);
  }
}