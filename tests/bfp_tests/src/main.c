// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdio.h>

#ifdef __XS3A__
# include <xscope.h>
#endif

#include "unity_fixture.h"

int main(int argc, const char* argv[])
{
#ifdef __XS3A__
xscope_config_io(XSCOPE_IO_BASIC);
#endif
  
  UnityGetCommandLineOptions(argc, argv);
  UnityBegin(argv[0]);

  RUN_TEST_GROUP(bfp_init);
  RUN_TEST_GROUP(bfp_alloc);
  RUN_TEST_GROUP(bfp_dealloc);

  RUN_TEST_GROUP(bfp_set);
  RUN_TEST_GROUP(bfp_headroom);
  RUN_TEST_GROUP(bfp_use_exponent);
  RUN_TEST_GROUP(bfp_complex_use_exponent);
  RUN_TEST_GROUP(bfp_shl);

  RUN_TEST_GROUP(bfp_add);
  RUN_TEST_GROUP(bfp_add_scalar);
  RUN_TEST_GROUP(bfp_sub);
  RUN_TEST_GROUP(bfp_mul);
  RUN_TEST_GROUP(bfp_scale);
  RUN_TEST_GROUP(bfp_abs);
  RUN_TEST_GROUP(bfp_clip);
  RUN_TEST_GROUP(bfp_rect);
  RUN_TEST_GROUP(bfp_sum);
  RUN_TEST_GROUP(bfp_dot);
  RUN_TEST_GROUP(bfp_sqrt);
  RUN_TEST_GROUP(bfp_abs_sum);
  RUN_TEST_GROUP(bfp_mean);
  RUN_TEST_GROUP(bfp_energy);
  RUN_TEST_GROUP(bfp_rms);
  RUN_TEST_GROUP(bfp_max);
  RUN_TEST_GROUP(bfp_max_elementwise);
  RUN_TEST_GROUP(bfp_min);
  RUN_TEST_GROUP(bfp_min_elementwise);
  RUN_TEST_GROUP(bfp_argmax);
  RUN_TEST_GROUP(bfp_argmin);
  RUN_TEST_GROUP(bfp_inverse);
  RUN_TEST_GROUP(bfp_macc);

  RUN_TEST_GROUP(bfp_complex_add);
  RUN_TEST_GROUP(bfp_complex_add_scalar);
  RUN_TEST_GROUP(bfp_complex_sub);
  RUN_TEST_GROUP(bfp_complex_mul);
  RUN_TEST_GROUP(bfp_complex_real_mul);
  RUN_TEST_GROUP(bfp_complex_conj_mul);
  RUN_TEST_GROUP(bfp_complex_scale);
  RUN_TEST_GROUP(bfp_complex_real_scale);
  RUN_TEST_GROUP(bfp_complex_squared_mag);
  RUN_TEST_GROUP(bfp_complex_mag);
  RUN_TEST_GROUP(bfp_complex_sum);
  RUN_TEST_GROUP(bfp_complex_macc);
  RUN_TEST_GROUP(bfp_complex_conj_macc);
  RUN_TEST_GROUP(bfp_complex_conjugate);
  RUN_TEST_GROUP(bfp_complex_energy);
  RUN_TEST_GROUP(bfp_complex_make);
  
  RUN_TEST_GROUP(bfp_depth_convert);
  RUN_TEST_GROUP(bfp_complex_depth_convert);

  RUN_TEST_GROUP(bfp_gradient_constraint);
  RUN_TEST_GROUP(bfp_convolve);

  RUN_TEST_GROUP(bfp_s16_accumulate);
  
  return UNITY_END();
}
