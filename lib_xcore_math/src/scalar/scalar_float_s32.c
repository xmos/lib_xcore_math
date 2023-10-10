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
  return s32_to_f32(x.mant, x.exp);
}

double float_s32_to_double(
    const float_s32_t x)
{
  return ldexp(x.mant, x.exp);
}

float_s32_t float_s32_mul(
    const float_s32_t x,
    const float_s32_t y)
{
  float_s32_t res;
  res.mant = s32_mul(&res.exp, x.mant, y.mant, x.exp, y.exp);
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

  t.mant = s32_inverse(&t.exp, y.mant);
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
    const uq2_30 coef_q30)
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
  res.mant = s32_sqrt(&res.exp, x.mant, x.exp, S32_SQRT_MAX_DEPTH);
  return res;
}

q2_30 q24_sin(
    const radian_q24_t theta)
{
  const sbrad_t alpha = radians_to_sbrads(theta);
  return sbrad_sin(alpha);
}

#define PI_HALF_Q24   Q24(M_PI / 2.0)
#define THREE_PI_OVER_TWO_Q24   Q24(3.0 * M_PI / 2.0)

q2_30 q24_cos(
    const radian_q24_t theta)
{
  // cos(x) = sin(x + pi/2) = sin(x - 3*pi/2)
  // BUT the span of a Q24 ( [-128, 128) --> 256 ) is not an
  //  integer multiple of 2*pi, so we can't let the angle overflow
  //  always adding pi/2
  const radian_q24_t theta_mod = theta
            + ((theta >= 0)? (-THREE_PI_OVER_TWO_Q24) : (PI_HALF_Q24));
  const sbrad_t alpha = radians_to_sbrads(theta_mod);
  return sbrad_sin(alpha);
}

/**
 * Like radians_to_sbrads, except it takes advantage of the symmetries of
 * tan(theta) instead of sin(theta).
 */
static inline
q1_31 radians_to_tbrads(
    const radian_q24_t theta)
{
  const q1_31 inv_rho = 0x517cc1b7;
  int64_t acc = ((int64_t)inv_rho) * theta;
  q1_31 alpha = (q1_31) (acc >> 24);
  return alpha;
}

float_s32_t q24_tan(
    const radian_q24_t theta)
{
  q1_31 alpha = radians_to_tbrads(theta);

  // sbrad_tan() requires the input to be within the range
  //   -0.5 <= alpha <= 0.5
  // If it isn't, we'll reflect across +/- 0.5, compute the tan() of that,
  // and then return the reciprocal.

  const int32_t hi =  0x40000000;
  const int32_t lo = -0x40000000;

  unsigned inv = (alpha > hi) || (alpha < lo);

  // This should correctly reflect across 0.5 or -0.5 because 0x80000000 = -0x80000000
  if(inv) alpha = 0x80000000 - alpha;

  q2_30 tmp = sbrad_tan(alpha);

  float_s32_t res;
  if(!inv){
    res.mant = tmp;
    res.exp = -30;
  } else {
    res.mant = s32_inverse(&res.exp, tmp);
    res.exp += 30;
  }

  return res;
}