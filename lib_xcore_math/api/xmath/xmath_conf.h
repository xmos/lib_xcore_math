// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#ifdef __xmath_user_conf_h_exists__
#include "xmath_user_conf.h"
#endif


/**
 * @defgroup config_options   XMath Compile Time Options
 */


#ifndef XMATH_BFP_DEBUG_CHECK_LENGTHS
/**
 * @brief Indicates whether the BFP functions should check vector lengths for errors.
 * 
 * Iff true, BFP functions will check (`assert()`) to ensure that each BFP vector argument does not violate any length
 * constraints. Most often this simply ensures that, where BFP functions take multiple vectors as parameters, each of 
 * the vectors has the same length.
 * 
 * Defaults to false (`0`).
 * 
 * @ingroup config_options
 */
#define XMATH_BFP_DEBUG_CHECK_LENGTHS (0)
#endif


#ifndef XMATH_BFP_SQRT_DEPTH_S16
/**
 * @brief The number of most significant bits which are computed by bfp_s16_sqrt(). 
 * 
 * The function bfp_sqrt_s16() computes results one bit at a time, starting with bit 14 (the
 * second-to-most significant bit). Because this is a relatively expensive operation, it may be
 * desirable to trade off precision of results for a speed-up.
 * 
 * The time cost of bfp_sqrt_s16() is approximately linear with respect to the depth.
 * 
 * Defaults to `VECT_SQRT_S16_MAX_DEPTH` (15)
 * 
 * @see bfp_s16_sqrt
 * 
 * @ingroup config_options
 */
#define XMATH_BFP_SQRT_DEPTH_S16 (VECT_SQRT_S16_MAX_DEPTH)
#endif


#ifndef XMATH_BFP_SQRT_DEPTH_S32
/**
 * @brief The number of most significant bits which are computed by bfp_s32_sqrt(). 
 * 
 * The function bfp_sqrt_s32() computes results one bit at a time, starting with bit 30 (the
 * second-to-most significant bit). Because this is a relatively expensive operation, it may be
 * desirable to trade off precision of results for a speed-up.
 * 
 * The time cost of bfp_sqrt_s32() is approximately linear with respect to the depth.
 * 
 * Defaults to `VECT_SQRT_S32_MAX_DEPTH` (31)
 * 
 * @see bfp_s32_sqrt
 * 
 * @ingroup config_options
 */
#define XMATH_BFP_SQRT_DEPTH_S32 (VECT_SQRT_S32_MAX_DEPTH)
#endif

#ifdef _WIN32
#include <stdlib.h> // needed for malloc() and free()
#endif

#ifndef XMATH_MALLOC
/**
 * @brief Function used to dynamically allocate memory.
 * 
 * This function is used to dynamically allocate memory. Defaults to `malloc`. Must have same 
 * signature as `malloc()`
 * 
 * @see XMATH_FREE
 * 
 * @ingroup config_options
 */
#define XMATH_MALLOC  malloc
#endif

/**
 * @page conf_option_free_func Dynamic Deallocation Function
 * 
 * @par Dynamic Deallocation Function
 * 
 * @ref XMATH_FREE
 */
#ifndef XMATH_FREE
/**
 * @brief Function use to free dynamically allocated memory.
 * 
 * This function is used to deallocate dynamically allocated memory. Defaults to `free`. Must have
 * same signature as `free()`
 * 
 * @see XMATH_MALLOC
 * 
 * @ingroup config_options
 */
#define XMATH_FREE    free
#endif
