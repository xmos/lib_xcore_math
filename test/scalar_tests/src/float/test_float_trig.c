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

TEST_GROUP_RUNNER(float_trig) {
  RUN_TEST_CASE(float_trig, float_sin);
  RUN_TEST_CASE(float_trig, xs3_norm_angle);
  RUN_TEST_CASE(float_trig, xs3_norm_sin);
  RUN_TEST_CASE(float_trig, xs3_norm_cos);
  RUN_TEST_CASE(float_trig, xs3_norm_tan);
}

TEST_GROUP(float_trig);
TEST_SETUP(float_trig) { fflush(stdout); }
TEST_TEAR_DOWN(float_trig) {}


#if SMOKE_TEST
#  define REPS       (100)
#else
#  define REPS       (1000)
#endif


TEST(float_trig, float_sin)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  printf("\n\n");

  for(int v = 0; v < REPS; v++){

    setExtraInfo_RS(v, seed);

    // -32.0 <= x < 32.0
    float x = ldexpf(pseudo_rand_int32(&seed), -26);

    // x = -5.4977f;

    volatile uint32_t t0 = get_reference_time();
    float exp = sinf(x);
    volatile uint32_t t1 = get_reference_time();

    volatile uint32_t t2 = get_reference_time();
    float result = float_sin(x);
    volatile uint32_t t3 = get_reference_time();

    volatile uint32_t t4 = get_reference_time();
    float result2 = float_sin2(x);
    volatile uint32_t t5 = get_reference_time();


    // Compare as Q31
    float diff = exp - result;

    TEST_ASSERT_FLOAT_WITHIN(ldexpf(1,-17), 0.0, diff);
    
    float mine = (t3-t2) / 100.0f;
    float ref = (t1-t0) / 100.0f;
    float mine2 = (t5-t4) / 100.0f;

    // printf("[result] Mine: %0.04f   \tRef: %0.04f   \tMine2: %0.04f   \n", result, exp, result2);
    printf("[timing] Mine: %0.04f us\tRef: %0.04f us\tMine2: %0.04f us\n\n", mine, ref, mine2);
  }


}


TEST(float_trig, xs3_norm_angle)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  for(int v = 0; v < 10*REPS; v++){

    setExtraInfo_RS(v, seed);

    // xs3_norm_sin() takes only non-negative q31's in the range [0,1)

    int32_t theta_q24 = pseudo_rand_int32(&seed);
    // theta_q24 = 0;

    double theta = ldexp(theta_q24, -24);
    int32_t thing = floor(ldexp(theta, 24));
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
    int32_t exp_q31 = floor(ldexp(exp, 31));
    TEST_ASSERT(exp_q31 >= 0);
    
    exp_q31 *= out_mul;

    volatile uint32_t t0 = get_reference_time();
    int32_t actual_q31 = xs3_norm_angle(theta_q24);
    volatile uint32_t t1 = get_reference_time();

    TEST_ASSERT_INT32_WITHIN_MESSAGE(30, exp_q31, actual_q31, "");

    float mine = (t1-t0) / 100.0f;
    printf("[timing] Mine: %0.02f us\n", mine);
  }

}



TEST(float_trig, xs3_norm_sin)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  int32_t max_error = 0;
  double max_error_pct = 0.0f;
  for(int v = 0; v < 10*REPS; v++){

    setExtraInfo_RS(v, seed);

    double theta = ldexp(pseudo_rand_int32(&seed), -24);

    // xs3_norm_sin() takes q31's in the range -1 to 1
    int32_t theta_q24 = floor(ldexp(theta, 24));


    double exp = sin(theta);
    float theta_flt = theta;
    volatile uint32_t t0 = get_reference_time();
    volatile float just_for_timing = sinf(theta_flt);
    volatile uint32_t t1 = get_reference_time();

    volatile uint32_t t2 = get_reference_time();
    volatile int32_t alpha = xs3_norm_angle(theta_q24);
    volatile int32_t result_q30 = xs3_norm_sin(alpha);
    volatile uint32_t t3 = get_reference_time();

    volatile uint32_t t4 = get_reference_time();
    volatile float mine_flt = float_sin2(theta_flt);
    volatile uint32_t t5 = get_reference_time();

    int32_t exp_q30 = round(ldexp(exp,30));

    int32_t er = exp_q30 - result_q30;
    er = (er < 0)? -er : er;
    max_error = (er > max_error)? er : max_error;

    double erp = ((double)er) / abs(exp_q30);
    max_error_pct = (erp > max_error_pct)? erp : max_error_pct;

    TEST_ASSERT_INT32_WITHIN(10000, exp_q30, result_q30);

    float ref  = (t1-t0) / 100.0f;
    float mine = (t3-t2) / 100.0f;
    float mine2 = (t5-t4) / 100.0f;

    mine_flt;
    just_for_timing;
    printf("\n[timing] Mine: %0.02f us\tRef: %0.02f us\tMine2: %0.02f us\n", mine, ref, mine2);
  }

  printf("max_error = %d\n", max_error);
  printf("max_error = %0.04f%%\n", 100.0f*max_error_pct);

}


TEST(float_trig, xs3_norm_cos)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  int32_t max_error = 0;
  double max_error_pct = 0.0f;
  for(int v = 0; v < 10*REPS; v++){

    setExtraInfo_RS(v, seed);

    double theta = ldexp(pseudo_rand_int32(&seed), -24);

    // xs3_norm_sin() takes q31's in the range -1 to 1
    int32_t theta_q24 = floor(ldexp(theta, 24));


    double exp = cos(theta);
    float theta_flt = theta;
    volatile uint32_t t0 = get_reference_time();
    volatile float just_for_timing = cosf(theta_flt);
    volatile uint32_t t1 = get_reference_time();

    volatile uint32_t t2 = get_reference_time();
    volatile int32_t result_q30 = xs3_scalar_cos(theta_q24);
    volatile uint32_t t3 = get_reference_time();

    int32_t exp_q30 = round(ldexp(exp,30));

    int32_t er = exp_q30 - result_q30;
    er = (er < 0)? -er : er;
    max_error = (er > max_error)? er : max_error;

    double erp = ((double)er) / abs(exp_q30);
    max_error_pct = (erp > max_error_pct)? erp : max_error_pct;

    TEST_ASSERT_INT32_WITHIN(10000, exp_q30, result_q30);

    float ref  = (t1-t0) / 100.0f;
    float mine = (t3-t2) / 100.0f;

    just_for_timing;
    printf("[timing] Mine: %0.02f us\tRef: %0.02f us\n", mine, ref);
  }

  printf("max_error = %d\n", max_error);
  printf("max_error = %0.04f%%\n", 100.0f*max_error_pct);

}




TEST(float_trig, xs3_norm_tan)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  int32_t max_error = 0;
  double max_error_pct = 0.0f;
  for(int v = 0; v < 100*REPS; v++){

    setExtraInfo_RS(v, seed);

    // xs3_norm_tan() takes angles in the interval [0, pi/4]
    //   which are represented as a normalized angle in interval [0, 0.5] (Q31)

    double alpha = ldexp(pseudo_rand_int32(&seed), -32);

    switch(v){
      case 0: alpha = 0.0;  break;
      case 1: alpha = 0.5;  break;
      default: break;
    }

    // printf("alpha = %f\n", alpha);

    double theta = alpha * M_PI / 2.0;

    q8_24 theta_q24 = floor(ldexp(theta, 24));

    double expected = tan(theta);

    float theta_flt = theta;
    volatile uint32_t t0 = get_reference_time();
    volatile float just_for_timing = tanf(theta_flt);
    volatile uint32_t t1 = get_reference_time();

    volatile uint32_t t2 = get_reference_time();
    volatile q1_31 alpha_q31 = xs3_norm_angle(theta_q24);
    volatile q2_30 result_q30 = xs3_norm_tan(alpha_q31);
    volatile uint32_t t3 = get_reference_time();

    q2_30 expected_q30 = round(ldexp(expected,30));

    // q2_30 just_for_timing_q30 = round(ldexp(just_for_timing, 30));

    q2_30 er = expected_q30 - result_q30;
    er = (er < 0)? -er : er;
    max_error = (er > max_error)? er : max_error;

    double erp = ((double)er) / abs(expected_q30);
    max_error_pct = (erp > max_error_pct)? erp : max_error_pct;

    // Unfortunately the error can be a bit large compared to sin() and cos()
    TEST_ASSERT_INT32_WITHIN_MESSAGE(1000, expected_q30, result_q30,"");

    float ref  = (t1-t0) / 100.0f;
    float mine = (t3-t2) / 100.0f;

    just_for_timing;
    // printf("[alpha = %f] Mine: %0.02f us\tRef: %0.02f us\terror: %ld\t error(%): %f%%\n", alpha, mine, ref, expected_q30 - result_q30, erp*100.0);
  }

  printf("\nmax_error = %d\n", max_error);
  printf("max_error = %0.04f%%\n", 100.0f*max_error_pct);

}