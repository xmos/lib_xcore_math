// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <xcore/hwtimer.h>

#include "xs3_math.h"

#include "../tst_common.h"

#include "unity_fixture.h"

TEST_GROUP_RUNNER(float_log) {
  RUN_TEST_CASE(float_log, xs3_f32_log2);
}

TEST_GROUP(float_log);
TEST_SETUP(float_log) { fflush(stdout); }
TEST_TEAR_DOWN(float_log) {}


#if SMOKE_TEST
#  define REPS       (100)
#else
#  define REPS       (1000)
#endif


TEST(float_log, xs3_f32_log2)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  for(int v = 0; v < REPS; v++){
    setExtraInfo_RS(v, seed);

    float x = ldexp(pseudo_rand_uint(&seed, 1, INT32_MAX), 
      pseudo_rand_int(&seed, -10, 10));

    float actual, exp;

    TIME_STATEMENT(actual = xs3_f32_log2(x), delta_us);
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