// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xmath/xmath.h"

#include "xmath/xs3/vpu_scalar_ops.h"

#include "../src/vect/vpu_helper.h"

#include "../tst_common.h"

#include "unity_fixture.h"

TEST_GROUP_RUNNER(vect_convolve) {
  RUN_TEST_CASE(vect_convolve, vect_s32_convolve_valid);
  RUN_TEST_CASE(vect_convolve, vect_s32_convolve_same_reflected);
  RUN_TEST_CASE(vect_convolve, vect_s32_convolve_same_zero);
  RUN_TEST_CASE(vect_convolve, vect_s32_convolve_same_extend);
}

TEST_GROUP(vect_convolve);
TEST_SETUP(vect_convolve) { fflush(stdout); }
TEST_TEAR_DOWN(vect_convolve) {}



#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (63)
#else
#  define REPS       (1000)
#  define MAX_LEN    (255)
#endif


TEST(vect_convolve, vect_s32_convolve_valid)
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

    int64_t tap_total = vect_s32_sum(taps, tap_count);

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

    headroom_t hr = vect_s32_convolve_valid(signal_out, signal_in, taps, length, tap_count);

    TEST_ASSERT_EQUAL_MESSAGE(vect_s32_headroom(signal_out, out_length), hr, "");

    XTEST_ASSERT_VECT_S32_EQUAL(expected, signal_out, out_length,
      "%s", "wrong");

  }
}


TEST(vect_convolve, vect_s32_convolve_same_reflected)
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

    int64_t tap_total = vect_s32_sum(taps, tap_count);

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

    headroom_t hr = vect_s32_convolve_same(signal_out, signal_in, taps, length, tap_count, PAD_MODE_REFLECT);

    TEST_ASSERT_EQUAL_MESSAGE(vect_s32_headroom(signal_out, length), hr, "");

    XTEST_ASSERT_VECT_S32_EQUAL(expected, signal_out, length,
      "%s", "wrong");

  }
}


TEST(vect_convolve, vect_s32_convolve_same_zero)
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
    //const unsigned tap_count = 1;

    //const unsigned length = 63;
    setExtraInfo_RSL(rep, old_seed, length);

    const int P = tap_count >> 1;

    right_shift_t shr = pseudo_rand_uint(&seed, 0, 6);
    //right_shift_t shr=5;

    printf("    const unsigned tap_count = %d;\n    const unsigned length = %d;\n    right_shift_t shr=%d;\n", tap_count, length, shr);

    for(int k = 0; k < length; k++) {
      signal_in[k] = pseudo_rand_int32(&seed) >> shr;
      //printf("    signal_in[%d] = %d;\n", k, signal_in[k]);
    }
    for(int k = 0; k < tap_count; k++){
      taps[k] = pseudo_rand_uint32(&seed) >> 1;
      //printf("    taps[%d] = %d;\n", k, taps[k]);
    }
/*
     signal_in[0] = 27443839;

     signal_in[1] = 37806704;

     signal_in[2] = 11882645;

     signal_in[3] = 17410903;

     signal_in[4] = 7217189;

     signal_in[5] = 61962985;

     signal_in[6] = 38724151;

     signal_in[7] = 64154158;

     signal_in[8] = 21516996;

     signal_in[9] = 40987002;

     signal_in[10] = 18946605;

     signal_in[11] = 35310583;

     signal_in[12] = 63199409;

     signal_in[13] = 39354441;

     signal_in[14] = 65550997;

     signal_in[15] = 60240099;

     signal_in[16] = 14887023;

     signal_in[17] = 23360124;

     signal_in[18] = 28416138;

     signal_in[19] = 17918509;

     signal_in[20] = 59251100;

     signal_in[21] = 17455281;

     signal_in[22] = 42324624;

     signal_in[23] = 1009998;

     signal_in[24] = 62358185;

     signal_in[25] = 59540419;

     signal_in[26] = 61182923;

     signal_in[27] = 62267406;

     signal_in[28] = 26167955;

     signal_in[29] = 47931642;

     signal_in[30] = 2183619;

     signal_in[31] = 43944151;

     signal_in[32] = 50735676;

     signal_in[33] = 14475582;

     signal_in[34] = 16454012;

     signal_in[35] = 36256478;

     signal_in[36] = 29708691;

     signal_in[37] = 18245671;

     signal_in[38] = 14625292;

     signal_in[39] = 45431772;

     signal_in[40] = 56357425;

     signal_in[41] = 14132138;

     signal_in[42] = 66188010;

     signal_in[43] = 24185640;

     signal_in[44] = 1907474;

     signal_in[45] = 1178045;

     signal_in[46] = 56136547;

     signal_in[47] = 55110384;

     signal_in[48] = 23003314;

     signal_in[49] = 36293304;

     signal_in[50] = 59074835;

     signal_in[51] = 9701924;

     signal_in[52] = 56904370;

     signal_in[53] = 29552662;

     signal_in[54] = 46299866;

     signal_in[55] = 15339911;

     signal_in[56] = 42075068;

     signal_in[57] = 11410230;

     signal_in[58] = 47524403;

     signal_in[59] = 22243700;

     signal_in[60] = 6871363;

     signal_in[61] = 4698805;

     signal_in[62] = 46342185;

    taps[0] = 202024044;
*/
    int64_t tap_total = vect_s32_sum(taps, tap_count);

    float scale = ldexpf(1, 30) / tap_total;

    for(int k = 0; k < tap_count; k++)
      taps[k] *= scale;

    for(int k = 0; k < length; k++){
      vpu_int32_acc_t acc = 0;
      for(int t = 0; t < tap_count; t++) {
        int i = k + t - P;
        int32_t input = 0;
        if( (i >= 0) && (i < length) )
          input = signal_in[ i ];
        acc = vlmacc32(acc, input, taps[t]);
      }
      expected[k] = acc;
    }

    memset(signal_out, 0, sizeof(signal_out));

    headroom_t hr = vect_s32_convolve_same(signal_out, signal_in, taps, length, tap_count, PAD_MODE_ZERO);

    TEST_ASSERT_EQUAL_MESSAGE(vect_s32_headroom(signal_out, length), hr, "");

    XTEST_ASSERT_VECT_S32_EQUAL(expected, signal_out, length,
      "Tap count: %u\n\n", tap_count);

  }
}


TEST(vect_convolve, vect_s32_convolve_same_extend)
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

    int64_t tap_total = vect_s32_sum(taps, tap_count);

    float scale = ldexpf(1, 30) / tap_total;

    for(int k = 0; k < tap_count; k++)
      taps[k] *= scale;

    for(int k = 0; k < length; k++){
      vpu_int32_acc_t acc = 0;
      for(int t = 0; t < tap_count; t++) {
        int i = k + t - P;
        int32_t input = 0;
        if( i < 0 )             input = signal_in[0];
        else if( i >= length )  input = signal_in[length - 1];
        else input = signal_in[ i ];
        acc = vlmacc32(acc, input, taps[t]);
      }
      expected[k] = acc;
    }

    memset(signal_out, 0, sizeof(signal_out));

    headroom_t hr = vect_s32_convolve_same(signal_out, signal_in, taps, length, tap_count, PAD_MODE_EXTEND);
    // printf("\n\n\n");
    // for(int i = 0; i < length; i++)
    //   printf("signal_in[%d] = 0x%08X\n", i, signal_in[i]);
    // printf("\n\n\n");

    TEST_ASSERT_EQUAL_MESSAGE(vect_s32_headroom(signal_out, length), hr, "");

    XTEST_ASSERT_VECT_S32_EQUAL(expected, signal_out, length,
      "Tap count: %u\n\n", tap_count);

  }
}

