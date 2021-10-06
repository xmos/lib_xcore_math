// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xs3_math.h"

#include "../src/vect/vpu_helper.h"

#include "../../tst_common.h"

#include "unity_fixture.h"


TEST_GROUP_RUNNER(xs3_vect_complex_conjugate) {
  RUN_TEST_CASE(xs3_vect_complex_conjugate, xs3_vect_complex_s32_conjugate);
}

TEST_GROUP(xs3_vect_complex_conjugate);
TEST_SETUP(xs3_vect_complex_conjugate) { fflush(stdout); }
TEST_TEAR_DOWN(xs3_vect_complex_conjugate) {}




#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (65)
#else
#  define REPS       (1000)
#  define MAX_LEN    (257)
#endif

TEST(xs3_vect_complex_conjugate, xs3_vect_complex_s32_conjugate)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  complex_s32_t WORD_ALIGNED B[MAX_LEN];
  complex_s32_t WORD_ALIGNED A[MAX_LEN];

  complex_s32_t expected[MAX_LEN];

  for(int r = 0; r < REPS; r++){
    const unsigned old_seed = seed;

    unsigned length = pseudo_rand_int(&seed, 1, MAX_LEN+1);

    setExtraInfo_RSL(r, old_seed, length);

    for(int k = 0; k < length; k++){
      B[k].re = pseudo_rand_int32(&seed);
      B[k].im = pseudo_rand_int32(&seed);

      expected[k].re = SAT32( B[k].re );
      expected[k].im = SAT32( -((int64_t) B[k].im) );
    }
    
    xs3_vect_complex_s32_conjugate(A, B, length);

    for(int k = 0; k < length; k++){
      TEST_ASSERT_EQUAL(expected[k].re, A[k].re);
      TEST_ASSERT_EQUAL(expected[k].im, A[k].im);
    }
  }
}
#undef REPS

