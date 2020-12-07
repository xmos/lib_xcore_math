
#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "../../../vect/vpu_helper.h"




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
        
        complex_s32_t B = {
            ASHR(16)(b_real[k], b_shr), 
            ASHR(16)(b_imag[k], b_shr)
        };

        B.re = (B.re >= 0)? B.re : -B.re;
        B.im = (B.im >= 0)? B.im : -B.im;

        for(int iter = 0; iter < table_rows; iter++){

            complex_s32_t rot = {
                rot_table[32 * iter],
                rot_table[32 * iter + 16 ],
            };

            int32_t q1 = B.re * rot.re;
            int32_t q2 =-B.im * rot.im;
            int32_t q3 = B.re * rot.im;
            int32_t q4 = B.im * rot.re;

            B.re = SAT(16)(ROUND_SHR(q1 + q2, 15));
            B.im = SAT(16)(ROUND_SHR(q3 + q4, 15));

            B.re = (B.re >= 0)? B.re : -B.re;
            B.im = (B.im >= 0)? B.im : -B.im;

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
        
        complex_s32_t B = {
            ASHR(32)(b[k].re, b_shr), 
            ASHR(32)(b[k].im, b_shr),
        };

        B.re = (B.re >= 0)? B.re : -B.re;
        B.im = (B.im >= 0)? B.im : -B.im;

        for(int iter = 0; iter < table_rows; iter++){

            complex_s32_t rot = {
                rot_table[iter * 4].re,
                rot_table[iter * 4].im
            };

            // printf("%d:\t%ld + i*%ld\n", iter, rot.re, rot.im);

            int64_t q1 = ROUND_SHR( ((int64_t)B.re) * rot.re, 30 );
            int64_t q2 = ROUND_SHR( ((int64_t)B.im) * rot.im, 30 );
            int64_t q3 = ROUND_SHR( ((int64_t)B.re) * rot.im, 30 );
            int64_t q4 = ROUND_SHR( ((int64_t)B.im) * rot.re, 30 );

            B.re = SAT(32)(q1 - q2);
            B.im = SAT(32)(q3 + q4);

            B.re = (B.re >= 0)? B.re : -B.re;
            B.im = (B.im >= 0)? B.im : -B.im;

        }

        a[k] = B.re;

    }

    return xs3_vect_s32_headroom( (int32_t*) a, length);
}


/////////////////////////////////////
/////  Squared Magnitude        /////
/////////////////////////////////////


headroom_t xs3_vect_complex_s16_squared_mag(
    int16_t a[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const unsigned length,
    const right_shift_t sat)
{
    for(int k = 0; k < length; k++){
        
        complex_s32_t B = { b_real[k], b_imag[k] };

        int32_t acc = B.re*B.re + B.im*B.im;
        a[k] = SAT(16)(ROUND_SHR(acc, sat));
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
            ASHR(32)(b[k].re, b_shr), 
            ASHR(32)(b[k].im, b_shr),
        };

        B.re = SAT(32)(ROUND_SHR(((int64_t)B.re)*B.re, 30));
        B.im = SAT(32)(ROUND_SHR(((int64_t)B.im)*B.im, 30));

        a[k] = B.re + B.im;
    }

    return xs3_vect_s32_headroom(a, length);
}