// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "xmath/xmath.h"
#include "vpu_helper.h"
#include "xmath/xs3/vpu_scalar_ops.h"
#include "vpu_const_vects.h"



static inline
int32_t mul_qXX(int32_t x, int32_t y, right_shift_t shr, unsigned round){
  int64_t p = ((int64_t)x) * y;
  int64_t m = 0;
  if(round)
   m = (1LL << shr) - 1;
  return (int32_t) ((p + m) >> shr);
}

static inline
int64_t maccs(int64_t acc, int32_t x, int32_t y)
{
  return acc + (((int64_t)x) * y);
}

static inline
int32_t lextract(int64_t acc, unsigned pos)
{
  return (acc >> pos) & 0xFFFFFFFF;
}






sbrad_t radians_to_sbrads(
    const radian_q24_t theta)
{
  const int32_t coef = 0x517cc1b7;
  int64_t acc = maccs(0, coef, theta);
  int32_t tmp = (acc >> 25) & 0xFFFFFFFF;
  int32_t kase = (tmp >> 30) & 0x00000003;
  tmp = tmp << 1;
  if(kase == 1 || kase == 2)
    tmp *= -1;

  return tmp;
}




q2_30 sbrad_tan(
    const sbrad_t theta)
{
  const int coef[] = {
    0x6487ED51, 0x52AEF398, 0x519AF19D, 0x517FFE6D,
    0x517D1CB8, 0x517CCBC9, 0x517CC2D5, 0x6b6cb9bd };
  const right_shift_t shr[] = {1, 3, 5, 7, 9, 11, 13, 15};
  const int32_t weights[] = {
      0x40000000, 0x40000000, 0x40000000, 0x40000000,
      0x40000000, 0x40000000, 0x40000000, 0x40000000 };
  
  int32_t t = theta;

  const int out_mul = (t < 0)? -1 : 1;

  t *= out_mul;

  const int32_t r = lextract(maccs(0, t, t), 30);

  int32_t powers[8];
  powers[0] = t;
  for(int k = 1; k < 8; k++){
    powers[k] = lextract(maccs(0, powers[k-1], r), 30);
  }

  for(int k = 0; k < 8; k++){
    powers[k] = vlsat32(vlmacc32(0, powers[k], coef[k]), shr[k]);
  }

  t = (int32_t) vlmaccr32(0, &powers[0], &weights[0]);

  return t * out_mul;
}




int32_t sbrad_sin(
    const sbrad_t theta)
{
  const int32_t coef[] = {
      0x6487ed51, -0x52aef399, 0x519af19d, -0x4cb4b33a,
      0x541e0d21, -0x78c1d3f8, 0x7a3d0d34, -0x5beb6e7d };
  const right_shift_t shr[] = {1, 2, 5, 9, 14, 20, 26, 32};
  const int32_t weights[] = {
      0x40000000, 0x40000000, 0x40000000, 0x40000000,
      0x40000000, 0x40000000, 0,          0 };
  
  int32_t t = theta;

  const int out_mul = (t < 0)? -1 : 1;

  t *= out_mul;

  const int32_t r = lextract(maccs(0, t, t), 31);

  int32_t powers[8];
  powers[0] = t;
  for(int k = 1; k < 6; k++){
    powers[k] = lextract(maccs(0, powers[k-1], r), 31);
  }

  for(int k = 0; k < 8; k++){
    powers[k] = vlsat32(vlmacc32(0, powers[k], coef[k]), shr[k]);
  }

  t = (int32_t) vlmaccr32(0, &powers[0], &weights[0]);

  return t * out_mul;
}




void s32_to_chunk_s32(
    int32_t a[VPU_INT32_EPV],
    int32_t b)
{
  for(int k = 0; k < VPU_INT32_EPV; k++)
    a[k] = b;
}





q8_24 q24_logistic_fast(
    const q8_24 x)
{
  const int32_t logi_slope[8] = {
    1015490930, 640498971, 297985800, 120120271,
    46079377, 17219453, 6371555, 3717288 };
  const int32_t logi_offset[8] = {
    8388608, 9853420, 12529304, 14613666,
    15770555, 16334225, 16588473, 16661050 };

  const unsigned out_sub = x < 0;
  const q8_24 XX = out_sub? ~x : x;

  int32_t tmp = XX >> 24;
  if(tmp >> 3) {
    tmp = Q24(1.0)-1;
  } else {
    int32_t slope = logi_slope[tmp];
    int32_t offset = logi_offset[tmp];
    tmp = lextract(maccs(0, XX, slope), 32) + offset;
  }

  if(out_sub)
    tmp = Q24(1.0) - tmp;

  return tmp;
}




void q30_powers(
    q2_30 a[],
    const q2_30 b,
    const unsigned length)
{
  a[0] = 0x40000000;
  for(unsigned k = 1; k < length; k++){
    a[k] = vlmul32(a[k-1], b);
  }
}




void s32_odd_powers(
    int32_t a[],
    const int32_t b,
    const unsigned count,
    const right_shift_t shr)
{
  int32_t sqr = mul_qXX(b, b, shr, 0);

  a[0] = b;
  for(unsigned k = 1; k < count; k++){
    a[k] = mul_qXX(a[k-1], sqr, shr, 0);
  }
}



q2_30 q30_exp_small(
    const q2_30 x)
{
  const int32_t coef[9] = {
    0x40000000, 0x20000000, 0x0AAAAAAB,
    0x02AAAAAB, 0x00888889, 0x0016C16C,
    0x00034034, 0x00006807, 0x00000B8F };
  
  q2_30 pow = Q30(1.0);

  int64_t acc = 0;

  acc = maccs(acc, coef[0], pow);

  for(int k = 0; k < 9; k++){
    pow = lextract(maccs(0, pow, x), 30);
    acc = maccs(acc, coef[k], pow);
  }

  return lextract(acc, 30);
}
