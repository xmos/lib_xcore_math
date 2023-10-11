// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "xmath/xmath.h"

#include "../tst_common.h"

#include "unity_fixture.h"

TEST_GROUP_RUNNER(float_logistic) {
  RUN_TEST_CASE(float_logistic, q24_logistic);
  RUN_TEST_CASE(float_logistic, q24_logistic_fast);
}

TEST_GROUP(float_logistic);
TEST_SETUP(float_logistic) { fflush(stdout); }
TEST_TEAR_DOWN(float_logistic) {}


#if SMOKE_TEST
#  define REPS       (1000)
#else
#  define REPS       (10000)
#endif


TEST(float_logistic, q24_logistic)
{
  q8_24 incr = Q24(20.0 / 1000.0);
  unsigned max_error = 0;
  float max_time = 0.0f;
  for(q8_24 k = Q24(-1.2); k < Q24(1.2); k += incr){
    q8_24 x = k;
    q8_24 y;
    double xf = F24(x);
    double expected_f = 1.0 / (1 + exp(-xf));
    q8_24 expected_q24 = Q24(expected_f);
    TIME_STATEMENT(y = q24_logistic(x), timing);
    unsigned error = abs(expected_q24 - y);
    max_error = (error > max_error)? error : max_error;
    max_time = (timing > max_time)? timing : max_time;
    if(error > 100){
      printf("x = %f  (%ld << -24)\n", xf, (long int) x);
      printf("exp = %f  (%ld << -24)\n", expected_f, (long int) expected_q24);
      printf("y = %f  (%ld << -24)\n", ldexp(y, -24), (long int) y);
      printf("error = %d\n", error);
    }
    TEST_ASSERT_INT32_WITHIN(100, expected_q24, y);
  }
  // printf("max_time: %f us\n", max_time);
  // printf("max_error: %u\n", max_error);
}


TEST(float_logistic, q24_logistic_fast)
{
  q8_24 incr = Q24(20.0 / 1000.0);
  unsigned max_error = 0;
  float max_time = 0.0f;
  for(q8_24 k = Q24(-1.2); k < Q24(1.2); k += incr){
    q8_24 x = k;
    q8_24 y;
    double xf = F24(x);
    double expected_f = 1.0 / (1 + exp(-xf));
    q8_24 expected_q24 = Q24(expected_f);
    TIME_STATEMENT(y = q24_logistic_fast(x), timing);
    unsigned error = abs(expected_q24 - y);
    max_error = (error > max_error)? error : max_error;
    max_time = (timing > max_time)? timing : max_time;
    if(error > 112000){
      printf("x = %f  (%ld << -24)\n", xf, (long int) x);
      printf("exp = %f  (%ld << -24)\n", expected_f, (long int) expected_q24);
      printf("y = %f  (%ld << -24)\n", ldexp(y, -24), (long int) y);
      printf("error = %d\n", error);
    }
    TEST_ASSERT_INT32_WITHIN(112000, expected_q24, y);
  }
  // printf("max_time: %f us\n", max_time);
  // printf("max_error: %u\n", max_error);
}