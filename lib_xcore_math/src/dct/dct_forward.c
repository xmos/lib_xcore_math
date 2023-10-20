// Copyright 2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "xmath/xmath.h"
#include "vpu_helper.h"

#ifndef __XS3A__
# include "xmath/xs3/vpu_scalar_ops.h"
#endif

int32_t dct12_lut[6] = {
0x3F73D526, 0x3B20D79E, 0x32C64D17, 0x26F5F27F, 0x187DE2A7, 0x085A8A88
};

int32_t dct16_lut[8] = {
0x3FB11B48, 0x3D3E82AE, 0x387165E3, 0x317900D6, 0x2899E64A, 0x1E2B5D38, 0x1294062F, 0x0645E9AF
};

int32_t dct24_lut[12] = {
0x3FDCEBAC, 0x3EC52FA0, 0x3C9A80D4, 0x39665CED, 0x3536CC52, 0x301E24B6, 0x2A32B8CA, 0x238E7673, 0x1C4E7539, 0x149276D6, 0x0C7C5C1E, 0x042F909B
};

int32_t dct32_lut[16] = {
0x3FEC43C7, 0x3F4EAAFE, 0x3E14FDF7, 0x3C42420A, 0x39DAF5E8, 0x36E5068A, 0x3367C090, 0x2F6BBE45, 0x2AFAD269, 0x261FEFFA, 0x20E70F32, 0x1B5D100A, 0x158F9A76, 0x0F8CFCBE, 0x09640837, 0x0323ECBE
};

int32_t dct48_lut[24] = {
0x3FF73A51, 0x3FB11B48, 0x3F252A13, 0x3E54001C, 0x3D3E82AE, 0x3BE5E1F9, 0x3A4B97C9, 0x387165E3, 0x3659541B, 0x3405AE1A, 0x317900D6, 0x2EB617CD, 0x2BBFF9ED, 0x2899E64A, 0x25475089, 0x21CBDD19, 0x1E2B5D38, 0x1A69CABF, 0x168B43C9, 0x1294062F, 0x0E886AE1, 0x0A6CE123, 0x0645E9AF, 0x021811C8
};

int32_t dct64_lut[32] = {
0x3FFB10C1, 0x3FD39B5A, 0x3F84C8E2, 0x3F0EC9F5, 0x3E71E759, 0x3DAE81CF, 0x3CC511D9, 0x3BB6276E, 0x3A8269A3, 0x392A9642, 0x37AF8159, 0x361214B0, 0x34534F41, 0x32744493, 0x30761C18, 0x2E5A1070, 0x2C216EAA, 0x29CD9578, 0x275FF452, 0x24DA0A9A, 0x223D66A8, 0x1F8BA4DC, 0x1CC66E99, 0x19EF7944, 0x17088531, 0x14135C94, 0x1111D263, 0x0E05C135, 0x0AF10A22, 0x07D59396, 0x04B54825, 0x0192155F
};


// Note: rows are in reverse order
// Note: 0's at end are so the input vector doesn't need to be masked off
int32_t dct6_matrix[6][8] = {
{ 0x02120FB8, 0xFA57D866, 0x07BA3752, 0xF845C8AE, 0x05A8279A, 0xFDEDF048, 0, 0 },
{ 0x04000000, 0xF8000000, 0x04000000, 0x04000000, 0xF8000000, 0x04000000, 0, 0 },
{ 0x05A8279A, 0xFA57D866, 0xFA57D866, 0x05A8279A, 0x05A8279A, 0xFA57D866, 0, 0 },
{ 0x06ED9EBA, 0x00000000, 0xF9126146, 0xF9126146, 0x00000000, 0x06ED9EBA, 0, 0 },
{ 0x07BA3752, 0x05A8279A, 0x02120FB8, 0xFDEDF048, 0xFA57D866, 0xF845C8AE, 0, 0 },
{ 0x08000000, 0x08000000, 0x08000000, 0x08000000, 0x08000000, 0x08000000, 0, 0 },
};

// Note: rows are in reverse order
int32_t dct8_matrix[9][8] = {
{ 0x0C7C5C1E, 0xDC71898D, 0x3536CC52, 0xC13AD060, 0x3EC52FA0, 0xCAC933AE, 0x238E7673, 0xF383A3E2 },
{ 0x187DE2A7, 0xC4DF2862, 0x3B20D79E, 0xE7821D59, 0xE7821D59, 0x3B20D79E, 0xC4DF2862, 0x187DE2A7 },
{ 0x238E7673, 0xC13AD060, 0x0C7C5C1E, 0x3536CC52, 0xCAC933AE, 0xF383A3E2, 0x3EC52FA0, 0xDC71898D },
{ 0x2D413CCD, 0xD2BEC333, 0xD2BEC333, 0x2D413CCD, 0x2D413CCD, 0xD2BEC333, 0xD2BEC333, 0x2D413CCD },
{ 0x3536CC52, 0xF383A3E2, 0xC13AD060, 0xDC71898D, 0x238E7673, 0x3EC52FA0, 0x0C7C5C1E, 0xCAC933AE },
{ 0x3B20D79E, 0x187DE2A7, 0xE7821D59, 0xC4DF2862, 0xC4DF2862, 0xE7821D59, 0x187DE2A7, 0x3B20D79E },
{ 0x3EC52FA0, 0x3536CC52, 0x238E7673, 0x0C7C5C1E, 0xF383A3E2, 0xDC71898D, 0xCAC933AE, 0xC13AD060 },
{ 0x40000000, 0x40000000, 0x40000000, 0x40000000, 0x40000000, 0x40000000, 0x40000000, 0x40000000 },
{ 3,3,3,3,3,3,3,3 }, // VLSAT vector
};


void dct32_forward(
    int32_t y[32],
    const int32_t x[32]) 
{
  const unsigned DCT_N = 32;
  const unsigned HALF_DCT_N = (DCT_N >> 1);
  const unsigned HALF_DCT_N_CHUNKS = (HALF_DCT_N + 7) >> 3;
  const int32_t* dct_lut = &dct32_lut[0];

  DWORD_ALIGNED
  int32_t buff[16];

  ////// Split/move data
  vect_s32_flip(&buff[0], &x[HALF_DCT_N], HALF_DCT_N);

  ////// Sum and differences
  dct_adsb_s32(&y[0], &buff[0], &x[0], &buff[0], HALF_DCT_N_CHUNKS, dct_lut);

  ////// Recursive DCT
  dct16_forward(&y[HALF_DCT_N], &y[0]);
  dct16_forward(&buff[0], &buff[0]);

  ////// Down-shift
  vect_s32_shr(&y[HALF_DCT_N], &y[HALF_DCT_N], HALF_DCT_N, 2);
  vect_s32_shr(&buff[0], &buff[0], HALF_DCT_N, 1);


  ////// Interleave
  dct_deconvolve_s32(&y[0], &y[HALF_DCT_N], &buff[0], DCT_N);
}


void dct48_forward(
    int32_t y[48],
    const int32_t x[48])
{
  DWORD_ALIGNED
  int32_t buff[24];

  ////// Split/move data
  // copy tail of x[] into buff[] in reverse order
  vect_s32_flip(&buff[0], &x[24], 24);

  ////// Sum and differences
  // the sums[] will go into y[0:24] and differences in buff[0:24]
  dct_adsb_s32(&y[0], &buff[0], &x[0], &buff[0], 3, dct48_lut);

  ////// Recursive DCT
  // Run dct24() on both the sums and diffs.
  // Calling the results of these two DCTs B[] and D[]
  // D[] = dct24(diffs) = dct24(buff[0:24])  will remain where it is
  // B[] = dct24(sums) = dct24(y[0:24]) will be placed in y[24:48] so that when it's
  //                                    time to interleave it won't be in the way.
  dct24_forward(&y[24], &y[0]);
  dct24_forward(&buff[0], &buff[0]);

  ////// Down-shift
  // right-shift D[] one bit and B[] two bits.
  // The reason for the difference between the two is because of how
  // B[] and D[] were derived. B[] directly has the DCT values for the
  // even-index outputs, but each element of D[] (except D[0]) is the
  // average of two neighboring odd-index elements of the DCT for DCT48(x[]).
  vect_s32_shr(&y[24], &y[24], 24, 2);
  vect_s32_shr(&buff[0], &buff[0], 24, 1);


  ////// Interleave
  // D[] still needs to be deconvolved to get the final values for the odd-index
  // elements of the result. But also because B[] contains the even-index elements
  // and D[] the odd-index elements, they need to be interleaved. This will do
  // both the deconvolution and interleaving simultaneously
  dct_deconvolve_s32(&y[0], &y[24], &buff[0], 48);
}


void dct64_forward(
    int32_t y[64],
    const int32_t x[64])
{
  const unsigned DCT_N = 64;
  const unsigned HALF_DCT_N = (DCT_N >> 1);
  const unsigned HALF_DCT_N_CHUNKS = (HALF_DCT_N + 7) >> 3;
  const int32_t* dct_lut = &dct64_lut[0];

  DWORD_ALIGNED
  int32_t buff[32];

  ////// Split/move data
  vect_s32_flip(&buff[0], &x[HALF_DCT_N], HALF_DCT_N);

  ////// Sum and differences
  dct_adsb_s32(&y[0], &buff[0], &x[0], &buff[0], HALF_DCT_N_CHUNKS, dct_lut);

  ////// Recursive DCT
  dct32_forward(&y[HALF_DCT_N], &y[0]);
  dct32_forward(&buff[0], &buff[0]);

  ////// Down-shift
  vect_s32_shr(&y[HALF_DCT_N], &y[HALF_DCT_N], HALF_DCT_N, 2);
  vect_s32_shr(&buff[0], &buff[0], HALF_DCT_N, 1);


  ////// Interleave
  dct_deconvolve_s32(&y[0], &y[HALF_DCT_N], &buff[0], DCT_N);
}