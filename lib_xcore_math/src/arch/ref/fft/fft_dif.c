// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>

#include "xmath/xmath.h"
#include "vpu_helper.h"
#include "xmath_fft_lut.h"

//load 4 complex 32-bit values into a buffer
static void load_vec(
    complex_s32_t dst[],
    const complex_s32_t src[])
{
    for(int i = 0; i < 4; i++){
        dst[i] = src[i];
    }
}

static void vftff(
    complex_s32_t vR[],
    const right_shift_t shift_mode)
{
    struct {
        int64_t re;
        int64_t im;
    } s[4];

    s[0].re = vR[0].re + vR[2].re;
    s[0].im = vR[0].im + vR[2].im;
    s[1].re = vR[1].re + vR[3].re;
    s[1].im = vR[1].im + vR[3].im;
    s[2].re = vR[0].re - vR[2].re;
    s[2].im = vR[0].im - vR[2].im;
    s[3].re = vR[1].im - vR[3].im;
    s[3].im = vR[3].re - vR[1].re;

    vR[0].re = (int32_t) ASHR(32)(s[0].re + s[1].re, shift_mode);
    vR[0].im = (int32_t) ASHR(32)(s[0].im + s[1].im, shift_mode);
    vR[1].re = (int32_t) ASHR(32)(s[0].re - s[1].re, shift_mode);
    vR[1].im = (int32_t) ASHR(32)(s[0].im - s[1].im, shift_mode);
    vR[2].re = (int32_t) ASHR(32)(s[2].re + s[3].re, shift_mode);
    vR[2].im = (int32_t) ASHR(32)(s[2].im + s[3].im, shift_mode);
    vR[3].re = (int32_t) ASHR(32)(s[2].re - s[3].re, shift_mode);
    vR[3].im = (int32_t) ASHR(32)(s[2].im - s[3].im, shift_mode);
}

static void vftfb(
    complex_s32_t vR[],
    const right_shift_t shift_mode)
{
    struct {
        int64_t re;
        int64_t im;
    } s[4];

    s[0].re = vR[0].re + vR[2].re;
    s[0].im = vR[0].im + vR[2].im;
    s[1].re = vR[1].re + vR[3].re;
    s[1].im = vR[1].im + vR[3].im;
    s[2].re = vR[0].re - vR[2].re;
    s[2].im = vR[0].im - vR[2].im;
    s[3].re = vR[3].im - vR[1].im;
    s[3].im = vR[1].re - vR[3].re;

    vR[0].re = (int32_t) ASHR(32)(s[0].re + s[1].re, shift_mode);
    vR[0].im = (int32_t) ASHR(32)(s[0].im + s[1].im, shift_mode);
    vR[1].re = (int32_t) ASHR(32)(s[0].re - s[1].re, shift_mode);
    vR[1].im = (int32_t) ASHR(32)(s[0].im - s[1].im, shift_mode);
    vR[2].re = (int32_t) ASHR(32)(s[2].re + s[3].re, shift_mode);
    vR[2].im = (int32_t) ASHR(32)(s[2].im + s[3].im, shift_mode);
    vR[3].re = (int32_t) ASHR(32)(s[2].re - s[3].re, shift_mode);
    vR[3].im = (int32_t) ASHR(32)(s[2].im - s[3].im, shift_mode);
}



void fft_dif_forward (
    complex_s32_t x[],
    const unsigned N,
    headroom_t* hr,
    exponent_t* exp)
{
    const unsigned FFT_N_LOG2 = 31 - CLS_S32(N);

    const complex_s32_t* W = XMATH_DIF_FFT_LUT(N);

    exponent_t exp_modifier = 0;
    right_shift_t shift_mode = 0;

    complex_s32_t vD[4] = {{0}}, vR[4] = {{0}}, vC[4] = {{0}};

    shift_mode = (*hr == 3)? 0 : (*hr < 3)? 1 : -1;
    exp_modifier += shift_mode;

    if(N != 4){

        for(unsigned n = 0; n < FFT_N_LOG2-2; n++){
            
            const int b = 1<<(FFT_N_LOG2-1-n);
            const int a = 1<<(2+n);

            for(int k = b-4; k >= 0; k -= 4){
                
                load_vec(vC, W);
                W = &W[4];

                for(int j = 0; j < a/4; j+=1){

                    const int s = 2*j*b+k;

                    load_vec(vR, &x[s]);

                    for(int i = 0; i < 4; i++){
                        vD[i].re = ASHR(32)(((int64_t)x[s+b+i].re) - vR[i].re, shift_mode);
                        vD[i].im = ASHR(32)(((int64_t)x[s+b+i].im) - vR[i].im, shift_mode);
                        vR[i].re = ASHR(32)(((int64_t)x[s+b+i].re) + vR[i].re, shift_mode);
                        vR[i].im = ASHR(32)(((int64_t)x[s+b+i].im) + vR[i].im, shift_mode);
                    }

                    load_vec(&x[s], vR);

                    vect_complex_s32_mul(vR, vD, vC, 4, 0, 0);

                    load_vec(&x[s+b], vR);
                    
                };
            }
            
            const headroom_t cur_hr = vect_complex_s32_headroom(x, N);
            
            shift_mode = (cur_hr == 3)? 0 : (cur_hr < 3)? 1 : -1;
            exp_modifier += shift_mode;
            
        }
    }
    

    for(unsigned j = 0; j < (N>>2); j++){
        load_vec(vR, &x[4*j]);
        vftff(vR, shift_mode);
        load_vec(&x[4*j], vR);
    }

    *hr = vect_complex_s32_headroom(x, N);
    *exp = *exp + exp_modifier;
}




void fft_dif_inverse (
    complex_s32_t x[],
    const unsigned N,
    headroom_t* hr,
    exponent_t* exp)
{
    const unsigned FFT_N_LOG2 = 31 - CLS_S32(N);

    const complex_s32_t* W = XMATH_DIF_FFT_LUT(N);

    exponent_t exp_modifier = -(int)FFT_N_LOG2;
    right_shift_t shift_mode = 0;

    complex_s32_t vD[4] = {{0}}, vR[4] = {{0}}, vC[4] = {{0}};

    shift_mode = (*hr == 3)? 0 : (*hr < 3)? 1 : -1;
    exp_modifier += shift_mode;

    if(N != 4){

        for(unsigned n = 0; n < FFT_N_LOG2-2; n++){
            
            const int b = 1<<(FFT_N_LOG2-1-n);
            const int a = 1<<(2+n);

            for(int k = b-4; k >= 0; k -= 4){
                
                load_vec(vC, W);
                W = &W[4];

                for(int j = 0; j < a/4; j+=1){

                    const int s = 2*j*b+k;

                    load_vec(vR, &x[s]);

                    for(int i = 0; i < 4; i++){
                        vD[i].re = ASHR(32)(((int64_t)x[s+b+i].re) - vR[i].re, shift_mode);
                        vD[i].im = ASHR(32)(((int64_t)x[s+b+i].im) - vR[i].im, shift_mode);
                        vR[i].re = ASHR(32)(((int64_t)x[s+b+i].re) + vR[i].re, shift_mode);
                        vR[i].im = ASHR(32)(((int64_t)x[s+b+i].im) + vR[i].im, shift_mode);
                    }

                    load_vec(&x[s], vR);

                    vect_complex_s32_conj_mul(vR, vD, vC, 4, 0, 0);

                    load_vec(&x[s+b], vR);
                    
                };
            }
            
            const headroom_t cur_hr = vect_complex_s32_headroom(x, N);
            
            shift_mode = (cur_hr == 3)? 0 : (cur_hr < 3)? 1 : -1;
            exp_modifier += shift_mode;
            
        }
    }
    

    for(unsigned j = 0; j < (N>>2); j++){
        load_vec(vR, &x[4*j]);
        vftfb(vR, shift_mode);
        load_vec(&x[4*j], vR);
    }

    *hr = vect_complex_s32_headroom(x, N);
    *exp = *exp + exp_modifier;
}
