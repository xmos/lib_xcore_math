// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "xs3_math.h"


void xs3_f32_unpack(
    int32_t* mantissa,
    exponent_t* exp,
    const float input)
{
  *mantissa = INT32_MAX * frexp(input, exp);
  *exp -= 31;
}


float_s32_t xs3_f32_to_float_s32(
    const float x)
{
  float_s32_t res;
  res.mant = round(INT32_MAX * frexpf(x, &res.exp));
  res.exp -= 31;
  return res;
}


float_s32_t xs3_f64_to_float_s32(
    const double x)
{
  float_s32_t res;
  double tmp = frexp(x, &res.exp);
  res.mant = round(INT32_MAX * tmp);
  res.exp -= 31;
  return res;
}


// The coefficients for the power series of sin(x). Currently used by xs3_f32_sin.S
const float sin_coef[] = {
  1.570796326795e+00, -6.459640975062e-01, 7.969262624617e-02, -4.681754135319e-03,
  1.604411847874e-04, -3.598843235212e-06, 5.692172921968e-08, -6.688035109811e-10,
};

const float two_over_pi = 6.366197723676e-01;

float xs3_f32_cos(
    const float theta)
{
  return xs3_f32_sin(theta + (((float)M_PI)/2));
}




const float log2_ps[16] = { 
  1.4426950216f, -0.7213475108f, 0.4808983505f, -0.3606737554f, 
  0.2885390222f, -0.2404491752f, 0.2060992867f, -0.1803368777f, 
  0.1602994502f, -0.1442695111f, 0.1311540902f, -0.1202245876f, 
  0.1109765396f, -0.1030496433f, 0.0961796716f, -0.0901684389f
};
