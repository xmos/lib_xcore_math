// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include "xmath/types.h"
#include "xmath/util.h"
#include "xmath/xs3/vpu_info.h"
#include "xmath/scalar/float_s32.h"

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
static inline int32_t s64_to_s32(
    exponent_t* a_exp,
    const int64_t b,
    const exponent_t b_exp)
{
  const headroom_t b_hr = HR_S64(b);
  const right_shift_t shr = MAX( 0, (int)(32-b_hr) );
  *a_exp = b_exp + shr;
  return (int32_t) (b >> shr);
}


/**
 * @brief Convert a @ref float_s64_t to a @ref float_s32_t.
 *
 * @note This operation may result in precision loss.
 *
 * @param[in] x Input value
 *
 * @returns `float_s32_t` representation of `x`
 *
 * @ingroup scalar_misc_api
 */
static inline float_s32_t float_s64_to_float_s32(
    const float_s64_t x)
{
  float_s32_t res;
  res.mant = s64_to_s32(&res.exp, x.mant, x.exp);
  return res;
}


/**
 * @brief Convert a @ref float_s64_t to a fixed-point @c int32_t with a specified exponent.
 *
 * Returns the @c int32_t value @math{a} such that @math{a \cdot 2^{out\_exp} \approx x}.
 *
 * @note This operation may result in precision loss due to the 64-to-32-bit mantissa conversion.
 *
 * This is equivalent to converting @math{x} to @ref float_s32_t via @ref float_s64_to_float_s32()
 * and then calling @ref float_s32_to_s32().
 *
 * @param[in] x         Input value @math{x}
 * @param[in] out_exp   Exponent of the output fixed-point representation
 *
 * @returns Fixed-point integer @math{a} such that @math{a \cdot 2^{out\_exp} \approx x}
 *
 * @ingroup scalar_misc_api
 */
static inline int32_t float_s64_to_s32(
    const float_s64_t x,
    const exponent_t out_exp)
{
  return float_s32_to_s32(float_s64_to_float_s32(x), out_exp);
}


/**
 * @brief Arithmetic shift right of a 64-bit integer.
 *
 * When a positive `shr` is given, returns `x` right-shifted by `shr` bits,
 * filling the most significant bits with the sign bit.
 * If `shr` is larger than 63, returns 0 for non-negative `x` or -1 for
 * negative `x`.
 *
 * When a negative `shr` is given, returns `x` left-shifted by `|shr|` bits,
 * saturating to `INT64_MAX` or `INT64_MIN` if the result overflows.
 *
 * @param[in] x         Input value
 * @param[in] shr       Right shift to apply to the input
 * @return int64_t      Shifted result
 *
 * @ingroup scalar_misc_api
 */
C_API
int64_t s64_ashr(
    const int64_t x,
    const right_shift_t shr);

#ifdef __XC__
}   //extern "C"
#endif
