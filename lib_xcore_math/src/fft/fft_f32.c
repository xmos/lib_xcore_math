// Copyright 2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "xmath/xmath.h"



complex_float_t* fft_f32_forward(
    float x[],
    const unsigned fft_length)
{
  int32_t* x_s32 = (int32_t*) &x[0];
  complex_float_t* X = (complex_float_t*) &x[0];

  exponent_t exp = vect_f32_max_exponent(x, fft_length) + 2;
  vect_f32_to_vect_s32(x_s32, x, fft_length, exp);

  // Now call the three functions to do an FFT
  fft_index_bit_reversal((complex_s32_t*) x_s32, fft_length/2);
  headroom_t hr = 2;
  fft_dit_forward((complex_s32_t*) x_s32, fft_length/2, &hr, &exp);
  fft_mono_adjust((complex_s32_t*) x_s32, fft_length, 0);

  // And unpack back to floating point values
  vect_s32_to_vect_f32(x, x_s32, fft_length, exp);

  return X;
}


float* fft_f32_inverse(
    complex_float_t X[],
    const unsigned fft_length)
{
  int32_t* x_s32 = (int32_t*) &X[0];
  float* x = (float*) &X[0];
  
  exponent_t exp = vect_f32_max_exponent(x, fft_length) + 2;
  vect_f32_to_vect_s32(x_s32, x, fft_length, exp);

  fft_mono_adjust((complex_s32_t*) x_s32, fft_length, 1);
  fft_index_bit_reversal((complex_s32_t*) x_s32, fft_length/2);
  headroom_t hr = 2;
  fft_dit_inverse((complex_s32_t*) x_s32, fft_length/2, &hr, &exp);

  vect_s32_to_vect_f32(x, x_s32, fft_length, exp);

  return x;
}
