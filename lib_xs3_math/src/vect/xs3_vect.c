// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "xs3_math.h"

// #include "xs3_vpu_scalar_ops.h"



headroom_t xs3_vect_complex_s16_add(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const int16_t c_real[],
    const int16_t c_imag[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr)
{
    const headroom_t re_hr = xs3_vect_s16_add(a_real, b_real, c_real, length, b_shr, c_shr);
    const headroom_t im_hr = xs3_vect_s16_add(a_imag, b_imag, c_imag, length, b_shr, c_shr);
    return MIN(re_hr, im_hr);
}

headroom_t xs3_vect_complex_s16_headroom(
    const int16_t a_real[],
    const int16_t a_imag[],
    const unsigned length)
{
    headroom_t hr_re = xs3_vect_s16_headroom(a_real, length);
    headroom_t hr_im = xs3_vect_s16_headroom(a_imag, length);
    return MIN(hr_re, hr_im);
}


headroom_t xs3_vect_complex_s16_real_scale(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const int16_t c,
    const unsigned length,
    const right_shift_t sat)
{
    const headroom_t re_hr = xs3_vect_s16_scale(a_real, b_real, length, c, sat);
    const headroom_t im_hr = xs3_vect_s16_scale(a_imag, b_imag, length, c, sat);
    return MIN(re_hr, im_hr);
}


void xs3_vect_complex_s16_set(
    int16_t real[],
    int16_t imag[],
    const int16_t real_value,
    const int16_t imag_value,
    const unsigned length)
{
    xs3_vect_s16_set(real, real_value, length);
    xs3_vect_s16_set(imag, imag_value, length);
}


headroom_t xs3_vect_complex_s16_shl(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const unsigned length,
    const left_shift_t shl)
{
    return xs3_vect_complex_s16_shr(a_real, a_imag, b_real, b_imag, length, -shl);
}

headroom_t xs3_vect_complex_s16_shr(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const unsigned length,
    const right_shift_t shr)
{
    headroom_t hr_re = xs3_vect_s16_shr(a_real, b_real, length, shr);
    headroom_t hr_im = xs3_vect_s16_shr(a_imag, b_imag, length, shr);

    return MIN(hr_re, hr_im);
}


headroom_t xs3_vect_complex_s16_sub(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const int16_t c_real[],
    const int16_t c_imag[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr)
{
    const headroom_t re_hr = xs3_vect_s16_sub(a_real, b_real, c_real, length, b_shr, c_shr);
    const headroom_t im_hr = xs3_vect_s16_sub(a_imag, b_imag, c_imag, length, b_shr, c_shr);
    return MIN(re_hr, im_hr);
}

complex_s32_t xs3_vect_complex_s16_sum(
    const int16_t b_real[],
    const int16_t b_imag[],
    const unsigned length)
{
    complex_s32_t s;
    s.re = xs3_vect_s16_sum(b_real, length);
    s.im = xs3_vect_s16_sum(b_imag, length);
    return s;
}


headroom_t xs3_vect_s16_shr(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const right_shift_t shr)
{
    return xs3_vect_s16_shl(a, b, length, -shr);
}



headroom_t xs3_vect_complex_s32_add(
    complex_s32_t a[],
    const complex_s32_t b[],
    const complex_s32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr)
{
    return xs3_vect_s32_add( (int32_t*) a, (int32_t*) b, (int32_t*) c, 2*length, b_shr, c_shr);
}



headroom_t xs3_vect_complex_s32_headroom(
    const complex_s32_t a[], 
    const unsigned length)
{
    return xs3_vect_s32_headroom((int32_t*)a, 2*length);
}


headroom_t xs3_vect_complex_s32_real_scale(
    complex_s32_t a[],
    const complex_s32_t b[],
    const int32_t c,
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr)
{
    return xs3_vect_s32_scale( (int32_t*) a, (int32_t*) b, 2*length, c, b_shr, c_shr );
}

headroom_t xs3_vect_complex_s32_shl(
    complex_s32_t a[],
    const complex_s32_t b[],
    const unsigned length,
    const left_shift_t shl)
{
    return xs3_vect_complex_s32_shr(a, b, length, -shl);
}

headroom_t xs3_vect_complex_s32_shr(
    complex_s32_t a[],
    const complex_s32_t b[],
    const unsigned length,
    const right_shift_t shr)
{
    return xs3_vect_s32_shr((int32_t*) a, (int32_t*) b, 2*length, shr);
}


headroom_t xs3_vect_complex_s32_sub(
    complex_s32_t a[], 
    const complex_s32_t b[],
    const complex_s32_t c[],
    const unsigned length, 
    const right_shift_t b_shr,
    const right_shift_t c_shr)
{
    return xs3_vect_s32_sub((int32_t*)a, (int32_t*)b, (int32_t*)c, 2*length, b_shr, c_shr);
}

headroom_t xs3_vect_s32_shr(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const right_shift_t shr)
{
    return xs3_vect_s32_shl(a, b, length, -shr);
}




headroom_t xs3_vect_s32_add_scalar(
    int32_t a[],
    const int32_t b[],
    const int32_t c,
    const unsigned length,
    const right_shift_t b_shr)
{
  return 31 - xs3_vect_sXX_add_scalar(a, b, (length<<2), c, c, b_shr, 0x0000);
}

headroom_t xs3_vect_s16_add_scalar(
    int16_t a[],
    const int16_t b[],
    const int16_t c,
    const unsigned length,
    const right_shift_t b_shr)
{
  int32_t cc = c;
  cc = (cc << 16) | (cc & 0x0000FFFF);
  return 15 - xs3_vect_sXX_add_scalar((int32_t*) a, (int32_t*)  b, 
                                      (length<<1), cc, cc, b_shr, 0x0100);
}


headroom_t xs3_vect_complex_s32_add_scalar(
    complex_s32_t a[],
    const complex_s32_t b[],
    const complex_s32_t c,
    const unsigned length,
    const right_shift_t b_shr)
{
  return 31 - xs3_vect_sXX_add_scalar((int32_t*) a,(int32_t*)  b, 
                                      (length<<3), c.re, c.im, b_shr, 0x0000);
}


headroom_t xs3_vect_complex_s16_add_scalar(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const complex_s16_t c,
    const unsigned length,
    const right_shift_t b_shr)
{
  int32_t cc = c.re;
  cc = (cc << 16) | (cc & 0x0000FFFF);

  unsigned mask_re = xs3_vect_sXX_add_scalar((int32_t*) a_real, (int32_t*)  b_real, 
                                             (length<<1), cc, cc, b_shr, 0x0100);

  cc = c.im;
  cc = (cc << 16) | (cc & 0x0000FFFF);
  unsigned mask_im = xs3_vect_sXX_add_scalar((int32_t*) a_imag, (int32_t*) b_imag, 
                                             (length<<1), cc, cc, b_shr, 0x0100);

  return 15 - MAX(mask_re, mask_im);
}


extern const int32_t exp_small_coef[];
extern const unsigned exp_small_term_count;

void xs3_vect_q30_power_series(
    int32_t a[],
    const q2_30 b[],
    const int32_t coef[],
    const unsigned term_count,
    const unsigned length)
{
  const unsigned full_chunks = length >> 3;
  const unsigned tail = length & 0x7;

  for(int k = 0; k < full_chunks; k++)
    xs3_chunk_q30_power_series(&a[k<<3], &b[k<<3], exp_small_coef, term_count);

  if(tail){
    int32_t DWORD_ALIGNED tmp[8];
    xs3_chunk_q30_power_series(&tmp[0], &b[full_chunks<<3], exp_small_coef, term_count);
    memcpy(&a[full_chunks<<3], &tmp[0], tail*sizeof(int32_t));
  }
}


/**
 * 
 * NOTE: Inputs should be between Q30(-0.5) and Q30(0.5).
 */
void xs3_vect_q30_exp_small(
    q2_30 a[],
    const q2_30 b[],
    const unsigned length)
{
  xs3_vect_q30_power_series(a, b, exp_small_coef, exp_small_term_count, length);
}



void xs3_vect_float_s32_log_base(
    q8_24 a[],
    const float_s32_t b[],
    const q2_30 inv_ln_base_q30,
    const unsigned length)
{
  const unsigned full_chunks = length >> 3;
  const unsigned tail = length & 0x7;

  for(int k = 0; k < full_chunks; k++)
    xs3_chunk_float_s32_log(&a[k<<3], &b[k<<3]);

  if(tail){
    int32_t DWORD_ALIGNED tmp[8];
    xs3_chunk_float_s32_log(&tmp[0], &b[full_chunks<<3]);
    memcpy(&a[full_chunks<<3], &tmp[0], tail*sizeof(int32_t));
  }

  if(inv_ln_base_q30){
    q2_30 scale_vec[VPU_INT32_EPV];
    xs3_s32_to_chunk_s32(&scale_vec[0], inv_ln_base_q30);
    xs3_vect_s32_scale(&a[0], &a[0], length, inv_ln_base_q30, 0, 0);
  }
}


void xs3_vect_float_s32_log(
    q8_24 a[],
    const float_s32_t b[],
    const unsigned length)
{
  xs3_vect_float_s32_log_base(a, b, 0, length);
}



void xs3_vect_float_s32_log2(
    q8_24 a[],
    const float_s32_t b[],
    const unsigned length)
{
  xs3_vect_float_s32_log_base(a, b, 0x5c551d95, length);
}


void xs3_vect_float_s32_log10(
    q8_24 a[],
    const float_s32_t b[],
    const unsigned length)
{
  xs3_vect_float_s32_log_base(a, b, 0x1bcb7b15, length);
}



void xs3_vect_s32_log_base(
    q8_24 a[],
    const int32_t b[],
    const exponent_t b_exp,
    const q2_30 inv_ln_base_q30,
    const unsigned length)
{
  const unsigned full_chunks = length >> 3;
  const unsigned tail = length & 0x7;

  for(int k = 0; k < full_chunks; k++)
    xs3_chunk_s32_log(&a[k<<3], &b[k<<3], b_exp);

  if(tail){
    int32_t DWORD_ALIGNED tmp[8];
    xs3_chunk_s32_log(&tmp[0], &b[full_chunks<<3], b_exp);
    memcpy(&a[full_chunks<<3], &tmp[0], tail*sizeof(int32_t));
  }

  if(inv_ln_base_q30){
    q2_30 scale_vec[VPU_INT32_EPV];
    xs3_s32_to_chunk_s32(&scale_vec[0], inv_ln_base_q30);
    xs3_vect_s32_scale(&a[0], &a[0], length, inv_ln_base_q30, 0, 0);
  }
}


void xs3_vect_s32_log(
    q8_24 a[],
    const int32_t b[],
    const exponent_t b_exp,
    const unsigned length)
{
  xs3_vect_s32_log_base(a, b, b_exp, 0, length);
}



void xs3_vect_s32_log2(
    q8_24 a[],
    const int32_t b[],
    const exponent_t b_exp,
    const unsigned length)
{
  xs3_vect_s32_log_base(a, b, b_exp, 0x5c551d95, length);
}


void xs3_vect_s32_log10(
    q8_24 a[],
    const int32_t b[],
    const exponent_t b_exp,
    const unsigned length)
{
  xs3_vect_s32_log_base(a, b, b_exp, 0x1bcb7b15, length);
}