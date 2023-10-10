// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>

#include "xmath/xmath.h"

void f32_unpack_s16(
    int16_t* mantissa,
    exponent_t* exp,
    const float input)
{
    int32_t mant32;
    f32_unpack(&mant32, exp, input);
    *mantissa = s32_to_s16(exp, mant32, *exp);
}

int32_t s64_to_s32(
    exponent_t* a_exp,
    const int64_t b,
    const exponent_t b_exp)
{
  const headroom_t b_hr = HR_S64(b);
  const right_shift_t shr = MAX( 0, (int)(32-b_hr) );
  *a_exp = b_exp + shr;
  return (int32_t) (b >> shr);
}


int16_t s32_to_s16(
    exponent_t* a_exp,
    const int32_t b,
    const exponent_t b_exp)
{
  const headroom_t b_hr = HR_S32(b);
  const right_shift_t shr = MAX( 0, (int)(16-b_hr) );
  *a_exp = b_exp + shr;
  return (int16_t) (b >> shr);
}
    
int32_t s16_to_s32(
    exponent_t* a_exp,
    const int16_t b,
    const exponent_t b_exp,
    const unsigned remove_hr)
{
  const left_shift_t shl = remove_hr? (16+HR_S16(b)) : 0;
  *a_exp = b_exp - shl;
  return ((int32_t)b) << shl;
}


int16_t s16_mul(
    exponent_t* a_exp,
    const int16_t b,
    const int16_t c,
    const exponent_t b_exp,
    const exponent_t c_exp)
{
  // astew: Might be quicker to just use the VPU..
  int16_t WORD_ALIGNED a;
  right_shift_t a_shr;
  vect_s16_mul_prepare(a_exp, &a_shr, b_exp, c_exp, HR_S16(b), HR_S16(c));
  vect_s16_mul(&a, &b, &c, 1, a_shr);
  return a;
}


int32_t s32_mul(
    exponent_t* a_exp,
    const int32_t b,
    const int32_t c,
    const exponent_t b_exp,
    const exponent_t c_exp)
{
  // astew: Might be quicker to just use the VPU..
  int32_t WORD_ALIGNED a;
  right_shift_t b_shr, c_shr;
  vect_s32_mul_prepare(a_exp, &b_shr, &c_shr, b_exp, c_exp, HR_S32(b), HR_S32(c));
  vect_s32_mul(&a, &b, &c, 1, b_shr, c_shr);
  return a;
}


int16_t s16_inverse(
    exponent_t* a_exp,
    const int16_t b)
{
  const headroom_t b_hr = HR_S16(b);
  const signed scale = 2*14 - b_hr; // Declare as signed to avoid warning C4146: unary minus operator applied to unsigned type, result still unsigned
  const int32_t dividend = 1 << scale;
  *a_exp = -scale;
  return (int16_t) (dividend / b);
}


int32_t s32_inverse(
    exponent_t* a_exp,
    const int32_t b)
{
  const headroom_t b_hr = HR_S32(b);
  const signed scale = 2*30 - b_hr;  // Declare as signed to avoid warning C4146: unary minus operator applied to unsigned type, result still unsigned
  const int64_t dividend = 1LL << scale;
  *a_exp = -scale;
  return (int32_t) (dividend / b);

}
