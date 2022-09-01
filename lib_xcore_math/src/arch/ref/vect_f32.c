// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "xmath/xmath.h"
#include "../../vect/vpu_helper.h"
#include "xmath/xs3/vpu_scalar_ops.h"
#include "../../vect/vpu_const_vects.h"



float vect_f32_dot(
    const float b[],
    const float c[],
    const unsigned length)
{
  float acc = 0.0f;
  for(int k = 0; k < length; k++){
    acc = fmacc(acc, b[k], c[k]);
  }
  return acc;
}


exponent_t vect_f32_max_exponent(
    const float b[], 
    const unsigned length)
{
  exponent_t res = INT32_MIN;
  for(int k = 0; k < length; k++){
    int tmp;
    frexpf(b[k], &tmp);
    tmp -= 31;
    res = (tmp > res)? tmp : res;
  }

  return res;
}


void vect_f32_to_vect_s32(
    int32_t a[],
    const float b[], 
    const unsigned length,
    const exponent_t exp)
{
  for(int k = 0; k < length; k++){
    const float B = b[k];
    const float_s32_t C = f32_to_float_s32(B);
    a[k] = vlashr32(C.mant, exp - C.exp);
  }
} 


void vect_s32_to_vect_f32(
    float a[],
    const int32_t b[], 
    const unsigned length, 
    const exponent_t b_exp)
{
  for(int k = 0; k < length; k++){
    a[k] = (float) ldexp(b[k], b_exp);
  }
}