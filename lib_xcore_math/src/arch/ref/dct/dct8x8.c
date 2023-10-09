// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "xmath/xmath.h"
#include "vpu_helper.h"
#include "xmath/xs3/vpu_scalar_ops.h"
#include "vpu_const_vects.h"



headroom_t dct8x8_stageA(
    int16_t y[8][8],
    const int8_t x[8][8],
    const int16_t matrix[8][16])
{
  // expand x[] to 16 bits
  int16_t buff[8][8];

  for(int r = 0; r < 8; r++)
    for(int c = 0; c < 8; c++)
      buff[r][c] = x[r][c];

  // Perform an 8-point 16-bit DCT on each row of buff[]

  for(int r = 0; r < 8; r++){
    vpu_int16_acc_t acc[8];
    for(int c = 0; c < 8; c++)
      acc[c] = vlmaccr16(0, &buff[r][0], &matrix[c][0]);
    for(int c = 0; c < 8; c++)
      buff[r][c] = vlsat16(acc[c], 10);
  }

  // move buff[] to y[] and transpose
  for(int r = 0; r < 8; r++){
    for(int c = 0; c < 8; c++){
      y[c][r] = buff[r][c];
    }
  }
  
  // return headroom
  return vect_s16_headroom(&y[0][0], 8*8);
}

headroom_t dct8x8_stageB(
    int8_t y[8][8],
    const int16_t x[8][8],
    const int16_t matrix[8][16],
    const right_shift_t sat)
{
  const right_shift_t shr = 16 + sat;
  int16_t buff[8][8];

  // Perform an 8-point 16-bit DCT on each row of x[], placing it in buff[]
  for(int r = 0; r < 8; r++){
    vpu_int16_acc_t acc[8];
    for(int c = 0; c < 8; c++)
      acc[c] = vlmaccr16(0, &x[r][0], &matrix[c][0]);

    for(int c = 0; c < 8; c++)
      buff[r][c] = vlsat16(acc[c], shr);
  }

  // move buff[] to y[] and transpose
  for(int r = 0; r < 8; r++){
    for(int c = 0; c < 8; c++){
      y[c][r] = vdepth8_16(buff[r][c]);
    }
  }
  
  // return headroom
  headroom_t hr = 8;
  for(int k = 0; k < 8; k++){
    for(int j = 0; j < 8; j++){
      hr = MIN(hr, HR_S8(y[k][j]));
    }
  }
  return hr;
}