// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include "xmath/xmath_conf.h"
#include "xmath/types.h"
#include "xmath/util.h"

#include "xmath/vect/vect_complex_s32_prepare.h"

/**
 * @defgroup vect_complex_s32_api   Complex 32-bit Vector Functions
 */

#ifdef __XC__
extern "C" {
#endif


/**
 * @brief Add one complex 32-bit vector to another.
 * 
 * `a[]`, `b[]` and `c[]` represent the complex 32-bit mantissa vectors @vector{a}, @vector{b} and
 * @vector{c}  respectively. Each must begin at a word-aligned address. This operation can be
 * performed safely in-place on `b[]` or `c[]`.
 *
 * `length` is the number of elements in each of the vectors.
 *
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b}
 * and @vector{c} respectively.
 * 
 * @operation{ 
 * &     b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
 * &     c_k' \leftarrow sat_{32}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)     \\
 * &     Re\\{a_k\\} \leftarrow Re\\{b_k'\\} + Re\\{c_k'\\}                  \\
 * &     Im\\{a_k\\} \leftarrow Im\\{b_k'\\} + Im\\{c_k'\\}                  \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} and @vector{c} are the complex 32-bit mantissas of BFP vectors @math{ \bar{b} \cdot
 * 2^{b\_exp} } and @math{\bar{c} \cdot 2^{c\_exp}}, then the resulting vector @vector{a} are the
 * complex 32-bit mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}. 
 *
 * In this case, @math{b\_shr} and @math{c\_shr} **must** be chosen so that @math{a\_exp = b\_exp +
 * b\_shr = c\_exp + c\_shr}. Adding or subtracting mantissas only makes sense if they are
 * associated with the same exponent.
 *
 * The function vect_complex_s32_add_prepare() can be used to obtain values for @math{a\_exp},
 * @math{b\_shr} and @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and
 * the input headrooms @math{b\_hr} and @math{c\_hr}.
 * @endparblock
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
 * @exception ET_LOAD_STORE Raised if `a`, `b` or `c` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @see vect_complex_s32_add_prepare
 * 
 * @ingroup vect_complex_s32_api
 */
C_API
headroom_t vect_complex_s32_add(
    complex_s32_t a[],
    const complex_s32_t b[],
    const complex_s32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Add a scalar to a complex 32-bit vector.
 * 
 * `a[]` and `b[]`represent the complex 32-bit mantissa vectors @vector{a} and @vector{b}
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
 * &     b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
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
 * In this case, @math{b\_shr} and @math{c\_shr} **must** be chosen so that 
 * @math{a\_exp = b\_exp + b\_shr = c\_exp + c\_shr}. Adding or subtracting mantissas only makes
 * sense if they are associated with the same exponent.
 *
 * The function vect_complex_s32_add_scalar_prepare() can be used to obtain values for
 * @math{a\_exp}, @math{b\_shr} and @math{c\_shr} based on the input exponents @math{b\_exp} and
 * @math{c\_exp} and the input headrooms @math{b\_hr} and @math{c\_hr}.
 *
 * Note that @math{c\_shr} is an output of `vect_complex_s32_add_scalar_prepare()`, but is not a
 * parameter to this function. The @math{c\_shr} produced by
 * `vect_complex_s32_add_scalar_prepare()` is to be applied by the user, and the result passed
 * as input `c`.
 * @endparblock
 * 
 * @param[out]      a           Complex output vector @vector{a}
 * @param[in]       b           Complex input vector @vector{b}
 * @param[in]       c           Complex input scalar @math{c}
 * @param[in]       length      Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]       b_shr       Right-shift applied to @vector{b}
 * 
 * @returns     Headroom of output vector @vector{a}.
 * 
 * @exception ET_LOAD_STORE Raised if `a` or `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @see vect_complex_s32_add_scalar_prepare
 * 
 * @ingroup vect_complex_s32_api
 */
C_API
headroom_t vect_complex_s32_add_scalar(
    complex_s32_t a[],
    const complex_s32_t b[],
    const complex_s32_t c,
    const unsigned length,
    const right_shift_t b_shr);

/**
 * @brief Multiply one complex 32-bit vector element-wise by the complex conjugate of another.
 * 
 * `a[]`, `b[]` and `c[]` represent the 32-bit mantissa vectors @vector{a}, @vector{b} and
 * @vector{c} respectively. Each must begin at a word-aligned address. This operation can be
 * performed safely in-place on `b[]` or `c[]`.
 *
 * `length` is the number of elements in each of the vectors.
 *
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b}
 * and @vector{c} respectively.
 * 
 * @operation{ 
 * &     b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)                     \\
 * &     c_k' \leftarrow sat_{32}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)                     \\
 * &     Re\\{a_k\\} \leftarrow \left( Re\\{b_k'\\} \cdot Re\\{c_k'\\} 
 *                                   + Im\\{b_k'\\} \cdot Im\\{c_k'\\} \right) \cdot 2^{-30} \\
 * &     Im\\{a_k\\} \leftarrow \left( Im\\{b_k'\\} \cdot Re\\{c_k'\\} 
 *                                   - Re\\{b_k'\\} \cdot Im\\{c_k'\\} \right) \cdot 2^{-30} \\
 * &     \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the complex 32-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} }
 * and @math{c} is the complex 32-bit mantissa of floating-point value @math{c \cdot 2^{c\_exp}},
 * then the resulting vector @vector{a} are the mantissas of BFP vector @math{\bar{a} \cdot
 * 2^{a\_exp}}, where @math{a\_exp = b\_exp + c\_exp + a\_shr}.
 *
 * The function vect_complex_s32_conj_mul_prepare() can be used to obtain values for @math{a\_exp}
 * and @math{a\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and the input
 * headrooms @math{b\_hr} and @math{c\_hr}.
 * @endparblock 
 * 
 * @param[out]      a           Complex output vector @vector{a}
 * @param[in]       b           Complex input vector @vector{b}
 * @param[in]       c           Complex input vector @vector{c}
 * @param[in]       length      Number of elements in vectors @vector{a}, @vector{b} and @vector{c}
 * @param[in]       b_shr       Right-shift applied to elements of @vector{b}.
 * @param[in]       c_shr       Right-shift applied to elements of @vector{c}.
 * 
 * @return      Headroom of the output vector @vector{a}
 * 
 * @exception ET_LOAD_STORE Raised if `a`, `b` or `c` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @see vect_complex_s32_conj_mul_prepare
 * 
 * @ingroup vect_complex_s32_api
 */
C_API
headroom_t vect_complex_s32_conj_mul(
    complex_s32_t a[],
    const complex_s32_t b[],
    const complex_s32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Calculate the headroom of a complex 32-bit array.
 * 
 * The headroom of an N-bit integer is the number of bits that the integer's value may be
 * left-shifted without any information being lost. Equivalently, it is one less than the number of
 * leading sign bits.
 *
 * The headroom of a `complex_s32_t` struct is the minimum of the headroom of each of its 32-bit
 * fields, `re` and `im`.
 *
 * The headroom of a `complex_s32_t` array is the minimum of the headroom of each of its
 * `complex_s32_t` elements.
 *
 * This function efficiently traverses the elements of @vector{x} to determine its headroom.
 *
 * `x[]` represents the complex 32-bit vector @vector{x}. `x[]` must begin at a word-aligned
 * address.
 *
 * `length` is the number of elements in `x[]`.
 * 
 * @operation{
 *      min\!\\{ HR_{32}\left(x_0\right), HR_{32}\left(x_1\right), ..., HR_{32}\left(x_{length-1}\right) \\}
 * }
 * 
 * @param[in]   x       Complex input vector @vector{x}
 * @param[in]   length  Number of elements in @vector{x}
 * 
 * @returns     Headroom of vector @vector{x} 
 * 
 * @exception ET_LOAD_STORE Raised if `x` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @see vect_s16_headroom, 
 *      vect_s32_headroom, 
 *      vect_complex_s16_headroom
 * 
 * @ingroup vect_complex_s32_api
 */
C_API
headroom_t vect_complex_s32_headroom(
    const complex_s32_t x[], 
    const unsigned length);


/**
 * @brief Multiply one complex 32-bit vector element-wise by another, and add the result to an 
 * accumulator.
 *
 * `acc[]` represents the complex 32-bit accumulator mantissa vector @vector{a}. Each @math{a_k} is
 * `acc[k]`.
 *
 * `b[]` and `c[]` represent the complex 32-bit input mantissa vectors @vector{b} and @vector{c},
 * where each @math{b_k} is `b[k]` and each @math{c_k} is `c[k]`.
 *
 * Each of the input vectors must begin at a word-aligned address.
 *
 * `length` is the number of elements in each of the vectors.
 *
 * `acc_shr`, `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to input elements
 * @math{a_k}, @math{b_k} and @math{c_k}.
 *
 * @operation{
 * &     \tilde{b}_k \leftarrow sat_{32}( b_k \cdot 2^{-b\_shr} )               \\
 * &     \tilde{c}_k \leftarrow sat_{32}( c_k \cdot 2^{-c\_shr} )               \\
 * &     \tilde{a}_k \leftarrow sat_{32}( a_k \cdot 2^{-acc\_shr} )             \\
 * &     v_k \leftarrow round( sat_{32}( ( 
 *                  Re\\{\tilde{b}_k\\} \cdot Re\\{\tilde{c}_k\\}
 *                - Im\\{\tilde{b}_k\\} \cdot Im\\{\tilde{c}_k\\}            
 *                          ) \cdot 2^{-30}) )                                  \\
 * &     s_k \leftarrow round( sat_{32}( ( 
 *                  Im\\{\tilde{b}_k\\} \cdot Re\\{\tilde{c}_k\\}
 *                + Re\\{\tilde{b}_k\\} \cdot Im\\{\tilde{c}_k\\}            
 *                          ) \cdot 2^{-30}) )                                  \\
 * &     Re\\{a_k\\} \leftarrow sat_{32}( Re\\{\tilde{a}_k\\} + v_k )           \\
 * &     Im\\{a_k\\} \leftarrow sat_{32}( Im\\{\tilde{a}_k\\} + s_k )           \\
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
 * For accumulation to make sense mathematically, @math{bc\_sat} must be chosen such that
 * @math{  a\_exp + acc\_shr = b\_exp + c\_exp + b\_shr + c\_shr }.
 *
 * The function vect_complex_s32_macc_prepare() can be used to obtain values for @math{a\_exp},
 * @math{acc\_shr}, @math{b\_shr} and @math{c\_shr} based on the input exponents @math{a\_exp}, 
 * @math{b\_exp} and @math{c\_exp} and the input headrooms @math{a\_hr}, @math{b\_hr} and 
 * @math{c\_hr}.
 * @endparblock
 *
 * @param[inout]  acc       Complex accumulator @vector{a}
 * @param[in]     b         Complex input vector @vector{b}
 * @param[in]     c         Complex input vector @vector{c}
 * @param[in]     length    Number of elements in vectors @vector{a}, @vector{b} and @vector{c}
 * @param[in]     acc_shr   Signed arithmetic right-shift applied to accumulator elements.
 * @param[in]     b_shr     Signed arithmetic right-shift applied to elements of @vector{b}
 * @param[in]     c_shr     Signed arithmetic right-shift applied to elements of @vector{c}
 * 
 * @returns   Headroom of the output vector @vector{a}
 *
 * @exception ET_LOAD_STORE Raised if `acc`, `b` or `c` is not word-aligned (See @ref note_vector_alignment)
 *
 * @see vect_complex_s32_macc_prepare
 *
 * @ingroup vect_complex_s32_api
 */
C_API
headroom_t vect_complex_s32_macc(
    complex_s32_t acc[],
    const complex_s32_t b[],
    const complex_s32_t c[],
    const unsigned length,
    const right_shift_t acc_shr,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Multiply one complex 32-bit vector element-wise by another, and subtract the result from 
 * an accumulator.
 *
 * `acc[]` represents the complex 32-bit accumulator mantissa vector @vector{a}. Each @math{a_k} is 
 * `acc[k]`.
 *
 * `b[]` and `c[]` represent the complex 32-bit input mantissa vectors @vector{b} and @vector{c},
 * where each @math{b_k} is `b[k]` and each @math{c_k} is `c[k]`.
 *
 * Each of the input vectors must begin at a word-aligned address.
 *
 * `length` is the number of elements in each of the vectors.
 *
 * `acc_shr`, `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to input elements
 * @math{a_k}, @math{b_k} and @math{c_k}.
 *
 * @operation{
 * &     \tilde{b}_k \leftarrow sat_{32}( b_k \cdot 2^{-b\_shr} )               \\
 * &     \tilde{c}_k \leftarrow sat_{32}( c_k \cdot 2^{-c\_shr} )               \\
 * &     \tilde{a}_k \leftarrow sat_{32}( a_k \cdot 2^{-acc\_shr} )             \\
 * &     v_k \leftarrow round( sat_{32}( ( 
 *                  Re\\{\tilde{b}_k\\} \cdot Re\\{\tilde{c}_k\\}
 *                - Im\\{\tilde{b}_k\\} \cdot Im\\{\tilde{c}_k\\}            
 *                          ) \cdot 2^{-30}) )                                  \\
 * &     s_k \leftarrow round( sat_{32}( ( 
 *                  Im\\{\tilde{b}_k\\} \cdot Re\\{\tilde{c}_k\\}
 *                + Re\\{\tilde{b}_k\\} \cdot Im\\{\tilde{c}_k\\}            
 *                          ) \cdot 2^{-30}) )                                  \\
 * &     Re\\{a_k\\} \leftarrow sat_{32}( Re\\{\tilde{a}_k\\} - v_k )           \\
 * &     Im\\{a_k\\} \leftarrow sat_{32}( Im\\{\tilde{a}_k\\} - s_k )           \\
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
 * For accumulation to make sense mathematically, @math{bc\_sat} must be chosen such that
 * @math{  a\_exp + acc\_shr = b\_exp + c\_exp + b\_shr + c\_shr }.
 *
 * The function vect_complex_s32_macc_prepare() can be used to obtain values for @math{a\_exp},
 * @math{acc\_shr}, @math{b\_shr} and @math{c\_shr} based on the input exponents @math{a\_exp}, 
 * @math{b\_exp} and @math{c\_exp} and the input headrooms @math{a\_hr}, @math{b\_hr} and 
 * @math{c\_hr}.
 * @endparblock
 *
 * @param[inout]  acc       Complex accumulator @vector{a}
 * @param[in]     b         Complex input vector @vector{b}
 * @param[in]     c         Complex input vector @vector{c}
 * @param[in]     length    Number of elements in vectors @vector{a}, @vector{b} and @vector{c}
 * @param[in]     acc_shr   Signed arithmetic right-shift applied to accumulator elements.
 * @param[in]     b_shr     Signed arithmetic right-shift applied to elements of @vector{b}
 * @param[in]     c_shr     Signed arithmetic right-shift applied to elements of @vector{c}
 * 
 * @returns   Headroom of the output vector @vector{a}
 *
 * @exception ET_LOAD_STORE Raised if `acc`, `b` or `c` is not word-aligned (See @ref note_vector_alignment)
 *
 * @see vect_complex_s32_nmacc_prepare
 *
 * @ingroup vect_complex_s32_api
 */
C_API
headroom_t vect_complex_s32_nmacc(
    complex_s32_t acc[],
    const complex_s32_t b[],
    const complex_s32_t c[],
    const unsigned length,
    const right_shift_t acc_shr,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Multiply one complex 32-bit vector element-wise by the complex conjugate of another, and 
 * add the result to an accumulator.
 *
 * `acc[]` represents the complex 32-bit accumulator mantissa vector @vector{a}. Each @math{a_k} is
 * `acc[k]`.
 *
 * `b[]` and `c[]` represent the complex 32-bit input mantissa vectors @vector{b} and @vector{c},
 * where each @math{b_k} is `b[k]` and each @math{c_k} is `c[k]`.
 *
 * Each of the input vectors must begin at a word-aligned address.
 *
 * `length` is the number of elements in each of the vectors.
 *
 * `acc_shr`, `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to input elements
 * @math{a_k}, @math{b_k} and @math{c_k}.
 *
 * @operation{
 * &     \tilde{b}_k \leftarrow sat_{32}( b_k \cdot 2^{-b\_shr} )               \\
 * &     \tilde{c}_k \leftarrow sat_{32}( c_k \cdot 2^{-c\_shr} )               \\
 * &     \tilde{a}_k \leftarrow sat_{32}( a_k \cdot 2^{-acc\_shr} )             \\
 * &     v_k \leftarrow round( sat_{32}( ( 
 *                  Re\\{\tilde{b}_k\\} \cdot Re\\{\tilde{c}_k\\}
 *                + Im\\{\tilde{b}_k\\} \cdot Im\\{\tilde{c}_k\\}            
 *                          ) \cdot 2^{-30}) )                                  \\
 * &     s_k \leftarrow round( sat_{32}( ( 
 *                  Im\\{\tilde{b}_k\\} \cdot Re\\{\tilde{c}_k\\}
 *                - Re\\{\tilde{b}_k\\} \cdot Im\\{\tilde{c}_k\\}            
 *                          ) \cdot 2^{-30}) )                                  \\
 * &     Re\\{a_k\\} \leftarrow sat_{32}( Re\\{\tilde{a}_k\\} + v_k )           \\
 * &     Im\\{a_k\\} \leftarrow sat_{32}( Im\\{\tilde{a}_k\\} + s_k )           \\
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
 * For accumulation to make sense mathematically, @math{bc\_sat} must be chosen such that
 * @math{  a\_exp + acc\_shr = b\_exp + c\_exp + b\_shr + c\_shr }.
 *
 * The function vect_complex_s32_conj_macc_prepare() can be used to obtain values for 
 * @math{a\_exp}, @math{acc\_shr}, @math{b\_shr} and @math{c\_shr} based on the input exponents 
 * @math{a\_exp}, @math{b\_exp} and @math{c\_exp} and the input headrooms @math{a\_hr}, @math{b\_hr}
 * and @math{c\_hr}.
 * @endparblock
 *
 * @param[inout]  acc       Complex accumulator @vector{a}
 * @param[in]     b         Complex input vector @vector{b}
 * @param[in]     c         Complex input vector @vector{c}
 * @param[in]     length    Number of elements in vectors @vector{a}, @vector{b} and @vector{c}
 * @param[in]     acc_shr   Signed arithmetic right-shift applied to accumulator elements.
 * @param[in]     b_shr     Signed arithmetic right-shift applied to elements of @vector{b}
 * @param[in]     c_shr     Signed arithmetic right-shift applied to elements of @vector{c}
 * 
 * @returns   Headroom of the output vector @vector{a}
 *
 * @exception ET_LOAD_STORE Raised if `acc`, `b` or `c` is not word-aligned (See @ref note_vector_alignment)
 *
 * @see vect_complex_s32_conj_macc_prepare
 *
 * @ingroup vect_complex_s32_api
 */
C_API
headroom_t vect_complex_s32_conj_macc(
    complex_s32_t acc[],
    const complex_s32_t b[],
    const complex_s32_t c[],
    const unsigned length,
    const right_shift_t acc_shr,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Multiply one complex 32-bit vector element-wise by the complex conjugate of another, and 
 * subtract the result from an accumulator.
 *
 * `acc[]` represents the complex 32-bit accumulator mantissa vector @vector{a}. Each @math{a_k} is
 * `acc[k]`.
 *
 * `b[]` and `c[]` represent the complex 32-bit input mantissa vectors @vector{b} and @vector{c},
 * where each @math{b_k} is `b[k]` and each @math{c_k} is `c[k]`.
 *
 * Each of the input vectors must begin at a word-aligned address.
 *
 * `length` is the number of elements in each of the vectors.
 *
 * `acc_shr`, `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to input elements
 * @math{a_k}, @math{b_k} and @math{c_k}.
 *
 * @operation{
 * &     \tilde{b}_k \leftarrow sat_{32}( b_k \cdot 2^{-b\_shr} )               \\
 * &     \tilde{c}_k \leftarrow sat_{32}( c_k \cdot 2^{-c\_shr} )               \\
 * &     \tilde{a}_k \leftarrow sat_{32}( a_k \cdot 2^{-acc\_shr} )             \\
 * &     v_k \leftarrow round( sat_{32}( ( 
 *                  Re\\{\tilde{b}_k\\} \cdot Re\\{\tilde{c}_k\\}
 *                + Im\\{\tilde{b}_k\\} \cdot Im\\{\tilde{c}_k\\}            
 *                          ) \cdot 2^{-30}) )                                  \\
 * &     s_k \leftarrow round( sat_{32}( ( 
 *                  Im\\{\tilde{b}_k\\} \cdot Re\\{\tilde{c}_k\\}
 *                - Re\\{\tilde{b}_k\\} \cdot Im\\{\tilde{c}_k\\}            
 *                          ) \cdot 2^{-30}) )                                  \\
 * &     Re\\{a_k\\} \leftarrow sat_{32}( Re\\{\tilde{a}_k\\} - v_k )           \\
 * &     Im\\{a_k\\} \leftarrow sat_{32}( Im\\{\tilde{a}_k\\} - s_k )           \\
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
 * For accumulation to make sense mathematically, @math{bc\_sat} must be chosen such that
 * @math{  a\_exp + acc\_shr = b\_exp + c\_exp + b\_shr + c\_shr }.
 *
 * The function vect_complex_s32_conj_nmacc_prepare() can be used to obtain values for 
 * @math{a\_exp}, @math{acc\_shr}, @math{b\_shr} and @math{c\_shr} based on the input exponents 
 * @math{a\_exp}, @math{b\_exp} and @math{c\_exp} and the input headrooms @math{a\_hr}, @math{b\_hr}
 * and @math{c\_hr}.
 * @endparblock
 *
 * @param[inout]  acc       Complex accumulator @vector{a}
 * @param[in]     b         Complex input vector @vector{b}
 * @param[in]     c         Complex input vector @vector{c}
 * @param[in]     length    Number of elements in vectors @vector{a}, @vector{b} and @vector{c}
 * @param[in]     acc_shr   Signed arithmetic right-shift applied to accumulator elements.
 * @param[in]     b_shr     Signed arithmetic right-shift applied to elements of @vector{b}
 * @param[in]     c_shr     Signed arithmetic right-shift applied to elements of @vector{c}
 * 
 * @returns   Headroom of the output vector @vector{a}
 *
 * @exception ET_LOAD_STORE Raised if `acc`, `b` or `c` is not word-aligned (See @ref note_vector_alignment)
 *
 * @see vect_complex_s32_conj_nmacc_prepare
 *
 * @ingroup vect_complex_s32_api
 */
C_API
headroom_t vect_complex_s32_conj_nmacc(
    complex_s32_t acc[],
    const complex_s32_t b[],
    const complex_s32_t c[],
    const unsigned length,
    const right_shift_t acc_shr,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


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
 * `rot_table` must point to a pre-computed table of complex vectors used in calculating the
 * magnitudes. `table_rows` is the number of rows in the table. This library is distributed with a
 * default version of the required rotation table. The following symbols can be used to refer to it
 * in user code:
 * 
 * @code
 *     const extern unsigned rot_table32_rows;
 *     const extern complex_s32_t rot_table32[30][4];
 * @endcode
 * 
 * Faster computation (with reduced precision) can be achieved by generating a smaller version of
 * the table. A python script is provided to generate this table.
 * @todo Point to documentation page on generating this table.
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
 * If @vector{b} are the complex 32-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} },
 * then the resulting vector @vector{a} are the real 32-bit mantissas of BFP vector @math{\bar{a}
 * \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp + b\_shr}.
 * 
 * The function vect_complex_s32_mag_prepare() can be used to obtain values for @math{a\_exp} and
 * @math{b\_shr} based on the input exponent @math{b\_exp} and headroom @math{b\_hr}.
 * @endparblock
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
 * @exception ET_LOAD_STORE Raised if `a` or `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @see vect_complex_s32_mag_prepare
 * 
 * @ingroup vect_complex_s32_api
 */
C_API
headroom_t vect_complex_s32_mag(
    int32_t a[],
    const complex_s32_t b[],
    const unsigned length,
    const right_shift_t b_shr,
    const complex_s32_t* rot_table,
    const unsigned table_rows);


/**
 * @brief Multiply one complex 32-bit vector element-wise by another.
 * 
 * `a[]`, `b[]` and `c[]` represent the 32-bit mantissa vectors @vector{a}, @vector{b} and
 * @vector{c} respectively. Each must begin at a word-aligned address. This operation can be
 * performed safely in-place on `b[]` or `c[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b}
 * and @vector{c} respectively.
 * 
 * @operation{ 
 * &     b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)                     \\
 * &     c_k' \leftarrow sat_{32}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)                     \\
 * &     Re\\{a_k\\} \leftarrow \left( Re\\{b_k'\\} \cdot Re\\{c_k'\\} 
 *                                   - Im\\{b_k'\\} \cdot Im\\{c_k'\\} \right) \cdot 2^{-30} \\
 * &     Im\\{a_k\\} \leftarrow \left( Im\\{b_k'\\} \cdot Re\\{c_k'\\} 
 *                                   + Re\\{b_k'\\} \cdot Im\\{c_k'\\} \right) \cdot 2^{-30} \\
 * &     \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the complex 32-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} }
 * and @math{c} is the complex 32-bit mantissa of floating-point value @math{c \cdot 2^{c\_exp}},
 * then the resulting vector @vector{a} are the mantissas of BFP vector @math{\bar{a} \cdot
 * 2^{a\_exp}}, where @math{a\_exp = b\_exp + c\_exp + b\_shr + c\_shr}.
 *
 * The function vect_complex_s32_mul_prepare() can be used to obtain values for @math{a\_exp},
 * @math{b\_shr} and @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and
 * the input headrooms @math{b\_hr} and @math{c\_hr}.
 * @endparblock
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
 * @exception ET_LOAD_STORE Raised if `a`, `b` or `c` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @see vect_complex_s32_mul_prepare
 * 
 * @ingroup vect_complex_s32_api
 */
C_API
headroom_t vect_complex_s32_mul(
    complex_s32_t a[],
    const complex_s32_t b[],
    const complex_s32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Multiply a complex 32-bit vector element-wise by a real 32-bit vector.
 * 
 * `a[]` and `b[]` represent the complex 32-bit mantissa vectors @vector{a} and @vector{b}
 * respectively. 
 *
 * `c[]` represents the real 32-bit mantissa vector @vector{c}.
 *
 * `a[]`, `b[]`, and `c[]` each must begin at a word-aligned address. This operation can be
 * performed safely in-place on `b[]`.
 *
 * `length` is the number of elements in each of the vectors.
 *
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b}
 * and @vector{c} 
 * respectively.
 * 
 * @operation{ 
 * &     b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)      \\
 * &     c_k' \leftarrow sat_{32}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)      \\
 * &     Re\\{a_k\\} \leftarrow \left( Re\\{b_k'\\} \cdot c_k' \right) \cdot 2^{-30} \\
 * &     Im\\{a_k\\} \leftarrow \left( Im\\{b_k'\\} \cdot c_k' \right) \cdot 2^{-30} \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1)
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the complex 32-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} }
 * and @math{c} is the complex 32-bit mantissa of floating-point value @math{c \cdot 2^{c\_exp}},
 * then the resulting vector @vector{a} are the mantissas of BFP vector @math{\bar{a} \cdot
 * 2^{a\_exp}}, where @math{a\_exp = b\_exp + c\_exp + b\_shr + c\_shr}.
 *
 * The function vect_complex_s32_real_mul_prepare() can be used to obtain values for @math{a\_exp},
 * @math{b\_shr} and @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and
 * the input headrooms @math{b\_hr} and @math{c\_hr}. 
 * @endparblock
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
 * @exception ET_LOAD_STORE Raised if `a`, `b` or `c` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @see vect_complex_s32_real_mul_prepare
 * 
 * @ingroup vect_complex_s32_api
 */
C_API
headroom_t vect_complex_s32_real_mul(
    complex_s32_t a[],
    const complex_s32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Multiply a complex 32-bit vector by a real scalar.
 * 
 * `a[]` and `b[]` represent the complex 32-bit mantissa vectors @vector{a} and @vector{b}
 * respectively. 
 *
 * `c` represents the real 32-bit scale factor @math{c}.
 *
 * `a[]` and `b[]` each must begin at a word-aligned address. This operation can be performed safely
 * in-place on `b[]`.
 *
 * `length` is the number of elements in each of the vectors.
 *
 * `b_shr` and `c_shr` are the signed arithmetic right-shift applied to each element of @vector{b}
 * and to @math{c}.
 * 
 * @operation{
 * &     b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
 * &     Re\\{a_k\\} \leftarrow Re\\{b_k'\\} \cdot c                         \\
 * &     Im\\{a_k\\} \leftarrow Im\\{b_k'\\} \cdot c                         \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1)
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the complex 16-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} }
 * and @math{c} is the complex 16-bit mantissa of floating-point value @math{c \cdot 2^{c\_exp}},
 * then the resulting vector @vector{a} are the mantissas of BFP vector @math{\bar{a} \cdot
 * 2^{a\_exp}}, where @math{a\_exp = b\_exp + c\_exp + b\_shr + c\_shr}.
 *
 * The function vect_complex_s32_real_scale_prepare() can be used to obtain values for
 * @math{a\_exp}, @math{b\_shr} and @math{c\_shr} based on the input exponents @math{b\_exp} and
 * @math{c\_exp} and the input headrooms @math{b\_hr} and @math{c\_hr}. 
 * @endparblock
 * 
 * @param[out]  a           Complex output vector @vector{a}
 * @param[in]   b           Complex input vector @vector{b}
 * @param[in]   c           Complex input vector @vector{c}
 * @param[in]   length      Number of elements in vectors @vector{a}, @vector{b}, and @vector{c}
 * @param[in]   b_shr       Right-shift applied to @vector{b}
 * @param[in]   c_shr       Right-shift applied to @math{c}
 * 
 * @returns     Headroom of the output vector @vector{a}.
 * 
 * @exception ET_LOAD_STORE Raised if `a` or `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_complex_s32_api
 */
C_API
headroom_t vect_complex_s32_real_scale(
    complex_s32_t a[],
    const complex_s32_t b[],
    const int32_t c,
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Multiply a complex 32-bit vector by a complex 32-bit scalar.
 * 
 * `a[]` and `b[]` represent the complex 32-bit mantissa vectors @vector{a} and @vector{b}
 * respectively. 
 *
 * `c` represents the complex 32-bit scale factor @math{c}.
 *
 * `a[]` and `b[]` each must begin at a word-aligned address. This operation can be performed safely
 * in-place on `b[]`.
 *
 * `length` is the number of elements in each of the vectors.
 *
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b}
 * and to @math{c}.
 * 
 * @operation{ 
 * &     b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)                     \\
 * &     Re\\{a_k\\} \leftarrow \left( Re\\{v_k\\} \cdot Re\\{c\\} 
 *                                   - Im\\{v_k\\} \cdot Im\\{c\\} \right) \cdot 2^{-30}     \\
 * &     Im\\{a_k\\} \leftarrow \left( Re\\{v_k\\} \cdot Im\\{c\\} 
 *                                   + Im\\{v_k\\} \cdot Re\\{c\\} \right) \cdot 2^{-30}     \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1)
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the complex 32-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} }
 * and @math{c} is the complex 32-bit mantissa of floating-point value @math{c \cdot 2^{c\_exp}},
 * then the resulting vector @vector{a} are the mantissas of BFP vector @math{\bar{a} \cdot
 * 2^{a\_exp}}, where @math{a\_exp = b\_exp + c\_exp + b\_shr + c\_shr}.
 *
 * The function vect_complex_s32_mul_prepare() can be used to obtain values for @math{a\_exp},
 * @math{b\_shr} and @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and
 * the input headrooms @math{b\_hr} and @math{c\_hr}. 
 * @endparblock
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
 * 
 * @exception ET_LOAD_STORE Raised if `a` or `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_complex_s32_api
 */
C_API
headroom_t vect_complex_s32_scale(
    complex_s32_t a[],
    const complex_s32_t b[],
    const int32_t c_real,
    const int32_t c_imag,
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Set each element of a complex 32-bit vector to a specified value.
 * 
 * `a[]` represents a complex 32-bit vector @vector{a}. `a[]` must begin at a word-aligned address.
 * 
 * `b_real` and `b_imag` are the real and imaginary parts to which each element will be set.
 * 
 * `length` is the number of elements in `a[]`.
 * 
 * @operation{ 
 * &     a_k \leftarrow  b\_real + j\cdot b\_imag              \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1)   \\
 * &         \qquad\text{ where } j^2 = -1
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
 * @param[out]  a           Complex output vector @vector{a}
 * @param[in]   b_real      Value to set real part of elements of @vector{a} to
 * @param[in]   b_imag      Value to set imaginary part of elements of @vector{a} to
 * @param[in]   length      Number of elements in @vector{a}
 * 
 * @exception ET_LOAD_STORE Raised if `a` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_complex_s32_api
 */
C_API
void vect_complex_s32_set(
    complex_s32_t a[],
    const int32_t b_real,
    const int32_t b_imag,
    const unsigned length);


/**
 * @brief Left-shift each element of a complex 32-bit vector by a specified number of bits.
 * 
 * `a[]` and `b[]` represent the complex 32-bit mantissa vectors @vector{a} and @vector{b}
 * respectively. Each must begin at a word-aligned address. This operation can be performed safely
 * in-place on `b[]`.
 *
 * `length` is the number of elements in @vector{a} and @vector{b}.
 *
 * `b_shl` is the signed arithmetic left-shift applied to each element of @vector{b}.
 * 
 * @operation{
 * &     Re\\{a_k\\} \leftarrow sat_{32}(\lfloor Re\\{b_k\\} \cdot 2^{b\_shl} \rfloor)     \\
 * &     Im\\{a_k\\} \leftarrow sat_{32}(\lfloor Im\\{b_k\\} \cdot 2^{b\_shl} \rfloor)     \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the complex 32-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} },
 * then the resulting vector @vector{a} are the complex 32-bit mantissas of BFP vector @math{\bar{a}
 * \cdot 2^{a\_exp}}, where @math{\bar{a} = \bar{b} \cdot 2^{b\_shl}} and @math{a\_exp = b\_exp}.
 * @endparblock
 * 
 * @param[out]  a           Complex output vector @vector{a}
 * @param[in]   b           Complex input vector @vector{b}
 * @param[in]   length      Number of elements in vector @vector{b}
 * @param[in]   b_shl       Left-shift applied to @vector{b} 
 * 
 * @returns     Headroom of the output vector @vector{a}
 * 
 * @exception ET_LOAD_STORE Raised if `a` or `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_complex_s32_api
 */
C_API
headroom_t vect_complex_s32_shl(
    complex_s32_t a[],
    const complex_s32_t b[],
    const unsigned length,
    const left_shift_t b_shl);


/**
 * @brief Right-shift each element of a complex 32-bit vector by a specified number of bits.
 * 
 * `a[]` and `b[]` represent the complex 32-bit mantissa vectors @vector{a} and @vector{b}
 * respectively. Each must begin at a word-aligned address. This operation can be performed safely
 * in-place on `b[]`.
 * 
 * `length` is the number of elements in @vector{a} and @vector{b}.
 * 
 * `b_shr` is the signed arithmetic right-shift applied to each element of @vector{b}.
 * 
 * @operation{
 * &     Re\\{a_k\\} \leftarrow sat_{32}(\lfloor Re\\{b_k\\} \cdot 2^{-b\_shr} \rfloor)     \\
 * &     Im\\{a_k\\} \leftarrow sat_{32}(\lfloor Im\\{b_k\\} \cdot 2^{-b\_shr} \rfloor)     \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the complex 32-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} },
 * then the resulting vector @vector{a} are the complex 32-bit mantissas of BFP vector @math{\bar{a}
 * \cdot 2^{a\_exp}}, where @math{\bar{a} = \bar{b} \cdot 2^{-b\_shr}} and @math{a\_exp = b\_exp}.
 * @endparblock
 * 
 * @param[out]  a           Complex output vector @vector{a}
 * @param[in]   b           Complex input vector @vector{b}
 * @param[in]   length      Number of elements in vector @vector{b}
 * @param[in]   b_shr       Right-shift applied to @vector{b} 
 * 
 * @returns     Headroom of the output vector @vector{a}
 * 
 * @exception ET_LOAD_STORE Raised if `a` or `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_complex_s32_api
 */
C_API
headroom_t vect_complex_s32_shr(
    complex_s32_t a[],
    const complex_s32_t b[],
    const unsigned length,
    const right_shift_t b_shr);


/**
 * @brief Computes the squared magnitudes of elements of a complex 32-bit vector.
 * 
 * `a[]` represents the complex 32-bit mantissa vector @vector{a}. `b[]` represents the real 32-bit
 * mantissa vector @vector{b}. Each must begin at a word-aligned address.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` is the signed arithmetic right-shift appled to each element of @vector{b}.
 * 
 * @operation{ 
 * &     b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
 * &     a_k \leftarrow ((Re\\{b_k'\\})^2 + (Im\\{b_k'\\})^2)\cdot 2^{-30}   \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1)
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the complex 32-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} },
 * then the resulting vector @vector{a} are the real 32-bit mantissas of BFP vector @math{\bar{a}
 * \cdot 2^{a\_exp}}, where @math{a\_exp = 2 \cdot (b\_exp + b\_shr)}.
 * 
 * The function vect_complex_s32_squared_mag_prepare() can be used to obtain values for
 * @math{a\_exp} and @math{b\_shr} based on the input exponent @math{b\_exp} and headroom
 * @math{b\_hr}.
 * @endparblock
 * 
 * @param[out]  a           Complex output vector @vector{a}
 * @param[in]   b           Complex input vector @vector{b}
 * @param[in]   length      Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]   b_shr       Right-shift appled to @vector{b}
 * 
 * @exception ET_LOAD_STORE Raised if `a` is not double word-aligned or `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @see vect_complex_s32_squared_mag_prepare
 * 
 * @ingroup vect_complex_s32_api
 */
C_API
headroom_t vect_complex_s32_squared_mag(
    int32_t a[],
    const complex_s32_t b[],
    const unsigned length,
    const right_shift_t b_shr);


/**
 * @brief Subtract one complex 32-bit vector from another.
 * 
 * `a[]`, `b[]` and `c[]` represent the complex 32-bit mantissa vectors @vector{a}, @vector{b} and
 * @vector{c}  
 * respectively. Each must begin at a word-aligned address. This operation can be performed safely
 * in-place on `b[]` or `c[]`.
 *
 * `length` is the number of elements in each of the vectors.
 *
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b}
 * and @vector{c} respectively.
 * 
 * @operation{ 
 * &     b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
 * &     c_k' \leftarrow sat_{32}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)     \\
 * &     Re\\{a_k\\} \leftarrow Re\\{b_k'\\} - Re\\{c_k'\\}                  \\
 * &     Im\\{a_k\\} \leftarrow Im\\{b_k'\\} - Im\\{c_k'\\}                  \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} and @vector{c} are the complex 32-bit mantissas of BFP vectors @math{ \bar{b} \cdot
 * 2^{b\_exp} } and @math{\bar{c} \cdot 2^{c\_exp}}, then the resulting vector @vector{a} are the
 * complex 32-bit mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}. 
 *
 * In this case, @math{b\_shr} and @math{c\_shr} **must** be chosen so that @math{a\_exp = b\_exp +
 * b\_shr = c\_exp + c\_shr}. Adding or subtracting mantissas only makes sense if they are
 * associated with the same exponent.
 *
 * The function vect_complex_s32_sub_prepare() can be used to obtain values for @math{a\_exp},
 * @math{b\_shr} and @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and
 * the input headrooms @math{b\_hr} and @math{c\_hr}.
 * @endparblock
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
 * @exception ET_LOAD_STORE Raised if `a`, `b` or `c` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @see vect_complex_s32_sub_prepare
 * 
 * @ingroup vect_complex_s32_api
 */
C_API
headroom_t vect_complex_s32_sub(
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
 * `b[]` represents the complex 32-bit mantissa vector @vector{b}. `b[]` must begin at a
 * word-aligned address.
 *
 * `length` is the number of elements in @vector{b}.
 *
 * `b_shr` is the **unsigned** arithmetic right-shift appled to each element of @vector{b}. `b_shr`
 * _cannot_ be 
 * negative.
 * 
 * @operation{ 
 * &     b_k' \leftarrow b_k \cdot 2^{-b\_shr}                                       \\
 * &     Re\\{a\\} \leftarrow \sum_{k=0}^{length-1} \left( Re\\{b_k'\\} \right)      \\
 * &     Im\\{a\\} \leftarrow \sum_{k=0}^{length-1} \left( Im\\{b_k'\\} \right)      
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then @math{a} is
 * the complex 64-bit mantissa of floating-point value @math{a \cdot 2^{a\_exp}}, where @math{a\_exp
 * = b\_exp + b\_shr}.
 *
 * The function vect_complex_s32_sum_prepare() can be used to obtain values for @math{a\_exp} and
 * @math{b\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and the input
 * headrooms @math{b\_hr} and @math{c\_hr}.
 * @endparblock
 * 
 * @par Additional Details
 * @parblock
 * 
 * Internally the sum accumulates into four separate complex 40-bit accumulators. These accumulators
 * apply symmetric 40-bit saturation logic (with bounds @math{\pm 2^{39}-1}) with each added
 * element. At the end, the 4 accumulators are summed together into the 64-bit fields of `a`. No
 * saturation logic is applied at this final step.
 *
 * In the most extreme case, each @math{b_k} may be @math{-2^{31}}. @math{256} of these added into
 * the same accumulator is @math{-2^{39}} which would saturate to @math{-2^{39}+1}, introducing 1
 * LSb of error (which may or may not be acceptable given a particular circumstance). The final
 * result for each part then may be as large as @math{4\cdot(-2^{39}+1) = -2^{41}+4 }, each fitting
 * into a 42-bit signed integer.
 * @endparblock
 * 
 * @param[out]  a           Complex sum @math{a}
 * @param[in]   b           Complex input vector @vector{b}.
 * @param[in]   length      Number of elements in vector @vector{b}.
 * @param[in]   b_shr       Right-shift appled to @vector{b}.
 * 
 * @exception ET_LOAD_STORE Raised if `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @see vect_complex_s32_sum_prepare
 * 
 * @ingroup vect_complex_s32_api
 */
C_API
void vect_complex_s32_sum(
    complex_s64_t* a,
    const complex_s32_t b[],
    const unsigned length,
    const right_shift_t b_shr);


/**
 * @brief Reverses the order of the tail of a complex 32-bit vector.
 * 
 * Reverses the order of elements in the tail of the complex 32-bit vector @vector{x}. The tail of
 * @vector{x}, in this context, is all elements of @vector{x} except for @math{x_0}. In other words,
 * the first element @math{x_0} remains where it is, and the remaining @math{length-1} elements are
 * rearranged to have their order reversed.
 *
 * This function is used when performing a forward or inverse FFT on a single sequence of real
 * values (i.e. the mono FFT), and operates in-place on `x[]`.
 * 
 * @par Parameter Details
 * @parblock
 * 
 * `x[]` represents the complex 32-bit vector @vector{x}, which is both an input to and an output of
 * this function. `x[]` must begin at a word-aligned address.
 * 
 * `length` is the number of elements in @vector{x}.
 * @endparblock
 * 
 * @operation{
 * &     x_0 \leftarrow x_0              \\
 * &     x_k \leftarrow x_{length - k}   \\
 * &         \qquad\text{ for }k\in 1\ ...\ (length-1) 
 * }
 * 
 * @param[inout]    x           Complex vector to have its tail reversed.
 * @param[in]       length      Number of elements in @vector{x}
 * 
 * @exception ET_LOAD_STORE Raised if `x` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @see bfp_fft_forward_mono, 
 *      bfp_fft_inverse_mono
 * 
 * @ingroup vect_complex_s32_api
 */
C_API
void vect_complex_s32_tail_reverse(
    complex_s32_t x[],
    const unsigned length);
    

/**
 * @brief Get the complex conjugate of a complex 32-bit vector.
 * 
 * The complex conjugate of a complex scalar @math{z = x + yi} is @math{z^* = x - yi}. This function
 * computes the complex conjugate of each element of @vector{b} (negates the imaginary part of each
 * element) and places the result in @vector{a}.
 *
 * `a[]` is the complex 32-bit output vector @vector{a}.
 *
 * `b[]` is the complex 32-bit input vector @vector{b}.
 *
 * Both `a` and `b` must point to word-aligned addresses.
 *
 * `length` is the number of elements in @vector{a} and @vector{b}.
 * 
 * @operation{
 * &     Re\\{a_k\\} \leftarrow Re\\{b_k\\}               \\
 * &     Im\\{a_k\\} \leftarrow - Im\\{b_k\\}             \\
 * &         \qquad\text{ for }k\in 1\ ...\ (length-1) 
 * }
 * 
 * @param[out]  a       Complex 32-bit output vector @vector{a}
 * @param[in]   b       Complex 32-bit input vector @vector{b}
 * @param[in]   length  Number of elements in vectors @vector{a} and @vector{b}
 * 
 * @returns     Headroom of the output vector @vector{a}.
 * 
 * @exception ET_LOAD_STORE Raised if `a` or `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_complex_s32_api
 */
C_API
headroom_t vect_complex_s32_conjugate(
    complex_s32_t a[],
    const complex_s32_t b[],
    const unsigned length);


/** 
 * @brief Convert a complex 32-bit vector into a complex 16-bit vector.
 *
 * This function converts a complex 32-bit mantissa vector @vector{b} into a complex 16-bit mantissa
 * vector @vector{a}. Conceptually, the output BFP vector @math{\bar{a}\cdot 2^{a\_exp}} represents
 * the same value as the input BFP vector @math{\bar{b}\cdot 2^{b\_exp}}, only with a reduced
 * bit-depth.
 *
 * In most cases @math{b\_shr} should be @math{16 - b\_hr}, where @math{b\_hr} is the headroom of
 * the 32-bit input mantissa vector @vector{b}. The output exponent @math{a\_exp} will then be given
 * by
 *
 * @math{ a\_exp = b\_exp + b\_shr }
 * 
 * @par Parameter Details
 * @parblock
 * 
 * `a_real[]` and `a_imag[]` together represent the complex 16-bit output mantissa vector
 * @vector{a}, with the real part of each @math{a_k} going in `a_real[]` and the imaginary part
 * going in `a_imag[]`.
 *
 * `b[]` represents the complex 32-bit mantissa vector @vector{b}.
 *
 * `a_real[]`, `a_imag[]` and `b[]` must each begin at a word-aligned address.
 *
 * `length` is the number of elements in each of the vectors.
 *
 * `b_shr` is the signed arithmetic right-shift applied to elements of @vector{b}.
 * @endparblock
 * 
 * @operation{ 
 * &     b_k' \leftarrow sat_{16}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
 * &     Re\\{a_k\\} \leftarrow Re\\{b_k'\\}                                 \\
 * &     Im\\{a_k\\} \leftarrow Im\\{b_k'\\}                                 \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the complex 32-bit mantissas of a BFP vector @math{\bar{b} \cdot 2^{b\_exp}},
 * then the resulting vector @vector{a} are the complex 16-bit mantissas of BFP vector @math{\bar{a}
 * \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp + b\_shr}.
 * @endparblock
 * 
 * @param[out]  a_real      Real part of complex output vector @vector{a}.
 * @param[out]  a_imag      Imaginary part of complex output vector @vector{a}.
 * @param[in]   b           Complex input vector @vector{b}.
 * @param[in]   length      Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]   b_shr       Right-shift appled to @vector{b}.
 * 
 * @exception ET_LOAD_STORE Raised if `a_real`, `a_imag` or `b` are not word-aligned (See @ref note_vector_alignment)
 * 
 * @see vect_s32_to_vect_s16, 
 *      vect_complex_s16_to_vect_complex_s32
 * 
 * @ingroup vect_complex_s32_api
 */
C_API
void vect_complex_s32_to_vect_complex_s16(
    int16_t a_real[],
    int16_t a_imag[],
    const complex_s32_t b[],
    const unsigned length,
    const right_shift_t b_shr);


#ifdef __XC__
}   //extern "C"
#endif

