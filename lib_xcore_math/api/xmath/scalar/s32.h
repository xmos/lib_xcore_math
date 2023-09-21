// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include "xmath/types.h"
#include "xmath/xs3/vpu_info.h"

#include <stdio.h>
#include <assert.h>
#include <math.h>

/**
 * @defgroup scalar_s32_api     32-bit Scalar API
 */

#ifdef __XC__
extern "C" {
#endif


/**
 * @brief Maximum bit-depth to calculate with s32_sqrt().
 * 
 * @ingroup scalar_s32_api
 */
#define S32_SQRT_MAX_DEPTH     (31)


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
 *    float val = s32_to_f32(mant, exp);
 *    
 *    printf("%e <-- %ld * 2^(%d)\n", val, mant, exp);
 * @endcode
 * 
 * @param[in] mantissa    Mantissa of value to be packed
 * @param[in] exp         Exponent of value to be packed
 * 
 * @returns   `float` representation of input value
 * 
 * @ingroup scalar_s32_api
 */
C_API
float s32_to_f32(
    const int32_t mantissa,
    const exponent_t exp);


/**
 * @brief Convert a 32-bit floating-point scalar to a 16-bit floating-point scalar.
 * 
 * Converts a 32-bit floating-point scalar, represented by the 32-bit mantissa `b` and exponent
 * `b_exp`, into a 16-bit floating-point scalar, represented by the 16-bit returned mantissa and
 * output exponent `a_exp`.
 * 
 * @param[out] a_exp    Output exponent
 * @param[in]  b        32-bit input mantissa
 * @param[in]  b_exp    Input exponent
 * 
 * @returns     16-bit output mantissa
 * 
 * @ingroup scalar_s32_api
 */
C_API
int16_t s32_to_s16(
    exponent_t* a_exp,
    const int32_t b,
    const exponent_t b_exp);


/**
 * @brief Compute the square root of a 32-bit floating-point scalar.
 * 
 * `b` and `b_exp` together represent the input @math{b \cdot 2^{b\_exp}}. Likewise, `a` and 
 * `a_exp` together represent the result @math{a \cdot 2^{a\_exp}}.
 * 
 * `depth` indicates the number of MSb's which will be calculated. Smaller values here will
 * execute more quickly at the cost of reduced precision. The maximum valid value for `depth`
 * is @ref S32_SQRT_MAX_DEPTH.
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
 * @ingroup scalar_s32_api
 */
C_API
int32_t s32_sqrt(
    exponent_t* a_exp,
    const int32_t b,
    const exponent_t b_exp,
    const unsigned depth);


/**
 * @brief Compute the inverse of a 32-bit integer.
 * 
 * `b` represents the integer @math{b}. `a` and `a_exp` together represent the result @math{a \cdot
 * 2^{a\_exp}}.
 * 
 * @operation{
 *    a \cdot 2^{a\_exp} \leftarrow \frac{1}{b}
 * }
 * 
 * @par Fixed- or Floating-point
 * 
 * If @math{b} is the mantissa of a fixed- or floating-point value with an implicit or explicit
 * exponent @math{b\_exp}, then 
 * 
 * @math{
 * \begin{aligned}
 *   \frac{1}{b \cdot 2^{b\_exp}} &= \frac{1}{b} \cdot 2^{-b\_exp}             \\
 *                                &= a \cdot 2^{a\_exp} \cdot 2^{-b\_exp}      \\
 *                                &= a \cdot 2^{a\_exp - b\_exp}
 * \end{aligned}
 * }
 * 
 * and so @math{b\_exp} should be subtracted from the output exponent @math{a\_exp}.
 * 
 * @param[out] a_exp    Output exponent @math{a\_exp}
 * @param[in]  b        Input integer @math{b}
 * 
 * @returns Output mantissa @math{a}
 * 
 * @ingroup scalar_s32_api
 */
C_API
int32_t s32_inverse(
    exponent_t* a_exp,
    const int32_t b);


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
 * @ingroup scalar_s32_api
 */
C_API
int32_t s32_mul(
    exponent_t* a_exp,
    const int32_t b,
    const int32_t c,
    const exponent_t b_exp,
    const exponent_t c_exp);


/**
 * @brief Convert angle from radians to a modified binary representation.
 *
 * Some trig functions, such as @ref sbrad_sin(), rather than taking an
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
 * @ingroup scalar_s32_api
 */
C_API
sbrad_t radians_to_sbrads(
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
 * @param[in] theta Input angle @math{\theta}, in sbrads (see @ref radians_to_sbrads)
 * 
 * @returns Sine of the specified angle in Q2.30 format.
 * 
 * @ingroup scalar_s32_api
 */
C_API
q2_30 sbrad_sin(
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
 * @param[in] theta Input angle @math{\theta}, in sbrads (see @ref radians_to_sbrads)
 * 
 * @returns Tangent of the specified angle in Q2.30 format.
 * 
 * @ingroup scalar_s32_api
 */
C_API
q2_30 sbrad_tan(
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
 * @ingroup scalar_s32_api
 */
C_API
q2_30 q24_sin(
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
 * @ingroup scalar_s32_api
 */
C_API
q2_30 q24_cos(
    const radian_q24_t theta);


/**
 * @brief Compute the tangent of the specified angle.
 * 
 * This function computes @math{tan(\theta)}. The result is returned as a @ref float_s32_t 
 * containing a mantissa and exponent.
 * 
 * The value of @math{tan(\theta)} is considered undefined where @math{theta=\frac{\pi}{2}+k\pi} for
 * any integer @math{k}. An exception will be raised if @math{\theta} meets this condition.
 * 
 * @operation{ 
 * &     tan(\theta)
 * }
 * 
 * @param[in] theta Input angle @math{\theta}, in radians (Q8.24)
 * 
 * @returns @math{tan(\theta)} as a @ref float_s32_t
 * 
 * @exception ET_ARITHMETIC Raised if @math{tan(\theta)} is undefined.
 * 
 * @ingroup scalar_s32_api
 */
C_API
float_s32_t q24_tan(
    const radian_q24_t theta);


/**
 * @brief Compute @math{e^x} for Q2.30 value near @math{0}.
 * 
 * This function computes @math{e^x} where @math{x} is a fixed-point value with 30 
 * fractional bits.
 * 
 * This function implements @math{e^x} using a truncated power series, and is only intended to be
 * used for inputs in the range @math{-0.5 \le x \le 0.5}.
 * 
 * The output is also in the Q2.30 format.
 * 
 * For the range @math{-0.5 \le x \le 0.5}, the maximum observed error (compared to `exp(double)` 
 * from `math.h`) was `2` (which corresponds to @math{2^{-29}}).
 * 
 * For the range @math{-1.0 \le x \le 1.0}, the corresponding maximum observed error was `324`, or
 * approximately @math{2^{-21}}.
 * 
 * To compute @math{e^x} for @math{x} outside of @math{\left[-0.5, 0.5\right]}, use 
 * `float_s32_exp()`.
 * 
 * @operation{
 * &  y \leftarrow e^x
 * }
 * 
 * @param[in]   x   Input value @math{x}
 * 
 * @returns @math{y}
 * 
 * @ingroup scalar_s32_api
 */
C_API
q2_30 q30_exp_small(
    const q2_30 x);


/**
 * @brief Evaluate the logistic function at the specified point.
 * 
 * This function computes the value of the logistic function @math{y =\frac{1}{1+e^{-x}}}. This is a
 * sigmoidal curve bounded below by @math{y = 0} and above by @math{y = 1}.
 * 
 * The input @math{x} and output @math{y} are both Q8.24 fixed-point values.
 * 
 * If speed is greatly preferred to precision, `q24_logistic_fast()` can be used instead.
 * 
 * @operation{
 * &  y \leftarrow  \frac{1}{1+e^{-x}}
 * }
 * 
 * @param[in] x   Input value @math{x}
 * 
 * @returns @math{y}
 * 
 * @ingroup scalar_s32_api
 */
C_API
q8_24 q24_logistic(
    const q8_24 x);


/**
 * @brief Evaluate the logistic function at the specified point.
 * 
 * This function computes the value of the logistic function @math{y =\frac{1}{1+e^{-x}}}. This is a
 * sigmoidal curve bounded below by @math{y = 0} and above by @math{y = 1}.
 * 
 * The input @math{x} and output @math{y} are both Q8.24 fixed-point values.
 * 
 * This implementation trades off precision for speed, approximating results in a piece-wise linear
 * manner. If a precise result is desired, `q24_logistic()` should be used instead.
 * 
 * @operation{
 * &  y \leftarrow  \frac{1}{1+e^{-x}}
 * }
 * 
 * @param[in] x   Input value @math{x}
 * 
 * @returns @math{y}
 * 
 * @ingroup scalar_s32_api
 */ 
C_API
q8_24 q24_logistic_fast(
    const q8_24 x);


/**
 * @brief Broadcast an integer to a vector chunk.
 * 
 * This function broadcasts the input @math{b} to the 8 elements of @vector{a}.
 * 
 * @operation{
 * &    a_k \leftarrow  b
 * }
 * 
 * @param[out]  a   Output chunk @vector{a}
 * @param[in]   b   Input value @math{b}
 * 
 * @exception ET_LOAD_STORE Raised if `a` is not double word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup scalar_s32_api
 */
C_API
void s32_to_chunk_s32(
    int32_t a[VPU_INT32_EPV],
    int32_t b);


/**
 * @brief Get the first @math{N} powers of @math{b}
 * 
 * This function computes the first @math{N} powers (starting with @math{0}) of the Q2.30 input
 * @math{b}. The results are output as @vector{a}, also in Q2.30 format.
 * 
 * @operation{
 * &    a_0 \leftarrow 2^{30} = \mathtt{Q30(1.0)}   \\
 * &    a_k \leftarrow  round\left(\frac{a_{k-1}\cdot b}{2^{30}}\right)   \\
 * &    \qquad\text{for }k \in \{0..N-1\}
 * }
 * 
 * @param[out]  a   Output @vector{a}
 * @param[in]   b   Input @math{b}
 * @param[in]   N   Number of elements of @vector{a} to compute
 * 
 * @ingroup scalar_s32_api
 */
C_API
void q30_powers(
    q2_30 a[],
    const q2_30 b,
    const unsigned N);

    


/**
 * @brief Fill vector with odd powers of @math{b}.
 * 
 * This function populates the elements of output vector @vector{a} with the odd powers of input
 * @math{b}. The first `count` odd powers of @math{b} are output. The highest power output will be
 * @math{2\cdot\mathtt{count}-1}.
 * 
 * The 64-bit product of each multiplication is right-shifted by `shr` bits and truncated to the 32
 * least significant bits. If @math{b} is a fixed-point value with `shr` fractional bits, then each
 * @math{a_k} will have the same Q-format as input @math{b}. `shr` must be non-negative.
 * 
 * This function neither rounds nor saturates results. It is up to the user to ensure overflows are
 * avoided.
 * 
 * Typical use-case is computing a power series of a function with odd symmetry.
 * 
 * @operation{ 
 * &     b_{sqr} = \frac{b^2}{2^{\mathtt{shr}}}   \\
 * &     a_0 \leftarrow b   \\
 * &     a_k \leftarrow \frac{a_{k-1}\,b_{sqr}}{\mathtt{shr}} \\
 * &      \qquad\text{for } k \in \{1, 2, 3, ..., \mathtt{count} - 1\}
 * }
 * 
 * @param[out]  a       Output vector @vector{a}
 * @param[in]   b       Input @math{b}
 * @param[in]   count   Number of elements to output.
 * @param[in]   shr     Number of bits to right-shift 64-bit products.
 * 
 * @ingroup scalar_s32_api
 */
C_API
void s32_odd_powers(
    int32_t a[],
    const int32_t b,
    const unsigned count,
    const right_shift_t shr);


#ifdef __XC__
}   //extern "C"
#endif
