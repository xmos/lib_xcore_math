// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "xmath/xmath.h"
#include "../tst_common.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(fixed_trig) {
  RUN_TEST_CASE(fixed_trig, radians_to_sbrads);
  RUN_TEST_CASE(fixed_trig, sbrad_sin);
  RUN_TEST_CASE(fixed_trig, sbrad_tan);
  RUN_TEST_CASE(fixed_trig, q24_sin);
  RUN_TEST_CASE(fixed_trig, q24_cos);
  RUN_TEST_CASE(fixed_trig, q24_tan);
}

TEST_GROUP(fixed_trig);
TEST_SETUP(fixed_trig) { fflush(stdout); }
TEST_TEAR_DOWN(fixed_trig) {}


#if SMOKE_TEST
#  define REPS       (1000)
#else
#  define REPS       (10000)
#endif



TEST(fixed_trig, radians_to_sbrads)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  for(unsigned int v = 0; v < REPS; v++){

    setExtraInfo_RS(v, seed);

    // sbrad_sin() takes only non-negative q31's in the range [0,1)

    int32_t theta_q24 = pseudo_rand_int32(&seed);
    // theta_q24 = 0;

    double theta = ldexp(theta_q24, -24);
    int32_t thing = (int32_t) floor(ldexp(theta, 24));
    TEST_ASSERT_EQUAL_INT32(theta_q24, thing);

    // Convert to normalized space
    double exp = theta * 2.0 / M_PI;
    int out_mul = 1;

    // Odd symmetry
    if(exp < 0){
      exp *= -1;
      out_mul *= -1;
    }

    // Periodicity
    exp = exp - 4.0 * floor(exp / 4.0);

    // sin(x + pi) == -sin(x)
    if(exp >= 2.0){
      exp -= 2.0;
      out_mul *= -1;
    }

    // symmetry about pi/2
    if(exp >= 1.0)
      exp = 2.0 - exp;
    
    TEST_ASSERT(exp >= 0.0);
    TEST_ASSERT(exp <= 1.0);

    // Convert float expectation to q31
    int32_t exp_q31 = (int32_t) floor(ldexp(exp, 31));
    TEST_ASSERT(exp_q31 >= 0);
    
    exp_q31 *= out_mul;

    // volatile uint32_t t0 = get_reference_time();
    int32_t actual_q31 = radians_to_sbrads(theta_q24);
    // volatile uint32_t t1 = get_reference_time();

    TEST_ASSERT_INT32_WITHIN_MESSAGE(30, exp_q31, actual_q31, "");

    // float mine = (float) ((t1-t0) / 100.0f);
    // printf("[timing] Mine: %0.02f us\n", mine);
  }

}



TEST(fixed_trig, sbrad_sin)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  int32_t max_error = 0;
  double max_error_pct = 0.0f;
  for(unsigned int v = 0; v < REPS; v++){

    setExtraInfo_RS(v, seed);

    double theta = ldexp(pseudo_rand_int32(&seed), -24);

    // sbrad_sin() takes q31's in the range -1 to 1
    int32_t theta_q24 = (int32_t) floor(ldexp(theta, 24));


    double exp = sin(theta);
    float theta_flt = (float) theta;
    // volatile uint32_t t0 = get_reference_time();
    volatile float just_for_timing = sinf(theta_flt);
    // volatile uint32_t t1 = get_reference_time();

    // volatile uint32_t t2 = get_reference_time();
    volatile int32_t alpha = radians_to_sbrads(theta_q24);
    volatile int32_t result_q30 = sbrad_sin(alpha);
    // volatile uint32_t t3 = get_reference_time();

    int32_t exp_q30 = lround(ldexp(exp,30));

    int32_t er = exp_q30 - result_q30;
    er = (er < 0)? -er : er;
    max_error = (er > max_error)? er : max_error;

    double erp = ((double)er) / abs(exp_q30);
    max_error_pct = (erp > max_error_pct)? erp : max_error_pct;

    TEST_ASSERT_INT32_WITHIN(10000, exp_q30, result_q30);

    // float ref  = (t1-t0) / 100.0f;
    // float mine = (t3-t2) / 100.0f;

    just_for_timing;
    // printf("\n[timing] Mine: %0.02f us\tRef: %0.02f us\n", mine, ref);
  }

  // printf("max_error = %d\n", max_error);
  // printf("max_error = %0.04f%%\n", 100.0f*max_error_pct);

}


TEST(fixed_trig, sbrad_tan)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  int32_t max_error = 0;
  double max_error_pct = 0.0f;
  for(unsigned int v = 0; v < REPS; v++){

    setExtraInfo_RS(v, seed);

    // sbrad_tan() takes angles in the interval [0, pi/4]
    //   which are represented as a normalized angle in interval [0, 0.5] (Q31)

    double alpha = ldexp(pseudo_rand_int32(&seed), -32);

    switch(v){
      case 0: alpha = 0.0;  break;
      case 1: alpha = 0.5;  break;
      default: break;
    }

    // printf("alpha = %f\n", alpha);

    double theta = alpha * M_PI / 2.0;

    q8_24 theta_q24 = (q8_24) floor(ldexp(theta, 24));

    double expected = tan(theta);

    float theta_flt = (float) theta;
    // volatile uint32_t t0 = get_reference_time();
    volatile float just_for_timing = tanf(theta_flt);
    // volatile uint32_t t1 = get_reference_time();

    // volatile uint32_t t2 = get_reference_time();
    volatile q1_31 alpha_q31 = radians_to_sbrads(theta_q24);
    volatile q2_30 result_q30 = sbrad_tan(alpha_q31);
    // volatile uint32_t t3 = get_reference_time();

    q2_30 expected_q30 = lround(ldexp(expected,30));

    // q2_30 just_for_timing_q30 = round(ldexp(just_for_timing, 30));

    q2_30 er = expected_q30 - result_q30;
    er = (er < 0)? -er : er;
    max_error = (er > max_error)? er : max_error;

    double erp = ((double)er) / abs(expected_q30);
    max_error_pct = (erp > max_error_pct)? erp : max_error_pct;

    // Unfortunately the error can be a bit large compared to sin() and cos()
    TEST_ASSERT_INT32_WITHIN_MESSAGE(1000, expected_q30, result_q30,"");

    // float ref  = (t1-t0) / 100.0f;
    // float mine = (t3-t2) / 100.0f;

    just_for_timing;
    // printf("[alpha = %f] Mine: %0.02f us\tRef: %0.02f us\terror: %ld\t error(%): %f%%\n", alpha, mine, ref, expected_q30 - result_q30, erp*100.0);
  }

  // printf("\nmax_error = %d\n", max_error);
  // printf("max_error = %0.04f%%\n", 100.0f*max_error_pct);

}


TEST(fixed_trig, q24_sin)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  int32_t max_error = 0;
  double max_error_pct = 0.0f;
  for(unsigned int v = 0; v < REPS; v++){

    setExtraInfo_RS(v, seed);

    double theta = ldexp(pseudo_rand_int32(&seed), -24);

    q8_24 theta_q24 = (q8_24) floor(ldexp(theta, 24));

    double exp = sin(theta);

    // volatile uint32_t t0 = get_reference_time();
    volatile float just_for_timing = sinf((float) theta);
    // volatile uint32_t t1 = get_reference_time();

    // volatile uint32_t t2 = get_reference_time();
    volatile q2_30 result_q30 = q24_sin(theta_q24);
    // volatile uint32_t t3 = get_reference_time();

    q2_30 exp_q30 = lround(ldexp(exp,30));

    q2_30 er = exp_q30 - result_q30;
    er = (er < 0)? -er : er;
    max_error = (er > max_error)? er : max_error;

    double erp = ((double)er) / abs(exp_q30);
    max_error_pct = (erp > max_error_pct)? erp : max_error_pct;

    TEST_ASSERT_INT32_WITHIN(10000, exp_q30, result_q30);

    // float ref  = (t1-t0) / 100.0f;
    // float mine = (t3-t2) / 100.0f;

    just_for_timing;
    // printf("[timing] Mine: %0.02f us\tRef: %0.02f us\n", mine, ref);
  }

  // printf("max_error = %d\n", max_error);
  // printf("max_error = %0.04f%%\n", 100.0f*max_error_pct);

}


TEST(fixed_trig, q24_cos)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  int32_t max_error = 0;
  double max_error_pct = 0.0f;
  for(unsigned int v = 0; v < REPS; v++){

    setExtraInfo_RS(v, seed);

    double theta = ldexp(pseudo_rand_int32(&seed), -24);

    // sbrad_sin() takes q31's in the range -1 to 1
    q8_24 theta_q24 = (q8_24) floor(ldexp(theta, 24));


    double exp = cos(theta);
    // volatile uint32_t t0 = get_reference_time();
    volatile float just_for_timing = cosf((float) theta);
    // volatile uint32_t t1 = get_reference_time();

    // volatile uint32_t t2 = get_reference_time();
    volatile q2_30 result_q30 = q24_cos(theta_q24);
    // volatile uint32_t t3 = get_reference_time();

    q2_30 exp_q30 = lround(ldexp(exp,30));

    q2_30 er = exp_q30 - result_q30;
    er = (er < 0)? -er : er;
    max_error = (er > max_error)? er : max_error;

    double erp = ((double)er) / abs(exp_q30);
    max_error_pct = (erp > max_error_pct)? erp : max_error_pct;

    TEST_ASSERT_INT32_WITHIN(10000, exp_q30, result_q30);

    // float ref  = (t1-t0) / 100.0f;
    // float mine = (t3-t2) / 100.0f;

    just_for_timing;
    // printf("[timing] Mine: %0.02f us\tRef: %0.02f us\n", mine, ref);
  }

  // printf("max_error = %d\n", max_error);
  // printf("max_error = %0.04f%%\n", 100.0f*max_error_pct);

}


TEST(fixed_trig, q24_tan)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  int32_t max_error = 0;
  double max_error_pct = 0.0f;
  for(unsigned int v = 0; v < REPS; v++){

    setExtraInfo_RS(v, seed);

    double theta = ldexp(pseudo_rand_int32(&seed), -24);

    q8_24 theta_q24 = (q8_24) floor(ldexp(theta, 24));

    double exp = tan(theta);
    // volatile uint32_t t0 = get_reference_time();
    volatile float just_for_timing = tanf((float) theta);
    // volatile uint32_t t1 = get_reference_time();

    // volatile uint32_t t2 = get_reference_time();
    volatile float_s32_t result = q24_tan(theta_q24);
    // volatile uint32_t t3 = get_reference_time();

    int32_t exp_fixed = lround(ldexp(exp,-result.exp));

    if(result.exp != -30)
      TEST_ASSERT_LESS_THAN_INT32(2, HR_S32(result.mant));

    if(fabs(exp) < 900) {
      // Unless we're very near the singular point, check that we're not far off
      TEST_ASSERT_INT32_WITHIN(10000, exp_fixed, result.mant);
      
      int32_t er = exp_fixed - result.mant;
      er = (er < 0)? -er : er;
      max_error = (er > max_error)? er : max_error;

      double erp = ((double)er) / abs(exp_fixed);
      max_error_pct = (erp > max_error_pct)? erp : max_error_pct;
    } else {
      // If we're really that close to the singular point, let's invert both the
      // expected result and actual result to see whether the output of sbrad_tan()
      // was very close to what it should have been.
      int32_t exp_inv_q30 = lround(ldexp(1/exp, 30));
      int32_t act_inv_q30 = lround(ldexp(1/ldexp(result.mant, result.exp), 30));

      TEST_ASSERT_INT32_WITHIN(100, exp_inv_q30, act_inv_q30);
      
      int32_t er = exp_inv_q30 - act_inv_q30;
      er = (er < 0)? -er : er;
      max_error = (er > max_error)? er : max_error;

      double erp = ((double)er) / abs(exp_inv_q30);
      max_error_pct = (erp > max_error_pct)? erp : max_error_pct;
    }

    just_for_timing;
  }

  // printf("max_error = %d\n", max_error);
  // printf("max_error = %0.04f%%\n", 100.0f*max_error_pct);
}