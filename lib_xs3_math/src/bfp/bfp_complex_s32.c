// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include "bfp_math.h"

#include "vect/xs3_vect_s32.h"
#include "vect/xs3_vect_s16.h"
#include "../vect/vpu_helper.h"


const extern unsigned rot_table32_rows;
const extern complex_s32_t rot_table32[30][4];


headroom_t bfp_complex_s32_headroom(
    bfp_complex_s32_t* a)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(a->length != 0);
#endif

    a->hr = xs3_vect_s32_headroom((int32_t*)a->data, 2 * a->length);
    return a->hr;
}

    
void bfp_complex_s32_use_exponent(
    bfp_complex_s32_t* a,
    const exponent_t exp)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
  assert(a->length != 0);
#endif

  right_shift_t delta_p = exp - a->exp;

  if(delta_p == 0) return;

  a->hr = xs3_vect_complex_s32_shr(a->data, a->data, a->length, delta_p);
  a->exp = exp;
}


void bfp_complex_s32_shl(
    bfp_complex_s32_t* a,
    const bfp_complex_s32_t* b,
    const left_shift_t shl)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(a->length == b->length);
    assert(b->length != 0);
#endif

    a->exp = b->exp;
    a->hr = xs3_vect_s32_shl((int32_t*) a->data, (int32_t*) b->data, 2*b->length, shl);
}


void bfp_complex_s32_add(
    bfp_complex_s32_t* a, 
    const bfp_complex_s32_t* b, 
    const bfp_complex_s32_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length == a->length);
    assert(b->length == c->length);
    assert(b->length != 0);
#endif

    right_shift_t b_shr, c_shr;

    xs3_vect_complex_s32_add_prepare(&a->exp, &b_shr, &c_shr, b->exp, c->exp, b->hr, c->hr);

    a->hr = xs3_vect_complex_s32_add(a->data, b->data, c->data, b->length, b_shr, c_shr);
}


void bfp_complex_s32_add_scalar(
    bfp_complex_s32_t* a, 
    const bfp_complex_s32_t* b, 
    const float_complex_s32_t c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    right_shift_t b_shr, c_shr;

    xs3_vect_complex_s32_add_scalar_prepare(&a->exp, &b_shr, &c_shr, b->exp, 
                                            c.exp, b->hr, HR_C32(c.mant));

    complex_s32_t cc = {
      .re = (c_shr >= 0)? (c.mant.re >> c_shr) : (c.mant.re << -c_shr),
      .im = (c_shr >= 0)? (c.mant.im >> c_shr) : (c.mant.im << -c_shr)
    };

    a->hr = xs3_vect_complex_s32_add_scalar(a->data, b->data, cc, b->length, 
                                            b_shr);
}


void bfp_complex_s32_sub(
    bfp_complex_s32_t* a, 
    const bfp_complex_s32_t* b,
    const bfp_complex_s32_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length == a->length);
    assert(b->length == c->length);
    assert(b->length != 0);
#endif

    right_shift_t b_shr, c_shr;

    xs3_vect_complex_s32_sub_prepare(&a->exp, &b_shr, &c_shr, b->exp, c->exp, b->hr, c->hr);

    a->hr = xs3_vect_complex_s32_sub(a->data, b->data, c->data, b->length, b_shr, c_shr);
}


void bfp_complex_s32_real_mul(
    bfp_complex_s32_t* a, 
    const bfp_complex_s32_t* b, 
    const bfp_s32_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length == a->length);
    assert(b->length == c->length);
    assert(b->length != 0);
#endif

    exponent_t a_exp;
    right_shift_t b_shr, c_shr;

    xs3_vect_complex_s32_real_mul_prepare(&a_exp, &b_shr, &c_shr, b->exp, c->exp, b->hr, c->hr);

    a->exp = a_exp;

    a->hr = xs3_vect_complex_s32_real_mul(a->data, b->data, c->data, b->length, b_shr, c_shr);
}


void bfp_complex_s32_mul(
    bfp_complex_s32_t* a, 
    const bfp_complex_s32_t* b, 
    const bfp_complex_s32_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length == a->length);
    assert(b->length == c->length);
    assert(b->length != 0);
#endif

    exponent_t a_exp;
    right_shift_t b_shr, c_shr;

    xs3_vect_complex_s32_mul_prepare(&a_exp, &b_shr, &c_shr, b->exp, c->exp, b->hr, c->hr);

    a->exp = a_exp;
    a->hr = xs3_vect_complex_s32_mul(a->data, b->data, c->data, b->length, b_shr, c_shr);
}


void bfp_complex_s32_conj_mul(
    bfp_complex_s32_t* a, 
    const bfp_complex_s32_t* b, 
    const bfp_complex_s32_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length == a->length);
    assert(b->length == c->length);
    assert(b->length != 0);
#endif

    exponent_t a_exp;
    right_shift_t b_shr, c_shr;

    xs3_vect_complex_s32_conj_mul_prepare(&a_exp, &b_shr, &c_shr, b->exp, c->exp, b->hr, c->hr);

    a->exp = a_exp;
    a->hr = xs3_vect_complex_s32_conj_mul(a->data, b->data, c->data, 
                                                  b->length, b_shr, c_shr);
}


void bfp_complex_s32_real_scale(
    bfp_complex_s32_t* a, 
    const bfp_complex_s32_t* b, 
    const float_s32_t c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    right_shift_t b_shr, c_shr;

    headroom_t c_hr = HR_S32(c.mant);

    xs3_vect_complex_s32_real_scale_prepare(&a->exp, &b_shr, &c_shr, b->exp, c.exp, b->hr, c_hr);

    a->hr = xs3_vect_complex_s32_real_scale( a->data, b->data, c.mant, b->length, b_shr, c_shr);
}


void bfp_complex_s32_scale(
    bfp_complex_s32_t* a, 
    const bfp_complex_s32_t* b, 
    const float_complex_s32_t c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    right_shift_t b_shr, c_shr;
    const headroom_t c_hr = HR_C32(c.mant);

    xs3_vect_complex_s32_scale_prepare(&a->exp, &b_shr, &c_shr, b->exp, c.exp, b->hr, c_hr);
    
    a->hr = xs3_vect_complex_s32_scale(a->data, b->data, c.mant.re, c.mant.im, b->length, b_shr, c_shr);
}


void bfp_complex_s32_squared_mag(
    bfp_s32_t* a, 
    const bfp_complex_s32_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    right_shift_t b_shr;

    xs3_vect_complex_s32_squared_mag_prepare(&a->exp, &b_shr, b->exp, b->hr);

    a->hr = xs3_vect_complex_s32_squared_mag(a->data, b->data, b->length, b_shr);
}


void bfp_complex_s32_mag(
    bfp_s32_t* a, 
    const bfp_complex_s32_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    right_shift_t b_shr;

    xs3_vect_complex_s32_mag_prepare(&a->exp, &b_shr, b->exp, b->hr);

    a->hr = xs3_vect_complex_s32_mag(a->data, b->data, b->length, 
                                     b_shr, (complex_s32_t*) rot_table32, rot_table32_rows);
}


float_complex_s64_t bfp_complex_s32_sum( 
    const bfp_complex_s32_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length != 0);
#endif

    right_shift_t b_shr;
    float_complex_s64_t a;

    xs3_vect_complex_s32_sum_prepare(&a.exp, &b_shr, b->exp, b->hr, b->length);
    xs3_vect_complex_s32_sum(&a.mant, b->data, b->length, b_shr);

    return a;
}


void bfp_complex_s32_to_complex_s16(
    bfp_complex_s16_t* a, 
    const bfp_complex_s32_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    const right_shift_t b_shr = 16 - b->hr;

    xs3_vect_complex_s32_to_complex_s16(a->real, a->imag, b->data, b->length, b_shr);

    a->exp = b->exp + b_shr;
    a->hr = 0;
}


void bfp_complex_s32_macc(
    bfp_complex_s32_t* acc, 
    const bfp_complex_s32_t* b, 
    const bfp_complex_s32_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length == acc->length);
    assert(b->length == c->length);
    assert(b->length != 0);
#endif

    exponent_t a_exp;
    right_shift_t acc_shr, b_shr, c_shr;

    xs3_vect_complex_s32_macc_prepare(&a_exp, &acc_shr, &b_shr, &c_shr, acc->exp, b->exp, c->exp, acc->hr, b->hr, c->hr);

    acc->exp = a_exp;
    acc->hr = xs3_vect_complex_s32_macc(acc->data, b->data, c->data, b->length, acc_shr, b_shr, c_shr);
}


void bfp_complex_s32_nmacc(
    bfp_complex_s32_t* acc, 
    const bfp_complex_s32_t* b, 
    const bfp_complex_s32_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length == acc->length);
    assert(b->length == c->length);
    assert(b->length != 0);
#endif

    exponent_t a_exp;
    right_shift_t acc_shr, b_shr, c_shr;

    xs3_vect_complex_s32_nmacc_prepare(&a_exp, &acc_shr, &b_shr, &c_shr, acc->exp, b->exp, c->exp, acc->hr, b->hr, c->hr);

    acc->exp = a_exp;
    acc->hr = xs3_vect_complex_s32_nmacc(acc->data, b->data, c->data, b->length, acc_shr, b_shr, c_shr);
}


void bfp_complex_s32_conj_macc(
    bfp_complex_s32_t* acc, 
    const bfp_complex_s32_t* b, 
    const bfp_complex_s32_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length == acc->length);
    assert(b->length == c->length);
    assert(b->length != 0);
#endif

    exponent_t a_exp;
    right_shift_t acc_shr, b_shr, c_shr;

    xs3_vect_complex_s32_conj_macc_prepare(&a_exp, &acc_shr, &b_shr, &c_shr, acc->exp, b->exp, c->exp, acc->hr, b->hr, c->hr);

    acc->exp = a_exp;
    acc->hr = xs3_vect_complex_s32_conj_macc(acc->data, b->data, c->data, b->length, acc_shr, b_shr, c_shr);
}


void bfp_complex_s32_conj_nmacc(
    bfp_complex_s32_t* acc, 
    const bfp_complex_s32_t* b, 
    const bfp_complex_s32_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length == acc->length);
    assert(b->length == c->length);
    assert(b->length != 0);
#endif

    exponent_t a_exp;
    right_shift_t acc_shr, b_shr, c_shr;

    xs3_vect_complex_s32_conj_nmacc_prepare(&a_exp, &acc_shr, &b_shr, &c_shr, acc->exp, b->exp, c->exp, acc->hr, b->hr, c->hr);

    acc->exp = a_exp;
    acc->hr = xs3_vect_complex_s32_conj_nmacc(acc->data, b->data, c->data, b->length, acc_shr, b_shr, c_shr);
}


void bfp_complex_s32_conjugate(
    bfp_complex_s32_t* a, 
    const bfp_complex_s32_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    a->exp = b->exp;
    a->hr = b->hr;

    xs3_vect_complex_s32_conjugate(a->data, b->data, b->length);
}


float_s64_t bfp_complex_s32_energy(
    const bfp_complex_s32_t* b)
{
  float_s64_t a;
  
  right_shift_t b_shr;
  xs3_vect_s32_energy_prepare(&a.exp, &b_shr, 2 * b->length, b->exp, b->hr);
  
  a.mant = xs3_vect_s32_energy( (int32_t*) b->data, 2 * b->length, b_shr);

  return a;
}