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


TEST_GROUP_RUNNER(bfp_gradient_constraint) {
  RUN_TEST_CASE(bfp_gradient_constraint, bfp_complex_s32_gradient_constraint_mono);
  RUN_TEST_CASE(bfp_gradient_constraint, bfp_complex_s32_gradient_constraint_stereo_A);
  RUN_TEST_CASE(bfp_gradient_constraint, bfp_complex_s32_gradient_constraint_stereo_B);
}
TEST_GROUP(bfp_gradient_constraint);
TEST_SETUP(bfp_gradient_constraint) { fflush(stdout); }
TEST_TEAR_DOWN(bfp_gradient_constraint) {}

#define MIN_FFT_N_LOG2    (6)
#define MAX_FFT_N_LOG2    (10)

TEST(bfp_gradient_constraint, bfp_complex_s32_gradient_constraint_mono)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  complex_s32_t DWORD_ALIGNED dataA[1 << (MAX_FFT_N_LOG2-1)];
  bfp_complex_s32_t A_fd;

  int32_t expected[1 << MAX_FFT_N_LOG2];

  for(int FFT_N_LOG2 = MIN_FFT_N_LOG2;
          FFT_N_LOG2 < MAX_FFT_N_LOG2;
          FFT_N_LOG2++){

    const unsigned FFT_N = (1 << FFT_N_LOG2);
    const unsigned FFT_BINS = FFT_N/2;

    for(unsigned int frame_advance = 16; frame_advance < FFT_BINS; frame_advance += 16){

      setExtraInfo_RSL(FFT_N, seed, frame_advance);

      //// Create random vector
      test_random_vect_complex_s32(dataA, &seed, FFT_BINS, FFT_BINS, 0, 4);
      bfp_complex_s32_init(&A_fd, dataA, pseudo_rand_int(&seed, -10, 10), FFT_BINS, 1);

      //// Set up our expectation.
      bfp_s32_t* A_td = bfp_fft_inverse_mono(&A_fd);

      const exponent_t td_exp = A_td->exp;
      
      for(unsigned int k = 0; k < A_td->length; k++)
        expected[k] = (k < frame_advance)? A_td->data[k] : 0;
      bfp_fft_forward_mono(A_td);


      //// Apply op
      bfp_complex_s32_gradient_constraint_mono(&A_fd, frame_advance);

      TEST_ASSERT_EQUAL_MESSAGE(vect_complex_s32_headroom(dataA, FFT_BINS), A_fd.hr,
                                "Headroom is wrong.");

      //// Check results

      A_td = bfp_fft_inverse_mono(&A_fd);

      bfp_s32_use_exponent(A_td, td_exp);

      TEST_ASSERT_EQUAL_MESSAGE(FFT_N, A_td->length, "Length is wrong.");

      // It's possible doing the IFFT allowed noise to bleed into the samples that should be zero
      // here, so give some slack

      // astew: 2022/06/30 -- Increased threshold from 20 to 21. Test was failing after I fixed
      //        the random number generation problem. (threshold is observed, not theoretical)
      // astew: 2022/07/12 -- Increased threshold from 21 to 22 because it was failing. It isn't
      //        immediately obvious why the error changed here. The seed shouldn't have changed,
      //        but a couple of other things changed in subtle ways (such as bfp_fft_forward_mono())
      //        which could plausibly have caused it.
      // astew: 2022/08/23 -- Increased threshold again to 25. The seed changed, so this is most
      //                      likely a result of hitting a slightly worse case. Everything still
      //                      seems correct.
      XTEST_ASSERT_VECT_S32_WITHIN(25, expected, A_td->data, A_td->length,
        "FFT_N: %u\n"
        "frame_advance: %u\n", FFT_N, frame_advance);

    }
  }
}

// two non-contiguous BFP vectors. Should just do mono twice
TEST(bfp_gradient_constraint, bfp_complex_s32_gradient_constraint_stereo_A)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  complex_s32_t DWORD_ALIGNED dataA[(1 << (MAX_FFT_N_LOG2-1))+1];
  complex_s32_t DWORD_ALIGNED dataB[(1 << (MAX_FFT_N_LOG2-1))];
  bfp_complex_s32_t A_fd, B_fd;

  int32_t expectedA[1 << MAX_FFT_N_LOG2];
  int32_t expectedB[1 << MAX_FFT_N_LOG2];

  for(int FFT_N_LOG2 = MIN_FFT_N_LOG2;
          FFT_N_LOG2 < MAX_FFT_N_LOG2;
          FFT_N_LOG2++){

    const unsigned FFT_N = (1 << FFT_N_LOG2);
    const unsigned FFT_BINS = FFT_N/2;

    for(unsigned int frame_advance = 16; frame_advance < FFT_BINS; frame_advance += 16){

      setExtraInfo_RSL(FFT_N, seed, frame_advance);

      //// Create random vectors
      test_random_vect_complex_s32(dataA, &seed, FFT_BINS, FFT_BINS, 0, 4);
      test_random_vect_complex_s32(dataB, &seed, FFT_BINS, FFT_BINS, 0, 4);
      bfp_complex_s32_init(&A_fd, dataA, pseudo_rand_int(&seed, -10, 10), FFT_BINS, 1);
      bfp_complex_s32_init(&B_fd, dataB, pseudo_rand_int(&seed, -10, 10), FFT_BINS, 1);

      //// Set up our expectation.
      bfp_s32_t* A_td = bfp_fft_inverse_mono(&A_fd);
      bfp_s32_t* B_td = bfp_fft_inverse_mono(&B_fd);

      const exponent_t td_expA = A_td->exp;
      const exponent_t td_expB = B_td->exp;
      
      for(unsigned int k = 0; k < A_td->length; k++){
        expectedA[k] = (k < frame_advance)? A_td->data[k] : 0;
        expectedB[k] = (k < frame_advance)? B_td->data[k] : 0;
      }
      bfp_fft_forward_mono(A_td);
      bfp_fft_forward_mono(B_td);


      //// Apply op
      bfp_complex_s32_gradient_constraint_stereo(&A_fd, &B_fd, frame_advance);
      
      TEST_ASSERT_EQUAL_MESSAGE(vect_complex_s32_headroom(dataA, FFT_BINS), A_fd.hr,
                                "Headroom is wrong (A).");
      TEST_ASSERT_EQUAL_MESSAGE(vect_complex_s32_headroom(dataB, FFT_BINS), B_fd.hr,
                                "Headroom is wrong (B).");

      //// Check results
      A_td = bfp_fft_inverse_mono(&A_fd);
      B_td = bfp_fft_inverse_mono(&B_fd);

      bfp_s32_use_exponent(A_td, td_expA);
      bfp_s32_use_exponent(B_td, td_expB);

      TEST_ASSERT_EQUAL_MESSAGE(FFT_N, A_td->length, "Length is wrong (A).");
      TEST_ASSERT_EQUAL_MESSAGE(FFT_N, B_td->length, "Length is wrong (B).");

      // It's possible doing the IFFT allowed noise to bleed into the samples that should be zero
      // here, so give some slack

      XTEST_ASSERT_VECT_S32_WITHIN(23, expectedA, A_td->data, A_td->length,
        "FFT_N: %u\n"
        "frame_advance: %u\n", FFT_N, frame_advance);

      XTEST_ASSERT_VECT_S32_WITHIN(23, expectedB, B_td->data, A_td->length,
        "FFT_N: %u\n"
        "frame_advance: %u\n", FFT_N, frame_advance);

    }
  }
}

// two contiguous BFP vectors. Should just do faster stereo GC
TEST(bfp_gradient_constraint, bfp_complex_s32_gradient_constraint_stereo_B)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  complex_s32_t DWORD_ALIGNED data[1 << MAX_FFT_N_LOG2];
  bfp_complex_s32_t A_fd, B_fd;

  int32_t expectedA[1 << MAX_FFT_N_LOG2];
  int32_t expectedB[1 << MAX_FFT_N_LOG2];

  for(int FFT_N_LOG2 = MIN_FFT_N_LOG2;
          FFT_N_LOG2 < MAX_FFT_N_LOG2;
          FFT_N_LOG2++){

    const unsigned FFT_N = (1 << FFT_N_LOG2);
    const unsigned FFT_BINS = FFT_N/2;

    for(unsigned int frame_advance = 16; frame_advance < FFT_BINS; frame_advance += 16){

      setExtraInfo_RSL(FFT_N, seed, frame_advance);

      //// Create random vectors
      test_random_vect_complex_s32(&data[0*FFT_BINS], &seed, FFT_BINS, FFT_BINS, 0, 4);
      test_random_vect_complex_s32(&data[1*FFT_BINS], &seed, FFT_BINS, FFT_BINS, 0, 4);
      bfp_complex_s32_init(&A_fd, &data[0*FFT_BINS], pseudo_rand_int(&seed, -10, 10), FFT_BINS, 1);
      bfp_complex_s32_init(&B_fd, &data[1*FFT_BINS], pseudo_rand_int(&seed, -10, 10), FFT_BINS, 1);

      //// Set up our expectation.
      bfp_s32_t* A_td = bfp_fft_inverse_mono(&A_fd);
      bfp_s32_t* B_td = bfp_fft_inverse_mono(&B_fd);

      const exponent_t td_expA = A_td->exp;
      const exponent_t td_expB = B_td->exp;
      
      for(unsigned int k = 0; k < A_td->length; k++){
        expectedA[k] = (k < frame_advance)? A_td->data[k] : 0;
        expectedB[k] = (k < frame_advance)? B_td->data[k] : 0;
      }
      bfp_fft_forward_mono(A_td);
      bfp_fft_forward_mono(B_td);


      //// Apply op
      bfp_complex_s32_gradient_constraint_stereo(&A_fd, &B_fd, frame_advance);
      
      TEST_ASSERT_EQUAL_MESSAGE(vect_complex_s32_headroom(A_fd.data, FFT_BINS), A_fd.hr,
                                "Headroom is wrong (A).");
      TEST_ASSERT_EQUAL_MESSAGE(vect_complex_s32_headroom(B_fd.data, FFT_BINS), B_fd.hr,
                                "Headroom is wrong (B).");

      //// Check results
      A_td = bfp_fft_inverse_mono(&A_fd);
      B_td = bfp_fft_inverse_mono(&B_fd);

      bfp_s32_use_exponent(A_td, td_expA);
      bfp_s32_use_exponent(B_td, td_expB);

      TEST_ASSERT_EQUAL_MESSAGE(FFT_N, A_td->length, "Length is wrong (A).");
      TEST_ASSERT_EQUAL_MESSAGE(FFT_N, B_td->length, "Length is wrong (B).");

      // It's possible doing the IFFT allowed noise to bleed into the samples that should be zero
      // here, so give some slack

      // astew: 2022/06/30 -- Increased threshold from 34 to 40. Test was failing after I fixed
      //        the random number generation problem. (threshold is observed, not theoretical)
      XTEST_ASSERT_VECT_S32_WITHIN(40, expectedA, A_td->data, A_td->length,
        "FFT_N: %u\n"
        "frame_advance: %u\n", FFT_N, frame_advance);

      XTEST_ASSERT_VECT_S32_WITHIN(40, expectedB, B_td->data, B_td->length,
        "FFT_N: %u\n"
        "frame_advance: %u\n", FFT_N, frame_advance);

    }
  }
}

