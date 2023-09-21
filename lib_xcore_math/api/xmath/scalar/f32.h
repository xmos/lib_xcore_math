// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include "xmath/types.h"

#include <stdio.h>
#include <assert.h>
#include <math.h>


/**
 * @defgroup scalar_f32_api   IEEE 754 float API
 */

#ifdef __XC__
extern "C" {
#endif


/**
 * @brief Unpack an IEEE 754 single-precision float into a 32-bit mantissa and exponent.
 * 
 * @par Example
 * @code{.c}
 *    // Unpack 1.52345246 * 10^(-5)
 *    float val = 1.52345246e-5;
 *    int32_t mant;
 *    exponent_t exp;
 *    f32_unpack(&mant, &exp, val);
 *    
 *    printf("%ld * 2^(%d) <-- %e\n", mant, exp, val);
 * @endcode
 * 
 * @param[out]  mantissa    Unpacked output mantissa
 * @param[out]  exp         Unpacked output exponent
 * @param[in]   input       Float value to be unpacked
 * 
 * @ingroup scalar_f32_api
 */
C_API
void f32_unpack(
    int32_t* mantissa,
    exponent_t* exp,
    const float input);

    
/**
 * @brief Unpack an IEEE 754 single-precision float into a 16-bit mantissa and exponent.
 * 
 * @par Example
 * @code{.c}
 *    // Unpack 1.52345246 * 10^(-5)
 *    float val = 1.52345246e-5;
 *    int16_t mant;
 *    exponent_t exp;
 *    f32_unpack_s16(&mant, &exp, val);
 *    
 *    printf("%ld * 2^(%d) <-- %e\n", mant, exp, val);
 * @endcode
 * 
 * @note This operation may result in a loss of precision.
 * 
 * @param[out]  mantissa    Unpacked output mantissa
 * @param[out]  exp         Unpacked output exponent
 * @param[in]   input       Float value to be unpacked
 * 
 * @ingroup scalar_f32_api
 */
C_API
void f32_unpack_s16(
    int16_t* mantissa,
    exponent_t* exp,
    const float input);


/**
 * @brief Convert an IEEE754 `float` to a @ref float_s32_t.
 * 
 * @param[in] x Input value
 * 
 * @returns `float_s32_t` representation of `x` 
 * 
 * @exception ET_ARITHMETIC Raised if `x` is infinite or NaN
 * 
 * @ingroup scalar_f32_api
 */
C_API 
float_s32_t f32_to_float_s32(
    const float x);


/**
 * @brief Convert an IEEE754 `double` to a @ref float_s32_t.
 * 
 * @note This operation may result in precision loss.
 * 
 * @param[in] x Input value
 * 
 * @returns `float_s32_t` representation of `x`
 * 
 * @exception ET_ARITHMETIC Raised if `x` is infinite or NaN
 * 
 * @ingroup scalar_f32_api
 */
C_API
float_s32_t f64_to_float_s32(
    const double x);


/**
 * @brief Get the sine of a specified angle.
 * 
 * Computes @math{sin(\theta)} using the power series expansion of @math{sin()} 
 * truncated to 8 terms.
 * 
 * This implementation is meant to make optimal use of the XS3 floating-point unit.
 * 
 * @param[in] theta   Angle @math{\theta} to compute the sine of (in radians)
 * 
 * @returns Sine of the angle @math{\theta}
 * 
 * @exception ET_ARITHMETIC Raised if @math{\theta} is infinite or NaN
 * 
 * @ingroup scalar_f32_api
 */
C_API
float f32_sin(
    const float theta);


/**
 * @brief Get the cosine of a specified angle.
 * 
 * Computes @math{cos(\theta) = sin(\theta+\frac{\pi}{2}} using the power series expansion of 
 * @math{sin()} truncated to 8 terms.
 * 
 * This implementation is meant to make optimal use of the XS3 floating-point unit.
 * 
 * @param[in] theta   Angle @math{\theta} to compute the cosine of (in radians)
 * 
 * @returns Cosine of the angle @math{\theta}
 * 
 * @exception ET_ARITHMETIC Raised if @math{\theta} is infinite or NaN
 * 
 * @ingroup scalar_f32_api
 */
C_API
float f32_cos(
    const float theta);



/**
 * @brief Get the base-2 logarithm of the specified value.
 * 
 * This function computes @math{log_2(x)} using the power series expansion of @math{log_2()}
 * truncated to 11 terms.
 * 
 * @param[in] x   Input value @math{x} to get the logarithm of.
 * 
 * @returns @math{log_2(x)}
 * 
 * @exception ET_ARITHMETIC Raised if @math{x} is infinite or NaN
 * 
 * @ingroup scalar_f32_api
 */
C_API
float f32_log2(
    const float x);


/**
 * @brief Compute power series summation using specified coefficients.
 * 
 * This function is used to compute the sum of terms in a power series, truncated to @math{N} terms,
 * starting with the @math{x^0} term.
 * 
 * `b` is an @math{N}-element vector of coefficients @vector{b} which are multiplied by the 
 * corresponding powers of @math{x}.
 * 
 * @math{N} is the length of @vector{b} and number of terms to sum together.
 * 
 * @operation{
 * &     a \leftarrow \sum_{k=0}^{N-1}\left( x^k\,b_k \right)
 * }
 * 
 * @param[in] x   Input value @math{x}.
 * @param[in] b   Vector of coefficients @vector{b}.
 * @param[in] N   Number of power series terms to sum.
 * 
 * @returns @math{a}, the sum of the first @math{N} power series terms.
 * 
 * @exception ET_ARITHMETIC Raised if @math{x} or any element of @vector{b} is infinite or NaN.
 * 
 * @ingroup scalar_f32_api
 */
C_API
float f32_power_series(
    const float x,
    const float b[],
    const unsigned N);


/**
 * @brief Get a representation of the input @math{x} in normalized form A.
 * 
 * This function is used internally to transform a `float` value into a representation required for
 * certain purposes.
 * 
 * In particular, this function behaves much like `frexpf()`, where it is guaranteed that the 
 * returned value @math{a} is either @math{0} or that  @math{0.5 \le \left| a \right| < 1.0}, and
 * the output exponent @math{p} is such that @math{x = a \cdot 2^{p}}.
 * 
 * In anticipation that future work may require alternative "normalized" representations, this form
 * is being defined here as form A.
 * 
 * @param[in] p   Output exponent @math{p}
 * @param[in] x   Input value @math{x}
 * 
 * @returns @math{a} in normalized form A.
 * 
 * @exception ET_ARITHMETIC Raised if @math{x} or any element of @vector{b} is infinite or NaN.
 * 
 * @ingroup scalar_f32_api
 */
C_API
float f32_normA(
    exponent_t* p,
    const float x);


#ifdef __XC__
}   //extern "C"
#endif