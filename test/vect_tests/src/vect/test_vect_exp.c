// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdarg.h>
#include "xmath/xmath.h"
#include "../tst_common.h"
#include "xmath/xs3/vpu_scalar_ops.h"
#include "unity_fixture.h"


TEST_GROUP_RUNNER(vect_exp) {
  RUN_TEST_CASE(vect_exp, chunk_q30_exp_small_RANDOM);
  RUN_TEST_CASE(vect_exp, vect_q30_exp_small_RANDOM);
}

TEST_GROUP(vect_exp);
TEST_SETUP(vect_exp) { fflush(stdout); }
TEST_TEAR_DOWN(vect_exp) {}

#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (64)
#else
#  define REPS       (1000)
#  define MAX_LEN    (256)
#endif
    


TEST(vect_exp, chunk_q30_exp_small_RANDOM)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  int32_t B[VPU_INT32_EPV];
  int32_t A[VPU_INT32_EPV];

  int32_t expected[VPU_INT32_EPV];

  int max_max_error = 0;

  for(unsigned int v = 0; v < REPS; v++){
    setExtraInfo_RS(v, seed);

    unsigned length = VPU_INT32_EPV;

    const exponent_t b_exp = -30;

    for(unsigned int i = 0; i < length; i++){
      B[i] = pseudo_rand_int32(&seed) / 4;
    }

    for(unsigned int i = 0; i < length; i++){
      double bi = ldexp(B[i], b_exp);
      double exp_dbl = exp(bi);
      expected[i] = lround(ldexp(exp_dbl, 30));
    }

    // volatile uint32_t t0 = get_reference_time();
    chunk_q30_exp_small(A, B);
    // volatile uint32_t t1 = get_reference_time();

    // uint32_t delta_ticks = t1 - t0;
    // float delta_us = (float) ( delta_ticks / 100.0);
    // float delta_us_per_elm = delta_us / length;

    // printf("!! B[0] = %f (0x%08X << -30);  ", ldexp(B[0], -30), (unsigned) B[0]);
    // printf("expected: %f (0x%08X << -30);  ", ldexp(expected[0], -30), (unsigned) expected[0]);
    // printf("A[0] = %f (0x%08X << -30) \n", ldexp(A[0], -30), (unsigned) A[0]);

    int max_error = 0;
    for(int i = 0; i < VPU_INT32_EPV; i++){
      int32_t error = abs(expected[i] - A[i]);
      max_error = (error > max_error)? error : max_error;

      TEST_ASSERT_INT32_WITHIN(3, expected[i], A[i]);
    }

    // printf("Len: % 3u;  time: % 7.02f us;      time/elm:  % 7.02f us;   max error: %u\n",
    //     length, delta_us, delta_us_per_elm, max_error);

    max_max_error = (max_error > max_max_error)? max_error : max_max_error;
  }

  // printf("Max max error: %u\n", max_max_error);
}








TEST(vect_exp, vect_q30_exp_small_RANDOM)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  int32_t B[MAX_LEN];
  int32_t A[MAX_LEN];

  int32_t expected[MAX_LEN];

  int max_max_error = 0;

  for(unsigned int v = 0; v < REPS; v++){
    setExtraInfo_RS(v, seed);

    unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);

    const exponent_t b_exp = -30;

    for(unsigned int i = 0; i < length; i++){
      B[i] = pseudo_rand_int32(&seed) / 4;
    }

    for(unsigned int i = 0; i < length; i++){
      double bi = ldexp(B[i], b_exp);
      double exp_dbl = exp(bi);
      expected[i] = lround(ldexp(exp_dbl, 30));
    }

    // volatile uint32_t t0 = get_reference_time();
    vect_q30_exp_small(A, B, length);
    // volatile uint32_t t1 = get_reference_time();

    // uint32_t delta_ticks = t1 - t0;
    // float delta_us = (float) (delta_ticks / 100.0);
    // float delta_us_per_elm = delta_us / length;

    // printf("!! B[0] = %f (0x%08X << -30);  ", ldexp(B[0], -30), (unsigned) B[0]);
    // printf("expected: %f (0x%08X << -30);  ", ldexp(expected[0], -30), (unsigned) expected[0]);
    // printf("A[0] = %f (0x%08X << -30) \n", ldexp(A[0], -30), (unsigned) A[0]);

    int max_error = 0;
    for(unsigned int i = 0; i < length; i++){
      int32_t error = abs(expected[i] - A[i]);
      max_error = (error > max_error)? error : max_error;

      TEST_ASSERT_INT32_WITHIN(4, expected[i], A[i]);
    }

    // printf("Len: % 3u;  time: % 7.02f us;      time/elm:  % 7.02f us;   max error: %u\n",
    //     length, delta_us, delta_us_per_elm, max_error);

    max_max_error = (max_error > max_max_error)? max_error : max_max_error;
  }

  // printf("Max max error: %u\n", max_max_error);
}

