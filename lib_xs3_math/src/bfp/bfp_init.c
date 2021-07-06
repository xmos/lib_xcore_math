// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include <stdio.h>

#include "bfp_math.h"

#include "vect/xs3_vect_s32.h"
#include "vect/xs3_vect_s16.h"


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




void bfp_ch_pair_s16_init(
    bfp_ch_pair_s16_t* a, 
    ch_pair_s16_t* data, 
    const exponent_t exp, 
    const unsigned length,
    const unsigned calc_hr)
{
    a->data = data;
    a->length = length;
    a->exp = exp;
    a->flags = 0;

    if(calc_hr) bfp_ch_pair_s16_headroom(a);
    else        a->hr = 0;
}


    
void bfp_ch_pair_s32_init(
    bfp_ch_pair_s32_t* a, 
    ch_pair_s32_t* data, 
    const exponent_t exp, 
    const unsigned length,
    const unsigned calc_hr)
{
    a->data = data;
    a->length = length;
    a->exp = exp;
    a->flags = 0;

    if(calc_hr) bfp_ch_pair_s32_headroom(a);
    else        a->hr = 0;
}



void bfp_s16_set(
    bfp_s16_t* a,
    const int16_t value,
    const exponent_t exp)
{
    a->exp = exp;
    a->hr = HR_S16(value);

    xs3_vect_s16_set(a->data, value, a->length);
}


void bfp_s32_set(
    bfp_s32_t* a,
    const int32_t value,
    const exponent_t exp)
{
    a->exp = exp;
    a->hr = HR_S32(value);
    a->flags = 0;

    xs3_vect_s32_set(a->data, value, a->length);
}



void bfp_complex_s16_set(
    bfp_complex_s16_t* a,
    const complex_s16_t value,
    const exponent_t exp)
{
    a->exp = exp;
    a->hr = HR_C16(value);
    a->flags = 0;

    xs3_vect_s16_set( a->real, value.re, a->length );
    xs3_vect_s16_set( a->imag, value.im, a->length );
}



void bfp_complex_s32_set(
    bfp_complex_s32_t* a,
    const complex_s32_t value,
    const exponent_t exp)
{
    a->exp = exp;
    a->hr = HR_C32(value);
    a->flags = 0;

    xs3_vect_complex_s32_set( a->data, value.re, value.im, a->length);
}



void bfp_ch_pair_s16_set(
    bfp_ch_pair_s16_t* a,
    const ch_pair_s16_t value,
    const exponent_t exp)
{
    a->exp = exp;
    a->hr = HR_C16(*((complex_s16_t*)&value));
    a->flags = 0;

    const int32_t x = value.ch_b << 16;
    const int32_t y = value.ch_a & 0xFFFF;
    const int32_t v = (x|y);
    xs3_vect_s32_set( (int32_t*) a->data, v, a->length);
}



void bfp_ch_pair_s32_set(
    bfp_ch_pair_s32_t* a,
    const ch_pair_s32_t value,
    const exponent_t exp)
{
    a->exp = exp;
    a->hr = HR_C32(*((complex_s32_t*)&value));
    a->flags = 0;

    xs3_vect_complex_s32_set( (complex_s32_t*) a->data, value.ch_a, value.ch_b, a->length);
}
