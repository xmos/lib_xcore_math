// Copyright 2021 XMOS LIMITED. This Software is subject to the terms of the 
// XMOS Public License: Version 1
#pragma once

#include "xs3_math.h"

#ifdef __XC__
extern "C" {
#endif

/**
 * 
 */
unsigned flt_bitrev(
    const unsigned index, 
    const size_t bit_width);

/**
 * 
 */
void flt_bit_reverse_indexes_float(
    complex_float_t a[],
    const unsigned length);

/**
 * 
 */
void flt_bit_reverse_indexes_double(
    complex_double_t a[],
    const unsigned length);

/**
 * sine_lut must have a length of `N/4 + 1`
 */
void flt_make_sine_table_float(
    float sine_lut[], 
    const unsigned N);

/**
 * sine_lut must have a length of `N/4 + 1`
 */
void flt_make_sine_table_double(
    double sine_lut[], 
    const unsigned N);

/**
 * 
 */
void flt_fft_forward_float(
    complex_float_t pts[],
    const unsigned N,
    const float sine_table[]);

/**
 * 
 */
void flt_fft_forward_double(
    complex_double_t pts[],
    const unsigned N,
    const double sine_table[]);

/**
 * 
 */
void flt_fft_inverse_float(
    complex_float_t pts[],
    const unsigned N,
    const float sine_table[]);

/**
 * 
 */
void flt_fft_inverse_double(
    complex_double_t pts[],
    const unsigned N,
    const double sine_table[]);

/**
 * 
 */
void flt_fft_split_spectrum_float( 
    complex_float_t pts[], 
    const unsigned N);

/**
 * 
 */
void flt_fft_split_spectrum_double( 
    complex_double_t pts[], 
    const unsigned N);

/**
 * 
 */
void flt_fft_merge_spectra_float(
    complex_float_t pts[], 
    const unsigned N);

/**
 * 
 */
void flt_fft_merge_spectra_double(
    complex_double_t pts[], 
    const unsigned N);

#ifdef __XC__
}
#endif