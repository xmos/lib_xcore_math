// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xmath/xmath.h"

#include "../tst_common.h"

#include "unity_fixture.h"


TEST_GROUP_RUNNER(chunk_s16_accumulate) {
  RUN_TEST_CASE(chunk_s16_accumulate, chunk_s16_accumulate_NONPOS_SHR);
  RUN_TEST_CASE(chunk_s16_accumulate, chunk_s16_accumulate_POS_SHR);
}

TEST_GROUP(chunk_s16_accumulate);
TEST_SETUP(chunk_s16_accumulate) { fflush(stdout); }
TEST_TEAR_DOWN(chunk_s16_accumulate) {}



#if SMOKE_TEST
#  define REPS       (100)
#else
#  define REPS       (1000)
#endif



TEST(chunk_s16_accumulate, chunk_s16_accumulate_NONPOS_SHR)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  int16_t DWORD_ALIGNED B[VPU_INT16_EPV];
  split_acc_s32_t DWORD_ALIGNED acc;

  int32_t DWORD_ALIGNED expected[VPU_INT16_EPV];
  int32_t DWORD_ALIGNED actual[VPU_INT16_EPV];

  for(unsigned int v = 0; v < REPS; v++){
    const unsigned length = VPU_INT16_EPV;

    for(unsigned int k = 0; k < length; k++)
      B[k] = pseudo_rand_int16(&seed);

    // Generate random accumulator vector as int32_t's with 1 bit of headroom
    for(unsigned int k = 0; k < length; k++){
      expected[k] = pseudo_rand_int32(&seed) >> 1;
    }

    // printf("acc[0] = 0x%08X\n", expected[0]);

    // Turn the int32_t accs into split_s32 accs
    vect_s32_split_accs(&acc, expected, length);

    right_shift_t b_shr = pseudo_rand_int(&seed, -14, 0);

    for(unsigned int k = 0; k < length; k++){
      expected[k] += ((int32_t)B[k]) << (-b_shr);
    }

    // printf("B[0] = 0x%04X << %d\n", B[0], b_shr);
    // printf("expected[0] = 0x%08X\n", expected[0]);

    unsigned vpu_ctrl = VPU_INT16_CTRL_INIT;
    vpu_ctrl = chunk_s16_accumulate(&acc, B, b_shr, vpu_ctrl);

    vect_s32_merge_accs(actual, &acc, length);
    // printf("actual[0] = 0x%08X\n", actual[0]);

    TEST_ASSERT_EQUAL_INT32_ARRAY(expected, actual, length);
  }
}


TEST(chunk_s16_accumulate, chunk_s16_accumulate_POS_SHR)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  int16_t DWORD_ALIGNED B[VPU_INT16_EPV];
  split_acc_s32_t DWORD_ALIGNED acc;

  int32_t DWORD_ALIGNED expected[VPU_INT16_EPV];
  int32_t DWORD_ALIGNED actual[VPU_INT16_EPV];

  for(unsigned int v = 0; v < REPS; v++){
    const unsigned length = VPU_INT16_EPV;

    for(unsigned int k = 0; k < length; k++)
      B[k] = pseudo_rand_int16(&seed);

    // Generate random accumulator vector as int32_t's with 1 bit of headroom
    for(unsigned int k = 0; k < length; k++){
      expected[k] = pseudo_rand_int32(&seed) >> 1;
    }

    // printf("acc[0] = 0x%08X\n", expected[0]);

    // Turn the int32_t accs into split_s32 accs
    vect_s32_split_accs(&acc, expected, length);

    right_shift_t b_shr = pseudo_rand_int(&seed, 1, 15);

    for(unsigned int k = 0; k < length; k++){
      expected[k] += ((int32_t)B[k]) >> (b_shr);
    }

    // printf("B[0] = 0x%04X << %d\n", B[0], b_shr);

    // printf("expected[0] = 0x%08X\n", expected[0]);

    unsigned vpu_ctrl = VPU_INT16_CTRL_INIT;
    vpu_ctrl = chunk_s16_accumulate(&acc, B, b_shr, vpu_ctrl);

    vect_s32_merge_accs(actual, &acc, length);

    // printf("actual[0] = 0x%08X\n", actual[0]);

    TEST_ASSERT_INT32_ARRAY_WITHIN(1, expected, actual, length);
  }
}
