// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include <stdio.h>

#include "xmath/xmath.h"


void bfp_s16_init(
    bfp_s16_t* a, 
    int16_t* data, 
    const exponent_t exp, 
    const unsigned length,
    const unsigned calc_hr)
{
    a->data = data;
    a->length = length;
    a->exp = exp;
    a->flags = 0;

    if(calc_hr) bfp_s16_headroom(a);
    else        a->hr = 0;
}



void bfp_s32_init(
    bfp_s32_t* a, 
    int32_t* data, 
    const exponent_t exp, 
    const unsigned length,
    const unsigned calc_hr)
{
    a->data = data;
    a->length = length;
    a->exp = exp;
    a->flags = 0;

    if(calc_hr) bfp_s32_headroom(a);
    else        a->hr = 0;
}


void bfp_complex_s16_init(
    bfp_complex_s16_t* a, 
    int16_t* real_data,
    int16_t* imag_data, 
    const exponent_t exp, 
    const unsigned length,
    const unsigned calc_hr)
{
    a->real = real_data;
    a->imag = imag_data;
    a->length = length;
    a->exp = exp;
    a->flags = 0;

    if(calc_hr) bfp_complex_s16_headroom(a);
    else        a->hr = 0;
}


void bfp_complex_s32_init(
    bfp_complex_s32_t* a, 
    complex_s32_t* data, 
    const exponent_t exp, 
    const unsigned length,
    const unsigned calc_hr)
{
    a->data = data;
    a->length = length;
    a->exp = exp;
    a->flags = 0;

    if(calc_hr) bfp_complex_s32_headroom(a);
    else        a->hr = 0;
}



void bfp_s16_set(
    bfp_s16_t* a,
    const int16_t value,
    const exponent_t exp)
{
    a->exp = exp;
    a->hr = HR_S16(value);

    vect_s16_set(a->data, value, a->length);
}


void bfp_s32_set(
    bfp_s32_t* a,
    const int32_t value,
    const exponent_t exp)
{
    a->exp = exp;
    a->hr = HR_S32(value);
    a->flags = 0;

    vect_s32_set(a->data, value, a->length);
}



void bfp_complex_s16_set(
    bfp_complex_s16_t* a,
    const complex_s16_t value,
    const exponent_t exp)
{
    a->exp = exp;
    a->hr = HR_C16(value);
    a->flags = 0;

    vect_s16_set( a->real, value.re, a->length );
    vect_s16_set( a->imag, value.im, a->length );
}



void bfp_complex_s32_set(
    bfp_complex_s32_t* a,
    const complex_s32_t value,
    const exponent_t exp)
{
    a->exp = exp;
    a->hr = HR_C32(value);
    a->flags = 0;

    vect_complex_s32_set( a->data, value.re, value.im, a->length);
}