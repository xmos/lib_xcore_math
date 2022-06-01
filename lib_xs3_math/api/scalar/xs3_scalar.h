// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include "../xs3_math_types.h"

#include <stdio.h>
#include <assert.h>
#include <math.h>



/**
 * @defgroup xs3_scalar_funcs  XS3 Scalar Functions
 */


/**
 * @brief Maximum bit-depth to calculate with xs3_s32_sqrt().
 * 
 * @ingroup xs3_scalar_funcs
 */
#define XS3_S32_SQRT_MAX_DEPTH     (31)


/**
 * @brief Pack a floating point value into an IEEE 754 single-precision float.
 * 
 * The value returned is the nearest representable approximation to @math{ m \cdot 2^{p} } where
 * @math{m} is `mantissa` and @math{p} is `exp`.
 * 
 * @note This operation may result in a loss of precision.
 * 
 * @par Example
 * @code{.c}
 *    // Pack -12345678 * 2^{-13} into a float
 *    int32_t mant = -12345678;
 *    exponent_t exp = -13;
 *    float val = xs3_pack_float(mant, exp);
 *    
 *    printf("%e <-- %ld * 2^(%d)\n", val, mant, exp);
 * @endcode
 * 
 * @param[in] mantissa    Mantissa of value to be packed
 * @param[in] exp         Exponent of value to be packed
 * 
 * @returns   `float` representation of input value
 * 
 * @ingroup xs3_scalar_funcs
 */
C_API
float xs3_pack_float(
    const int32_t mantissa,
    const exponent_t exp);


/**
 * @brief Unpack an IEEE 754 single-precision float into a 32-bit mantissa and exponent.
 * 
 * @par Example
 * @code{.c}
 *    // Unpack 1.52345246 * 10^(-5)
 *    float val = 1.52345246e-5;
 *    int32_t mant;
 *    exponent_t exp;
 *    xs3_unpack_float(&mant, &exp, val);
 *    
 *    printf("%ld * 2^(%d) <-- %e\n", mant, exp, val);
 * @endcode
 * 
 * @param[out]  mantissa    Unpacked output mantissa
 * @param[out]  exp         Unpacked output exponent
 * @param[in]   input       Float value to be unpacked
 * 
 * @ingroup xs3_scalar_funcs
 */
C_API
void xs3_unpack_float(
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
 *    xs3_unpack_float_s16(&mant, &exp, val);
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
 * @ingroup xs3_scalar_funcs
 */
C_API
void xs3_unpack_float_s16(
    int16_t* mantissa,
    exponent_t* exp,
    const float input);

/**
 * @brief Convert a 64-bit floating-point scalar to a 32-bit floating-point scalar.
 * 
 * Converts a 64-bit floating-point scalar, represented by the 64-bit mantissa `b` and exponent `b_exp`, into a 32-bit
 * floating-point scalar, represented by the 32-bit returned mantissa and output exponent `a_exp`.
 * 
 * @param[out] a_exp    Output exponent
 * @param[in]  b        64-bit input mantissa
 * @param[in]  b_exp    Input exponent
 * 
 * @returns     32-bit output mantissa
 * 
 * @ingroup xs3_scalar_funcs
 */
C_API
int32_t xs3_scalar_s64_to_s32(
    exponent_t* a_exp,
    const int64_t b,
    const exponent_t b_exp);

/**
 * @brief Convert a 32-bit floating-point scalar to a 16-bit floating-point scalar.
 * 
 * Converts a 32-bit floating-point scalar, represented by the 32-bit mantissa `b` and exponent `b_exp`, into a 16-bit
 * floating-point scalar, represented by the 16-bit returned mantissa and output exponent `a_exp`.
 * 
 * @param[out] a_exp    Output exponent
 * @param[in]  b        32-bit input mantissa
 * @param[in]  b_exp    Input exponent
 * 
 * @returns     16-bit output mantissa
 * 
 * @ingroup xs3_scalar_funcs
 */
C_API
int16_t xs3_scalar_s32_to_s16(
    exponent_t* a_exp,
    const int32_t b,
    const exponent_t b_exp);

/**
 * @brief Convert a 16-bit floating-point scalar to a 32-bit floating-point scalar.
 * 
 * Converts a 16-bit floating-point scalar, represented by the 16-bit mantissa `b` and exponent `b_exp`, into a 32-bit
 * floating-point scalar, represented by the 32-bit returned mantissa and output exponent `a_exp`.
 * 
 * `remove_hr`, if nonzero, indicates that the output mantissa should have no headroom. Otherwise, the output mantissa
 * will be the same as the input mantissa.
 * 
 * @param[out] a_exp        Output exponent
 * @param[in]  b            16-bit input mantissa
 * @param[in]  b_exp        Input exponent
 * @param[in]  remove_hr    Whether to remove headroom in output
 * 
 * @returns     32-bit output mantissa
 * 
 * @ingroup xs3_scalar_funcs
 */
C_API
int32_t xs3_scalar_s16_to_s32(
    exponent_t* a_exp,
    const int16_t b,
    const exponent_t b_exp,
    const unsigned remove_hr);


/**
 * @brief Compute the square root of a 32-bit floating-point scalar.
 * 
 * `b` and `b_exp` together represent the input @math{b \cdot 2^{b\_exp}}. Likewise, `a` and 
 * `a_exp` together represent the result @math{a \cdot 2^{a\_exp}}.
 * 
 * `depth` indicates the number of MSb's which will be calculated. Smaller values here will
 * execute more quickly at the cost of reduced precision. The maximum valid value for `depth`
 * is @ref XS3_S32_SQRT_MAX_DEPTH.
 * 
 * @operation{
 *    a \cdot 2^{a\_exp} \leftarrow \sqrt{\left( b \cdot 2^{b\_exp} \right)}
 * }
 * 
 * @param[out] a_exp    Output exponent @math{a\_exp}
 * @param[in]  b        Input mantissa @math{b}
 * @param[in]  b_exp    Input exponent @math{b\_exp}
 * @param[in]  depth    Number of most significant bits to calculate
 * 
 * @returns Output mantissa @math{a}
 * 
 * @ingroup xs3_scalar_funcs
 */
C_API
int32_t xs3_s32_sqrt(
    exponent_t* a_exp,
    const int32_t b,
    const exponent_t b_exp,
    const unsigned depth);

/**
 * @brief Compute the inverse of a 32-bit integer.
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
 * @ingroup xs3_scalar_funcs
 */
C_API
int32_t xs3_s32_inverse(
    exponent_t* a_exp,
    const int32_t b);
    

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
 * @ingroup xs3_scalar_funcs
 */
C_API
int16_t xs3_s16_inverse(
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
 * @ingroup xs3_scalar_funcs
 */
C_API
int16_t xs3_s16_mul(
    exponent_t* a_exp,
    const int16_t b,
    const int16_t c,
    const exponent_t b_exp,
    const exponent_t c_exp);

/**
 * @brief Compute the product of two 32-bit floating-point scalars.
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
 * @ingroup xs3_scalar_funcs
 */
C_API
int32_t xs3_s32_mul(
    exponent_t* a_exp,
    const int32_t b,
    const int32_t c,
    const exponent_t b_exp,
    const exponent_t c_exp);


/**
 * @brief Get the size of a 32-bit unsigned number.
 * 
 * This function reports the size of the number as @math{a}, the number of bits required to store unsigned integer @math{N}. This is equivalent to @f$ ceil\left(log_2\left(N\right)\right) @f$.
 * 
 * `N` is the input @math{N}.
 * 
 * @operation{
 *  a \leftarrow
 *  \begin{cases}
 *    & 0 && N = 0 \\
 *    & \lceil log_2\left( N \right) \rceil && otherwise
 *  \end{cases}
 * }
 * 
 * @param[in] N     Number to get the size of
 * 
 * @returns     Number of bits @math{a} required to store @math{N}
 * 
 * @ingroup xs3_scalar_funcs
 */
static inline unsigned ceil_log2(
    unsigned N)
{
    if(N == 0) return 0;
    return 32-cls(N-1);
}


/**
 * @brief Convert angle from radians to a modified binary representation.
 *
 * Some trig functions, such as @ref xs3_sbrad_sin() and @ref xs3_sbrad_tan(), rather than taking an
 * angle specified in radians (e.g. @ref radian_q24_t), require their argument to be a modified
 * representation of the angle, as an @ref sbrad_t. The modified binary representation takes into
 * account various properies of the @math{sin(\theta)} function to simplify certain operations.
 * 
 * For any angle @math{\theta} there is a unique angle @math{\alpha} where 
 * @math{-1\le\alpha\le1} and @math{sin(\frac{\pi}{2}\alpha) = sin(\theta)}. This function
 * essentially just maps the input angle @math{\theta} onto the corresponding angle @math{\alpha} in
 * that region and returns the result in a Q1.31 format.
 * 
 * In this library, the unit of the resulting angle @math{\alpha} is referred to as an 'sbrad'.
 * 'brad' because @math{\alpha} is a kind of 
 * [binary angular measurement](https://en.wikipedia.org/wiki/Binary_angular_measurement), and 's'
 * because the symmetries of @math{sin(\theta)} are what's being accounted for.
 * 
 * @param[in] theta Input angle @math{\theta}, in radians (Q8.24)
 * 
 * @returns Output angle @math{\alpha}, in sbrads
 * 
 * @ingroup xs3_scalar_funcs
 */
C_API
sbrad_t xs3_radians_to_sbrads(
    const radian_q24_t theta);


/**
 * @brief Compute the sine of the specified angle.
 * 
 * This function computes @math{sin(\frac{\pi}{2}\theta)}, returning the result in Q2.30 format.
 * 
 * The input angle @math{\theta} must be expressed in sbrads (@ref sbrad_t), and must
 * represent a value between @math{\pm 0.5} (inclusive) (as a Q1.31).
 * 
 * @operation{ 
 * &     sin(\frac{\pi}{2}\theta)
 * }
 * 
 * @param[in] theta Input angle @math{\theta}, in sbrads (see @ref xs3_radians_to_sbrads)
 * 
 * @returns Sine of the specified angle in Q2.30 format.
 * 
 * @ingroup xs3_scalar_funcs
 */
C_API
q2_30 xs3_sbrad_sin(
    const sbrad_t theta);


/**
 * @brief Compute the tangent of the specified angle.
 * 
 * This function computes @math{tan(\frac{\pi}{2}\theta)}, returning the result in Q2.30 format.
 * 
 * The input angle @math{\theta} must be expressed in sbrads (@ref sbrad_t), and must
 * represent a value between @math{\pm 0.25} (inclusive) (as a Q1.31).
 * 
 * @operation{ 
 * &     tan(\frac{\pi}{2}\theta)
 * }
 * 
 * @param[in] theta Input angle @math{\theta}, in sbrads (see @ref xs3_radians_to_sbrads)
 * 
 * @returns Tangent of the specified angle in Q2.30 format.
 * 
 * @ingroup xs3_scalar_funcs
 */
C_API
q2_30 xs3_sbrad_tan(
    const sbrad_t theta);


/**
 * @brief Compute the sine of the specified angle.
 * 
 * This function computes @math{sin(\theta)}, returning the result in Q2.30 format.
 * 
 * @operation{ 
 * &     sin(\theta)
 * }
 * 
 * @param[in] theta Input angle @math{\theta}, in radians (Q8.24)
 * 
 * @returns @math{sin(\theta)} as a Q2.30
 * 
 * @ingroup xs3_scalar_funcs
 */
C_API
q2_30 xs3_q24_sin(
    const radian_q24_t theta);
    

/**
 * @brief Compute the cosine of the specified angle.
 * 
 * This function computes @math{cos(\theta)}, returning the result in Q2.30 format.
 * 
 * @operation{ 
 * &     cos(\theta)
 * }
 * 
 * @param[in] theta Input angle @math{\theta}, in radians (Q8.24)
 * 
 * @returns @math{cos(\theta)} as a Q2.30
 * 
 * @ingroup xs3_scalar_funcs
 */
C_API
q2_30 xs3_q24_cos(
    const radian_q24_t theta);