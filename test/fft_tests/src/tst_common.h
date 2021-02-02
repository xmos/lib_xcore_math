// Copyright 2021 XMOS LIMITED. This Software is subject to the terms of the 
// XMOS Public License: Version 1
#pragma once

#include <stdint.h>
#include <stdio.h>

#include "bfp_math.h"
#include "test_random.h"

#define SET_TEST_FILE()     Unity.TestFile = __FILE__

#ifdef __xcore__
#define WORD_ALIGNED __attribute__((aligned (4)))
#define DWORD_ALIGNED __attribute__((aligned (8)))
#else
#define WORD_ALIGNED
#endif

#ifndef PRINT_FUNC_NAMES
#define PRINT_FUNC_NAMES 1
#endif

#ifndef PRINT_ERRORS
#define PRINT_ERRORS 0
#endif

#ifndef TIME_FUNCS
#define TIME_FUNCS 1
#endif

#ifndef WRITE_PERFORMANCE_INFO
#define WRITE_PERFORMANCE_INFO 1
#endif

#ifndef PERFORMANCE_INFO_FILENAME
#define PERFORMANCE_INFO_FILENAME  "perf_info.csv"
#endif

#define TEST_ASSERT_CONVERSION(V) do{ \
    char qwe[100];  if((V)){ sprintf(qwe, "Conversion failure (0x%X)", (V)); \
        TEST_ASSERT_FALSE_MESSAGE(V, qwe); }} while(0)


#if defined(__XC__) || defined(__CPLUSPLUS__)
extern "C" {
#endif 

#ifndef __XC__
static inline signed sext(int a, unsigned b){
#ifdef __XS3A__
    asm("sext %0, %1": "=r"(a): "r"(b));
#else
    unsigned mask = ~((1<<b)-1);

    unsigned p = a >= 0;
    a = p? (a & ~mask) : (a | mask);
#endif
    return a;
}
#endif //__XC__



extern FILE* perf_file;

void xs3_fft_index_bit_reversal_double(
    complex_double_t* a,
    const unsigned length);


void test_double_from_s16(
    double* d_out,
    bfp_s16_t* d_in);
void test_double_from_s32(
    double* d_out,
    bfp_s32_t* d_in);
void test_double_from_complex_s16(
    double* d_out_real,
    double* d_out_imag,
    bfp_complex_s16_t* d_in);
void test_double_from_complex_s32(
    double* d_out_real,
    double* d_out_imag,
    bfp_complex_s32_t* d_in);


void test_s16_from_double(
    int16_t* d_out,
    double* d_in,
    unsigned length,
    exponent_t use_exp);

void test_s32_from_double(
    int32_t* d_out,
    double* d_in,
    unsigned length,
    exponent_t use_exp);

void test_complex_s16_from_double(
    int16_t* d_out_real,
    int16_t* d_out_imag,
    double* d_in_real,
    double* d_in_imag,
    unsigned length,
    exponent_t use_exp);

void test_complex_s32_from_double(
    complex_s32_t* d_out,
    double* d_in_real,
    double* d_in_imag,
    unsigned length,
    exponent_t use_exp);

#if defined(__XC__) || defined(__CPLUSPLUS__)
}   // extern "C"
#endif
