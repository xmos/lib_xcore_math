// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xmath/xmath.h"

#include "../tst_common.h"
#include "unity_fixture.h"

TEST_GROUP(vect_copy);
TEST_SETUP(vect_copy) { fflush(stdout); }
TEST_TEAR_DOWN(vect_copy) {}

TEST_GROUP_RUNNER(vect_copy) {
  RUN_TEST_CASE(vect_copy, vect_s32_copy);
  RUN_TEST_CASE(vect_copy, vpu_memcpy_boundaries);
}



#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_VECTS  (10)
#else
#  define REPS       (1000)
#  define MAX_VECTS  (40)
#endif

#define MAX_LEN     (MAX_VECTS * 8)

TEST(vect_copy, vect_s32_copy)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  int32_t WORD_ALIGNED A[MAX_LEN];
  int32_t WORD_ALIGNED B[MAX_LEN];

  for(unsigned int v = 0; v < REPS; v++){
    unsigned old_seed = seed;
    unsigned len = 8*pseudo_rand_uint(&seed, 1, MAX_VECTS+1);

    setExtraInfo_RSL(v, old_seed, len);

    headroom_t b_hr = pseudo_rand_uint(&seed, 0, 20);

    for(unsigned int i = 0; i < len; i++)
        B[i] = pseudo_rand_int32(&seed) >> b_hr;

    vect_s32_copy(A, B, len);

    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(B, A, len, "");
  }
}

TEST(vect_copy, vpu_memcpy_boundaries)
{
  DWORD_ALIGNED
  uint8_t source[96];
  DWORD_ALIGNED
  uint8_t result[96];
  const unsigned lengths[] = { 0, 1, 7, 31, 32, 33, 63, 64, 65 };

  for (unsigned k = 0; k < sizeof(source); ++k)
    source[k] = (uint8_t) (k * 17);

  for (unsigned n = 0; n < sizeof(lengths) / sizeof(lengths[0]); ++n) {
    const unsigned length = lengths[n];
    memset(result, 0x5a, sizeof(result));
    vpu_memcpy(result, source, length);
    if (length)
      TEST_ASSERT_EQUAL_MEMORY(source, result, length);
    for (unsigned k = length; k < sizeof(result); ++k)
      TEST_ASSERT_EQUAL_UINT8(0x5a, result[k]);
  }
}
