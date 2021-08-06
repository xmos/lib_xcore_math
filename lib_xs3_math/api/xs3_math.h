// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#ifndef XS3_MATH_H_
#define XS3_MATH_H_


#ifdef __XC__
extern "C" { 
#endif

/**
 * @page page_xs3_math_h  xs3_math.h
 * 
 * This header includes the header files necessary to use `lib_xs3_math`'s low-level vector API, including 
 * types and functions for 16- and 32-bit vectors.
 * 
 * To access the block floating-point API, include @ref page_bfp_math_h instead (which will include this 
 * file).
 * 
 * @ingroup xs3_math_header_file
 */

/**
 * @defgroup xs3_vect8_func       XS3 16-Bit Vector Functions
 * @defgroup xs3_vect16_func      XS3 16-Bit Vector Functions
 * @defgroup xs3_vect32_func      XS3 32-Bit Vector Functions
 * @defgroup xs3_fft_func         XS3 FFT-Related Functions
 * @defgroup xs3_mixed_vect_func  XS3 Mixed-Depth Vector Functions
 * 
 * @defgroup xs3_vect16_prepare   XS3 16-Bit Prepare Functions
 * @defgroup xs3_vect32_prepare   XS3 32-Bit Prepare Functions
 */

/**
 * @defgroup xs3_math_header_file   lib_xs3_math Header Files
 */

#include "xs3_math_conf.h"
#include "xs3_api.h"
#include "xs3_math_types.h"

#include "vect/xs3_vect_s32.h"
#include "vect/xs3_vect_s16.h"
#include "vect/xs3_vect_s8.h"
#include "vect/xs3_mixed.h"
#include "vect/xs3_fft.h"
#include "vect/xs3_filters.h"
#include "xs3_scalar.h"
#include "xs3_util.h"

#include "xs3_vpu_info.h"


#ifdef __XC__
} // extern "C"
#endif

#endif //XS3_MATH_H_