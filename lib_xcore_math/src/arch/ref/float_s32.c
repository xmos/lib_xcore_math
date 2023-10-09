// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "xmath/xmath.h"
#include "vpu_helper.h"
#include "xmath/xs3/vpu_scalar_ops.h"
#include "vpu_const_vects.h"


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

float_s32_t float_s32_exp(
    const float_s32_t b)
{
  float_s32_t res = {0,0};

  const int32_t one    = 0x40000000;
  const int32_t sqrt_2 = 0x5a82799a;
  const int32_t log2_e = 0x5c551d95;
  const int32_t ln_2   = 0x2c5c85fe;

  headroom_t hr = HR_S32(b.mant);

  int32_t tmp1 = vlashr32(b.mant, -(int)hr);
  tmp1 = vlashr32(tmp1, 1);

  res.exp = (b.exp - hr) + 1;

  // compute y = x * log2(e)
  int32_t y = lextract(maccs(0, log2_e, tmp1), 30);

  if( res.exp >= 0 ){
    res.mant = one;
    res.exp = res.exp - 30;
    return res;
  }

  int frac_bits = -res.exp;
  
  right_shift_t shr = -30 + frac_bits;

  int32_t alpha = 0;
  int32_t rho = 0;
  int32_t beta = 0;
  unsigned mask = 0;

  if(frac_bits == 31){
    alpha = (y < 0)? -1 : 0;
    rho = y & 0x40000000;
    beta = y & 0x3FFFFFFF;
    beta = vlashr32(beta, shr);
  } else if(frac_bits >= 32){
    alpha = (y < 0)? -1 : 0;
    rho = (y < 0)? 1 : 0;
    beta = vlashr32(y, shr) + ((y < 0)? 0x20000000 : 0);
  } else {
    mask = (1 << (frac_bits-1)) - 1;
    beta = y & mask;
    tmp1 = y >> (frac_bits-1);
    rho = tmp1 & 1;
    alpha = tmp1 >> 1;
    beta = vlashr32(beta, shr);
  }

  res.exp = alpha - 30;
  int32_t two_to_rho = rho? sqrt_2 : one;
  int32_t z = lextract(maccs(0, beta, ln_2), 30);
  z = q30_exp_small(z);
  res.mant = lextract(maccs(0, two_to_rho, z), 30);
  return res;
}
