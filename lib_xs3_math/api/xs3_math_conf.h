// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#ifndef XS3_MATH_CONF_H_
#define XS3_MATH_CONF_H_

#ifdef __xs3_math_user_conf_h_exists__
#include "xs3_math_user_conf.h"
#endif

/**
 * @defgroup conf_option_macro  Compile Time Options
 */


/**
 * @page conf_option_debug_check_lengths BFP Vector Length Checking
 * 
 * @par BFP Vector Length Checking
 * 
 * @ref XS3_BFP_DEBUG_CHECK_LENGTHS
 * 
 * Iff true, BFP functions will check (`assert()`) to ensure that each BFP vector argument does not violate any length
 * constraints. Most often this simply ensures that, where BFP functions take multiple vectors as parameters, each of 
 * the vectors has the same length.
 * 
 * Defaults to false (`0`).
 */
#ifndef XS3_BFP_DEBUG_CHECK_LENGTHS

/**
 * Indicates whether the BFP functions should check vector lengths for errors. 
 * 
 * See @ref conf_option_debug_check_lengths for more details.
 * 
 * @ingroup conf_option_macro
 */
#define XS3_BFP_DEBUG_CHECK_LENGTHS (0)
#endif



/**
 * @page conf_option_sqrt_s16_depth 16-bit BFP Square Root Depth
 * 
 * @par 16-bit BFP Square Root Depth
 * 
 * @ref XS3_BFP_SQRT_DEPTH_S16
 * 
 * The function bfp_sqrt_s16() computes results one bit at a time, starting with bit 14 (the second-to-most significant
 * bit). Because this is a relatively expensive operation, it may be desirable to trade off precision of results for a
 * speed-up.
 * 
 * The time cost of bfp_sqrt_s16() is approximately linear with respect to the depth.
 * 
 * Defaults to `XS3_VECT_SQRT_S16_MAX_DEPTH` (15)
 * 
 * @see bfp_s16_sqrt
 */
#ifndef XS3_BFP_SQRT_DEPTH_S16

/**
 * The number of most significant bits which are computed by bfp_s16_sqrt(). 
 * 
 * See @ref conf_option_sqrt_s16_depth for details.
 * 
 * @ingroup conf_option_macro
 */
#define XS3_BFP_SQRT_DEPTH_S16 (XS3_VECT_SQRT_S16_MAX_DEPTH)
#endif



/**
 * @page conf_option_sqrt_s32_depth 32-bit BFP Square Root Depth
 * 
 * @par 32-bit BFP Square Root Depth
 * 
 * @ref XS3_BFP_SQRT_DEPTH_S32
 * 
 * The function bfp_sqrt_s32() computes results one bit at a time, starting with bit 30 (the second-to-most significant
 * bit). Because this is a relatively expensive operation, it may be desirable to trade off precision of results for a
 * speed-up.
 * 
 * The time cost of bfp_sqrt_s32() is approximately linear with respect to the depth.
 * 
 * Defaults to `XS3_VECT_SQRT_S32_MAX_DEPTH` (31)
 * 
 * @see bfp_s32_sqrt
 */
#ifndef XS3_BFP_SQRT_DEPTH_S32

/**
 * The number of most significant bits which are computed by bfp_s32_sqrt(). 
 * 
 * See @ref conf_option_sqrt_s32_depth for details.
 * 
 * @ingroup conf_option_macro
 */
#define XS3_BFP_SQRT_DEPTH_S32 (XS3_VECT_SQRT_S32_MAX_DEPTH)
#endif



/**
 * @page conf_option_malloc_func Dynamic Allocation Function
 * 
 * @par Dynamic Allocation Function
 * 
 * @ref XS3_MALLOC
 * 
 * This function is used to dynamically allocate memory. Defaults to `malloc`. Must have same 
 * signature as `malloc()`
 * 
 */
#ifndef XS3_MALLOC

/**
 * Macro resolves to `malloc` or a user-specified override. 
 * 
 * See @ref conf_option_malloc_func for details.
 * 
 * @see XS3_FREE
 * 
 * @ingroup conf_option_macro
 */
#define XS3_MALLOC  malloc
#endif

/**
 * @page conf_option_free_func Dynamic Deallocation Function
 * 
 * @par Dynamic Deallocation Function
 * 
 * @ref XS3_FREE
 * 
 * This function is used to deallocate dynamically allocated memory. Defaults to `free`. Must have same 
 * signature as `free()`
 */
#ifndef XS3_FREE

/**
 * Macro resolves to `free` or a user-specified override. 
 * 
 * See @ref conf_option_free_func for details.
 * 
 * @see XS3_MALLOC
 * 
 * @ingroup conf_option_macro
 */
#define XS3_FREE    free
#endif

#endif //XS3_MATH_CONF_H_