// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include "xs3_math_conf.h"
#include "xs3_math_types.h"
#include "xs3_util.h"

#ifdef __XC__
extern "C" {
#endif


/**
 * @page page_xs3_vect_f32_h  xs3_vect_f32.h
 * 
 * This header contains functions implementing operations on single-precision
 * (32-bit) floats, optimized for xCore XS3.
 * 
 * @note This header is included automatically through `xs3_math.h` or `bfp_math.h`.
 * 
 * @ingroup xs3_math_header_file
 */



/**
 * 
 */
C_API
exponent_t xs3_vect_f32_max_exponent(
    const float b[], 
    const unsigned length);

/**
 * 
 */
C_API
void xs3_vect_f32_to_s32(
    int32_t a[],
    const float b[], 
    const unsigned length,
    const exponent_t exp);

/**
 * 
 */
C_API
void xs3_vect_f32_fft_forward(
    float a[],
    const unsigned length);

/**
 * 
 */
C_API
void xs3_vect_f32_fft_inverse(
    float buffer[],
    const unsigned length);

    
/**
 * 
 */
C_API
float xs3_vect_f32_dot(
    const float b[],
    const float c[],
    const unsigned length);

    
/**
 * 
 */
C_API
float xs3_vect_f32_dot_fast(
    const float b[],
    const float c[],
    const unsigned length);


#ifdef __XC__
}   //extern "C"
#endif

