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
    f32_unpack_s32(&mant32, exp, input);
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

int64_t s64_ashr(int64_t x, right_shift_t shr){
  int64_t res;

// Not super elegant but all implementations share the nagative shift code
// as it can't be easily implemented in inline assembly.
// Given that there is an else fall-back case, should alsways work as expected.
#if defined(__VX4B__)

  int32_t ah = (int32_t)(x >> 32);
  int32_t al = (int32_t)x;
  int32_t res_hi, res_lo;
  if (shr == 0) {
    res = x;
  } else if (shr >= 32) {
    // xm.shl handles shifts >= 32 correctly (returns sign extension),
    // so this branch covers all shr in [32, +inf).
    left_shift_t shl_eff  = -(shr - 32);
    asm("sra %0, %1, 31": "=r" (res_hi): "r" (ah));
    asm("xm.shl %0, %1, %2": "=r"(res_lo): "r"(ah), "r"(shl_eff));
    res = ((int64_t)res_hi << 32) | (uint32_t)res_lo;
  } else if (shr >= 0) {
    // 1 <= shr <= 31: split the 64-bit shift across the two 32-bit halves
    asm("xm.shl %0, %1, %2": "=r" (res_hi): "r" (ah), "r" (-shr));
    asm("xm.lextract %0, %1, %2, %3, 32": "=r" (res_lo): "r" (ah), "r" (al), "r" (shr));
    res = ((int64_t)res_hi << 32) | (uint32_t)res_lo;
  } else

#elif defined(__XS3A__)

  int32_t ah = (int32_t)(x >> 32);
  int32_t al = (int32_t)x;
  int32_t res_hi, res_lo;
  if (shr == 0) {
    res = x;
  } else if (shr >= 32) {
    // xm.shl handles shifts >= 32 correctly (returns sign extension),
    // so this branch covers all shr in [32, +inf).
    int32_t shr_eff  = shr - 32;
    int32_t shr_sign = 31;
    asm("ashr %0, %1, %2": "=r"(res_hi): "r"(ah), "r"(shr_sign));
    asm("ashr %0, %1, %2": "=r"(res_lo): "r"(ah), "r"(shr_eff));
    res = ((int64_t)res_hi << 32) | (uint32_t)res_lo;
  } else if (shr >= 0) {
    // 1 <= shr <= 31: ashr gives the high word; lextract extracts the
    // 32 bits that straddle the word boundary for the low word.
    asm("ashr %0, %1, %2": "=r"(res_hi): "r"(ah), "r"(shr));
    asm("lextract %0, %1, %2, %3, 32": "=r"(res_lo): "r"(ah), "r"(al), "r"(shr));
    res = ((int64_t)res_hi << 32) | (uint32_t)res_lo;
  } else

#else

  if (shr > 63) {
    res = (x >= 0) ? 0 : (int64_t)0xffffffffffffffffLL;
  } else if (shr >= 0) {
    res = x >> shr;
  } else

#endif

  if (shr < -63) {
    res = (x == 0) ? 0 : (x > 0) ? INT64_MAX : INT64_MIN;
  } else {
    int64_t tmp = (int64_t)((uint64_t)x << -shr);
    res = ((tmp >> -shr) != x) ? ((x > 0) ? INT64_MAX : INT64_MIN) : tmp;
  }

  return res;
}

int16_t s16_ashr(int16_t x, right_shift_t shr){
  int16_t res;

#if defined(__VX4B__)

  if (shr > -16) {
    // using xm.shl here as it works for both positive and negative shifts.
    // For positive, it targets the sra with the negative shift,
    // but will also work for when shr > 31.
    // For negative > -16, fits in the 32 bit word, needs saturation.
    left_shift_t shl = -shr;
    int32_t y = (int32_t)x, zero = 0, _16 = 16;
    asm("xm.shl %0, %1, %2": "=r" (y): "r" (y), "r" (shl));
    // not initialising low word to anything here as xm.lsats going to saturate to 32 + 16 bits,
    // low word is not going to affect the result and we're only interested in the high word
    // should save a cycle
    asm("xm.lsats %0, %1, %2": "=r" (y), "=r"(zero): "r"(_16), "0" (y));
    res = (int16_t)y;
  }
  else {
    res = (x == 0) ? 0 : (x > 0) ? INT16_MAX : INT16_MIN;
  }

#elif defined(__XS3A__)

  if (shr > -16) {
    // using ashr here as it works for both positive and negative shifts.
    // For positive, works as expected.
    // For neagtive, will do a shl.
    // As the shift > -16, it'll fit in the 32 bit word, needs saturation.
    int32_t y = (int32_t)x, zero = 0, _16 = 16;
    asm("ashr %0, %1, %2": "=r" (y): "r" (y), "r" (shr));
    // not initialising low word to anything here as lsats going to saturate to 32 + 16 bits,
    // low word is not going to affect the result and we're only interested in the high word
    // should save a cycle
    asm("lsats %0, %1, %2": "=r" (y), "=r" (zero): "r" (_16), "0" (y));
    res = (int16_t)y;
  }
  else {
    res = (x == 0) ? 0 : (x > 0) ? INT16_MAX : INT16_MIN;
  }

#else

  if (shr > 15){
    res = (x >= 0) ? 0 : 0xffff;
  }
  else if (shr >=0 ) {
    res = x >> shr;
  }
  else if (shr < -15) {
    res = (x == 0) ? 0 : (x > 0) ? INT16_MAX : INT16_MIN;
  }
  else {
    int32_t tmp = ((int32_t) x) << -shr;
    res = (tmp > INT16_MAX) ? INT16_MAX : (tmp < INT16_MIN) ? INT16_MIN : (int16_t) tmp;
  }
#endif

  return res;
}