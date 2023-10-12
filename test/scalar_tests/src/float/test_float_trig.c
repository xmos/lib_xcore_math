// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "xmath/xmath.h"

#include "../tst_common.h"

#include "unity_fixture.h"

TEST_GROUP_RUNNER(float_trig) {
  RUN_TEST_CASE(float_trig, f32_sin);
  RUN_TEST_CASE(float_trig, f32_cos);
}

TEST_GROUP(float_trig);
TEST_SETUP(float_trig) { fflush(stdout); }
TEST_TEAR_DOWN(float_trig) {}


#if SMOKE_TEST
#  define REPS       (100)
#else
#  define REPS       (1000)
#endif


TEST(float_trig, f32_sin)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  // printf("\n\n");

  for(unsigned int v = 0; v < REPS; v++){

    setExtraInfo_RS(v, seed);

    // -32.0 <= x < 32.0
    float x = ldexpf((float) pseudo_rand_int32(&seed), -26);

    // x = -5.4977f;

    // volatile uint32_t t0 = get_reference_time();
    float exp = sinf(x);
    // volatile uint32_t t1 = get_reference_time();

    // volatile uint32_t t2 = get_reference_time();
    float result = f32_sin(x);
    // volatile uint32_t t3 = get_reference_time();


    // Compare as Q31
    float diff = exp - result;

    TEST_ASSERT_FLOAT_WITHIN(ldexpf(1,-17), 0.0, diff);
    
    // float ref = (t1-t0) / 100.0f;
    // float mine = (t3-t2) / 100.0f;

    // printf("[timing] Mine: %0.04f us\tRef: %0.04f us\n", mine, ref);
  }


}


TEST(float_trig, f32_cos)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  // printf("\n\n");

  for(unsigned int v = 0; v < REPS; v++){

    setExtraInfo_RS(v, seed);

    // -32.0 <= x < 32.0
    float x = ldexpf((float) pseudo_rand_int32(&seed), -26);

    // x = -5.4977f;

    // volatile uint32_t t0 = get_reference_time();
    float exp = cosf(x);
    // volatile uint32_t t1 = get_reference_time();

    // volatile uint32_t t2 = get_reference_time();
    float result = f32_cos(x);
    // volatile uint32_t t3 = get_reference_time();


    // Compare as Q31
    float diff = exp - result;

    TEST_ASSERT_FLOAT_WITHIN(ldexpf(1,-17), 0.0, diff);
    
    // float ref = (t1-t0) / 100.0f;
    // float mine = (t3-t2) / 100.0f;

    // printf("[timing] Mine: %0.04f us\tRef: %0.04f us\n", mine, ref);
  }


}
