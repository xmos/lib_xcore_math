// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include "xs3_math_conf.h"
#include "xs3_math_types.h"
#include "xs3_util.h"

#ifdef __XC__
extern "C" {
#endif

/**
 * @page vector_functions32 32-bit Vector Functions
 * 
 * Below is a listing of the low-level API functions provided by this library that operate on 32-bit data.
 */

/**
 * Maximum bit-depth that can be calculated by xs3_vect_s32_sqrt().
 * 
 * @see xs3_vect_s32_sqrt
 */
#define XS3_VECT_SQRT_S32_MAX_DEPTH     (31)


/**
 * @brief Obtain the output exponent and input shifts to add or subtract two 16- or 32-bit BFP vectors.
 * 
 * The block floating-point functions in this library which add or subtract vectors are of the general form:
 * 
 * @math{ 
 *      \bar{a} \cdot 2^{a\_exp} = \bar{b}\cdot 2^{b\_exp} \pm \bar{c}\cdot 2^{c\_exp} }
 * }
 * 
 * @vector{b} and @vector{c} are the input mantissa vectors with exponents @math{b\_exp} and @math{c\_exp}, which are 
 * shared by each element of their respective vectors. @vector{a} is the output mantissa vector with exponent 
 * @math{a\_exp}. Two additional properties, @math{b\_hr} and @math{c\_hr}, which are the 
 * headroom of mantissa vectors @vector{b} and @vector{c} respectively, are required by this function.
 * 
 * In order to avoid any overflows in the output mantissas, the output exponent @math{a\_exp} must be chosen such that 
 * the largest (in the sense of absolute value) possible output mantissa will fit into the allotted space (e.g. 32 bits
 * for xs3_vect_s32_add()). Once @math{a\_exp} is chosen, the input bit-shifts @math{b\_shr} and @math{c\_shr} are 
 * calculated to achieve that resulting exponent.
 * 
 * This function chooses @math{a\_exp} to be the minimum exponent known to avoid overflows, given the input exponents
 * (@math{b\_exp} and @math{c\_exp}) and input headroom (@math{b\_hr} and @math{c\_hr}).
 * 
 * This function is used calculate the output exponent and input bit-shifts for each of the following functions:
 * * xs3_vect_s16_add()
 * * xs3_vect_s32_add()
 * * xs3_vect_s16_sub()
 * * xs3_vect_s32_sub()
 * * xs3_vect_complex_s16_add()
 * * xs3_vect_complex_s32_add()
 * * xs3_vect_complex_s16_sub()
 * * xs3_vect_complex_s32_sub()
 * 
 * @par Adjusting Output Exponents
 * 
 * If a specific output exponent `desired_exp` is needed for the result (e.g. for emulating fixed-point arithmetic), the 
 * `b_shr` and `c_shr` produced by this function can be adjusted according to the following:
 * \code{.c}
 *      exponent_t desired_exp = ...; // Value known a priori
 *      right_shift_t new_b_shr = b_shr + (desired_exp - a_exp);
 *      right_shift_t new_c_shr = c_shr + (desired_exp - a_exp);
 * \endcode
 * 
 * When applying the above adjustment, the following conditions should be maintained:
 * * `b_hr + b_shr >= 0`
 * * `c_hr + c_shr >= 0`
 * 
 * Be aware that using smaller values than strictly necessary for `b_shr` and `c_shr` can result in saturation, and 
 * using larger values may result in unnecessary underflows or loss of precision.
 * 
 * @par Notes
 * 
 * * If @math{b\_hr} or @math{c\_hr} are unknown, they can be calculated using the appropriate headroom function 
 * (e.g. xs3_vect_complex_s16_headroom() for complex 16-bit vectors) or the value `0` can always be safely used (but may 
 * result in reduced precision).
 *
 * @param[out] a_exp    Output exponent associated with output mantissa vector @vector{a}
 * @param[out] b_shr    Signed arithmetic right-shift to be applied to elements of @vector{b}. Used by the function 
 *                      which computes the output mantissas @vector{a}
 * @param[out] c_shr    Signed arithmetic right-shift to be applied to elements of @vector{c}. Used by the function 
 *                      which computes the output mantissas @vector{a}
 * @param[in]  b_exp    Exponent of BFP vector @vector{b}
 * @param[in]  c_exp    Exponent of BFP vector @vector{c}
 * @param[in]  b_hr     Headroom of BFP vector @vector{b} 
 * @param[in]  c_hr     Headroom of BFP vector @vector{c}
 * 
 * @see xs3_vect_s16_add
 * @see xs3_vect_s32_add
 * @see xs3_vect_s16_sub
 * @see xs3_vect_s32_sub
 * @see xs3_vect_complex_s16_add
 * @see xs3_vect_complex_s32_add
 * @see xs3_vect_complex_s16_sub
 * @see xs3_vect_complex_s32_sub
 */
C_API
void xs3_vect_add_sub_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    right_shift_t* c_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr);


/**
 * @brief Calculate the headroom of a 32-bit channel-pair vector.
 * 
 * The headroom of an N-bit integer is the number of bits that the integer's value may be left-shifted
 * without any information being lost. Equivalently, it is one less than the number of leading sign bits.
 * 
 * The headroom of a `ch_pair_s32_t` struct is the minimum of the headroom of each of its 16-bit fields,
 * `ch_a` and `ch_b`.
 * 
 * The headroom of a 32-bit channel-pair vector is the minimum of the headroom of each of its `ch_pair_s32_t`
 * elements.
 * 
 * This function efficiently traverses the elements of @vector{b} to determine its headroom.
 * 
 * `b[]` represents a 32-bit channel-pair vector @vector{b}. `b[]` must begin at a word-aligned address.
 * 
 * `length` is the number of elements in `b[]`.
 * 
 * @low_op{32, @f$
 *      a \leftarrow min\!\\{ HR_{32}\left(x_0\right), HR_{32}\left(x_1\right), ..., 
 *              HR_{32}\left(x_{length-1}\right) \\}
 * @f$ }
 * 
 * @param[in]   b       Input channel-pair vector @vector{b}
 * @param[in]   length  Number of elements in @vector{b}
 * 
 * @returns     @math{a}, the headroom of vector @vector{b}
 * 
 * @see xs3_vect_ch_pair_s16_headroom
 * @see xs3_vect_s16_headroom
 * @see xs3_vect_s32_headroom
 * @see xs3_vect_complex_s16_headroom
 * @see xs3_vect_complex_s32_headroom
 */
C_API
headroom_t xs3_vect_ch_pair_s32_headroom(
    const ch_pair_s32_t x[],
    const unsigned length);


/**
 * @brief Set all elements of a 32-bit channel-pair vector to the specified value.
 * 
 * `a[]` represents the 32-bit vector @vector{a}. It must begin at a word-aligned address.
 * 
 * `ch_a` and `ch_b` are respectively the channel A and channel B values to which each element will be set.
 * 
 * `length` is the number of elements in `a[]`.
 * 
 * @low_op{32, @f$ 
 *      ChA\\{a_k\\} \leftarrow ch\_a \\
 *      ChB\\{a_k\\} \leftarrow ch\_b \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If `ch_a` and `ch_b` are channel A and channel B mantissas of a floating-point channel-pair with exponent 
 * @math{b\_exp}, then the output channel-pair vector @vector{a} are the mantissas of BFP channel-pair vector 
 * @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp}.
 * 
 * @param[out]  a           Output channel-pair vector @vector{a}
 * @param[in]   ch_b        Value to set channel A of elements of @vector{a} to
 * @param[in]   ch_a        Value to set channel B of elements of @vector{a} to
 * @param[in]   length      Number of elements in vector{a}
 * 
 * @see xs3_vect_ch_pair_s16_set
 * @see xs3_vect_s16_set
 * @see xs3_vect_s32_set
 * @see xs3_vect_complex_s16_set
 * @see xs3_vect_complex_s32_set
 */
C_API
void xs3_vect_ch_pair_s32_set(
    ch_pair_s32_t a[],
    const int32_t ch_a,
    const int32_t ch_b,
    const unsigned length);


/**
 * @brief Left-shift the elements of a 32-bit channel-pair vector by a specified number of bits.
 * 
 * `a[]` and `b[]` represent the 32-bit channel-pair vectors @vector{a} and @vector{b} respectively. Each must begin 
 * at a word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in vectors @vector{a} and @vector{b}.
 * 
 * `b_shl` is the signed arithmetic left-shift applied to each element of @vector{b}. 
 * 
 * @low_op{32, @f$ 
 *      ChA\\{a_k\\} \leftarrow sat_{32}(\lfloor ChA\\{b_k\\} \cdot {2^{b\_shl}} \rfloor) \\
 *      ChB\\{a_k\\} \leftarrow sat_{32}(\lfloor ChB\\{b_k\\} \cdot {2^{b\_shl}} \rfloor) \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the mantissas of a BFP channel-pair vector @math{ \bar{b} \cdot 2^{b\_exp} }, then the resulting 
 * vector @vector{a} are the mantissas of BFP channel-pair vector @math{\bar{a} \cdot 2^{a\_exp}}, where 
 * @math{\bar{a} = \bar{b} \cdot 2^{b\_shl}} and @math{a\_exp = b\_exp}.
 * 
 * @param[out]  a           Output channel-pair vector @vector{a}
 * @param[in]   b           Input channel-pair vector @vector{b}
 * @param[in]   length      Number of elements in @vector{a} and @vector{b}
 * @param[in]   b_shl       Arithmetic left-shift applied to elements of @vector{b}
 * 
 * @returns     Headroom of output vector @vector{a}
 * 
 * @see xs3_vect_ch_pair_s32_shr
 */
C_API
headroom_t xs3_vect_ch_pair_s32_shl(
    ch_pair_s32_t a[],
    const ch_pair_s32_t b[],
    const unsigned length,
    const left_shift_t b_shl);


/**
 * @brief Right-shift the elements of a 32-bit channel-pair vector by a specified number of bits.
 * 
 * `a[]` and `b[]` represent the 32-bit channel-pair vectors @vector{a} and @vector{b} respectively. Each must begin 
 * at a word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in vectors @vector{a} and @vector{b}.
 * 
 * `b_shr` is the signed arithmetic right-shift applied to each element of @vector{b}. 
 * 
 * @low_op{32, @f$ 
 *      ChA\\{a_k\\} \leftarrow sat_{32}(\lfloor ChA\\{b_k\\} \cdot {2^{-b\_shr}} \rfloor) \\
 *      ChB\\{a_k\\} \leftarrow sat_{32}(\lfloor ChB\\{b_k\\} \cdot {2^{-b\_shr}} \rfloor) \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the mantissas of a BFP channel-pair vector @math{ \bar{b} \cdot 2^{b\_exp} }, then the resulting 
 * vector @vector{a} are the mantissas of BFP channel-pair vector @math{\bar{a} \cdot 2^{a\_exp}}, where 
 * @math{\bar{a} = \bar{b} \cdot 2^{-b\_shr}} and @math{a\_exp = b\_exp}.
 * 
 * @param[out]  a           Output channel-pair vector @vector{a}
 * @param[in]   b           Input channel-pair vector @vector{b}
 * @param[in]   length      Number of elements in @vector{a} and @vector{b}
 * @param[in]   b_shr       Arithmetic right-shift applied to elements of @vector{b}
 * 
 * @returns     Headroom of output vector @vector{a}
 * 
 * @see xs3_vect_ch_pair_s32_shr
 */
C_API
headroom_t xs3_vect_ch_pair_s32_shr(
    ch_pair_s32_t a[],
    const ch_pair_s32_t b[],
    const unsigned length,
    const right_shift_t b_shr);


/**
 * @brief Obtain the output exponent and shift parameter used by xs3_vect_complex_s32_mag() and 
 *        xs3_vect_complex_s16_mag().
 * 
 * Computing the magnitudes of elements of a complex block-floating point vector can be represented as 
 * 
 * @math{
 *      a_k \cdot 2^{a\_exp} \leftarrow \left| b_k \cdot 2^{b\_exp} \right|
 * }
 * 
 * where each @math{a\_k} is an element of @vector{a}, the output mantissa vector, and each @math{b_k} is an element of
 * @vector{b}, the input mantissa vector, and @math{b\_exp} and @math{a\_exp} are the input and output exponents 
 * respectively.
 * 
 * Using @math{b\_exp} and @math{b\_hr} (the headroom of mantissa vector @vector{b}) this function computes 
 * @math{a\_exp}, as well as @math{b\_shr}, a parameter required by xs3_vect_complex_s32_mag(), which computes the 
 * output mantissa vector @vector{a}.
 * 
 * To maximize precision, the output exponent @math{a\_exp} should be chosen such that the output vector @vector{a} has
 * as little headroom as possible.
 * 
 * This function is used calculate the output exponent and input bit-shift for each of the following functions:
 * * xs3_vect_complex_s32_mag()
 * * xs3_vect_complex_s16_mag()
 * 
 * @par Adjusting Output Exponents
 * 
 * If a specific output exponent `desired_exp` is needed for the result (e.g. for emulating fixed-point arithmetic), the 
 * `b_shr` produced by this function can be adjusted according to the following:
 * \code{.c}
 *      exponent_t desired_exp = ...; // Value known a priori
 *      right_shift_t new_b_shr = b_shr + (desired_exp - a_exp);
 * \endcode
 * 
 * When applying the above adjustment, the following condition should be maintained:
 * * `b_hr + b_shr >= 0`
 * 
 * Using larger values than strictly necessary for `b_shr` may result in unnecessary underflows or loss of precision.
 * 
 * @par Notes
 * 
 * * If @math{b\_hr} is unknown, it can be calculated using the appropriate headroom function (e.g. 
 * xs3_vect_complex_s32_headroom()) or the value `0` can always be safely used (but may result in reduced precision).
 * 
 * @param[out]  a_exp               Output exponent associated with output mantissa vector @vector{a}
 * @param[out]  b_shr               Signed arithmetic right-shift to be applied to elements of @vector{b}. Used by the
 *                                  function which computes the output mantissas @vector{a}
 * @param[in]   b_exp               Exponent associated with input mantissa vector @vector{b}
 * @param[in]   b_hr                Headroom of input mantissa vector @vector{b}
 * 
 * @see xs3_vect_complex_s16_mag()
 * @see xs3_vect_complex_s32_mag()
 */
C_API
void xs3_vect_complex_mag_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    const exponent_t b_exp,
    const headroom_t b_hr);


/**
 * @brief Add one complex 32-bit vector to another.
 * 
 * `a[]`, `b[]` and `c[]` represent the complex 32-bit mantissa vectors @vector{a}, @vector{b} and @vector{c}  
 * respectively. Each must begin at a word-aligned address. This operation can be performed safely in-place on `b[]` or
 * `c[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b} and @vector{c} 
 * respectively.
 * 
 * @low_op{32, @f$ 
 *      b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
 *      c_k' \leftarrow sat_{32}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)     \\
 *      Re\\{a_k\\} \leftarrow Re\\{b_k'\\} + Re\\{c_k'\\}                  \\
*       Im\\{a_k\\} \leftarrow Im\\{b_k'\\} + Im\\{c_k'\\}                  \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} and @vector{c} are the complex 32-bit mantissas of BFP vectors @math{ \bar{b} \cdot 2^{b\_exp} } and 
 * @math{\bar{c} \cdot 2^{c\_exp}}, then the resulting vector @vector{a} are the complex 32-bit mantissas of BFP vector 
 * @math{\bar{a} \cdot 2^{a\_exp}}. 
 * 
 * In this case, @math{b\_shr} and @math{c\_shr} **must** be chosen so that 
 * @math{a\_exp = b\_exp + b\_shr = c\_exp + c\_shr}. Adding or subtracting mantissas only makes sense if they
 * are associated with the same exponent.
 * 
 * The function xs3_vect_add_sub_prepare() can be used to obtain values for @math{a\_exp}, @math{b\_shr} and 
 * @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and the input headrooms @math{b\_hr} and 
 * @math{c\_hr}.
 * 
 * @param[out]      a_real      Real part of complex output vector @vector{a}
 * @param[out]      a_imag      Imaginary aprt of complex output vector @vector{a}
 * @param[in]       b_real      Real part of complex input vector @vector{b}
 * @param[in]       b_imag      Imaginary part of complex input vector @vector{b}
 * @param[in]       c_real      Real part of complex input vector @vector{c}
 * @param[in]       c_imag      Imaginary part of complex input vector @vector{c} 
 * @param[in]       length      Number of elements in vectors @vector{a}, @vector{b} and @vector{c}
 * @param[in]       b_shr       Right-shift applied to @vector{b}
 * @param[in]       c_shr       Right-shift applied to @vector{c}
 * 
 * @returns     Headroom of output vector @vector{a}.
 * 
 * @see xs3_vect_add_sub_prepare
 */
C_API
headroom_t xs3_vect_complex_s32_add(
    complex_s32_t a[],
    const complex_s32_t b[],
    const complex_s32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Multiply one complex 32-bit vector element-wise by the complex conjugate of another.
 * 
 * `a[]`, `b[]` and `c[]` represent the 32-bit mantissa vectors @vector{a}, @vector{b} and @vector{c} respectively. 
 * Each must begin at a word-aligned address. This operation can be performed safely in-place on `b[]` or `c[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b} and @vector{c} 
 * respectively.
 * 
 * @low_op{32, @f$ 
 *      b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)                     \\
 *      c_k' \leftarrow sat_{32}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)                     \\
 *      Re\\{a_k\\} \leftarrow \left( Re\\{b_k'\\} \cdot Re\\{c_k'\\} 
 *                                  + Im\\{b_k'\\} \cdot Im\\{c_k'\\} \right) \cdot 2^{-30} \\
 *      Im\\{a_k\\} \leftarrow \left( Im\\{b_k'\\} \cdot Re\\{c_k'\\} 
 *                                  - Re\\{b_k'\\} \cdot Im\\{c_k'\\} \right) \cdot 2^{-30} \\
 *      \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the complex 32-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} } and @math{c} is the 
 * complex 32-bit mantissa of floating-point value @math{c \cdot 2^{c\_exp}, then the resulting vector @vector{a} are 
 * the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp + c\_exp + a\_shr}.
 * 
 * The function xs3_vect_complex_s32_mul_prepare() can be used to obtain values for @math{a\_exp} and @math{a\_shr} 
 * based on the input exponents @math{b\_exp} and @math{c\_exp} and the input headrooms @math{b\_hr} and @math{c\_hr}. 
 * 
 * @param[out]      a_real      Real part of complex output vector @vector{a}
 * @param[out]      a_imag      Imaginary aprt of complex output vector @vector{a}
 * @param[in]       b_real      Real part of complex input vector @vector{b}
 * @param[in]       b_imag      Imaginary part of complex input vector @vector{b}
 * @param[in]       c_real      Real part of complex input vector @vector{c}
 * @param[in]       c_imag      Imaginary part of complex input vector @vector{c} 
 * @param[in]       length      Number of elements in vectors @vector{a}, @vector{b} and @vector{c}
 * @param[in]       a_shr       Right-shift applied to 32-bit intermediate results.
 * 
 * @return      Headroom of the output vector @vector{a}
 * 
 * @see xs3_vect_complex_s32_mul_prepare
 */
C_API
headroom_t xs3_vect_complex_s32_conj_mul(
    complex_s32_t a[],
    const complex_s32_t b[],
    const complex_s32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Calculate the headroom of a complex 32-bit array.
 * 
 * The headroom of an N-bit integer is the number of bits that the integer's value may be left-shifted
 * without any information being lost. Equivalently, it is one less than the number of leading sign bits.
 * 
 * The headroom of a `complex_s32_t` struct is the minimum of the headroom of each of its 32-bit fields,
 * `re` and `im`.
 * 
 * The headroom of a `complex_s32_t` array is the minimum of the headroom of each of its `complex_s32_t`
 * elements.
 * 
 * This function efficiently traverses the elements of @vector{x} to determine its headroom.
 * 
 * `x[]` represents the complex 32-bit vector @vector{x}. `x[]` must begin at a word-aligned address.
 * 
 * `length` is the number of elements in `x[]`.
 * 
 * @low_op{32, @f$
 *      min\!\\{ HR_{32}\left(x_0\right), HR_{32}\left(x_1\right), ..., HR_{32}\left(x_{length-1}\right) \\}
 * @f$ }
 * 
 * @param[in]   x       Complex input vector @vector{x}
 * @param[in]   length  Number of elements in @vector{x}
 * 
 * @returns     Headroom of vector @vector{x} 
 * 
 * @see xs3_vect_ch_pair_s16_headroom
 * @see xs3_vect_ch_pair_s32_headroom
 * @see xs3_vect_s16_headroom
 * @see xs3_vect_s32_headroom
 * @see xs3_vect_complex_s16_headroom
 */
C_API
headroom_t xs3_vect_complex_s32_headroom(
    const complex_s32_t x[], 
    const unsigned length);


/**
 * @brief Compute the magnitude of each element of a complex 32-bit vector.
 * 
 * `a[]` represents the real 32-bit output mantissa vector @vector{a}.
 * 
 * `b[]` represents the complex 32-bit input mantissa vector @vector{b}.
 * 
 * `a[]` and `b[]` must each begin at a word-aligned address.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` is the signed arithmetic right-shift applied to elements of @vector{b}.
 * 
 * `rot_table` must point to a pre-computed table of complex vectors used in calculating the magnitudes. `table_rows` is
 * the number of rows in the table. This library is distributed with a default version of the required rotation table. 
 * The following symbols can be used to refer to it in user code:
 * 
 * @code
 *     const extern unsigned rot_table32_rows;
 *     const extern complex_s32_t rot_table32[30][4];
 * @endcode
 * 
 * Faster computation (with reduced precision) can be achieved by generating a smaller version of the table. A python
 * script is provided to generate this table.
 * @todo Point to documentation page on generating this table.
 * 
 * @low_op{32, @f$ 
 *      v_k \leftarrow b_k \cdot 2^{-b\_shr}    \\
 *      a_k \leftarrow \sqrt { {\left( Re\\{v_k\\} \right)}^2 + {\left( Im\\{v_k\\} \right)}^2 }
 *        \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the complex 32-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} }, then the resulting 
 * vector @vector{a} are the real 32-bit mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where 
 * @math{a\_exp = b\_exp + b\_shr}.
 * 
 * The function xs3_vect_complex_mag_prepare() can be used to obtain values for @math{a\_exp} and @math{b\_shr} based on 
 * the input exponent @math{b\_exp} and headroom @math{b\_hr}.
 * 
 * @param[out]  a           Real output vector @vector{a}
 * @param[in]   b           Complex input vector @vector{b}
 * @param[in]   length      Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]   b_shr       Right-shift appled to @vector{b}
 * @param[in]   rot_table   Pre-computed rotation table required for calculating magnitudes
 * @param[in]   table_rows  Number of rows in `rot_table`
 * 
 * @returns     Headroom of the output vector @vector{a}.
 * 
 * @see xs3_vect_complex_mag_prepare
 */
C_API
headroom_t xs3_vect_complex_s32_mag(
    int32_t a[],
    const complex_s32_t b[],
    const unsigned length,
    const right_shift_t b_shr,
    const complex_s32_t* rot_table,
    const unsigned table_rows);


/**
 * @brief Obtain the output exponent and input shift used by xs3_vect_complex_s32_mag() and xs3_vect_complex_s16_mag().
 * 
 * This function is used in conjunction with xs3_vect_complex_s32_mag() to compute the magnitude of each element of a
 * complex 32-bit BFP vector.
 * 
 * This function computes `a_exp` and `b_shr`.
 * 
 * `a_exp` is the exponent associated with mantissa vector @vector{a}, and is be chosen to maximize precision when
 * elements of @vector{a} are computed. The `a_exp` chosen by this function is derived from the exponent and headroom
 * associated with the input vector.
 * 
 * `b_shr` is the shift parameter required by xs3_vect_complex_s32_mag() to achieve the chosen output exponent `a_exp`.
 * 
 * `b_exp` is the exponent associated with the input mantissa vector @vector{b}.
 * 
 * `b_hr` is the headroom of @vector{b}. If the headroom of @vector{b} is unknown it can be calculated using 
 * xs3_vect_complex_s32_headroom(). Alternatively, the value `0` can always be safely used (but may result in reduced
 * precision).
 * 
 * @par Adjusting Output Exponents
 * 
 * If a specific output exponent `desired_exp` is needed for the result (e.g. for emulating fixed-point arithmetic), the 
 * `b_shr` produced by this function can be adjusted according to the following:
 * \code{.c}
 *      exponent_t desired_exp = ...; // Value known a priori
 *      right_shift_t new_b_shr = b_shr + (desired_exp - a_exp);
 * \endcode
 * 
 * When applying the above adjustment, the following condition should be maintained:
 * * `b_hr + b_shr >= 0`
 * 
 * Using larger values than strictly necessary for `b_shr` may result in unnecessary underflows or loss of precision.
 * 
 * @param[out]  a_exp               Output exponent associated with output mantissa vector @vector{a}
 * @param[out]  b_shr               Signed arithmetic right-shift for @vector{b} used by xs3_vect_complex_s32_mag()
 * @param[in]   b_exp               Exponent associated with input mantissa vector @vector{b}
 * @param[in]   b_hr                Headroom of input mantissa vector @vector{b}
 * 
 * @see xs3_vect_complex_s32_mag()
 */
C_API
void xs3_vect_complex_s32_mag_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    const exponent_t b_exp,
    const headroom_t b_hr);


/**
 * @brief Multiply one complex 32-bit vector element-wise by another.
 * 
 * `a[]`, `b[]` and `c[]` represent the 32-bit mantissa vectors @vector{a}, @vector{b} and @vector{c} respectively. 
 * Each must begin at a word-aligned address. This operation can be performed safely in-place on `b[]` or `c[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b} and @vector{c} 
 * respectively.
 * 
 * @low_op{32, @f$ 
 *      b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)                     \\
 *      c_k' \leftarrow sat_{32}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)                     \\
 *      Re\\{a_k\\} \leftarrow \left( Re\\{b_k'\\} \cdot Re\\{c_k'\\} 
 *                                  - Im\\{b_k'\\} \cdot Im\\{c_k'\\} \right) \cdot 2^{-30} \\
 *      Im\\{a_k\\} \leftarrow \left( Im\\{b_k'\\} \cdot Re\\{c_k'\\} 
 *                                  + Re\\{b_k'\\} \cdot Im\\{c_k'\\} \right) \cdot 2^{-30} \\
 *      \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the complex 32-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} } and @math{c} is the 
 * complex 32-bit mantissa of floating-point value @math{c \cdot 2^{c\_exp}, then the resulting vector @vector{a} are 
 * the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp + c\_exp + b\_shr + c\_shr}.
 * 
 * The function xs3_vect_complex_s32_mul_prepare() can be used to obtain values for @math{a\_exp}, @math{b\_shr} and 
 * @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and the input headrooms @math{b\_hr} and 
 * @math{c\_hr}.
 * 
 * @param[out] a        Complex output vector @vector{a}
 * @param[in]  b        Complex input vector @vector{b}
 * @param[in]  c        Complex input vector @vector{c}
 * @param[in]  length   Number of elements in vectors @vector{a}, @vector{b}, and @vector{c}
 * @param[in]  b_shr    Right-shift appled to @vector{b}
 * @param[in]  c_shr    Right-shift appled to @vector{c}
 * 
 * @return      Headroom of the output vector @vector{a}
 * 
 * @see xs3_vect_complex_s32_mul_prepare
 */
C_API
headroom_t xs3_vect_complex_s32_mul(
    complex_s32_t a[],
    const complex_s32_t b[],
    const complex_s32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Obtain the output exponent and input shifts used by xs3_vect_complex_s32_mul() and 
 * xs3_vect_complex_s32_conj_mul().
 * 
 * This function is used in conjunction with xs3_vect_complex_s32_mul() to perform a complex element-wise multiplication
 * of two complex 32-bit BFP vectors.
 * 
 * This function computes `a_exp`, `b_shr` and `c_shr`.
 * 
 * `a_exp` is the exponent associated with mantissa vector @vector{a}, and must be chosen to be large enough to avoid 
 * overflow when elements of @vector{a} are computed. To maximize precision, this function chooses `a_exp` to be the 
 * smallest exponent known to avoid saturation (see exception below). The `a_exp` chosen by this function is derived 
 * from the exponents and headrooms of associated with the input vectors.
 * 
 * `b_shr` and `c_shr` are the shift parameters required by xs3_vect_complex_s32_mul() to achieve the chosen output 
 * exponent `a_exp`.
 * 
 * `b_exp` and `c_exp` are the exponents associated with the input mantissa vectors @vector{b} and @vector{c} 
 * respectively.
 * 
 * `b_hr` and `c_hr` are the headroom of @vector{b} and @vector{c} respectively. If the headroom of @vector{b} or 
 * @vector{c} is unknown, they can be obtained by calling xs3_vect_complex_s32_headroom(). Alternatively, the value `0`
 * can always be safely used (but may result in reduced precision).
 * 
 * @par Adjusting Output Exponents
 * 
 * If a specific output exponent `desired_exp` is needed for the result (e.g. for emulating fixed-point arithmetic), the 
 * `b_shr` and `c_shr` produced by this function can be adjusted according to the following:
 * \code{.c}
 *      exponent_t desired_exp = ...; // Value known a priori
 *      right_shift_t new_b_shr = b_shr + (desired_exp - a_exp);
 *      right_shift_t new_c_shr = c_shr + (desired_exp - a_exp);
 * \endcode
 * 
 * When applying the above adjustment, the following conditions should be maintained:
 * * `b_hr + b_shr >= 0`
 * * `c_hr + c_shr >= 0`
 * 
 * Be aware that using smaller values than strictly necessary for `b_shr` and `c_shr` can result in saturation, and 
 * using larger values may result in unnecessary underflows or loss of precision.
 * 
 * @par Notes
 * 
 * * Using the outputs of this function, an output mantissa which would otherwise be `INT32_MIN` will instead saturate 
 *   to `-INT32_MAX`. This is due to the symmetric saturation logic employed by the VPU and is a hardware feature. This 
 *   is a corner case which is usually unlikely and results in 1 LSb of error when it occurs.
 * 
 * @param[out]  a_exp               Exponent associated with output mantissa vector @vector{a}
 * @param[out]  b_shr               Signed arithmetic right-shift for @vector{b} used by xs3_vect_complex_s32_mul()
 * @param[out]  c_shr               Signed arithmetic right-shift for @vector{c} used by xs3_vect_complex_s32_mul()
 * @param[in]   b_exp               Exponent associated with input mantissa vector @vector{b}
 * @param[in]   c_exp               Exponent associated with input mantissa vector @vector{c}
 * @param[in]   b_hr                Headroom of input mantissa vector @vector{b}
 * @param[in]   c_hr                Headroom of input mantissa vector @vector{c}
 * 
 * @see xs3_vect_complex_s32_conj_mul
 * @see xs3_vect_complex_s32_mul
 */
C_API
void xs3_vect_complex_s32_mul_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    right_shift_t* c_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr);


/**
 * @brief Multiply a complex 32-bit vector element-wise by a real 32-bit vector.
 * 
 * `a[]` and `b[]` represent the complex 32-bit mantissa vectors @vector{a} and @vector{b} respectively. 
 * 
 * `c[]` represents the real 32-bit mantissa vector @vector{c}.
 * 
 * `a[]`, `b[]`, and `c[]` each must begin at a word-aligned address. This operation can be performed safely in-place on 
 * `b[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b} and @vector{c} 
 * respectively.
 * 
 * @low_op{32, @f$ 
 *      b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)      \\
 *      c_k' \leftarrow sat_{32}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)      \\
 *      Re\\{a_k\\} \leftarrow \left( Re\\{b_k'\\} \cdot c_k' \right) \cdot 2^{-30} \\
 *      Im\\{a_k\\} \leftarrow \left( Im\\{b_k'\\} \cdot c_k' \right) \cdot 2^{-30} \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the complex 32-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} } and @math{c} is the 
 * complex 32-bit mantissa of floating-point value @math{c \cdot 2^{c\_exp}, then the resulting vector @vector{a} are 
 * the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp + c\_exp + b\_shr + c\_shr}.
 * 
 * The function xs3_vect_s32_real_mul_prepare() can be used to obtain values for @math{a\_exp}, @math{b\_shr} and 
 * @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and the input headrooms @math{b\_hr} and 
 * @math{c\_hr}. 
 * 
 * @param[out] a        Complex output vector @vector{a}.
 * @param[in]  b        Complex input vector @vector{b}.
 * @param[in]  c        Real input vector @vector{c}.
 * @param[in]  length   Number of elements in vectors @vector{a}, @vector{b}, and @vector{c}.
 * @param[in]  b_shr    Right-shift appled to @vector{b}.
 * @param[in]  c_shr    Right-shift appled to @vector{c}.
 * 
 * @returns     Headroom of the output vector @vector{a}.
 * 
 * @see xs3_vect_complex_s32_real_mul_prepare
 */
C_API
headroom_t xs3_vect_complex_s32_real_mul(
    complex_s32_t a[],
    const complex_s32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Obtain the output exponent and input shifts used by xs3_vect_complex_s32_real_mul().
 * 
 * This function is used in conjunction with xs3_vect_complex_s32_real_mul() to perform a the element-wise 
 * multiplication of complex 32-bit BFP vector by a real 32-bit BFP vector.
 * 
 * This function computes `a_exp`, `b_shr` and `c_shr`.
 * 
 * `a_exp` is the exponent associated with mantissa vector @vector{a}, and must be chosen to be large enough to avoid 
 * overflow when elements of @vector{a} are computed. To maximize precision, this function chooses `a_exp` to be the 
 * smallest exponent known to avoid saturation (see exception below). The `a_exp` chosen by this function is derived 
 * from the exponents and headrooms of associated with the input vectors.
 * 
 * `b_shr` and `c_shr` are the shift parameters required by xs3_vect_complex_s32_mul() to achieve the chosen output 
 * exponent `a_exp`.
 * 
 * `b_exp` and `c_exp` are the exponents associated with the input mantissa vectors @vector{b} and @vector{c} 
 * respectively.
 * 
 * `b_hr` and `c_hr` are the headroom of @vector{b} and @vector{c} respectively. If the headroom of @vector{b} or 
 * @vector{c} is unknown, they can be obtained by calling xs3_vect_complex_s32_headroom(). Alternatively, the value `0`
 * can always be safely used (but may result in reduced precision).
 * 
 * @par Adjusting Output Exponents
 * 
 * If a specific output exponent `desired_exp` is needed for the result (e.g. for emulating fixed-point arithmetic), the 
 * `b_shr` and `c_shr` produced by this function can be adjusted according to the following:
 * \code{.c}
 *      exponent_t desired_exp = ...; // Value known a priori
 *      right_shift_t new_b_shr = b_shr + (desired_exp - a_exp);
 *      right_shift_t new_c_shr = c_shr + (desired_exp - a_exp);
 * \endcode
 * 
 * When applying the above adjustment, the following conditions should be maintained:
 * * `b_hr + b_shr >= 0`
 * * `c_hr + c_shr >= 0`
 * 
 * Be aware that using smaller values than strictly necessary for `b_shr` and `c_shr` can result in saturation, and 
 * using larger values may result in unnecessary underflows or loss of precision.
 * 
 * @par Notes
 * 
 * * Using the outputs of this function, an output mantissa which would otherwise be `INT32_MIN` will instead saturate 
 *   to `-INT32_MAX`. This is due to the symmetric saturation logic employed by the VPU and is a hardware feature. This 
 *   is a corner case which is usually unlikely and results in 1 LSb of error when it occurs.
 * 
 * @param[out]  a_exp               Output exponent associated with @vector{a}
 * @param[out]  b_shr               Signed arithmetic right-shift for @vector{b} used by xs3_vect_complex_s32_real_mul()
 * @param[out]  c_shr               Signed arithmetic right-shift for @vector{c} used by xs3_vect_complex_s32_real_mul()
 * @param[in]   b_exp               Exponent associated with @vector{b}
 * @param[in]   c_exp               Exponent associated with @vector{c}
 * @param[in]   b_hr                Headroom of mantissa vector @vector{b}
 * @param[in]   c_hr                Headroom of mantissa vector @vector{c}
 * 
 * @see xs3_vect_complex_s32_real_mul
 */
C_API
void xs3_vect_complex_s32_real_mul_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    right_shift_t* c_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr);


/**
 * @brief Multiply a complex 32-bit vector by a real scalar.
 * 
 * `a[]` and `b[]` represent the complex 32-bit mantissa vectors @vector{a} and @vector{b} respectively. 
 * 
 * `c` represents the real 32-bit scale factor @math{c}.
 * 
 * `a[]` and `b[]` each must begin at a word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` and `c_shr` are the signed arithmetic right-shift applied to each element of @vector{b} and to @math{c}.
 * 
 * @low_op{32, @f$
 *      b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
 *      Re\\{a_k\\} \leftarrow Re\\{b_k'\\} \cdot c                         \\
 *      Im\\{a_k\\} \leftarrow Im\\{b_k'\\} \cdot c                         \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the complex 16-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} } and @math{c} is the 
 * complex 16-bit mantissa of floating-point value @math{c \cdot 2^{c\_exp}, then the resulting vector @vector{a} are 
 * the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp + c\_exp + b\_shr + c\_shr}.
 * 
 * The function xs3_vect_s16_real_mul_prepare() can be used to obtain values for @math{a\_exp}, @math{b\_shr} and 
 * @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and the input headrooms @math{b\_hr} and 
 * @math{c\_hr}. 
 * 
 * @param[out]  a           Complex output vector @vector{a}
 * @param[in]   b           Complex input vector @vector{b}
 * @param[in]   c           Complex input vector @vector{c}
 * @param[in]   length      Number of elements in vectors @vector{a}, @vector{b}, and @vector{c}
 * @param[in]   b_shr       Right-shift applied to @vector{b}
 * @param[in]   c_shr       Right-shift applied to @math{c}
 * 
 * @returns     Headroom of the output vector @vector{a}.
 */
C_API
headroom_t xs3_vect_complex_s32_real_scale(
    complex_s32_t a[],
    const complex_s32_t b[],
    const int32_t c,
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Multiply a complex 32-bit vector by a complex 32-bit scalar.
 * 
 * `a[]` and `b[]` represent the complex 32-bit mantissa vectors @vector{a} and @vector{b} respectively. 
 * 
 * `c` represents the complex 32-bit scale factor @math{c}.
 * 
 * `a[]` and `b[]` each must begin at a word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b} and to @math{c}.
 * 
 * @low_op{32, @f$ 
 *      b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)                     \\
 *      Re\\{a_k\\} \leftarrow \left( Re\\{v_k\\} \cdot Re\\{c\\} 
 *                                  - Im\\{v_k\\} \cdot Im\\{c\\} \right) \cdot 2^{-30}     \\
 *      Im\\{a_k\\} \leftarrow \left( Re\\{v_k\\} \cdot Im\\{c\\} 
 *                                  + Im\\{v_k\\} \cdot Re\\{c\\} \right) \cdot 2^{-30}     \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the complex 32-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} } and @math{c} is the 
 * complex 32-bit mantissa of floating-point value @math{c \cdot 2^{c\_exp}, then the resulting vector @vector{a} are 
 * the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp + c\_exp + b\_shr + c\_shr}.
 * 
 * The function xs3_vect_complex_s32_mul_prepare() can be used to obtain values for @math{a\_exp}, @math{b\_shr} and 
 * @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and the input headrooms @math{b\_hr} and 
 * @math{c\_hr}. 
 * 
 * @param[out] a        Complex output vector @vector{a}.
 * @param[in]  b        Complex input vector @vector{b}.
 * @param[in]  c_real   Real part of @math{c}
 * @param[in]  c_imag   Imaginary part of @math{c}
 * @param[in]  length   Number of elements in vectors @vector{a} and @vector{b}.
 * @param[in]  b_shr    Right-shift appled to @vector{b}.
 * @param[in]  c_shr    Right-shift applied to @math{c}.
 * 
 * @returns     Headroom of the output vector @vector{a}.
 */
C_API
headroom_t xs3_vect_complex_s32_scale(
    complex_s32_t a[],
    const complex_s32_t b[],
    const int32_t c_real,
    const int32_t c_imag,
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Obtain the output exponent and input shifts used by xs3_vect_complex_s32_scale().
 * 
 * This function is used in conjunction with xs3_vect_complex_s32_scale() to perform a complex multiplication of a 
 * complex 32-bit BFP vector by a complex 32-bit scalar.
 * 
 * This function computes `a_exp`, `b_shr` and `c_shr`.
 * 
 * `a_exp` is the exponent associated with mantissa vector @vector{a}, and must be chosen to be large enough to avoid 
 * overflow when elements of @vector{a} are computed. To maximize precision, this function chooses `a_exp` to be the 
 * smallest exponent known to avoid saturation (see exception below). The `a_exp` chosen by this function is derived 
 * from the exponents and headrooms associated with the input vectors.
 * 
 * `b_shr` and `c_shr` are the shift parameters required by xs3_vect_complex_s32_mul() to achieve the chosen output 
 * exponent `a_exp`.
 * 
 * `b_exp` and `c_exp` are the exponents associated with the input mantissa vectors @vector{b} and @vector{c} 
 * respectively.
 * 
 * `b_hr` and `c_hr` are the headroom of @vector{b} and @vector{c} respectively. If the headroom of @vector{b} or 
 * @vector{c} is unknown, they can be obtained by calling xs3_vect_complex_s32_headroom(). Alternatively, the value `0`
 * can always be safely used (but may result in reduced precision).
 * 
 * @par Adjusting Output Exponents
 * 
 * If a specific output exponent `desired_exp` is needed for the result (e.g. for emulating fixed-point arithmetic), the 
 * `b_shr` and `c_shr` produced by this function can be adjusted according to the following:
 * \code{.c}
 *      exponent_t desired_exp = ...; // Value known a priori
 *      right_shift_t new_b_shr = b_shr + (desired_exp - a_exp);
 *      right_shift_t new_c_shr = c_shr + (desired_exp - a_exp);
 * \endcode
 * 
 * When applying the above adjustment, the following conditions should be maintained:
 * * `b_hr + b_shr >= 0`
 * * `c_hr + c_shr >= 0`
 * 
 * Be aware that using smaller values than strictly necessary for `b_shr` and `c_shr` can result in saturation, and 
 * using larger values may result in unnecessary underflows or loss of precision.
 * 
 * @par Notes
 * 
 * * Using the outputs of this function, an output mantissa which would otherwise be `INT32_MIN` will instead saturate 
 *   to `-INT32_MAX`. This is due to the symmetric saturation logic employed by the VPU and is a hardware feature. This 
 *   is a corner case which is usually unlikely and results in 1 LSb of error when it occurs.
 * 
 * @param[out]  a_exp               Exponent associated with output mantissa vector @vector{a}
 * @param[out]  b_shr               Signed arithmetic right-shift for @vector{b} used by xs3_vect_complex_s32_scale()
 * @param[out]  c_shr               Signed arithmetic right-shift for @vector{c} used by xs3_vect_complex_s32_scale()
 * @param[in]   b_exp               Exponent associated with input mantissa vector @vector{b}
 * @param[in]   c_exp               Exponent associated with input mantissa vector @vector{c}
 * @param[in]   b_hr                Headroom of input mantissa vector @vector{b}
 * @param[in]   c_hr                Headroom of input mantissa vector @vector{c}
 * 
 * @see xs3_vect_complex_s32_scale
 */
C_API
void xs3_vect_complex_s32_scale_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    right_shift_t* c_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr);


/**
 * @brief Set each element of a complex 32-bit vector to a specified value.
 * 
 * `a[]` represents a complex 32-bit vector @vector{a}. `a[]` must begin at a word-aligned address.
 * 
 * `b_real` and `b_imag` are the real and imaginary parts to which each element will be set.
 * 
 * `length` is the number of elements in `a[]`.
 * 
 * @low_op{32, @f$ 
 *      a_k \leftarrow  b\_real + j\cdot b\_imag              \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)   \\
 *          \qquad\text{ where } j^2 = -1
 *  @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @math{b} is the mantissa of floating-point value @math{b \cdot 2^{b\_exp}}, then the output vector @vector{a} are
 * the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp}.
 * 
 * @param[out]  a           Complex output vector @vector{a}
 * @param[in]   b_real      Value to set real part of elements of @vector{a} to
 * @param[in]   b_imag      Value to set imaginary part of elements of @vector{a} to
 * @param[in]   length      Number of elements in @vector{a}
 */
C_API
void xs3_vect_complex_s32_set(
    complex_s32_t a[],
    const int32_t b_real,
    const int32_t b_imag,
    const unsigned length);


/**
 * @brief Left-shift each element of a complex 32-bit vector by a specified number of bits.
 * 
 * `a[]` and `b[]` represent the complex 32-bit mantissa vectors @vector{a} and @vector{b} respectively. Each must begin 
 * at a word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in @vector{a} and @vector{b}.
 * 
 * `b_shl` is the signed arithmetic left-shift applied to each element of @vector{b}.
 * 
 * @low_op{32, @f$
 *      Re\\{a_k\\} \leftarrow sat_{32}(\lfloor Re\\{b_k\\} \cdot 2^{b\_shl} \rfloor)     \\
 *      Im\\{a_k\\} \leftarrow sat_{32}(\lfloor Im\\{b_k\\} \cdot 2^{b\_shl} \rfloor)     \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the complex 32-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} }, then the resulting 
 * vector @vector{a} are the complex 32-bit mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where 
 * @math{\bar{a} = \bar{b} \cdot 2^{b\_shl}} and @math{a\_exp = b\_exp}.
 * 
 * @param[out]  a           Complex output vector @vector{a}
 * @param[in]   b           Complex input vector @vector{b}
 * @param[in]   length      Number of elements in vector @vector{b}
 * @param[in]   b_shl       Left-shift applied to @vector{b} 
 * 
 * @returns     Headroom of the output vector @vector{a}
 */
C_API
headroom_t xs3_vect_complex_s32_shl(
    complex_s32_t a[],
    const complex_s32_t b[],
    const unsigned length,
    const left_shift_t b_shl);


/**
 * @brief Right-shift each element of a complex 32-bit vector by a specified number of bits.
 * 
 * `a[]` and `b[]` represent the complex 32-bit mantissa vectors @vector{a} and @vector{b} respectively. Each must begin 
 * at a word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in @vector{a} and @vector{b}.
 * 
 * `b_shr` is the signed arithmetic right-shift applied to each element of @vector{b}.
 * 
 * @low_op{32, @f$
 *      Re\\{a_k\\} \leftarrow sat_{32}(\lfloor Re\\{b_k\\} \cdot 2^{-b\_shr} \rfloor)     \\
 *      Im\\{a_k\\} \leftarrow sat_{32}(\lfloor Im\\{b_k\\} \cdot 2^{-b\_shr} \rfloor)     \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the complex 32-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} }, then the resulting 
 * vector @vector{a} are the complex 32-bit mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where 
 * @math{\bar{a} = \bar{b} \cdot 2^{-b\_shr}} and @math{a\_exp = b\_exp}.
 * 
 * @param[out]  a           Complex output vector @vector{a}
 * @param[in]   b           Complex input vector @vector{b}
 * @param[in]   length      Number of elements in vector @vector{b}
 * @param[in]   b_shr       Right-shift applied to @vector{b} 
 * 
 * @returns     Headroom of the output vector @vector{a}
 */
C_API
headroom_t xs3_vect_complex_s32_shr(
    complex_s32_t a[],
    const complex_s32_t b[],
    const unsigned length,
    const right_shift_t b_shr);


/**
 * @brief Computes the squared magnitudes of elements of a complex 32-bit vector.
 * 
 * `a[]` represents the complex 32-bit mantissa vector @vector{a}. `b[]` represents the real 32-bit mantissa vector
 * @vector{b}. Each must begin at a word-aligned address.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` is the signed arithmetic right-shift appled to each element of @vector{b}.
 * 
 * @low_op{32, @f$ 
 *      b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
 *      a_k \leftarrow ((Re\\{b_k'\\})^2 + (Im\\{b_k'\\})^2)\cdot 2^{-30}   \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the complex 32-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} }, then the resulting 
 * vector @vector{a} are the real 32-bit mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where 
 * @math{a\_exp = 2 \cdot (b\_exp + b\_shr)}.
 * 
 * The function xs3_vect_complex_s32_squared_mag_prepare() can be used to obtain values for @math{a\_exp} and 
 * @math{b\_shr} based on the input exponent @math{b\_exp} and headroom @math{b\_hr}.
 * 
 * @param[out]  a           Complex output vector @vector{a}
 * @param[in]   b           Complex input vector @vector{b}
 * @param[in]   length      Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]   b_shr       Right-shift appled to @vector{b}
 * 
 * @see xs3_vect_complex_s32_squared_mag_prepare
 */
C_API
headroom_t xs3_vect_complex_s32_squared_mag(
    int32_t a[],
    const complex_s32_t b[],
    const unsigned length,
    const right_shift_t b_shr);


/**
 * @brief Obtain the output exponent and input shift used by xs3_vect_complex_s32_squared_mag().
 * 
 * This function is used in conjunction with xs3_vect_complex_s32_squared_mag() to compute the squared magnitude of each 
 * element of a complex 32-bit BFP vector.
 * 
 * This function computes `a_exp` and `b_shr`.
 * 
 * `a_exp` is the exponent associated with mantissa vector @vector{a}, and is be chosen to maximize precision when
 * elements of @vector{a} are computed. The `a_exp` chosen by this function is derived from the exponent and headroom
 * associated with the input vector.
 * 
 * `b_shr` is the shift parameter required by xs3_vect_complex_s32_mag() to achieve the chosen output exponent `a_exp`.
 * 
 * `b_exp` is the exponent associated with the input mantissa vector @vector{b}.
 * 
 * `b_hr` is the headroom of @vector{b}. If the headroom of @vector{b} is unknown it can be calculated using 
 * xs3_vect_complex_s32_headroom(). Alternatively, the value `0` can always be safely used (but may result in reduced
 * precision).
 * 
 * @par Adjusting Output Exponents
 * 
 * If a specific output exponent `desired_exp` is needed for the result (e.g. for emulating fixed-point arithmetic), the 
 * `b_shr` produced by this function can be adjusted according to the following:
 * \code{.c}
 *      exponent_t desired_exp = ...; // Value known a priori
 *      right_shift_t new_b_shr = b_shr + (desired_exp - a_exp);
 * \endcode
 * 
 * When applying the above adjustment, the following condition should be maintained:
 * * `b_hr + b_shr >= 0`
 * 
 * Using larger values than strictly necessary for `b_shr` may result in unnecessary underflows or loss of precision.
 * 
 * @param[out]  a_exp               Output exponent associated with output mantissa vector @vector{a}
 * @param[out]  b_shr               Signed arithmetic right-shift for @vector{b} used by xs3_vect_complex_s32_squared_mag()
 * @param[in]   b_exp               Exponent associated with input mantissa vector @vector{b}
 * @param[in]   b_hr                Headroom of input mantissa vector @vector{b}
 * 
 * @see xs3_vect_complex_s32_squared_mag()
 */
C_API
void xs3_vect_complex_s32_squared_mag_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    const exponent_t b_exp,
    const headroom_t b_hr);


/**
 * @brief Subtract one complex 32-bit vector from another.
 * 
 * `a[]`, `b[]` and `c[]` represent the complex 32-bit mantissa vectors @vector{a}, @vector{b} and @vector{c}  
 * respectively. Each must begin at a word-aligned address. This operation can be performed safely in-place on `b[]` or
 * `c[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b} and @vector{c} 
 * respectively.
 * 
 * @low_op{32, @f$ 
 *      b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
 *      c_k' \leftarrow sat_{32}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)     \\
 *      Re\\{a_k\\} \leftarrow Re\\{b_k'\\} - Re\\{c_k'\\}                  \\
*       Im\\{a_k\\} \leftarrow Im\\{b_k'\\} - Im\\{c_k'\\}                  \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} and @vector{c} are the complex 32-bit mantissas of BFP vectors @math{ \bar{b} \cdot 2^{b\_exp} } and 
 * @math{\bar{c} \cdot 2^{c\_exp}}, then the resulting vector @vector{a} are the complex 32-bit mantissas of BFP vector 
 * @math{\bar{a} \cdot 2^{a\_exp}}. 
 * 
 * In this case, @math{b\_shr} and @math{c\_shr} **must** be chosen so that 
 * @math{a\_exp = b\_exp + b\_shr = c\_exp + c\_shr}. Adding or subtracting mantissas only makes sense if they
 * are associated with the same exponent.
 * 
 * The function xs3_vect_add_sub_prepare() can be used to obtain values for @math{a\_exp}, @math{b\_shr} and 
 * @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and the input headrooms @math{b\_hr} and 
 * @math{c\_hr}.
 * 
 * @param[out]      a           Complex output vector @vector{a}
 * @param[in]       b           Complex input vector @vector{b}
 * @param[in]       c           Complex input vector @vector{c}
 * @param[in]       length      Number of elements in vectors @vector{a}, @vector{b} and @vector{c}
 * @param[in]       b_shr       Right-shift applied to @vector{b}
 * @param[in]       c_shr       Right-shift applied to @vector{c}
 * 
 * @returns     Headroom of output vector @vector{a}.
 * 
 * @see xs3_vect_add_sub_prepare
 */
C_API
headroom_t xs3_vect_complex_s32_sub(
    complex_s32_t a[], 
    const complex_s32_t b[],
    const complex_s32_t c[],
    const unsigned length, 
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Compute the sum of elements of a complex 32-bit vector.
 * 
 * `a` is the complex 64-bit mantissa of the resulting sum.
 * 
 * `b[]` represents the complex 32-bit mantissa vector @vector{b}. `b[]` must begin at a word-aligned address.
 * 
 * `length` is the number of elements in @vector{b}.
 * 
 * `b_shr` is the **unsigned** arithmetic right-shift appled to each element of @vector{b}. `b_shr` _cannot_ be 
 * negative.
 * 
 * @low_op{32, @f$ 
 *      b_k' \leftarrow b_k \cdot 2^{-b\_shr}                                       \\
 *      Re\\{a\\} \leftarrow \sum_{k=0}^{length-1} \left( Re\\{b_k'\\} \right)      \\
 *      Im\\{a\\} \leftarrow \sum_{k=0}^{length-1} \left( Im\\{b_k'\\} \right)      
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then @math{a} is the complex 64-bit 
 * mantissa of floating-point value @math{a \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp + b\_shr}.
 * 
 * The function xs3_vect_complex_s32_sum_prepare() can be used to obtain values for @math{a\_exp} and @math{b\_shr} 
 * based on the input exponents @math{b\_exp} and @math{c\_exp} and the input headrooms @math{b\_hr} and @math{c\_hr}.
 * 
 * @par Additional Details
 * 
 * Internally the sum accumulates into four separate complex 40-bit accumulators. These accumulators apply symmetric 
 * 40-bit saturation logic (with bounds @math{\pm 2^{39}-1}) with each added element. At the end, the 4 accumulators are
 * summed together into the 64-bit fields of `a`. No saturation logic is applied at this final step.
 * 
 * In the most extreme case, each @math{b_k} may be @math{-2^{31}}. @math{256} of these added into the same accumulator 
 * is @math{-2^{39}} which would saturate to @math{-2^{39}+1}, introducing 1 LSb of error (which may or may not be 
 * acceptable given a particular circumstance). The final result for each part then may be as large as 
 * @math{4\cdot(-2^{39}+1) = -2^{41}+4 }, each fitting into a 42-bit signed integer.
 * 
 * @param[out]  a           Complex sum @math{a}
 * @param[in]   b           Complex input vector @vector{b}.
 * @param[in]   length      Number of elements in vector @vector{b}.
 * @param[in]   b_shr       Right-shift appled to @vector{b}.
 * 
 * @see xs3_vect_complex_s32_sum_prepare
 */
C_API
void xs3_vect_complex_s32_sum(
    complex_s64_t* a,
    const complex_s32_t b[],
    const unsigned length,
    const right_shift_t b_shr);


/**
 * @brief Obtain the output exponent and input shift used by xs3_vect_complex_s32_sum().
 * 
 * This function is used in conjunction with xs3_vect_complex_s32_sum() to compute the sum of elements of a complex
 * 32-bit BFP vector.
 * 
 * This function computes `a_exp` and `b_shr`.
 * 
 * `a_exp` is the exponent associated with the 64-bit mantissa @math{a} returned by xs3_vect_complex_s32_sum(), and must be 
 * chosen to be large enough to avoid saturation when @math{a} is computed. To maximize precision, this function chooses
 * `a_exp` to be the smallest exponent known to avoid saturation (see exception below). The `a_exp` chosen by this 
 * function is derived from the exponents and headrooms associated with the input vector.
 * 
 * `b_shr` is the shift parameter required by xs3_vect_complex_s32_sum() to achieve the chosen output exponent `a_exp`.
 * 
 * `b_exp` is the exponent associated with the input mantissa vector @vector{b}.
 * 
 * `b_hr` is the headroom of @vector{b}. If the headroom of @vector{b} is unknown it can be calculated using 
 * xs3_vect_complex_s32_headroom(). Alternatively, the value `0` can always be safely used (but may result in reduced
 * precision).
 * 
 * `length` is the number of elements in the input mantissa vector @vector{b}.
 * 
 * @par Adjusting Output Exponents
 * 
 * If a specific output exponent `desired_exp` is needed for the result (e.g. for emulating fixed-point arithmetic), the 
 * `b_shr` produced by this function can be adjusted according to the following:
 * \code{.c}
 *      exponent_t desired_exp = ...; // Value known a priori
 *      right_shift_t new_b_shr = b_shr + (desired_exp - a_exp);
 * \endcode
 * 
 * When applying the above adjustment, the following conditions should be maintained:
 * * `b_hr + b_shr >= 0`
 * 
 * Be aware that using smaller values than strictly necessary for `b_shr` can result in saturation, and using larger 
 * values may result in unnecessary underflows or loss of precision.
 * 
 * @param[out]  a_exp                   Exponent associated with output mantissa @math{a}
 * @param[out]  b_shr                   Signed arithmetic right-shift for @vector{b} used by xs3_vect_complex_s32_sum()
 * @param[in]   b_exp                   Exponent associated with input mantissa vector @vector{b}
 * @param[in]   b_hr                    Headroom of input mantissa vector @vector{b}
 * @param[in]   length                  Number of elements in @vector{b}
 * 
 * @see xs3_vect_complex_s32_sum
 */
C_API
void xs3_vect_complex_s32_sum_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    const exponent_t b_exp,
    const headroom_t b_hr,
    const unsigned length);


/**
 * @brief Reverses the order of the tail of a complex 32-bit vector.
 * 
 * Reverses the order of elements in the tail of the complex 32-bit vector @vector{x}. The tail of @vector{x}, in this 
 * context, is all elements of @vector{x} except for @math{x_0}. In other words, the first element @math{x_0} remains 
 * where it is, and the remaining @math{length-1} elements are rearranged to have their order reversed.
 * 
 * This function is used when performing a forward or inverse FFT on a single sequence of real values (i.e. the mono 
 * FFT), and operates in-place on `x[]`.
 * 
 * @par Parameter Details
 * 
 * `x[]` represents the complex 32-bit vector @vector{x}, which is both an input to and an output of this function. 
 * `x[]` must begin at a word-aligned address.
 * 
 * `length` is the number of elements in @vector{x}.
 * 
 * @low_op{32, @f$
 *      x_0 \leftarrow x_0              \\
 *      x_k \leftarrow x_{length - k}   \\
 *          \qquad\text{ for }k\in 1\ ...\ (length-1) 
 * @f$ }
 * 
 * @param[inout]    x           Complex vector to have its tail reversed.
 * @param[in]       length      Number of elements in @vector{x}
 * 
 * @see bfp_fft_forward_mono
 * @see bfp_fft_inverse_mono
 */
C_API
void xs3_vect_complex_s32_tail_reverse(
    complex_s32_t x[],
    const unsigned length);


/** 
 * @brief Convert a complex 32-bit vector into a complex 16-bit vector.
 * 
 * This function converts a complex 32-bit mantissa vector @vector{b} into a complex 16-bit mantissa vector @vector{a}.
 * Conceptually, the output BFP vector @math{\bar{a}\cdot 2^{a\_exp}} represents the same value as the input BFP vector
 * @math{\bar{b}\cdot 2^{b\_exp}}, only with a reduced bit-depth.
 * 
 * In most cases @math{b\_shr} should be @math{16 - b\_hr}, where @math{b\_hr} is the headroom of the 32-bit input
 * mantissa vector @vector{b}. The output exponent @math{a\_exp} will then be given by
 * 
 * @math{ a\_exp = b\_exp + b\_shr }
 * 
 * @par Parameter Details
 * 
 * `a_real[]` and `a_imag[]` together represent the complex 16-bit output mantissa vector @vector{a}, with the real part
 * of each @math{a_k} going in `a_real[]` and the imaginary part going in `a_imag[]`.
 * 
 * `b[]` represents the complex 32-bit mantissa vector @vector{b}.
 * 
 * `a_real[]`, `a_imag[]` and `b[]` must each begin at a word-aligned address.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` is the signed arithmetic right-shift applied to elements of @vector{b}.
 * 
 * @low_op{32, @f$ 
 *      b_k' \leftarrow sat_{16}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
 *      Re\\{a_k\\} \leftarrow Re\\{b_k'\\}                                 \\
 *      Im\\{a_k\\} \leftarrow Im\\{b_k'\\}                                 \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the complex 32-bit mantissas of a BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the resulting 
 * vector @vector{a} are the complex 16-bit mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where
 * @math{a\_exp = b\_exp + b\_shr}.
 * 
 * @param[out]  a_real      Real part of complex output vector @vector{a}.
 * @param[out]  a_imag      Imaginary part of complex output vector @vector{a}.
 * @param[in]   b           Complex input vector @vector{b}.
 * @param[in]   length      Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]   b_shr       Right-shift appled to @vector{b}.
 * 
 * @see xs3_vect_s32_to_s16
 * @see xs3_vect_complex_s16_to_complex_s32
 */
C_API
void xs3_vect_complex_s32_to_complex_s16(
    int16_t a_real[],
    int16_t a_imag[],
    const complex_s32_t b[],
    const unsigned length,
    const right_shift_t b_shr);


/** 
 * @brief Compute the element-wise absolute value of a 32-bit vector.
 * 
 * `a[]` and `b[]` represent the 32-bit vectors @vector{a} and @vector{b} respectively. Each must begin at a
 * word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * @low_op{32, @f$ 
 *      a_k \leftarrow sat_{32}(\left| b_k \right|)         \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the output vector @vector{a} are
 * the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp}.
 * 
 * @param[out] a        Output vector @vector{a}
 * @param[in]  b        Input vector @vector{b}
 * @param[in]  length   Number of elements in vectors @vector{a} and @vector{b}
 * 
 * @returns     Headroom of the output vector @vector{a}.
 */
C_API
headroom_t xs3_vect_s32_abs(
    int32_t a[],
    const int32_t b[],
    const unsigned length);


/**
 * @brief Compute the sum of the absolute values of elements of a 32-bit vector.
 * 
 * `b[]` represents the 32-bit mantissa vector @vector{b}. `b[]` must begin at a word-aligned address.
 * 
 * `length` is the number of elements in @vector{b}.
 * 
 * @low_op{32, @f$
 *      \sum_{k=0}^{length-1} sat_{32}(\left| b_k \right|)
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the returned value @math{a} is 
 * the 64-bit mantissa of floating-point value @math{a \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp}.
 * 
 * @par Additional Details
 * 
 * Internally the sum accumulates into 8 separate 40-bit accumulators. These accumulators apply symmetric 40-bit 
 * saturation logic (with bounds @math{\pm (2^{39}-1)}) with each added element. At the end, the 8 accumulators are summed
 * together into the 64-bit value @math{a} which is returned by this function. No saturation logic is applied at this
 * final step.
 * 
 * Because symmetric 32-bit saturation logic is applied when computing the absolute value, in the corner case where 
 * each element is `INT32_MIN`, each of the 8 accumulators can accumulate @math{256} elements before saturation is
 * possible. Therefore, with @math{b\_hr} bits of headroom, no saturation of intermediate results is possible with fewer 
 * than @math{2^{11 + b\_hr}} elements in @vector{b}.
 * 
 * If the length of @vector{b} is greater than @math{2^{11 + b\_hr}}, the sum can be computed piece-wise in several 
 * calls to this function, with the partial results summed in user code.
 * 
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   length      Number of elements in @vector{b}
 * 
 * @returns The 64-bit sum @math{a}
 */
C_API
int64_t xs3_vect_s32_abs_sum(
    const int32_t b[],
    const unsigned length);


/**
 * @brief Add together two 32-bit vectors.
 * 
 * `a[]`, `b[]` and `c[]` represent the 32-bit mantissa vectors @vector{a}, @vector{b} and @vector{c} respectively. 
 * Each must begin at a word-aligned address. This operation can be performed safely in-place on `b[]` or `c[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b} and @vector{c} 
 * respectively.
 * 
 * @low_op{32, @f$ 
 *      b_k' = sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)  \\
 *      c_k' = sat_{32}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)  \\
 *      a_k \leftarrow sat_{32}\!\left( b_k' + c_k' \right)     \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} and @vector{c} are the mantissas of BFP vectors @math{ \bar{b} \cdot 2^{b\_exp} } and 
 * @math{\bar{c} \cdot 2^{c\_exp}}, then the resulting vector @vector{a} are the mantissas of BFP vector 
 * @math{\bar{a} \cdot 2^{a\_exp}}. 
 * 
 * In this case, @math{b\_shr} and @math{c\_shr} **must** be chosen so that 
 * @math{a\_exp = b\_exp + b\_shr = c\_exp + c\_shr}. Adding or subtracting mantissas only makes sense if they
 * are associated with the same exponent.
 * 
 * The function xs3_vect_add_sub_prepare() can be used to obtain values for @math{a\_exp}, @math{b\_shr} and 
 * @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and the input headrooms @math{b\_hr} and 
 * @math{c\_hr}.
 * 
 * @param[out]      a           Output vector @vector{a}
 * @param[in]       b           Input vector @vector{b}
 * @param[in]       c           Input vector @vector{c}
 * @param[in]       length      Number of elements in vectors @vector{a}, @vector{b} and @vector{c}
 * @param[in]       b_shr       Right-shift appled to @vector{b}
 * @param[in]       c_shr       Right-shift appled to @vector{c}
 * 
 * @returns     Headroom of the output vector @vector{a}.
 * 
 * @see xs3_vect_add_sub_prepare
 */
C_API
headroom_t xs3_vect_s32_add(
    int32_t a[],
    const int32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Obtain the array index of the maximum element of a 32-bit vector.
 * 
 * `b[]` represents the 32-bit input vector @vector{b}. It must begin at a word-aligned address.
 * 
 * `length` is the number of elements in @vector{b}.
 * 
 * @low_op{32, @f$ 
 *      a \leftarrow argmax_k\\{ b_k \\}     \\ 
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
 * 
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   length      Number of elemetns in @vector{b}
 * 
 * @returns @math{a}, the index of the maximum element of vector @vector{b}. If there is a tie for the maximum value, 
 *          the lowest tying index is returned.
 */
C_API
unsigned xs3_vect_s32_argmax(
    const int32_t b[],
    const unsigned length);


/**
 * @brief Obtain the array index of the minimum element of a 32-bit vector.
 * 
 * `b[]` represents the 32-bit input vector @vector{b}. It must begin at a word-aligned address.
 * 
 * `length` is the number of elements in @vector{b}.
 * 
 * @low_op{32, @f$ 
 *      a \leftarrow argmin_k\\{ b_k \\}     \\ 
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
 * 
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   length      Number of elemetns in @vector{b}
 * 
 * @returns @math{a}, the index of the minimum element of vector @vector{b}. If there is a tie for the minimum value, 
 *          the lowest tying index is returned.
 */
C_API
unsigned xs3_vect_s32_argmin(
    const int32_t b[],
    const unsigned length);


/**
 * @brief Clamp the elements of a 32-bit vector to a specified range.
 * 
 * `a[]` and `b[]` represent the 32-bit vectors @vector{a} and @vector{b} respectively. Each must begin at a 
 * word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `lower_bound` and `upper_bound` are the lower and upper bounds of the clipping range respectively. These bounds are
 * checked for each element of @vector{b} only _after_ `b_shr` is applied.
 * 
 * `b_shr` is the signed arithmetic right-shift applied to elements of @vector{b} _before_ being compared to the upper
 * and lower bounds.
 * 
 * If @vector{b} are the mantissas for a BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the exponent @math{a\_exp} of
 * the output BFP vector @math{\bar{a} \cdot 2^{a\_exp}} is given by @math{a\_exp = b\_exp + b\_shr}.
 * 
 * @low_op{32, @f$
 *      b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor) \\
 *      a_k \leftarrow \begin\{cases\}
 *          lower\_bound & b_k' \le lower\_bound                        \\
 *          upper\_bound & b_k' \ge upper\_bound                        \\
 *          b_k' & otherwise \end\{cases\}                              \\
 *      \qquad\text{ for }k\in 0\ ...\ (length-1)
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the output vector @vector{a} are
 * the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp + b\_shr}.
 * 
 * @param[out]  a               Output vector @vector{a}
 * @param[in]   b               Input vector @vector{b}
 * @param[in]   length          Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]   lower_bound     Lower bound of clipping range
 * @param[in]   upper_bound     Upper bound of clipping range
 * @param[in]   b_shr           Arithmetic right-shift applied to elements of @vector{b} prior to clipping
 * 
 * @returns  Headroom of output vector @vector{a}
 */
C_API
headroom_t xs3_vect_s32_clip(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const int32_t lower_bound,
    const int32_t upper_bound,
    const right_shift_t b_shr);


/**
 * @brief Compute the inner product between two 32-bit vectors.
 * 
 * `b[]` and `c[]` represent the 32-bit mantissa vectors @vector{b} and @vector{c} respectively. Each must begin at a 
 * word-aligned address.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b} and @vector{c} 
 * respectively.
 * 
 * \low_op{32, @f$ 
 *      b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)                     \\
 *      c_k' \leftarrow sat_{32}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)                     \\
 *      a \leftarrow \sum_{k=0}^{length-1}\left(round( b_k' \cdot c_k' \cdot 2^{-30} ) \right)   \\
 *      \qquad\text{where } a \text{ is returned}
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} and @vector{c} are the mantissas of the BFP vectors @math{ \bar{b} \cdot 2^{b\_exp} } and 
 * @math{\bar{c}\cdot 2^{c\_exp}, then result @math{a} is the 64-bit mantissa of the result @math{a \cdot 2^{a\_exp}}, 
 * where @math{a\_exp = b\_exp + c\_exp + b\_shr + c\_shr + 30}.
 * 
 * If needed, the bit-depth of @math{a} can then be reduced to 32 bits to get a new result 
 * @math{a' \cdot 2^{a\_exp'}} where @math{a' = a \cdot 2^{-a\_shr}} and @math{a\_exp' = a\_exp + a\_shr}.
 * 
 * The function xs3_vect_s32_dot_prepare() can be used to obtain values for @math{a\_exp}, @math{b\_shr} and 
 * @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and the input headrooms @math{b\_hr} and 
 * @math{c\_hr}.
 * 
 * @par Additional Details
 * 
 * The 30-bit rounding right-shift applied to each of the 64-bit products @math{b_k \cdot c_k} is a feature of the 
 * hardware and cannot be avoided. As such, if the input vectors @vector{b} and @vector{c} together have too much 
 * headroom (i.e. @math{b\_hr + c\_hr}), the sum may effectively vanish. To avoid this situation, negative values of 
 * `b_shr` and `c_shr` may be used (with the stipulation that @math{b\_shr \ge -b\_hr} and @math{c\_shr \ge -c\_hr} if
 * saturation of @math{b_k'} and @math{c_k'} is to be avoided). The less headroom @math{b_k'} and @math{c_k'} have, the
 * greater the precision of the final result.
 * 
 * Internally, each product @math{(b_k' \cdot c_k' \cdot 2^{-30})} accumulates into one of eight 40-bit accumulators 
 * (which are all used simultaneously) which apply symmetric 40-bit saturation logic (with bounds @math{\approx 2^{39})
 * with each value added. The saturating arithmetic employed is _not associative_ and no indication is given if 
 * saturation occurs at an intermediate step. To avoid satuation errors, `length` should be no greater than 
 * @math{2^{10+b\_hr+c\_hr}}, where @math{b\_hr} and @math{c\_hr} are the headroom of @vector{b} and @vector{c} 
 * respectively.
 * 
 * If the caller's mantissa vectors are longer than that, the full inner product can be found by calling this function 
 * multiple times for partial inner products on sub-sequences of the input vectors, and adding the results in user code. 
 * 
 * In many situations the caller may have _a priori_ knowledge that saturation is impossible (or very nearly so), in 
 * which case this guideline may be disregarded. However, such situations are application-specific and are well beyond 
 * the scope of this documentation, and as such are left to the user's discretion.
 * 
 * @param[in] b             Input vector @vector{b}
 * @param[in] c             Input vector @vector{c}
 * @param[in] length        Number of elements in vectors @vector{b} and @vector{c}
 * @param[in] b_shr         Right-shift appled to @vector{b}
 * @param[in] c_shr         Right-shift appled to @vector{c}
 * 
 * @returns The inner product of vectors @vector{b} and @vector{c}, scaled as indicated above.
 */
C_API
int64_t xs3_vect_s32_dot(
    const int32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Obtain the output exponent and input shift used by xs3_vect_s32_dot().
 * 
 * This function is used in conjunction with xs3_vect_s32_dot() to compute the inner product of two 32-bit BFP vectors.
 * 
 * This function computes `a_exp`, `b_shr` and `c_shr`.
 * 
 * `a_exp` is the exponent associated with the 64-bit mantissa @math{a} returned by xs3_vect_s32_dot(), and must be 
 * chosen to be large enough to avoid saturation when @math{a} is computed. To maximize precision, this function chooses
 * `a_exp` to be the smallest exponent known to avoid saturation (see exception below). The `a_exp` chosen by this 
 * function is derived from the exponents and headrooms associated with the input vectors.
 * 
 * `b_shr` and `c_shr` are the shift parameters required by xs3_vect_s32_dot() to achieve the chosen output exponent 
 * `a_exp`.
 * 
 * `b_exp` and `c_exp` are the exponents associated with the input mantissa vectors @vector{b} and @vector{c} 
 * respectively.
 * 
 * `b_hr` and `c_hr` are the headroom of @vector{b} and @vector{c} respectively. If either is unknown, they can be 
 * obtained using xs3_vect_s32_headroom(). Alternatively, the value `0` can always be safely used (but may result in 
 * reduced precision).
 * 
 * `length` is the number of elements in the input mantissa vectors @vector{b} and @vector{c}.
 * 
 * @par Adjusting Output Exponents
 * 
 * If a specific output exponent `desired_exp` is needed for the result (e.g. for emulating fixed-point arithmetic), the 
 * `b_shr` and `c_shr` produced by this function can be adjusted according to the following:
 * \code{.c}
 *      exponent_t desired_exp = ...; // Value known a priori
 *      right_shift_t new_b_shr = b_shr + (desired_exp - a_exp);
 *      right_shift_t new_c_shr = c_shr + (desired_exp - a_exp);
 * \endcode
 * 
 * When applying the above adjustment, the following conditions should be maintained:
 * * `b_hr + b_shr >= 0`
 * * `c_hr + c_shr >= 0`
 * 
 * Be aware that using smaller values than strictly necessary for `b_shr` or `c_shr` can result in saturation, and using 
 * larger values may result in unnecessary underflows or loss of precision.
 * 
 * @param[out]  a_exp               Exponent associated with output mantissa @math{a}
 * @param[out]  b_shr               Signed arithmetic right-shift for @vector{b} used by xs3_vect_s32_dot()
 * @param[out]  c_shr               Signed arithmetic right-shift for @vector{c} used by xs3_vect_s32_dot()
 * @param[in]   b_exp               Exponent associated with input mantissa vector @vector{b}
 * @param[in]   c_exp               Exponent associated with input mantissa vector @vector{b}
 * @param[in]   b_hr                Headroom of input mantissa vector @vector{b}
 * @param[in]   c_hr                Headroom of input mantissa vector @vector{b}
 * @param[in]   length              Number of elements in vectors @vector{b} and @vector{c}
 * 
 * @see xs3_vect_s32_dot
 */
C_API
void xs3_vect_s32_dot_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    right_shift_t* c_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr,
    const unsigned length);


/**
 * @brief Calculate the energy (sum of squares of elements) of a 32-bit vector.
 * 
 * `b[]` represents the 32-bit mantissa vector @vector{b}. `b[]` must begin at a word-aligned address.
 * 
 * `length` is the number of elements in @vector{b}.
 * 
 * `b_shr` is the signed arithmetic right-shift applied to elements of @vector{b}.
 * 
 * @low_op{32, @f$
 *      b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)                 \\
 *      a \leftarrow \sum_{k=0}^{length-1} round((b_k')^2 \cdot 2^{-30})
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the mantissas of the BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then floating-point result is
 * @math{a \cdot 2^{a\_exp}}, where the 64-bit mantissa @math{a} is returned by this function, and 
 * @math{a\_exp = 30 + 2 \cdot (b\_exp + b\_shr) }.
 * 
 * The function xs3_vect_s32_energy_prepare() can be used to obtain values for @math{a\_exp}, @math{b\_shr} and 
 * @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and the input headrooms @math{b\_hr} and 
 * @math{c\_hr}.
 * 
 * @par Additional Details
 * 
 * The 30-bit rounding right-shift applied to each element of the 64-bit products @math{(b_k')^2} is a feature of the 
 * hardware and cannot be avoided. As such, if the input vector @vector{b} has too much headroom (i.e. 
 * @math{2\cdot b\_hr}), the sum may effectively vanish. To avoid this situation, negative values of `b_shr` may be used
 * (with the stipulation that @math{b\_shr \ge -b\_hr} if satuartion of @math{b_k'} is to be avoided). The less headroom
 * @math{b_k'} has, the greater the precision of the final result.
 * 
 * Internally, each product @math{(b_k')^2 \cdot 2^{-30}} accumulates into one of eight 40-bit accumulators (which are 
 * all used simultaneously) which apply symmetric 40-bit saturation logic (with bounds @math{\approx 2^{39}) with each
 * value added. The saturating arithmetic employed is _not associative_ and no indication is given if saturation occurs
 * at an intermediate step. To avoid saturation errors, `length` should be no greater than @math{2^{10+2\cdotb\_hr}}, 
 * where @math{b\_hr} is the headroom of @vector{b}.
 * 
 * If the caller's mantissa vector is longer than that, the full result can be found by calling this function multiple 
 * times for partial results on sub-sequences of the input, and adding the results in user code. 
 * 
 * In many situations the caller may have _a priori_ knowledge that saturation is impossible (or very nearly so), in 
 * which case this guideline may be disregarded. However, such situations are application-specific and are well beyond 
 * the scope of this documentation, and as such are left to the user's discretion.
 * 
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   length      Number of elements in @vector{b}
 * @param[in]   b_shr       Right-shift appled to @vector{b}
 * 
 * @returns 64-bit mantissa of vector @vector{b}'s energy
 */
C_API
int64_t xs3_vect_s32_energy(
    const int32_t b[],
    const unsigned length,
    const right_shift_t b_shr);


/**
 * @brief Obtain the output exponent and input shift used by xs3_vect_s32_energy().
 * 
 * This function is used in conjunction with xs3_vect_s32_energy() to compute the inner product of a 32-bit BFP vector
 * with itself.
 * 
 * This function computes `a_exp` and `b_shr`.
 * 
 * `a_exp` is the exponent associated with the 64-bit mantissa @math{a} returned by xs3_vect_s32_energy(), and must be 
 * chosen to be large enough to avoid saturation when @math{a} is computed. To maximize precision, this function chooses
 * `a_exp` to be the smallest exponent known to avoid saturation (see exception below). The `a_exp` chosen by this 
 * function is derived from the exponent and headroom associated with the input vector.
 * 
 * `b_shr` is the shift parameter required by xs3_vect_s32_energy() to achieve the chosen output exponent `a_exp`.
 * 
 * `b_exp` is the exponent associated with the input mantissa vector @vector{b}.
 * 
 * `b_hr` is the headroom of @vector{b}. If it is unknown, it can be obtained using xs3_vect_s32_headroom(). 
 * Alternatively, the value `0` can always be safely used (but may result in reduced precision).
 * 
 * `length` is the number of elements in the input mantissa vector @vector{b}.
 * 
 * @par Adjusting Output Exponents
 * 
 * If a specific output exponent `desired_exp` is needed for the result (e.g. for emulating fixed-point arithmetic), the 
 * `b_shr` produced by this function can be adjusted according to the following:
 * \code{.c}
 *      exponent_t desired_exp = ...; // Value known a priori
 *      right_shift_t new_b_shr = b_shr + (desired_exp - a_exp);
 * \endcode
 * 
 * When applying the above adjustment, the following condition should be maintained:
 * * `b_hr + b_shr >= 0`
 * 
 * Be aware that using smaller values than strictly necessary for `b_shr` can result in saturation, and using larger 
 * values may result in unnecessary underflows or loss of precision.
 * 
 * @param[out]  a_exp       Exponent of outputs of xs3_vect_s32_energy()
 * @param[out]  b_shr       Right-shift to be applied to elements of @vector{b}
 * @param[in]   length      Number of elements in vector @vector{b}
 * @param[in]   b_exp       Exponent of vector{b}
 * @param[in]   b_hr        Headroom of vector{b}
 * 
 * @see xs3_vect_s32_energy
 */
C_API
void xs3_vect_s32_energy_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    const unsigned length,
    const exponent_t b_exp,
    const headroom_t b_hr);


/**
 * @brief Calculate the headroom of a 32-bit vector.
 * 
 * The headroom of an N-bit integer is the number of bits that the integer's value may be left-shifted
 * without any information being lost. Equivalently, it is one less than the number of leading sign bits.
 * 
 * The headroom of an `int32_t` array is the minimum of the headroom of each of its `int32_t` elements.
 * 
 * This function efficiently traverses the elements of `a[]` to determine its headroom.
 * 
 * `x[]` represents the 32-bit vector @vector{x}. `x[]` must begin at a word-aligned address.
 * 
 * `length` is the number of elements in `x[]`.
 * 
 * 
 * @low_op{32, @f$
 *      min\!\\{ HR_{32}\left(x_0\right), HR_{32}\left(x_1\right), ..., HR_{32}\left(x_{length-1}\right) \\}
 * @f$ }
 * 
 * @param[in]   x           Input vector @vector{x}
 * @param[in]   length      The number of elements in `x[]`
 * 
 * @returns     Headroom of vector @vector{x} 
 * 
 * @see xs3_vect_ch_pair_s16_headroom
 * @see xs3_vect_ch_pair_s32_headroom
 * @see xs3_vect_s16_headroom
 * @see xs3_vect_complex_s16_headroom
 * @see xs3_vect_complex_s32_headroom
 */
C_API
headroom_t xs3_vect_s32_headroom(
    const int32_t x[],
    const unsigned length);


/**
 * @brief Compute the inverse of elements of a 32-bit vector.
 * 
 * `a[]` and `b[]` represent the 32-bit mantissa vectors @vector{a} and @vector{b} respectively. Each vectors must begin
 * at a word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `scale` is a scaling parameter used to maximize the precision of the result.
 * 
 * @low_op{32, @f$
 *      a_k \leftarrow \lfloor\frac{2^{scale}}{b_k}\rfloor      \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the resulting vector @vector{a}
 * are the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = scale - b\_exp}.
 * 
 * The function xs3_vect_s32_inverse_prepare() can be used to obtain values for @math{a\_exp} and @math{scale}.
 * 
 * @param[out]  a           Output vector @vector{a}
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   length      Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]   scale       Scale factor applied to dividend when computing inverse
 * 
 * @returns     Headroom of output vector @vector{a}
 * 
 * @see xs3_vect_s32_inverse_prepare
 */
C_API
headroom_t xs3_vect_s32_inverse(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const unsigned scale);


/**
 * @brief Obtain the output exponent and scale used by xs3_vect_s32_inverse().
 * 
 * This function is used in conjunction with xs3_vect_s32_inverse() to compute the inverse of elements of a 32-bit
 * BFP vector.
 * 
 * This function computes `a_exp` and `scale`.
 * 
 * `a_exp` is the exponent associated with output mantissa vector @vector{a}, and must be chosen to avoid overflow in 
 * the smallest element of the input vector, which when inverted becomes the largest output element. To maximize 
 * precision, this function chooses `a_exp` to be the smallest exponent known to avoid saturation. The `a_exp` chosen
 * by this function is derived from the exponent and smallest element of the input vector.
 * 
 * `scale` is a scaling parameter used by xs3_vect_s32_inverse() to achieve the chosen output exponent.
 * 
 * `b[]` is the input mantissa vector @vector{b}.
 * 
 * `b_exp` is the exponent associated with the input mantissa vector @vector{b}.
 * 
 * `length` is the number of elements in @vector{b}.
 * 
 * @todo In lib_dsp, the inverse function has a floor, which prevents tiny values from completely dominating the output
 *       behavior. Perhaps I should include that?
 * 
 * @param[out]  a_exp       Exponent of output vector @vector{a}
 * @param[out]  scale       Scale factor to be applied when computing inverse
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   b_exp       Exponent of @vector{b}
 * @param[in]   length      Number of elements in vector @vector{b}
 * 
 * @see xs3_vect_s32_inverse
 */
C_API
void xs3_vect_s32_inverse_prepare(
    exponent_t* a_exp,
    unsigned* scale,
    const int32_t b[],
    const exponent_t b_exp,
    const unsigned length);


/**
 * @brief Find the maximum value in a 32-bit vector.
 * 
 * `b[]` represents the 32-bit vector @vector{b}. It must begin at a word-aligned address.
 * 
 * `length` is the number of elements in @vector{b}.
 * 
 * @low_op{32, @f$ 
 *      max\\{ x_0, x_1, ..., x_{length-1} \\}
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the returned value @math{a} is 
 * the 32-bit mantissa of floating-point value @math{a \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp}.
 * 
 * @param[in]   b           Input vector @vector{b}     
 * @param[in]   length      Number of elements in @vector{b}
 * 
 * @returns     Maximum value from @vector{b}
 */
C_API
int32_t xs3_vect_s32_max(
    const int32_t b[],
    const unsigned length);


/**
 * @brief Find the minimum value in a 32-bit vector.
 * 
 * `b[]` represents the 32-bit vector @vector{b}. It must begin at a word-aligned address.
 * 
 * `length` is the number of elements in @vector{b}.
 * 
 * @low_op{32, @f$ 
 *      max\\{ x_0, x_1, ..., x_{length-1} \\}
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the returned value @math{a} is 
 * the 32-bit mantissa of floating-point value @math{a \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp}.
 * 
 * @param[in]   b           Input vector @vector{b}     
 * @param[in]   length      Number of elements in @vector{b}
 * 
 * @returns     Minimum value from @vector{b}
 */
C_API
int32_t xs3_vect_s32_min(
    const int32_t x[],
    const unsigned length);


/**
 * @brief Multiply one 32-bit vector element-wise by another.
 * 
 * `a[]`, `b[]` and `c[]` represent the 32-bit mantissa vectors @vector{a}, @vector{b} and @vector{c} respectively. 
 * Each must begin at a word-aligned address. This operation can be performed safely in-place on `b[]` or `c[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b} and @vector{c} 
 * respectively.
 * 
 * \low_op{32, @f$
 *      b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
 *      c_k' \leftarrow sat_{32}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)     \\
 *      a_k \leftarrow sat_{32}(round(b_k' \cdot c_k' \cdot 2^{-30}))       \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} and @vector{c} are the mantissas of BFP vectors @math{ \bar{b} \cdot 2^{b\_exp} } and 
 * @math{\bar{c} \cdot 2^{c\_exp}}, then the resulting vector @vector{a} are the mantissas of BFP vector 
 * @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp + c\_exp + b\_shr + c\_shr + 30}.
 * 
 * The function xs3_vect_s32_mul_prepare() can be used to obtain values for @math{a\_exp}, @math{b\_shr} and 
 * @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and the input headrooms @math{b\_hr} and 
 * @math{c\_hr}. 
 * 
 * @param[out] a        Output vector @vector{a}
 * @param[in]  b        Input vector @vector{b}
 * @param[in]  c        Input vector @vector{c}
 * @param[in]  length   Number of elements in vectors @vector{a}, @vector{b} and @vector{c}
 * @param[in]  b_shr    Right-shift appled to @vector{b}
 * @param[in]  c_shr    Right-shift appled to @vector{c}
 * 
 * @returns  Headroom of vector @vector{a}
 */
//! [xs3_vect_s32_mul]
C_API
headroom_t xs3_vect_s32_mul(
    int32_t a[],
    const int32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);
//! [xs3_vect_s32_mul]


/**
 * @brief Obtain the output exponent and input shifts used by xs3_vect_s32_mul().
 * 
 * This function is used in conjunction with xs3_vect_s32_mul() to perform an element-wise multiplication of two 32-bit
 * BFP vectors.
 * 
 * This function computes `a_exp`, `b_shr`, `c_shr`.
 * 
 * `a_exp` is the exponent associated with mantissa vector @vector{a}, and must be chosen to be large enough to avoid 
 * overflow when elements of @vector{a} are computed. To maximize precision, this function chooses `a_exp` to be the 
 * smallest exponent known to avoid saturation (see exception below). The `a_exp` chosen by this function is derived 
 * from the exponents and headrooms of associated with the input vectors.
 * 
 * `b_shr` and `c_shr` are the shift parameters required by xs3_vect_complex_s32_mul() to achieve the chosen output 
 * exponent `a_exp`.
 * 
 * `b_exp` and `c_exp` are the exponents associated with the input mantissa vectors @vector{b} and @vector{c} 
 * respectively.
 * 
 * `b_hr` and `c_hr` are the headroom of @vector{b} and @vector{c} respectively. If the headroom of @vector{b} or 
 * @vector{c} is unknown, they can be obtained by calling xs3_vect_s32_headroom(). Alternatively, the value `0` can 
 * always be safely used (but may result in reduced precision).
 * 
 * @par Adjusting Output Exponents
 * 
 * If a specific output exponent `desired_exp` is needed for the result (e.g. for emulating fixed-point arithmetic), the 
 * `b_shr` and `c_shr` produced by this function can be adjusted according to the following:
 * \code{.c}
 *      exponent_t desired_exp = ...; // Value known a priori
 *      right_shift_t new_b_shr = b_shr + (desired_exp - a_exp);
 *      right_shift_t new_c_shr = c_shr + (desired_exp - a_exp);
 * \endcode
 * 
 * When applying the above adjustment, the following conditions should be maintained:
 * * `b_hr + b_shr >= 0`
 * * `c_hr + c_shr >= 0`
 * 
 * Be aware that using smaller values than strictly necessary for `b_shr` and `c_shr` can result in saturation, and 
 * using larger values may result in unnecessary underflows or loss of precision.
 * 
 * @par Notes
 * 
 * * Using the outputs of this function, an output mantissa which would otherwise be `INT32_MIN` will instead saturate 
 *   to `-INT32_MAX`. This is due to the symmetric saturation logic employed by the VPU and is a hardware feature. This 
 *   is a corner case which is usually unlikely and results in 1 LSb of error when it occurs.
 * 
 * 
 * @param[out]  a_exp       Exponent of output elements of xs3_vect_s32_mul()
 * @param[out]  b_shr       Right-shift to be applied to elements of @vector{b}
 * @param[out]  c_shr       Right-shift to be applied to elemetns of @vector{c}
 * @param[in]   b_exp       Exponent of @vector{b}
 * @param[in]   c_exp       Exponent of @vector{c}
 * @param[in]   b_hr        Headroom of @vector{b}
 * @param[in]   c_hr        Headroom of @vector{c}
 * 
 * @see xs3_vect_s32_mul
 */
C_API
void xs3_vect_s32_mul_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    right_shift_t* c_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr);


/** 
 * @brief Rectify the elements of a 32-bit vector.
 * 
 * `a[]` and `b[]` represent the 32-bit mantissa vectors @vector{a} and @vector{b} respectively. Each must begin at a 
 * word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * \low_op{32, @f$
 *      a_k \leftarrow 
 *          \begin\{cases\}
 *              b_k & b_k \gt 0 \\ 
 *              0 & b_k \leq 0
 *      \end\{cases\}           \\
 *      \qquad\text{ for }k\in 0\ ...\ (length-1)
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the output vector @vector{a} are
 * the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp}.
 * 
 * @param[out]  a       Output vector @vector{a}
 * @param[in]   b       Input vector @vector{b}
 * @param[in]   length  Number of elements in vectors @vector{a} and @vector{b}
 * 
 * @returns  Headroom of the output vector @vector{a}
 */
C_API
headroom_t xs3_vect_s32_rect(
    int32_t a[],
    const int32_t b[],
    const unsigned length);


/**
 * @brief Multiply a 32-bit vector by a scalar.
 * 
 * `a[]` and `b[]`represent the 32-bit mantissa vectors @vector{a} and @vector{b} respectively. Each must begin at a 
 * word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `c` is the 32-bit scalar @math{c} by which each element of @vector{b} is multiplied.
 * 
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b} and to @math{c}.
 * 
 * \low_op{32, @f$
 *      b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
 *      a_k  \leftarrow sat_{32}(round(c \cdot b_k' \cdot 2^{-30}))         \\
 *           \qquad\text{ for }k\in 0\ ...\ (length-1)
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} } and @math{c} is the mantissa of
 * floating-point value @math{c \cdot 2^{c\_exp}, then the resulting vector @vector{a} are the mantissas of BFP vector 
 * @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp + c\_exp + b\_shr + c\_shr + 30}.
 * 
 * The function xs3_vect_s32_scale_prepare() can be used to obtain values for @math{a\_exp}, @math{b\_shr} and 
 * @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and the input headrooms @math{b\_hr} and 
 * @math{c\_hr}. 
 * 
 * @param[out]  a           Output vector @vector{a}
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   length      Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]   c           Scalar to be multiplied by elements of @vector{b}
 * @param[in]   b_shr       Right-shift appled to @vector{b}
 * @param[in]   c_shr       Right-shift applied to @math{c}
 * 
 * @returns  Headroom of output vector @vector{a}
 * 
 * @see xs3_vect_s32_mul_prepare
 */
C_API
headroom_t xs3_vect_s32_scale(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const int32_t c,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/** 
 * @brief Set all elements of a 32-bit vector to the specified value.
 * 
 * `a[]` represents the 32-bit output vector @vector{a}. `a[]` must begin at a word-aligned address.
 * 
 * `b` is the new value to set each element of @vector{a} to.
 * 
 * @low_op{32, @f$
 *      a_k \leftarrow b    \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @math{b} is the mantissa of floating-point value @math{b \cdot 2^{b\_exp}}, then the output vector @vector{a} are
 * the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp}.
 * 
 * @param[out]  a           Output vector @vector{a}
 * @param[in]   b           New value for the elements of @vector{a}
 * @param[in]   length      Number of elements in @vector{a}
 */
C_API
void xs3_vect_s32_set(
    int32_t a[],
    const int32_t b,
    const unsigned length);


/**
 * @brief Left-shift the elements of a 32-bit vector by a specified number of bits.
 * 
 * `a[]` and `b[]` represent the 32-bit vectors @vector{a} and @vector{b} respectively. Each must begin at a 
 * word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in vectors @vector{a} and @vector{b}.
 * 
 * `b_shl` is the signed arithmetic left-shift applied to each element of @vector{b}. 
 * 
 * @low_op{32, @f$ 
 *      a_k \leftarrow sat_{32}(\lfloor b_k \cdot 2^{b\_shl} \rfloor)       \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} }, then the resulting vector 
 * @vector{a} are the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where 
 * @math{\bar{a} = \bar{b} \cdot 2^{b\_shl}} and @math{a\_exp = b\_exp}.
 * 
 * @param[out]  a           Output vector @vector{a}
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   length      Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]   b_shl       Arithmetic left-shift applied to elements of @vector{b}
 * 
 * @returns     Headroom of output vector @vector{a}
 */
C_API
headroom_t xs3_vect_s32_shl(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const left_shift_t b_shl);


/**
 * @brief Right-shift the elements of a 32-bit vector by a specified number of bits.
 * 
 * `a[]` and `b[]` represent the 32-bit vectors @vector{a} and @vector{b} respectively. Each must begin at a 
 * word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in vectors @vector{a} and @vector{b}.
 * 
 * `b_shr` is the signed arithmetic right-shift applied to each element of @vector{b}. 
 * 
 * @low_op{32, @f$ 
 *      a_k \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)      \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} }, then the resulting vector 
 * @vector{a} are the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where 
 * @math{\bar{a} = \bar{b} \cdot 2^{-b\_shr}} and @math{a\_exp = b\_exp}.
 * 
 * @param[out]  a           Output vector @vector{a}
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   length      Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]   b_shr       Arithmetic right-shift applied to elements of @vector{b}
 * 
 * @returns     Headroom of output vector @vector{a}
 */
C_API
headroom_t xs3_vect_s32_shr(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const right_shift_t shr);


/**
 * @brief Compute the square root of elements of a 32-bit vector.
 * 
 * `a[]` and `b[]` represent the 32-bit mantissa vectors @vector{a} and @vector{b} respectively. Each vectors must begin
 * at a word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` is the signed arithmetic right-shift applied to elements of @vector{b}.
 * 
 * `depth` is the number of most significant bits to calculate of each @math{a_k}. For example, a `depth` value of 8 
 * will only compute the 8 most significant byte of the result, with the remaining 3 bytes as 0. The maximum value
 * for this parameter is `XS3_VECT_SQRT_S32_MAX_DEPTH` (31). The time cost of this operation is approximately 
 * proportional to the number of bits computed.
 * 
 * @low_op{32, @f$ 
 *      b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
 *      a_k \leftarrow \sqrt{ b_k' }                                        \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)                       \\
 *          \qquad\text{ where } sqrt() \text{ computes the first } depth \text{ bits of the square root.}
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the resulting vector @vector{a}
 * are the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = (b\_exp + b\_shr - 30)/2}.
 * 
 * Note that because exponents must be integers, that means @math{b\_exp + b\_shr} **must be even**.
 * 
 * The function xs3_vect_s32_sqrt_prepare() can be used to obtain values for @math{a\_exp} and @math{b\_shr} based on 
 * the input exponent @math{b\_exp} and headroom @math{b\_hr}.
 * 
 * @param[out]  a           Output vector @vector{a}
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   length      Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]   b_shr       Right-shift appled to @vector{b}
 * @param[in]   depth       Number of bits of each output value to compute
 * 
 * @returns     Headroom of output vector @vector{a}
 */
C_API
headroom_t xs3_vect_s32_sqrt(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const right_shift_t b_shr,
    const unsigned depth);


/**
 * @brief Obtain the output exponent and shift parameter used by xs3_vect_s32_sqrt().
 * 
 * This function is used in conjunction withx xs3_vect_s32_sqrt() to compute the square root of elements of a 32-bit
 * BFP vector.
 * 
 * This function computes `a_exp` and `b_shr`.
 * 
 * `a_exp` is the exponent associated with output mantissa vector @vector{a}, and should be chosen to maximize the 
 * precision of the results. To that end, this function chooses `a_exp` to be the smallest exponent known to avoid 
 * saturation of the resulting mantissa vector @vector{a}. It is derived from the exponent and headroom of the input
 * BFP vector.
 * 
 * `b_shr` is the shift parameter required by xs3_vect_s32_sqrt() to achieve the chosen output exponent `a_exp`.
 * 
 * `b_exp` is the exponent associated with the input mantissa vector @vector{b}.
 * 
 * `b_hr` is the headroom of @vector{b}. If it is unknown, it can be obtained using xs3_vect_s32_headroom(). 
 * Alternatively, the value `0` can always be safely used (but may result in reduced precision).
 * 
 * @par Adjusting Output Exponents
 * 
 * If a specific output exponent `desired_exp` is needed for the result (e.g. for emulating fixed-point arithmetic), the 
 * `b_shr` produced by this function can be adjusted according to the following:
 * \code{.c}
 *      exponent_t a_exp;
 *      right_shift_t b_shr;
 *      xs3_vect_s16_mul_prepare(&a_exp, &b_shr, b_exp, c_exp, b_hr, c_hr);
 *      exponent_t desired_exp = ...; // Value known a priori
 *      b_shr = b_shr + (desired_exp - a_exp);
 *      a_exp = desired_exp;
 * \endcode
 * 
 * When applying the above adjustment, the following condition should be maintained:
 * * `b_hr + b_shr >= 0`
 * 
 * Be aware that using smaller values than strictly necessary for `b_shr` can result in saturation, and using larger 
 * values may result in unnecessary underflows or loss of precision.
 * 
 * Also, if a larger exponent is used than necessary, a larger `depth` parameter (see xs3_vect_s32_sqrt()) will be 
 * required to achieve the same precision, as the results are computed bit by bit, starting with the most significant
 * bit.
 * 
 * @param[out]  a_exp       Exponent of outputs of xs3_vect_s32_sqrt()
 * @param[out]  b_shr       Right-shift to be applied to elements of @vector{b}
 * @param[in]   b_exp       Exponent of vector{b}
 * @param[in]   b_hr        Headroom of vector{b}
 * 
 * @see xs3_vect_s32_sqrt
 */
C_API
void xs3_vect_s32_sqrt_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    const exponent_t b_exp,
    const right_shift_t b_hr);


/**
 * @brief Subtract one 32-bit vector from another.
 * 
 * `a[]`, `b[]` and `c[]` represent the 32-bit mantissa vectors @vector{a}, @vector{b} and @vector{c} 
 * respectively. Each must begin at a word-aligned address. This operation can be performed safely in-place on `b[]` or
 * `c[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b} and @vector{c} 
 * respectively.
 * 
 * @low_op{32, @f$ 
 *      b_k' = sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)  \\
 *      c_k' = sat_{32}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)  \\
 *      a_k \leftarrow sat_{32}\!\left( b_k' - c_k' \right)     \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} and @vector{c} are the mantissas of BFP vectors @math{ \bar{b} \cdot 2^{b\_exp} } and 
 * @math{\bar{c} \cdot 2^{c\_exp}}, then the resulting vector @vector{a} are the mantissas of BFP vector 
 * @math{\bar{a} \cdot 2^{a\_exp}}. 
 * 
 * In this case, @math{b\_shr} and @math{c\_shr} **must** be chosen so that 
 * @math{a\_exp = b\_exp + b\_shr = c\_exp + c\_shr}. Adding or subtracting mantissas only makes sense if they
 * are associated with the same exponent.
 * 
 * The function xs3_vect_add_sub_prepare() can be used to obtain values for @math{a\_exp}, @math{b\_shr} and 
 * @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and the input headrooms @math{b\_hr} and 
 * @math{c\_hr}.
 * 
 * @param[out] a        Output vector @vector{a}
 * @param[in] b         Input vector @vector{b}
 * @param[in] c         Input vector @vector{c}
 * @param[in] length    Number of elements in vectors @vector{a}, @vector{b} and @vector{c}
 * @param[in] b_shr     Right-shift appled to @vector{b}
 * @param[in] c_shr     Right-shift appled to @vector{c}
 * 
 * @returns  Headroom of output vector @vector{a}
 * 
 * @see xs3_vect_add_sub_prepare
 */
C_API
headroom_t xs3_vect_s32_sub(
    int32_t a[],
    const int32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Sum the elements of a 32-bit vector.
 * 
 * `b[]` represents the 32-bit mantissa vector @vector{b}. `b[]` must begin at a word-aligned address.
 * 
 * `length` is the number of elements in @vector{b}.
 * 
 * \low_op{32, @f$ 
 *      a \leftarrow \sum_\{k=0\}^\{length-1\} b_k     
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the returned value @math{a} is 
 * the 64-bit mantissa of floating-point value @math{a \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp}.
 * 
 * @par Additional Details
 * 
 * Internally, each element accumulates into one of eight 40-bit accumulators (which are all used simultaneously) which 
 * apply symmetric 40-bit saturation logic (with bounds @math{\approx 2^{39}) with each value added. The saturating 
 * arithmetic employed is _not associative_ and no indication is given if saturation occurs at an intermediate step. To 
 * avoid the possibility of saturation errors, `length` should be no greater than @math{2^{11+b\_hr}}, where 
 * @math{b\_hr} is the headroom of @vector{b}.
 * 
 * If the caller's mantissa vector is longer than that, the full result can be found by calling this function multiple 
 * times for partial results on sub-sequences of the input, and adding the results in user code. 
 * 
 * In many situations the caller may have _a priori_ knowledge that saturation is impossible (or very nearly so), in 
 * which case this guideline may be disregarded. However, such situations are application-specific and are well beyond 
 * the scope of this documentation, and as such are left to the user's discretion.
 * 
 * @param[in]   b         Input vector @vector{b}
 * @param[in]   length    Number of elements in vector @vector{b}
 * 
 * @returns  64-bit mantissa of the sum, @math{a}.
 */
C_API
int64_t xs3_vect_s32_sum(
    const int32_t b[],
    const unsigned length);


/** 
 * @brief Convert a 32-bit vector to a 16-bit vector.
 * 
 * This function converts a 32-bit mantissa vector @vector{b} into a 16-bit mantissa vector @vector{a}. Conceptually,
 * the output BFP vector @math{\bar{a}\cdot 2^{a\_exp}} represents the same values as the input BFP vector
 * @math{\bar{b}\cdot 2^{b\_exp}}, only with a reduced bit-depth.
 * 
 * In most cases @math{b\_shr} should be @math{16 - b\_hr}, where @math{b\_hr} is the headroom of the 32-bit input
 * mantissa vector @vector{b}. 
 * 
 * The output exponent @math{a\_exp} will be given by
 * 
 * @math{ a\_exp = b\_exp + b\_shr }
 * 
 * @par Parameter Details
 * 
 * `a[]` represents the 16-bit output mantissa vector @vector{a}.
 * 
 * `b[]` represents the 32-bit input mantissa vector @vector{b}.
 * 
 * `a[]` and `b[]` must each begin at a word-aligned address.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` is the signed arithmetic right-shift applied to elements of @vector{b}.
 * 
 * \low_op{32, @f$
 *      a_k \leftarrow sat_{16}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)      \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the 32-bit mantissas of a BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the resulting 
 * vector @vector{a} are the 16-bit mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where
 * @math{a\_exp = b\_exp + b\_shr}.
 * 
 * @param[out]  a        Output vector @vector{a}
 * @param[in]   b        Input vector @vector{b}
 * @param[in]   length   Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]   b_shr    Right-shift appled to @vector{b}
 * 
 * @see xs3_vect_s16_to_s32
 */
C_API
void xs3_vect_s32_to_s16(
    int16_t a[],
    const int32_t b[],
    const unsigned length,
    const right_shift_t b_shr);


#ifdef __XC__
}   //extern "C"
#endif

