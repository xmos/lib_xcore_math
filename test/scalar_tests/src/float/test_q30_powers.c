// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "xmath/xmath.h"
#include "../tst_common.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(q30_powers) {
  RUN_TEST_CASE(q30_powers, q30_powers);
}

TEST_GROUP(q30_powers);
TEST_SETUP(q30_powers) { fflush(stdout); }
TEST_TEAR_DOWN(q30_powers) {}


#if SMOKE_TEST
#  define REPS       (1000)
#else
#  define REPS       (10000)
#endif




TEST(q30_powers, q30_powers)
{
  unsigned seed = SEED_FROM_FUNC_NAME();
  // printf("\n");
  q2_30 B;
  q2_30 A[32];
  q2_30 expected[32];
  unsigned max_max_error = 0;

  for(unsigned int v = 0; v < REPS; v++){
    setExtraInfo_RS(v, seed);
    unsigned length = pseudo_rand_uint(&seed, 1, 32+1);
    const exponent_t b_exp = -30;
    B = pseudo_rand_int32(&seed) >> 1;
    expected[0] = 0x40000000;
    double bf = ldexp(B, b_exp);
    double pow = 1.0;
    for(unsigned int i = 1; i < length; i++){
      pow *= bf;
      expected[i] = lround(ldexp(pow, 30));
    }

    // volatile uint32_t t0 = get_reference_time();
    q30_powers(A, B, length);
    // volatile uint32_t t1 = get_reference_time();

    // uint32_t delta_ticks = t1 - t0;
    // float delta_us = (float) (delta_ticks / 100.0);
    // float delta_us_per_elm = delta_us / length;

    int max_error = 0;
    for(unsigned int i = 0; i < length; i++){
      int32_t error = abs(expected[i] - A[i]);
      max_error = (error > max_error)? error : max_error;

      // printf("[%d] %f  %f\n", i, ldexp(A[i], -30), ldexp(A[i], -30) * ldexp(B,-30));

      TEST_ASSERT_INT32_WITHIN(20, expected[i], A[i]);
    }

    // printf("Len: % 3u;  time: % 7.02f us;      time/elm:  % 7.02f us;   max error: %u\n",
    //     length, delta_us, delta_us_per_elm, max_error);

    max_max_error = MAX(max_max_error, (unsigned int) max_error);
  }

  // printf("Max max error: %u\n", max_max_error);
}






