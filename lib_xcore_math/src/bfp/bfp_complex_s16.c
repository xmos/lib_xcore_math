// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "xmath/xmath.h"
#include "vpu_helper.h"

extern const unsigned rot_table16_rows;
extern const int16_t rot_table16[14][2][16];


static inline 
complex_s16_t safe_complex_ashr16(complex_s16_t x, right_shift_t shr)
{
  complex_s16_t y;
  if(shr >= 16){
    y.re = (x.re >= 0)? 0 : -1;
    y.im = (x.im >= 0)? 0 : -1;
  } else if(shr >= 0){
    y.re = x.re >> shr;
    y.im = x.im >> shr;
  } else {
    y.re = x.re << (-shr);
    y.im = x.im << (-shr);
  }
  return y;
}


headroom_t bfp_complex_s16_headroom(
    bfp_complex_s16_t* a)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(a->length != 0);
#endif

    a->hr = vect_complex_s16_headroom(a->real, a->imag, a->length);
    return a->hr;
}

    
void bfp_complex_s16_use_exponent(
    bfp_complex_s16_t* a,
    const exponent_t exp)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
  assert(a->length != 0);
#endif

  right_shift_t delta_p = exp - a->exp;

  if(delta_p == 0) return;

  a->hr = vect_complex_s16_shr(a->real, a->imag, a->real, a->imag, a->length, delta_p);
  a->exp = exp;
}


void bfp_complex_s16_shl(
    bfp_complex_s16_t* a,
    const bfp_complex_s16_t* b,
    const left_shift_t shl)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(a->length == b->length);
    assert(b->length != 0);
#endif

    a->exp = b->exp;

    a->hr = vect_complex_s16_shl(a->real, a->imag, b->real, b->imag, b->length, shl);
    // const headroom_t re_hr = vect_s16_shl(a->real, b->real, b->length, shl);
    // const headroom_t im_hr = vect_s16_shl(a->imag, b->imag, b->length, shl);
    // a->hr = (re_hr <= im_hr)? re_hr : im_hr;
}


void bfp_complex_s16_add(
    bfp_complex_s16_t* a, 
    const bfp_complex_s16_t* b, 
    const bfp_complex_s16_t* c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == a->length);
    assert(b->length == c->length);
    assert(b->length != 0);
#endif

    right_shift_t b_shr, c_shr;

    vect_complex_s16_add_prepare(&a->exp, &b_shr, &c_shr, b->exp, c->exp, b->hr, c->hr);
    
    a->hr = vect_complex_s16_add(a->real, a->imag, b->real, b->imag, c->real, c->imag, 
                                     b->length, b_shr, c_shr);
}


void bfp_complex_s16_add_scalar(
    bfp_complex_s16_t* a, 
    const bfp_complex_s16_t* b,
    const float_complex_s16_t c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    right_shift_t b_shr, c_shr;

    vect_complex_s16_add_scalar_prepare(&a->exp, &b_shr, &c_shr, b->exp, 
                                            c.exp, b->hr, HR_C16(c.mant));

    complex_s16_t cc = safe_complex_ashr16(c.mant, c_shr);

    a->hr = vect_complex_s16_add_scalar(a->real, a->imag, b->real,
                                            b->imag, cc, b->length, b_shr);
}


void bfp_complex_s16_sub(
    bfp_complex_s16_t* a, 
    const bfp_complex_s16_t* b, 
    const bfp_complex_s16_t* c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == a->length);
    assert(b->length == c->length);
    assert(b->length != 0);
#endif

    right_shift_t b_shr, c_shr;

    vect_complex_s16_sub_prepare(&a->exp, &b_shr, &c_shr, b->exp, c->exp, b->hr, c->hr);
    
    a->hr = vect_complex_s16_sub(a->real, a->imag, b->real, b->imag, c->real, c->imag,
                                     b->length, b_shr, c_shr);
}


void bfp_complex_s16_real_mul(
    bfp_complex_s16_t* a, 
    const bfp_complex_s16_t* b, 
    const bfp_s16_t* c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == a->length);
    assert(b->length == c->length);
    assert(b->length != 0);
#endif

    exponent_t a_exp;
    right_shift_t sat;

    vect_complex_s16_real_mul_prepare(&a_exp, &sat, b->exp, c->exp, b->hr, c->hr);

    a->exp = a_exp;

    a->hr = vect_complex_s16_real_mul(a->real, a->imag, b->real, b->imag, 
                                          c->data, b->length, sat);
    
    // const headroom_t re_hr = vect_s16_mul(a->real, b->real, c->data, b->length, sat);
    // const headroom_t im_hr = vect_s16_mul(a->imag, b->imag, c->data, b->length, sat);
    // a->hr = (re_hr <= im_hr)? re_hr : im_hr;
}


void bfp_complex_s16_mul(
    bfp_complex_s16_t* a, 
    const bfp_complex_s16_t* b, 
    const bfp_complex_s16_t* c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == a->length);
    assert(b->length == c->length);
    assert(b->length != 0);
#endif

    exponent_t a_exp;
    right_shift_t sat;

    vect_complex_s16_mul_prepare(&a_exp, &sat, b->exp, c->exp, b->hr, c->hr);

    a->exp = a_exp;
    a->hr = vect_complex_s16_mul(a->real, a->imag, b->real, b->imag, c->real, c->imag, b->length, sat);
}


void bfp_complex_s16_conj_mul(
    bfp_complex_s16_t* a, 
    const bfp_complex_s16_t* b, 
    const bfp_complex_s16_t* c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == a->length);
    assert(b->length == c->length);
    assert(b->length != 0);
#endif

    exponent_t a_exp;
    right_shift_t sat;

    vect_complex_s16_conj_mul_prepare(&a_exp, &sat, b->exp, c->exp, b->hr, c->hr);

    a->exp = a_exp;
    a->hr = vect_complex_s16_conj_mul(a->real, a->imag, b->real, b->imag, c->real, c->imag, b->length, sat);
}


void bfp_complex_s16_real_scale(
    bfp_complex_s16_t* a, 
    const bfp_complex_s16_t* b, 
    const float alpha)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    int16_t alpha_mant;
    exponent_t alpha_exp;
    f32_unpack_s16(&alpha_mant, &alpha_exp, alpha);

    right_shift_t a_shr;
    headroom_t s_hr = HR_S16(alpha_mant);

    vect_complex_s16_real_scale_prepare(&a->exp, &a_shr, b->exp, alpha_exp, b->hr, s_hr);

    a->hr = vect_complex_s16_real_scale(a->real, a->imag, b->real, b->imag, 
                                            alpha_mant, b->length, a_shr);
}


void bfp_complex_s16_scale(
    bfp_complex_s16_t* a, 
    const bfp_complex_s16_t* b,
    const float_complex_s16_t alpha)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    right_shift_t a_shr;

    headroom_t alpha_hr = HR_C16(alpha.mant);

    vect_complex_s16_scale_prepare(&a->exp, &a_shr, b->exp, alpha.exp, b->hr, alpha_hr);

    a->hr = vect_complex_s16_scale(a->real, a->imag, b->real, b->imag, 
                                                  alpha.mant.re, alpha.mant.im, 
                                                  b->length, a_shr);
}


void bfp_complex_s16_squared_mag(
    bfp_s16_t* a, 
    const bfp_complex_s16_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    right_shift_t sat;

    vect_complex_s16_squared_mag_prepare(&a->exp, &sat, b->exp, b->hr);

    a->hr = vect_complex_s16_squared_mag(a->data, b->real, b->imag, b->length, sat);
}


void bfp_complex_s16_mag(
    bfp_s16_t* a, 
    const bfp_complex_s16_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    right_shift_t b_shr;

    vect_complex_s16_mag_prepare(&a->exp, &b_shr, b->exp, b->hr);

    a->hr = vect_complex_s16_mag(a->data, b->real, b->imag, b->length, 
                                     b_shr, (int16_t*) rot_table16, rot_table16_rows);
}


float_complex_s32_t bfp_complex_s16_sum(
    const bfp_complex_s16_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length != 0);
#endif

    float_complex_s32_t a;

    a.mant = vect_complex_s16_sum(b->real, b->imag, b->length);
    a.exp = b->exp;

    return a;
}


void bfp_complex_s16_to_bfp_complex_s32(
    bfp_complex_s32_t* a, 
    const bfp_complex_s16_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    vect_complex_s16_to_vect_complex_s32(a->data, b->real, b->imag, b->length);
    
    a->exp = b->exp;
    a->hr = b->hr + 16;
}


void bfp_complex_s16_macc(
    bfp_complex_s16_t* acc, 
    const bfp_complex_s16_t* b, 
    const bfp_complex_s16_t* c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == acc->length);
    assert(b->length == c->length);
    assert(b->length != 0);
#endif

    exponent_t a_exp;
    right_shift_t acc_shr, bc_sat;

    vect_complex_s16_macc_prepare(&a_exp, &acc_shr, &bc_sat, acc->exp, b->exp, c->exp, acc->hr, b->hr, c->hr);

    acc->exp = a_exp;
    acc->hr = vect_complex_s16_macc(acc->real, acc->imag, b->real, b->imag, c->real, c->imag, b->length, acc_shr, bc_sat);
}


void bfp_complex_s16_nmacc(
    bfp_complex_s16_t* acc, 
    const bfp_complex_s16_t* b, 
    const bfp_complex_s16_t* c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == acc->length);
    assert(b->length == c->length);
    assert(b->length != 0);
#endif

    exponent_t a_exp;
    right_shift_t acc_shr, bc_sat;

    vect_complex_s16_nmacc_prepare(&a_exp, &acc_shr, &bc_sat, acc->exp, b->exp, c->exp, acc->hr, b->hr, c->hr);

    acc->exp = a_exp;
    acc->hr = vect_complex_s16_nmacc(acc->real, acc->imag, b->real, b->imag, c->real, c->imag, b->length, acc_shr, bc_sat);
}


void bfp_complex_s16_conj_macc(
    bfp_complex_s16_t* acc, 
    const bfp_complex_s16_t* b, 
    const bfp_complex_s16_t* c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == acc->length);
    assert(b->length == c->length);
    assert(b->length != 0);
#endif

    exponent_t a_exp;
    right_shift_t acc_shr, bc_sat;

    vect_complex_s16_conj_macc_prepare(&a_exp, &acc_shr, &bc_sat, acc->exp, b->exp, c->exp, acc->hr, b->hr, c->hr);

    acc->exp = a_exp;
    acc->hr = vect_complex_s16_conj_macc(acc->real, acc->imag, b->real, b->imag, c->real, c->imag, b->length, acc_shr, bc_sat);
}


void bfp_complex_s16_conj_nmacc(
    bfp_complex_s16_t* acc, 
    const bfp_complex_s16_t* b, 
    const bfp_complex_s16_t* c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == acc->length);
    assert(b->length == c->length);
    assert(b->length != 0);
#endif

    exponent_t a_exp;
    right_shift_t acc_shr, bc_sat;

    vect_complex_s16_conj_nmacc_prepare(&a_exp, &acc_shr, &bc_sat, acc->exp, b->exp, c->exp, acc->hr, b->hr, c->hr);

    acc->exp = a_exp;
    acc->hr = vect_complex_s16_conj_nmacc(acc->real, acc->imag, b->real, b->imag, c->real, c->imag, b->length, acc_shr, bc_sat);
}


void bfp_complex_s16_conjugate(
    bfp_complex_s16_t* a, 
    const bfp_complex_s16_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    a->exp = b->exp;
    
    if(a != b)
      memcpy(a->real, b->real, b->length * sizeof(int16_t));

    headroom_t im_hr = vect_s16_scale(a->imag, b->imag, b->length, -1, 0);

    a->hr = MIN(b->hr, im_hr);
}


float_s64_t bfp_complex_s16_energy(
    const bfp_complex_s16_t* b)
{
    float_s64_t a;
    a.exp = 2*b->exp;
    a.mant = 0;
    a.mant += vect_s16_dot(b->real, b->real, b->length);
    a.mant += vect_s16_dot(b->imag, b->imag, b->length);
    return a;
}