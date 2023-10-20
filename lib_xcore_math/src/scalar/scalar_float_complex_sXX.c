// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "xmath/xmath.h"




static inline int32_t ashr32(int32_t x, right_shift_t shr)
{
  if(shr >= 0)
    return x >> shr;
  
  int64_t tmp = ((int64_t)x) << -shr;

  if(tmp > INT32_MAX)       return INT32_MAX;
  else if(tmp < INT32_MIN)  return INT32_MIN;
  else                      return (int32_t) tmp;
}

// Non-saturating
static inline int64_t ashr64(int64_t x, right_shift_t shr)
{
  if(shr >= 0)
    return x >> shr;
  return x << -shr;
}


float_complex_s32_t float_complex_s32_mul(
    const float_complex_s32_t x,
    const float_complex_s32_t y)
{
  float_complex_s32_t res;

  exponent_t p_exp = x.exp + y.exp;
  int64_t p_re = (((int64_t)x.mant.re) * y.mant.re) - (((int64_t)x.mant.im) * y.mant.im);
  int64_t p_im = (((int64_t)x.mant.re) * y.mant.im) + (((int64_t)x.mant.im) * y.mant.re);

  headroom_t p_hr = MIN( HR_S64(p_re), HR_S64(p_im) );

  right_shift_t p_shr = 32 - p_hr;

  res.mant.re = (int32_t) ashr64(p_re, p_shr);
  res.mant.im = (int32_t) ashr64(p_im, p_shr);
  res.exp = p_exp + p_shr;
  return res;
}


float_complex_s32_t float_complex_s32_add(
    const float_complex_s32_t x,
    const float_complex_s32_t y)
{
  float_complex_s32_t res;

  const headroom_t x_hr = HR_C32(x.mant);
  const headroom_t y_hr = HR_C32(y.mant);

  const exponent_t x_min_exp = x.exp - x_hr;
  const exponent_t y_min_exp = y.exp - y_hr;

  res.exp = MAX(x_min_exp, y_min_exp) + 1;

  const right_shift_t x_shr = res.exp - x.exp;
  const right_shift_t y_shr = res.exp - y.exp;

  res.mant.re = ashr32(x.mant.re, x_shr) + ashr32(y.mant.re, y_shr);
  res.mant.im = ashr32(x.mant.im, x_shr) + ashr32(y.mant.im, y_shr);

  return res;
}


float_complex_s32_t float_complex_s32_sub(
    const float_complex_s32_t x,
    const float_complex_s32_t y)
{
  float_complex_s32_t res;

  const headroom_t x_hr = HR_C32(x.mant);
  const headroom_t y_hr = HR_C32(y.mant);

  const exponent_t x_min_exp = x.exp - x_hr;
  const exponent_t y_min_exp = y.exp - y_hr;

  res.exp = MAX(x_min_exp, y_min_exp) + 1;

  const right_shift_t x_shr = res.exp - x.exp;
  const right_shift_t y_shr = res.exp - y.exp;

  res.mant.re = ashr32(x.mant.re, x_shr) - ashr32(y.mant.re, y_shr);
  res.mant.im = ashr32(x.mant.im, x_shr) - ashr32(y.mant.im, y_shr);

  return res;
}





float_complex_s16_t float_complex_s16_mul(
    const float_complex_s16_t x,
    const float_complex_s16_t y)
{

  exponent_t p_exp = x.exp + y.exp;
  int32_t p_re = (((int32_t)x.mant.re) * y.mant.re) - (((int32_t)x.mant.im) * y.mant.im);
  int32_t p_im = (((int32_t)x.mant.re) * y.mant.im) + (((int32_t)x.mant.im) * y.mant.re);

  headroom_t p_hr = MIN( HR_S32(p_re), HR_S32(p_im) );

  right_shift_t p_shr = 16 - p_hr;

  float_complex_s16_t res;
  res.mant.re = (int16_t) ashr32(p_re, p_shr);
  res.mant.im = (int16_t) ashr32(p_im, p_shr);
  res.exp = p_exp + p_shr;
  return res;
}


float_complex_s16_t float_complex_s16_add(
    const float_complex_s16_t x,
    const float_complex_s16_t y)
{

  const headroom_t x_hr = HR_C16(x.mant);
  const headroom_t y_hr = HR_C16(y.mant);

  const exponent_t x_min_exp = x.exp - x_hr;
  const exponent_t y_min_exp = y.exp - y_hr;

  float_complex_s16_t res;
  res.exp = MAX(x_min_exp, y_min_exp) + 1;

  const right_shift_t x_shr = res.exp - x.exp;
  const right_shift_t y_shr = res.exp - y.exp;

  res.mant.re = (int16_t) ( ashr32(x.mant.re, x_shr) + ashr32(y.mant.re, y_shr) );
  res.mant.im = (int16_t) ( ashr32(x.mant.im, x_shr) + ashr32(y.mant.im, y_shr) );

  return res;
}


float_complex_s16_t float_complex_s16_sub(
    const float_complex_s16_t x,
    const float_complex_s16_t y)
{

  const headroom_t x_hr = HR_C16(x.mant);
  const headroom_t y_hr = HR_C16(y.mant);

  const exponent_t x_min_exp = x.exp - x_hr;
  const exponent_t y_min_exp = y.exp - y_hr;

  float_complex_s16_t res;
  res.exp = MAX(x_min_exp, y_min_exp) + 1;

  const right_shift_t x_shr = res.exp - x.exp;
  const right_shift_t y_shr = res.exp - y.exp;

  res.mant.re = (int16_t) ( ashr32(x.mant.re, x_shr) - ashr32(y.mant.re, y_shr) );
  res.mant.im = (int16_t) ( ashr32(x.mant.im, x_shr) - ashr32(y.mant.im, y_shr) );

  return res;
}


    