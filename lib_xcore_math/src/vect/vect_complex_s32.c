// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "xmath/xmath.h"



headroom_t vect_complex_s32_add(
    complex_s32_t a[],
    const complex_s32_t b[],
    const complex_s32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr)
{
    return vect_s32_add( (int32_t*) a, (int32_t*) b, (int32_t*) c, 2*length, b_shr, c_shr);
}



headroom_t vect_complex_s32_headroom(
    const complex_s32_t a[], 
    const unsigned length)
{
    return vect_s32_headroom((int32_t*)a, 2*length);
}


headroom_t vect_complex_s32_real_scale(
    complex_s32_t a[],
    const complex_s32_t b[],
    const int32_t c,
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr)
{
    return vect_s32_scale( (int32_t*) a, (int32_t*) b, 2*length, c, b_shr, c_shr );
}

headroom_t vect_complex_s32_shl(
    complex_s32_t a[],
    const complex_s32_t b[],
    const unsigned length,
    const left_shift_t shl)
{
    return vect_complex_s32_shr(a, b, length, -shl);
}

headroom_t vect_complex_s32_shr(
    complex_s32_t a[],
    const complex_s32_t b[],
    const unsigned length,
    const right_shift_t shr)
{
    return vect_s32_shr((int32_t*) a, (int32_t*) b, 2*length, shr);
}


headroom_t vect_complex_s32_sub(
    complex_s32_t a[], 
    const complex_s32_t b[],
    const complex_s32_t c[],
    const unsigned length, 
    const right_shift_t b_shr,
    const right_shift_t c_shr)
{
    return vect_s32_sub((int32_t*)a, (int32_t*)b, (int32_t*)c, 2*length, b_shr, c_shr);
}


headroom_t vect_complex_s32_add_scalar(
    complex_s32_t a[],
    const complex_s32_t b[],
    const complex_s32_t c,
    const unsigned length,
    const right_shift_t b_shr)
{
  return 31 - vect_sXX_add_scalar((int32_t*) a,(int32_t*)  b, 
                                      (length<<3), c.re, c.im, b_shr, 0x0000);
}