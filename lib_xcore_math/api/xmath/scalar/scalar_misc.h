// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include "xmath/types.h"
#include "xmath/xs3/vpu_info.h"

#include <stdio.h>
#include <assert.h>
#include <math.h>

/**
 * @defgroup scalar_misc_api     Miscellaneous Scalar API
 */

#ifdef __XC__
extern "C" {
#endif



/**
 * @brief Get the size of a 32-bit unsigned number.
 * 
 * This function reports the size of the number as @math{a}, the number of bits required to store
 * unsigned integer @math{N}. This is equivalent to @f$ ceil\left(log_2\left(N\right)\right) @f$.
 *
 * `N` is the input @math{N}.
 * 
 * @operation{
 *  a \leftarrow 
 *  \begin{cases}
 *      0 & N = 0 \\
 *      \lceil log_2\left( N \right) \rceil & otherwise
 *  \end{cases}
 * }
 * 
 * @param[in] N     Number to get the size of
 * 
 * @returns     Number of bits @math{a} required to store @math{N}
 * 
 * @ingroup scalar_misc_api
 */
static inline unsigned u32_ceil_log2(
    unsigned N)
{
    if(N == 0) return 0;
    return 32-cls(N-1);
}


/**
 * @brief Convert a 64-bit floating-point scalar to a 32-bit floating-point scalar.
 * 
 * Converts a 64-bit floating-point scalar, represented by the 64-bit mantissa `b` and exponent
 * `b_exp`, into a 32-bit floating-point scalar, represented by the 32-bit returned mantissa and
 * output exponent `a_exp`.
 * 
 * @param[out] a_exp    Output exponent
 * @param[in]  b        64-bit input mantissa
 * @param[in]  b_exp    Input exponent
 * 
 * @returns     32-bit output mantissa
 * 
 * @ingroup scalar_misc_api
 */
C_API
int32_t s64_to_s32(
    exponent_t* a_exp,
    const int64_t b,
    const exponent_t b_exp);


/**
 * @brief Convert a @ref float_s64_t to a @ref float_s32_t.
 * 
 * @note This operation may result in precision loss.
 * 
 * @param[in] x Input value
 * 
 * @returns `float_s32_t` representation of `x`
 * 
 * @ingroup float_s32_api
 */
C_API
float_s32_t float_s64_to_float_s32(
    const float_s64_t x);


#ifdef __XC__
}   //extern "C"
#endif
