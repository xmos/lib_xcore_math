// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "xmath/xmath.h"
#include "vpu_helper.h"
#include "xmath/xs3/vpu_scalar_ops.h"
#include "vpu_const_vects.h"



void vect_s32_flip(
    int32_t a[],
    const int32_t b[],
    const unsigned length)
{
  for(unsigned k = 0; k < (length>>1); k++){
    int32_t x = b[k];
    int32_t y = b[length-1-k];
    a[k] = y;
    a[length-1-k] = x;
  }
}

headroom_t dct_adsb_s32(
    int32_t sums[],
    int32_t diffs[],
    const int32_t head[],
    const int32_t tail[],
    const unsigned chunks,
    const int32_t dct_lut[])
{
  const unsigned length = 8*chunks;
  for(unsigned k = 0; k < length; k++){
    int64_t h = head[k];
    int64_t t = tail[k];
    int64_t s = h + t;
    int64_t d = h - t;
    sums[k] = (int32_t) (s >> 1);
    diffs[k] = (int32_t) (d >> 1);
    diffs[k] = vlmul32(diffs[k], dct_lut[k]);
  }

  return MIN(vect_s32_headroom(sums, length),
             vect_s32_headroom(diffs, length));
}

void dct_deconvolve_s32(
    int32_t res[],
    const int32_t B[],
    const int32_t D[],
    const unsigned res_length)
{
  int32_t prev = D[0] >> 1;
  for(unsigned k = 0; k < (res_length >> 1); k++){
    int32_t b = B[k];
    int32_t d = D[k];
    res[2*k] = b;
    prev = res[2*k+1] = d - prev;
  }
}
 

void dct6_forward(
    int32_t y[6],
    const int32_t x[6])
{
  const unsigned DCT_N = 6;
  int32_t tmp[6];
  // initialising 8 element even though using 6 to fit the vlmaccr32 API
  int32_t x_tmp[8];
  memcpy(x_tmp, x, 6 * sizeof(int32_t));
  for(unsigned k = 0; k < DCT_N; k++)
    tmp[k] = (int32_t) vlmaccr32(0, x_tmp, &dct6_matrix[DCT_N-1-k][0]);
  memcpy(y, tmp, sizeof(tmp));
}


void dct8_forward(
    int32_t y[8],
    const int32_t x[8])
{
  const unsigned DCT_N = 8;
  int32_t tmp[8];
  for(unsigned k = 0; k < DCT_N; k++)
    tmp[k] = vlsat32(vlmaccr32(0, x, &dct8_matrix[DCT_N-1-k][0]), 3);
  memcpy(y, tmp, sizeof(tmp));
}



void dct12_forward(
    int32_t y[12],
    const int32_t x[12])
{
  const unsigned DCT_N = 12;
  const unsigned HALF_DCT_N = 6;
  const unsigned HALF_DCT_N_CHUNKS = 1;
  const int32_t* dct_lut = &dct12_lut[0];

  DWORD_ALIGNED
  int32_t buff[8];

  ////// Split/move data
  vect_s32_flip(&buff[0], &x[HALF_DCT_N], HALF_DCT_N);

  ////// Sum and differences
  dct_adsb_s32(&y[0], &buff[0], &x[0], &buff[0], HALF_DCT_N_CHUNKS, dct_lut);

  ////// Recursive DCT
  dct6_forward(&y[HALF_DCT_N], &y[0]);
  dct6_forward(&buff[0], &buff[0]);

  ////// Down-shift
  vect_s32_shr(&y[HALF_DCT_N], &y[HALF_DCT_N], HALF_DCT_N, 2);
  vect_s32_shr(&buff[0], &buff[0], HALF_DCT_N, 1);


  ////// Interleave
  dct_deconvolve_s32(&y[0], &y[HALF_DCT_N], &buff[0], DCT_N);
}


void dct16_forward(
    int32_t y[16],
    const int32_t x[16])
{
  const unsigned DCT_N = 16;
  const unsigned HALF_DCT_N = 8;
  const unsigned HALF_DCT_N_CHUNKS = 1;
  const int32_t* dct_lut = &dct16_lut[0];

  DWORD_ALIGNED
  int32_t buff[8];

  ////// Split/move data
  vect_s32_flip(&buff[0], &x[HALF_DCT_N], HALF_DCT_N);

  ////// Sum and differences
  dct_adsb_s32(&y[0], &buff[0], &x[0], &buff[0], HALF_DCT_N_CHUNKS, dct_lut);

  ////// Recursive DCT
  dct8_forward(&y[HALF_DCT_N], &y[0]);
  dct8_forward(&buff[0], &buff[0]);

  ////// Down-shift
  vect_s32_shr(&y[HALF_DCT_N], &y[HALF_DCT_N], HALF_DCT_N, 2);
  vect_s32_shr(&buff[0], &buff[0], HALF_DCT_N, 1);


  ////// Interleave
  dct_deconvolve_s32(&y[0], &y[HALF_DCT_N], &buff[0], DCT_N);
}



void dct24_forward(
    int32_t y[24],
    const int32_t x[24])
{
  const unsigned DCT_N = 24;
  const unsigned HALF_DCT_N = 12;
  const unsigned HALF_DCT_N_CHUNKS = 2;
  const int32_t* dct_lut = &dct24_lut[0];

  DWORD_ALIGNED
  int32_t buff[16];

  ////// Split/move data
  vect_s32_flip(&buff[0], &x[HALF_DCT_N], HALF_DCT_N);

  ////// Sum and differences
  dct_adsb_s32(&y[0], &buff[0], &x[0], &buff[0], HALF_DCT_N_CHUNKS, dct_lut);

  ////// Recursive DCT
  dct12_forward(&y[HALF_DCT_N], &y[0]);
  dct12_forward(&buff[0], &buff[0]);

  ////// Down-shift
  vect_s32_shr(&y[HALF_DCT_N], &y[HALF_DCT_N], HALF_DCT_N, 2);
  vect_s32_shr(&buff[0], &buff[0], HALF_DCT_N, 1);


  ////// Interleave
  dct_deconvolve_s32(&y[0], &y[HALF_DCT_N], &buff[0], DCT_N);
}
