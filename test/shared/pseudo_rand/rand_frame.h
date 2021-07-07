// Copyright 2021 XMOS LIMITED. This Software is subject to the terms of the 
// XMOS Public License: Version 1

#pragma once

#include "bfp_math.h"


EXTERN_C
void test_random_bfp_s16(
    bfp_s16_t* B, 
    unsigned max_len, 
    unsigned* seed,
    bfp_s16_t* A,
    int length);


EXTERN_C
void test_random_bfp_s32(
    bfp_s32_t* B, 
    unsigned max_len, 
    unsigned* seed,
    bfp_s32_t* A,
    int length);

EXTERN_C
void test_random_bfp_complex_s16(
    bfp_complex_s16_t* B, 
    unsigned max_len, 
    unsigned* seed,
    bfp_complex_s16_t* A,
    int length);

EXTERN_C
void test_random_bfp_complex_s32(
    bfp_complex_s32_t* B, 
    unsigned max_len, 
    unsigned* seed,
    bfp_complex_s32_t* A,
    int length);

EXTERN_C
void test_random_bfp_ch_pair_s16(
    bfp_ch_pair_s16_t* B, 
    unsigned max_len, 
    unsigned* seed,
    bfp_ch_pair_s16_t* A,
    int length);

EXTERN_C
void test_random_bfp_ch_pair_s32(
    bfp_ch_pair_s32_t* B, 
    unsigned max_len, 
    unsigned* seed,
    bfp_ch_pair_s32_t* A,
    int length);


EXTERN_C
void test_double_from_s16(
    double* d_out,
    bfp_s16_t* d_in);

EXTERN_C
void test_double_from_s32(
    double* d_out,
    bfp_s32_t* d_in);

EXTERN_C
void test_double_from_complex_s16(
    double* d_out_real,
    double* d_out_imag,
    bfp_complex_s16_t* d_in);

EXTERN_C
void test_double_from_complex_s32(
    double* d_out_real,
    double* d_out_imag,
    bfp_complex_s32_t* d_in);


EXTERN_C
void test_s16_from_double(
    int16_t* d_out,
    double* d_in,
    unsigned length,
    exponent_t use_exp);


EXTERN_C
void test_s32_from_double(
    int32_t* d_out,
    double* d_in,
    unsigned length,
    exponent_t use_exp);


EXTERN_C
void test_complex_s16_from_double(
    int16_t* d_out_real,
    int16_t* d_out_imag,
    double* d_in_real,
    double* d_in_imag,
    unsigned length,
    exponent_t use_exp);


EXTERN_C
void test_complex_s32_from_double(
    complex_s32_t* d_out,
    double* d_in_real,
    double* d_in_imag,
    unsigned length,
    exponent_t use_exp);
