// Copyright 2020-2026 XMOS LIMITED.
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
  return s32_divide_s64_s32(dividend, b);
}

int32_t s32_ashr(int32_t x, right_shift_t shr){
  int32_t res;

  // vx4b and xs3a use different conditions for "normal" shift left case due to the difference in the way lsats works.
  // On xs3a shr == -31 will return INT32_MAX / 0 / INT32_MIN from the final else.
  // On vx4b shr == -31 it will compute x << 31, which either overflows or hits INT32_MIN.
  // so result is the same but with no branching.
#if defined(__VX4B__)

  if (shr >= 0) {
    // using xm.shl here as it targets the sra with the negative shift
    // but will also work for when shr > 31
    left_shift_t shl = -shr;
    asm("xm.shl %0, %1, %2": "=r"(res) : "r"(x), "r"(shl));
  }
  else if (shr > -32) {
    int32_t ah = 0, al = 0, sat = 0;
    left_shift_t shl = -shr;
    asm("xm.linsert %0, %1, %2, %3, 32": "=r" (ah), "=r"(al): "r" (x), "r" (shl), "0" (ah), "1" (al));
    asm("xm.sext %0, %1": "=r" (ah): "r" (shl), "0" (ah));
    asm("xm.lsats %0, %1, %2": "=r" (ah), "=r" (res): "r" (sat), "0" (ah), "1" (al));
  }
  else {
    res = (x == 0) ? 0 : (x > 0) ? INT32_MAX : INT32_MIN;
  }

#elif defined(__XS3A__)

  if (shr >= 0) {
    asm("ashr %0, %1, %2": "=r" (res): "r" (x), "r" (shr));
  }
  else if (shr > -31){
    // lsats doesn't work if you give in 0 on xs3a
    // have to shift one more bit up, saturate and extract
    // loses one bit of precision
    int32_t ah = 0, al = 0, sat = 1;
    left_shift_t shl = -shr + 1;
    asm("linsert %0, %1, %2, %3, 32": "=r" (ah), "=r"(al): "r" (x), "r" (shl), "0" (ah), "1" (al));
    asm("sext %0, %1": "=r" (ah): "r" (shl), "0" (ah));
    asm("lsats %0, %1, %2": "=r" (ah), "=r" (al): "r" (sat), "0" (ah), "1" (al));
    asm("lextract %0, %1, %2, %3, 32": "=r" (res): "r" (ah), "r" (al), "r" (sat));
  }
  else {
    res = (x == 0) ? 0 : (x > 0) ? INT32_MAX : INT32_MIN;
  }

#else

  if (shr > 31){
    res = (x >= 0) ? 0 : 0xffffffff;
  }
  else if (shr >=0 ) {
    res = x >> shr;
  }
  else if (shr < -31) {
    res = (x == 0) ? 0 : (x > 0) ? INT32_MAX : INT32_MIN;
  }
  else {
    int64_t tmp = ((int64_t) x) << -shr;
    res = (tmp > INT32_MAX) ? INT32_MAX : (tmp < INT32_MIN) ? INT32_MIN : (int32_t) tmp;
  }
#endif

  return res;
}
