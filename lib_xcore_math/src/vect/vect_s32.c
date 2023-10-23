// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "xmath/xmath.h"


headroom_t vect_s32_shr(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const right_shift_t shr)
{
    return vect_s32_shl(a, b, length, -shr);
}

headroom_t vect_s32_add_scalar(
    int32_t a[],
    const int32_t b[],
    const int32_t c,
    const unsigned length,
    const right_shift_t b_shr)
{
  return 31 - vect_sXX_add_scalar(a, b, (length<<2), c, c, b_shr, 0x0000);
}


extern const int32_t exp_small_coef[];
extern const unsigned exp_small_term_count;

void vect_q30_power_series(
    int32_t a[],
    const q2_30 b[],
    const int32_t coef[],
    const unsigned term_count,
    const unsigned length)
{
  (void) coef;
  const unsigned full_chunks = length >> 3;
  const unsigned tail = length & 0x7;

  for(unsigned k = 0; k < full_chunks; k++)
    chunk_q30_power_series(&a[k<<3], &b[k<<3], exp_small_coef, term_count);

  if(tail){
    int32_t DWORD_ALIGNED tmp[8];
    chunk_q30_power_series(&tmp[0], &b[full_chunks<<3], exp_small_coef, term_count);
    memcpy(&a[full_chunks<<3], &tmp[0], tail*sizeof(int32_t));
  }
}


/**
 * 
 * NOTE: Inputs should be between Q30(-0.5) and Q30(0.5).
 */
void vect_q30_exp_small(
    q2_30 a[],
    const q2_30 b[],
    const unsigned length)
{
  vect_q30_power_series(a, b, exp_small_coef, exp_small_term_count, length);
}



void vect_s32_log_base(
    q8_24 a[],
    const int32_t b[],
    const exponent_t b_exp,
    const q2_30 inv_ln_base_q30,
    const unsigned length)
{
  const unsigned full_chunks = length >> 3;
  const unsigned tail = length & 0x7;

  for(unsigned k = 0; k < full_chunks; k++)
    chunk_s32_log(&a[k<<3], &b[k<<3], b_exp);

  if(tail){
    int32_t DWORD_ALIGNED tmp[8];
    chunk_s32_log(&tmp[0], &b[full_chunks<<3], b_exp);
    memcpy(&a[full_chunks<<3], &tmp[0], tail*sizeof(int32_t));
  }

  if(inv_ln_base_q30){
    q2_30 scale_vec[VPU_INT32_EPV];
    s32_to_chunk_s32(&scale_vec[0], inv_ln_base_q30);
    vect_s32_scale(&a[0], &a[0], length, inv_ln_base_q30, 0, 0);
  }
}


void vect_s32_log(
    q8_24 a[],
    const int32_t b[],
    const exponent_t b_exp,
    const unsigned length)
{
  vect_s32_log_base(a, b, b_exp, 0, length);
}



void vect_s32_log2(
    q8_24 a[],
    const int32_t b[],
    const exponent_t b_exp,
    const unsigned length)
{
  vect_s32_log_base(a, b, b_exp, 0x5c551d95, length);
}


void vect_s32_log10(
    q8_24 a[],
    const int32_t b[],
    const exponent_t b_exp,
    const unsigned length)
{
  vect_s32_log_base(a, b, b_exp, 0x1bcb7b15, length);
}


void vect_split_acc_s32_shr(
    split_acc_s32_t a[],
    const unsigned length,
    const right_shift_t shr)
{
  vect_s32_merge_accs((int32_t*) &a[0], &a[0], length);
  vect_s32_shr((int32_t*) &a[0], (int32_t*) &a[0], length, shr);
  vect_s32_split_accs(&a[0], (int32_t*) &a[0], length);
}
