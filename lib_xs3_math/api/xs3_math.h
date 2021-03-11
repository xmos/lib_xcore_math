// Copyright 2020 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#ifndef XS3_MATH_H_
#define XS3_MATH_H_


/**
 * @file xs3_math.h
 * 
 * Include this file from a source file to access the low-level API, as well as the `lib_xs3_math` types and 
 * configuration. To access the block floating-point API, include `bfp_math.h` instead (it will include this).
 */

#include "xs3_math_conf.h"
#include "xs3_math_types.h"


#include "vect/xs3_vect_s32.h"
#include "vect/xs3_vect_s16.h"
#include "vect/xs3_fft.h"
#include "vect/xs3_filters.h"
#include "xs3_util.h"

#include "xs3_vpu_info.h"


#endif //XS3_MATH_H_