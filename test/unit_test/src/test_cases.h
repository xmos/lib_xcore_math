
#ifndef TEST_CASES_H_
#define TEST_CASES_H_

/*  Basic   */

int test_CLS_funcs();
int test_HR_funcs();

/*  Low-level API   */

int test_xs3_set_vect();
int test_xs3_headroom_vect();
int test_xs3_shl_vect();
int test_xs3_add_vect();
int test_xs3_sub_vect();
int test_xs3_mul_vect();
int test_xs3_scalar_mul_vect();

int test_xs3_rect_vect();
int test_xs3_clip_vect();
int test_xs3_abs_vect();

int test_xs3_sum();
int test_xs3_dot();

int test_xs3_s32_to_s16();
int test_xs3_s16_to_s32();

/*  High-level API  */

int test_bfp_init_vect();
int test_bfp_set_vect();
int test_bfp_headroom_vect();
int test_bfp_ldexp_vect();
int test_bfp_add_vect();
int test_bfp_sub_vect();
int test_bfp_abs_vect();
int test_bfp_rect_vect();

int test_bfp_sum();

int test_bfp_s32_to_s16();
int test_bfp_s16_to_s32();

#endif //TEST_CASES_H_