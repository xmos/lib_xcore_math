// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "xmath/xmath.h"


void vect_float_s32_log_base(
    q8_24 a[],
    const float_s32_t b[],
    const q2_30 inv_ln_base_q30,
    const unsigned length)
{
  const unsigned full_chunks = length >> 3;
  const unsigned tail = length & 0x7;

  for(unsigned k = 0; k < full_chunks; k++)
    chunk_float_s32_log(&a[k<<3], &b[k<<3]);

  if(tail){
    int32_t DWORD_ALIGNED tmp[8];
    chunk_float_s32_log(&tmp[0], &b[full_chunks<<3]);
    memcpy(&a[full_chunks<<3], &tmp[0], tail*sizeof(int32_t));
  }

  if(inv_ln_base_q30){
    q2_30 scale_vec[VPU_INT32_EPV];
    s32_to_chunk_s32(&scale_vec[0], inv_ln_base_q30);
    vect_s32_scale(&a[0], &a[0], length, inv_ln_base_q30, 0, 0);
  }
}


void vect_float_s32_log(
    q8_24 a[],
    const float_s32_t b[],
    const unsigned length)
{
  vect_float_s32_log_base(a, b, 0, length);
}



void vect_float_s32_log2(
    q8_24 a[],
    const float_s32_t b[],
    const unsigned length)
{
  vect_float_s32_log_base(a, b, 0x5c551d95, length);
}


void vect_float_s32_log10(
    q8_24 a[],
    const float_s32_t b[],
    const unsigned length)
{
  vect_float_s32_log_base(a, b, 0x1bcb7b15, length);
}

