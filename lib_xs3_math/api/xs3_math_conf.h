
#ifndef XS3_MATH_CONF_H_
#define XS3_MATH_CONF_H_

#ifdef __xs3_math_user_conf_h_exists__
#include "xs3_math_user_conf.h"
#endif


/**
 * \page compile_time_options Compile Time Options
 * 
 * \par BFP Length Checking
 * 
 *     XS3_BFP_DEBUG_CHECK_LENGTHS
 * 
 * Iff true, vectorized BFP functions which take multiple vectors as input will check to 
 * ensure that each BFP vector has the same length. 
 * 
 * Defaults to false (`0`).
 */
#ifndef XS3_BFP_DEBUG_CHECK_LENGTHS
#define XS3_BFP_DEBUG_CHECK_LENGTHS (0)
#endif


/**
 * \page compile_time_options Compile Time Options
 * 
 * \par Allow Arithmetic Saturation
 * 
 *     XS3_BFP_ALLOW_SATURATION
 * 
 * Iff true, certain BFP operations may result in values that would have been `INT32_MIN`
 * saturating to `INT32_MIN+1` (for 32-bit BFP operations), or `INT16_MIN` values saturating
 * to `INT16_MIN+1` (for 16-bit operations).
 * 
 * This saturation can be prevented by forcing that there always be 1 bit of headroom in
 * a result (save for the case in which the result contains `INT*_MIN`), and thus comes at
 * the cost of one fewer possible bits of precision in results.
 * 
 * Defaults to false (`0`).
 */
#ifndef XS3_BFP_ALLOW_SATURATION
#define XS3_BFP_ALLOW_SATURATION (0)
#endif


/**
 * \page compile_time_options Compile Time Options
 * 
 * \par Disable Vector Tail Support
 * 
 *     XS3_MATH_VECTOR_TAIL_SUPPORT
 * 
 * Most of the `lib_xs3_math` API functions take vectors as inputs or outputs. By default
 * there are no restrictions on the lengths of these vectors (other than being non-zero).
 * 
 * The XS3 vector registers are `256` bits. Optimal performance is obtained when the length
 * of a vector is a multiple of the vector register length. The table below indicates the
 * bit depth (bits-per-element; BPE) and number of elements-per-vector (EPV) for frequently-used 
 * data types. Vector elements in excess of a natural boundary (i.e.  `vector_length mod EPV`) 
 * are often referred to as the "tail" of a vector.
 * 
 * Usually checking for vector tails does not cost more than a few thread cycles per API
 * function call when there is no tail. Where there is a tail, handling the tail within an 
 * API function is often significantly more expensive than dealing with a full EPV elements.
 * It is, for example, considerably faster to multiple two 16-bit vectors of length `16` 
 * (using the VPU) than two vectors of length `1`.
 * 
 * The main drawback of allowing vector tails is that it entails a sometimes significant 
 * increase in the code memory footprint. To that end, where an application is guaranteed
 * that all vector lengths are an integer multiple of EPV for that element type, support 
 * for vectors tails in many API functions can be disabled by compiling with the macro
 * `XS3_MATH_VECTOR_TAIL_SUPPORT` defined to be zero.
 * 
 * If vector tail support is disabled, the behavior of API functions where tails are implied
 * by vector lengths is undefined.
 * 
 * Element Type  | BPE | EPV 
 * ------------- | --- | ---
 * int8_t        |  8  | 32
 * int16_t       | 16  | 16
 * int32_t       | 32  |  8
 * complex_s32_t | 64  |  4
 *  
 * 
 * 
 */
 #ifndef XS3_MATH_VECTOR_TAIL_SUPPORT
 #define XS3_MATH_VECTOR_TAIL_SUPPORT (1)
 #endif

#endif //XS3_MATH_CONF_H_