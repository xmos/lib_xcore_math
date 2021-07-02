// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"


// These were originally declared as 'static inline', but everything is a lot cleaner
// if they are not. I'll just trust the compiler to do the right thing, and if it doesn't,
// it isn't much of a hit.

headroom_t xs3_vect_ch_pair_s16_headroom(
    const ch_pair_s16_t a[],
    const unsigned length)
{
    return xs3_vect_s16_headroom((int16_t *) a, 2*length);
}



void xs3_vect_ch_pair_s16_set(
    ch_pair_s16_t data[],
    const int16_t ch_a,
    const int16_t ch_b,
    const unsigned length)
{
    union {
        int32_t s32;
        ch_pair_s16_t cp16;
    } tmp;

    tmp.cp16.ch_a = ch_a;
    tmp.cp16.ch_b = ch_b;
    
    xs3_vect_s32_set((int32_t*) data, tmp.s32, length);
}


headroom_t xs3_vect_ch_pair_s16_shl(
    ch_pair_s16_t a[],
    const ch_pair_s16_t b[],
    const unsigned length,
    const left_shift_t shl)
{
    return xs3_vect_ch_pair_s16_shr(a, b, length, -shl);
}


headroom_t xs3_vect_ch_pair_s16_shr(
    ch_pair_s16_t a[],
    const ch_pair_s16_t b[],
    const unsigned length,
    const right_shift_t shr)
{
    return xs3_vect_s16_shr((int16_t*) a, (int16_t*) b, 2*length, shr);
}


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




headroom_t xs3_vect_ch_pair_s32_headroom(
    const ch_pair_s32_t a[],
    const unsigned length)
{
    return xs3_vect_s32_headroom((int32_t*) a, 2*length);
}

void xs3_vect_ch_pair_s32_set(
    ch_pair_s32_t data[],
    const int32_t ch_a,
    const int32_t ch_b,
    const unsigned length)
{
    xs3_vect_complex_s32_set((complex_s32_t*) data, ch_a, ch_b, length);
}

headroom_t xs3_vect_ch_pair_s32_shl(
    ch_pair_s32_t a[],
    const ch_pair_s32_t b[],
    const unsigned length,
    const left_shift_t shl)
{
    return xs3_vect_ch_pair_s32_shr(a, b, length, -shl);
}

headroom_t xs3_vect_ch_pair_s32_shr(
    ch_pair_s32_t a[],
    const ch_pair_s32_t b[],
    const unsigned length,
    const right_shift_t shr)
{
    return xs3_vect_s32_shr((int32_t*) a, (int32_t*) b, 2*length, shr);
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