// Copyright 2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include <xs1.h>
#include <xclib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "bfp_math.h"



void xs3_vect_f32_fft_forward(
    float a[],
    const unsigned length)
{
  int32_t* buffer_s32 = (int32_t*) a;

  exponent_t exp = xs3_vect_f32_max_exponent(a, length) + 2;
  xs3_vect_f32_to_s32(buffer_s32, a, length, exp);

  // Now call the three functions to do an FFT
  xs3_fft_index_bit_reversal((complex_s32_t*) (void*) buffer_s32, length/2);
  headroom_t hr = 2;
  xs3_fft_dit_forward((complex_s32_t*) (void*) buffer_s32, length/2, &hr, &exp);
  xs3_fft_mono_adjust((complex_s32_t*) (void*) buffer_s32, length, 0);

  // And unpack back to floating point values
  xs3_vect_s32_to_f32(a, buffer_s32, length, exp);
}


void xs3_vect_f32_fft_inverse(
    float buffer[],
    const unsigned length)
{
  int32_t* buffer_s32 = (int32_t*) buffer;
  
  exponent_t exp = xs3_vect_f32_max_exponent(buffer, length) + 2;
  xs3_vect_f32_to_s32(buffer_s32, buffer, length, exp);

  xs3_fft_mono_adjust((complex_s32_t*) (void*) buffer_s32, length, 1);
  xs3_fft_index_bit_reversal((complex_s32_t*) (void*) buffer_s32, length/2);
  headroom_t hr = 2;
  xs3_fft_dit_inverse((complex_s32_t*) (void*) buffer_s32, length/2, &hr, &exp);

  xs3_vect_s32_to_f32(buffer, buffer_s32, length, exp);
}
