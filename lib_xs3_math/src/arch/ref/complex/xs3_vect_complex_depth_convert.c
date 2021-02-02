// Copyright 2021 XMOS LIMITED. This Software is subject to the terms of the 
// XMOS Public License: Version 1

#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "../../../vect/vpu_helper.h"




void xs3_vect_complex_s16_to_complex_s32(
    complex_s32_t a[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const unsigned length)
{
    for(int k = 0; k < length; k++){
        
        complex_s32_t B = {
            ((int32_t)b_real[k]),
            ((int32_t)b_imag[k]),
        };

        a[k] = B;
    }
}



void xs3_vect_complex_s32_to_complex_s16(
    int16_t a_real[],
    int16_t a_imag[],
    const complex_s32_t b[],
    const unsigned length,
    const right_shift_t b_shr)
{
    const right_shift_t shr_mod = b_shr - 16;

    for(int k = 0; k < length; k++){
        a_real[k] = (int16_t) SAT(16)(ROUND_SHR(ASHR(32)(b[k].re, shr_mod), 16));
        a_imag[k] = (int16_t) SAT(16)(ROUND_SHR(ASHR(32)(b[k].im, shr_mod), 16));
    }
}