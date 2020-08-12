

#include <stdio.h>

#include "unity.h"
#include "test_cases.h"

int main(int argc, char** argv)
{
    UNITY_BEGIN();

    /* **** Basic **** */

    // test_CLS_funcs();
    // test_HR_funcs();


    /* **** Low-level API **** */

    // test_xs3_set_vect();
    // test_xs3_headroom_vect();
    // test_xs3_shl_vect();
    // test_xs3_add_vect();
    // test_xs3_sub_vect();
    // test_xs3_mul_vect();
    // test_xs3_scalar_mul_vect();
    // test_xs3_rect_vect();
    // test_xs3_abs_vect();
    // test_xs3_clip_vect();

    // test_xs3_sum();
    // test_xs3_dot();
    
    // test_xs3_s32_to_s16();
    // test_xs3_s16_to_s32();

    // test_xs3_complex_mul_vect_complex();
    // test_xs3_complex_conj_mul_vect_complex();
    // test_xs3_complex_scal_mul_vect_complex();
    // test_xs3_squared_mag_vect_complex();
    // test_xs3_mag_vect_complex();


    /* **** High-level API **** */

    // test_bfp_init_vect();
    // test_bfp_set_vect();
    // test_bfp_headroom_vect();
    // test_bfp_ldexp_vect();
    // test_bfp_add_vect();
    // test_bfp_sub_vect();
    // test_bfp_mul_vect();
    // test_bfp_scalar_mul_vect();
    // test_bfp_abs_vect();
    // test_bfp_rect_vect();
    // test_bfp_clip_vect();

    // test_bfp_sum();
    // test_bfp_dot();

    // test_bfp_s32_to_s16();
    // test_bfp_s16_to_s32();

    // test_bfp_complex_mul_vect_complex();
    // test_bfp_complex_conj_mul_vect_complex();


    return UNITY_END();
}
