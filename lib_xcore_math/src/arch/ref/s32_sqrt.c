// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>

#include "xmath/xmath.h"
#include "vpu_helper.h"


static int32_t isqrt_s64(
    int64_t X,
    unsigned depth)
{
  // p = -(2^31)
  int32_t p = 0x80000000;
  p = -p;

  int32_t result = 0;

  for(int i = MAX(depth, 31); i; i--){

      int64_t tmp = result + p;

      if( (tmp*tmp) <= X ){
          result += p;
      }

      p >>= 1;
  }

  return -result;
}



int32_t s32_sqrt(
    exponent_t* a_exp,
    const int32_t B,
    const exponent_t b_exp,
    const unsigned depth)
{
  const headroom_t b_hr = HR_S32(B);

  int64_t X = B << b_hr;
  exponent_t x_exp = b_exp - b_hr;

  X = X << 31;
  x_exp = x_exp - 31;

  if(((unsigned)x_exp) % 2 == 1){
      X = X >> 1;
      x_exp += 1;
  }

  *a_exp = x_exp >> 1;

  return isqrt_s64(X, depth);
}