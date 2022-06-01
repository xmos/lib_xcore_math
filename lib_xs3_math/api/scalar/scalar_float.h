// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include "../xs3_math_types.h"

#include <stdio.h>
#include <assert.h>
#include <math.h>

/**
 * @page page_scalar_float_h  xs3_scalar_float.h
 * 
 * The functions contained here are part of the scalar (non-IEEE754) floating-point API.
 * 
 * @ingroup xs3_math_header_file
 */


/**
 * @defgroup scalar_float_funcs  Scalar floating-point functions
 */


/**
 * @brief Convert a @ref float_s64_t to a @ref float_s32_t.
 * 
 * @note This operation may result in precision loss.
 * 
 * @param[in] x Input value
 * 
 * @returns `float_s32_t` representation of `x`
 * 
 * @ingroup scalar_float_funcs
 */
C_API
float_s32_t float_s64_to_float_s32(
    const float_s64_t x);
    

/**
 * @brief Convert an IEEE754 `float` to a @ref float_s32_t.
 * 
 * @param[in] x Input value
 * 
 * @returns `float_s32_t` representation of `x` 
 * 
 * @exception ET_ARITHMETIC Raised if `x` is infinite or NaN
 * 
 * @ingroup scalar_float_funcs
 */
C_API 
float_s32_t float_to_float_s32(
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
 * @ingroup scalar_float_funcs
 */
C_API
float_s32_t double_to_float_s32(
    const double x);


/**
 * @brief Convert a @ref float_s32_t to a @ref float_s64_t.
 * 
 * @param[in] x Input value
 * 
 * @returns `float_s64_t` representation of `x`
 * 
 * @ingroup scalar_float_funcs
 */
C_API
float_s64_t float_s32_to_float_s64(
    const float_s32_t x);


/**
 * @brief Convert a @ref float_s32_t to an IEEE754 `float`.
 * 
 * @param[in] x Input value
 * 
 * @returns `float` representation of `x`
 * 
 * @ingroup scalar_float_funcs
 */
C_API
float float_s32_to_float(
    const float_s32_t x);


/**
 * @brief Convert a @ref float_s32_t to an IEEE754 `double`.
 * 
 * @param[in] x Input value
 * 
 * @returns `double` representation of `x`
 * 
 * @ingroup scalar_float_funcs
 */
C_API
double float_s32_to_double(
    const float_s32_t x);


/**
 * @brief Multiply two @ref float_s32_t together.
 * 
 * The inputs @math{x} and @math{y} are multiplied together for a result @math{a}, which is
 * returned.
 * 
 * @operation{ 
 * &     a \leftarrow x \cdot y
 * }
 * 
 * @param[in] x   Input operand @math{x}
 * @param[in] y   Input operand @math{y}
 * 
 * @returns The product of @math{x} and @math{y}
 * 
 * @ingroup scalar_float_funcs
 */
C_API
float_s32_t float_s32_mul(
    const float_s32_t x,
    const float_s32_t y);


/**
 * @brief Add two @ref float_s32_t together.
 * 
 * The inputs @math{x} and @math{y} are added together for a result @math{a}, which is returned.
 * 
 * @operation{ 
 * &     a \leftarrow x + y
 * }
 * 
 * @param[in] x   Input operand @math{x}
 * @param[in] y   Input operand @math{y}
 * 
 * @returns The sum of @math{x} and @math{y}
 * 
 * @ingroup scalar_float_funcs
 */
C_API
float_s32_t float_s32_add(
    const float_s32_t x,
    const float_s32_t y);


/**
 * @brief Subtract one @ref float_s32_t from another.
 * 
 * The input @math{y} is subtracted from the input @math{x} for a result @math{a}, which is
 * returned.
 * 
 * @operation{ 
 * &     a \leftarrow x - y
 * }
 * 
 * @param[in] x   Input operand @math{x}
 * @param[in] y   Input operand @math{y}
 * 
 * @returns The difference of @math{x} and @math{y}
 * 
 * @ingroup scalar_float_funcs
 */
C_API
float_s32_t float_s32_sub(
    const float_s32_t x,
    const float_s32_t y);


/**
 * @brief Divide one @ref float_s32_t from another.
 * 
 * The input @math{x} is divided by the input @math{y} for a result @math{a}, which is returned.
 * 
 * @operation{ 
 * &     a \leftarrow \frac{x}{y}
 * }
 * 
 * @param[in] x   Input operand @math{x}
 * @param[in] y   Input operand @math{y}
 * 
 * @returns The result of @math{x / y}
 * 
 * @exception ET_ARITHMETIC if @math{Y} is @math{0}
 * 
 * @ingroup scalar_float_funcs
 */
C_API
float_s32_t float_s32_div(
    const float_s32_t x,
    const float_s32_t y);


/**
 * @brief Get the absolute value of a @ref float_s32_t.
 * 
 * @math{a}, the absolute value of @math{x} is returned.
 * 
 * @operation{ 
 * &     a \leftarrow \left| x \right|
 * }
 * 
 * @param[in] x   Input operand @math{x}
 * 
 * @returns The absolute value of @math{x}
 * 
 * @ingroup scalar_float_funcs
 */
C_API
float_s32_t float_s32_abs(
    const float_s32_t x);


/**
 * @brief Determine whether one @ref float_s32_t is greater than another.
 * 
 * The inputs @math{x} and @math{y} are compared. The result @math{a} is true iff @math{x} is
 * greater than @math{y} and false otherwise. @math{a} is returned.
 * 
 * @operation{ 
 * &     a \leftarrow \begin{cases}
 * &         1  &   x \gt y             \\
 * &         0  &   otherwise
 * &     \end{cases}  
 * }
 * 
 * @param[in] x   Input operand @math{x}
 * @param[in] y   Input operand @math{y}
 * 
 * @returns 1 iff @math{x \gt y}; 0 otherwise
 * 
 * @ingroup scalar_float_funcs
 */
C_API
unsigned float_s32_gt(
    const float_s32_t x,
    const float_s32_t y);


/**
 * @brief Determine whether one @ref float_s32_t is greater or equal to another.
 * 
 * The inputs @math{x} and @math{y} are compared. The result @math{a} is true iff @math{x} is
 * greater than or equal to @math{y} and false otherwise. @math{a} is returned.
 * 
 * @operation{ 
 * &     a \leftarrow \begin{cases}
 * &         1  &   x \geq y  \\
 * &         0  &   otherwise
 * &     \end{cases}  
 * }
 * 
 * @param[in] x   Input operand @math{x}
 * @param[in] y   Input operand @math{y}
 * 
 * @returns 1 iff @math{x \geq y}; 0 otherwise
 * 
 * @ingroup scalar_float_funcs
 */
C_API
unsigned float_s32_gte(
    const float_s32_t x,
    const float_s32_t y);


/**
 * @brief Update an exponential moving average.
 * 
 * This function updates an exponential moving average by applying a single new sample. @math{x} is
 * taken as the previous EMA state, with @math{y} as the new sample. The EMA coefficient
 * @math{\alpha} is applied to the term including @math{x}.
 * 
 * `coef` is a fixed-point value in a UQ2.30 format (i.e. has an implied exponent of
 * @math{-30}), and should be in the range @math{0 \leq \alpha \leq 1}.
 * 
 * @operation{ 
 * &     a \leftarrow \alpha \cdot x + (1 - \alpha) \cdot y
 * }
 * 
 * @param[in] x           Input operand @math{x}
 * @param[in] y           Input operand @math{y}
 * @param[in] coef        EMA coefficient @math{\alpha} encoded in UQ2.30 format
 * 
 * @returns The new EMA state
 * 
 * @ingroup scalar_float_funcs
 */
C_API
float_s32_t float_s32_ema(
    const float_s32_t x,
    const float_s32_t y,
    const uq2_30 coef);


/**
 * @brief Get the square root of a @ref float_s32_t.
 * 
 * This function computes the square root of @math{x}. The result, @math{a} is returned.
 * 
 * The precision with which @math{a} is computed is configurable via the @ref XS3_BFP_SQRT_DEPTH_S32
 * configuration parameter. It indicates the number of most significant bits to be calculated.
 * 
 * @operation{ 
 * &     a \leftarrow \sqrt{x}
 * }
 * 
 * @warning @math{x} must be non-negative to get a correct result.
 * 
 * @param[in] x   Input operand @math{x}
 * 
 * @returns The square root of @math{x}
 * 
 * @sa XS3_BFP_SQRT_DEPTH_S32
 * 
 * @ingroup scalar_float_funcs
 */
C_API
float_s32_t float_s32_sqrt(
    const float_s32_t x);


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
 * @ingroup scalar_float_funcs
 */
C_API
float float_sin(
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
 * @ingroup scalar_float_funcs
 */
C_API
float float_cos(
    const float theta);