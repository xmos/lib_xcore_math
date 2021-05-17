// Copyright 2020 XMOS LIMITED. This Software is subject to the terms of the 
// XMOS Public License: Version 1

#pragma once

#include <stdint.h>
#include <stdio.h>

#include "bfp_math.h"
#include "test_random.h"
#include "pseudo_rand.h"
#include "rand_frame.h"

#define SET_TEST_FILE()     Unity.TestFile = __FILE__

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



EXTERN_C
static inline signed sext(int a, unsigned b){

#if !defined(__XC__) && defined(__XS3A__)
    asm("sext %0, %1": "=r"(a): "r"(b));
#else
    unsigned mask = ~((1<<b)-1);

    unsigned p = a >= 0;
    a = p? (a & ~mask) : (a | mask);
#endif

    return a;
}


extern FILE* perf_file;

EXTERN_C
void xs3_fft_index_bit_reversal_double(
    complex_double_t* a,
    const unsigned length);
    
EXTERN_C
void print_warns(
    int start_case, 
    unsigned test_c, 
    unsigned test_asm);
