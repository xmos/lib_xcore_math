// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xs3_math.h"

#include "xs3_vpu_scalar_ops.h"

#include "../src/vect/vpu_helper.h"

#include "../tst_common.h"

#include "unity_fixture.h"

TEST_GROUP_RUNNER(xs3_vect_convolve) {
  RUN_TEST_CASE(xs3_vect_convolve, xs3_vect_s32_convolve_valid);
  RUN_TEST_CASE(xs3_vect_convolve, xs3_vect_s32_convolve_same_reflected);
  RUN_TEST_CASE(xs3_vect_convolve, xs3_vect_s32_convolve_same_zero);
  RUN_TEST_CASE(xs3_vect_convolve, xs3_vect_s32_convolve_same_extend);
}

TEST_GROUP(xs3_vect_convolve);
TEST_SETUP(xs3_vect_convolve) { fflush(stdout); }
TEST_TEAR_DOWN(xs3_vect_convolve) {}



#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (63)
#else
#  define REPS       (1000)
#  define MAX_LEN    (255)
#endif


TEST(xs3_vect_convolve, xs3_vect_s32_convolve_valid)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  // Tap count must be one of {1,3,5,7}
  const unsigned ALLOWED_TAPS[4] = {1,3,5,7};

  int32_t WORD_ALIGNED signal_in[MAX_LEN];
  int32_t WORD_ALIGNED signal_out[MAX_LEN];

  int32_t WORD_ALIGNED taps[7];

  int32_t expected[MAX_LEN];

  for(int rep = 0; rep < REPS; rep++) {

    const unsigned tap_count = ALLOWED_TAPS[pseudo_rand_uint(&seed, 0, 4)];
    const unsigned length = pseudo_rand_uint(&seed, tap_count, MAX_LEN+1);

    right_shift_t shr = pseudo_rand_uint(&seed, 0, 6);
    for(int k = 0; k < length; k++)
      signal_in[k] = pseudo_rand_int32(&seed) >> shr;

    for(int k = 0; k < tap_count; k++)
      taps[k] = pseudo_rand_uint32(&seed) >> 1;

    int64_t tap_total = xs3_vect_s32_sum(taps, tap_count);

    float scale = ldexpf(1, 30) / tap_total;

    for(int k = 0; k < tap_count; k++)
      taps[k] *= scale;

    const unsigned out_length = length - ((tap_count>>1)<<1);

    for(int k = 0; k < out_length; k++){
      vpu_int32_acc_t acc = 0;
      for(int p = 0; p < tap_count; p++)
        acc = vlmacc32(acc, signal_in[k+p], taps[p]);
      expected[k] = acc;
    }
    
    headroom_t hr = xs3_vect_s32_convolve_valid(signal_out, signal_in, taps, length, tap_count);

    TEST_ASSERT_EQUAL_MESSAGE(xs3_vect_s32_headroom(signal_out, out_length), hr, "");

    XTEST_ASSERT_VECT_S32_EQUAL(expected, signal_out, out_length,
      "%s", "wrong");

  }
}


TEST(xs3_vect_convolve, xs3_vect_s32_convolve_same_reflected)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  // Tap count must be one of {1,3,5,7}
  const unsigned ALLOWED_TAPS[4] = {1,3,5,7};

  int32_t WORD_ALIGNED signal_in[MAX_LEN];
  int32_t WORD_ALIGNED signal_out[MAX_LEN];

  int32_t WORD_ALIGNED taps[7];

  int32_t expected[MAX_LEN];

  for(int rep = 0; rep < REPS; rep++) {

    const unsigned tap_count = ALLOWED_TAPS[pseudo_rand_uint(&seed, 0, 4)];
    const unsigned length = pseudo_rand_uint(&seed, tap_count, MAX_LEN+1);

    const int P = tap_count >> 1;

    right_shift_t shr = pseudo_rand_uint(&seed, 0, 6);
    for(int k = 0; k < length; k++)
      signal_in[k] = pseudo_rand_int32(&seed) >> shr;

    for(int k = 0; k < tap_count; k++)
      taps[k] = pseudo_rand_uint32(&seed) >> 1;

    int64_t tap_total = xs3_vect_s32_sum(taps, tap_count);

    float scale = ldexpf(1, 30) / tap_total;

    for(int k = 0; k < tap_count; k++)
      taps[k] *= scale;

    for(int k = 0; k < length; k++){
      vpu_int32_acc_t acc = 0;
      for(int t = 0; t < tap_count; t++) {
        int i = k + t - P;
        int32_t input = signal_in[ (i < 0)? (-i) : (i >= length)? (2*length - i - 2) : i ];
        acc = vlmacc32(acc, input, taps[t]);
      }
      expected[k] = acc;
    }

    memset(signal_out, 0, sizeof(signal_out));
    
    headroom_t hr = xs3_vect_s32_convolve_same(signal_out, signal_in, taps, length, tap_count, PAD_MODE_REFLECT);

    TEST_ASSERT_EQUAL_MESSAGE(xs3_vect_s32_headroom(signal_out, length), hr, "");

    XTEST_ASSERT_VECT_S32_EQUAL(expected, signal_out, length,
      "%s", "wrong");

  }
}


TEST(xs3_vect_convolve, xs3_vect_s32_convolve_same_zero)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  // Tap count must be one of {1,3,5,7}
  const unsigned ALLOWED_TAPS[4] = {1,3,5,7};

  int32_t WORD_ALIGNED signal_in[MAX_LEN];
  int32_t WORD_ALIGNED signal_out[MAX_LEN];   

  int32_t WORD_ALIGNED taps[7];

  int32_t expected[MAX_LEN];

  for(int rep = 0; rep < REPS; rep++) {
    const unsigned old_seed = seed;

    const unsigned tap_count = ALLOWED_TAPS[pseudo_rand_uint(&seed, 0, 4)];
    const unsigned length = pseudo_rand_uint(&seed, tap_count, MAX_LEN+1);

    setExtraInfo_RSL(rep, old_seed, length);

    const int P = tap_count >> 1;

    right_shift_t shr = pseudo_rand_uint(&seed, 0, 6);
    for(int k = 0; k < length; k++)
      signal_in[k] = pseudo_rand_int32(&seed) >> shr;

    for(int k = 0; k < tap_count; k++)
      taps[k] = pseudo_rand_uint32(&seed) >> 1;

    int64_t tap_total = xs3_vect_s32_sum(taps, tap_count);

    float scale = ldexpf(1, 30) / tap_total;

    for(int k = 0; k < tap_count; k++)
      taps[k] *= scale;

    for(int k = 0; k < length; k++){
      vpu_int32_acc_t acc = 0;
      for(int t = 0; t < tap_count; t++) {
        int i = k + t - P;
        int32_t input = signal_in[ i ];
        if( (i < 0) || (i >= length) )
          input = 0;
        acc = vlmacc32(acc, input, taps[t]);
      }
      expected[k] = acc;
    }

    memset(signal_out, 0, sizeof(signal_out));
    
    headroom_t hr = xs3_vect_s32_convolve_same(signal_out, signal_in, taps, length, tap_count, PAD_MODE_ZERO);

    TEST_ASSERT_EQUAL_MESSAGE(xs3_vect_s32_headroom(signal_out, length), hr, "");

    XTEST_ASSERT_VECT_S32_EQUAL(expected, signal_out, length,
      "Tap count: %u\n\n", tap_count);

  }
}


TEST(xs3_vect_convolve, xs3_vect_s32_convolve_same_extend)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  // Tap count must be one of {1,3,5,7}
  const unsigned ALLOWED_TAPS[4] = {1,3,5,7};

  int32_t WORD_ALIGNED signal_in[MAX_LEN];
  int32_t WORD_ALIGNED signal_out[MAX_LEN];   

  int32_t WORD_ALIGNED taps[7];

  int32_t expected[MAX_LEN];

  for(int rep = 0; rep < REPS; rep++) {
    const unsigned old_seed = seed;

    const unsigned tap_count = ALLOWED_TAPS[pseudo_rand_uint(&seed, 0, 4)];
    const unsigned length = pseudo_rand_uint(&seed, tap_count, MAX_LEN+1);

    setExtraInfo_RSL(rep, old_seed, length);

    const int P = tap_count >> 1;

    right_shift_t shr = pseudo_rand_uint(&seed, 0, 6);
    for(int k = 0; k < length; k++)
      signal_in[k] = pseudo_rand_int32(&seed) >> shr;

    for(int k = 0; k < tap_count; k++)
      taps[k] = pseudo_rand_uint32(&seed) >> 1;

    int64_t tap_total = xs3_vect_s32_sum(taps, tap_count);

    float scale = ldexpf(1, 30) / tap_total;

    for(int k = 0; k < tap_count; k++)
      taps[k] *= scale;

    for(int k = 0; k < length; k++){
      vpu_int32_acc_t acc = 0;
      for(int t = 0; t < tap_count; t++) {
        int i = k + t - P;
        int32_t input = signal_in[ i ];
        if( i < 0 )             input = signal_in[0];
        else if( i >= length )  input = signal_in[length - 1];
        acc = vlmacc32(acc, input, taps[t]);
      }
      expected[k] = acc;
    }

    memset(signal_out, 0, sizeof(signal_out));
    
    headroom_t hr = xs3_vect_s32_convolve_same(signal_out, signal_in, taps, length, tap_count, PAD_MODE_EXTEND);

    // printf("\n\n\n");
    // for(int i = 0; i < length; i++)
    //   printf("signal_in[%d] = 0x%08X\n", i, signal_in[i]);
    // printf("\n\n\n");

    TEST_ASSERT_EQUAL_MESSAGE(xs3_vect_s32_headroom(signal_out, length), hr, "");

    XTEST_ASSERT_VECT_S32_EQUAL(expected, signal_out, length,
      "Tap count: %u\n\n", tap_count);

  }
}

