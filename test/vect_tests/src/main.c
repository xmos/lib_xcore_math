// Copyright 2020-2021 XMOS LIMITED.
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

    // real vector
    RUN_TEST_GROUP(xs3_vect_copy);
    RUN_TEST_GROUP(xs3_vect_headroom);
    RUN_TEST_GROUP(xs3_vect_set);
    RUN_TEST_GROUP(xs3_vect_shr);
    RUN_TEST_GROUP(xs3_vect_shl);

    RUN_TEST_GROUP(xs3_vect_add);
    RUN_TEST_GROUP(xs3_vect_add_scalar);
    RUN_TEST_GROUP(xs3_vect_sub);
    RUN_TEST_GROUP(xs3_vect_mul);
    RUN_TEST_GROUP(xs3_vect_scale);
    RUN_TEST_GROUP(xs3_vect_abs);
    RUN_TEST_GROUP(xs3_vect_clip);
    RUN_TEST_GROUP(xs3_vect_rect);
    RUN_TEST_GROUP(xs3_vect_inverse);
    RUN_TEST_GROUP(xs3_vect_sum);
    RUN_TEST_GROUP(xs3_vect_abs_sum);
    RUN_TEST_GROUP(xs3_vect_dot);
    RUN_TEST_GROUP(xs3_vect_max);
    RUN_TEST_GROUP(xs3_vect_max_elementwise);
    RUN_TEST_GROUP(xs3_vect_min_elementwise);
    RUN_TEST_GROUP(xs3_vect_min);
    RUN_TEST_GROUP(xs3_vect_argmax);
    RUN_TEST_GROUP(xs3_vect_argmin);
    RUN_TEST_GROUP(xs3_vect_energy);
    RUN_TEST_GROUP(xs3_vect_sqrt);
    RUN_TEST_GROUP(xs3_vect_log);
    RUN_TEST_GROUP(xs3_vect_exp);
    RUN_TEST_GROUP(xs3_vect_bitdepth_convert);
    RUN_TEST_GROUP(xs3_vect_macc);
    RUN_TEST_GROUP(xs3_vect_zip);

    // complex vector
    RUN_TEST_GROUP(xs3_vect_complex_add);
    RUN_TEST_GROUP(xs3_vect_complex_add_scalar);
    RUN_TEST_GROUP(xs3_vect_complex_sub);
    RUN_TEST_GROUP(xs3_vect_complex_mul);
    RUN_TEST_GROUP(xs3_vect_complex_conj_mul);
    RUN_TEST_GROUP(xs3_vect_complex_real_mul);
    RUN_TEST_GROUP(xs3_vect_complex_scale);
    RUN_TEST_GROUP(xs3_vect_complex_real_scale);
    RUN_TEST_GROUP(xs3_vect_complex_squared_mag);
    RUN_TEST_GROUP(xs3_vect_complex_mag);
    RUN_TEST_GROUP(xs3_vect_complex_sum);
    RUN_TEST_GROUP(xs3_vect_complex_s32_to_complex_s16);
    RUN_TEST_GROUP(xs3_vect_complex_s16_to_complex_s32);
    RUN_TEST_GROUP(xs3_vect_complex_macc);
    RUN_TEST_GROUP(xs3_vect_complex_conj_macc);
    RUN_TEST_GROUP(xs3_vect_complex_conjugate);

    // float vector
    RUN_TEST_GROUP(xs3_vect_f32_max_exponent);
    RUN_TEST_GROUP(xs3_vect_f32_to_s32);
    RUN_TEST_GROUP(xs3_vect_s32_to_f32);
    RUN_TEST_GROUP(xs3_vect_f32_dot);

    // misc
    RUN_TEST_GROUP(xs3_vect_extract);
    RUN_TEST_GROUP(xs3_mat_mul_s8_x_s8_yield_s32);
    RUN_TEST_GROUP(xs3_mat_mul_s8_x_s16_yield_s32);
    RUN_TEST_GROUP(xs3_vect_boolean);

    RUN_TEST_GROUP(xs3_vect_convolve);

    return UNITY_END();
}
