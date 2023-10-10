// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include <stdint.h>
#include <stdio.h>

#include "xmath/xmath.h"


static void apply_pad_constant(
    int32_t buff[],
    const unsigned P,
    const int32_t pad_value)
{
  for(unsigned i = 0; i < P; i++)
    buff[i] = pad_value;
}

static void apply_pad_reflect(
    int32_t buff[],
    const unsigned P,
    const int32_t signal_in[],
    const unsigned sig_len,
    const unsigned right)
{
  if(!right){
    for(unsigned i = 0; i < P; i++)
      buff[i] = signal_in[P-i];
  } else {
    for(unsigned i = 0; i < P; i++)
      buff[i] = signal_in[sig_len - 2 - i];
  }
}



C_API
headroom_t vect_s32_convolve_same(
    int32_t signal_out[],
    const int32_t signal_in[],
    const int32_t filter_q30[],
    const unsigned signal_in_length,
    const unsigned filter_taps,
    const pad_mode_e padding_mode )
{

  const int P = filter_taps >> 1;

  headroom_t res_hr, hr;

  ////// Do main body
  hr = vect_s32_convolve_valid(
          &signal_out[P],
          &signal_in[0],
          filter_q30,
          signal_in_length,
          filter_taps);

  res_hr = hr;

  const int copy_count = filter_taps + P - 1;

  ////// Do left tail
  int32_t buff[9] = {0};

  for(int i = P; i < copy_count; i++)
    buff[i] = signal_in[i-P];

  switch(padding_mode){
    case PAD_MODE_REFLECT:
      apply_pad_reflect(&buff[0], P, signal_in, signal_in_length, 0);
      break;
    case PAD_MODE_EXTEND:
      apply_pad_constant(&buff[0], P, signal_in[0]);
      break;
    case PAD_MODE_ZERO:
    default:
      apply_pad_constant(&buff[0], P, (int32_t) padding_mode);
  }

  // printf("\n\n\n");
  // for(int i = 0; i < copy_count; i++){
  //   printf("buff[%d] = 0x%08lX\n", i, buff[i]);
  // }
  // printf("\n\n\n");

  hr = vect_s32_convolve_valid(
          &signal_out[0],
          buff,
          filter_q30,
          filter_taps + P - 1,
          filter_taps);
  res_hr = MIN(res_hr, hr);

  ////// Do right tail
  for(unsigned i  = 0; i < filter_taps - 1; i++)
    buff[i] = signal_in[signal_in_length + 1 - filter_taps + i];

  switch(padding_mode){
    case PAD_MODE_REFLECT:
      apply_pad_reflect(&buff[filter_taps-1], P, signal_in, signal_in_length, 1);
      break;
    case PAD_MODE_EXTEND:
      apply_pad_constant(&buff[filter_taps-1], P, signal_in[signal_in_length-1]);
      break;
    case PAD_MODE_ZERO:
    default:
      apply_pad_constant(&buff[filter_taps-1], P, (int32_t) padding_mode);
  }

  // printf("\n\n\n");
  // for(int i = 0; i < copy_count; i++){
  //   printf("buff[%d] = 0x%08lX\n", i, buff[i]);
  // }
  // printf("\n\n\n");

  hr = vect_s32_convolve_valid(
        &signal_out[signal_in_length-P],
        buff,
        filter_q30,
        filter_taps + P - 1,
        filter_taps);
  res_hr = MIN(res_hr, hr);

  return res_hr;

}
