// Copyright 2021 XMOS LIMITED. This Software is subject to the terms of the 
// XMOS Public License: Version 1

#include "tst_common.h"

#include <xs1.h>
#include <xclib.h>
#include <stdio.h>
#include <assert.h>

#include "bfp_math.h"


void rand_vect_s16(
    int16_t output[],
    const unsigned length,
    const unsigned headroom,
    unsigned* seed)
{
    for(int i = 0; i < length; i++)
        output[i] = pseudo_rand_int16(seed) >> headroom;
}

void rand_vect_s32(
    int32_t output[],
    const unsigned length,
    const unsigned headroom,
    unsigned* seed)
{
    for(int i = 0; i < length; i++)
        output[i] = pseudo_rand_int32(seed) >> headroom;
}

void rand_vect_complex_s16(
    complex_s16_t output[],
    const unsigned length,
    const unsigned headroom,
    unsigned* seed)
{
    for(int i = 0; i < length; i++){
        output[i].re = pseudo_rand_int16(seed) >> headroom;
        output[i].im = pseudo_rand_int16(seed) >> headroom;
    }
}

void rand_vect_complex_s32(
    complex_s32_t output[],
    const unsigned length,
    const unsigned headroom,
    unsigned* seed)
{
    for(int i = 0; i < length; i++){
        output[i].re = pseudo_rand_int32(seed) >> headroom;
        output[i].im = pseudo_rand_int32(seed) >> headroom;
    }
}

void test_random_bfp_s16(
    bfp_s16_t* B, 
    unsigned max_len, 
    unsigned* seed,
    bfp_s16_t* A,
    int length)
{
    if(length <= 0){
        if(max_len != 1)
            length = (pseudo_rand_uint32(seed) % (max_len-1)) + 1;
        else
            length = 1;
    }

    exponent_t exponent = (pseudo_rand_int32(seed) % 40) - 20;

    int shr = (pseudo_rand_uint32(seed) % 10) + 1;

    for(int i = 0; i < length; i++)
        B->data[i] = pseudo_rand_int16(seed) >> shr;
    
    bfp_s16_init(B, B->data, exponent, length, 1);

    if(A != NULL){
        A->length = B->length;
    }
}

void test_random_bfp_s32(
    bfp_s32_t* B, 
    unsigned max_len, 
    unsigned* seed,
    bfp_s32_t* A,
    int length)
{
    if(length <= 0){
        if(max_len != 1)
            length = (pseudo_rand_uint32(seed) % (max_len-1)) + 1;
        else
            length = 1;
    }

    exponent_t exponent = (pseudo_rand_int32(seed) % 40) - 20;

    int shr = (pseudo_rand_uint32(seed) % 10) + 1;

    for(int i = 0; i < length; i++)
        B->data[i] = pseudo_rand_int32(seed) >> shr;
    
    bfp_s32_init(B, B->data, exponent, length, 1);

    if(A != NULL){
        A->length = B->length;
    }

}

void test_random_bfp_complex_s16(
    bfp_complex_s16_t* B, 
    unsigned max_len, 
    unsigned* seed,
    bfp_complex_s16_t* A,
    int length)
{
    if(length <= 0){
        if(max_len != 1)    length = (pseudo_rand_uint32(seed) % (max_len-1)) + 1;
        else                length = 1;
    }

    exponent_t exponent = (pseudo_rand_int32(seed) % 40) - 20;

    int shr = (pseudo_rand_uint32(seed) % 10) + 1;

    for(int i = 0; i < length; i++){
        B->real[i] = pseudo_rand_int16(seed) >> shr;
        B->imag[i] = pseudo_rand_int16(seed) >> shr;
    }

    bfp_complex_s16_init(B, B->real, B->imag, exponent, length, 1);

    if(A != NULL){
        A->length = B->length;
    }

}

void test_random_bfp_complex_s32(
    bfp_complex_s32_t* B, 
    unsigned max_len, 
    unsigned* seed,
    bfp_complex_s32_t* A,
    int length)
{
    if(length <= 0){
        if(max_len != 1)
            length = (pseudo_rand_uint32(seed) % (max_len-1)) + 1;
        else
            length = 1;
    }

    exponent_t exponent = (pseudo_rand_int32(seed) % 40) - 20;

    int shr = (pseudo_rand_uint32(seed) % 10) + 1;

    for(int i = 0; i < length; i++){
        B->data[i].re = pseudo_rand_int32(seed) >> shr;
        B->data[i].im = pseudo_rand_int32(seed) >> shr;
    }
    
    bfp_complex_s32_init(B, B->data, exponent, length, 1);

    if(A != NULL){
        A->length = B->length;
    }

}

void test_random_bfp_ch_pair_s16(
    bfp_ch_pair_s16_t* B, 
    unsigned max_len, 
    unsigned* seed,
    bfp_ch_pair_s16_t* A,
    int length)
{
    if(length <= 0){
        if(max_len != 1)
            length = (pseudo_rand_uint32(seed) % (max_len-1)) + 1;
        else
            length = 1;
    }

    exponent_t exponent = (pseudo_rand_int32(seed) % 40) - 20;

    int shr = (pseudo_rand_uint32(seed) % 10) + 1;

    for(int i = 0; i < length; i++){
        B->data[i].ch_a = pseudo_rand_int16(seed) >> shr;
        B->data[i].ch_b = pseudo_rand_int16(seed) >> shr;
    }
    
    bfp_ch_pair_s16_init(B, B->data, exponent, length, 1);

    if(A != NULL){
        A->length = B->length;
    }

}

void test_random_bfp_ch_pair_s32(
    bfp_ch_pair_s32_t* B, 
    unsigned max_len, 
    unsigned* seed,
    bfp_ch_pair_s32_t* A,
    int length)
{
    if(length <= 0){
        if(max_len != 1)
            length = (pseudo_rand_uint32(seed) % (max_len-1)) + 1;
        else
            length = 1;
    }

    exponent_t exponent = (pseudo_rand_int32(seed) % 40) - 20;

    int shr = (pseudo_rand_uint32(seed) % 10) + 1;

    for(int i = 0; i < length; i++){
        B->data[i].ch_a = pseudo_rand_int32(seed) >> shr;
        B->data[i].ch_b = pseudo_rand_int32(seed) >> shr;
    }
    
    bfp_ch_pair_s32_init(B, B->data, exponent, length, 1);

    if(A != NULL){
        A->length = B->length;
    }

}




void test_double_from_s16(
    double* d_out,
    bfp_s16_t* d_in)
{
    for(int i = 0; i < d_in->length; i++){
        d_out[i] = ldexp(d_in->data[i], d_in->exp);
    }
}

void test_double_from_s32(
    double* d_out,
    bfp_s32_t* d_in)
{
    for(int i = 0; i < d_in->length; i++){
        d_out[i] = ldexp(d_in->data[i], d_in->exp);
    }
}

void test_double_from_complex_s16(
    double* d_out_real,
    double* d_out_imag,
    bfp_complex_s16_t* d_in)
{
    for(int i = 0; i < d_in->length; i++){
        // printf("## %d\t%d\t%d\n", d_in->real[i], d_in->imag[i], d_in->exp);
        d_out_real[i] = ldexp(d_in->real[i], d_in->exp);
        d_out_imag[i] = ldexp(d_in->imag[i], d_in->exp);
    }
}

void test_double_from_complex_s32(
    double* d_out_real,
    double* d_out_imag,
    bfp_complex_s32_t* d_in)
{
    for(int i = 0; i < d_in->length; i++){
        d_out_real[i] = ldexp(d_in->data[i].re, d_in->exp);
        d_out_imag[i] = ldexp(d_in->data[i].im, d_in->exp);
    }
}


void test_s16_from_double(
    int16_t* d_out,
    double* d_in,
    unsigned length,
    exponent_t use_exp)
{
    for(int i = 0; i < length; i++){
        int exp;
        double mant = frexp(d_in[i], &exp);
        mant = mant * ldexp(1, exp - use_exp);
        d_out[i] = (int16_t) mant;
    }
}

void test_s32_from_double(
    int32_t* d_out,
    double* d_in,
    unsigned length,
    exponent_t use_exp)
{
    for(int i = 0; i < length; i++){
        int exp;
        double mant = frexp(d_in[i], &exp);
        mant = mant * ldexp(1, exp - use_exp);
        d_out[i] = (int32_t) mant;
    }
}


void test_complex_s16_from_double(
    int16_t* d_out_real,
    int16_t* d_out_imag,
    double* d_in_real,
    double* d_in_imag,
    unsigned length,
    exponent_t use_exp)
{
    for(int i = 0; i < length; i++){
        int exp;
        double mant;

        mant = frexp(d_in_real[i], &exp);
        mant = mant * ldexp(1, exp - use_exp);
        d_out_real[i] = (int16_t) mant;
        
        mant = frexp(d_in_imag[i], &exp);
        mant = mant * ldexp(1, exp - use_exp);
        d_out_imag[i] = (int16_t) mant;
    }
}


void test_complex_s32_from_double(
    complex_s32_t* d_out,
    double* d_in_real,
    double* d_in_imag,
    unsigned length,
    exponent_t use_exp)
{
    for(int i = 0; i < length; i++){
        int exp;
        double mant;

        mant = frexp(d_in_real[i], &exp);
        mant = mant * ldexp(1, exp - use_exp);
        d_out[i].re = (int32_t) mant;
        
        mant = frexp(d_in_imag[i], &exp);
        mant = mant * ldexp(1, exp - use_exp);
        d_out[i].im = (int32_t) mant;
    }
}