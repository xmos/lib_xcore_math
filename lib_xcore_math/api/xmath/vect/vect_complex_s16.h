// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include "xmath/xmath_conf.h"
#include "xmath/types.h"
#include "xmath/util.h"

#include "xmath/vect/vect_complex_s16_prepare.h"

/**
 * @defgroup vect_complex_s16_api   Complex 16-bit Vector Functions
 */

#ifdef __XC__
extern "C" {
#endif




/**
 * @brief Add one complex 16-bit vector to another.
 * 
 * `a_real[]` and `a_imag[]` together represent the complex 16-bit output mantissa vector
 * @vector{a}. Each @math{Re\\{a_k\\}} is `a_real[k]`, and each @math{Im\\{a_k\\}} is `a_imag[k]`.
 *
 * `b_real[]` and `b_imag[]` together represent the complex 16-bit input mantissa vector @vector{b}.
 * Each @math{Re\\{b_k\\}} is `b_real[k]`, and each @math{Im\\{b_k\\}} is `b_imag[k]`.
 *
 * `c_real[]` and `c_imag[]` together represent the complex 16-bit input mantissa vector @vector{c}.
 * Each @math{Re\\{c_k\\}} is `c_real[k]`, and each @math{Im\\{c_k\\}} is `c_imag[k]`.
 *
 * Each of the input vectors must begin at a word-aligned address. This operation can be performed
 * safely in-place on inputs `b_real[]`, `b_imag[]`, `c_real[]` and `c_imag[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 *
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b}
 * and @vector{c} respectively.
 * 
 * @operation{ 
 * &     b_k' \leftarrow sat_{16}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
 * &     c_k' \leftarrow sat_{16}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)     \\
 * &     Re\\{a_k\\} \leftarrow Re\\{b_k'\\} + Re\\{c_k'\\}                  \\
*  &     Im\\{a_k\\} \leftarrow Im\\{b_k'\\} + Im\\{c_k'\\}                  \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} and @vector{c} are the complex 16-bit mantissas of BFP vectors @math{ \bar{b} \cdot
 * 2^{b\_exp} } and @math{\bar{c} \cdot 2^{c\_exp}}, then the resulting vector @vector{a} are the
 * complex 16-bit mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}. 
 *
 * In this case, @math{b\_shr} and @math{c\_shr} **must** be chosen so that @math{a\_exp = b\_exp +
 * b\_shr = c\_exp + c\_shr}. Adding or subtracting mantissas only makes sense if they are
 * associated with the same exponent.
 *
 * The function vect_complex_s16_add_prepare() can be used to obtain values for @math{a\_exp},
 * @math{b\_shr} and @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and
 * the input headrooms @math{b\_hr} and @math{c\_hr}.
 * @endparblock
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
 * @exception ET_LOAD_STORE Raised if `a_real`, `a_imag`, `b_real`, `b_imag`, `c_real` or `c_imag` is not 
 *            word-aligned (See @ref note_vector_alignment)
 * 
 * @see vect_complex_s16_add_prepare
 * 
 * @ingroup vect_complex_s16_api
 */
C_API
headroom_t vect_complex_s16_add(
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
 * @brief Add a scalar to a complex 16-bit vector.
 * 
 * `a[]` and `b[]`represent the complex 16-bit mantissa vectors @vector{a} and @vector{b}
 * respectively. Each must begin at a word-aligned address. This operation can be performed safely
 * in-place on `b[]`.
 *
 * `c` is the complex scalar @math{c}to be added to each element of @vector{b}.
 *
 * `length` is the number of elements in each of the vectors.
 *
 * `b_shr` is the signed arithmetic right-shift applied to each element of @vector{b}.
 * 
 * @operation{ 
 * &     b_k' \leftarrow sat_{16}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
 * &     Re\\{a_k\\} \leftarrow Re\\{b_k'\\} + Re\\{c\\}                     \\
 * &     Im\\{a_k\\} \leftarrow Im\\{b_k'\\} + Im\\{c\\}                     \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 *
 * If elements of @vector{b} are the complex mantissas of BFP vector @math{ \bar{b} \cdot
 * 2^{b\_exp}}, and @math{c} is the mantissa of floating-point value @math{c \cdot 2^{c\_exp}}, then
 * the resulting vector @vector{a} are the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}. 
 *
 * In this case, @math{b\_shr} and @math{c\_shr} **must** be chosen so that @math{a\_exp = b\_exp +
 * b\_shr = c\_exp + c\_shr}. Adding or subtracting mantissas only makes sense if they are
 * associated with the same exponent.
 *
 * The function vect_complex_s16_add_scalar_prepare() can be used to obtain values for
 * @math{a\_exp}, @math{b\_shr} and @math{c\_shr} based on the input exponents @math{b\_exp} and
 * @math{c\_exp} and the input headrooms @math{b\_hr} and @math{c\_hr}.
 *
 * Note that @math{c\_shr} is an output of `vect_complex_s16_add_scalar_prepare()`, but is not a
 * parameter to this function. The @math{c\_shr} produced by `vect_complex_s16_add_scalar_prepare()`
 * is to be applied by the user, and the result passed as input `c`.
 * @endparblock
 * 
 * @param[out]      a_real      Real part of complex output vector @vector{a}
 * @param[out]      a_imag      Imaginary aprt of complex output vector @vector{a}
 * @param[in]       b_real      Real part of complex input vector @vector{b}
 * @param[in]       b_imag      Imaginary part of complex input vector @vector{b}
 * @param[in]       c           Complex input scalar @math{c}
 * @param[in]       length      Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]       b_shr       Right-shift applied to @vector{b}
 * 
 * @returns     Headroom of output vector @vector{a}.
 * 
 * @exception ET_LOAD_STORE Raised if `a` or `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @see vect_complex_s16_add_scalar_prepare
 * 
 * @ingroup vect_complex_s16_api
 */
C_API
headroom_t vect_complex_s16_add_scalar(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const complex_s16_t c,
    const unsigned length,
    const right_shift_t b_shr);


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
 * Each of the input vectors must begin at a word-aligned address. This operation can be performed
 * safely in-place on inputs `b_real[]`, `b_imag[]`, `c_real[]` and `c_imag[]`.
 *
 * `length` is the number of elements in each of the vectors.
 *
 * `a_shr` is the unsigned arithmetic right-shift applied to the 32-bit accumulators holding the
 * penultimate results.
 * 
 * @operation{ 
 * &     v_k = \leftarrow Re\\{b_k\\} \cdot Re\\{c_k\\}
 *                     + Im\\{b_k\\} \cdot Im\\{c_k\\}                      \\
 * &     s_k = \leftarrow Im\\{b_k\\} \cdot Re\\{c_k\\}
 *                     - Re\\{b_k\\} \cdot Im\\{c_k\\}                      \\
 * &     Re\\{a_k\\} \leftarrow  round( sat_{16}( v_k \cdot 2^{-a\_shr} ) )  \\
 * &     Im\\{a_k\\} \leftarrow  round( sat_{16}( s_k \cdot 2^{-a\_shr} ) )  \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * If @vector{b} are the complex 16-bit mantissas of a BFP vector @math{\bar{b} \cdot 2^{b\_exp}}
 * and @math{c} is the complex 16-bit mantissa of floating-point value @math{c \cdot 2^{c\_exp}},
 * then the resulting vector @vector{a} are the mantissas of BFP vector @math{\bar{a} \cdot
 * 2^{a\_exp}}, where @math{a\_exp = b\_exp + c\_exp + a\_shr}.
 *
 * The function vect_complex_s16_mul_prepare() can be used to obtain values for @math{a\_exp} and
 * @math{a\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and the input
 * headrooms @math{b\_hr} and @math{c\_hr}.
 * @endparblock 
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
 * @exception ET_LOAD_STORE Raised if `a_real`, `a_imag`, `b_real`, `b_imag`, `c_real` or `c_imag` is not 
 *            word-aligned (See @ref note_vector_alignment)
 * 
 * @see vect_complex_s16_mul_prepare
 * 
 * @ingroup vect_complex_s16_api
 */
C_API
headroom_t vect_complex_s16_conj_mul(
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
 * The headroom of an N-bit integer is the number of bits that the integer's value may be
 * left-shifted without any information being lost. Equivalently, it is one less than the number of
 * leading sign bits.
 *
 * The headroom of a `complex_s16_t` struct is the minimum of the headroom of each of its 16-bit
 * fields, `re` and `im`.
 *
 * The headroom of a `complex_s16_t` array is the minimum of the headroom of each of its
 * `complex_s16_t` elements.
 *
 * This function efficiently traverses the elements of @vector{x} to determine its headroom.
 *
 * `b_real[]` and `b_imag[]` together represent the complex 16-bit input mantissa vector @vector{b}.
 *
 * `length` is the number of elements in `b_real[]` and `b_imag[]`.
 * 
 * @operation{
 *      min\!\\{ HR_{16}\left(x_0\right), HR_{16}\left(x_1\right), ..., HR_{16}\left(x_{length-1}\right) \\}
 * }
 * 
 * @param[in]   b_real      Real part of complex input vector @vector{b}
 * @param[in]   b_imag      Imaginary part of complex input vector @vector{b}
 * @param[in]   length      Number of elements in @vector{x}
 * 
 * @returns     Headroom of vector @vector{x} 
 * 
 * @see vect_s16_headroom, 
 *      vect_s32_headroom, 
 *      vect_complex_s32_headroom
 * 
 * @ingroup vect_complex_s16_api
 */
C_API
headroom_t vect_complex_s16_headroom(
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
 * Each of the input vectors must begin at a word-aligned address. This operation can be performed
 * safely in-place on inputs `b_real[]` or `b_imag[]`.
 *
 * `length` is the number of elements in each of the vectors.
 *
 * `b_shr` is the signed arithmetic right-shift applied to elements of @vector{b}.
 *
 * `rot_table` must point to a pre-computed table of complex vectors used in calculating the
 * magnitudes. `table_rows` is the number of rows in the table. This library is distributed with a
 * default version of the required rotation table. The following symbols can be used to refer to it
 * in user code:
 * 
 * @code
 *     const extern unsigned rot_table16_rows;
 *     const extern complex_s16_t rot_table16[30][4];
 * @endcode
 * 
 * Faster computation (with reduced precision) can be achieved by generating a smaller version of
 * the table. A python script is provided to generate this table.
 * 
 * @operation{ 
 * &     v_k \leftarrow b_k \cdot 2^{-b\_shr}    \\
 * &     a_k \leftarrow \sqrt { {\left( Re\\{v_k\\} \right)}^2 + {\left( Im\\{v_k\\} \right)}^2 } \\
 * &       \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 *
 * If @vector{b} are the complex 16-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} },
 * then the resulting vector @vector{a} are the real 16-bit mantissas of BFP vector @math{\bar{a}
 * \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp + b\_shr}.
 *
 * The function vect_complex_s16_mag_prepare() can be used to obtain values for @math{a\_exp} and
 * @math{b\_shr} based on the input exponent @math{b\_exp} and headroom @math{b\_hr}.
 * @endparblock
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
 * @exception ET_LOAD_STORE Raised if `a`, `b_real` or `b_imag` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @see vect_complex_s16_mag_prepare
 * 
 * @ingroup vect_complex_s16_api
 */
C_API
headroom_t vect_complex_s16_mag(
    int16_t a[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const unsigned length,
    const right_shift_t b_shr,
    const int16_t* rot_table,
    const unsigned table_rows);


/**
 * @brief Multiply one complex 16-bit vector element-wise by another, and add the result to an
 * accumulator.
 *
 * `acc_real[]` and `acc_imag[]` together represent the complex 16-bit accumulator mantissa vector
 * @vector{a}. Each @math{Re\\{a_k\\}} is `acc_real[k]`, and each @math{Im\\{a_k\\}} is
 * `acc_imag[k]`.
 *
 * `b_real[]` and `b_imag[]` together represent the complex 16-bit input mantissa vector @vector{b}.
 * Each @math{Re\\{b_k\\}} is `b_real[k]`, and each @math{Im\\{b_k\\}} is `b_imag[k]`.
 *
 * `c_real[]` and `c_imag[]` together represent the complex 16-bit input mantissa vector @vector{c}.
 * Each @math{Re\\{c_k\\}} is `c_real[k]`, and each @math{Im\\{c_k\\}} is `c_imag[k]`.
 *
 * Each of the input vectors must begin at a word-aligned address.
 *
 * `length` is the number of elements in each of the vectors.
 *
 * `acc_shr` is the signed arithmetic right-shift applied to the accumulators @math{a_k}.
 *
 * `bc_sat` is the unsigned arithmetic right-shift applied to the product of @math{b_k} and
 * @math{c_k} before being added to the accumulator.
 * 
 * @operation{
 * &     v_k \leftarrow Re\\{b_k\\} \cdot Re\\{c_k\\}
 *                    - Im\\{b_k\\} \cdot Im\\{c_k\\}                      \\
 * &     s_k \leftarrow Im\\{b_k\\} \cdot Re\\{c_k\\}
 *                    + Re\\{b_k\\} \cdot Im\\{c_k\\}                      \\
 * &     \hat{a}_k \leftarrow sat_{16}( a_k \cdot 2^{-acc\_shr} )          \\
 * &     Re\\{a_k\\} \leftarrow sat_{16}( Re\\{\hat{a}_k\\} + round( sat_{16}( v_k \cdot 2^{-bc\_sat} ) ) ) \\
 * &     Im\\{a_k\\} \leftarrow sat_{16}( Im\\{\hat{a}_k\\} + round( sat_{16}( s_k \cdot 2^{-bc\_sat} ) ) ) \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If inputs @vector{b} and @vector{c} are the mantissas of BFP vectors @math{ \bar{b} \cdot
 * 2^{b\_exp} } and @math{\bar{c} \cdot 2^{c\_exp}}, and input @vector{a} is the accumulator BFP
 * vector @math{\bar{a} \cdot 2^{a\_exp}}, then the output values of @vector{a} have the exponent
 * @math{2^{a\_exp + acc\_shr}}.
 *
 * For accumulation to make sense mathematically, @math{bc\_sat} must be chosen such that @math{
 * a\_exp + acc\_shr = b\_exp + c\_exp + bc\_sat }.
 *
 * The function vect_complex_s16_macc_prepare() can be used to obtain values for @math{a\_exp},
 * @math{acc\_shr} and @math{bc\_sat} based on the input exponents @math{a\_exp}, @math{b\_exp} and
 * @math{c\_exp} and the input headrooms @math{a\_hr}, @math{b\_hr} and @math{c\_hr}.
 * @endparblock
 * 
 * @param[inout]    acc_real    Real part of complex accumulator @vector{a}
 * @param[inout]    acc_imag    Imaginary aprt of complex accumulator @vector{a}
 * @param[in]       b_real      Real part of complex input vector @vector{b}
 * @param[in]       b_imag      Imaginary part of complex input vector @vector{b}
 * @param[in]       c_real      Real part of complex input vector @vector{c}
 * @param[in]       c_imag      Imaginary part of complex input vector @vector{c} 
 * @param[in]       length      Number of elements in vectors @vector{a}, @vector{b} and @vector{c}
 * @param[in]       acc_shr     Signed arithmetic right-shift applied to accumulator elements.
 * @param[in]       bc_sat      Unsigned arithmetic right-shift applied to the products of elements @math{b_k} 
 *                              and @math{c_k}
 * 
 * @returns   Headroom of the output vector @vector{a}
 *
 * @exception ET_LOAD_STORE Raised if `acc_real`, `acc_imag`, `b_real`, `b_imag`, `c_real` or 
 *                          `c_imag` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @see vect_complex_s16_macc_prepare
 * 
 * @ingroup vect_complex_s16_api
 */
C_API
headroom_t vect_complex_s16_macc(
    int16_t acc_real[],
    int16_t acc_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const int16_t c_real[],
    const int16_t c_imag[],
    const unsigned length,
    const right_shift_t acc_shr,
    const right_shift_t bc_sat);


/**
 * @brief Multiply one complex 16-bit vector element-wise by another, and subtract the result from
 * an accumulator.
 * 
 * `acc_real[]` and `acc_imag[]` together represent the complex 16-bit accumulator mantissa vector
 * @vector{a}. Each @math{Re\\{a_k\\}} is `acc_real[k]`, and each @math{Im\\{a_k\\}} is
 * `acc_imag[k]`.
 *
 * `b_real[]` and `b_imag[]` together represent the complex 16-bit input mantissa vector @vector{b}.
 * Each @math{Re\\{b_k\\}} is `b_real[k]`, and each @math{Im\\{b_k\\}} is `b_imag[k]`.
 *
 * `c_real[]` and `c_imag[]` together represent the complex 16-bit input mantissa vector @vector{c}.
 * Each @math{Re\\{c_k\\}} is `c_real[k]`, and each @math{Im\\{c_k\\}} is `c_imag[k]`.
 *
 * Each of the input vectors must begin at a word-aligned address.
 *
 * `length` is the number of elements in each of the vectors.
 *
 * `acc_shr` is the signed arithmetic right-shift applied to the accumulators @math{a_k}.
 *
 * `bc_sat` is the unsigned arithmetic right-shift applied to the product of @math{b_k} and
 * @math{c_k} before being subtracted from the accumulator.
 * 
 * @operation{
 * &     v_k \leftarrow Re\\{b_k\\} \cdot Re\\{c_k\\}
 *                    - Im\\{b_k\\} \cdot Im\\{c_k\\}                      \\
 * &     s_k \leftarrow Im\\{b_k\\} \cdot Re\\{c_k\\}
 *                    + Re\\{b_k\\} \cdot Im\\{c_k\\}                      \\
 * &     \hat{a}_k \leftarrow sat_{16}( a_k \cdot 2^{-acc\_shr} )            \\
 * &     Re\\{a_k\\} \leftarrow sat_{16}( Re\\{\hat{a}_k\\} - round( sat_{16}( v_k \cdot 2^{-bc\_sat} ) ) ) \\
 * &     Im\\{a_k\\} \leftarrow sat_{16}( Im\\{\hat{a}_k\\} - round( sat_{16}( s_k \cdot 2^{-bc\_sat} ) ) ) \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If inputs @vector{b} and @vector{c} are the mantissas of BFP vectors @math{ \bar{b} \cdot
 * 2^{b\_exp} } and @math{\bar{c} \cdot 2^{c\_exp}}, and input @vector{a} is the accumulator BFP
 * vector @math{\bar{a} \cdot 2^{a\_exp}}, then the output values of @vector{a} have the exponent
 * @math{2^{a\_exp + acc\_shr}}.
 *
 * For accumulation to make sense mathematically, @math{bc\_sat} must be chosen such that @math{
 * a\_exp + acc\_shr = b\_exp + c\_exp + bc\_sat }.
 *
 * The function vect_complex_s16_nmacc_prepare() can be used to obtain values for @math{a\_exp},
 * @math{acc\_shr} and @math{bc\_sat} based on the input exponents @math{a\_exp}, @math{b\_exp} and
 * @math{c\_exp} and the input headrooms @math{a\_hr}, @math{b\_hr} and @math{c\_hr}.
 * @endparblock
 * 
 * @param[inout]    acc_real    Real part of complex accumulator @vector{a}
 * @param[inout]    acc_imag    Imaginary aprt of complex accumulator @vector{a}
 * @param[in]       b_real      Real part of complex input vector @vector{b}
 * @param[in]       b_imag      Imaginary part of complex input vector @vector{b}
 * @param[in]       c_real      Real part of complex input vector @vector{c}
 * @param[in]       c_imag      Imaginary part of complex input vector @vector{c} 
 * @param[in]       length      Number of elements in vectors @vector{a}, @vector{b} and @vector{c}
 * @param[in]       acc_shr     Signed arithmetic right-shift applied to accumulator elements.
 * @param[in]       bc_sat      Unsigned arithmetic right-shift applied to the products of elements @math{b_k} 
 *                              and @math{c_k}
 * 
 * @returns   Headroom of the output vector @vector{a}
 *
 * @exception ET_LOAD_STORE Raised if `acc_real`, `acc_imag`, `b_real`, `b_imag`, `c_real` or 
 *                          `c_imag` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @see vect_complex_s16_nmacc_prepare
 * 
 * @ingroup vect_complex_s16_api
 */
C_API
headroom_t vect_complex_s16_nmacc(
    int16_t acc_real[],
    int16_t acc_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const int16_t c_real[],
    const int16_t c_imag[],
    const unsigned length,
    const right_shift_t acc_shr,
    const right_shift_t bc_sat);


/**
 * @brief Multiply one complex 16-bit vector element-wise by the complex conjugate of another, and
 * add the result to an accumulator.
 *
 * `acc_real[]` and `acc_imag[]` together represent the complex 16-bit accumulator mantissa vector
 * @vector{a}. Each @math{Re\\{a_k\\}} is `acc_real[k]`, and each @math{Im\\{a_k\\}} is
 * `acc_imag[k]`.
 *
 * `b_real[]` and `b_imag[]` together represent the complex 16-bit input mantissa vector @vector{b}.
 * Each @math{Re\\{b_k\\}} is `b_real[k]`, and each @math{Im\\{b_k\\}} is `b_imag[k]`.
 *
 * `c_real[]` and `c_imag[]` together represent the complex 16-bit input mantissa vector @vector{c}.
 * Each @math{Re\\{c_k\\}} is `c_real[k]`, and each @math{Im\\{c_k\\}} is `c_imag[k]`.
 *
 * Each of the input vectors must begin at a word-aligned address.
 *
 * `length` is the number of elements in each of the vectors.
 *
 * `acc_shr` is the signed arithmetic right-shift applied to the accumulators @math{a_k}.
 *
 * `bc_sat` is the unsigned arithmetic right-shift applied to the product of @math{b_k} and
 * @math{c_k^*} before being added to the accumulator.
 * 
 * @operation{
 * &     v_k \leftarrow Re\\{b_k\\} \cdot Re\\{c_k\\}
 *                    + Im\\{b_k\\} \cdot Im\\{c_k\\}                      \\
 * &     s_k \leftarrow Im\\{b_k\\} \cdot Re\\{c_k\\}
 *                    - Re\\{b_k\\} \cdot Im\\{c_k\\}                      \\
 * &     \hat{a}_k \leftarrow sat_{16}( a_k \cdot 2^{-acc\_shr} )            \\
 * &     Re\\{a_k\\} \leftarrow sat_{16}( Re\\{\hat{a}_k\\} + round( sat_{16}( v_k \cdot 2^{-bc\_sat} ) ) ) \\
 * &     Im\\{a_k\\} \leftarrow sat_{16}( Im\\{\hat{a}_k\\} + round( sat_{16}( s_k \cdot 2^{-bc\_sat} ) ) ) \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If inputs @vector{b} and @vector{c} are the mantissas of BFP vectors @math{ \bar{b} \cdot
 * 2^{b\_exp} } and @math{\bar{c} \cdot 2^{c\_exp}}, and input @vector{a} is the accumulator BFP
 * vector @math{\bar{a} \cdot 2^{a\_exp}}, then the output values of @vector{a} have the exponent
 * @math{2^{a\_exp + acc\_shr}}.
 *
 * For accumulation to make sense mathematically, @math{bc\_sat} must be chosen such that @math{
 * a\_exp + acc\_shr = b\_exp + c\_exp + bc\_sat }.
 *
 * The function vect_complex_s16_macc_prepare() can be used to obtain values for @math{a\_exp},
 * @math{acc\_shr} and @math{bc\_sat} based on the input exponents @math{a\_exp}, @math{b\_exp} and
 * @math{c\_exp} and the input headrooms @math{a\_hr}, @math{b\_hr} and @math{c\_hr}.
 * @endparblock
 * 
 * @param[inout]    acc_real    Real part of complex accumulator @vector{a}
 * @param[inout]    acc_imag    Imaginary aprt of complex accumulator @vector{a}
 * @param[in]       b_real      Real part of complex input vector @vector{b}
 * @param[in]       b_imag      Imaginary part of complex input vector @vector{b}
 * @param[in]       c_real      Real part of complex input vector @vector{c}
 * @param[in]       c_imag      Imaginary part of complex input vector @vector{c} 
 * @param[in]       length      Number of elements in vectors @vector{a}, @vector{b} and @vector{c}
 * @param[in]       acc_shr     Signed arithmetic right-shift applied to accumulator elements.
 * @param[in]       bc_sat      Unsigned arithmetic right-shift applied to the products of elements @math{b_k} 
 *                              and @math{c_k^*}
 * 
 * @returns   Headroom of the output vector @vector{a}
 *
 * @exception ET_LOAD_STORE Raised if `acc_real`, `acc_imag`, `b_real`, `b_imag`, `c_real` or 
 *                          `c_imag` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @see vect_complex_s16_conj_macc_prepare
 * 
 * @ingroup vect_complex_s16_api
 */
C_API
headroom_t vect_complex_s16_conj_macc(
    int16_t acc_real[],
    int16_t acc_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const int16_t c_real[],
    const int16_t c_imag[],
    const unsigned length,
    const right_shift_t acc_shr,
    const right_shift_t bc_sat);


/**
 * @brief Multiply one complex 16-bit vector element-wise by the complex conjugate of another, and
 * subtract the result from an accumulator.
 *
 * `acc_real[]` and `acc_imag[]` together represent the complex 16-bit accumulator mantissa vector
 * @vector{a}. Each @math{Re\\{a_k\\}} is `acc_real[k]`, and each @math{Im\\{a_k\\}} is
 * `acc_imag[k]`.
 *
 * `b_real[]` and `b_imag[]` together represent the complex 16-bit input mantissa vector @vector{b}.
 * Each @math{Re\\{b_k\\}} is `b_real[k]`, and each @math{Im\\{b_k\\}} is `b_imag[k]`.
 *
 * `c_real[]` and `c_imag[]` together represent the complex 16-bit input mantissa vector @vector{c}.
 * Each @math{Re\\{c_k\\}} is `c_real[k]`, and each @math{Im\\{c_k\\}} is `c_imag[k]`.
 *
 * Each of the input vectors must begin at a word-aligned address.
 *
 * `length` is the number of elements in each of the vectors.
 *
 * `acc_shr` is the signed arithmetic right-shift applied to the accumulators @math{a_k}.
 *
 * `bc_sat` is the unsigned arithmetic right-shift applied to the product of @math{b_k} and
 * @math{c_k^*} before being subtracted from the accumulator.
 * 
 * @operation{
 * &     v_k \leftarrow Re\\{b_k\\} \cdot Re\\{c_k\\}
 *                    + Im\\{b_k\\} \cdot Im\\{c_k\\}                      \\
 * &     s_k \leftarrow Im\\{b_k\\} \cdot Re\\{c_k\\}
 *                    - Re\\{b_k\\} \cdot Im\\{c_k\\}                      \\
 * &     \hat{a}_k \leftarrow sat_{16}( a_k \cdot 2^{-acc\_shr} )            \\
 * &     Re\\{a_k\\} \leftarrow sat_{16}( Re\\{\hat{a}_k\\} - round( sat_{16}( v_k \cdot 2^{-bc\_sat} ) ) ) \\
 * &     Im\\{a_k\\} \leftarrow sat_{16}( Im\\{\hat{a}_k\\} - round( sat_{16}( s_k \cdot 2^{-bc\_sat} ) ) ) \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If inputs @vector{b} and @vector{c} are the mantissas of BFP vectors @math{ \bar{b} \cdot
 * 2^{b\_exp} } and @math{\bar{c} \cdot 2^{c\_exp}}, and input @vector{a} is the accumulator BFP
 * vector @math{\bar{a} \cdot 2^{a\_exp}}, then the output values of @vector{a} have the exponent
 * @math{2^{a\_exp + acc\_shr}}.
 *
 * For accumulation to make sense mathematically, @math{bc\_sat} must be chosen such that @math{
 * a\_exp + acc\_shr = b\_exp + c\_exp + bc\_sat }.
 *
 * The function vect_complex_s16_macc_prepare() can be used to obtain values for @math{a\_exp},
 * @math{acc\_shr} and @math{bc\_sat} based on the input exponents @math{a\_exp}, @math{b\_exp} and
 * @math{c\_exp} and the input headrooms @math{a\_hr}, @math{b\_hr} and @math{c\_hr}.
 * @endparblock
 * 
 * @param[inout]    acc_real    Real part of complex accumulator @vector{a}
 * @param[inout]    acc_imag    Imaginary aprt of complex accumulator @vector{a}
 * @param[in]       b_real      Real part of complex input vector @vector{b}
 * @param[in]       b_imag      Imaginary part of complex input vector @vector{b}
 * @param[in]       c_real      Real part of complex input vector @vector{c}
 * @param[in]       c_imag      Imaginary part of complex input vector @vector{c} 
 * @param[in]       length      Number of elements in vectors @vector{a}, @vector{b} and @vector{c}
 * @param[in]       acc_shr     Signed arithmetic right-shift applied to accumulator elements.
 * @param[in]       bc_sat      Unsigned arithmetic right-shift applied to the products of elements @math{b_k} 
 *                              and @math{c_k^*}
 * 
 * @returns   Headroom of the output vector @vector{a}
 *
 * @exception ET_LOAD_STORE Raised if `acc_real`, `acc_imag`, `b_real`, `b_imag`, `c_real` or 
 *                          `c_imag` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @see vect_complex_s16_conj_nmacc_prepare
 * 
 * @ingroup vect_complex_s16_api
 */
C_API
headroom_t vect_complex_s16_conj_nmacc(
    int16_t acc_real[],
    int16_t acc_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const int16_t c_real[],
    const int16_t c_imag[],
    const unsigned length,
    const right_shift_t acc_shr,
    const right_shift_t bc_sat);


/**
 * @brief Multiply one complex 16-bit vector element-wise by another.
 * 
 * `a_real[]` and `a_imag[]` together represent the complex 16-bit output mantissa vector
 * @vector{a}. Each @math{Re\\{a_k\\}} is `a_real[k]`, and each @math{Im\\{a_k\\}} is `a_imag[k]`.
 *
 * `b_real[]` and `b_imag[]` together represent the complex 16-bit input mantissa vector @vector{b}.
 * Each @math{Re\\{b_k\\}} is `b_real[k]`, and each @math{Im\\{b_k\\}} is `b_imag[k]`.
 *
 * `c_real[]` and `c_imag[]` together represent the complex 16-bit input mantissa vector @vector{c}.
 * Each @math{Re\\{c_k\\}} is `c_real[k]`, and each @math{Im\\{c_k\\}} is `c_imag[k]`.
 *
 * Each of the input vectors must begin at a word-aligned address. This operation can be performed
 * safely in-place on inputs `b_real[]`, `b_imag[]`, `c_real[]` and `c_imag[]`.
 *
 * `length` is the number of elements in each of the vectors.
 *
 * `a_shr` is the unsigned arithmetic right-shift applied to the 32-bit accumulators holding
 * intermediate results.
 * 
 * @operation{ 
 * &     v_k = \leftarrow Re\\{b_k\\} \cdot Re\\{c_k\\}
 *                     - Im\\{b_k\\} \cdot Im\\{c_k\\}                      \\
 * &     s_k = \leftarrow Im\\{b_k\\} \cdot Re\\{c_k\\}
 *                     + Re\\{b_k\\} \cdot Im\\{c_k\\}                      \\
 * &     Re\\{a_k\\} \leftarrow  round( sat_{16}( v_k \cdot 2^{-a\_shr} ) )  \\
 * &     Im\\{a_k\\} \leftarrow  round( sat_{16}( s_k \cdot 2^{-a\_shr} ) )  \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * If @vector{b} are the complex 16-bit mantissas of a BFP vector @math{\bar{b} \cdot 2^{b\_exp}}
 * and @math{c} is the complex 16-bit mantissa of floating-point value @math{c \cdot 2^{c\_exp}},
 * then the resulting vector @vector{a} are the mantissas of BFP vector @math{\bar{a} \cdot
 * 2^{a\_exp}}, where @math{a\_exp = b\_exp + c\_exp + a\_shr}.
 *
 * The function vect_complex_s16_mul_prepare() can be used to obtain values for @math{a\_exp} and
 * @math{a\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and the input
 * headrooms @math{b\_hr} and @math{c\_hr}.
 * @endparblock 
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
 * @exception ET_LOAD_STORE Raised if `a_real`, `a_imag`, `b_real`, `b_imag`, `c_real` or `c_imag`
 *            is not word-aligned (See @ref note_vector_alignment)
 * 
 * @see vect_complex_s16_mul_prepare
 * 
 * @ingroup vect_complex_s16_api
 */
C_API
headroom_t vect_complex_s16_mul(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const int16_t c_real[],
    const int16_t c_imag[],
    const unsigned length,
    const right_shift_t a_shr);


/**
 * @brief Multiply a complex 16-bit vector element-wise by a real 16-bit vector.
 *
 * `a_real[]` and `a_imag[]` together represent the complex 16-bit output mantissa vector
 * @vector{a}. Each @math{Re\\{a_k\\}} is `a_real[k]`, and each @math{Im\\{a_k\\}} is `a_imag[k]`.
 *
 * `b_real[]` and `b_imag[]` together represent the complex 16-bit input mantissa vector @vector{b}.
 * Each @math{Re\\{b_k\\}} is `b_real[k]`, and each @math{Im\\{b_k\\}} is `b_imag[k]`.
 *
 * `c_real[]` represents the real 16-bit input mantissa vector @vector{c}.
 *
 * Each of the input vectors must begin at a word-aligned address. This operation can be performed
 * safely in-place on inputs `b_real[]`, `b_imag[]` and `c_real[]`.
 *
 * `length` is the number of elements in each of the vectors.
 *
 * `a_shr` is the unsigned arithmetic right-shift applied to the 32-bit accumulators holding the
 * penultimate results.
 * 
 * @operation{ 
 * &     v_k = \leftarrow Re\\{b_k\\} \cdot c_k                              \\
 * &     s_k = \leftarrow Im\\{b_k\\} \cdot c_k                              \\
 * &     Re\\{a_k\\} \leftarrow  round( sat_{16}( v_k \cdot 2^{-a\_shr} ) )  \\
 * &     Im\\{a_k\\} \leftarrow  round( sat_{16}( s_k \cdot 2^{-a\_shr} ) )  \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the complex 16-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} }
 * and @math{c} is the complex 16-bit mantissa of floating-point value @math{c \cdot 2^{c\_exp}},
 * then the resulting vector @vector{a} are the mantissas of BFP vector @math{\bar{a} \cdot
 * 2^{a\_exp}}, where @math{a\_exp = b\_exp + c\_exp + a\_shr}.
 *
 * The function vect_s16_real_mul_prepare() can be used to obtain values for @math{a\_exp} and
 * @math{a\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and the input
 * headrooms @math{b\_hr} and @math{c\_hr}. 
 * @endparblock
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
 * @exception ET_LOAD_STORE Raised if `a_real`, `a_imag`, `b_real`, `b_imag` or `c_real` is not 
 *            word-aligned (See @ref note_vector_alignment)
 * 
 * @see vect_complex_s16_real_mul_prepare
 * 
 * @ingroup vect_complex_s16_api
 */
C_API
headroom_t vect_complex_s16_real_mul(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const int16_t c_real[],
    const unsigned length,
    const right_shift_t a_shr);


/**
 * @brief Multiply a complex 16-bit vector by a real scalar.
 * 
 * `a_real[]` and `a_imag[]` together represent the complex 16-bit output mantissa vector
 * @vector{a}. Each @math{Re\\{a_k\\}} is `a_real[k]`, and each @math{Im\\{a_k\\}} is `a_imag[k]`.
 *
 * `b_real[]` and `b_imag[]` together represent the complex 16-bit input mantissa vector @vector{b}.
 * Each @math{Re\\{b_k\\}} is `b_real[k]`, and each @math{Im\\{b_k\\}} is `b_imag[k]`.
 *
 * Each of the input vectors must begin at a word-aligned address. This operation can be performed
 * safely in-place on inputs `b_real[]` and `b_imag[]`.
 *
 * `c` is the real 16-bit input mantissa @math{c}.
 *
 * `length` is the number of elements in each of the vectors.
 *
 * `a_shr` is an unsigned arithmetic right-shift applied to the 32-bit accumulators holding the
 * penultimate results.
 * 
 * @operation{
 * &     v_k = \leftarrow Re\\{b_k\\} \cdot c                                \\
 * &     s_k = \leftarrow Im\\{b_k\\} \cdot c                                \\
 * &     Re\\{a_k\\} \leftarrow  round( sat_{16}( v_k \cdot 2^{-a\_shr} ) )  \\
 * &     Im\\{a_k\\} \leftarrow  round( sat_{16}( s_k \cdot 2^{-a\_shr} ) )  \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1)
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the complex 16-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} }
 * and @math{c} is the complex 16-bit mantissa of floating-point value @math{c \cdot 2^{c\_exp}},
 * then the resulting vector @vector{a} are the mantissas of BFP vector @math{\bar{a} \cdot
 * 2^{a\_exp}}, where @math{a\_exp = b\_exp + c\_exp + a\_shr}.
 *
 * The function vect_complex_s16_real_scale_prepare() can be used to obtain values for @math{a\_exp}
 * and @math{a\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and the input
 * headrooms @math{b\_hr} and @math{c\_hr}. 
 * @endparblock
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
 * 
 * @exception ET_LOAD_STORE Raised if `a_real`, `a_imag`, `b_real`, `b_imag` or `c` is not 
 *            word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_complex_s16_api
 */
C_API
headroom_t vect_complex_s16_real_scale(
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
 * `a_real[]` and `a_imag[]` together represent the complex 16-bit output mantissa vector
 * @vector{a}. Each @math{Re\\{a_k\\}} is `a_real[k]`, and each @math{Im\\{a_k\\}} is `a_imag[k]`.
 *
 * `b_real[]` and `b_imag[]` together represent the complex 16-bit input mantissa vector @vector{b}.
 * Each @math{Re\\{b_k\\}} is `b_real[k]`, and each @math{Im\\{b_k\\}} is `b_imag[k]`.
 *
 * Each of the input vectors must begin at a word-aligned address. This operation can be performed
 * safely in-place on inputs `b_real[]` and `b_imag[]`.
 *
 * `c_real` and `c_imag` are the real and imaginary parts of the complex 16-bit input mantissa
 * @math{c}.
 *
 * `length` is the number of elements in each of the vectors.
 *
 * `a_shr` is the unsigned arithmetic right-shift applied to the 32-bit accumulators holding the
 * penultimate results.
 * 
 * @operation{
 * &     v_k = \leftarrow Re\\{b_k\\} \cdot Re\\{c\\}  
 *                      - Im\\{b_k\\} \cdot Im\\{c\\}                        \\
 * &     s_k = \leftarrow Im\\{b_k\\} \cdot Re\\{c\\}  
 *                      + Re\\{b_k\\} \cdot Im\\{c\\}                        \\
 * &     Re\\{a_k\\} \leftarrow  round( sat_{16}( v_k \cdot 2^{-a\_shr} ) )  \\
 * &     Im\\{a_k\\} \leftarrow  round( sat_{16}( s_k \cdot 2^{-a\_shr} ) )  \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1)
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the complex 16-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} }
 * and @math{c} is the complex 16-bit mantissa of floating-point value @math{c \cdot 2^{c\_exp}},
 * then the resulting vector @vector{a} are the mantissas of BFP vector @math{\bar{a} \cdot
 * 2^{a\_exp}}, where @math{a\_exp = b\_exp + c\_exp + a\_shr}.
 *
 * The function vect_complex_s16_scale_prepare() can be used to obtain values for @math{a\_exp} and
 * @math{a\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and the input
 * headrooms @math{b\_hr} and @math{c\_hr}. 
 * @endparblock
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
 * 
 * @exception ET_LOAD_STORE Raised if `a_real`, `a_imag`, `b_real`, `b_imag`, `c_real` or `c_imag` is not 
 *            word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_complex_s16_api
 */
C_API
headroom_t vect_complex_s16_scale(
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
 * `a_real[]` and `a_imag[]` together represent the complex 16-bit output mantissa vector
 * @vector{a}. Each @math{Re\\{a_k\\}} is `a_real[k]`, and each @math{Im\\{a_k\\}} is `a_imag[k]`.
 * Each must begin at a word-aligned address.
 *
 * `b_real` and `b_imag` are the real and imaginary parts of the complex 16-bit input mantissa
 * @math{b}. Each `a_real[k]` will be set to `b_real`. Each `a_imag[k]` will be set to `b_imag`.
 *
 * `length` is the number of elements in `a_real[]` and `a_imag[]`.
 * 
 * @operation{ 
 * &     Re\\{a_k\\} \leftarrow Re\\{b\\}                \\
 * &     Im\\{a_k\\} \leftarrow Im\\{b\\}                \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1)
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @math{b} is the mantissa of floating-point value @math{b \cdot 2^{b\_exp}}, then the output
 * vector @vector{a} are the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where
 * @math{a\_exp = b\_exp}.
 * @endparblock
 * 
 * @param[out]      a_real      Real part of complex output vector @vector{a}
 * @param[out]      a_imag      Imaginary aprt of complex output vector @vector{a}
 * @param[in]       b_real      Real part of complex input scalar @math{b}
 * @param[in]       b_imag      Imaginary part of complex input scalar @math{b}
 * @param[in]       length      Number of elements in vectors @vector{a} and @vector{b}
 * 
 * @exception ET_LOAD_STORE Raised if `a_real` or `a_imag` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_complex_s16_api
 */
C_API
void vect_complex_s16_set(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real,
    const int16_t b_imag,
    const unsigned length);


/**
 * @brief Left-shift each element of a complex 16-bit vector by a specified number of bits.
 *
 * `a_real[]` and `a_imag[]` together represent the complex 16-bit output mantissa vector
 * @vector{a}. Each @math{Re\\{a_k\\}} is `a_real[k]`, and each @math{Im\\{a_k\\}} is `a_imag[k]`.
 *
 * `b_real[]` and `b_imag[]` together represent the complex 16-bit input mantissa vector @vector{b}.
 * Each @math{Re\\{b_k\\}} is `b_real[k]`, and each @math{Im\\{b_k\\}} is `b_imag[k]`.
 *
 * Each of the input vectors must begin at a word-aligned address. This operation can be performed
 * safely in-place on inputs `b_real[]` and `b_imag[]`.
 *
 * `length` is the number of elements in @vector{a} and @vector{b}.
 *
 * `b_shl` is the signed arithmetic left-shift applied to each element of @vector{b}.
 * 
 * @operation{
 * &     Re\\{a_k\\} \leftarrow sat_{16}(\lfloor Re\\{b_k\\} \cdot 2^{b\_shl} \rfloor)     \\
 * &     Im\\{a_k\\} \leftarrow sat_{16}(\lfloor Im\\{b_k\\} \cdot 2^{b\_shl} \rfloor)     \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the complex 16-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} },
 * then the resulting vector @vector{a} are the complex 16-bit mantissas of BFP vector @math{\bar{a}
 * \cdot 2^{a\_exp}}, where @math{\bar{a} = \bar{b} \cdot 2^{b\_shl}} and @math{a\_exp = b\_exp}.
 * @endparblock
 * 
 * @param[out] a_real       Real part of complex output vector @vector{a}
 * @param[out] a_imag       Imaginary aprt of complex output vector @vector{a}
 * @param[in]  b_real       Real part of complex input vector @vector{b}
 * @param[in]  b_imag       Imaginary part of complex input vector @vector{b}
 * @param[in]  length       Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]  b_shl        Left-shift applied to @vector{b} 
 * 
 * @returns     Headroom of the output vector @vector{a}
 * 
 * @exception ET_LOAD_STORE Raised if `a_real`, `a_imag`, `b_real` or `b_imag` is not 
 *            word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_complex_s16_api
 */
C_API
headroom_t vect_complex_s16_shl(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const unsigned length,
    const left_shift_t b_shl);


/**
 * @brief Right-shift each element of a complex 16-bit vector by a specified number of bits.
 *
 * `a_real[]` and `a_imag[]` together represent the complex 16-bit output mantissa vector
 * @vector{a}. Each @math{Re\\{a_k\\}} is `a_real[k]`, and each @math{Im\\{a_k\\}} is `a_imag[k]`.
 *
 * `b_real[]` and `b_imag[]` together represent the complex 16-bit input mantissa vector @vector{b}.
 * Each @math{Re\\{b_k\\}} is `b_real[k]`, and each @math{Im\\{b_k\\}} is `b_imag[k]`.
 *
 * Each of the input vectors must begin at a word-aligned address. This operation can be performed
 * safely in-place on inputs `b_real[]` and `b_imag[]`.
 *
 * `length` is the number of elements in @vector{a} and @vector{b}.
 *
 * `b_shr` is the signed arithmetic right-shift applied to each element of @vector{b}.
 * 
 * @operation{
 * &     Re\\{a_k\\} \leftarrow sat_{16}(\lfloor Re\\{b_k\\} \cdot 2^{-b\_shr} \rfloor)     \\
 * &     Im\\{a_k\\} \leftarrow sat_{16}(\lfloor Im\\{b_k\\} \cdot 2^{-b\_shr} \rfloor)     \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the complex 16-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} },
 * then the resulting vector @vector{a} are the complex 16-bit mantissas of BFP vector @math{\bar{a}
 * \cdot 2^{a\_exp}}, where @math{\bar{a} = \bar{b} \cdot 2^{-b\_shr}} and @math{a\_exp = b\_exp}.
 * @endparblock
 * 
 * @param[out] a_real       Real part of complex output vector @vector{a}
 * @param[out] a_imag       Imaginary aprt of complex output vector @vector{a}
 * @param[in]  b_real       Real part of complex input vector @vector{b}
 * @param[in]  b_imag       Imaginary part of complex input vector @vector{b}
 * @param[in]  length       Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]  b_shr        Right-shift applied to @vector{b} 
 * 
 * @returns     Headroom of the output vector @vector{a}
 * 
 * @exception ET_LOAD_STORE Raised if `a_real`, `a_imag`, `b_real` or `b_imag` is not 
 *            word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_complex_s16_api
 */
C_API
headroom_t vect_complex_s16_shr(
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
 * `a_shr` is the unsigned arithmetic right-shift applied to the 32-bit accumulators holding the
 * penultimate results.
 * 
 * @operation{ 
 * &     a_k \leftarrow ((Re\\{b_k'\\})^2 + (Im\\{b_k'\\})^2)\cdot 2^{-a\_shr}   \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1)
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the complex 16-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} },
 * then the resulting vector @vector{a} are the real 16-bit mantissas of BFP vector @math{\bar{a}
 * \cdot 2^{a\_exp}}, where @math{a\_exp = 2 \cdot b\_exp + a\_shr}.
 *
 * The function vect_complex_s16_squared_mag_prepare() can be used to obtain values for
 * @math{a\_exp} and @math{a\_shr} based on the input exponent @math{b\_exp} and headroom
 * @math{b\_hr}.
 * @endparblock
 * 
 * @param[out]  a           Real output vector @vector{a}
 * @param[in]   b_real      Real part of complex input vector @vector{b}
 * @param[in]   b_imag      Imaginary part of complex input vector @vector{b}
 * @param[in]   length      Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]   a_shr       Right-shift appled to 32-bit intermediate results
 * 
 * @exception ET_LOAD_STORE Raised if `a`, `b_real` or `b_imag` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @see vect_complex_s16_squared_mag_prepare
 * 
 * @ingroup vect_complex_s16_api
 */
C_API
headroom_t vect_complex_s16_squared_mag(
    int16_t a[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const unsigned length,
    const right_shift_t a_shr);


/**
 * @brief Subtract one complex 16-bit vector from another.
 * 
 * `a_real[]` and `a_imag[]` together represent the complex 16-bit output mantissa vector
 * @vector{a}. Each @math{Re\\{a_k\\}} is `a_real[k]`, and each @math{Im\\{a_k\\}} is `a_imag[k]`.
 *
 * `b_real[]` and `b_imag[]` together represent the complex 16-bit input mantissa vector @vector{b}.
 * Each @math{Re\\{b_k\\}} is `b_real[k]`, and each @math{Im\\{b_k\\}} is `b_imag[k]`.
 *
 * `c_real[]` and `c_imag[]` together represent the complex 16-bit input mantissa vector @vector{c}.
 * Each @math{Re\\{c_k\\}} is `c_real[k]`, and each @math{Im\\{c_k\\}} is `c_imag[k]`.
 *
 * Each of the input vectors must begin at a word-aligned address. This operation can be performed
 * safely in-place on inputs `b_real[]`, `b_imag[]`, `c_real[]` and `c_imag[]`.
 *
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b}
 * and @vector{c} respectively.
 * 
 * @operation{ 
 * &     b_k' \leftarrow sat_{16}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
 * &     c_k' \leftarrow sat_{16}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)     \\
 * &     Re\\{a_k\\} \leftarrow Re\\{b_k'\\} - Re\\{c_k'\\}                  \\
 * &     Im\\{a_k\\} \leftarrow Im\\{b_k'\\} - Im\\{c_k'\\}                  \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} and @vector{c} are the complex 16-bit mantissas of BFP vectors @math{ \bar{b} \cdot
 * 2^{b\_exp} } and @math{\bar{c} \cdot 2^{c\_exp}}, then the resulting vector @vector{a} are the
 * complex 16-bit mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}. 
 *
 * In this case, @math{b\_shr} and @math{c\_shr} **must** be chosen so that @math{a\_exp = b\_exp +
 * b\_shr = c\_exp + c\_shr}. Adding or subtracting mantissas only makes sense if they are
 * associated with the same exponent.
 *
 * The function vect_complex_s16_sub_prepare() can be used to obtain values for @math{a\_exp},
 * @math{b\_shr} and @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and
 * the input headrooms @math{b\_hr} and @math{c\_hr}.
 * @endparblock
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
 * @exception ET_LOAD_STORE Raised if `a_real`, `a_imag`, `b_real`, `b_imag`, `c_real` or `c_imag`
 *            is not word-aligned (See @ref note_vector_alignment)
 * 
 * @see vect_complex_s16_sub_prepare
 * 
 * @ingroup vect_complex_s16_api
 */
C_API
headroom_t vect_complex_s16_sub(
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
 * `b_real[]` and `b_imag[]` together represent the complex 16-bit input mantissa vector @vector{b},
 * and must both begin at a word-aligned address. Each @math{Re\\{b_k\\}} is `b_real[k]`, and each
 * @math{Im\\{b_k\\}} is `b_imag[k]`.
 *
 * `length` is the number of elements in @vector{b}.
 * 
 * @operation{ 
 * &     Re\\{a\\} \leftarrow \sum_{k=0}^{length-1} \left( Re\\{b_k\\} \right)      \\
 * &     Im\\{a\\} \leftarrow \sum_{k=0}^{length-1} \left( Im\\{b_k\\} \right)      
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 *
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the returned
 * value @math{a} is the complex 32-bit mantissa of floating-point value @math{a \cdot 2^{a\_exp}},
 * where @math{a\_exp = b\_exp}.
 * @endparblock
 * 
 * @param[in] b_real    Real part of complex input vector @vector{b}
 * @param[in] b_imag    Imaginary part of complex input vector @vector{b}
 * @param[in] length    Number of elements in vector @vector{b}.
 * 
 * @returns @math{a}, the 32-bit complex sum of elements in @vector{b}.
 * 
 * @exception ET_LOAD_STORE Raised if `b_real` or `b_imag` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_complex_s16_api
 */
C_API
complex_s32_t vect_complex_s16_sum(
    const int16_t b_real[],
    const int16_t b_imag[],
    const unsigned length);


/**
 * @brief Convert a complex 16-bit vector into a complex 32-bit vector.
 *
 * `a[]` represents the complex 32-bit output vector @vector{a}. It must begin at a double word
 * (8-byte) aligned address.
 *
 * `b_real[]` and `b_imag[]` together represent the complex 16-bit input mantissa vector @vector{b}.
 * Each @math{Re\\{b_k\\}} is `b_real[k]`, and each @math{Im\\{b_k\\}} is `b_imag[k]`.
 *
 * The parameter `length` is the number of elements in each of the vectors.
 *
 * `length` is the number of elements in each of the vectors.
 * 
 * @operation{ 
 * &     Re\\{a_k\\} \leftarrow Re\\{b_k\\}              \\
 * &     Im\\{a_k\\} \leftarrow Im\\{b_k\\}              \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1)
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the complex 16-bit mantissas of a BFP vector @math{\bar{b} \cdot 2^{b\_exp}},
 * then the resulting vector @vector{a} are the complex 32-bit mantissas of BFP vector @math{\bar{a}
 * \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp}.
 * @endparblock
 * 
 * @par Notes
 * @parblock
 * * The headroom of output vector @vector{a} is not returned by this function. The headroom of the
 *   output is always 16 bits greater than the headroom of the input.
 * @endparblock
 * 
 * @param[out] a        Complex output vector @vector{a}.
 * @param[in]  b_real   Real part of complex input vector @vector{b}.
 * @param[in]  b_imag   Imaginary part of complex input vector @vector{b}.
 * @param[in]  length   Number of elements in vectors @vector{a} and @vector{b}
 * 
 * @exception ET_LOAD_STORE Raised if `a` is not double-word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_complex_s16_api
 */
C_API
void vect_complex_s16_to_vect_complex_s32(
    complex_s32_t a[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const unsigned length);


#ifdef __XC__
}   //extern "C"
#endif

