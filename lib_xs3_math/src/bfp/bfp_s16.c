// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include "bfp_math.h"

#include "vect/xs3_vect_s32.h"
#include "vect/xs3_vect_s16.h"

#include <assert.h>
#include <stdio.h>


static inline 
int16_t safe_ashr16(int16_t x, right_shift_t shr)
{
  if(shr >= 16){
    return (x >= 0)? 0 : -1;
  } else if(shr >= 0){
    return x >> shr;
  } else {
    return x << (-shr);
  }
}


    
headroom_t bfp_s16_headroom(
    bfp_s16_t* a)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
  assert(a->length != 0);
#endif

  a->hr = xs3_vect_s16_headroom(a->data, a->length);

  return a->hr;
}

    
void bfp_s16_use_exponent(
    bfp_s16_t* a,
    const exponent_t exp)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
  assert(a->length != 0);
#endif

  right_shift_t delta_p = exp - a->exp;

  if(delta_p == 0) return;

  a->hr = xs3_vect_s16_shr(a->data, a->data, a->length, delta_p);
  a->exp = exp;
}


void bfp_s16_shl(
    bfp_s16_t* a,
    const bfp_s16_t* b,
    const left_shift_t shl)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(a->length == b->length);
    assert(b->length != 0);
#endif

    a->exp = b->exp;
    a->hr = xs3_vect_s16_shl(a->data, b->data, b->length, shl);
}


void bfp_s16_add(
    bfp_s16_t* a, 
    const bfp_s16_t* b, 
    const bfp_s16_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length == c->length);
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    right_shift_t b_shr, c_shr;

    xs3_vect_s16_add_prepare(&a->exp, &b_shr, &c_shr, b->exp, c->exp, b->hr, c->hr);

    a->hr = xs3_vect_s16_add(a->data, b->data, c->data, b->length, b_shr, c_shr);
}


void bfp_s16_add_scalar(
    bfp_s16_t* a, 
    const bfp_s16_t* b, 
    const float c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    right_shift_t b_shr, c_shr;

    int16_t c_mant;
    exponent_t c_exp;
    xs3_f32_unpack_s16(&c_mant, &c_exp, c);

    xs3_vect_s16_add_scalar_prepare(&a->exp, &b_shr, &c_shr, b->exp, c_exp, 
                                    b->hr, HR_S16(c_mant));

    int16_t cc = safe_ashr16(c_mant, c_shr);

    a->hr = xs3_vect_s16_add_scalar(a->data, b->data, cc, b->length, 
                                    b_shr);
}


void bfp_s16_sub(
    bfp_s16_t* a, 
    const bfp_s16_t* b, 
    const bfp_s16_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length == c->length);
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    right_shift_t b_shr, c_shr;

    xs3_vect_s16_sub_prepare(&a->exp, &b_shr, &c_shr, b->exp, c->exp, b->hr, c->hr);

    a->hr = xs3_vect_s16_sub(a->data, b->data, c->data, b->length, b_shr, c_shr);
}


void bfp_s16_mul(
    bfp_s16_t* a, 
    const bfp_s16_t* b, 
    const bfp_s16_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length == c->length);
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    right_shift_t a_shr;

    xs3_vect_s16_mul_prepare(&a->exp, &a_shr,
            b->exp, c->exp, b->hr, c->hr);

    a->hr = xs3_vect_s16_mul(a->data, b->data, c->data, b->length, a_shr);
}


void bfp_s16_scale(
    bfp_s16_t* a, 
    const bfp_s16_t* b, 
    const float alpha)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    int16_t alpha_mant;
    exponent_t alpha_exp;
    xs3_f32_unpack_s16(&alpha_mant, &alpha_exp, alpha);

    right_shift_t a_shr;
    headroom_t alpha_hr = HR_S16(alpha_mant);

    xs3_vect_s16_scale_prepare(&a->exp, &a_shr, b->exp, alpha_exp, b->hr, alpha_hr);

    a->hr = xs3_vect_s16_scale(a->data, b->data, b->length, alpha_mant, a_shr);
}


void bfp_s16_abs(
    bfp_s16_t* a,
    const bfp_s16_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    a->exp = b->exp;
    a->hr = xs3_vect_s16_abs(a->data, b->data, b->length);
}


float_s32_t bfp_s16_sum(
    const bfp_s16_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length != 0);
#endif

    float_s32_t a;
    a.mant = xs3_vect_s16_sum(b->data, b->length);
    a.exp = b->exp;
    return a;
}


float_s64_t bfp_s16_dot(
    const bfp_s16_t* b, 
    const bfp_s16_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length == c->length);
    assert(b->length != 0);
#endif

    float_s64_t a;
    a.mant = xs3_vect_s16_dot(b->data, c->data, b->length);
    a.exp = b->exp + c->exp;
    return a;
}


void bfp_s16_clip(
    bfp_s16_t* a, 
    const bfp_s16_t* b, 
    const int16_t lower_bound, 
    const int16_t upper_bound, 
    const int bound_exp)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    assert(lower_bound <= upper_bound);

    exponent_t a_exp;
    right_shift_t b_shr;

    int16_t lb = lower_bound;
    int16_t ub = upper_bound;

    xs3_vect_s16_clip_prepare(&a_exp, &b_shr, &lb, &ub, b->exp, bound_exp, b->hr);

    if(ub == VPU_INT16_MIN){
        /* upper bound must be smaller than any element of b, so set everything to that */
        a->exp = bound_exp;
        a->hr = HR_S16(upper_bound);
        xs3_vect_s16_set(a->data, upper_bound, b->length);
    } else if(lb == VPU_INT16_MAX){
        /* lower bound must be larger than any element of b, so set everything to that */
        a->exp = bound_exp;
        a->hr = HR_S16(lower_bound);
        xs3_vect_s16_set(a->data, lower_bound, b->length);
    } else if(ub == lb){
        /* upper and lower bounds are indistinguishable */
        a->exp = a_exp;
        a->hr = HR_S16(ub);
        xs3_vect_s16_set(a->data, ub, b->length);

    } else {
        /* some elements may be between bounds */
        a->exp = a_exp;
        a->hr = xs3_vect_s16_clip(a->data, b->data, b->length, lb, ub, b_shr);
    }
}


void bfp_s16_rect(
    bfp_s16_t* a,
    const bfp_s16_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    a->exp = b->exp;
    a->hr = xs3_vect_s16_rect(a->data, b->data, b->length);
}


void bfp_s16_sqrt(
    bfp_s16_t* a,
    const bfp_s16_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    right_shift_t b_shr;

    xs3_vect_s16_sqrt_prepare(&a->exp, &b_shr, b->exp, b->hr);

    a->hr = xs3_vect_s16_sqrt(a->data, b->data, b->length, b_shr, XS3_BFP_SQRT_DEPTH_S16);
}


void bfp_s16_inverse(
    bfp_s16_t* a,
    const bfp_s16_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    unsigned scale;

    xs3_vect_s16_inverse_prepare(&a->exp, &scale, b->data, b->exp, b->length);
    
    xs3_vect_s16_inverse(a->data, b->data, b->length, scale);
    
    bfp_s16_headroom(a);
}


float_s32_t bfp_s16_abs_sum(
    const bfp_s16_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length != 0);
#endif

    float_s32_t a;
    a.mant = xs3_vect_s16_abs_sum(b->data, b->length);
    a.exp = b->exp;
    return a;
}


float bfp_s16_mean(
    const bfp_s16_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length != 0);
#endif

    int32_t sum = xs3_vect_s16_sum(b->data, b->length);

    headroom_t hr = HR_S32(sum) + 32;
    int64_t sum64 = ((int64_t)sum) << hr;
    int64_t mean64 = sum64 / ((int)b->length);
    right_shift_t shr = MAX(0, 48 - HR_S64(mean64));

    //TODO: astew: there's no reason to force the precision down to 16 bits after
    //             getting rid of float_s16_t because xs3_s32_to_f32 handles 32 bits
    if(shr > 0) 
        mean64 += 1 << (shr-1);

    return xs3_s32_to_f32(mean64 >> shr, 
                          b->exp - hr + shr);
}


float_s64_t bfp_s16_energy(
    const bfp_s16_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length != 0);
#endif

    float_s64_t a;
    a.exp = 2*b->exp;
    a.mant = xs3_vect_s16_dot(b->data, b->data, b->length);
    return a;
}


float_s32_t bfp_s16_rms(
    const bfp_s16_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length != 0);
#endif

    float_s32_t a;

    exponent_t exp, len_inv_exp;
    const float_s64_t energy64 = bfp_s16_energy(b);
    const int32_t energy32 = xs3_s64_to_s32(&exp, energy64.mant, energy64.exp);
    const int32_t len_inv = xs3_s32_inverse(&len_inv_exp, b->length);
    const int32_t mean_energy = xs3_s32_mul(&exp, energy32, len_inv, exp, len_inv_exp);

    a.mant = xs3_s32_sqrt(&a.exp, mean_energy, exp, XS3_BFP_SQRT_DEPTH_S32);

    return a;
}


float bfp_s16_max(
    const bfp_s16_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length != 0);
#endif

    return xs3_s32_to_f32(xs3_vect_s16_max(b->data, b->length), 
                          b->exp);
}


void bfp_s16_max_elementwise(
    bfp_s16_t* a, 
    const bfp_s16_t* b, 
    const bfp_s16_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
  assert(b->length == c->length);
  assert(b->length == a->length);
  assert(b->length != 0);
#endif

  // The elements of b[] and c[] need to have the same exponent when compared.
  // We'll choose the smallest exponent that leaves at least 1 bit of headroom.
  const unsigned min_required_operand_headroom = 1;
  right_shift_t b_shr, c_shr;
  xs3_vect_2vec_prepare(&a->exp, &b_shr, &c_shr, 
                        b->exp, c->exp, b->hr, c->hr, 
                        min_required_operand_headroom);

  a->hr = xs3_vect_s16_max_elementwise(a->data, b->data, c->data, 
                                       b->length, b_shr, c_shr);
}


float bfp_s16_min(
    const bfp_s16_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length != 0);
#endif

    return xs3_s32_to_f32(xs3_vect_s16_min(b->data, b->length), 
                          b->exp);
}


void bfp_s16_min_elementwise(
    bfp_s16_t* a, 
    const bfp_s16_t* b, 
    const bfp_s16_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
  assert(b->length == c->length);
  assert(b->length == a->length);
  assert(b->length != 0);
#endif

  // The elements of b[] and c[] need to have the same exponent when compared.
  // We'll choose the smallest exponent that leaves at least 1 bit of headroom.
  const unsigned min_required_operand_headroom = 1;
  right_shift_t b_shr, c_shr;
  xs3_vect_2vec_prepare(&a->exp, &b_shr, &c_shr, 
                        b->exp, c->exp, b->hr, c->hr, 
                        min_required_operand_headroom);

  a->hr = xs3_vect_s16_min_elementwise(a->data, b->data, c->data, 
                                       b->length, b_shr, c_shr);
}


unsigned bfp_s16_argmax(
    const bfp_s16_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length != 0);
#endif

    return xs3_vect_s16_argmax(b->data, b->length);
}


unsigned bfp_s16_argmin(
    const bfp_s16_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length != 0);
#endif

    return xs3_vect_s16_argmin(b->data, b->length);
}


void bfp_s16_to_s32(
    bfp_s32_t* a,
    const bfp_s16_t* b)
{    
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    const exponent_t a_exp = b->exp;

    a->exp = a_exp - 8;
    a->hr = b->hr + 8;
    xs3_vect_s16_to_s32(a->data, b->data, b->length);
}



void bfp_s16_macc(
    bfp_s16_t* acc, 
    const bfp_s16_t* b, 
    const bfp_s16_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length == c->length);
    assert(b->length == acc->length);
    assert(b->length != 0);
#endif

    right_shift_t acc_shr, bc_shr;

    xs3_vect_s16_macc_prepare(&acc->exp, &acc_shr, &bc_shr, acc->exp, b->exp, c->exp, acc->hr, b->hr, c->hr);

    acc->hr = xs3_vect_s16_macc(acc->data, b->data, c->data, b->length, acc_shr, bc_shr);
}


void bfp_s16_nmacc(
    bfp_s16_t* acc, 
    const bfp_s16_t* b, 
    const bfp_s16_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS) // See xs3_math_conf.h
    assert(b->length == c->length);
    assert(b->length == acc->length);
    assert(b->length != 0);
#endif

    right_shift_t acc_shr, bc_shr;

    xs3_vect_s16_macc_prepare(&acc->exp, &acc_shr, &bc_shr, acc->exp, b->exp, c->exp, acc->hr, b->hr, c->hr);

    acc->hr = xs3_vect_s16_nmacc(acc->data, b->data, c->data, b->length, acc_shr, bc_shr);
}