// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "xs3_math.h"
#include "../../vect/vpu_helper.h"
#include "xs3_vpu_scalar_ops.h"
#include "../../vect/vpu_const_vects.h"



float xs3_pack_float(
    const int32_t mantissa,
    const exponent_t exp)
{
  return ldexp(mantissa, exp);
}



void xs3_unpack_float(
    int32_t* mantissa,
    exponent_t* exp,
    const float input)
{
  *mantissa = INT32_MAX * frexp(input, exp);
  *exp -= 31;
}