// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include "xmath/types.h"
#include "xmath/xs3/vpu_info.h"

#include <stdio.h>
#include <assert.h>
#include <math.h>

/**
 * @defgroup scalar_s16_api     32-bit Scalar API
 */

#ifdef __XC__
extern "C" {
#endif


/**
 * @brief Convert a 16-bit floating-point scalar to a 32-bit floating-point scalar.
 * 
 * Converts a 16-bit floating-point scalar, represented by the 16-bit mantissa `b` and exponent
 * `b_exp`, into a 32-bit floating-point scalar, represented by the 32-bit returned mantissa and
 * output exponent `a_exp`.
 *
 * `remove_hr`, if nonzero, indicates that the output mantissa should have no headroom. Otherwise,
 * the output mantissa will be the same as the input mantissa.
 * 
 * @param[out] a_exp        Output exponent
 * @param[in]  b            16-bit input mantissa
 * @param[in]  b_exp        Input exponent
 * @param[in]  remove_hr    Whether to remove headroom in output
 * 
 * @returns     32-bit output mantissa
 * 
 * @ingroup scalar_s16_api
 */
C_API
int32_t s16_to_s32(
    exponent_t* a_exp,
    const int16_t b,
    const exponent_t b_exp,
    const unsigned remove_hr);
    

/**
 * @brief Compute the inverse of a 16-bit integer.
 * 
 * `b` represents the integer @math{b}. `a` and `a_exp` together represent the 
 * result @math{a \cdot 2^{a\_exp}}.
 * 
 * @operation{
 *    a \cdot 2^{a\_exp} \leftarrow \frac{1}{b}
 * }
 * 
 * @param[out] a_exp    Output exponent @math{a\_exp}
 * @param[in]  b        Input integer @math{b}
 * 
 * @returns Output mantissa @math{a}
 * 
 * @ingroup scalar_s16_api
 */
C_API
int16_t s16_inverse(
    exponent_t* a_exp,
    const int16_t b);

/**
 * @brief Compute the product of two 16-bit floating-point scalars.
 * 
 * `a` and `a_exp` together represent the result @math{a \cdot 2^{a\_exp}}.
 * 
 * `b` and `b_exp` together represent the result @math{b \cdot 2^{b\_exp}}.
 * 
 * `c` and `c_exp` together represent the result @math{c \cdot 2^{c\_exp}}.
 * 
 * @operation{
 *    a \cdot 2^{a\_exp} \leftarrow \left( b\cdot 2^{b\_exp} \right) \cdot
 *                                  \left( c\cdot 2^{c\_exp} \right)
 * }
 * 
 * @param[out] a_exp  Output exponent @math{a\_exp}
 * @param[in]  b      First input mantissa @math{b}
 * @param[in]  c      Second input mantissa @math{c}
 * @param[in]  b_exp  First input exponent @math{b\_exp}
 * @param[in]  c_exp  Second input exponent @math{c\_exp}
 * 
 * @returns Output mantissa @math{a}
 * 
 * @ingroup scalar_s16_api
 */
C_API
int16_t s16_mul(
    exponent_t* a_exp,
    const int16_t b,
    const int16_t c,
    const exponent_t b_exp,
    const exponent_t c_exp);


#ifdef __XC__
}   //extern "C"
#endif
