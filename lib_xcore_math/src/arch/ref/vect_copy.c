// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>

#include "xmath/xmath.h"
#include "vpu_helper.h"
#include "xmath/xs3/vpu_scalar_ops.h"
#include "vpu_const_vects.h"


headroom_t vect_s16_copy(
  int16_t a[],
  const int16_t b[],
  const unsigned length)
{
  for(unsigned i = 0; i < length; i++)
    a[i] = b[i];

  return vect_s16_headroom(a, length);
}

headroom_t vect_s32_copy(
  int32_t a[],
  const int32_t b[],
  const unsigned length)
{
  for(unsigned i = 0; i < length; i++)
    a[i] = b[i];

  return vect_s32_headroom(a, length);
}

