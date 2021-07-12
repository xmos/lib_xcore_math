// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#ifndef XS3_MATH_CONF_H_
#define XS3_MATH_CONF_H_

#ifdef __xs3_math_user_conf_h_exists__
#include "xs3_math_user_conf.h"
#endif


/**
 * @page compile_time_options Compile Time Options
 * 
 * @par BFP Vector Length Checking
 * 
 *     XS3_BFP_DEBUG_CHECK_LENGTHS
 * 
 * Iff true, BFP functions will check (`assert()`) to ensure that each BFP vector argument does not violate any length
 * constraints. Most often this simply ensures that, where BFP functions take multiple vectors as parameters, each of 
 * the vectors has the same length.
 * 
 * Defaults to false (`0`).
 */
#ifndef XS3_BFP_DEBUG_CHECK_LENGTHS

/**
 * Indicates whether the BFP functions should check vector lengths for errors. See @ref compile_time_options for more
 * details.
 */
#define XS3_BFP_DEBUG_CHECK_LENGTHS (0)
#endif



/**
 * @page compile_time_options Compile Time Options
 * 
 * @par 16-bit BFP Square Root Depth
 * 
 *     XS3_BFP_SQRT_DEPTH_S16
 * 
 * The function bfp_sqrt_s16() computes results one bit at a time, starting with bit 14 (the second-to-most significant
 * bit). Because this is a relatively expensive operation, it may be desirable to trade off precision of results for a
 * speed-up.
 * 
 * The time cost of bfp_sqrt_s16() is approximately linear with respect to the depth.
 * 
 * Defaults to `XS3_VECT_SQRT_S16_MAX_DEPTH` (15)
 * 
 * @see bfp_sqrt_s16
 */
#ifndef XS3_BFP_SQRT_DEPTH_S16

/**
 * See @ref compile_time_options for details.
 */
#define XS3_BFP_SQRT_DEPTH_S16 (XS3_VECT_SQRT_S16_MAX_DEPTH)
#endif



/**
 * @page compile_time_options Compile Time Options
 * 
 * @par 32-bit BFP Square Root Depth
 * 
 *     XS3_BFP_SQRT_DEPTH_S32
 * 
 * The function bfp_sqrt_s32() computes results one bit at a time, starting with bit 30 (the second-to-most significant
 * bit). Because this is a relatively expensive operation, it may be desirable to trade off precision of results for a
 * speed-up.
 * 
 * The time cost of bfp_sqrt_s32() is approximately linear with respect to the depth.
 * 
 * Defaults to `XS3_VECT_SQRT_S32_MAX_DEPTH` (31)
 * 
 * @see bfp_sqrt_s32
 */
#ifndef XS3_BFP_SQRT_DEPTH_S32

/**
 * See @ref compile_time_options for details.
 */
#define XS3_BFP_SQRT_DEPTH_S32 (XS3_VECT_SQRT_S32_MAX_DEPTH)
#endif



/**
 * 
 */
#ifndef XS3_MALLOC

/**
 * 
 */
#define XS3_MALLOC  malloc
#endif

/**
 * 
 */
#ifndef XS3_FREE

/**
 * 
 */
#define XS3_FREE    free
#endif

#endif //XS3_MATH_CONF_H_