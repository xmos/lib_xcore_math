

#include <stdio.h>

#include "unity.h"

#define CALL(F)     do { void F(); F(); } while(0)

int main(int argc, char** argv)
{
    UNITY_BEGIN();

    /* **** Basic **** */

    CALL(test_CLS_funcs);
    CALL(test_HR_funcs);


    /* **** Low-level API **** */

    CALL(test_xs3_set_vect);
    CALL(test_xs3_headroom_vect);
    CALL(test_xs3_shl_vect);
    CALL(test_xs3_add_vect);
    CALL(test_xs3_sub_vect);
    CALL(test_xs3_mul_vect);
    CALL(test_xs3_scalar_mul_vect);
    CALL(test_xs3_rect_vect);
    CALL(test_xs3_abs_vect);
    CALL(test_xs3_clip_vect);

    CALL(test_xs3_sum);
    CALL(test_xs3_dot);
    
    CALL(test_xs3_s32_to_s16);
    CALL(test_xs3_s16_to_s32);

    CALL(test_xs3_mul_vect_complex);
    CALL(test_xs3_complex_mul_vect_complex);
    CALL(test_xs3_complex_conj_mul_vect_complex);
    CALL(test_xs3_complex_scal_mul_vect_complex);
    CALL(test_xs3_squared_mag_vect_complex);
    CALL(test_xs3_mag_vect_complex);
    CALL(test_xs3_sum_complex);

    CALL(test_xs3_complex_s32_to_complex_s16);
    CALL(test_xs3_complex_s16_to_complex_s32);

    CALL(test_xs3_filter_fir_s32);
    CALL(test_xs3_push_sample_s16);

    CALL(test_xs3_filter_biquad_s32);

    CALL(test_xs3_abs_sum);
    CALL(test_xs3_max);
    CALL(test_xs3_min);
    CALL(test_xs3_argmax);
    CALL(test_xs3_argmin);
    CALL(test_xs3_energy);

    /* **** High-level API **** */

    CALL(test_bfp_init_vect);
    CALL(test_bfp_set_vect);
    CALL(test_bfp_headroom_vect);
    CALL(test_bfp_ldexp_vect);
    CALL(test_bfp_add_vect);
    CALL(test_bfp_sub_vect);
    CALL(test_bfp_mul_vect);
    CALL(test_bfp_scalar_mul_vect);
    CALL(test_bfp_abs_vect);
    CALL(test_bfp_rect_vect);
    CALL(test_bfp_clip_vect);

    CALL(test_bfp_sum);
    CALL(test_bfp_dot);

    CALL(test_bfp_s32_to_s16);
    CALL(test_bfp_s16_to_s32);

    CALL(test_bfp_complex_mul_vect_complex);
    CALL(test_bfp_complex_conj_mul_vect_complex);


    return UNITY_END();
}
