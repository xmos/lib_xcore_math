
#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "../vpu_helper.h"


////////////////////////////////////////
//      16-Bit Multiplication         //
////////////////////////////////////////

// complex vector multiplied by real vector
WEAK_FUNC
headroom_t xs3_vect_complex_s16_mul(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const int16_t c[],
    const unsigned length,
    const right_shift_t sat)
{
    for(int k = 0; k < length; k++){
        
        complex_s32_t B = {b_real[k], b_imag[k]};
        int32_t C = c[k];

        struct{ int64_t re; int64_t im; } P = {
            ((int64_t)B.re * C),
            ((int64_t)B.im * C),
        };

        P.re = ROUND_SHR(P.re, sat);
        P.im = ROUND_SHR(P.im, sat);

        a_real[k] = SAT(16)(P.re);
        a_imag[k] = SAT(16)(P.im);
    }

    return xs3_vect_complex_s16_headroom(a_real, a_imag, length);
}


// complex vector multiplied by complex vector
WEAK_FUNC
headroom_t xs3_vect_complex_s16_complex_mul(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const int16_t c_real[],
    const int16_t c_imag[],
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

    return xs3_vect_complex_s16_headroom(a_real, a_imag, length);
}


// complex vector (conjugate) multiplied by complex vector
WEAK_FUNC
headroom_t xs3_vect_complex_s16_complex_conj_mul(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const int16_t c_real[],
    const int16_t c_imag[],
    const unsigned length,
    const right_shift_t sat)
{
    for(int k = 0; k < length; k++){
        
        complex_s32_t B = {b_real[k], b_imag[k]};
        complex_s32_t C = {c_real[k], c_imag[k]};

        struct{ int64_t re; int64_t im; } P = {
            ((int64_t)B.re * C.re) + B.im * C.im,
            ((int64_t)B.im * C.re) - B.re * C.im,
        };

        P.re = ROUND_SHR(P.re, sat);
        P.im = ROUND_SHR(P.im, sat);

        a_real[k] = SAT(16)(P.re);
        a_imag[k] = SAT(16)(P.im);
    }

    return xs3_vect_complex_s16_headroom(a_real, a_imag, length);
}


// complex vector multiplied by complex vector
WEAK_FUNC
headroom_t xs3_vect_complex_s16_complex_scal_mul(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const int16_t c_real,
    const int16_t c_imag,
    const unsigned length,
    const right_shift_t sat)
{

    for(int k = 0; k < length; k++){
        
        complex_s32_t B = {b_real[k], b_imag[k]};

        struct{ int64_t re; int64_t im; } P = {
            ((int64_t)B.re * c_real) - B.im * c_imag,
            ((int64_t)B.re * c_imag) + B.im * c_real,
        };

        P.re = ROUND_SHR(P.re, sat);
        P.im = ROUND_SHR(P.im, sat);

        a_real[k] = SAT(16)(P.re);
        a_imag[k] = SAT(16)(P.im);
    }

    return xs3_vect_complex_s16_headroom(a_real, a_imag, length);
}







////////////////////////////////////////
//      32-Bit Multiplication         //
////////////////////////////////////////


// complex vector multiplied by real vector
WEAK_FUNC
headroom_t xs3_vect_complex_s32_mul(
    complex_s32_t a[],
    const complex_s32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr)
{
    for(int k = 0; k < length; k++){
        
        complex_s32_t B = {
            ASHR(32)(b[k].re, b_shr), 
            ASHR(32)(b[k].im, b_shr),
        };

        int32_t C = ASHR(32)(c[k], c_shr);

        a[k].re = SAT(32)(ROUND_SHR(((int64_t)B.re) * C, 30));
        a[k].im = SAT(32)(ROUND_SHR(((int64_t)B.im) * C, 30));
    }


    return xs3_vect_complex_s32_headroom(a, length);
}


// complex vector multiplied by complex vector
WEAK_FUNC
headroom_t xs3_vect_complex_s32_complex_mul(
    complex_s32_t a[],
    const complex_s32_t b[],
    const complex_s32_t c[],
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

    return xs3_vect_complex_s32_headroom(a, length);
}


// complex vector (conjugate) multiplied by complex vector
WEAK_FUNC
headroom_t xs3_vect_complex_s32_complex_conj_mul(
    complex_s32_t a[],
    const complex_s32_t b[],
    const complex_s32_t c[],
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

        a[k].re = SAT(32)(q1 + q2);
        a[k].im = SAT(32)(q4 - q3);
    }

    return xs3_vect_complex_s32_headroom(a, length);
}



// complex vector multiplied by complex scalar
WEAK_FUNC
headroom_t xs3_vect_complex_s32_complex_scal_mul(
    complex_s32_t a[],
    const complex_s32_t b[],
    const int32_t c_real,
    const int32_t c_imag,
    const unsigned length,
    const right_shift_t b_shr)
{
    for(int k = 0; k < length; k++){
        
        const complex_s32_t B = {
            ASHR(32)(b[k].re, b_shr), 
            ASHR(32)(b[k].im, b_shr),
        };
        const int64_t q1 = ROUND_SHR( ((int64_t)B.re) * c_real, 30 );
        const int64_t q2 = ROUND_SHR( ((int64_t)B.im) * c_imag, 30 );
        const int64_t q3 = ROUND_SHR( ((int64_t)B.re) * c_imag, 30 );
        const int64_t q4 = ROUND_SHR( ((int64_t)B.im) * c_real, 30 );

        a[k].re = SAT(32)(q1 - q2);
        a[k].im = SAT(32)(q3 + q4);
    }

    return xs3_vect_complex_s32_headroom(a, length);
}


