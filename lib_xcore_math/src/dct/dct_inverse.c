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

int32_t idct12_lut[6] = {
0x0408D608, 0x04545E9F, 0x050AB948, 0x06921A9C, 0x0A73D749, 0x1EA52B31
};

int32_t idct16_lut[8] = {
0x0404F467, 0x042E13C1, 0x048919F4, 0x052CB0E6, 0x064E2403, 0x087C4495, 0x0DC79258, 0x28CF2702
};

int32_t idct24_lut[12] = {
0x0201193D, 0x020A07DA, 0x021CB1D1, 0x023ADF5D, 0x0267C6F4, 0x02A8FF02, 0x030886FB, 0x0399935E, 0x04859D6D, 0x0638D5BC, 0x0A406CE8, 0x1E945F22
};

int32_t idct32_lut[16] = {
0x02009E13, 0x02059A2E, 0x020FD16B, 0x021FC9A1, 0x023660DE, 0x0254ECE8, 0x027D71B9, 0x02B300F5, 0x02FA67B7, 0x035B7E79, 0x03E3E8EE, 0x04AD81A9, 0x05EFC8D9, 0x083B2AF2, 0x0DA1641A, 0x28C2917D
};

int32_t idct48_lut[24] = {
0x0100231C, 0x01013D1A, 0x01037731, 0x0106DDEC, 0x010B84F0, 0x01118822, 0x01190D49, 0x0122467D, 0x012D759D, 0x013AF14A, 0x014B2C3A, 0x015EC010, 0x01767E02, 0x01938901, 0x01B78076, 0x01E4C944, 0x021F1125, 0x026C4B74, 0x02D6C1A8, 0x0371E496, 0x04675FE3, 0x06238F58, 0x0A33C9C0, 0x1E902E22
};

int32_t idct64_lut[32] = {
0x010013BF, 0x0100B20E, 0x0101F099, 0x0103D343, 0x0106600A, 0x01099F33, 0x010D9B8B, 0x011262C6, 0x011805F5, 0x011E9A26, 0x01263934, 0x012F02D7, 0x01391E0B, 0x0144BAF7, 0x01521570, 0x01617875, 0x017342FF, 0x0187EEDA, 0x01A01A7D, 0x01BC97AE, 0x01DE81BE, 0x02076085, 0x02396174, 0x0277B8ED, 0x02C7518A, 0x03301D23, 0x03BFD588, 0x049068DE, 0x05D968E6, 0x082B5225, 0x0D97EFB6, 0x28BF6CF6
};

int32_t idct6_matrix[7][8] = {
{ 0x15555555, 0xD6C983A1, 0x24F34E8B, 0xE1D48222, 0x15555555, 0xF4F5017F, 0, 0 },
{ 0x15555555, 0xE1D48222, 0x00000000, 0x1E2B7DDE, 0xD5555555, 0x1E2B7DDE, 0, 0 },
{ 0x15555555, 0xF4F5017F, 0xDB0CB175, 0x1E2B7DDE, 0x15555555, 0xD6C983A1, 0, 0 },
{ 0x15555555, 0x0B0AFE81, 0xDB0CB175, 0xE1D48222, 0x15555555, 0x29367C5F, 0, 0 },
{ 0x15555555, 0x1E2B7DDE, 0x00000000, 0xE1D48222, 0xD5555555, 0xE1D48222, 0, 0 },
{ 0x15555555, 0x29367C5F, 0x24F34E8B, 0x1E2B7DDE, 0x15555555, 0x0B0AFE81, 0, 0 },
{ 2,2,2,2,2,2,2,2 }, // VLSAT vector
};

int32_t idct8_matrix[9][8] = {
{ 0x10000000, 0xE09D6830, 0x1D906BCF, 0xE56499D7, 0x16A09E66, 0xEE38C4C6, 0x0C3EF153, 0xF9C1D1F1 },
{ 0x10000000, 0xE56499D7, 0x0C3EF153, 0x063E2E0F, 0xE95F619A, 0x1F6297D0, 0xE26F9431, 0x11C73B3A },
{ 0x10000000, 0xEE38C4C6, 0xF3C10EAD, 0x1F6297D0, 0xE95F619A, 0xF9C1D1F1, 0x1D906BCF, 0xE56499D7 },
{ 0x10000000, 0xF9C1D1F1, 0xE26F9431, 0x11C73B3A, 0x16A09E66, 0xE56499D7, 0xF3C10EAD, 0x1F6297D0 },
{ 0x10000000, 0x063E2E0F, 0xE26F9431, 0xEE38C4C6, 0x16A09E66, 0x1A9B6629, 0xF3C10EAD, 0xE09D6830 },
{ 0x10000000, 0x11C73B3A, 0xF3C10EAD, 0xE09D6830, 0xE95F619A, 0x063E2E0F, 0x1D906BCF, 0x1A9B6629 },
{ 0x10000000, 0x1A9B6629, 0x0C3EF153, 0xF9C1D1F1, 0xE95F619A, 0xE09D6830, 0xE26F9431, 0xEE38C4C6 },
{ 0x10000000, 0x1F6297D0, 0x1D906BCF, 0x1A9B6629, 0x16A09E66, 0x11C73B3A, 0x0C3EF153, 0x063E2E0F },
{ 2,2,2,2,2,2,2,2 }, // VLSAT vector
};



void dct12_inverse(
    int32_t y[12],
    const int32_t x[12])
{
  const unsigned DCT_N = 12;
  const unsigned HALF_DCT_N = (DCT_N >> 1);
  const signed DCT_N_LOG2_CEIL = 4; // Declare as signed to avoid warning C4146: unary minus operator applied to unsigned type, result still unsigned
  const unsigned HALF_DCT_N_CHUNKS = ((HALF_DCT_N + 7) >> 3);
  const int32_t* dct_lut = &idct12_lut[0];

  DWORD_ALIGNED
  int32_t buff[8];

  ////// Split x[] into even-index elements and odd-index elements
  // place even-index elements in y[]
  // place odd-index elements in buff[]
  vect_s32_unzip(&y[0], &buff[0], (complex_s32_t*) &x[0], HALF_DCT_N);

  ////// Apply [0.5, 0.5] convolution kernel to the odd-index elements (except buff[0])
  idct_convolve(&buff[0], &buff[0], HALF_DCT_N_CHUNKS);


  ////// Compute s[] and t[] vectors, which are the HALF_DCT_N-point IDCTs of the even and
  //     odd elements respectively.
  // Place t[] at the end of y[]
  dct6_inverse(&y[HALF_DCT_N], &y[0]); // s[]
  dct6_inverse(&buff[0], &buff[0]); // t[]
  

  ////// Shift s[] and t[] right 1 bit
  vect_s32_shr(&y[HALF_DCT_N], &y[HALF_DCT_N], HALF_DCT_N, 1);
  vect_s32_shr(&buff[0], &buff[0], HALF_DCT_N, 1);


  ////// Compute t_tilde[], which is t[] multiplied by the appropriate adjustment vector
  idct_scale(&buff[0], dct_lut, HALF_DCT_N_CHUNKS, -DCT_N_LOG2_CEIL);
  
  ////// Compute the sum and difference of s[] and t_tilde[]
  // Sums go at the head of y[], diffs stay in buff[] because it acts on chunks
  idct_adsb(&y[0], &buff[0], &y[HALF_DCT_N], &buff[0], HALF_DCT_N_CHUNKS);

  ////// Finally, flip the contents of buff[] into tail half of y[]
  vect_s32_flip(&y[HALF_DCT_N], &buff[0], HALF_DCT_N);
}




void dct16_inverse(
    int32_t y[16],
    const int32_t x[16])
{
  const unsigned DCT_N = 16;
  const unsigned HALF_DCT_N = (DCT_N >> 1);
  const signed DCT_N_LOG2_CEIL = 4; // Declare as signed to avoid warning C4146: unary minus operator applied to unsigned type, result still unsigned
  const unsigned HALF_DCT_N_CHUNKS = ((HALF_DCT_N + 7) >> 3);
  const int32_t* dct_lut = &idct16_lut[0];

  DWORD_ALIGNED
  int32_t buff[16];
  vect_s32_unzip(&buff[0], &buff[HALF_DCT_N], (complex_s32_t*) &x[0], HALF_DCT_N);
  idct_convolve(&buff[HALF_DCT_N], &buff[HALF_DCT_N], HALF_DCT_N_CHUNKS);
  dct8_inverse(&y[HALF_DCT_N], &buff[0]);
  dct8_inverse(&buff[0], &buff[HALF_DCT_N]);
  vect_s32_shr(&y[HALF_DCT_N], &y[HALF_DCT_N], HALF_DCT_N, 1);
  vect_s32_shr(&buff[0], &buff[0], HALF_DCT_N, 1);
  idct_scale(&buff[0], dct_lut, HALF_DCT_N_CHUNKS, -DCT_N_LOG2_CEIL);
  idct_adsb(&y[0], &buff[0], &y[HALF_DCT_N], &buff[0], HALF_DCT_N_CHUNKS);
  vect_s32_flip(&y[HALF_DCT_N], &buff[0], HALF_DCT_N);
}




void dct24_inverse(
    int32_t y[24],
    const int32_t x[24])
{
  const unsigned DCT_N = 24;
  const unsigned HALF_DCT_N = (DCT_N >> 1);
  const signed DCT_N_LOG2_CEIL = 5; // Declare as signed to avoid warning C4146: unary minus operator applied to unsigned type, result still unsigned
  const unsigned HALF_DCT_N_CHUNKS = ((HALF_DCT_N + 7) >> 3);
  const int32_t* dct_lut = &idct24_lut[0];

  DWORD_ALIGNED
  int32_t buff[28];
  vect_s32_unzip(&buff[0], &buff[HALF_DCT_N], (complex_s32_t*) &x[0], HALF_DCT_N);
  idct_convolve(&buff[HALF_DCT_N], &buff[HALF_DCT_N], HALF_DCT_N_CHUNKS);
  dct12_inverse(&y[HALF_DCT_N], &buff[0]);
  dct12_inverse(&buff[0], &buff[HALF_DCT_N]);
  vect_s32_shr(&y[HALF_DCT_N], &y[HALF_DCT_N], HALF_DCT_N, 1);
  vect_s32_shr(&buff[0], &buff[0], HALF_DCT_N, 1);
  idct_scale(&buff[0], dct_lut, HALF_DCT_N_CHUNKS, -DCT_N_LOG2_CEIL);
  idct_adsb(&y[0], &buff[0], &y[HALF_DCT_N], &buff[0], HALF_DCT_N_CHUNKS);
  vect_s32_flip(&y[HALF_DCT_N], &buff[0], HALF_DCT_N);
}




void dct32_inverse(
    int32_t y[32],
    const int32_t x[32])
{
  const unsigned DCT_N = 32;
  const unsigned HALF_DCT_N = (DCT_N >> 1);
  const signed DCT_N_LOG2_CEIL = 5; // Declare as signed to avoid warning C4146: unary minus operator applied to unsigned type, result still unsigned
  const unsigned HALF_DCT_N_CHUNKS = ((HALF_DCT_N + 7) >> 3);
  const int32_t* dct_lut = &idct32_lut[0];

  DWORD_ALIGNED
  int32_t buff[32];
  vect_s32_unzip(&buff[0], &buff[HALF_DCT_N], (complex_s32_t*) &x[0], HALF_DCT_N);
  idct_convolve(&buff[HALF_DCT_N], &buff[HALF_DCT_N], HALF_DCT_N_CHUNKS);
  dct16_inverse(&y[HALF_DCT_N], &buff[0]);
  dct16_inverse(&buff[0], &buff[HALF_DCT_N]);
  vect_s32_shr(&y[HALF_DCT_N], &y[HALF_DCT_N], HALF_DCT_N, 1);
  vect_s32_shr(&buff[0], &buff[0], HALF_DCT_N, 1);
  idct_scale(&buff[0], dct_lut, HALF_DCT_N_CHUNKS, -DCT_N_LOG2_CEIL);
  idct_adsb(&y[0], &buff[0], &y[HALF_DCT_N], &buff[0], HALF_DCT_N_CHUNKS);
  vect_s32_flip(&y[HALF_DCT_N], &buff[0], HALF_DCT_N);
}




void dct48_inverse(
    int32_t y[48],
    const int32_t x[48])
{
  const unsigned DCT_N = 48;
  const unsigned HALF_DCT_N = (DCT_N >> 1);
  const signed DCT_N_LOG2_CEIL = 6; // Declare as signed to avoid warning C4146: unary minus operator applied to unsigned type, result still unsigned
  const unsigned HALF_DCT_N_CHUNKS = ((HALF_DCT_N + 7) >> 3);
  const int32_t* dct_lut = &idct48_lut[0];

  DWORD_ALIGNED
  int32_t buff[48];
  vect_s32_unzip(&buff[0], &buff[HALF_DCT_N], (complex_s32_t*) &x[0], HALF_DCT_N);
  idct_convolve(&buff[HALF_DCT_N], &buff[HALF_DCT_N], HALF_DCT_N_CHUNKS);
  dct24_inverse(&y[HALF_DCT_N], &buff[0]);
  dct24_inverse(&buff[0], &buff[HALF_DCT_N]);
  vect_s32_shr(&y[HALF_DCT_N], &y[HALF_DCT_N], HALF_DCT_N, 1);
  vect_s32_shr(&buff[0], &buff[0], HALF_DCT_N, 1);
  idct_scale(&buff[0], dct_lut, HALF_DCT_N_CHUNKS, -DCT_N_LOG2_CEIL);
  idct_adsb(&y[0], &buff[0], &y[HALF_DCT_N], &buff[0], HALF_DCT_N_CHUNKS);
  vect_s32_flip(&y[HALF_DCT_N], &buff[0], HALF_DCT_N);
}




void dct64_inverse(
    int32_t y[64],
    const int32_t x[64])
{
  const unsigned DCT_N = 64;
  const unsigned HALF_DCT_N = (DCT_N >> 1);
  const signed DCT_N_LOG2_CEIL = 6; // Declare as signed to avoid warning C4146: unary minus operator applied to unsigned type, result still unsigned
  const unsigned HALF_DCT_N_CHUNKS = ((HALF_DCT_N + 7) >> 3);
  const int32_t* dct_lut = &idct64_lut[0];

  DWORD_ALIGNED
  int32_t buff[64];
  vect_s32_unzip(&buff[0], &buff[HALF_DCT_N], (complex_s32_t*) &x[0], HALF_DCT_N);
  idct_convolve(&buff[HALF_DCT_N], &buff[HALF_DCT_N], HALF_DCT_N_CHUNKS);
  dct32_inverse(&y[HALF_DCT_N], &buff[0]);
  dct32_inverse(&buff[0], &buff[HALF_DCT_N]);
  vect_s32_shr(&y[HALF_DCT_N], &y[HALF_DCT_N], HALF_DCT_N, 1);
  vect_s32_shr(&buff[0], &buff[0], HALF_DCT_N, 1);
  idct_scale(&buff[0], dct_lut, HALF_DCT_N_CHUNKS, -DCT_N_LOG2_CEIL);
  idct_adsb(&y[0], &buff[0], &y[HALF_DCT_N], &buff[0], HALF_DCT_N_CHUNKS);
  vect_s32_flip(&y[HALF_DCT_N], &buff[0], HALF_DCT_N);
}