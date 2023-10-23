// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>

#include "xmath/xmath.h"
#include "vpu_helper.h"




// 
// complex_s32_t vect_complex_s16_sum(
//     const int16_t b_real[],
//     const int16_t b_imag[],
//     const unsigned length)
// {
//     complex_s32_t res = {
//         vect_s16_sum(b_real, length),
//         vect_s16_sum(b_imag, length) 
//     };

//     return res;
// }



void vect_complex_s32_sum(
    complex_s64_t* res,
    const complex_s32_t b[],
    const unsigned length,
    const right_shift_t b_shr)
{
    int64_t sum_real = 0;
    int64_t sum_imag = 0;

    for(unsigned k = 0; k < length; k++){
        
        complex_s32_t B = {
            ROUND_SHR(b[k].re, b_shr), 
            ROUND_SHR(b[k].im, b_shr),
        };

        sum_real += B.re;
        sum_imag += B.im;
    }

    res->re = sum_real;
    res->im = sum_imag;
}
