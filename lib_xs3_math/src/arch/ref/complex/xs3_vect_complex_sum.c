
#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "../../../vect/vpu_helper.h"




// 
// complex_s32_t xs3_sum_complex_s16(
//     const int16_t b_real[],
//     const int16_t b_imag[],
//     const unsigned length)
// {
//     complex_s32_t res = {
//         xs3_vect_s16_sum(b_real, length),
//         xs3_vect_s16_sum(b_imag, length) 
//     };

//     return res;
// }



void xs3_sum_complex_s32(
    complex_s64_t* res,
    const complex_s32_t b[],
    const unsigned length,
    const right_shift_t b_shr)
{
    int64_t sum_real = 0;
    int64_t sum_imag = 0;

    for(int k = 0; k < length; k++){
        
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