// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xmath/xmath.h"

#include "../../tst_common.h"

#include "unity_fixture.h"


TEST_GROUP_RUNNER(bfp_s16_accumulate) {
  RUN_TEST_CASE(bfp_s16_accumulate, bfp_s16_accumulate);
}

TEST_GROUP(bfp_s16_accumulate);
TEST_SETUP(bfp_s16_accumulate) { fflush(stdout); }
TEST_TEAR_DOWN(bfp_s16_accumulate) {}



#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (128)
#else
#  define REPS       (1000)
#  define MAX_LEN    (1024)
#endif



TEST(bfp_s16_accumulate, bfp_s16_accumulate)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  int16_t DWORD_ALIGNED b_data[MAX_LEN];
  split_acc_s32_t DWORD_ALIGNED acc[(MAX_LEN+VPU_INT16_EPV) / VPU_INT16_EPV];

  bfp_s16_t B;

  double expected_f[MAX_LEN];
  int32_t DWORD_ALIGNED expected[MAX_LEN];
  int32_t DWORD_ALIGNED actual[MAX_LEN];

  bfp_s16_init(&B, b_data, 0, 0, 0);

  for(unsigned int v = 0; v < REPS; v++){
    test_random_bfp_s16(&B, MAX_LEN, &seed, NULL, 0);
    B.length = pseudo_rand_int(&seed, 1, MAX_LEN+1);

    exponent_t acc_exp = pseudo_rand_int(&seed, -30, 30);

    B.exp = acc_exp + pseudo_rand_int(&seed, -14, 15);

    for(unsigned int k = 0; k < B.length; k++) {
      B.data[k] = pseudo_rand_int16(&seed);
    }

    // Generate random accumulator vector as int32_t's with 1 bit of headroom
    for(unsigned int k = 0; k < B.length; k++) {
      expected[k] = pseudo_rand_int32(&seed) >> 1;
    }

    // Turn the int32_t accs into split_s32 accs
    vect_s32_split_accs(&acc[0], expected, B.length);

    for(unsigned int k = 0; k < B.length; k++){
      expected_f[k] = ldexp(expected[k], acc_exp)
                    + ldexp(B.data[k], B.exp);
    }

    unsigned chunks = B.length >> VPU_INT16_EPV_LOG2;
    unsigned tail = B.length & (VPU_INT16_EPV - 1);

    for(int k = tail; k < VPU_INT16_EPV; k++){
      acc[chunks].vD[k] = 0;
      acc[chunks].vR[k] = 0;
    }

    headroom_t res_hr = bfp_s16_accumulate(acc, acc_exp, &B);

    vect_s32_merge_accs(actual, &acc[0], B.length);

    headroom_t exp_hr = vect_s32_headroom(actual, B.length);

    exp_hr = MIN(15, exp_hr);

    TEST_ASSERT_EQUAL(exp_hr, res_hr);
    test_s32_from_double(expected, expected_f, B.length, acc_exp);

    TEST_ASSERT_INT32_ARRAY_WITHIN(1, expected, actual, B.length);
  }
}

