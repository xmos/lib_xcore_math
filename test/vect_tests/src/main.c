// Copyright 2021 XMOS LIMITED. This Software is subject to the terms of the 
// XMOS Public License: Version 1


#include <stdio.h>

#include "unity.h"

#define CALL(F)     do { void F(); F(); } while(0)

int main(int argc, char** argv)
{
    UNITY_BEGIN();



    CALL(test_CLS_funcs);
    CALL(test_HR_funcs);
    CALL(test_xs3_vpu_scalar_ops_s8);
    CALL(test_xs3_vpu_scalar_ops_s16);
    CALL(test_xs3_vpu_scalar_ops_s32);
    CALL(test_xs3_sqrt_s32);
    CALL(test_xs3_inverse_s32);
    CALL(test_xs3_mul_s32);



    CALL(test_xs3_set_vect);
    CALL(test_xs3_headroom_vect);
    CALL(test_xs3_shr_shl_vect);
    CALL(test_xs3_add_sub_vect);
    CALL(test_xs3_mul_vect);
    CALL(test_xs3_abs_clip_rect_vect);
    CALL(test_xs3_sum);
    CALL(test_xs3_dot);
    CALL(test_xs3_bitdepth_convert);
    CALL(test_xs3_add_sub_vect_complex);
    CALL(test_xs3_mul_vect_complex);
    CALL(test_xs3_complex_mul_vect_complex);
    CALL(test_xs3_complex_conj_mul_vect_complex);
    CALL(test_xs3_complex_scal_mul_vect_complex);
    CALL(test_xs3_scalar_mul_vect_complex);
    CALL(test_xs3_squared_mag_vect_complex);
    CALL(test_xs3_mag_vect_complex);
    CALL(test_xs3_sum_complex);
    CALL(test_xs3_vect_complex_s32_to_complex_s16);
    CALL(test_xs3_vect_complex_s16_to_complex_s32);
    CALL(test_xs3_filter_fir_s32);
    CALL(test_xs3_push_sample_s16);
    CALL(test_xs3_filter_biquad_s32);
    CALL(test_xs3_abs_sum);
    CALL(test_xs3_max_min);
    CALL(test_xs3_energy);
    CALL(test_xs3_sqrt_vect);
    CALL(test_xs3_inverse_vect);


    return UNITY_END();
}
