// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "xmath/xmath.h"


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
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
  assert(a->length != 0);
#endif

  a->hr = vect_s16_headroom(a->data, a->length);

  return a->hr;
}

    
void bfp_s16_use_exponent(
    bfp_s16_t* a,
    const exponent_t exp)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
  assert(a->length != 0);
#endif

  right_shift_t delta_p = exp - a->exp;

  if(delta_p == 0) return;

  a->hr = vect_s16_shr(a->data, a->data, a->length, delta_p);
  a->exp = exp;
}


void bfp_s16_shl(
    bfp_s16_t* a,
    const bfp_s16_t* b,
    const left_shift_t shl)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(a->length == b->length);
    assert(b->length != 0);
#endif

    a->exp = b->exp;
    a->hr = vect_s16_shl(a->data, b->data, b->length, shl);
}


void bfp_s16_add(
    bfp_s16_t* a,
    const bfp_s16_t* b,
    const bfp_s16_t* c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == c->length);
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    right_shift_t b_shr, c_shr;

    vect_s16_add_prepare(&a->exp, &b_shr, &c_shr, b->exp, c->exp, b->hr, c->hr);

    a->hr = vect_s16_add(a->data, b->data, c->data, b->length, b_shr, c_shr);
}


void bfp_s16_add_scalar(
    bfp_s16_t* a,
    const bfp_s16_t* b,
    const float c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    right_shift_t b_shr, c_shr;

    int16_t c_mant;
    exponent_t c_exp;
    f32_unpack_s16(&c_mant, &c_exp, c);

    vect_s16_add_scalar_prepare(&a->exp, &b_shr, &c_shr, b->exp, c_exp,
                                    b->hr, HR_S16(c_mant));

    int16_t cc = safe_ashr16(c_mant, c_shr);

    a->hr = vect_s16_add_scalar(a->data, b->data, cc, b->length,
                                    b_shr);
}


void bfp_s16_sub(
    bfp_s16_t* a,
    const bfp_s16_t* b,
    const bfp_s16_t* c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == c->length);
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    right_shift_t b_shr, c_shr;

    vect_s16_sub_prepare(&a->exp, &b_shr, &c_shr, b->exp, c->exp, b->hr, c->hr);

    a->hr = vect_s16_sub(a->data, b->data, c->data, b->length, b_shr, c_shr);
}


void bfp_s16_mul(
    bfp_s16_t* a,
    const bfp_s16_t* b,
    const bfp_s16_t* c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == c->length);
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    right_shift_t a_shr;

    vect_s16_mul_prepare(&a->exp, &a_shr,
            b->exp, c->exp, b->hr, c->hr);

    a->hr = vect_s16_mul(a->data, b->data, c->data, b->length, a_shr);
}


void bfp_s16_scale(
    bfp_s16_t* a,
    const bfp_s16_t* b,
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
    headroom_t alpha_hr = HR_S16(alpha_mant);

    vect_s16_scale_prepare(&a->exp, &a_shr, b->exp, alpha_exp, b->hr, alpha_hr);

    a->hr = vect_s16_scale(a->data, b->data, b->length, alpha_mant, a_shr);
}


void bfp_s16_abs(
    bfp_s16_t* a,
    const bfp_s16_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    a->exp = b->exp;
    a->hr = vect_s16_abs(a->data, b->data, b->length);
}


float_s32_t bfp_s16_sum(
    const bfp_s16_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length != 0);
#endif

    float_s32_t a;
    a.mant = vect_s16_sum(b->data, b->length);
    a.exp = b->exp;
    return a;
}


float_s64_t bfp_s16_dot(
    const bfp_s16_t* b,
    const bfp_s16_t* c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == c->length);
    assert(b->length != 0);
#endif

    float_s64_t a;
    a.mant = vect_s16_dot(b->data, c->data, b->length);
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
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    assert(lower_bound <= upper_bound);

    exponent_t a_exp;
    right_shift_t b_shr;

    int16_t lb = lower_bound;
    int16_t ub = upper_bound;

    vect_s16_clip_prepare(&a_exp, &b_shr, &lb, &ub, b->exp, bound_exp, b->hr);

    if(ub == VPU_INT16_MIN){
        /* upper bound must be smaller than any element of b, so set everything to that */
        a->exp = bound_exp;
        a->hr = HR_S16(upper_bound);
        vect_s16_set(a->data, upper_bound, b->length);
    } else if(lb == VPU_INT16_MAX){
        /* lower bound must be larger than any element of b, so set everything to that */
        a->exp = bound_exp;
        a->hr = HR_S16(lower_bound);
        vect_s16_set(a->data, lower_bound, b->length);
    } else if(ub == lb){
        /* upper and lower bounds are indistinguishable */
        a->exp = a_exp;
        a->hr = HR_S16(ub);
        vect_s16_set(a->data, ub, b->length);

    } else {
        /* some elements may be between bounds */
        a->exp = a_exp;
        a->hr = vect_s16_clip(a->data, b->data, b->length, lb, ub, b_shr);
    }
}


void bfp_s16_rect(
    bfp_s16_t* a,
    const bfp_s16_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    a->exp = b->exp;
    a->hr = vect_s16_rect(a->data, b->data, b->length);
}


void bfp_s16_sqrt(
    bfp_s16_t* a,
    const bfp_s16_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    right_shift_t b_shr;

    vect_s16_sqrt_prepare(&a->exp, &b_shr, b->exp, b->hr);

    a->hr = vect_s16_sqrt(a->data, b->data, b->length, b_shr, XMATH_BFP_SQRT_DEPTH_S16);
}


void bfp_s16_inverse(
    bfp_s16_t* a,
    const bfp_s16_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    unsigned scale;

    vect_s16_inverse_prepare(&a->exp, &scale, b->data, b->exp, b->length);
    
    vect_s16_inverse(a->data, b->data, b->length, scale);
    
    bfp_s16_headroom(a);
}


float_s32_t bfp_s16_abs_sum(
    const bfp_s16_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length != 0);
#endif

    float_s32_t a;
    a.mant = vect_s16_abs_sum(b->data, b->length);
    a.exp = b->exp;
    return a;
}


float bfp_s16_mean(
    const bfp_s16_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length != 0);
#endif

    int32_t sum = vect_s16_sum(b->data, b->length);

    headroom_t hr = HR_S32(sum) + 32;
    int64_t sum64 = ((int64_t)sum) << hr;
    int64_t mean64 = sum64 / ((int)b->length);
    right_shift_t shr = MAX(0, 32 - HR_S64(mean64));

    if(shr > 0)
        mean64 += ((uint64_t)1 << (shr-1));

    return s32_to_f32((int32_t) (mean64 >> shr),
                          b->exp - hr + shr);
}


float_s64_t bfp_s16_energy(
    const bfp_s16_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length != 0);
#endif

    float_s64_t a;
    a.exp = 2*b->exp;
    a.mant = vect_s16_dot(b->data, b->data, b->length);
    return a;
}


float_s32_t bfp_s16_rms(
    const bfp_s16_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length != 0);
#endif

    float_s32_t a;

    exponent_t exp, len_inv_exp;
    const float_s64_t energy64 = bfp_s16_energy(b);
    const int32_t energy32 = s64_to_s32(&exp, energy64.mant, energy64.exp);
    const int32_t len_inv = s32_inverse(&len_inv_exp, b->length);
    const int32_t mean_energy = s32_mul(&exp, energy32, len_inv, exp, len_inv_exp);

    a.mant = s32_sqrt(&a.exp, mean_energy, exp, XMATH_BFP_SQRT_DEPTH_S32);

    return a;
}


float bfp_s16_max(
    const bfp_s16_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length != 0);
#endif

    return s32_to_f32(vect_s16_max(b->data, b->length),
                          b->exp);
}


void bfp_s16_max_elementwise(
    bfp_s16_t* a,
    const bfp_s16_t* b,
    const bfp_s16_t* c)
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

  a->hr = vect_s16_max_elementwise(a->data, b->data, c->data,
                                       b->length, b_shr, c_shr);
}


float bfp_s16_min(
    const bfp_s16_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length != 0);
#endif

    return s32_to_f32(vect_s16_min(b->data, b->length),
                          b->exp);
}


void bfp_s16_min_elementwise(
    bfp_s16_t* a,
    const bfp_s16_t* b,
    const bfp_s16_t* c)
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

  a->hr = vect_s16_min_elementwise(a->data, b->data, c->data,
                                       b->length, b_shr, c_shr);
}


unsigned bfp_s16_argmax(
    const bfp_s16_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length != 0);
#endif

    return vect_s16_argmax(b->data, b->length);
}


unsigned bfp_s16_argmin(
    const bfp_s16_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length != 0);
#endif

    return vect_s16_argmin(b->data, b->length);
}


void bfp_s16_to_bfp_s32(
    bfp_s32_t* a,
    const bfp_s16_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == a->length);
    assert(b->length != 0);
#endif

    const exponent_t a_exp = b->exp;

    a->exp = a_exp - 8;
    a->hr = b->hr + 8;
    vect_s16_to_vect_s32(a->data, b->data, b->length);
}



void bfp_s16_macc(
    bfp_s16_t* acc,
    const bfp_s16_t* b,
    const bfp_s16_t* c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == c->length);
    assert(b->length == acc->length);
    assert(b->length != 0);
#endif

    right_shift_t acc_shr, bc_shr;

    vect_s16_macc_prepare(&acc->exp, &acc_shr, &bc_shr, acc->exp, b->exp, c->exp, acc->hr, b->hr, c->hr);

    acc->hr = vect_s16_macc(acc->data, b->data, c->data, b->length, acc_shr, bc_shr);
}


void bfp_s16_nmacc(
    bfp_s16_t* acc,
    const bfp_s16_t* b,
    const bfp_s16_t* c)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length == c->length);
    assert(b->length == acc->length);
    assert(b->length != 0);
#endif

    right_shift_t acc_shr, bc_shr;

    vect_s16_macc_prepare(&acc->exp, &acc_shr, &bc_shr, acc->exp, b->exp, c->exp, acc->hr, b->hr, c->hr);

    acc->hr = vect_s16_nmacc(acc->data, b->data, c->data, b->length, acc_shr, bc_shr);
}


headroom_t bfp_s16_accumulate(
    split_acc_s32_t acc[],
    const exponent_t acc_exp,
    const bfp_s16_t* b)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS) // See xmath_conf.h
    assert(b->length != 0);
#endif

  const unsigned chunks = b->length >> VPU_INT16_EPV_LOG2;
  const unsigned tail = b->length & (VPU_INT16_EPV - 1);
  const right_shift_t b_shr = acc_exp - b->exp;

  unsigned vpu_ctrl = VPU_INT16_CTRL_INIT; // VPU 16-bit mode with zeroed headroom

  for(unsigned k = 0; k < chunks; k++){
    vpu_ctrl = chunk_s16_accumulate(
        &acc[k], &b->data[k << VPU_INT16_EPV_LOG2], b_shr, vpu_ctrl);
  }

  if(tail){
    int16_t b_tmp[VPU_INT16_EPV] = {0};
    memcpy(&b_tmp[0], &b->data[chunks << VPU_INT16_EPV_LOG2], sizeof(int16_t) * tail);
    
    // for(int k = 0; k < VPU_INT16_EPV; k++){
    //   printf("!! acc[%d].vD[%d] = 0x%04X\n", chunks, k, acc[chunks].vD[k]);
    //   printf("@@ b_tmp[%d] = 0x%04X\n", k, b_tmp[k]);
    // }

    vpu_ctrl = chunk_s16_accumulate(
       &acc[chunks], &b_tmp[0], b_shr, vpu_ctrl);
  }

  return VPU_INT16_HEADROOM_FROM_CTRL(vpu_ctrl);
}
