// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#ifndef BFP_MATH_H_
#define BFP_MATH_H_

#ifdef __XC__
extern "C" { 
#endif


/**
 * @page page_bfp_math_h  bfp_math.h
 * 
 * This header includes the header files necessary to use `lib_xs3_math`'s high-level block floating-
 * point API as well as its low-level vector API, including types and functions for 16- and 32-bit
 * BFP vectors.
 *  
 * @ingroup xs3_math_header_file
 */

/**
 * @defgroup bfp16_func     16-Bit Block Floating-Point Functions
 * @defgroup bfp32_func     32-Bit Block Floating-Point Functions
 * @defgroup bfp_fft_func   Block Floating-Point FFT Functions
 */

#include "xs3_math.h"

#include "bfp/bfp_init.h"
#include "bfp/bfp_s32.h"
#include "bfp/bfp_s16.h"
#include "bfp/bfp_complex_s32.h"
#include "bfp/bfp_complex_s16.h"
#include "bfp/bfp_fft.h"

#ifdef __XC__
} // extern "C"
#endif

#endif //BFP_MATH_H_