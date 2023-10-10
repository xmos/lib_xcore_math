// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "xmath/xmath.h"
#include "vpu_helper.h"
#include "xmath/xs3/vpu_scalar_ops.h"
#include "vpu_const_vects.h"


extern const float sin_coef[];
extern const float two_over_pi;

float f32_sin(
    const float theta)
{
  float t = theta;

  int sign = (t >= 0)? 1 : -1;

  t *= sign;

  float rho = t * two_over_pi;

  rho = rho - (4 * floorf(rho / 4.0f));

  if(rho > 2.0){
    rho -= 2.0;
    sign *= -1;
  }

  if(rho > 1.0){
    rho = (float) (2.0) - rho;
  }

  float phi = rho * rho;
  float total = 0;

  for(int k = 0; k < 8; k++){
    total += rho * sin_coef[k];
    rho *= phi;
  }

  return total * sign;
}




float f32_normA(
    exponent_t* exp,
    const float x)
{
  float y = frexpf(x, exp);
  return y;
}

float f32_power_series(
    const float x,
    const float coef[],
    const unsigned terms_count)
{
  float y = x;
  float res = 0;
  for(unsigned k = 0; k < terms_count; k++){
    res += coef[k] * y;
    y *= x;
  }
  return res;
}

extern const float log2_ps[16];

float f32_log2(
    const float x)
{
  exponent_t s;
  float y = f32_normA(&s, x) - 1.0f;
  return f32_power_series(y, log2_ps, 11) + s;
}


