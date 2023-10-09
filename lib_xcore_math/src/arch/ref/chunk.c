// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "xmath/xmath.h"
#include "vpu_helper.h"
#include "xmath/xs3/vpu_scalar_ops.h"
#include "vpu_const_vects.h"


void chunk_float_s32_log(
    q8_24 a[VPU_INT32_EPV],
    const float_s32_t b[VPU_INT32_EPV])
{
  const int32_t coef[6] = {0x01000000, -0x800000, 0x555555, -0x400000, 0x333333, -0x2aaaab };
  const int32_t ln_2 = 0x2c5c85fe;

  for(int k = 0; k < VPU_INT32_EPV; k++){
    int32_t x = b[k].mant;
    exponent_t x_exp = b[k].exp;

    if(x <= 0){
      a[k] = INT32_MIN;
      continue;
    }

    unsigned shl = CLS_S32(x);
    x = x << shl;
    exponent_t new_exp = x_exp - shl;

    q8_24 res = 0;

    res += (new_exp << 24);
    res += (0x20000000);
    res = vlmul32(res, ln_2);

    x = ((uint32_t)x) >> 2;
    x -= 0x40000000;

    int32_t pow = 0x40000000;
    for(int i = 0; i < 6; i++){
      pow = vlmul32(pow, x);
      res += vlmul32(pow, coef[i]);
    }

    a[k] = res;
  }
}

void chunk_s32_log(
    q8_24 a[VPU_INT32_EPV],
    const int32_t b[VPU_INT32_EPV],
    const exponent_t b_exp)
{
  float_s32_t f[VPU_INT32_EPV];
  for(int k = 0; k < VPU_INT32_EPV; k++){
    f[k].mant = b[k];
    f[k].exp = b_exp;
  }
  
  chunk_float_s32_log(a, f);
}

void chunk_q30_power_series(
    int32_t a[VPU_INT32_EPV],
    const q2_30 b[VPU_INT32_EPV],
    const int32_t coef[],
    const unsigned term_count)
{

  for(int k = 0; k < VPU_INT32_EPV; k++){
    int32_t acc = 0;

    int32_t pow = 0x40000000;
    for(unsigned i = 0; i < term_count; i++){
      acc = (int32_t) vlmacc32(acc, pow, coef[8*i]);
      pow = vlmul32(pow, b[k]);
    }

    a[k] = acc;
  }
}

int32_t chunk_s32_dot(
    const int32_t b[VPU_INT32_EPV],
    const q2_30 c[VPU_INT32_EPV])
{
  int64_t total = 0;
  for(int k = 0; k < VPU_INT32_EPV; k++){
    total += vlmul32(b[k], c[k]);
  }
  
  return (int32_t) (0xFFFFFFFF & total);
}
