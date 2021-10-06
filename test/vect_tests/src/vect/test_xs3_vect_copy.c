// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xs3_math.h"
#include "xs3_vpu_scalar_ops.h"

#include "../tst_common.h"
#include "unity_fixture.h"

TEST_GROUP(xs3_vect_copy);
TEST_SETUP(xs3_vect_copy) { fflush(stdout); }
TEST_TEAR_DOWN(xs3_vect_copy) {}

TEST_GROUP_RUNNER(xs3_vect_copy) {
  RUN_TEST_CASE(xs3_vect_copy, xs3_vect_s32_copy);
}



#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_VECTS  (10)
#else
#  define REPS       (1000)
#  define MAX_VECTS  (40)
#endif

#define MAX_LEN     (MAX_VECTS * 8)

TEST(xs3_vect_copy, xs3_vect_s32_copy)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  int32_t WORD_ALIGNED A[MAX_LEN];
  int32_t WORD_ALIGNED B[MAX_LEN];

  for(int v = 0; v < REPS; v++){
    unsigned old_seed = seed;
    unsigned len = 8*pseudo_rand_uint(&seed, 1, MAX_VECTS+1);

    setExtraInfo_RSL(v, old_seed, len);

    headroom_t b_hr = pseudo_rand_uint(&seed, 0, 20);
    
    for(int i = 0; i < len; i++)
        B[i] = pseudo_rand_int32(&seed) >> b_hr;

    
    headroom_t hr = xs3_vect_s32_copy(A, B, len);

    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(B, A, len, "");
  }
}

