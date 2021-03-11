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
 * @par Disable Vector Tail Support
 * 
 *     XS3_MATH_VECTOR_TAIL_SUPPORT
 * 
 * Most of the `lib_xs3_math` API functions take vectors as inputs or outputs. By default there are no restrictions on 
 * the lengths of these vectors (other than being non-zero).
 * 
 * The XS3 vector registers are `256` bits. Optimal performance is obtained when the length of a vector is a multiple of 
 * the vector register length. The table below indicates the bit depth (bits-per-element; BPE) and number of 
 * elements-per-vector (EPV) for frequently-used data types. Vector elements in excess of a natural boundary (i.e. 
 * `vector_length mod EPV`) are often referred to in this library as the "tail" of a vector.
 * 
 * Usually checking for vector tails does not cost more than a few thread cycles per API function call when there is no 
 * tail. Where there is a tail, handling the tail within an API function is often significantly more expensive than 
 * dealing with a full EPV elements. It is, for example, considerably faster to multiply two 16-bit vectors of length 
 * `16` (using the VPU) than two vectors of length `1`.
 * 
 * The main drawback of allowing vector tails is that it entails a sometimes significant increase in the code memory 
 * footprint. To that end, where an application can guarantee that all vector lengths are an integer multiple of EPV for 
 * that element type, support for vectors tails in some API functions can be disabled by compiling with the macro 
 * `XS3_MATH_VECTOR_TAIL_SUPPORT` defined to be zero.
 * 
 * If vector tail support is disabled, the behavior of API functions where tails are implied by vector lengths is 
 * undefined.
 * 
 * @note Currently, most functions do not yet respect this option.
 * 
 * Element Type  | BPE | EPV 
 * ------------- | --- | ---
 * int8_t        |  8  | 32
 * int16_t       | 16  | 16
 * int32_t       | 32  |  8
 * complex_s32_t | 64  |  4
 * 
 * By default, tails are supported (1).
 * 
 */
 #ifndef XS3_MATH_VECTOR_TAIL_SUPPORT

/**
 * See @ref compile_time_options for details.
 */
 #define XS3_MATH_VECTOR_TAIL_SUPPORT (1)
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



#endif //XS3_MATH_CONF_H_