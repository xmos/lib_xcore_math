// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"




static inline int32_t ashr32(int32_t x, right_shift_t shr)
{
  if(shr >= 0)
    return x >> shr;
  
  int64_t tmp = ((int64_t)x) << -shr;

  if(tmp > INT32_MAX)       return INT32_MAX;
  else if(tmp < INT32_MIN)  return INT32_MIN;
  else                      return tmp;
}



float_s64_t float_s32_to_float_s64(
    const float_s32_t x)
{
  float_s64_t res = {
    .exp = x.exp,
    .mant = x.mant
  };
  return res;
}


float float_s32_to_float(
    const float_s32_t x)
{
  return xs3_pack_float(x.mant, x.exp);
}


float_s32_t float_to_float_s32(
    const float x)
{
  float_s32_t res;
  res.mant = round(INT32_MAX * frexpf(x, &res.exp));
  res.exp -= 31;
  return res;
}


double float_s32_to_double(
    const float_s32_t x)
{
  return ldexp(x.mant, x.exp);
}



float_s32_t double_to_float_s32(
    const double x)
{
  float_s32_t res;
  double tmp = frexp(x, &res.exp);
  res.mant = round(INT32_MAX * tmp);
  res.exp -= 31;
  return res;
}



float_s32_t float_s32_mul(
    const float_s32_t x,
    const float_s32_t y)
{
  float_s32_t res;
  res.mant = xs3_s32_mul(&res.exp, x.mant, y.mant, x.exp, y.exp);
  return res;
}


    
float_s32_t float_s32_add(
    const float_s32_t x,
    const float_s32_t y)
{
  float_s32_t res;

  const headroom_t x_hr = HR_S32(x.mant);
  const headroom_t y_hr = HR_S32(y.mant);

  const exponent_t x_min_exp = x.exp - x_hr;
  const exponent_t y_min_exp = y.exp - y_hr;

  res.exp = MAX(x_min_exp, y_min_exp) + 1;

  const right_shift_t x_shr = res.exp - x.exp;
  const right_shift_t y_shr = res.exp - y.exp;

  res.mant = ashr32(x.mant, x_shr) + ashr32(y.mant, y_shr);

  return res;
}


    
float_s32_t float_s32_sub(
    const float_s32_t x,
    const float_s32_t y)
{
  float_s32_t res;

  const headroom_t x_hr = HR_S32(x.mant);
  const headroom_t y_hr = HR_S32(y.mant);

  const exponent_t x_min_exp = x.exp - x_hr;
  const exponent_t y_min_exp = y.exp - y_hr;

  res.exp = MAX(x_min_exp, y_min_exp) + 1;

  const right_shift_t x_shr = res.exp - x.exp;
  const right_shift_t y_shr = res.exp - y.exp;

  res.mant = ashr32(x.mant, x_shr) - ashr32(y.mant, y_shr);

  return res;
}


    
float_s32_t float_s32_div(
    const float_s32_t x,
    const float_s32_t y)
{
  float_s32_t t;

  t.mant = xs3_s32_inverse(&t.exp, y.mant);
  t.exp -= y.exp;

  return float_s32_mul(x, t);
}


    
float_s32_t float_s32_abs(
    const float_s32_t x)
{
  float_s32_t res = x;
  if(res.mant < 0) res.mant = -res.mant;
  if(res.mant < 0) res.mant = INT32_MAX; // because (-INT32_MIN == INT32_MIN)
  return res;
}

    
unsigned float_s32_gt(
    const float_s32_t x,
    const float_s32_t y)
{
  float_s32_t delta = float_s32_sub(x, y);
  return delta.mant > 0;
}


    
unsigned float_s32_gte(
    const float_s32_t x,
    const float_s32_t y)
{
  float_s32_t delta = float_s32_sub(x, y);
  return delta.mant >= 0;
}

    
float_s32_t float_s32_ema(
    const float_s32_t x,
    const float_s32_t y,
    const fixed_s32_t coef_q30)
{
  float_s32_t t = {
    .exp = -30,
    .mant = coef_q30
  };

  float_s32_t s = {
    .exp = -30,
    .mant = 0x40000000 - t.mant
  };

  return float_s32_add(float_s32_mul(x, t), float_s32_mul(y, s));
}

    
float_s32_t float_s32_sqrt(
    const float_s32_t x)
{
  float_s32_t res = x;
  res.mant = xs3_s32_sqrt(&res.exp, x.mant, x.exp, XS3_S32_SQRT_MAX_DEPTH);
  return res;
}