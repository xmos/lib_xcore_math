// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include "floating_fft.h"

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


void flt_bit_reverse_indexes_double(
    complex_double_t a[],
    const unsigned length)
{
    size_t logn = u32_ceil_log2(length);
    for(unsigned int i = 0; i < length; i++){
        unsigned rev = flt_bitrev(i, logn);
        if(rev < i) continue;

        complex_double_t tmp = a[i];
        
        a[i] = a[rev];
        a[rev] = tmp;
    }
}


void flt_make_sine_table_double(
    double sine_lut[],
    const unsigned N)
{
    for(unsigned i = 0; i < (N/4) + 1; i++){
        sine_lut[i] = (double) sin( 2.0 * M_PI * (double)i / (double)N);
    }
}


void flt_fft_forward_double(
    complex_double_t pts[],
    const unsigned N,
    const double sine_table[])
{
    uint32_t shift = 30 - cls(N);

    for(uint32_t step = 2; step <= N; step = 2*step, shift--) {
        uint32_t step2 = step >> 1;
        uint32_t step4 = step2 >> 1;
        uint32_t k;

        for(k = 0; k < (step4 + (step2 & 0x1)); k++)
        {
            double rRe = sine_table[(N>>2)-(k<<shift)];
            double rIm = sine_table[k<<shift];
            for(int32_t block = k+N-step; block >= 0; block-=step) {
                double tRe = pts[block].re;
                double tIm = pts[block].im;
                double tRe2 = pts[block + step2].re;
                double tIm2 = pts[block + step2].im;

                double sRe2 = (tRe2*rRe) + (tIm2*rIm);
                double sIm2 =  (tIm2*rRe) - (tRe2*rIm);

                pts[block].re = tRe + sRe2;
                pts[block].im = tIm + sIm2;
                pts[block+step2].re = tRe - sRe2;
                pts[block+step2].im = tIm - sIm2;
            }
        }

        for(k = (step2 & 1); k < step4; k++)
        {
            double rRe = -sine_table[k<<shift];
            double rIm = sine_table[(N>>2)-(k<<shift)];
            for(int32_t block = k+step4+N-step; block >= 0; block-=step) {
                double tRe = pts[block].re;
                double tIm = pts[block].im;
                double tRe2 = pts[block + step2].re;
                double tIm2 = pts[block + step2].im;

                double sRe2 = (tRe2 * rRe) + (tIm2 * rIm);
                double sIm2 = (tIm2 * rRe) - (tRe2 * rIm);

                pts[block].re = tRe + sRe2;
                pts[block].im = tIm + sIm2;
                pts[block+step2].re = tRe - sRe2;
                pts[block+step2].im = tIm - sIm2;
            }
        }
    }
}


void flt_fft_inverse_double (
    complex_double_t pts[],
    const unsigned N,
    const double sine_table[] )
{
    uint32_t shift = 30-cls(N);

    for(uint32_t step = 2; step <= N; step = 2*step, shift--) {
        uint32_t step2 = step >> 1;
        uint32_t step4 = step2 >> 1;
        uint32_t k;

        for(k = 0; k < (step4 + (step2 & 0x1)); k++) {
            double rRe = sine_table[(N>>2)-(k<<shift)];
            double rIm = sine_table[k<<shift];

            for(unsigned block = k; block < k+N; block+=step) {
                double tRe = pts[block].re;
                double tIm = pts[block].im;
                double tRe2 = pts[block + step2].re;
                double tIm2 = pts[block + step2].im;

                double sRe2 = (tRe2*rRe)-(tIm2*rIm);
                double sIm2 = (tRe2*rIm)+(tIm2*rRe);

                pts[block].re = tRe + sRe2;
                pts[block].im = tIm + sIm2;
                pts[block+step2].re = tRe - sRe2;
                pts[block+step2].im = tIm - sIm2;
            }
        }

        for(k=(step2 & 1); k < (step2-step4); k++) {
            double rRe = -sine_table[k<<shift];
            double rIm =  sine_table[(N>>2)-(k<<shift)];

            for(unsigned block = k+step4; block < k+step4+N; block+=step) {
                double tRe = pts[block].re;
                double tIm = pts[block].im;
                double tRe2 = pts[block + step2].re;
                double tIm2 = pts[block + step2].im;

                double sRe2 = (tRe2*rRe)-(tIm2*rIm);
                double sIm2 = (tRe2*rIm)+(tIm2*rRe);

                pts[block].re = tRe + sRe2;
                pts[block].im = tIm + sIm2;
                pts[block+step2].re = tRe - sRe2;
                pts[block+step2].im = tIm - sIm2;
            }
        }
    }

    unsigned log2_N = 31-cls(N);

    for(unsigned i=0;i<N;i++){
        pts[i].re = ldexp(pts[i].re, (int)-(int)log2_N);
        pts[i].im = ldexp(pts[i].im, (int)-(int)log2_N);
    }
}


void flt_fft_split_spectrum_double(
    complex_double_t pts[],
    const unsigned N)
{

    //pts[0:N] is the combined spectrum X[f]
    //output pts[0:N/2] is A[f]
    //output pts[N/2:N] is B[f]

    for(uint32_t i = 1; i < N/2; i++){

        double a_re = ldexp(( pts[i].re + pts[N-i].re), -1);
        double a_im = ldexp(( pts[i].im - pts[N-i].im), -1);
        double b_re = ldexp(( pts[i].im + pts[N-i].im), -1);
        double b_im = ldexp((-pts[i].re + pts[N-i].re), -1);

        //A[i].re   = ( X[i].re + X[N-i].im ) / 2
        pts[  i].re = a_re;

        //A[i].im   = ( X[i].im - X[N-i].im ) / 2
        pts[  i].im = a_im;

        // pts[N-i] = Z[N/2 - i]

        // Z[N/2 - i].re = ( X[i].im + X[N-i].im ) / 2
        pts[N-i].re = b_re;

        // Z[N/2 - i].im = (-X[i].re + X[N-i].re ) / 2
        pts[N-i].im = b_im;
    }

    double re = pts[N/2].re;
    double im = pts[N/2].im;

    pts[N/2].re = pts[0].im;
    pts[N/2].im = im;
    pts[  0].im = re;

    // B[1:N/2] = Z[N/2:1:-1]
    for(uint32_t i = 1; i < N/4; i++){
        complex_double_t a = pts[N/2 + i];
        complex_double_t b = pts[N - i];

        pts[N/2 + i] = b;
        pts[N - i]  = a;
    }
}


void flt_fft_merge_spectra_double(
    complex_double_t pts[],
    const unsigned N)
{
    //pts[0:N/2] is a spectrum A[f] of a real signal a[n]
    //pts[N/2:N] is a spectrum B[f] of a real signal b[n]
    //output spectrum X[f]  is pts[0:N]

    //Reverses the order of pts[N/2+1:N]
    // pts[N/2+k] = pts[N-k]  -->  B[k] = B[N/2 - k]
    for(uint32_t i=1; i < (N/4); i++){
        complex_double_t a = pts[N/2 + i];
        complex_double_t b = pts[  N - i];
        pts[N/2 + i] = b;
        pts[  N - i] = a;
    }

    //X[N/2].re = A[0].im  (the Nyquist rate for A[])
    //X[0].im = B[0].re (the DC value for B[])
    double t    = pts[  0].im;
    pts[  0].im = pts[N/2].re;
    pts[N/2].re = t;
    //Note that X[0].re and X[N/2].im are already in their correct places.

    for(uint32_t i = 1; i< (N/2); i++)
    {
        //Because of the reversal above:
        // pts[N-i] = pts[N/2 + i] = B[i]

        double a_re = pts[  i].re;
        double a_im = pts[  i].im;
        double b_re = pts[N-i].re;
        double b_im = pts[N-i].im;

        //X[i].re = A[i].re - B[i].im
        pts[  i].re = a_re - b_im;
        //X[i].im = A[i].im + B[i].re
        pts[  i].im = a_im + b_re;
        //X[N-i].re = A[i].re + B[i].im
        pts[N-i].re = b_im + a_re;
        //X[N-i].re = -A[i].im + B[i].re
        pts[N-i].im = b_re - a_im;
    }
}
