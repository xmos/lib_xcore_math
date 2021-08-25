// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "../../vect/vpu_helper.h"
#include "xs3_vpu_scalar_ops.h"
#include "../../vect/vpu_const_vects.h"


headroom_t xs3_vect_s16_copy(
  int16_t a[],
  const int16_t b[],
  const unsigned length)
{
  for(int i = 0; i < length; i++)
    a[i] = b[i];

  return xs3_vect_s16_headroom(a, length);
}

headroom_t xs3_vect_s32_copy(
  int32_t a[],
  const int32_t b[],
  const unsigned length)
{
  for(int i = 0; i < length; i++)
    a[i] = b[i];

  return xs3_vect_s32_headroom(a, length);
}

