// Copyright 2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "bfp_math.h"



complex_float_t* xs3_vect_f32_fft_forward(
    float x[],
    const unsigned fft_length)
{
  int32_t* x_s32 = (int32_t*) &x[0];
  complex_float_t* X = (complex_float_t*) &x[0];

  exponent_t exp = xs3_vect_f32_max_exponent(x, fft_length) + 2;
  xs3_vect_f32_to_s32(x_s32, x, fft_length, exp);

  // Now call the three functions to do an FFT
  xs3_fft_index_bit_reversal((complex_s32_t*) x_s32, fft_length/2);
  headroom_t hr = 2;
  xs3_fft_dit_forward((complex_s32_t*) x_s32, fft_length/2, &hr, &exp);
  xs3_fft_mono_adjust((complex_s32_t*) x_s32, fft_length, 0);

  // And unpack back to floating point values
  xs3_vect_s32_to_f32(x, x_s32, fft_length, exp);

  return X;
}


float* xs3_vect_f32_fft_inverse(
    complex_float_t X[],
    const unsigned fft_length)
{
  int32_t* x_s32 = (int32_t*) &X[0];
  float* x = (float*) &X[0];
  
  exponent_t exp = xs3_vect_f32_max_exponent(x, fft_length) + 2;
  xs3_vect_f32_to_s32(x_s32, x, fft_length, exp);

  xs3_fft_mono_adjust((complex_s32_t*) x_s32, fft_length, 1);
  xs3_fft_index_bit_reversal((complex_s32_t*) x_s32, fft_length/2);
  headroom_t hr = 2;
  xs3_fft_dit_inverse((complex_s32_t*) x_s32, fft_length/2, &hr, &exp);

  xs3_vect_s32_to_f32(x, x_s32, fft_length, exp);

  return x;
}
