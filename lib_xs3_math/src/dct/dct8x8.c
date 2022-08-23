// Copyright 2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "xmath/xmath.h"
#include "../vect/vpu_helper.h"


int16_t dct8_matrix_16bit[8][16] = {
{ 0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0,0,0,0,0,0,0,0 },
{ 0x3EC5, 0x3537, 0x238E, 0xC7C, 0xF384, 0xDC72, 0xCAC9, 0xC13B, 0,0,0,0,0,0,0,0 },
{ 0x3B21, 0x187E, 0xE782, 0xC4DF, 0xC4DF, 0xE782, 0x187E, 0x3B21, 0,0,0,0,0,0,0,0 },
{ 0x3537, 0xF384, 0xC13B, 0xDC72, 0x238E, 0x3EC5, 0xC7C, 0xCAC9, 0,0,0,0,0,0,0,0 },
{ 0x2D41, 0xD2BF, 0xD2BF, 0x2D41, 0x2D41, 0xD2BF, 0xD2BF, 0x2D41, 0,0,0,0,0,0,0,0 },
{ 0x238E, 0xC13B, 0xC7C, 0x3537, 0xCAC9, 0xF384, 0x3EC5, 0xDC72, 0,0,0,0,0,0,0,0 },
{ 0x187E, 0xC4DF, 0x3B21, 0xE782, 0xE782, 0x3B21, 0xC4DF, 0x187E, 0,0,0,0,0,0,0,0 },
{ 0xC7C, 0xDC72, 0x3537, 0xC13B, 0x3EC5, 0xCAC9, 0x238E, 0xF384, 0,0,0,0,0,0,0,0 },
};


int16_t idct8_matrix_16bit[8][16] = {
{ 0x2000, 0x3EC5, 0x3B21, 0x3537, 0x2D41, 0x238E, 0x187E, 0xC7C, 0,0,0,0,0,0,0,0 },
{ 0x2000, 0x3537, 0x187E, 0xF384, 0xD2BF, 0xC13B, 0xC4DF, 0xDC72, 0,0,0,0,0,0,0,0 },
{ 0x2000, 0x238E, 0xE782, 0xC13B, 0xD2BF, 0xC7C, 0x3B21, 0x3537, 0,0,0,0,0,0,0,0 },
{ 0x2000, 0xC7C, 0xC4DF, 0xDC72, 0x2D41, 0x3537, 0xE782, 0xC13B, 0,0,0,0,0,0,0,0 },
{ 0x2000, 0xF384, 0xC4DF, 0x238E, 0x2D41, 0xCAC9, 0xE782, 0x3EC5, 0,0,0,0,0,0,0,0 },
{ 0x2000, 0xDC72, 0xE782, 0x3EC5, 0xD2BF, 0xF384, 0x3B21, 0xCAC9, 0,0,0,0,0,0,0,0 },
{ 0x2000, 0xCAC9, 0x187E, 0xC7C, 0xD2BF, 0x3EC5, 0xC4DF, 0x238E, 0,0,0,0,0,0,0,0 },
{ 0x2000, 0xC13B, 0x3B21, 0xCAC9, 0x2D41, 0xDC72, 0x187E, 0xF384, 0,0,0,0,0,0,0,0 },
};


headroom_t dct8x8_forward(
    int8_t y[8][8],
    const int8_t x[8][8],
    const right_shift_t sat)
{
  int16_t DWORD_ALIGNED buff[8][8];
  dct8x8_stageA(buff, x, dct8_matrix_16bit);
  return dct8x8_stageB(y, buff, dct8_matrix_16bit, sat);
}



headroom_t dct8x8_inverse(
    int8_t y[8][8],
    const int8_t x[8][8],
    const right_shift_t sat)
{
  int16_t DWORD_ALIGNED buff[8][8];
  dct8x8_stageA(buff, x, idct8_matrix_16bit);
  return dct8x8_stageB(y, buff, idct8_matrix_16bit, sat);
}