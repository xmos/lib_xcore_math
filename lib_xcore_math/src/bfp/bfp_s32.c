// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include <assert.h>
#include <stdio.h>

#include "xmath/xmath.h"


static inline
int32_t safe_ashr32(int32_t x, right_shift_t shr)
{
  if(shr >= 32){
    return (x >= 0)? 0 : -1;
  } else if(shr >= 0){
    return x >> shr;
  } else {
    return x << (-shr);
  }
}


    
headroom_t bfp_s32_headroom(
    bfp_s32_t* a)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(a->length != 0);
#endif

     a->hr = vect_s32_headroom(a->data, a->length);

     return a->hr;
}

    
void bfp_s32_use_exponent(
    bfp_s32_t* a,
    const exponent_t exp)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
  assert(a->length != 0);
#endif

  right_shift_t delta_p = exp - a->exp;

  if(delta_p == 0) return;

  a->hr = vect_s32_shr(a->data, a->data, a->length, delta_p);
  a->exp = exp;
}


void bfp_s32_shl(
    bfp_s32_t* a,
    const bfp_s32_t* b,
    const left_shift_t shl)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(a->length == b->length);
    assert(b->length != 0);
#endif
    a->length = b->length;
    a->exp = b->exp;
    a->hr = vect_s32_shl(a->data, b->data, b->length, shl);
}


void bfp_s32_add(
    bfp_s32_t* a,
    const bfp_s32_t* b,
    const bfp_s32_t* c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == c->length);
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    right_shift_t b_shr, c_shr;

    vect_s32_add_prepare(&a->exp, &b_shr, &c_shr, b->exp, c->exp, b->hr, c->hr);

    a->hr = vect_s32_add(a->data, b->data, c->data, b->length, b_shr, c_shr);
}


void bfp_s32_add_scalar(
    bfp_s32_t* a,
    const bfp_s32_t* b,
    const float_s32_t c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    right_shift_t b_shr, c_shr;

    vect_s32_add_scalar_prepare(&a->exp, &b_shr, &c_shr, b->exp, c.exp,
                                    b->hr, HR_S32(c.mant));
    int32_t cc = safe_ashr32(c.mant, c_shr);

    a->hr = vect_s32_add_scalar(a->data, b->data, cc, b->length,
                                    b_shr);
}


void bfp_s32_sub(
    bfp_s32_t* a,
    const bfp_s32_t* b,
    const bfp_s32_t* c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == c->length);
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    right_shift_t b_shr, c_shr;

    vect_s32_sub_prepare(&a->exp, &b_shr, &c_shr, b->exp, c->exp, b->hr, c->hr);

    a->hr = vect_s32_sub(a->data, b->data, c->data, b->length, b_shr, c_shr);
}


void bfp_s32_mul(
    bfp_s32_t* a,
    const bfp_s32_t* b,
    const bfp_s32_t* c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == c->length);
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    right_shift_t b_shr, c_shr;
    vect_s32_mul_prepare(&a->exp, &b_shr, &c_shr, b->exp, c->exp, b->hr, c->hr);

    a->hr = vect_s32_mul(a->data, b->data, c->data, b->length, b_shr, c_shr);
}


void bfp_s32_scale(
    bfp_s32_t* a,
    const bfp_s32_t* b,
    const float_s32_t c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    right_shift_t b_shr, c_shr;

    headroom_t c_hr = HR_S32(c.mant);

    vect_s32_scale_prepare(&a->exp, &b_shr, &c_shr, b->exp, c.exp, b->hr, c_hr);

    a->hr = vect_s32_scale(a->data, b->data, b->length, c.mant, b_shr, c_shr);
}


void bfp_s32_abs(
    bfp_s32_t* a,
    const bfp_s32_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    a->exp = b->exp;
    a->hr = vect_s32_abs(a->data, b->data, b->length);
}


float_s64_t bfp_s32_sum(
    const bfp_s32_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length != 0);
#endif

    float_s64_t a;
    a.mant = vect_s32_sum(b->data, b->length);
    a.exp = b->exp;
    return a;
}


float_s64_t bfp_s32_dot(
    const bfp_s32_t* b,
    const bfp_s32_t* c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == c->length);
    assert(b->length != 0);
#endif

    float_s64_t a;
    right_shift_t b_shr, c_shr;

    vect_s32_dot_prepare(&a.exp, &b_shr, &c_shr, b->exp, c->exp, b->hr, c->hr, b->length);
    a.mant = vect_s32_dot(b->data, c->data, b->length, b_shr, c_shr);
    return a;
}


void bfp_s32_clip(
    bfp_s32_t* a,
    const bfp_s32_t* b,
    const int32_t lower_bound,
    const int32_t upper_bound,
    const int bound_exp)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    assert(lower_bound <= upper_bound);

    exponent_t a_exp;
    right_shift_t b_shr;

    int32_t lb = lower_bound;
    int32_t ub = upper_bound;

    vect_s32_clip_prepare(&a_exp, &b_shr, &lb, &ub, b->exp, bound_exp, b->hr);
    
    if(ub == VPU_INT32_MIN){
        /* upper bound must be smaller than any element of b, so set everything to that */
        a->exp = bound_exp;
        a->hr = HR_S32(upper_bound);
        vect_s32_set(a->data, upper_bound, b->length);
    } else if(lb == VPU_INT32_MAX){
        /* lower bound must be larger than any element of b, so set everything to that */
        a->exp = bound_exp;
        a->hr = HR_S32(lower_bound);
        vect_s32_set(a->data, lower_bound, b->length);
    } else if(ub == lb){
        /* upper and lower bounds are indistinguishable */
        a->exp = a_exp;
        a->hr = HR_S32(ub);
        vect_s32_set(a->data, ub, b->length);

    } else {
        /* some elements may be between bounds */
        a->exp = a_exp;
        a->hr = vect_s32_clip(a->data, b->data, b->length, lb, ub, b_shr);
    }
}


void bfp_s32_rect(
    bfp_s32_t* a,
    const bfp_s32_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    a->exp = b->exp;
    a->hr = vect_s32_rect(a->data, b->data, b->length);
}


void bfp_s32_sqrt(
    bfp_s32_t* a,
    const bfp_s32_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    right_shift_t b_shr;

    vect_s32_sqrt_prepare(&a->exp, &b_shr, b->exp, b->hr);

    a->hr = vect_s32_sqrt(a->data, b->data, b->length, b_shr, XMATH_BFP_SQRT_DEPTH_S32);
}


void bfp_s32_inverse(
    bfp_s32_t* a,
    const bfp_s32_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    unsigned scale;

    vect_s32_inverse_prepare(&a->exp, &scale, b->data, b->exp, b->length);

    a->hr = vect_s32_inverse(a->data, b->data, b->length, scale);
}


float_s64_t bfp_s32_abs_sum(
    const bfp_s32_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length != 0);
#endif

    float_s64_t a;
    a.mant = vect_s32_abs_sum(b->data, b->length);
    a.exp = b->exp;
    return a;
}


float_s32_t bfp_s32_mean(
    const bfp_s32_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length != 0);
#endif

    float_s32_t a;

    int64_t sum = vect_s32_sum(b->data, b->length);
    
    headroom_t hr = HR_S64(sum);
    sum = sum << hr;
    int64_t mean = sum / ((int)b->length);
    right_shift_t shr = MAX(0, 32 - HR_S64(mean));

    if(shr > 0)
        mean += ((uint64_t)1 << (shr-1));

    a.mant = (int32_t) (mean >> shr);
    a.exp = b->exp - hr + shr;

    return a;

}


float_s64_t bfp_s32_energy(
    const bfp_s32_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length != 0);
#endif

    float_s64_t a;
    right_shift_t b_shr;
    vect_s32_energy_prepare(&a.exp, &b_shr, b->length, b->exp, b->hr);
    a.mant = vect_s32_energy(b->data, b->length, b_shr);
    return a;
}


float_s32_t bfp_s32_rms(
    const bfp_s32_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length != 0);
#endif

    float_s32_t a;
    exponent_t exp, len_inv_exp;
    const float_s64_t energy64 = bfp_s32_energy(b);
    const int32_t energy32 = s64_to_s32(&exp, energy64.mant, energy64.exp);
    const int32_t len_inv = s32_inverse(&len_inv_exp, b->length);
    const int32_t mean_energy = s32_mul(&exp, energy32, len_inv, exp, len_inv_exp);

    a.mant = s32_sqrt(&a.exp, mean_energy, exp, XMATH_BFP_SQRT_DEPTH_S32);
    return a;
}


float_s32_t bfp_s32_max(
    const bfp_s32_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length != 0);
#endif

    float_s32_t a;
    a.mant = vect_s32_max(b->data, b->length);
    a.exp = b->exp;
    return a;
}


void bfp_s32_max_elementwise(
    bfp_s32_t* a,
    const bfp_s32_t* b,
    const bfp_s32_t* c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
  assert(b->length == c->length);
  assert(b->length == a->length);
  assert(b->length != 0);
#endif

  // The elements of b[] and c[] need to have the same exponent when compared.
  // We'll choose the smallest exponent that leaves at least 1 bit of headroom.
  const unsigned min_required_operand_headroom = 1;
  right_shift_t b_shr, c_shr;
  vect_2vec_prepare(&a->exp, &b_shr, &c_shr,
                        b->exp, c->exp, b->hr, c->hr,
                        min_required_operand_headroom);

  a->hr = vect_s32_max_elementwise(a->data, b->data, c->data,
                                       b->length, b_shr, c_shr);
}


float_s32_t bfp_s32_min(
    const bfp_s32_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length != 0);
#endif

    float_s32_t a;
    a.mant = vect_s32_min(b->data, b->length);
    a.exp = b->exp;
    return a;
}


void bfp_s32_min_elementwise(
    bfp_s32_t* a,
    const bfp_s32_t* b,
    const bfp_s32_t* c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
  assert(b->length == c->length);
  assert(b->length == a->length);
  assert(b->length != 0);
#endif

  // The elements of b[] and c[] need to have the same exponent when compared.
  // We'll choose the smallest exponent that leaves at least 1 bit of headroom.
  const unsigned min_required_operand_headroom = 1;
  right_shift_t b_shr, c_shr;
  vect_2vec_prepare(&a->exp, &b_shr, &c_shr,
                        b->exp, c->exp, b->hr, c->hr,
                        min_required_operand_headroom);

  a->hr = vect_s32_min_elementwise(a->data, b->data, c->data,
                                       b->length, b_shr, c_shr);
}


unsigned bfp_s32_argmax(
    const bfp_s32_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length != 0);
#endif

    return vect_s32_argmax(b->data, b->length);
}


unsigned bfp_s32_argmin(
    const bfp_s32_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length != 0);
#endif

    return vect_s32_argmin(b->data, b->length);
}


void bfp_s32_to_bfp_s16(
    bfp_s16_t* a,
    const bfp_s32_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif
    
    right_shift_t b_shr = 16 - b->hr;

    a->exp = b->exp + b_shr;
    a->hr = 0;

    vect_s32_to_vect_s16(a->data, b->data, b->length, b_shr);
}




void bfp_s32_macc(
    bfp_s32_t* acc,
    const bfp_s32_t* b,
    const bfp_s32_t* c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == c->length);
    assert(b->length == acc->length);
    assert(b->length != 0);
#endif

    right_shift_t acc_shr, b_shr, c_shr;
    vect_s32_macc_prepare(&acc->exp,
                              &acc_shr, &b_shr, &c_shr,
                              acc->exp, b->exp, c->exp,
                              acc->hr, b->hr, c->hr);

    acc->hr = vect_s32_macc(acc->data, b->data, c->data,
                                b->length, acc_shr, b_shr, c_shr);
}

void bfp_s32_nmacc(
    bfp_s32_t* acc,
    const bfp_s32_t* b,
    const bfp_s32_t* c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == c->length);
    assert(b->length == acc->length);
    assert(b->length != 0);
#endif

    right_shift_t acc_shr, b_shr, c_shr;
    vect_s32_macc_prepare(&acc->exp,
                              &acc_shr, &b_shr, &c_shr,
                              acc->exp, b->exp, c->exp,
                              acc->hr, b->hr, c->hr);

    acc->hr = vect_s32_nmacc(acc->data, b->data, c->data,
                                 b->length, acc_shr, b_shr, c_shr);
}


void bfp_s32_convolve_valid(
  bfp_s32_t* a,
  const bfp_s32_t* b,
  const int32_t filter_q30[],
  const unsigned filter_tap_count)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length >= filter_tap_count);
    const unsigned P = filter_tap_count >> 1;
    assert(a->length == (b->length - 2*P));
    assert((filter_tap_count > 0) && (filter_tap_count <= VPU_INT32_EPV) && (filter_tap_count & 1));
#endif

  a->hr = vect_s32_convolve_valid(a->data, b->data, filter_q30, b->length, filter_tap_count);
  a->exp = b->exp;
}


void bfp_s32_convolve_same(
  bfp_s32_t* a,
  const bfp_s32_t* b,
  const int32_t filter_q30[],
  const unsigned filter_tap_count,
  const pad_mode_e padding_mode)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length >= filter_tap_count);
    const unsigned P = filter_tap_count >> 1;
    assert(a->length == b->length);
    assert((filter_tap_count > 0) && (filter_tap_count <= VPU_INT32_EPV) && (filter_tap_count & 1));
#endif

  a->hr = vect_s32_convolve_same(a->data, b->data, filter_q30, b->length, filter_tap_count, padding_mode);
  a->exp = b->exp;

}
