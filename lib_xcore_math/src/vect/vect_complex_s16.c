// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "xmath/xmath.h"



headroom_t vect_complex_s16_add(
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
    const headroom_t re_hr = vect_s16_add(a_real, b_real, c_real, length, b_shr, c_shr);
    const headroom_t im_hr = vect_s16_add(a_imag, b_imag, c_imag, length, b_shr, c_shr);
    return MIN(re_hr, im_hr);
}

headroom_t vect_complex_s16_headroom(
    const int16_t a_real[],
    const int16_t a_imag[],
    const unsigned length)
{
    headroom_t hr_re = vect_s16_headroom(a_real, length);
    headroom_t hr_im = vect_s16_headroom(a_imag, length);
    return MIN(hr_re, hr_im);
}


headroom_t vect_complex_s16_real_scale(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const int16_t c,
    const unsigned length,
    const right_shift_t sat)
{
    const headroom_t re_hr = vect_s16_scale(a_real, b_real, length, c, sat);
    const headroom_t im_hr = vect_s16_scale(a_imag, b_imag, length, c, sat);
    return MIN(re_hr, im_hr);
}


void vect_complex_s16_set(
    int16_t real[],
    int16_t imag[],
    const int16_t real_value,
    const int16_t imag_value,
    const unsigned length)
{
    vect_s16_set(real, real_value, length);
    vect_s16_set(imag, imag_value, length);
}


headroom_t vect_complex_s16_shl(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const unsigned length,
    const left_shift_t shl)
{
    return vect_complex_s16_shr(a_real, a_imag, b_real, b_imag, length, -shl);
}

headroom_t vect_complex_s16_shr(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const unsigned length,
    const right_shift_t shr)
{
    headroom_t hr_re = vect_s16_shr(a_real, b_real, length, shr);
    headroom_t hr_im = vect_s16_shr(a_imag, b_imag, length, shr);

    return MIN(hr_re, hr_im);
}


headroom_t vect_complex_s16_sub(
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
    const headroom_t re_hr = vect_s16_sub(a_real, b_real, c_real, length, b_shr, c_shr);
    const headroom_t im_hr = vect_s16_sub(a_imag, b_imag, c_imag, length, b_shr, c_shr);
    return MIN(re_hr, im_hr);
}

complex_s32_t vect_complex_s16_sum(
    const int16_t b_real[],
    const int16_t b_imag[],
    const unsigned length)
{
    complex_s32_t s;
    s.re = vect_s16_sum(b_real, length);
    s.im = vect_s16_sum(b_imag, length);
    return s;
}


headroom_t vect_complex_s16_add_scalar(
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

  unsigned mask_re = vect_sXX_add_scalar((int32_t*) a_real, (int32_t*)  b_real, 
                                             (length<<1), cc, cc, b_shr, 0x0100);

  cc = c.im;
  cc = (cc << 16) | (cc & 0x0000FFFF);
  unsigned mask_im = vect_sXX_add_scalar((int32_t*) a_imag, (int32_t*) b_imag, 
                                             (length<<1), cc, cc, b_shr, 0x0100);

  return 15 - MAX(mask_re, mask_im);
}
