
#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "../vpu_helper.h"



WEAK_FUNC
headroom_t xs3_complex_mul_vect_complex_s16(
    int16_t* a_real,
    int16_t* a_imag,
    const int16_t* b_real,
    const int16_t* b_imag,
    const int16_t* c_real,
    const int16_t* c_imag,
    const unsigned length,
    const right_shift_t sat)
{
    for(int k = 0; k < length; k++){
        
        complex_s32_t B = {b_real[k], b_imag[k]};
        complex_s32_t C = {c_real[k], c_imag[k]};

        struct{ int64_t re; int64_t im; } P = {
            ((int64_t)B.re * C.re) - B.im * C.im,
            ((int64_t)B.re * C.im) + B.im * C.re,
        };

        P.re = ROUND_SHR(P.re, sat);
        P.im = ROUND_SHR(P.im, sat);

        a_real[k] = SAT(16)(P.re);
        a_imag[k] = SAT(16)(P.im);
    }

    headroom_t hr_real = xs3_headroom_vect_s16(a_real, length);
    headroom_t hr_imag = xs3_headroom_vect_s16(a_imag, length);

    return (hr_real <= hr_imag)? hr_real : hr_imag;
}


WEAK_FUNC
headroom_t xs3_complex_mul_vect_complex_s32(
    complex_s32_t* a,
    const complex_s32_t* b,
    const complex_s32_t* c,
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr)
{
    for(int k = 0; k < length; k++){
        
        complex_s32_t B = {
            ASHR(32)(b[k].re, b_shr), 
            ASHR(32)(b[k].im, b_shr),
        };
        complex_s32_t C = {
            ASHR(32)(c[k].re, c_shr), 
            ASHR(32)(c[k].im, c_shr),
        };

        int64_t q1 = ROUND_SHR( ((int64_t)B.re) * C.re, 30 );
        int64_t q2 = ROUND_SHR( ((int64_t)B.im) * C.im, 30 );
        int64_t q3 = ROUND_SHR( ((int64_t)B.re) * C.im, 30 );
        int64_t q4 = ROUND_SHR( ((int64_t)B.im) * C.re, 30 );

        a[k].re = SAT(32)(q1 - q2);
        a[k].im = SAT(32)(q3 + q4);
    }

    return xs3_headroom_vect_s32( (int32_t*) a, 2*length);
}