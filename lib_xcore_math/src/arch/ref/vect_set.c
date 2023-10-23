// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>

#include "xmath/xmath.h"
#include "vpu_helper.h"


void vect_s16_set(
    int16_t data[],
    const int16_t value,
    const unsigned length)
{
    for(unsigned i = 0; i < length; i++)
        data[i] = value;
}



void vect_s32_set(
    int32_t data[],
    const int32_t value,
    const unsigned length)
{
    for(unsigned i = 0; i < length; i++)
        data[i] = value;
}



void vect_complex_s32_set(
    complex_s32_t data[],
    const int32_t real_part,
    const int32_t imag_part,
    const unsigned length)
{
    for(unsigned i = 0; i < length; i++){
        data[i].re = real_part;
        data[i].im = imag_part;
    }
}

