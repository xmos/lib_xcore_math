// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xmath/xmath.h"
#include "xmath/xs3/vpu_scalar_ops.h"

#include "../../tst_common.h"

#include "unity_fixture.h"


TEST_GROUP_RUNNER(bfp_complex_use_exponent) {
  RUN_TEST_CASE(bfp_complex_use_exponent, bfp_complex_s16_use_exponent);
  RUN_TEST_CASE(bfp_complex_use_exponent, bfp_complex_s32_use_exponent);
}

TEST_GROUP(bfp_complex_use_exponent);
TEST_SETUP(bfp_complex_use_exponent) { fflush(stdout); }
TEST_TEAR_DOWN(bfp_complex_use_exponent) {}

#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (128)
#else
#  define REPS       (1000)
#  define MAX_LEN    (512)
#endif


TEST(bfp_complex_use_exponent, bfp_complex_s16_use_exponent)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  int16_t realA[MAX_LEN], imagA[MAX_LEN];
  int16_t realA_copy[MAX_LEN], imagA_copy[MAX_LEN];
  int16_t expected_re[MAX_LEN], expected_im[MAX_LEN];

  bfp_complex_s16_t A;
  

  for(int r = 0; r < REPS; r++){
    const unsigned old_seed = seed;
    
    const unsigned len = pseudo_rand_uint(&seed, 1, MAX_LEN+1);

    setExtraInfo_RSL(r, old_seed, len);

    const exponent_t init_exp = pseudo_rand_int(&seed, -5, 5);
    const exponent_t end_exp = pseudo_rand_int(&seed, -5, 5);
    const right_shift_t delta_p = end_exp - init_exp;

    const right_shift_t data_hr = pseudo_rand_uint(&seed, 0, 5);

    for(unsigned int k = 0; k < len; k++){
      realA[k] = pseudo_rand_int16(&seed) >> data_hr;
      imagA[k] = pseudo_rand_int16(&seed) >> data_hr;

      expected_re[k] = delta_p? vlashr16(realA[k], delta_p) : realA[k];
      expected_im[k] = delta_p? vlashr16(imagA[k], delta_p) : imagA[k];

    }
    memcpy(realA_copy, realA, sizeof(realA));
    memcpy(imagA_copy, imagA, sizeof(imagA));
    
    bfp_complex_s16_init(&A, realA, imagA, init_exp, len, 1);

    bfp_complex_s16_use_exponent(&A, end_exp);

    TEST_ASSERT_EQUAL_MESSAGE(vect_complex_s16_headroom(realA, imagA, len), A.hr, "HR is wrong.");
    TEST_ASSERT_EQUAL_MESSAGE(end_exp, A.exp, "Exponent is wrong.");

    XTEST_ASSERT_VECT_S16_EQUAL(expected_re, realA, len,
      "init_exp: %d\n"
      "end_exp: %d\n"
      "Expected: %d * 2^(%d)  <--  (%d >> %d) * 2^(%d + %d)  // was: %d\n",
      init_exp, end_exp,
      expected_re[i], end_exp, realA_copy[i], delta_p, init_exp, delta_p, realA[i]);

    XTEST_ASSERT_VECT_S16_EQUAL(expected_im, imagA, len,
      "init_exp: %d\n"
      "end_exp: %d\n"
      "Expected: %d * 2^(%d)  <--  (%d >> %d) * 2^(%d + %d)  // was: %d\n",
      init_exp, end_exp,
      expected_im[i], end_exp, imagA_copy[i], delta_p, init_exp, delta_p, imagA[i]);
      
  }
}


TEST(bfp_complex_use_exponent, bfp_complex_s32_use_exponent)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  complex_s32_t dataA[MAX_LEN];
  complex_s32_t dataA_copy[MAX_LEN];
  complex_s32_t expected[MAX_LEN];

  bfp_complex_s32_t A;
  

  for(int r = 0; r < REPS; r++){
    const unsigned old_seed = seed;
    
    const unsigned len = pseudo_rand_uint(&seed, 1, MAX_LEN+1);

    setExtraInfo_RSL(r, old_seed, len);

    const exponent_t init_exp = pseudo_rand_int(&seed, -5, 5);
    const exponent_t end_exp = pseudo_rand_int(&seed, -5, 5);
    const right_shift_t delta_p = end_exp - init_exp;

    const right_shift_t data_hr = pseudo_rand_uint(&seed, 0, 5);

    for(unsigned int k = 0; k < len; k++){
      dataA[k].re = pseudo_rand_int32(&seed) >> data_hr;
      dataA[k].im = pseudo_rand_int32(&seed) >> data_hr;

      expected[k].re = delta_p? vlashr32(dataA[k].re, delta_p) : dataA[k].re;
      expected[k].im = delta_p? vlashr32(dataA[k].im, delta_p) : dataA[k].im;
    }
    memcpy(dataA_copy, dataA, sizeof(dataA));
    
    bfp_complex_s32_init(&A, dataA, init_exp, len, 1);

    bfp_complex_s32_use_exponent(&A, end_exp);

    TEST_ASSERT_EQUAL_MESSAGE(vect_complex_s32_headroom(dataA, len), A.hr, "HR is wrong.");
    TEST_ASSERT_EQUAL_MESSAGE(end_exp, A.exp, "Exponent is wrong.");

    XTEST_ASSERT_VECT_S32_EQUAL(((int32_t*)expected), ((int32_t*)dataA), 2*len,
      "init_exp: %d\n"
      "end_exp: %d\n"
      "Expected: %ld * 2^(%d)  <--  (%ld >> %d) * 2^(%d + %d)  // was: %ld\n",
      init_exp, end_exp,
      (long int)((int32_t*)expected)[i], end_exp, (long int)((int32_t*)dataA_copy)[i], delta_p, init_exp, delta_p, (long int)((int32_t*)dataA)[i]);
      
  }
}

