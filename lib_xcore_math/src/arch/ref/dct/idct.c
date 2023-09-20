// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "xmath/xmath.h"
#include "../../vect/vpu_helper.h"
#include "xmath/xs3/vpu_scalar_ops.h"
#include "../../vect/vpu_const_vects.h"



void idct_convolve(
    int32_t y[],
    const int32_t x[],
    const unsigned chunks)
{
  const unsigned length = 8*chunks;
  int32_t tmp = x[0];
  for(int k = length - 1; k > 0; k--){
    int64_t X0 = x[k];
    int64_t X1 = x[k-1];
    y[k] = (X0 + X1) >> 1;
  }
  y[0] = tmp;
}

void idct_scale(
    int32_t x[],
    const int32_t idct_lut[],
    const unsigned chunks,
    const right_shift_t shr)
{
  for(size_t k = 0; k < 8*chunks; k++){
    int32_t p = vlmul32(x[k], idct_lut[k]);
    x[k] = vlashr32(p, shr);
  }
}

void idct_adsb(
    int32_t sums[],
    int32_t diffs[],
    const int32_t s[],
    const int32_t t_tilde[],
    const unsigned chunks)
{
  const unsigned length = 8*chunks;
  for(size_t k = 0; k < length; k++){
    int64_t s_elm = s[k];
    int64_t t_tilde_elm = t_tilde[k];
    sums[k] = (s_elm + t_tilde_elm);
    diffs[k] = (s_elm - t_tilde_elm);
  }
}


 

void dct6_inverse(
    int32_t y[6],
    const int32_t x[6])
{
  const unsigned DCT_N = 6;
  int32_t buff[6];

  for(size_t k = 0; k < DCT_N; k++)
    buff[k] = vlsat32(vlmaccr32(0, x, &idct6_matrix[DCT_N-1-k][0]), 2);
  memcpy(y, buff, sizeof(buff));
}


void dct8_inverse(
    int32_t y[8],
    const int32_t x[8])
{
  const unsigned DCT_N = 8;
  int32_t buff[8];

  for(size_t k = 0; k < DCT_N; k++)
    buff[k] = vlsat32(vlmaccr32(0, x, &idct8_matrix[DCT_N-1-k][0]), 2);
  memcpy(y, buff, sizeof(buff));
}


