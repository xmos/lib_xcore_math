// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>
#include <math.h>

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


// The coefficients for the power series of sin(x)
const float sin_coef[] = {
  1.570796326795e+00, -6.459640975062e-01, 7.969262624617e-02, -4.681754135319e-03,
  1.604411847874e-04, -3.598843235212e-06, 5.692172921968e-08, -6.688035109811e-10,
};

const float two_over_pi = 6.366197723676e-01;

float float_sin(
    const float theta)
{
  const int N = 8;

  // alpha = 2 / pi
  const float alpha = two_over_pi;

  // r is effectively the number of quarter-circles represented by the angle theta,
  // so pi/2 radians (90 deg) becomes 1.0,  2*pi becomes 4.0, etc.
  float r = theta * alpha;

  int out_mul = 1;

  // sin(-x) = -sin(x)
  if(r < 0){
    r = -r;
    out_mul *= -1;
  }
  // now:  r >= 0.0

  // sin(x + k*2*pi) = sin(x)   for int k
  // sin(r + k*4) = sin(r)      for int k
  r -=  4 * ((int)(r*0.25f));
  // now:  0.0 <= r < 4.0

  // sin(pi + x) = -sin(x)
  // sin(2 + r) = -sin(r)
  if(r > 2){
    r -= 2;
    out_mul *= -1;
  }
  // now: 0.0 <= r < 2.0

  // sin(pi/2 + x) = sin(pi - x)
  if(r > 1) r = 2.0f - r;
  // now: 0.0 <= r <= 1.0
  // this means we only need to worry about the first quadrant

  /*
    The remaining steps are based on the power series expansion of sin(x):

    sin(x) = x - (x^3)/(3!) + (x^5)/(5!) - (x^7)/(7!) + ...

    r = x * alpha = x * 2 / pi
    beta = 1/alpha = pi/2
    x = r*beta

    sin(x) = r*beta - (beta^3)(r^3)/(3!) + (beta^5)(r^5)/(5!) - (beta^7)(r^7)/7! + ...
           = beta * r - ((beta^3)/(3!)) * r^3 + ((beta^5)/(5!)) * r^5 - ((beta^7)/(7!)) * r^7 + ...

    So the solution is the inner product of two vectors:

    R[] = [r, r^3, r^5, r^7, ...]
    C[] = [beta, (beta^3)/(3!), (beta^5)/(5!), (beta^7)/(7!), ...]

    The coefficient vector C[] has been precomputed and is in the array sin_coef[]

    Each successive term in the R[] vector is just multiplying by another r^2

    phi = r^2
    R[] = [r, r*phi, r*phi*phi, r*phi*phi*phi, ...]
  */
  const float phi = r*r;
  float total = r * sin_coef[0];

  for(int k = 1; k < N; k++) {
    r *= phi;
    total += r * sin_coef[k];
  }

  return out_mul * total;
}

#define PI_HALF_Q24   Q24(M_PI / 2.0)
#define THREE_PI_OVER_TWO_Q24   Q24(3.0 * M_PI / 2.0)

int32_t xs3_scalar_cos(
    const int32_t theta_q24)
{
  // cos(x) = sin(x + pi/2) = sin(x - 3*pi/2)
  // BUT the span of a Q24 ( [-128, 128) --> 256 ) is not an 
  //  integer multiple of 2*pi, so we can't let the angle overflow
  //  always adding pi/2
  const int32_t theta_mod_q24 = theta_q24 
            + ((theta_q24 >= 0)? (-THREE_PI_OVER_TWO_Q24) : (PI_HALF_Q24));
  const int32_t alpha_q31 = xs3_norm_angle(theta_mod_q24);
  return xs3_norm_sin(alpha_q31);
}