// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "xmath/xmath.h"
#include "vpu_helper.h"


extern const unsigned rot_table32_rows;
extern const complex_s32_t rot_table32[30][4];


static inline
complex_s32_t safe_complex_ashr32(complex_s32_t x, right_shift_t shr)
{
  complex_s32_t y;
  if(shr >= 32){
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



headroom_t bfp_complex_s32_headroom(
    bfp_complex_s32_t* a)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(a->length != 0);
#endif

    a->hr = vect_s32_headroom((int32_t*)a->data, 2 * a->length);
    return a->hr;
}

    
void bfp_complex_s32_use_exponent(
    bfp_complex_s32_t* a,
    const exponent_t exp)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
  assert(a->length != 0);
#endif

  right_shift_t delta_p = exp - a->exp;

  if(delta_p == 0) return;

  a->hr = vect_complex_s32_shr(a->data, a->data, a->length, delta_p);
  a->exp = exp;
}


void bfp_complex_s32_shl(
    bfp_complex_s32_t* a,
    const bfp_complex_s32_t* b,
    const left_shift_t shl)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(a->length == b->length);
    assert(b->length != 0);
#endif

    a->exp = b->exp;
    a->hr = vect_s32_shl((int32_t*) a->data, (int32_t*) b->data, 2*b->length, shl);
}


void bfp_complex_s32_add(
    bfp_complex_s32_t* a,
    const bfp_complex_s32_t* b,
    const bfp_complex_s32_t* c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == a->length);
    assert(b->length == c->length);
    assert(b->length != 0);
#endif

    right_shift_t b_shr, c_shr;

    vect_complex_s32_add_prepare(&a->exp, &b_shr, &c_shr, b->exp, c->exp, b->hr, c->hr);

    a->hr = vect_complex_s32_add(a->data, b->data, c->data, b->length, b_shr, c_shr);
}


void bfp_complex_s32_add_scalar(
    bfp_complex_s32_t* a,
    const bfp_complex_s32_t* b,
    const float_complex_s32_t c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    right_shift_t b_shr, c_shr;

    vect_complex_s32_add_scalar_prepare(&a->exp, &b_shr, &c_shr, b->exp,
                                            c.exp, b->hr, HR_C32(c.mant));

    complex_s32_t cc = safe_complex_ashr32(c.mant, c_shr);

    a->hr = vect_complex_s32_add_scalar(a->data, b->data, cc, b->length,
                                            b_shr);
}


void bfp_complex_s32_sub(
    bfp_complex_s32_t* a,
    const bfp_complex_s32_t* b,
    const bfp_complex_s32_t* c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == a->length);
    assert(b->length == c->length);
    assert(b->length != 0);
#endif

    right_shift_t b_shr, c_shr;

    vect_complex_s32_sub_prepare(&a->exp, &b_shr, &c_shr, b->exp, c->exp, b->hr, c->hr);

    a->hr = vect_complex_s32_sub(a->data, b->data, c->data, b->length, b_shr, c_shr);
}


void bfp_complex_s32_real_mul(
    bfp_complex_s32_t* a,
    const bfp_complex_s32_t* b,
    const bfp_s32_t* c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == a->length);
    assert(b->length == c->length);
    assert(b->length != 0);
#endif

    exponent_t a_exp;
    right_shift_t b_shr, c_shr;

    vect_complex_s32_real_mul_prepare(&a_exp, &b_shr, &c_shr, b->exp, c->exp, b->hr, c->hr);

    a->exp = a_exp;

    a->hr = vect_complex_s32_real_mul(a->data, b->data, c->data, b->length, b_shr, c_shr);
}


void bfp_complex_s32_mul(
    bfp_complex_s32_t* a,
    const bfp_complex_s32_t* b,
    const bfp_complex_s32_t* c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == a->length);
    assert(b->length == c->length);
    assert(b->length != 0);
#endif

    exponent_t a_exp;
    right_shift_t b_shr, c_shr;

    vect_complex_s32_mul_prepare(&a_exp, &b_shr, &c_shr, b->exp, c->exp, b->hr, c->hr);

    a->exp = a_exp;
    a->hr = vect_complex_s32_mul(a->data, b->data, c->data, b->length, b_shr, c_shr);
}


void bfp_complex_s32_conj_mul(
    bfp_complex_s32_t* a,
    const bfp_complex_s32_t* b,
    const bfp_complex_s32_t* c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == a->length);
    assert(b->length == c->length);
    assert(b->length != 0);
#endif

    exponent_t a_exp;
    right_shift_t b_shr, c_shr;

    vect_complex_s32_conj_mul_prepare(&a_exp, &b_shr, &c_shr, b->exp, c->exp, b->hr, c->hr);

    a->exp = a_exp;
    a->hr = vect_complex_s32_conj_mul(a->data, b->data, c->data,
                                                  b->length, b_shr, c_shr);
}


void bfp_complex_s32_real_scale(
    bfp_complex_s32_t* a,
    const bfp_complex_s32_t* b,
    const float_s32_t c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    right_shift_t b_shr, c_shr;

    headroom_t c_hr = HR_S32(c.mant);

    vect_complex_s32_real_scale_prepare(&a->exp, &b_shr, &c_shr, b->exp, c.exp, b->hr, c_hr);

    a->hr = vect_complex_s32_real_scale( a->data, b->data, c.mant, b->length, b_shr, c_shr);
}


void bfp_complex_s32_scale(
    bfp_complex_s32_t* a,
    const bfp_complex_s32_t* b,
    const float_complex_s32_t c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    right_shift_t b_shr, c_shr;
    const headroom_t c_hr = HR_C32(c.mant);

    vect_complex_s32_scale_prepare(&a->exp, &b_shr, &c_shr, b->exp, c.exp, b->hr, c_hr);
    
    a->hr = vect_complex_s32_scale(a->data, b->data, c.mant.re, c.mant.im, b->length, b_shr, c_shr);
}


void bfp_complex_s32_squared_mag(
    bfp_s32_t* a,
    const bfp_complex_s32_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    right_shift_t b_shr;

    vect_complex_s32_squared_mag_prepare(&a->exp, &b_shr, b->exp, b->hr);

    a->hr = vect_complex_s32_squared_mag(a->data, b->data, b->length, b_shr);
}


void bfp_complex_s32_mag(
    bfp_s32_t* a,
    const bfp_complex_s32_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    right_shift_t b_shr;

    vect_complex_s32_mag_prepare(&a->exp, &b_shr, b->exp, b->hr);

    a->hr = vect_complex_s32_mag(a->data, b->data, b->length,
                                     b_shr, (complex_s32_t*) rot_table32, rot_table32_rows);
}


float_complex_s64_t bfp_complex_s32_sum(
    const bfp_complex_s32_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length != 0);
#endif

    right_shift_t b_shr;
    float_complex_s64_t a;

    vect_complex_s32_sum_prepare(&a.exp, &b_shr, b->exp, b->hr, b->length);
    vect_complex_s32_sum(&a.mant, b->data, b->length, b_shr);

    return a;
}


void bfp_complex_s32_to_bfp_complex_s16(
    bfp_complex_s16_t* a,
    const bfp_complex_s32_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    const right_shift_t b_shr = 16 - b->hr;

    vect_complex_s32_to_vect_complex_s16(a->real, a->imag, b->data, b->length, b_shr);

    a->exp = b->exp + b_shr;
    a->hr = 0;
}


void bfp_complex_s32_macc(
    bfp_complex_s32_t* acc,
    const bfp_complex_s32_t* b,
    const bfp_complex_s32_t* c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == acc->length);
    assert(b->length == c->length);
    assert(b->length != 0);
#endif

    exponent_t a_exp;
    right_shift_t acc_shr, b_shr, c_shr;

    vect_complex_s32_macc_prepare(&a_exp, &acc_shr, &b_shr, &c_shr, acc->exp, b->exp, c->exp, acc->hr, b->hr, c->hr);

    acc->exp = a_exp;
    acc->hr = vect_complex_s32_macc(acc->data, b->data, c->data, b->length, acc_shr, b_shr, c_shr);
}


void bfp_complex_s32_nmacc(
    bfp_complex_s32_t* acc,
    const bfp_complex_s32_t* b,
    const bfp_complex_s32_t* c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == acc->length);
    assert(b->length == c->length);
    assert(b->length != 0);
#endif

    exponent_t a_exp;
    right_shift_t acc_shr, b_shr, c_shr;

    vect_complex_s32_nmacc_prepare(&a_exp, &acc_shr, &b_shr, &c_shr, acc->exp, b->exp, c->exp, acc->hr, b->hr, c->hr);

    acc->exp = a_exp;
    acc->hr = vect_complex_s32_nmacc(acc->data, b->data, c->data, b->length, acc_shr, b_shr, c_shr);
}


void bfp_complex_s32_conj_macc(
    bfp_complex_s32_t* acc,
    const bfp_complex_s32_t* b,
    const bfp_complex_s32_t* c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == acc->length);
    assert(b->length == c->length);
    assert(b->length != 0);
#endif

    exponent_t a_exp;
    right_shift_t acc_shr, b_shr, c_shr;

    vect_complex_s32_conj_macc_prepare(&a_exp, &acc_shr, &b_shr, &c_shr, acc->exp, b->exp, c->exp, acc->hr, b->hr, c->hr);

    acc->exp = a_exp;
    acc->hr = vect_complex_s32_conj_macc(acc->data, b->data, c->data, b->length, acc_shr, b_shr, c_shr);
}


void bfp_complex_s32_conj_nmacc(
    bfp_complex_s32_t* acc,
    const bfp_complex_s32_t* b,
    const bfp_complex_s32_t* c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == acc->length);
    assert(b->length == c->length);
    assert(b->length != 0);
#endif

    exponent_t a_exp;
    right_shift_t acc_shr, b_shr, c_shr;

    vect_complex_s32_conj_nmacc_prepare(&a_exp, &acc_shr, &b_shr, &c_shr, acc->exp, b->exp, c->exp, acc->hr, b->hr, c->hr);

    acc->exp = a_exp;
    acc->hr = vect_complex_s32_conj_nmacc(acc->data, b->data, c->data, b->length, acc_shr, b_shr, c_shr);
}


void bfp_complex_s32_conjugate(
    bfp_complex_s32_t* a,
    const bfp_complex_s32_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    a->exp = b->exp;
    a->hr = vect_complex_s32_conjugate(a->data, b->data, b->length);
}


float_s64_t bfp_complex_s32_energy(
    const bfp_complex_s32_t* b)
{
  float_s64_t a;
  
  right_shift_t b_shr;
  vect_s32_energy_prepare(&a.exp, &b_shr, 2 * b->length, b->exp, b->hr);
  
  a.mant = vect_s32_energy( (int32_t*) b->data, 2 * b->length, b_shr);

  return a;
}



void bfp_complex_s32_make(
    bfp_complex_s32_t* a,
    const bfp_s32_t* b,
    const bfp_s32_t* c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
  assert(b->length == a->length);
  assert(b->length == c->length);
  assert(b->length != 0);
#endif

  const exponent_t b_min_exp = b->exp - b->hr;
  const exponent_t c_min_exp = c->exp - c->hr;
  a->exp = MAX(b_min_exp, c_min_exp);
  a->hr = 0;

  const right_shift_t b_shr = a->exp - b->exp;
  const right_shift_t c_shr = a->exp - c->exp;
  
  vect_s32_zip(&a->data[0], &b->data[0], &c->data[0],
                    b->length, b_shr, c_shr);
}


void bfp_complex_s32_real_part(
    bfp_s32_t* a,
    const bfp_complex_s32_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
  assert(b->length == a->length);
  assert(b->length != 0);
#endif

  a->exp = b->exp;
  a->hr = b->hr; // not necessarily correct, but safe

  for(unsigned k = 0; k < b->length; k++){
    a->data[k] = b->data[k].re;
  }
}


void bfp_complex_s32_imag_part(
    bfp_s32_t* a,
    const bfp_complex_s32_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
  assert(b->length == a->length);
  assert(b->length != 0);
#endif

  a->exp = b->exp;
  a->hr = b->hr; // not necessarily correct, but safe

  for(unsigned k = 0; k < b->length; k++){
    a->data[k] = b->data[k].im;
  }

}
