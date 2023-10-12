// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdio.h>

#include "xmath/xmath.h"
#include "vpu_helper.h"
#include "vpu_const_vects.h"
#include "xmath_fft_lut.h"

#include "xmath/xs3/vpu_scalar_ops.h"

// Disable warning messages C4293:
// warning C4293: '<<': shift count negative or too big, undefined behavior
#ifdef _WIN32
#pragma warning( disable : 4293)
#endif


//load 4 complex 32-bit values into a buffer
static void load_vec(
    complex_s32_t dst[],
    const complex_s32_t src[])
{
    for(int i = 0; i < 4; i++){
        dst[i] = src[i];
    }
}



void fft_index_bit_reversal(
    complex_s32_t* a,
    const unsigned length)
{
    unsigned int logn = u32_ceil_log2(length);
    for(unsigned i = 0; i < length; i++){
        
        unsigned rev = n_bitrev(i, logn);
        if(rev < i) continue;

        complex_s32_t tmp = a[i];
        
        a[i] = a[rev];
        a[rev] = tmp;
    }
}



headroom_t fft_spectra_split(
    complex_s32_t* X,
    const unsigned N)
{
    const unsigned K = N/2;

    //First, reverse order of X[N/2+1:N]
    for(unsigned f = 1; f < N/4; f++){
        const complex_s32_t tmp = X[K+f];
        X[K+f] = X[N-f];
        X[N-f] = tmp;
    }

    //For spectrum of real signal, the DC and Nyquist bins must both be purely real. We will stuff
    // the Nyquist rate value into the imaginary part of the DC bin.
    complex_s32_t X0 = X[0];
    complex_s32_t XN = X[K];
    
    // If we change [X[0].re, X[0].im, X[K].re, X[K].im] to be this:
    //  [DC.re - Ny.im,  Ny.re + DC.im,   DC.re + Ny.im,  -Ny.re + DC.im  ]
    // then we can just compute those bins in the loop below. Not very important
    // here, but makes the assembly much faster.
    // This does unfortunately introduce the requirement that the input have at least 1 bit of headroom.
    X[0].re =  X0.re - XN.im;
    X[0].im =  X0.im + XN.re;
    X[K].re =  X0.re + XN.im;
    X[K].im =  X0.im - XN.re;

    // X[0].re = X0.re;
    // X[0].im = XN.re;
    // X[K].re = X0.im;
    // X[K].im = XN.im;


    //Now split the spectrum
    for(unsigned f = 0; f < K; f++){
        //A[f] = ((X[f].re + X[-f].re) + j*(X[f].im - X[-f].im)) / 2
        //B[f] = ((X[f].im + X[-f].im) - j*(X[f].re - X[-f].re)) / 2
        //X[-f] = X[-f+N] because X[f] is periodic with period N

        complex_s32_t Xp = { X[0+f].re>>1, X[0+f].im>>1 };
        complex_s32_t Xn = { X[K+f].re>>1, X[K+f].im>>1 };

        X[0+f].re =  Xp.re + Xn.re;
        X[0+f].im =  Xp.im - Xn.im;
        X[K+f].re =  Xp.im + Xn.im;
        X[K+f].im = -Xp.re + Xn.re;

        // Really hard to add/sub real parts from imaginary parts using the VPU because those
        // aren't aligned together nicely in memory. But we can do some complex arithmetic...

        //X[f]   = Xp.re + Xn.re + j*(Xp.im - Xn.im)
        //       = conj(Xn) + Xp

        //X[K+f] = Xp.im + Xn.im + j*(-Xp.re + Xn.re)
        //       = Xp.im + Xn.im - j*Xp.re + j*Xn.re
        //       = -j*Xp.re+Xp.im + j*Xn.re + Xn.im
        //       = -j*Xp + j*Xn.re + -j*j*Xn.im
        //       = -j*Xp + j*(Xn.re - j*Xn.im)
        //       = -j*Xp + j*conj(Xn)
        //       = j*(conj(Xn) - Xp)

        // Note:  j*conj(z) = -conj(j*z)
    }

    return vect_s32_headroom((int32_t*)X, 2*N);
}


headroom_t fft_spectra_merge(
    complex_s32_t* X,
    const unsigned N)
{
    const unsigned K = N/2;

    {
        //Pre-boggle DC and Nyquist
        complex_s32_t DC = { X[0].re>>1, X[0].im>>1 };
        complex_s32_t Ny = { X[K].re>>1, X[K].im>>1 };

        X[0].re =  DC.re + DC.im;
        X[0].im =  Ny.re - Ny.im;
        X[K].re =  Ny.re + Ny.im;
        X[K].im = -DC.re + DC.im;
    }

    for(unsigned f = 0; f < N/2; f++){
        complex_s32_t a = X[0+f];
        complex_s32_t b = X[K+f];

        X[0+f].re = a.re - b.im;
        X[0+f].im = a.im + b.re;
        X[K+f].re = b.im + a.re;
        X[K+f].im = b.re - a.im;

        //X[f]   = (a.re-b.im) + j*(a.im+b.re)
        //       = a + j*b.re - b.im
        //       = a + j*b

        //X[K+f] = (a.re+b.im) - j*(a.im+b.re)
        //       = a.re - j*a.im + j*b.re + b.im
        //       = conj(a) + j*conj(b)
        //       = conj(a) - conj(j*b)
        //       = conj(a - j*b)


    }

    for(unsigned f = 1; f < N/4; f++){
        complex_s32_t tmp = X[K+f];
        X[K+f] = X[N-f];
        X[N-f] = tmp;
    }

    return vect_s32_headroom((int32_t*)X, 2*N);
}


void fft_mono_adjust(
    complex_s32_t x[],
    const unsigned FFT_N,
    const unsigned inverse)
{
    // Assembly only supports FFT_N >= 16
    assert(FFT_N >= 16);

    #define VEC_ELMS 4 //complex elements per vector

    const complex_s32_t* W = XMATH_DIT_REAL_FFT_LUT(FFT_N);
    
    // REMEMBER: The length of x[] is only FFT_N/2!
    complex_s32_t X0 = x[0];
    complex_s32_t XQ = x[FFT_N/4];

    vect_complex_s32_tail_reverse(&x[FFT_N/4], FFT_N/4);

    complex_s32_t* p_X_lo = &x[0];
    complex_s32_t* p_X_hi = &x[FFT_N/4];

    if(inverse){
        complex_s32_t* tmp = p_X_hi;
        p_X_hi = p_X_lo;
        p_X_lo = tmp;
    }

    for(unsigned k = 0; k < (FFT_N/4); k+=4){

        complex_s32_t X_lo[VEC_ELMS], X_hi[VEC_ELMS], tmp[VEC_ELMS], A[VEC_ELMS], B[VEC_ELMS];

        load_vec(X_lo, p_X_lo);
        load_vec(X_hi, p_X_hi);

        // tmp = W
        load_vec(tmp, W);

        // tmp = j*W
        vect_complex_s32_mul(tmp, tmp, vpu_vec_complex_pos_j, VEC_ELMS, 0, 0);


        // A = 0.5*(1 - j*W)
        // B = 0.5*(1 + j*W)

        // (Shifting each right by 1 gives the *0.5)
        
        // for(int i = 0; i < 4; i++){
        //     A[i].re = ASHR(32)(((int64_t) 0x40000000) - tmp[i].re, 1);
        //     A[i].im = ASHR(32)(((int64_t) 0x00000000) - tmp[i].im, 1);
        //     B[i].re = ASHR(32)(((int64_t) 0x40000000) + tmp[i].re, 1);
        //     B[i].im = ASHR(32)(((int64_t) 0x00000000) + tmp[i].im, 1);
        // }
        vect_complex_s32_sub(A, vpu_vec_complex_ones, tmp, VEC_ELMS, 1, 1);
        vect_complex_s32_add(B, vpu_vec_complex_ones, tmp, VEC_ELMS, 1, 1);


        // new_X_lo = A*X_lo + B*conjugate(X_hi)
        vect_complex_s32_mul(p_X_lo, A, X_lo, VEC_ELMS, 0, 0);
        vect_complex_s32_conj_mul(tmp, B, X_hi, VEC_ELMS, 0, 0);
        vect_complex_s32_add(p_X_lo, p_X_lo, tmp, VEC_ELMS, 0, 0);

        // new_X_hi = conjugate(A)*X_hi + conjugate(B)*conjugate(X_lo)
        vect_complex_s32_conj_mul(p_X_hi, X_hi, A, VEC_ELMS, 0, 0);
        vect_s32_mul((int32_t*)B,(int32_t*)B,(int32_t*)vpu_vec_complex_conj_op, 2*VEC_ELMS, 0, 0);
        vect_complex_s32_conj_mul(tmp, B, X_lo, VEC_ELMS, 0, 0);
        vect_complex_s32_add(p_X_hi, p_X_hi, tmp, VEC_ELMS, 0, 0);

        W = &W[-VEC_ELMS];
        p_X_lo = &p_X_lo[VEC_ELMS];
        p_X_hi = &p_X_hi[VEC_ELMS];
    }

    if(inverse){
        X0.re = ASHR(32)(X0.re, 1);
        X0.im = ASHR(32)(X0.im, 1);
    }

    //Fix DC and Nyquist
    x[0].re = X0.re + X0.im;
    x[0].im = X0.re - X0.im;
    x[FFT_N/4].re =  XQ.re;
    x[FFT_N/4].im = -XQ.im;
    
    vect_complex_s32_tail_reverse(&x[FFT_N/4], FFT_N/4);
}



void vect_complex_s32_tail_reverse(
    complex_s32_t x[],
    const unsigned N)
{
    for(unsigned i = 1; i < N/2; i++){
        int k = N-i;

        complex_s32_t tmp = x[i];
        x[i] = x[k];
        x[k] = tmp;
    }
}

