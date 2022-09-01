// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include "xmath/xmath.h"

/**
 * Forward DCT (type II)
 * 
 * This is a naive implementation intended to minimize chances of errors, and should be
 * used only for testing purposes.
 * 
 * Compared against SciPy DCT implementation.
 */
EXTERN_C
void flt_dct_forward(
    float output[],
    float input[],
    const unsigned DCT_N);

/**
 * Inverse DCT (type II)
 * 
 * This is a naive implementation intended to minimize chances of errors, and should be
 * used only for testing purposes.
 * 
 * Compared against SciPy DCT implementation.
 */
EXTERN_C
void flt_dct_inverse(
    float output[],
    float input[],
    const unsigned DCT_N);

/**
 * Forward DCT (type II)
 * 
 * This is a naive implementation intended to minimize chances of errors, and should be
 * used only for testing purposes.
 * 
 * Compared against SciPy DCT implementation.
 */
EXTERN_C
void dbl_dct_forward(
    double output[],
    double input[],
    const unsigned DCT_N);

/**
 * Inverse DCT (type II)
 * 
 * This is a naive implementation intended to minimize chances of errors, and should be
 * used only for testing purposes.
 * 
 * Compared against SciPy DCT implementation.
 */
EXTERN_C
void dbl_dct_inverse(
    double output[],
    double input[],
    const unsigned DCT_N);
