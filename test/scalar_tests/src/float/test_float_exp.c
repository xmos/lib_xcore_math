// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "xmath/xmath.h"

#include "../tst_common.h"

#include "unity_fixture.h"

TEST_GROUP_RUNNER(float_exp) {
  RUN_TEST_CASE(float_exp, q30_exp_small);
  RUN_TEST_CASE(float_exp, float_s32_exp_SPECIFIC_CASES);
  RUN_TEST_CASE(float_exp, float_s32_exp_RANDOM);
}

TEST_GROUP(float_exp);
TEST_SETUP(float_exp) { fflush(stdout); }
TEST_TEAR_DOWN(float_exp) {}


#if SMOKE_TEST
#  define REPS       (1000)
#else
#  define REPS       (10000)
#endif



static
double diff_ratio(float_s32_t a, double b)
{
  double aa = ldexp(a.mant, a.exp);
  double diff = b - aa;
  double rat = fabs(diff / b);
  return rat;
}


TEST(float_exp, q30_exp_small)
{
  unsigned seed = SEED_FROM_FUNC_NAME();
  unsigned max_diff = 0;

  for(q2_30 x = Q30(-0.5); x <= Q30(0.5); x += Q30((1.0/REPS))){
    setExtraInfo_RS(x, seed);
    q2_30 actual;
    TIME_STATEMENT(actual = q30_exp_small(x), timing);
    double expected_f = exp(ldexp(x, -30));
    q2_30 expected = Q30(expected_f);
    unsigned int diff = abs(expected - actual);
    max_diff = MAX(diff, max_diff);
    TEST_ASSERT_INT32_WITHIN_MESSAGE(2, expected, actual, "");
  }
  // printf("max_diff = %u\n", max_diff);
}


TEST(float_exp, float_s32_exp_SPECIFIC_CASES)
{
  float_s32_t x, res;

  // exp(0.0) = 1.0
  x.mant = Q24(0.0);
  x.exp = -24;
  res = float_s32_exp(x);
  TEST_ASSERT( diff_ratio(res, 1.0) < ldexp(1,-24)  );

  // exp(0.0) = 1.0
  x.mant = Q28(0.0);
  x.exp = -28;
  res = float_s32_exp(x);
  TEST_ASSERT( diff_ratio(res, 1.0) < ldexp(1,-24)  );

  // exp(0.0) = 1.0
  x.mant = 0;
  x.exp = 4;
  res = float_s32_exp(x);
  TEST_ASSERT( diff_ratio(res, 1.0) < ldexp(1,-24)  );

  // exp(1.0) = e
  x.mant = Q24(1.0);
  x.exp = -24;
  res = float_s32_exp(x);
  TEST_ASSERT( diff_ratio(res, M_E) < ldexp(1,-24)  );

  // exp(1.0) = e
  x.mant = Q16(1.0);
  x.exp = -16;
  res = float_s32_exp(x);
  TEST_ASSERT( diff_ratio(res, M_E) < ldexp(1,-24)  );
  
  // exp(1.0) = e
  x.mant = 1;
  x.exp = 0;
  res = float_s32_exp(x);
  TEST_ASSERT( diff_ratio(res, M_E) < ldexp(1,-24)  );
  
  // exp(-1.0) = 1/e
  x.mant = Q24(-1.0);
  x.exp = -24;
  res = float_s32_exp(x);
  TEST_ASSERT( diff_ratio(res, (1 / M_E)) < ldexp(1,-24)  );

}


TEST(float_exp, float_s32_exp_RANDOM)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  for(unsigned int v = 0; v < REPS; v++){
    setExtraInfo_RS(v, seed);

    float_s32_t x = {
      pseudo_rand_int32(&seed),
      pseudo_rand_int(&seed, -37, -25)
    };

    float_s32_t actual = float_s32_exp(x);
    double expected_f = exp(ldexp(x.mant, x.exp));
    int32_t expected_mant = lround(ldexp(expected_f,-actual.exp));

    if(expected_mant == 0){
      // Just make sure our answer is real close to zero.
      TEST_ASSERT( fabs(ldexp(actual.mant, actual.exp)) < ldexp(1,-24) );
    } else {
      TEST_ASSERT_INT32_WITHIN_MESSAGE(200, expected_mant, actual.mant, "");
    }
  }
}
