// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include "xmath/xmath.h"
#include "testing.h"
#include "tst_common.h"
#include "unity_fixture.h"

#include <string.h>


TEST_GROUP_RUNNER(bfp_fft_packing) {
  RUN_TEST_CASE(bfp_fft_packing, bfp_fft_mono_unpack_pack);
}

TEST_GROUP(bfp_fft_packing);
TEST_SETUP(bfp_fft_packing) { fflush(stdout); }
TEST_TEAR_DOWN(bfp_fft_packing) {}

#define FFT_N   (512)
#define REPS    (30)

TEST(bfp_fft_packing, bfp_fft_mono_unpack_pack)
{
#define FUNC_NAME "bfp_fft_mono_unpack_pack"

#if PRINT_FUNC_NAMES
    printf("\n%s..\n", FUNC_NAME);
#endif

  unsigned seed = SEED_FROM_FUNC_NAME();

  complex_s32_t buffer[FFT_N/2+1];
  complex_s32_t buffer2[FFT_N/2];

  bfp_complex_s32_t X;
  X.data = buffer;
  X.length = FFT_N/2;
  X.exp = 0;
  X.hr = 0;

  for(int rep = 0; rep < REPS; rep++){

    for(unsigned int k = 0; k < X.length; k++){
      X.data[k].re = pseudo_rand_int32(&seed);
      X.data[k].im = pseudo_rand_int32(&seed);
    }

    memcpy(buffer2, buffer, sizeof(buffer2));

    bfp_fft_unpack_mono(&X);

    TEST_ASSERT_EQUAL(0, X.data[0].im);
    TEST_ASSERT_EQUAL(0, X.data[FFT_N/2].im);
    TEST_ASSERT_EQUAL(buffer2[0].re, X.data[0].re);
    TEST_ASSERT_EQUAL(buffer2[0].im, X.data[FFT_N/2].re);
    TEST_ASSERT_FALSE( memcmp(&buffer2[1], &buffer[1], (FFT_N/2)-1 * sizeof(complex_s32_t)) );
    TEST_ASSERT_EQUAL(FFT_N/2 + 1, X.length);

    bfp_fft_pack_mono(&X);

    TEST_ASSERT_EQUAL(FFT_N/2, X.length);
    TEST_ASSERT_FALSE( memcmp(&buffer2[0], &buffer[0], (FFT_N/2) * sizeof(complex_s32_t)) );

  }
}

#undef FUNC_NAME


