// Copyright 2020 XMOS LIMITED. This Software is subject to the terms of the 
// XMOS Public License: Version 1
#ifndef XS3_VECT_S16_H_
#define XS3_VECT_S16_H_

#include "xs3_math_conf.h"
#include "xs3_math_types.h"
#include "xs3_util.h"

#ifdef __XC__
extern "C" {
#endif


/**
 * @page vector_functions16 16-bit Vector Functions
 * 
 * Below is a listing of the low-level API functions provided by this library that operate on 16-bit data.
 */

/**
 * Maximum bit-depth that can be calculated by xs3_vect_s16_sqrt().
 * 
 * @sa xs3_vect_s16_sqrt
 */
#define XS3_VECT_SQRT_S16_MAX_DEPTH     (15)


/**
 * @brief Calculate the headroom of a 16-bit channel-pair vector.
 * 
 * The headroom of an N-bit integer is the number of bits that the integer's value may be left-shifted
 * without any information being lost. Equivalently, it is one less than the number of leading sign bits.
 * 
 * The headroom of a `ch_pair_s16_t` struct is the minimum of the headroom of each of its 16-bit fields,
 * `ch_a` and `ch_b`.
 * 
 * The headroom of a 16-bit channel-pair vector is the minimum of the headroom of each of its `ch_pair_s16_t`
 * elements.
 * 
 * This function efficiently traverses the elements of @vector{b} to determine its headroom.
 * 
 * `b[]` represents a 16-bit channel-pair vector @vector{b}. `b[]` must begin at a word-aligned address.
 * 
 * `length` is the number of elements in `b[]`.
 * 
 * @low_op{16, @f$
 *      a \leftarrow min\!\\{ HR_{16}\left(x_0\right), HR_{16}\left(x_1\right), ..., 
 *              HR_{16}\left(x_{length-1}\right) \\}
 * @f$ }
 * 
 * @param[in]   b       Input channel-pair vector @vector{b}
 * @param[in]   length  Number of elements in @vector{b}
 * 
 * @returns     @math{a}, the headroom of vector @vector{b}
 * 
 * @see xs3_vect_ch_pair_s32_headroom
 * @see xs3_vect_s16_headroom
 * @see xs3_vect_s32_headroom
 * @see xs3_vect_complex_s16_headroom
 * @see xs3_vect_complex_s32_headroom
 */
headroom_t xs3_vect_ch_pair_s16_headroom(
    const ch_pair_s16_t b[],
    const unsigned length);


/**
 * @brief Set all elements of a 16-bit channel-pair vector to the specified value.
 * 
 * `a[]` represents the 16-bit vector @vector{a}. It must begin at a word-aligned address.
 * 
 * `ch_a` and `ch_b` are respectively the channel A and channel B values to which each element will be set.
 * 
 * `length` is the number of elements in `a[]`.
 * 
 * @low_op{16, @f$ 
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
 * @see xs3_vect_ch_pair_s32_set
 * @see xs3_vect_s16_set
 * @see xs3_vect_s32_set
 * @see xs3_vect_complex_s16_set
 * @see xs3_vect_complex_s32_set
 */
void xs3_vect_ch_pair_s16_set(
    ch_pair_s16_t a[],
    const int16_t ch_a,
    const int16_t ch_b,
    const unsigned length);


/**
 * @brief Left-shift the elements of a 16-bit channel-pair vector by a specified number of bits.
 * 
 * `a[]` and `b[]` represent the 16-bit channel-pair vectors @vector{a} and @vector{b} respectively. Each must begin 
 * at a word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in vectors @vector{a} and @vector{b}.
 * 
 * `b_shl` is the signed arithmetic left-shift applied to each element of @vector{b}. 
 * 
 * @low_op{16, @f$ 
 *      ChA\\{a_k\\} \leftarrow sat_{16}(\lfloor ChA\\{b_k\\} \cdot {2^{b\_shl}} \rfloor) \\
 *      ChB\\{a_k\\} \leftarrow sat_{16}(\lfloor ChB\\{b_k\\} \cdot {2^{b\_shl}} \rfloor) \\
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
 * @see xs3_vect_ch_pair_s16_shr
 */
headroom_t xs3_vect_ch_pair_s16_shl(
    ch_pair_s16_t a[],
    const ch_pair_s16_t b[],
    const unsigned length,
    const left_shift_t shl);


/**
 * @brief Right-shift the elements of a 16-bit channel-pair vector by a specified number of bits.
 * 
 * `a[]` and `b[]` represent the 16-bit channel-pair vectors @vector{a} and @vector{b} respectively. Each must begin 
 * at a word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in vectors @vector{a} and @vector{b}.
 * 
 * `b_shr` is the signed arithmetic right-shift applied to each element of @vector{b}. 
 * 
 * @low_op{16, @f$ 
 *      ChA\\{a_k\\} \leftarrow sat_{16}(\lfloor ChA\\{b_k\\} \cdot {2^{-b\_shr}} \rfloor) \\
 *      ChB\\{a_k\\} \leftarrow sat_{16}(\lfloor ChB\\{b_k\\} \cdot {2^{-b\_shr}} \rfloor) \\
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
 * @see xs3_vect_ch_pair_s16_shr
 */
headroom_t xs3_vect_ch_pair_s16_shr(
    ch_pair_s16_t a[],
    const ch_pair_s16_t b[],
    const unsigned length,
    const right_shift_t shr);


/**
 * @brief Add one complex 16-bit vector to another.
 * 
 * `a_real[]` and `a_imag[]` together represent the complex 16-bit output mantissa vector @vector{a}. 
 * Each @math{Re\\{a_k\\}} is `a_real[k]`, and each @math{Im\\{a_k\\}} is `a_imag[k]`.
 * 
 * `b_real[]` and `b_imag[]` together represent the complex 16-bit input mantissa vector @vector{b}.
 * Each @math{Re\\{b_k\\}} is `b_real[k]`, and each @math{Im\\{b_k\\}} is `b_imag[k]`.
 * 
 * `c_real[]` and `c_imag[]` together represent the complex 16-bit input mantissa vector @vector{c}.
 * Each @math{Re\\{c_k\\}} is `c_real[k]`, and each @math{Im\\{c_k\\}} is `c_imag[k]`.
 * 
 * Each of the input vectors must begin at a word-aligned address. This operation can be performed safely in-place on
 * inputs `b_real[]`, `b_imag[]`, `c_real[]` and `c_imag[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b} and @vector{c} 
 * respectively.
 * 
 * @low_op{16, @f$ 
 *      b_k' \leftarrow sat_{16}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
 *      c_k' \leftarrow sat_{16}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)     \\
 *      Re\\{a_k\\} \leftarrow Re\\{b_k'\\} + Re\\{c_k'\\}                  \\
*       Im\\{a_k\\} \leftarrow Im\\{b_k'\\} + Im\\{c_k'\\}                  \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} and @vector{c} are the complex 16-bit mantissas of BFP vectors @math{ \bar{b} \cdot 2^{b\_exp} } and 
 * @math{\bar{c} \cdot 2^{c\_exp}}, then the resulting vector @vector{a} are the complex 16-bit mantissas of BFP vector 
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
headroom_t xs3_vect_complex_s16_add(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const int16_t c_real[],
    const int16_t c_imag[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Multiply one complex 16-bit vector element-wise by the complex conjugate of another.
 * 
 * `a_real[]` and `a_imag[]` together represent the complex 16-bit output mantissa vector @vector{a}. 
 * Each @math{Re\\{a_k\\}} is `a_real[k]`, and each @math{Im\\{a_k\\}} is `a_imag[k]`.
 * 
 * `b_real[]` and `b_imag[]` together represent the complex 16-bit input mantissa vector @vector{b}.
 * Each @math{Re\\{b_k\\}} is `b_real[k]`, and each @math{Im\\{b_k\\}} is `b_imag[k]`.
 * 
 * `c_real[]` and `c_imag[]` together represent the complex 16-bit input mantissa vector @vector{c}.
 * Each @math{Re\\{c_k\\}} is `c_real[k]`, and each @math{Im\\{c_k\\}} is `c_imag[k]`.
 * 
 * Each of the input vectors must begin at a word-aligned address. This operation can be performed safely in-place on
 * inputs `b_real[]`, `b_imag[]`, `c_real[]` and `c_imag[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `a_shr` is the unsigned arithmetic right-shift applied to the 32-bit accumulators holding the penultimate results.
 * 
 * @low_op{16, @f$ 
 *      v_k = \leftarrow Re\\{b_k\\} \cdot Re\\{c_k\\}
 *                     + Im\\{b_k\\} \cdot Im\\{c_k\\}                      \\
 *      s_k = \leftarrow Im\\{b_k\\} \cdot Re\\{c_k\\}
 *                     - Re\\{b_k\\} \cdot Im\\{c_k\\}                      \\
 *      Re\\{a_k\\} \leftarrow  round( sat_{16}( v_k \cdot 2^{-a\_shr} ) )  \\
 *      Im\\{a_k\\} \leftarrow  round( sat_{16}( s_k \cdot 2^{-a\_shr} ) )  \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the complex 16-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} } and @math{c} is the 
 * complex 16-bit mantissa of floating-point value @math{c \cdot 2^{c\_exp}, then the resulting vector @vector{a} are 
 * the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp + c\_exp + a\_shr}.
 * 
 * The function xs3_vect_complex_s16_mul_prepare() can be used to obtain values for @math{a\_exp} and @math{a\_shr} 
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
 * @see xs3_vect_complex_s16_mul_prepare
 */
headroom_t xs3_vect_complex_s16_conj_mul(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const int16_t c_real[],
    const int16_t c_imag[],
    const unsigned length,
    const right_shift_t a_shr);

/**
 * @brief Calculate the headroom of a complex 16-bit array.
 * 
 * The headroom of an N-bit integer is the number of bits that the integer's value may be left-shifted
 * without any information being lost. Equivalently, it is one less than the number of leading sign bits.
 * 
 * The headroom of a `complex_s16_t` struct is the minimum of the headroom of each of its 16-bit fields,
 * `re` and `im`.
 * 
 * The headroom of a `complex_s16_t` array is the minimum of the headroom of each of its `complex_s16_t`
 * elements.
 * 
 * This function efficiently traverses the elements of @vector{x} to determine its headroom.
 * 
 * `b_real[]` and `b_imag[]` together represent the complex 16-bit input mantissa vector @vector{b}.
 * 
 * `length` is the number of elements in `b_real[]` and `b_imag[]`.
 * 
 * @low_op{16, @f$
 *      min\!\\{ HR_{16}\left(x_0\right), HR_{16}\left(x_1\right), ..., HR_{16}\left(x_{length-1}\right) \\}
 * @f$ }
 * 
 * @param[in]   b_real      Real part of complex input vector @vector{b}
 * @param[in]   b_imag      Imaginary part of complex input vector @vector{b}
 * @param[in]   length      Number of elements in @vector{x}
 * 
 * @returns     Headroom of vector @vector{x} 
 * 
 * @see xs3_vect_ch_pair_s16_headroom
 * @see xs3_vect_ch_pair_s32_headroom
 * @see xs3_vect_s16_headroom
 * @see xs3_vect_s32_headroom
 * @see xs3_vect_complex_s32_headroom
 */
headroom_t xs3_vect_complex_s16_headroom(
    const int16_t b_real[],
    const int16_t b_imag[],
    const unsigned length);


/**
 * @brief Compute the magnitude of each element of a complex 16-bit vector.
 * 
 * `a[]` represents the real 16-bit output mantissa vector @vector{a}.
 * 
 * `b_real[]` and `b_imag[]` together represent the complex 16-bit input mantissa vector @vector{b}.
 * Each @math{Re\\{b_k\\}} is `b_real[k]`, and each @math{Im\\{b_k\\}} is `b_imag[k]`.
 * 
 * Each of the input vectors must begin at a word-aligned address. This operation can be performed safely in-place on
 * inputs `b_real[]` or `b_imag[]`.
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
 *     const extern unsigned rot_table16_rows;
 *     const extern complex_s16_t rot_table16[30][4];
 * @endcode
 * 
 * Faster computation (with reduced precision) can be achieved by generating a smaller version of the table. A python
 * script is provided to generate this table.
 * @todo Point to documentation page on generating this table.
 * 
 * @low_op{16, @f$ 
 *      v_k \leftarrow b_k \cdot 2^{-b\_shr}    \\
 *      a_k \leftarrow \sqrt { {\left( Re\\{v_k\\} \right)}^2 + {\left( Im\\{v_k\\} \right)}^2 }
 *        \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the complex 16-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} }, then the resulting 
 * vector @vector{a} are the real 16-bit mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where 
 * @math{a\_exp = b\_exp + b\_shr}.
 * 
 * The function xs3_vect_complex_mag_prepare() can be used to obtain values for @math{a\_exp} and @math{b\_shr} based on 
 * the input exponent @math{b\_exp} and headroom @math{b\_hr}.
 * 
 * @param[out]  a           Real output vector @vector{a}
 * @param[in]   b_real      Real part of complex input vector @vector{b}
 * @param[in]   b_imag      Imag part of complex input vector @vector{b}
 * @param[in]   length      Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]   b_shr       Right-shift appled to @vector{b}
 * @param[in]   rot_table   Pre-computed rotation table required for calculating magnitudes
 * @param[in]   table_rows  Number of rows in `rot_table`
 * 
 * @returns     Headroom of the output vector @vector{a}.
 * 
 * @see xs3_vect_complex_mag_prepare
 */
headroom_t xs3_vect_complex_s16_mag(
    int16_t a[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const unsigned length,
    const right_shift_t b_shr,
    const int16_t* rot_table,
    const unsigned table_rows);


/**
 * @brief Multiply one complex 16-bit vector element-wise by another.
 * 
 * `a_real[]` and `a_imag[]` together represent the complex 16-bit output mantissa vector @vector{a}. 
 * Each @math{Re\\{a_k\\}} is `a_real[k]`, and each @math{Im\\{a_k\\}} is `a_imag[k]`.
 * 
 * `b_real[]` and `b_imag[]` together represent the complex 16-bit input mantissa vector @vector{b}.
 * Each @math{Re\\{b_k\\}} is `b_real[k]`, and each @math{Im\\{b_k\\}} is `b_imag[k]`.
 * 
 * `c_real[]` and `c_imag[]` together represent the complex 16-bit input mantissa vector @vector{c}.
 * Each @math{Re\\{c_k\\}} is `c_real[k]`, and each @math{Im\\{c_k\\}} is `c_imag[k]`.
 * 
 * Each of the input vectors must begin at a word-aligned address. This operation can be performed safely in-place on
 * inputs `b_real[]`, `b_imag[]`, `c_real[]` and `c_imag[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `a_shr` is the unsigned arithmetic right-shift applied to the 32-bit accumulators holding intermediate results.
 * 
 * @low_op{16, @f$ 
 *      v_k = \leftarrow Re\\{b_k\\} \cdot Re\\{c_k\\}
 *                     - Im\\{b_k\\} \cdot Im\\{c_k\\}                      \\
 *      s_k = \leftarrow Im\\{b_k\\} \cdot Re\\{c_k\\}
 *                     + Re\\{b_k\\} \cdot Im\\{c_k\\}                      \\
 *      Re\\{a_k\\} \leftarrow  round( sat_{16}( v_k \cdot 2^{-a\_shr} ) )  \\
 *      Im\\{a_k\\} \leftarrow  round( sat_{16}( s_k \cdot 2^{-a\_shr} ) )  \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the complex 16-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} } and @math{c} is the 
 * complex 16-bit mantissa of floating-point value @math{c \cdot 2^{c\_exp}, then the resulting vector @vector{a} are 
 * the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp + c\_exp + a\_shr}.
 * 
 * The function xs3_vect_complex_s16_mul_prepare() can be used to obtain values for @math{a\_exp} and @math{a\_shr} 
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
 * @see xs3_vect_complex_s16_mul_prepare
 */
headroom_t xs3_vect_complex_s16_mul(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const int16_t c_real[],
    const int16_t c_imag[],
    const unsigned length,
    const right_shift_t a_shr);


/**
 * @brief Obtain the output exponent and output shift used by xs3_vect_complex_s16_mul() and 
 * xs3_vect_complex_s16_conj_mul().
 * 
 * This function is used in conjunction with xs3_vect_complex_s16_mul() to perform a complex element-wise multiplication
 * of two complex 16-bit BFP vectors.
 * 
 * This function computes `a_exp` and `a_shr`.
 * 
 * `a_exp` is the exponent associated with mantissa vector @vector{a}, and must be chosen to be large enough to avoid 
 * overflow when elements of @vector{a} are computed. To maximize precision, this function chooses `a_exp` to be the 
 * smallest exponent known to avoid saturation (see exception below). The `a_exp` chosen by this function is derived 
 * from the exponents and headrooms of associated with the input vectors.
 * 
 * `a_shr` is the shift parameter required by xs3_vect_complex_s16_mul() to achieve the chosen output exponent `a_exp`.
 * 
 * `b_exp` and `c_exp` are the exponents associated with the input mantissa vectors @vector{b} and @vector{c} 
 * respectively.
 * 
 * `b_hr` and `c_hr` are the headroom of @vector{b} and @vector{c} respectively. If the headroom of @vector{b} or 
 * @vector{c} is unknown, they can be obtained by calling xs3_vect_complex_s16_headroom(). Alternatively, the value `0`
 * can always be safely used (but may result in reduced precision).
 * 
 * @par Adjusting Output Exponents
 * 
 * If a specific output exponent `desired_exp` is needed for the result (e.g. for emulating fixed-point arithmetic), the 
 * `a_shr` and `c_shr` produced by this function can be adjusted according to the following:
 * \code{.c}
 *      exponent_t desired_exp = ...; // Value known a priori
 *      right_shift_t new_a_shr = a_shr + (desired_exp - a_exp);
 * \endcode
 * 
 * When applying the above adjustment, the following conditions should be maintained:
 * * `new_a_shr >= 0`
 * 
 * Be aware that using smaller values than strictly necessary for `a_shr` can result in saturation, and using larger 
 * values may result in unnecessary underflows or loss of precision.
 * 
 * @par Notes
 * 
 * * Using the outputs of this function, an output mantissa which would otherwise be `INT16_MIN` will instead saturate 
 *   to `-INT16_MAX`. This is due to the symmetric saturation logic employed by the VPU and is a hardware feature. This 
 *   is a corner case which is usually unlikely and results in 1 LSb of error when it occurs.
 * 
 * @param[out]  a_exp               Exponent associated with output mantissa vector @vector{a}
 * @param[out]  a_shr               Unsigned arithmetic right-shift for @vector{b} used by xs3_vect_complex_s16_mul()
 * @param[in]   b_exp               Exponent associated with input mantissa vector @vector{b}
 * @param[in]   c_exp               Exponent associated with input mantissa vector @vector{c}
 * @param[in]   b_hr                Headroom of input mantissa vector @vector{b}
 * @param[in]   c_hr                Headroom of input mantissa vector @vector{c}
 * 
 * @see xs3_vect_complex_s16_conj_mul
 * @see xs3_vect_complex_s16_mul
 */
void xs3_vect_complex_s16_mul_prepare(
    exponent_t* a_exp,
    right_shift_t* a_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr);


/**
 * @brief Multiply a complex 16-bit vector element-wise by a real 16-bit vector.
 * 
 * `a_real[]` and `a_imag[]` together represent the complex 16-bit output mantissa vector @vector{a}. 
 * Each @math{Re\\{a_k\\}} is `a_real[k]`, and each @math{Im\\{a_k\\}} is `a_imag[k]`.
 * 
 * `b_real[]` and `b_imag[]` together represent the complex 16-bit input mantissa vector @vector{b}.
 * Each @math{Re\\{b_k\\}} is `b_real[k]`, and each @math{Im\\{b_k\\}} is `b_imag[k]`.
 * 
 * `c_real[]` represents the real 16-bit input mantissa vector @vector{c}.
 * 
 * Each of the input vectors must begin at a word-aligned address. This operation can be performed safely in-place on
 * inputs `b_real[]`, `b_imag[]` and `c_real[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `a_shr` is the unsigned arithmetic right-shift applied to the 32-bit accumulators holding the penultimate results.
 * 
 * @low_op{16, @f$ 
 *      v_k = \leftarrow Re\\{b_k\\} \cdot c_k                              \\
 *      s_k = \leftarrow Im\\{b_k\\} \cdot c_k                              \\
 *      Re\\{a_k\\} \leftarrow  round( sat_{16}( v_k \cdot 2^{-a\_shr} ) )  \\
 *      Im\\{a_k\\} \leftarrow  round( sat_{16}( s_k \cdot 2^{-a\_shr} ) )  \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the complex 16-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} } and @math{c} is the 
 * complex 16-bit mantissa of floating-point value @math{c \cdot 2^{c\_exp}, then the resulting vector @vector{a} are 
 * the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp + c\_exp + a\_shr}.
 * 
 * The function xs3_vect_s16_real_mul_prepare() can be used to obtain values for @math{a\_exp} and @math{a\_shr} based 
 * on the input exponents @math{b\_exp} and @math{c\_exp} and the input headrooms @math{b\_hr} and @math{c\_hr}. 
 * 
 * @param[out]      a_real      Real part of complex output vector @vector{a}
 * @param[out]      a_imag      Imaginary aprt of complex output vector @vector{a}
 * @param[in]       b_real      Real part of complex input vector @vector{b}
 * @param[in]       b_imag      Imaginary part of complex input vector @vector{b}
 * @param[in]       c_real      Real part of complex input vector @vector{c}
 * @param[in]       length      Number of elements in vectors @vector{a}, @vector{b} and @vector{c}
 * @param[in]       a_shr       Right-shift applied to 32-bit intermediate results.
 * 
 * @returns     Headroom of the output vector @vector{a}.
 * 
 * @see xs3_vect_complex_s16_real_mul_prepare
 */
headroom_t xs3_vect_complex_s16_real_mul(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const int16_t c_real[],
    const unsigned length,
    const right_shift_t a_shr);


/**
 * @brief Obtain the output exponent and output shift used by xs3_vect_complex_s16_real_mul().
 * 
 * This function is used in conjunction with xs3_vect_complex_s16_real_mul() to perform a complex element-wise 
 * multiplication of a complex 16-bit BFP vector by a real 16-bit vector.
 * 
 * This function computes `a_exp` and `a_shr`.
 * 
 * `a_exp` is the exponent associated with mantissa vector @vector{a}, and must be chosen to be large enough to avoid 
 * overflow when elements of @vector{a} are computed. To maximize precision, this function chooses `a_exp` to be the 
 * smallest exponent known to avoid saturation (see exception below). The `a_exp` chosen by this function is derived 
 * from the exponents and headrooms of associated with the input vectors.
 * 
 * `a_shr` is the shift parameter required by xs3_vect_complex_s16_real_mul() to achieve the chosen output exponent 
 * `a_exp`.
 * 
 * `b_exp` and `c_exp` are the exponents associated with the input mantissa vectors @vector{b} and @vector{c} 
 * respectively.
 * 
 * `b_hr` and `c_hr` are the headroom of @vector{b} and @vector{c} respectively. If the headroom of @vector{b} or 
 * @vector{c} is unknown, they can be obtained by calling xs3_vect_complex_s16_headroom(). Alternatively, the value `0`
 * can always be safely used (but may result in reduced precision).
 * 
 * @par Adjusting Output Exponents
 * 
 * If a specific output exponent `desired_exp` is needed for the result (e.g. for emulating fixed-point arithmetic), the 
 * `a_shr` and `c_shr` produced by this function can be adjusted according to the following:
 * \code{.c}
 *      exponent_t desired_exp = ...; // Value known a priori
 *      right_shift_t new_a_shr = a_shr + (desired_exp - a_exp);
 * \endcode
 * 
 * When applying the above adjustment, the following conditions should be maintained:
 * * `new_a_shr >= 0`
 * 
 * Be aware that using smaller values than strictly necessary for `a_shr` can result in saturation, and using larger 
 * values may result in unnecessary underflows or loss of precision.
 * 
 * @par Notes
 * 
 * * Using the outputs of this function, an output mantissa which would otherwise be `INT16_MIN` will instead saturate 
 *   to `-INT16_MAX`. This is due to the symmetric saturation logic employed by the VPU and is a hardware feature. This 
 *   is a corner case which is usually unlikely and results in 1 LSb of error when it occurs.
 * 
 * @param[out]  a_exp               Exponent associated with output mantissa vector @vector{a}
 * @param[out]  a_shr               Unsigned arithmetic right-shift for @vector{a} used by 
 *                                  xs3_vect_complex_s16_real_mul()
 * @param[in]   b_exp               Exponent associated with input mantissa vector @vector{b}
 * @param[in]   c_exp               Exponent associated with input mantissa vector @vector{c}
 * @param[in]   b_hr                Headroom of input mantissa vector @vector{b}
 * @param[in]   c_hr                Headroom of input mantissa vector @vector{c}
 * 
 * @see xs3_vect_complex_s16_real_mul
 */
void xs3_vect_complex_s16_real_mul_prepare(
    exponent_t* a_exp,
    right_shift_t* a_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr);


/**
 * @brief Multiply a complex 16-bit vector by a real scalar.
 * 
 * `a_real[]` and `a_imag[]` together represent the complex 16-bit output mantissa vector @vector{a}. 
 * Each @math{Re\\{a_k\\}} is `a_real[k]`, and each @math{Im\\{a_k\\}} is `a_imag[k]`.
 * 
 * `b_real[]` and `b_imag[]` together represent the complex 16-bit input mantissa vector @vector{b}.
 * Each @math{Re\\{b_k\\}} is `b_real[k]`, and each @math{Im\\{b_k\\}} is `b_imag[k]`.
 * 
 * Each of the input vectors must begin at a word-aligned address. This operation can be performed safely in-place on
 * inputs `b_real[]` and `b_imag[]`.
 * 
 * `c` is the real 16-bit input mantissa @math{c}.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `a_shr` is an unsigned arithmetic right-shift applied to the 32-bit accumulators holding the penultimate results.
 * 
 * @low_op{16, @f$
 *      v_k = \leftarrow Re\\{b_k\\} \cdot c                                \\
 *      s_k = \leftarrow Im\\{b_k\\} \cdot c                                \\
 *      Re\\{a_k\\} \leftarrow  round( sat_{16}( v_k \cdot 2^{-a\_shr} ) )  \\
 *      Im\\{a_k\\} \leftarrow  round( sat_{16}( s_k \cdot 2^{-a\_shr} ) )  \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the complex 16-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} } and @math{c} is the 
 * complex 16-bit mantissa of floating-point value @math{c \cdot 2^{c\_exp}, then the resulting vector @vector{a} are 
 * the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp + c\_exp + a\_shr}.
 * 
 * The function xs3_vect_s16_real_mul_prepare() can be used to obtain values for @math{a\_exp} and @math{a\_shr} based 
 * on the input exponents @math{b\_exp} and @math{c\_exp} and the input headrooms @math{b\_hr} and @math{c\_hr}. 
 * 
 * @param[out]      a_real      Real part of complex output vector @vector{a}
 * @param[out]      a_imag      Imaginary aprt of complex output vector @vector{a}
 * @param[in]       b_real      Real part of complex input vector @vector{b}
 * @param[in]       b_imag      Imaginary part of complex input vector @vector{b}
 * @param[in]       c           Real input scalar @math{c}
 * @param[in]       length      Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]       a_shr       Right-shift applied to 32-bit intermediate results.
 * 
 * @returns     Headroom of the output vector @vector{a}.
 */
headroom_t xs3_vect_complex_s16_real_scale(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const int16_t c,
    const unsigned length,
    const right_shift_t a_shr);


/**
 * @brief Multiply a complex 16-bit vector by a complex 16-bit scalar.
 * 
 * `a_real[]` and `a_imag[]` together represent the complex 16-bit output mantissa vector @vector{a}. 
 * Each @math{Re\\{a_k\\}} is `a_real[k]`, and each @math{Im\\{a_k\\}} is `a_imag[k]`.
 * 
 * `b_real[]` and `b_imag[]` together represent the complex 16-bit input mantissa vector @vector{b}.
 * Each @math{Re\\{b_k\\}} is `b_real[k]`, and each @math{Im\\{b_k\\}} is `b_imag[k]`.
 * 
 * Each of the input vectors must begin at a word-aligned address. This operation can be performed safely in-place on
 * inputs `b_real[]` and `b_imag[]`.
 * 
 * `c_real` and `c_imag` are the real and imaginary parts of the complex 16-bit input mantissa @math{c}.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `a_shr` is the unsigned arithmetic right-shift applied to the 32-bit accumulators holding the penultimate results.
 * 
 * @low_op{16, @f$
 *      v_k = \leftarrow Re\\{b_k\\} \cdot Re\\{c\\}  
 *                     - Im\\{b_k\\} \cdot Im\\{c\\}                        \\
 *      s_k = \leftarrow Im\\{b_k\\} \cdot Re\\{c\\}  
 *                     + Re\\{b_k\\} \cdot Im\\{c\\}                        \\
 *      Re\\{a_k\\} \leftarrow  round( sat_{16}( v_k \cdot 2^{-a\_shr} ) )  \\
 *      Im\\{a_k\\} \leftarrow  round( sat_{16}( s_k \cdot 2^{-a\_shr} ) )  \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the complex 16-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} } and @math{c} is the 
 * complex 16-bit mantissa of floating-point value @math{c \cdot 2^{c\_exp}, then the resulting vector @vector{a} are 
 * the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp + c\_exp + a\_shr}.
 * 
 * The function xs3_vect_complex_s16_mul_prepare() can be used to obtain values for @math{a\_exp} and @math{a\_shr} 
 * based on the input exponents @math{b\_exp} and @math{c\_exp} and the input headrooms @math{b\_hr} and @math{c\_hr}. 
 * 
 * @param[out]      a_real      Real part of complex output vector @vector{a}
 * @param[out]      a_imag      Imaginary aprt of complex output vector @vector{a}
 * @param[in]       b_real      Real part of complex input vector @vector{b}
 * @param[in]       b_imag      Imaginary part of complex input vector @vector{b}
 * @param[in]       c_real      Real part of complex input scalar @math{c}
 * @param[in]       c_imag      Imaginary part of complex input scalar @math{c}
 * @param[in]       length      Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]       a_shr       Right-shift applied to 32-bit intermediate results
 * 
 * @returns     Headroom of the output vector @vector{a}.
 */
headroom_t xs3_vect_complex_s16_scale(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const int16_t c_real,
    const int16_t c_imag,
    const unsigned length,
    const right_shift_t a_shr);


/**
 * @brief Set each element of a complex 16-bit vector to a specified value.
 * 
 * `a_real[]` and `a_imag[]` together represent the complex 16-bit output mantissa vector @vector{a}. 
 * Each @math{Re\\{a_k\\}} is `a_real[k]`, and each @math{Im\\{a_k\\}} is `a_imag[k]`. Each must begin at a word-aligned 
 * address.
 * 
 * `b_real` and `b_imag` are the real and imaginary parts of the complex 16-bit input mantissa @math{b}. Each 
 * `a_real[k]` will be set to `b_real`. Each `a_imag[k]` will be set to `b_imag`.
 * 
 * `length` is the number of elements in `a_real[]` and `a_imag[]`.
 * 
 * @low_op{16, @f$ 
 *      Re\\{a_k\\} \leftarrow Re\\{b\\}                \\
 *      Im\\{a_k\\} \leftarrow Im\\{b\\}                \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @math{b} is the mantissa of floating-point value @math{b \cdot 2^{b\_exp}}, then the output vector @vector{a} are
 * the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp}.
 * 
 * @param[out]      a_real      Real part of complex output vector @vector{a}
 * @param[out]      a_imag      Imaginary aprt of complex output vector @vector{a}
 * @param[in]       b_real      Real part of complex input scalar @math{b}
 * @param[in]       b_imag      Imaginary part of complex input scalar @math{b}
 * @param[in]       length      Number of elements in vectors @vector{a} and @vector{b}
 */
void xs3_vect_complex_s16_set(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real,
    const int16_t b_imag,
    const unsigned length);


/**
 * @brief Left-shift each element of a complex 16-bit vector by a specified number of bits.
 * 
 * `a_real[]` and `a_imag[]` together represent the complex 16-bit output mantissa vector @vector{a}. 
 * Each @math{Re\\{a_k\\}} is `a_real[k]`, and each @math{Im\\{a_k\\}} is `a_imag[k]`.
 * 
 * `b_real[]` and `b_imag[]` together represent the complex 16-bit input mantissa vector @vector{b}.
 * Each @math{Re\\{b_k\\}} is `b_real[k]`, and each @math{Im\\{b_k\\}} is `b_imag[k]`.
 * 
 * Each of the input vectors must begin at a word-aligned address. This operation can be performed safely in-place on
 * inputs `b_real[]` and `b_imag[]`.
 * 
 * `length` is the number of elements in @vector{a} and @vector{b}.
 * 
 * `b_shl` is the signed arithmetic left-shift applied to each element of @vector{b}.
 * 
 * @low_op{16, @f$
 *      Re\\{a_k\\} \leftarrow sat_{16}(\lfloor Re\\{b_k\\} \cdot 2^{b\_shl} \rfloor)     \\
 *      Im\\{a_k\\} \leftarrow sat_{16}(\lfloor Im\\{b_k\\} \cdot 2^{b\_shl} \rfloor)     \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the complex 16-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} }, then the resulting 
 * vector @vector{a} are the complex 16-bit mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where 
 * @math{\bar{a} = \bar{b} \cdot 2^{b\_shl}} and @math{a\_exp = b\_exp}.
 * 
 * @param[out] a_real       Real part of complex output vector @vector{a}
 * @param[out] a_imag       Imaginary aprt of complex output vector @vector{a}
 * @param[in]  b_real       Real part of complex input vector @vector{b}
 * @param[in]  b_imag       Imaginary part of complex input vector @vector{b}
 * @param[in]  length       Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]  b_shl        Left-shift applied to @vector{b} 
 * 
 * @returns     Headroom of the output vector @vector{a}
 */
headroom_t xs3_vect_complex_s16_shl(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const unsigned length,
    const left_shift_t b_shl);


/**
 * @brief Right-shift each element of a complex 16-bit vector by a specified number of bits.
 * 
 * `a_real[]` and `a_imag[]` together represent the complex 16-bit output mantissa vector @vector{a}. 
 * Each @math{Re\\{a_k\\}} is `a_real[k]`, and each @math{Im\\{a_k\\}} is `a_imag[k]`.
 * 
 * `b_real[]` and `b_imag[]` together represent the complex 16-bit input mantissa vector @vector{b}.
 * Each @math{Re\\{b_k\\}} is `b_real[k]`, and each @math{Im\\{b_k\\}} is `b_imag[k]`.
 * 
 * Each of the input vectors must begin at a word-aligned address. This operation can be performed safely in-place on
 * inputs `b_real[]` and `b_imag[]`.
 * 
 * `length` is the number of elements in @vector{a} and @vector{b}.
 * 
 * `b_shr` is the signed arithmetic right-shift applied to each element of @vector{b}.
 * 
 * @low_op{16, @f$
 *      Re\\{a_k\\} \leftarrow sat_{16}(\lfloor Re\\{b_k\\} \cdot 2^{-b\_shr} \rfloor)     \\
 *      Im\\{a_k\\} \leftarrow sat_{16}(\lfloor Im\\{b_k\\} \cdot 2^{-b\_shr} \rfloor)     \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the complex 16-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} }, then the resulting 
 * vector @vector{a} are the complex 16-bit mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where 
 * @math{\bar{a} = \bar{b} \cdot 2^{-b\_shr}} and @math{a\_exp = b\_exp}.
 * 
 * @param[out] a_real       Real part of complex output vector @vector{a}
 * @param[out] a_imag       Imaginary aprt of complex output vector @vector{a}
 * @param[in]  b_real       Real part of complex input vector @vector{b}
 * @param[in]  b_imag       Imaginary part of complex input vector @vector{b}
 * @param[in]  length       Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]  b_shr        Right-shift applied to @vector{b} 
 * 
 * @returns     Headroom of the output vector @vector{a}
 */
headroom_t xs3_vect_complex_s16_shr(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const unsigned length,
    const right_shift_t b_shr);


/**
 * @brief Get the squared magnitudes of elements of a complex 16-bit vector.
 * 
 * `a[]` represents the real 16-bit output mantissa vector @vector{a}.
 * 
 * `b_real[]` and `b_imag[]` together represent the complex 16-bit input mantissa vector @vector{b}.
 * Each @math{Re\\{b_k\\}} is `b_real[k]`, and each @math{Im\\{b_k\\}} is `b_imag[k]`.
 * 
 * Each of the input vectors must begin at a word-aligned address.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `a_shr` is the unsigned arithmetic right-shift applied to the 32-bit accumulators holding the penultimate results.
 * 
 * @low_op{16, @f$ 
 *      a_k \leftarrow ((Re\\{b_k'\\})^2 + (Im\\{b_k'\\})^2)\cdot 2^{-a\_shr}   \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the complex 16-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} }, then the resulting 
 * vector @vector{a} are the real 16-bit mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where 
 * @math{a\_exp = 2 \cdot b\_exp + a\_shr}.
 * 
 * The function xs3_vect_complex_s16_squared_mag_prepare() can be used to obtain values for @math{a\_exp} and 
 * @math{a\_shr} based on the input exponent @math{b\_exp} and headroom @math{b\_hr}.
 * 
 * @param[out]  a           Real output vector @vector{a}
 * @param[in]   b_real      Real part of complex input vector @vector{b}
 * @param[in]   b_imag      Imaginary part of complex input vector @vector{b}
 * @param[in]   length      Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]   a_shr       Right-shift appled to 32-bit intermediate results
 * 
 * @see xs3_vect_complex_s16_squared_mag_prepare
 */
headroom_t xs3_vect_complex_s16_squared_mag(
    int16_t a[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const unsigned length,
    const right_shift_t a_shr);


/**
 * @brief Obtain the output exponent and input shift used by xs3_vect_complex_s16_squared_mag().
 * 
 * This function is used in conjunction with xs3_vect_complex_s16_squared_mag() to compute the squared magnitude of each 
 * element of a complex 16-bit BFP vector.
 * 
 * This function computes `a_exp` and `a_shr`.
 * 
 * `a_exp` is the exponent associated with mantissa vector @vector{a}, and is be chosen to maximize precision when
 * elements of @vector{a} are computed. The `a_exp` chosen by this function is derived from the exponent and headroom
 * associated with the input vector.
 * 
 * `a_shr` is the shift parameter required by xs3_vect_complex_s16_mag() to achieve the chosen output exponent `a_exp`.
 * 
 * `b_exp` is the exponent associated with the input mantissa vector @vector{b}.
 * 
 * `b_hr` is the headroom of @vector{b}. If the headroom of @vector{b} is unknown it can be calculated using 
 * xs3_vect_complex_s16_headroom(). Alternatively, the value `0` can always be safely used (but may result in reduced
 * precision).
 * 
 * @par Adjusting Output Exponents
 * 
 * If a specific output exponent `desired_exp` is needed for the result (e.g. for emulating fixed-point arithmetic), the 
 * `a_shr` produced by this function can be adjusted according to the following:
 * \code{.c}
 *      exponent_t a_exp;
 *      right_shift_t a_shr;
 *      xs3_vect_s16_mul_prepare(&a_exp, &a_shr, b_exp, c_exp, b_hr, c_hr);
 *      exponent_t desired_exp = ...; // Value known a priori
 *      a_shr = a_shr + (desired_exp - a_exp);
 *      a_exp = desired_exp;
 * \endcode
 * 
 * When applying the above adjustment, the following condition should be maintained:
 * * `a_shr >= 0`
 * 
 * Using larger values than strictly necessary for `a_shr` may result in unnecessary underflows or loss of precision.
 * 
 * @param[out]  a_exp               Output exponent associated with output mantissa vector @vector{a}
 * @param[out]  a_shr               Unsigned arithmetic right-shift for @vector{a} used by 
 *                                  xs3_vect_complex_s16_squared_mag()
 * @param[in]   b_exp               Exponent associated with input mantissa vector @vector{b}
 * @param[in]   b_hr                Headroom of input mantissa vector @vector{b}
 * 
 * @see xs3_vect_complex_s16_squared_mag()
 */
void xs3_vect_complex_s16_squared_mag_prepare(
    exponent_t* a_exp,
    right_shift_t* a_shr,
    const exponent_t b_exp,
    const headroom_t b_hr);


/**
 * @brief Subtract one complex 16-bit vector from another.
 * 
 * `a_real[]` and `a_imag[]` together represent the complex 16-bit output mantissa vector @vector{a}. 
 * Each @math{Re\\{a_k\\}} is `a_real[k]`, and each @math{Im\\{a_k\\}} is `a_imag[k]`.
 * 
 * `b_real[]` and `b_imag[]` together represent the complex 16-bit input mantissa vector @vector{b}.
 * Each @math{Re\\{b_k\\}} is `b_real[k]`, and each @math{Im\\{b_k\\}} is `b_imag[k]`.
 * 
 * `c_real[]` and `c_imag[]` together represent the complex 16-bit input mantissa vector @vector{c}.
 * Each @math{Re\\{c_k\\}} is `c_real[k]`, and each @math{Im\\{c_k\\}} is `c_imag[k]`.
 * 
 * Each of the input vectors must begin at a word-aligned address. This operation can be performed safely in-place on
 * inputs `b_real[]`, `b_imag[]`, `c_real[]` and `c_imag[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b} and @vector{c} 
 * respectively.
 * 
 * @low_op{16, @f$ 
 *      b_k' \leftarrow sat_{16}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
 *      c_k' \leftarrow sat_{16}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)     \\
 *      Re\\{a_k\\} \leftarrow Re\\{b_k'\\} - Re\\{c_k'\\}                  \\
*       Im\\{a_k\\} \leftarrow Im\\{b_k'\\} - Im\\{c_k'\\}                  \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} and @vector{c} are the complex 16-bit mantissas of BFP vectors @math{ \bar{b} \cdot 2^{b\_exp} } and 
 * @math{\bar{c} \cdot 2^{c\_exp}}, then the resulting vector @vector{a} are the complex 16-bit mantissas of BFP vector 
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
headroom_t xs3_vect_complex_s16_sub(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const int16_t c_real[],
    const int16_t c_imag[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Get the sum of elements of a complex 16-bit vector.
 * 
 * `b_real[]` and `b_imag[]` together represent the complex 16-bit input mantissa vector @vector{b}, and must both begin
 * at a word-aligned address. Each @math{Re\\{b_k\\}} is `b_real[k]`, and each @math{Im\\{b_k\\}} is `b_imag[k]`.
 * 
 * `length` is the number of elements in @vector{b}.
 * 
 * @low_op{16, @f$ 
 *      Re\\{a\\} \leftarrow \sum_{k=0}^{length-1} \left( Re\\{b_k\\} \right)      \\
 *      Im\\{a\\} \leftarrow \sum_{k=0}^{length-1} \left( Im\\{b_k\\} \right)      
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the returned value @math{a} is 
 * the complex 32-bit mantissa of floating-point value @math{a \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp}.
 * 
 * @param[in] b_real    Real part of complex input vector @vector{b}
 * @param[in] b_imag    Imaginary part of complex input vector @vector{b}
 * @param[in] length    Number of elements in vector @vector{b}.
 * 
 * @returns @math{a}, the 32-bit complex sum of elements in @vector{b}.
 */
complex_s32_t xs3_vect_complex_s16_sum(
    const int16_t b_real[],
    const int16_t b_imag[],
    const unsigned length);

/**
 * @brief Convert a complex 16-bit vector into a complex 32-bit vector.
 * 
 * `a[]` represents the complex 32-bit output vector @vector{a}. It must begin at a double word (8-byte) aligned 
 * address.
 * 
 * `b_real[]` and `b_imag[]` together represent the complex 16-bit input mantissa vector @vector{b}.
 * Each @math{Re\\{b_k\\}} is `b_real[k]`, and each @math{Im\\{b_k\\}} is `b_imag[k]`.
 * 
 * The parameter `length` is the number of elements in each of the vectors.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * @low_op{16, @f$ 
 *      Re\\{a_k\\} \leftarrow Re\\{b_k\\}              \\
 *      Im\\{a_k\\} \leftarrow Im\\{b_k\\}              \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the complex 16-bit mantissas of a BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the resulting 
 * vector @vector{a} are the complex 32-bit mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where
 * @math{a\_exp = b\_exp}.
 * 
 * @par Notes
 * * The headroom of output vector @vector{a} is not returned by this function. The headroom of the output is always
 *   16 bits greater than the headroom of the input.
 * 
 * @param[out] a        Complex output vector @vector{a}.
 * @param[in]  b_real   Real part of complex input vector @vector{b}.
 * @param[in]  b_imag   Imaginary part of complex input vector @vector{b}.
 * @param[in]  length   Number of elements in vectors @vector{a} and @vector{b}
 */
void xs3_vect_complex_s16_to_complex_s32(
    complex_s32_t a[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const unsigned length);


/** 
 * @brief Compute the element-wise absolute value of a 16-bit vector.
 * 
 * `a[]` and `b[]` represent the 16-bit vectors @vector{a} and @vector{b} respectively. Each must begin at a
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
headroom_t xs3_vect_s16_abs(
    int16_t a[],
    const int16_t b[],
    const unsigned length);


/**
 * @brief Compute the sum of the absolute values of elements of a 16-bit vector.
 * 
 * `b[]` represents the 16-bit vector @vector{b}. `b[]` must begin at a word-aligned address.
 * 
 * `length` is the number of elements in @vector{b}.
 * 
 * @low_op{16, @f$
 *      a \leftarrow \sum_{k=0}^{length-1} sat_{16}(\left| b_k \right|)
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
 * @returns The 32-bit sum @math{a}
 */
int32_t xs3_vect_s16_abs_sum(
    const int16_t b[],
    const unsigned length);


/**
 * @brief Add one 16-bit BFP vector to another.
 * 
 * `a[]`, `b[]` and `c[]` represent the 16-bit vectors @vector{a}, @vector{b} and @vector{c} respectively. Each must 
 * begin at a word-aligned address. This operation can be performed safely in-place on `b[]` or `c[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b} and @vector{c} 
 * respectively.
 * 
 * @low_op{16, @f$ 
 *      b_k' = sat_{16}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)  \\
 *      c_k' = sat_{16}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)  \\
 *      a_k \leftarrow sat_{16}\!\left( b_k' + c_k' \right)     \\
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
headroom_t xs3_vect_s16_add(
    int16_t a[],
    const int16_t b[], 
    const int16_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Obtain the array index of the maximum element of a 16-bit vector.
 * 
 * `b[]` represents the 16-bit input vector @vector{b}. It must begin at a word-aligned address.
 * 
 * `length` is the number of elements in @vector{b}.
 * 
 * @low_op{16, @f$ 
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
unsigned xs3_vect_s16_argmax(
    const int16_t b[], 
    const unsigned length);


/**
 * @brief Obtain the array index of the minimum element of a 16-bit vector.
 * 
 * `b[]` represents the 16-bit input vector @vector{b}. It must begin at a word-aligned address.
 * 
 * `length` is the number of elements in @vector{b}.
 * 
 * @low_op{16, @f$ 
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
unsigned xs3_vect_s16_argmin(
    const int16_t b[], 
    const unsigned length);


/**
 * @brief Clamp the elements of a 16-bit vector to a specified range.
 * 
 * `a[]` and `b[]` represent the 16-bit vectors @vector{a} and @vector{b} respectively. Each must begin at a 
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
 * @low_op{16, @f$
 *      b_k' \leftarrow sat_{16}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor) \\
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
headroom_t xs3_vect_s16_clip(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const int16_t lower_bound,
    const int16_t upper_bound,
    const right_shift_t b_shr);


/**
 * @brief Compute the inner product of two 16-bit vectors.
 * 
 * `b[]` and `c[]` represent the 32-bit vectors @vector{a} and @vector{b} respectively. Each must begin at a 
 * word-aligned address.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * \low_op{16, @f$ 
 *      a \leftarrow \sum_{k=0}^{length-1}\left( b_k \cdot c_k \right)
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} and @vector{c} are the mantissas of the BFP vectors @math{ \bar{b} \cdot 2^{b\_exp} } and 
 * @math{\bar{c}\cdot 2^{c\_exp}, then result @math{a} is the mantissa of the result @math{a \cdot 2^{a\_exp}}, where
 * @math{a\_exp = b\_exp + c\_exp}.
 * 
 * If needed, the bit-depth of @math{a} can then be reduced to 16 or 32 bits to get a new result 
 * @math{a' \cdot 2^{a\_exp'}} where @math{a' = a \cdot 2^{-a\_shr}} and @math{a\_exp' = a\_exp + a\_shr}.
 * 
 * @todo I don't think there are currently any functions in this library to perform this bit-depth reduction in a 
 *       user-friendly way.
 * 
 * @par Notes
 * 
 * The sum @math{a} is accumulated simultaneously into 16 48-bit accumulators which are summed together at the final
 * step. So long as `length` is less than roughly 2 million, no overflow or saturation of the resulting sum is possible. 
 * 
 * @param[in] b             Input vector @vector{b}
 * @param[in] c             Input vector @vector{c}
 * @param[in] length        Number of elements in vectors @vector{b} and @vector{c}
 * 
 * @returns @math{a}, the inner product of vectors @vector{b} and @vector{c}.
 */
int64_t xs3_vect_s16_dot(
    const int16_t b[],
    const int16_t c[],
    const unsigned length);


/**
 * @brief Calculate the energy (sum of squares of elements) of a 16-bit vector.
 * 
 * `b[]` represents the 16-bit vector @vector{b}. `b[]` must begin at a word-aligned address.
 * 
 * `length` is the number of elements in @vector{b}.
 * 
 * `b_shr` is the signed arithmetic right-shift applied to elements of @vector{b}. `b_shr` should be chosen to avoid
 * the possibility of saturation. See the note below.
 * 
 * @low_op{16, @f$
 *      b_k' \leftarrow sat_{16}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)         \\
 *      a \leftarrow \sum_{k=0}^{length-1} (b_k')^2
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the mantissas of the BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then floating-point result is
 * @math{a \cdot 2^{a\_exp}}, where the 32-bit mantissa @math{a} is returned by this function, and 
 * @math{a\_exp = 2 \cdot (b\_exp + b\_shr) }.
 * 
 * @par Additional Details
 * 
 * If @vector{b} has @math{b\_hr} bits of headroom, then each product @math{(b_k')^2} can be a maximum of 
 * @math{ 2^{30 - 2 \cdot (b\_hr + b\_shr)}}. So long as `length` is less than @math{1 + 2\cdot (b\_hr + b\_shr) }, 
 * such errors should not be possible. Each increase of @math{b\_shr} by @math{1} doubles the number of elements that
 * can be summed without risk of overflow.
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
int32_t xs3_vect_s16_energy(
    const int16_t b[],
    const unsigned length,
    const right_shift_t b_shr);


/**
 * @brief Calculate the headroom of a 16-bit vector.
 * 
 * The headroom of an N-bit integer is the number of bits that the integer's value may be left-shifted
 * without any information being lost. Equivalently, it is one less than the number of leading sign bits.
 * 
 * The headroom of an `int16_t` array is the minimum of the headroom of each of its `int16_t` elements.
 * 
 * This function efficiently traverses the elements of `b[]` to determine its headroom.
 * 
 * `b[]` represents the 16-bit vector @vector{b}. `b[]` must begin at a word-aligned address.
 * 
 * `length` is the number of elements in `b[]`.
 * 
 * @low_op{16, @f$
 *      a \leftarrow min\!\\{ HR_{16}\left(x_0\right), HR_{16}\left(x_1\right), ..., 
 *              HR_{16}\left(x_{length-1}\right) \\}
 * @f$ }
 * 
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   length      The number of elements in vector @vector{b}
 * 
 * @returns     Headroom of vector @vector{b} 
 * 
 * @see xs3_vect_ch_pair_s16_headroom
 * @see xs3_vect_ch_pair_s32_headroom
 * @see xs3_vect_s32_headroom
 * @see xs3_vect_complex_s16_headroom
 * @see xs3_vect_complex_s32_headroom
 */
headroom_t xs3_vect_s16_headroom(
    const int16_t b[], 
    const unsigned length);


/**
 * @brief Compute the inverse of elements of a 16-bit vector.
 * 
 * `a[]` and `b[]` represent the 16-bit mantissa vectors @vector{a} and @vector{b} respectively. This operation can be 
 * performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `scale` is a scaling parameter used to maximize the precision of the result.
 * 
 * @low_op{16, @f$
 *      a_k \leftarrow \lfloor\frac{2^{scale}}{b_k}\rfloor      \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the resulting vector @vector{a}
 * are the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = scale - b\_exp}.
 * 
 * The function xs3_vect_s16_inverse_prepare() can be used to obtain values for @math{a\_exp} and @math{scale}.
 * 
 * @param[out]  a           Output vector @vector{a}
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   length      Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]   scale       Scale factor applied to dividend when computing inverse
 * 
 * @returns     Headroom of output vector @vector{a}
 * 
 * @see xs3_vect_s16_inverse_prepare
 */
void xs3_vect_s16_inverse(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const unsigned scale);


/**
 * @brief Obtain the output exponent and scaling parameter used by xs3_vect_s16_inverse().
 * 
 * This function is used in conjunction with xs3_vect_s16_inverse() to compute the inverse of elements of a 16-bit
 * BFP vector.
 * 
 * This function computes `a_exp` and `scale`.
 * 
 * `a_exp` is the exponent associated with output mantissa vector @vector{a}, and must be chosen to avoid overflow in 
 * the smallest element of the input vector, which when inverted becomes the largest output element. To maximize 
 * precision, this function chooses `a_exp` to be the smallest exponent known to avoid saturation. The `a_exp` chosen
 * by this function is derived from the exponent and smallest element of the input vector.
 * 
 * `scale` is a scaling parameter used by xs3_vect_s16_inverse() to achieve the chosen output exponent.
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
 * @see xs3_vect_s16_inverse
 */
void xs3_vect_s16_inverse_prepare(
    exponent_t* a_exp,
    unsigned* scale,
    const int16_t b[],
    const exponent_t b_exp,
    const unsigned length);


/**
 * @brief Find the maximum value in a 16-bit vector.
 * 
 * `b[]` represents the 16-bit vector @vector{b}. It must begin at a word-aligned address.
 * 
 * `length` is the number of elements in @vector{b}.
 * 
 * @low_op{16, @f$ 
 *      max\\{ x_0, x_1, ..., x_{length-1} \\}
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the returned value @math{a} is 
 * the 16-bit mantissa of floating-point value @math{a \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp}.
 * 
 * @param[in]   b           Input vector @vector{b}     
 * @param[in]   length      Number of elements in @vector{b}
 * 
 * @returns     Maximum value from @vector{b}
 */
int16_t xs3_vect_s16_max(
    const int16_t b[], 
    const unsigned length);


/**
 * @brief Find the minimum value in a 16-bit vector.
 * 
 * `b[]` represents the 16-bit vector @vector{b}. It must begin at a word-aligned address.
 * 
 * `length` is the number of elements in @vector{b}.
 * 
 * @low_op{16, @f$ 
 *      max\\{ x_0, x_1, ..., x_{length-1} \\}
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the returned value @math{a} is 
 * the 16-bit mantissa of floating-point value @math{a \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp}.
 * 
 * @param[in]   b           Input vector @vector{b}     
 * @param[in]   length      Number of elements in @vector{b}
 * 
 * @returns     Minimum value from @vector{b}
 */
int16_t xs3_vect_s16_min(
    const int16_t a[], 
    const unsigned length);


/**
 * @brief Multiply two 16-bit vectors together element-wise.
 * 
 * `a[]`, `b[]` and `c[]` represent the 16-bit vectors @vector{a}, @vector{b} and @vector{c} respectively. 
 * Each must begin at a word-aligned address. This operation can be performed safely in-place on `b[]` or `c[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `a_shr` is an unsigned arithmetic right-shift applied to the 32-bit accumulators holding the penultimate results.
 * 
 * \low_op{16, @f$
 *      a_k' \leftarrow b_k \cdot c_k                               \\
 *      a_k \leftarrow sat_{16}(round(a_k' \cdot 2^{-a\_shr}))      \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} and @vector{c} are the mantissas of BFP vectors @math{ \bar{b} \cdot 2^{b\_exp} } and 
 * @math{\bar{c} \cdot 2^{c\_exp}}, then the resulting vector @vector{a} are the mantissas of BFP vector 
 * @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp + c\_exp + a\_shr}.
 * 
 * The function xs3_vect_s16_mul_prepare() can be used to obtain values for @math{a\_exp} and @math{a\_shr} based on the
 * input exponents @math{b\_exp} and @math{c\_exp} and the input headrooms @math{b\_hr} and @math{c\_hr}. 
 * 
 * @param[out] a        Output vector @vector{a}
 * @param[in]  b        Input vector @vector{b}
 * @param[in]  c        Input vector @vector{c}
 * @param[in]  length   Number of elements in vectors @vector{a}, @vector{b} and @vector{c}
 * @param[in]  a_shr    Right-shift appled to 32-bit products
 * 
 * @returns  Headroom of vector @vector{a}
 */
//! [xs3_vect_s16_mul]
headroom_t xs3_vect_s16_mul(
    int16_t a[],
    const int16_t b[],
    const int16_t c[],
    const unsigned length,
    const right_shift_t a_shr);
//! [xs3_vect_s16_mul]


/**
 * @brief Obtain the output exponent and output shift used by xs3_vect_s16_mul().
 * 
 * This function is used in conjunction with xs3_vect_s16_mul() to perform an element-wise multiplication of two 16-bit
 * BFP vectors.
 * 
 * This function computes `a_exp` and `a_shr`.
 * 
 * `a_exp` is the exponent associated with mantissa vector @vector{a}, and must be chosen to be large enough to avoid 
 * overflow when elements of @vector{a} are computed. To maximize precision, this function chooses `a_exp` to be the 
 * smallest exponent known to avoid saturation (see exception below). The `a_exp` chosen by this function is derived 
 * from the exponents and headrooms of associated with the input vectors.
 * 
 * `a_shr` is an arithmetic right-shift applied by xs3_vect_complex_s16_mul() to the 32-bit products of input elements 
 * to achieve the chosen output exponent `a_exp`.
 * 
 * `b_exp` and `c_exp` are the exponents associated with the input mantissa vectors @vector{b} and @vector{c} 
 * respectively.
 * 
 * `b_hr` and `c_hr` are the headroom of @vector{b} and @vector{c} respectively. If the headroom of @vector{b} or 
 * @vector{c} is unknown, they can be obtained by calling xs3_vect_s16_headroom(). Alternatively, the value `0` can 
 * always be safely used (but may result in reduced precision).
 * 
 * @par Adjusting Output Exponents
 * 
 * If a specific output exponent `desired_exp` is needed for the result (e.g. for emulating fixed-point arithmetic), the 
 * `a_shr` produced by this function can be adjusted according to the following:
 * \code{.c}
 *      exponent_t a_exp;
 *      right_shift_t a_shr;
 *      xs3_vect_s16_mul_prepare(&a_exp, &a_shr, b_exp, c_exp, b_hr, c_hr);
 *      exponent_t desired_exp = ...; // Value known a priori
 *      a_shr = a_shr + (desired_exp - a_exp);
 *      a_exp = desired_exp;
 * \endcode
 * 
 * When applying the above adjustment, the following conditions should be maintained:
 * * `a_shr >= 0`
 * 
 * Be aware that using a smaller value than strictly necessary for `a_shr` can result in saturation, and using larger 
 * values may result in unnecessary underflows or loss of precision.
 * 
 * @par Notes
 * 
 * * Using the outputs of this function, an output mantissa which would otherwise be `INT16_MIN` will instead saturate 
 *   to `-INT16_MAX`. This is due to the symmetric saturation logic employed by the VPU and is a hardware feature. This 
 *   is a corner case which is usually unlikely and results in 1 LSb of error when it occurs.
 * 
 * @param[out]  a_exp       Exponent of output elements of xs3_vect_s16_mul()
 * @param[out]  a_shr       Right-shift supplied to xs3_vect_s16_mul()
 * @param[in]   b_exp       Exponent associated with @vector{b}
 * @param[in]   c_exp       Exponent associated with @vector{c}
 * @param[in]   b_hr        Headroom of @vector{b}
 * @param[in]   c_hr        Headroom of @vector{c}
 * 
 * @see xs3_vect_s16_mul
 */
void xs3_vect_s16_mul_prepare(
    exponent_t* a_exp,
    right_shift_t* a_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr);


/** 
 * @brief Rectify the elements of a 16-bit vector.
 * 
 * Rectification ensures that all outputs are non-negative, changing negative values to 0.
 * 
 * `a[]` and `b[]` represent the 16-bit vectors @vector{a} and @vector{b} respectively. Each must begin at a
 * word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * 
 * Each output element `a[k]` is set to the value of the corresponding input element 
 * `b[k]` if it is positive, and `a[k]` is set to zero otherwise. 
 * 
 * \low_op{16, @f$
 *      a_k \leftarrow \begin{cases}
 *          b_k & b_k \gt 0             \\ 
 *          0 & b_k \leq 0\end{cases}   \\
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
headroom_t xs3_vect_s16_rect(
    int16_t a[],
    const int16_t b[],
    const unsigned length);


/**
 * @brief Multiply a 16-bit vector by a 16-bit scalar.
 * 
 * `a[]` and `b[]` represent the 16-bit vectors @vector{a} and @vector{b} respectively. Each must begin at a
 *  word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `c` is the 16-bit scalar @math{c} by which elements of @vector{b} are multiplied.
 * 
 * `a_shr` is an unsigned arithmetic right-shift applied to the 32-bit accumulators holding the penultimate results.
 * 
 * \low_op{16, @f$
 *      a_k' \leftarrow b_k \cdot c                                 \\
 *      a_k \leftarrow sat_{16}(round(a_k' \cdot 2^{-a\_shr}))      \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} } and @math{c} is the mantissa of
 * floating-point value @math{c \cdot 2^{c\_exp}, then the resulting vector @vector{a} are the mantissas of BFP vector 
 * @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp + c\_exp + a\_shr}.
 * 
 * The function xs3_vect_s16_scale_prepare() can be used to obtain values for @math{a\_exp} and @math{a\_shr} based on 
 * the input exponents @math{b\_exp} and @math{c\_exp} and the input headrooms @math{b\_hr} and @math{c\_hr}. 
 * 
 * @param[out] a        Output vector @vector{a}
 * @param[in]  b        Input vector @vector{b}
 * @param[in]  c        Input vector @vector{c}
 * @param[in]  length   Number of elements in vectors @vector{a}, @vector{b} and @vector{c}
 * @param[in]  a_shr    Right-shift appled to 32-bit products
 * 
 * @returns  Headroom of vector @vector{a}
 */
headroom_t xs3_vect_s16_scale(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const int16_t c,
    const right_shift_t a_shr);


/**
 * @brief Obtain the output exponent and output shift used by xs3_vect_s16_scale().
 * 
 * This function is used in conjunction with xs3_vect_s16_scale() to perform multiplication of a 16-bit BFP vector 
 * @math{\bar{b} \cdot 2^{b\_exp}} by a 16-bit scalar @math{c \cdot 2^{c\_exp}}. The result is another 16-bit BFP vector
 * @math{\bar{a} \cdot 2^{a\_exp}}.
 * 
 * This function computes `a_exp` and `a_shr`.
 * 
 * `a_exp` is the exponent associated with mantissa vector @vector{a}, and must be chosen to be large enough to avoid 
 * overflow when elements of @vector{a} are computed. To maximize precision, this function chooses `a_exp` to be the 
 * smallest exponent known to avoid saturation (see exception below). The `a_exp` chosen by this function is derived 
 * from the exponents and headrooms of associated with the inputs.
 * 
 * `a_shr` is an arithmetic right-shift applied by xs3_vect_complex_s16_scale() to the 32-bit products of input elements 
 * to achieve the chosen output exponent `a_exp`.
 * 
 * `b_exp` and `c_exp` are the exponents associated with @vector{b} and @math{c} 
 * respectively.
 * 
 * `b_hr` and `c_hr` are the headroom of @vector{b} and @math{c} respectively. If the headroom of @vector{b} or 
 * @math{c} are unknown, they can be obtained by calling xs3_vect_s16_headroom(). Alternatively, the value `0` can 
 * always be safely used (but may result in reduced precision).
 * 
 * @par Adjusting Output Exponents
 * 
 * If a specific output exponent `desired_exp` is needed for the result (e.g. for emulating fixed-point arithmetic), the 
 * `a_shr` produced by this function can be adjusted according to the following:
 * \code{.c}
 *      exponent_t a_exp;
 *      right_shift_t a_shr;
 *      xs3_vect_s16_scale_prepare(&a_exp, &a_shr, b_exp, c_exp, b_hr, c_hr);
 *      exponent_t desired_exp = ...; // Value known a priori
 *      a_shr = a_shr + (desired_exp - a_exp);
 *      a_exp = desired_exp;
 * \endcode
 * 
 * When applying the above adjustment, the following conditions should be maintained:
 * * `a_shr >= 0`
 * 
 * Be aware that using a smaller value than strictly necessary for `a_shr` can result in saturation, and using larger 
 * values may result in unnecessary underflows or loss of precision.
 * 
 * @par Notes
 * 
 * * Using the outputs of this function, an output mantissa which would otherwise be `INT16_MIN` will instead saturate 
 *   to `-INT16_MAX`. This is due to the symmetric saturation logic employed by the VPU and is a hardware feature. This 
 *   is a corner case which is usually unlikely and results in 1 LSb of error when it occurs.
 * 
 * @param[out]  a_exp       Exponent of output elements of xs3_vect_s16_scale()
 * @param[out]  a_shr       Right-shift supplied to xs3_vect_s16_scale()
 * @param[in]   b_exp       Exponent associated with @vector{b}
 * @param[in]   c_exp       Exponent associated with @vector{c}
 * @param[in]   b_hr        Headroom of @vector{b}
 * @param[in]   c_hr        Headroom of @vector{c}
 * 
 * @see xs3_vect_s16_scale
 */
void xs3_vect_s16_scale_prepare(
    exponent_t* a_exp,
    right_shift_t* a_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr);

/** 
 * @brief Set all elements of a 16-bit vector to the specified value.
 * 
 * `a[]` represents the 16-bit vector @vector{a}. It must begin at a word-aligned address.
 * 
 * `b` is the value elements of @vector{a} are set to.
 * 
 * `length` is the number of elements in `a[]`.
 * 
 * @low_op{16, @f$
 *      a_k \leftarrow b                                    \\
 *          \qquad\text{for }k\in 0\ ...\ (length-1)
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @math{b} is the mantissa of floating-point value @math{b \cdot 2^{b\_exp}}, then the output vector @vector{a} are
 * the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp}.
 * 
 * @param[out] a        Output vector @vector{a}
 * @param[in]  b        Input value @math{b}
 * @param[in]  length   Number of elements in vector @vector{a}
 */
void xs3_vect_s16_set(
    int16_t a[],
    const int16_t b,
    const unsigned length);


/**
 * @brief Left-shift the elements of a 16-bit vector by a specified number of bits.
 * 
 * `a[]` and `b[]` represent the 16-bit vectors @vector{a} and @vector{b} respectively. Each must begin at a 
 * word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in vectors @vector{a} and @vector{b}.
 * 
 * `b_shl` is the signed arithmetic left-shift applied to each element of @vector{b}. 
 * 
 * @low_op{16, @f$ 
 *      a_k \leftarrow sat_{16}(\lfloor b_k \cdot 2^{b\_shl} \rfloor)       \\
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
headroom_t xs3_vect_s16_shl(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const left_shift_t b_shl);


/**
 * @brief Right-shift the elements of a 16-bit vector by a specified number of bits.
 * 
 * `a[]` and `b[]` represent the 16-bit vectors @vector{a} and @vector{b} respectively. Each must begin at a 
 * word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in vectors @vector{a} and @vector{b}.
 * 
 * `b_shr` is the signed arithmetic right-shift applied to each element of @vector{b}. 
 * 
 * @low_op{16, @f$ 
 *      a_k \leftarrow sat_{16}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)      \\
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
headroom_t xs3_vect_s16_shr(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const right_shift_t b_shr);


/**
 * @brief Compute the square roots of elements of a 16-bit vector.
 * 
 * `a[]` and `b[]` represent the 16-bit vectors @vector{a} and @vector{b} respectively. Each vector must begin at a 
 * word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` is the signed arithmetic right-shift applied to elements of @vector{b}.
 * 
 * `depth` is the number of most significant bits to calculate of each @math{a_k}. For example, a `depth` value of 8 
 * will only compute the 8 most significant byte of the result, with the remaining byte as 0. The maximum value for this 
 * parameter is `XS3_VECT_SQRT_S16_MAX_DEPTH` (31). The time cost of this operation is approximately proportional to the 
 * number of bits computed.
 * 
 * @low_op{16, @f$ 
 *      b_k' \leftarrow sat_{16}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
 *      a_k \leftarrow \begin{cases}
 *          \sqrt{ b_k' } & b_k' >= 0                                       \\
 *          0   &   otherwise\end{cases}                                    \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)                       \\
 *          \qquad\text{ where } \sqrt{\cdot} \text{ computes the most significant } depth 
 *                      \text{ bits of the square root.}
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the resulting vector @vector{a}
 * are the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = (b\_exp + b\_shr - 14)/2}.
 * 
 * Note that because exponents must be integers, that means @math{b\_exp + b\_shr} **must be even**.
 * 
 * The function xs3_vect_s16_sqrt_prepare() can be used to obtain values for @math{a\_exp} and @math{b\_shr} based on 
 * the input exponent @math{b\_exp} and headroom @math{b\_hr}.
 * 
 * @par Notes
 * 
 * * This function assumes roots are real. Negative input elements will result in corresponding outputs of 0.
 * 
 * @param[out]  a           Output vector @vector{a}
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   length      Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]   b_shr       Right-shift appled to @vector{b}
 * @param[in]   depth       Number of bits of each output value to compute
 * 
 * @returns     Headroom of output vector @vector{a}
 */
headroom_t xs3_vect_s16_sqrt(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const right_shift_t b_shr,
    const unsigned depth);


/**
 * @brief Obtain the output exponent and shift parameter used by xs3_vect_s16_sqrt().
 * 
 * This function is used in conjunction withx xs3_vect_s16_sqrt() to compute the square root of elements of a 16-bit
 * BFP vector.
 * 
 * This function computes `a_exp` and `b_shr`.
 * 
 * `a_exp` is the exponent associated with output mantissa vector @vector{a}, and should be chosen to maximize the 
 * precision of the results. To that end, this function chooses `a_exp` to be the smallest exponent known to avoid 
 * saturation of the resulting mantissa vector @vector{a}. It is derived from the exponent and headroom of the input
 * BFP vector.
 * 
 * `b_shr` is the shift parameter required by xs3_vect_s16_sqrt() to achieve the chosen output exponent `a_exp`.
 * 
 * `b_exp` is the exponent associated with the input mantissa vector @vector{b}.
 * 
 * `b_hr` is the headroom of @vector{b}. If it is unknown, it can be obtained using xs3_vect_s16_headroom(). 
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
 * Also, if a larger exponent is used than necessary, a larger `depth` parameter (see xs3_vect_s16_sqrt()) will be 
 * required to achieve the same precision, as the results are computed bit by bit, starting with the most significant
 * bit.
 * 
 * @param[out]  a_exp       Exponent of outputs of xs3_vect_s16_sqrt()
 * @param[out]  b_shr       Right-shift to be applied to elements of @vector{b}
 * @param[in]   b_exp       Exponent of vector{b}
 * @param[in]   b_hr        Headroom of vector{b}
 * 
 * @see xs3_vect_s16_sqrt
 */
void xs3_vect_s16_sqrt_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    const exponent_t b_exp,
    const right_shift_t b_hr);


/**
 * @brief Subtract one 16-bit BFP vector from another.
 * 
 * `a[]`, `b[]` and `c[]` represent the 16-bit vectors @vector{a}, @vector{b} and @vector{c} respectively. Each must 
 * begin at a word-aligned address. This operation can be performed safely in-place on `b[]` or `c[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b} and @vector{c} 
 * respectively.
 * 
 * @low_op{16, @f$ 
 *      b_k' = sat_{16}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)  \\
 *      c_k' = sat_{16}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)  \\
 *      a_k \leftarrow sat_{16}\!\left( b_k' - c_k' \right)     \\
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
headroom_t xs3_vect_s16_sub(
    int16_t a[],
    const int16_t b[],
    const int16_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Get the sum of elements of a 16-bit vector.
 * 
 * `b[]` represents the 16-bit vector @vector{b}. `b[]` must begin at a word-aligned address.
 * 
 * `length` is the number of elements in @vector{b}.
 * 
 * @low_op{16, @f$
 *      a \leftarrow \sum_{k=0}^{length-1} b_k
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
 * @returns The 32-bit sum @math{a}
 */
int32_t xs3_vect_s16_sum(
    const int16_t b[],
    const unsigned length);

    
/**
 * @brief Convert a 16-bit vector to a 32-bit vector.
 * 
 * `a[]` represents the 32-bit output vector @vector{a}. 
 * 
 * `b[]` represents the 16-bit input vector @vector{b}.
 * 
 * Each vector must begin at a word-aligned address.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * @low_op{16, @f$
 *      a_k \leftarrow b_k \cdot 2^{8}                  \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)
 * @f$ }
 * 
 * @par Block Floating-Point
 * 
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the resulting vector @vector{a}
 * are the 32-bit mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}. If @math{a\_exp = b\_exp - 8}, then this
 * operation has effectively not changed the values represented.
 * 
 * @par Notes
 * * The multiplication by @math{2^8} is an artifact of the VPU's behavior. It turns out to be significantly more
 *   efficient to include the factor of @math{2^8}. If this is unwanted, xs3_vect_s32_shr() can be used with a `b_shr`
 *   value of 8 to remove the scaling afterwards.
 * * The headroom of output vector @vector{a} is not returned by this function. The headroom of the output is always
 *   8 bits greater than the headroom of the input.
 * 
 * @param[out]  a           32-bit output vector @vector{a}
 * @param[in]   b           16-bit input vector @vector{b}
 * @param[in]   length      Number of elements in vectors @vector{a} and @vector{b}
 */
void xs3_vect_s16_to_s32(
    int32_t a[],
    const int16_t b[],
    const unsigned length);






#ifdef __XC__
}   //extern "C"
#endif

#endif //XS3_VECT_S16_H_