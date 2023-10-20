// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>

#include "xmath/xmath.h"
#include "vpu_helper.h"
#include "xmath/xs3/vpu_scalar_ops.h"
#include "vpu_const_vects.h"



headroom_t vect_s32_convolve_valid(
    int32_t y[],
    const int32_t x[],
    const int32_t b_q30[],
    const unsigned x_length,
    const unsigned b_length)
{
  const unsigned P = b_length >> 1;
  const unsigned y_length = x_length - 2*P;

  for(unsigned k = 0; k < y_length; k++){

    vpu_int32_acc_t acc = 0;
    for(unsigned t = 0; t < b_length; t++){
      acc = vlmacc32(acc, x[k+t], b_q30[t]);
    }

    y[k] = (int32_t) acc;
  }

  return vect_s32_headroom(y, y_length);
}
