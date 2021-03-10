// Copyright 2020 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public License: Version 1.


#include <stdio.h>

#include "unity.h"

#define CALL(F)     do { void F(); F(); } while(0)

int main(int argc, char** argv)
{
    UNITY_BEGIN();

    CALL(test_bfp_init_vect);
    CALL(test_bfp_set_vect);
    CALL(test_bfp_headroom_vect);
    CALL(test_bfp_shl_vect);
    CALL(test_bfp_add_sub_vect);
    CALL(test_bfp_mul_vect);
    CALL(test_bfp_abs_clip_rect_vect);
    CALL(test_bfp_sum);
    CALL(test_bfp_dot);
    CALL(test_bfp_s32_to_s16);
    CALL(test_bfp_s16_to_s32);
    CALL(test_bfp_add_vect_complex);
    CALL(test_bfp_sub_vect_complex);
    CALL(test_bfp_mul_vect_complex);
    CALL(test_bfp_complex_mul_vect_complex);
    CALL(test_bfp_complex_conj_mul_vect_complex);
    CALL(test_bfp_scalar_mul_vect_complex);
    CALL(test_bfp_complex_scal_mul_vect_complex);
    CALL(test_bfp_squared_mag_vect_complex);
    CALL(test_bfp_mag_vect_complex);
    CALL(test_bfp_sum_complex);
    CALL(test_bfp_complex_bitdepth_convert);
    CALL(test_bfp_sqrt_vect);

    CALL(test_bfp_abs_sum);
    CALL(test_bfp_mean);
    CALL(test_bfp_energy);
    CALL(test_bfp_rms);
    CALL(test_bfp_max_min);
    CALL(test_bfp_inverse_vect);

    return UNITY_END();
}
