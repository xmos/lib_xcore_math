// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include "xmath/types.h"

#include <stdio.h>
#include <assert.h>
#include <math.h>

/**
 * @defgroup float_complex_s32_api     Scalar Complex 32-bit Floating-point API
 */

#ifdef __XC__
extern "C" {
#endif


/**
 * @brief Multiply two @ref float_complex_s32_t together.
 * 
 * The inputs @math{x} and @math{y} are multiplied together (using complex multiplication) for a
 * result @math{a}, which is returned.
 * 
 * @operation{ 
 * &     a \leftarrow x \cdot y
 * }
 * 
 * @param[in] x   Input operand @math{x}
 * @param[in] y   Input operand @math{y}
 * 
 * @returns @math{a}, the complex product of @math{x} and @math{y}
 * 
 * @ingroup float_complex_s32_api
 */
C_API
float_complex_s32_t float_complex_s32_mul(
    const float_complex_s32_t x,
    const float_complex_s32_t y);


/**
 * @brief Add two @ref float_complex_s32_t together.
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
 * @returns @math{a}, the sum of @math{x} and @math{y}
 * 
 * @ingroup float_complex_s32_api
 */
C_API
float_complex_s32_t float_complex_s32_add(
    const float_complex_s32_t x,
    const float_complex_s32_t y);


/**
 * @brief Subtract one @ref float_complex_s32_t from another.
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
 * @returns @math{a}, the difference of @math{x} and @math{y}
 * 
 * @ingroup float_complex_s32_api
 */
C_API
float_complex_s32_t float_complex_s32_sub(
    const float_complex_s32_t x,
    const float_complex_s32_t y);


#ifdef __XC__
}   //extern "C"
#endif

