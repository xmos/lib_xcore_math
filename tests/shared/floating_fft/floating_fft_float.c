// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include "floating_fft.h"

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


void flt_bit_reverse_indexes_float(
    complex_float_t a[],
    const unsigned length)
{
    size_t logn = u32_ceil_log2(length);
    for(unsigned int i = 0; i < length; i++){
        unsigned rev = flt_bitrev(i, logn);
        if(rev < i) continue;

        complex_float_t tmp = a[i];
        
        a[i] = a[rev];
        a[rev] = tmp;
    }
}

void flt_make_sine_table_float(
    float sine_lut[],
    const unsigned N)
{
    for(unsigned i = 0; i < (N/4) + 1; i++){
        sine_lut[i] = (float) sin( 2.0 * M_PI * (double)i / (double)N);
    }
}


void flt_fft_forward_float(
    complex_float_t pts[],
    const unsigned N,
    const float sine_table[])
{
    uint32_t shift = 30 - cls(N);

    for(uint32_t step = 2; step <= N; step = 2*step, shift--) {
        uint32_t step2 = step >> 1;
        uint32_t step4 = step2 >> 1;
        uint32_t k;

        for(k = 0; k < (step4 + (step2 & 0x1)); k++)
        {
            float rRe = sine_table[(N>>2)-(k<<shift)];
            float rIm = sine_table[k<<shift];
            for(int32_t block = k+N-step; block >= 0; block-=step) {
                float tRe = pts[block].re;
                float tIm = pts[block].im;
                float tRe2 = pts[block + step2].re;
                float tIm2 = pts[block + step2].im;

                float sRe2 = (tRe2*rRe) + (tIm2*rIm);
                float sIm2 =  (tIm2*rRe) - (tRe2*rIm);

                pts[block].re = tRe + sRe2;
                pts[block].im = tIm + sIm2;
                pts[block+step2].re = tRe - sRe2;
                pts[block+step2].im = tIm - sIm2;
            }
        }

        for(k = (step2 & 1); k < step4; k++)
        {
            float rRe = -sine_table[k<<shift];
            float rIm =  sine_table[(N>>2)-(k<<shift)];
            for(int32_t block = k+step4+N-step; block >= 0; block-=step) {
                float tRe = pts[block].re;
                float tIm = pts[block].im;
                float tRe2 = pts[block + step2].re;
                float tIm2 = pts[block + step2].im;

                float sRe2 = (tRe2 * rRe) + (tIm2 * rIm);
                float sIm2 = (tIm2 * rRe) - (tRe2 * rIm);

                pts[block].re = tRe + sRe2;
                pts[block].im = tIm + sIm2;
                pts[block+step2].re = tRe - sRe2;
                pts[block+step2].im = tIm - sIm2;
            }
        }
    }
}


void flt_fft_inverse_float (
    complex_float_t pts[],
    const unsigned N,
    const float sine_table[])
{
    uint32_t shift = 30-cls(N);

    for(uint32_t step = 2; step <= N; step = 2*step, shift--) {
        uint32_t step2 = step >> 1;
        uint32_t step4 = step2 >> 1;
        uint32_t k;

        for(k = 0; k < (step4 + (step2 & 0x1)); k++) {
            float rRe = sine_table[(N>>2)-(k<<shift)];
            float rIm = sine_table[k<<shift];

            for(unsigned block = k; block < k+N; block+=step) {
                float tRe = pts[block].re;
                float tIm = pts[block].im;
                float tRe2 = pts[block + step2].re;
                float tIm2 = pts[block + step2].im;

                float sRe2 = (tRe2*rRe)-(tIm2*rIm);
                float sIm2 = (tRe2*rIm)+(tIm2*rRe);

                pts[block].re = tRe + sRe2;
                pts[block].im = tIm + sIm2;
                pts[block+step2].re = tRe - sRe2;
                pts[block+step2].im = tIm - sIm2;
            }
        }

        for(k=(step2 & 1); k < (step2-step4); k++) {
            float rRe = -sine_table[k<<shift];
            float rIm = sine_table[(N>>2)-(k<<shift)];

            for(unsigned block = k+step4; block < k+step4+N; block+=step) {
                float tRe = pts[block].re;
                float tIm = pts[block].im;
                float tRe2 = pts[block + step2].re;
                float tIm2 = pts[block + step2].im;

                float sRe2 = (tRe2*rRe)-(tIm2*rIm);
                float sIm2 = (tRe2*rIm)+(tIm2*rRe);

                pts[block].re = tRe + sRe2;
                pts[block].im = tIm + sIm2;
                pts[block+step2].re = tRe - sRe2;
                pts[block+step2].im = tIm - sIm2;
            }
        }
    }

    unsigned log2_N = 31-cls(N);

    for(unsigned i=0;i<N;i++){
        pts[i].re = ldexpf(pts[i].re, -(int)log2_N);
        pts[i].im = ldexpf(pts[i].im, -(int)log2_N);
    }
}


void flt_fft_split_spectrum_float(
    complex_float_t pts[],
    const unsigned N)
{

    for(uint32_t i=1;i<N/2;i++){
        float a_re = ldexpf(( pts[i].re + pts[N-i].re), -1);
        float a_im = ldexpf(( pts[i].im - pts[N-i].im), -1);
        float b_re = ldexpf(( pts[i].im + pts[N-i].im), -1);
        float b_im = ldexpf((-pts[i].re + pts[N-i].re), -1);

        pts[  i].re = a_re;
        pts[  i].im = a_im;
        pts[N-i].re = b_re;
        pts[N-i].im = b_im;
    }

    float re = pts[N/2].re;
    float im = pts[N/2].im;

    pts[N/2].re = pts[0].im;
    pts[N/2].im = im;
    pts[  0].im = re;


    for(uint32_t i=1;i<N/4;i++){
        complex_float_t a = pts[N/2 + i];
        complex_float_t b = pts[  N - i];
        pts[N/2 + i] = b;
        pts[  N - i] = a;
    }
}


void flt_fft_merge_spectra_float(
    complex_float_t pts[],
    const unsigned N)
{
    for(uint32_t i=1; i < (N/4); i++){
        complex_float_t a = pts[N/2 + i];
        complex_float_t b = pts[  N - i];
        pts[N/2 + i] = b;
        pts[  N - i] = a;
    }

    float t    = pts[  0].im;
    pts[  0].im = pts[N/2].re;
    pts[N/2].re = t;

    for(uint32_t i = 1; i< (N/2); i++)
    {
        float a_re = pts[  i].re;
        float a_im = pts[  i].im;
        float b_re = pts[N-i].re;
        float b_im = pts[N-i].im;

        pts[  i].re = a_re - b_im;
        pts[  i].im = a_im + b_re;
        pts[N-i].re = b_im + a_re;
        pts[N-i].im = b_re - a_im;
    }
}

