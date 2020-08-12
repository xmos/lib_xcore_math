
#ifndef TEST_CASES_H_
#define TEST_CASES_H_

/*  Basic   */

void test_CLS_funcs();
void test_HR_funcs();

/*  Low-level API   */

void test_xs3_set_vect();
void test_xs3_headroom_vect();
void test_xs3_shl_vect();
void test_xs3_add_vect();
void test_xs3_sub_vect();
void test_xs3_mul_vect();
void test_xs3_scalar_mul_vect();

void test_xs3_rect_vect();
void test_xs3_clip_vect();
void test_xs3_abs_vect();

void test_xs3_sum();
void test_xs3_dot();

void test_xs3_s32_to_s16();
void test_xs3_s16_to_s32();

void test_xs3_complex_scal_mul_vect_complex();
void test_xs3_complex_mul_vect_complex();
void test_xs3_complex_conj_mul_vect_complex();
void test_xs3_squared_mag_vect_complex();
void test_xs3_mag_vect_complex();

/*  High-level API  */

void test_bfp_init_vect();
void test_bfp_set_vect();
void test_bfp_headroom_vect();
void test_bfp_ldexp_vect();
void test_bfp_add_vect();
void test_bfp_sub_vect();
void test_bfp_mul_vect();
void test_bfp_scalar_mul_vect();
void test_bfp_abs_vect();
void test_bfp_rect_vect();
void test_bfp_clip_vect();

void test_bfp_sum();
void test_bfp_dot();

void test_bfp_s32_to_s16();
void test_bfp_s16_to_s32();

void test_bfp_complex_scal_mul_vect_complex();
void test_bfp_complex_mul_vect_complex();
void test_bfp_complex_conj_mul_vect_complex();

#endif //TEST_CASES_H_