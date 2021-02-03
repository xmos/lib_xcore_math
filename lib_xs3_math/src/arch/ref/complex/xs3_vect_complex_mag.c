// Copyright 2020 XMOS LIMITED. This Software is subject to the terms of the 
// XMOS Public License: Version 1

#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "../../../vect/vpu_helper.h"

#include "xs3_vpu_scalar_ops.h"
#include "../../../vect/vpu_const_vects.h"


#define negative_one_s16    (vpu_vec_neg_0x4000[0])


headroom_t xs3_vect_complex_s16_mag(
    int16_t a[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const unsigned length,
    const right_shift_t b_shr,
    const int16_t* rot_table,
    const unsigned table_rows)
{
    for(int k = 0; k < length; k++){
        
        complex_s16_t B = {
            vlashr16(b_real[k], b_shr),
            vlashr16(b_imag[k], b_shr),
        };

        B.re = vlmul16(vsign16(B.re), B.re);
        B.im = vlmul16(vsign16(B.im), B.im);

        for(int iter = 0; iter < table_rows; iter++){
            vpu_int16_acc_t acc;

            complex_s16_t rot = {
                rot_table[32 * iter],
                rot_table[32 * iter + 16 ],
            };

            complex_s16_t new_B;

            acc = 0;

            acc = vlmacc16(acc, rot.im, vlmul16( negative_one_s16 , B.im));
            acc = vlmacc16(acc, rot.re, B.re);
            
            new_B.re = vlsat16(acc, 15);

            acc = 0;

            acc = vlmacc16(acc, rot.im, B.re);
            acc = vlmacc16(acc, rot.re, B.im);

            new_B.im = vlsat16(acc, 15);

            B.re = vlmul16(vsign16(new_B.re), new_B.re);
            B.im = vlmul16(vsign16(new_B.im), new_B.im);

        }

        a[k] = B.re;
    }


    return xs3_vect_s16_headroom(a, length);
}



headroom_t xs3_vect_complex_s32_mag(
    int32_t a[],
    const complex_s32_t b[],
    const unsigned length,
    const right_shift_t b_shr,
    const complex_s32_t* rot_table,
    const unsigned table_rows)
{

    for(int k = 0; k < length; k++){
        
        // Apply a right-shift to b[k]
        complex_s32_t B = {
            vlashr32(b[k].re, b_shr), 
            vlashr32(b[k].im, b_shr),
        };

        // Reflect B into the first quadrant
        B.re = vlmul32(vsign32(B.re), B.re);
        B.im = vlmul32(vsign32(B.im), B.im);

        for(int iter = 0; iter < table_rows; iter++){

            complex_s32_t rot = {
                rot_table[iter * 4].re,
                rot_table[iter * 4].im
            };

            complex_s32_t new_B = {
                vcmr32(B, rot),
                vcmi32(B, rot),
            };

            B.re = vlmul32(vsign32(new_B.re), new_B.re);
            B.im = vlmul32(vsign32(new_B.im), new_B.im);
        }

        a[k] = B.re;

    }

    return xs3_vect_s32_headroom( a, length);
}


/////////////////////////////////////
/////  Squared Magnitude        /////
/////////////////////////////////////


headroom_t xs3_vect_complex_s16_squared_mag(
    int16_t a[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const unsigned length,
    const right_shift_t a_shr)
{
    for(int k = 0; k < length; k++){

        vpu_int16_acc_t acc = 0;

        acc = vlmacc16(acc, b_real[k], b_real[k]);
        acc = vlmacc16(acc, b_imag[k], b_imag[k]);
        
        a[k] = vlsat16(acc, a_shr);
    }

    return xs3_vect_s16_headroom(a, length);
}



headroom_t xs3_vect_complex_s32_squared_mag(
    int32_t a[],
    const complex_s32_t b[],
    const unsigned length,
    const right_shift_t b_shr)
{

    for(int k = 0; k < length; k++){

        complex_s32_t B = {
            vlashr32(b[k].re, b_shr),
            vlashr32(b[k].im, b_shr),
        };

        a[k] = vcmcr32(B, B);
    }

    return xs3_vect_s32_headroom(a, length);
}