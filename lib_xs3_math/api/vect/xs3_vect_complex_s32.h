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
 * @page page_xs3_vect_complex_s32_h  xs3_vect_complex_s32.h
 * 
 * This header contains functions implementing arithmetic operations on complex 32-bit vectors, 
 * optimized for xCore XS3.
 * 
 * @note This header is included automatically through `xs3_math.h` or `bfp_math.h`.
 * 
 * @ingroup xs3_math_header_file
 */


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
 * If @vector{b} and @vector{c} are the complex 32-bit mantissas of BFP vectors @math{ \bar{b} \cdot 2^{b\_exp} } and 
 * @math{\bar{c} \cdot 2^{c\_exp}}, then the resulting vector @vector{a} are the complex 32-bit mantissas of BFP vector 
 * @math{\bar{a} \cdot 2^{a\_exp}}. 
 * 
 * In this case, @math{b\_shr} and @math{c\_shr} **must** be chosen so that 
 * @math{a\_exp = b\_exp + b\_shr = c\_exp + c\_shr}. Adding or subtracting mantissas only makes sense if they
 * are associated with the same exponent.
 * 
 * The function xs3_vect_complex_s32_add_prepare() can be used to obtain values for @math{a\_exp}, @math{b\_shr} and 
 * @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and the input headrooms @math{b\_hr} and 
 * @math{c\_hr}.
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
 * @see xs3_vect_complex_s32_add_prepare
 * 
 * @ingroup xs3_vect32_func
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
 * @brief Obtain the output exponent and shifts required for a call to `xs3_vect_complex_s32_add()`.
 * 
 * The logic for computing the shifts and exponents of `xs3_vect_complex_s32_add()` is identical to that for
 * `xs3_vect_s32_add()`.
 * 
 * This macro is provided as a convenience to developers and to make the code more coherent.
 * 
 * @see xs3_vect_s32_add_prepare()
 * 
 * @ingroup xs3_vect32_prepare
 */
#define xs3_vect_complex_s32_add_prepare xs3_vect_s32_add_prepare


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
 * If @vector{b} are the complex 32-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} } and @math{c} is the 
 * complex 32-bit mantissa of floating-point value @math{c \cdot 2^{c\_exp}}, then the resulting vector @vector{a} are 
 * the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp + c\_exp + a\_shr}.
 * 
 * The function xs3_vect_complex_s32_conj_mul_prepare() can be used to obtain values for @math{a\_exp} and @math{a\_shr} 
 * based on the input exponents @math{b\_exp} and @math{c\_exp} and the input headrooms @math{b\_hr} and @math{c\_hr}.
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
 * @see xs3_vect_complex_s32_conj_mul_prepare
 * 
 * @ingroup xs3_vect32_func
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
 * @brief Obtain the output exponent and shifts required for a call to `xs3_vect_complex_s32_conj_mul()`.
 * 
 * The logic for computing the shifts and exponents of `xs3_vect_complex_s32_conj_mul()` is identical to that for
 * `xs3_vect_complex_s32_mul()`.
 * 
 * This macro is provided as a convenience to developers and to make the code more readable.
 * 
 * @see xs3_vect_complex_s32_mul_prepare()
 * 
 * @ingroup xs3_vect32_prepare
 */
#define xs3_vect_complex_s32_conj_mul_prepare xs3_vect_complex_s32_mul_prepare


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
 * @see xs3_vect_ch_pair_s16_headroom, 
 *      xs3_vect_ch_pair_s32_headroom, 
 *      xs3_vect_s16_headroom, 
 *      xs3_vect_s32_headroom, 
 *      xs3_vect_complex_s16_headroom
 * 
 * @ingroup xs3_vect32_func
 */
C_API
headroom_t xs3_vect_complex_s32_headroom(
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
 * The function xs3_vect_complex_s32_macc_prepare() can be used to obtain values for @math{a\_exp},
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
 * @see xs3_vect_complex_s32_macc_prepare
 *
 * @ingroup xs3_vect32_func
 */
C_API
headroom_t xs3_vect_complex_s32_macc(
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
 * The function xs3_vect_complex_s32_macc_prepare() can be used to obtain values for @math{a\_exp},
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
 * @see xs3_vect_complex_s32_nmacc_prepare
 *
 * @ingroup xs3_vect32_func
 */
C_API
headroom_t xs3_vect_complex_s32_nmacc(
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
 * The function xs3_vect_complex_s32_conj_macc_prepare() can be used to obtain values for 
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
 * @see xs3_vect_complex_s32_conj_macc_prepare
 *
 * @ingroup xs3_vect32_func
 */
C_API
headroom_t xs3_vect_complex_s32_conj_macc(
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
 * The function xs3_vect_complex_s32_conj_nmacc_prepare() can be used to obtain values for 
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
 * @see xs3_vect_complex_s32_conj_nmacc_prepare
 *
 * @ingroup xs3_vect32_func
 */
C_API
headroom_t xs3_vect_complex_s32_conj_nmacc(
    complex_s32_t acc[],
    const complex_s32_t b[],
    const complex_s32_t c[],
    const unsigned length,
    const right_shift_t acc_shr,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Obtain the output exponent and shifts needed by xs3_vect_complex_s32_macc().
 *
 * This function is used in conjunction with xs3_vect_complex_s32_macc() to perform an element-wise
 * multiply-accumlate of 32-bit BFP vectors.
 *
 * This function computes `new_acc_exp`, `acc_shr`, `b_shr` and `c_shr`, which are selected to
 * maximize precision in the resulting accumulator vector without causing saturation of final or
 * intermediate values. Normally the caller will pass these outputs to their corresponding inputs of
 * xs3_vect_complex_s32_macc().
 *
 * `acc_exp` is the exponent associated with the accumulator mantissa vector @vector{a} prior to the
 * operation, whereas `new_acc_exp` is the exponent corresponding to the updated accumulator vector.
 *
 * `b_exp` and `c_exp` are the exponents associated with the complex input mantissa vectors
 * @vector{b} and @vector{c} respectively.
 *
 * `acc_hr`, `b_hr` and `c_hr` are the headrooms of @vector{a}, @vector{b} and @vector{c}
 * respectively. If the headroom of any of these vectors is unknown, it can be obtained by calling
 * xs3_vect_complex_s32_headroom(). Alternatively, the value `0` can always be safely used (but may
 * result in reduced precision).
 *
 * @par Adjusting Output Exponents
 * @parblock
 *
 * If a specific output exponent `desired_exp` is needed for the result (e.g. for emulating
 * fixed-point arithmetic), the `acc_shr` and `bc_sat` produced by this function can be adjusted
 * according to the following: 
 *
 * @code{.c}
 *      // Presumed to be set somewhere
 *      exponent_t acc_exp, b_exp, c_exp;
 *      headroom_t acc_hr, b_hr, c_hr;
 *      exponent_t desired_exp;
 *      
 *      ...
 * 
 *      // Call prepare
 *      right_shift_t acc_shr, b_shr, c_shr;
 *      xs3_vect_complex_s32_macc_prepare(&acc_exp, &acc_shr, &b_shr, &c_shr, 
 *                                        acc_exp, b_exp, c_exp,
 *                                        acc_hr, b_hr, c_hr);
 * 
 *      // Modify results
 *      right_shift_t mant_shr = desired_exp - acc_exp;
 *      acc_exp += mant_shr;
 *      acc_shr += mant_shr;
 *      b_shr  += mant_shr;
 *      c_shr  += mant_shr;
 *      
 *      // acc_shr, b_shr and c_shr may now be used in a call to xs3_vect_complex_s32_macc() 
 * @endcode
 *
 * When applying the above adjustment, the following conditions should be maintained:
 * * `acc_shr > -acc_hr`  (Shifting any further left may cause saturation)
 * * `b_shr => -b_hr`  (Shifting any further left may cause saturation)
 * * `c_shr => -c_hr`  (Shifting any further left may cause saturation)
 *
 * It is up to the user to ensure any such modification does not result in saturation or
 * unacceptable loss of precision.
 * @endparblock
 *
 *
 * @param[out]  new_acc_exp   Exponent associated with output mantissa vector @vector{a} (after macc)
 * @param[out]  acc_shr       Signed arithmetic right-shift used for @vector{a} in  
 *                            xs3_vect_complex_s32_macc()
 * @param[out]  b_shr         Signed arithmetic right-shift used for @vector{b} in  
 *                            xs3_vect_complex_s32_macc()
 * @param[out]  c_shr         Signed arithmetic right-shift used for @vector{c} in  
 *                            xs3_vect_complex_s32_macc()
 * @param[in]   acc_exp       Exponent associated with input mantissa vector @vector{a} (before macc)
 * @param[in]   b_exp         Exponent associated with input mantissa vector @vector{b}
 * @param[in]   c_exp         Exponent associated with input mantissa vector @vector{c}
 * @param[in]   acc_hr        Headroom of input mantissa vector @vector{a} (before macc)
 * @param[in]   b_hr          Headroom of input mantissa vector @vector{b}
 * @param[in]   c_hr          Headroom of input mantissa vector @vector{c}
 *
 * @see xs3_vect_complex_s32_macc
 *
 * @ingroup xs3_vect32_prepare
 */
C_API
void xs3_vect_complex_s32_macc_prepare(
    exponent_t* new_acc_exp,
    right_shift_t* acc_shr,
    right_shift_t* b_shr,
    right_shift_t* c_shr,
    const exponent_t acc_exp,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const exponent_t acc_hr,
    const headroom_t b_hr,
    const headroom_t c_hr);


/**
 * @brief Obtain the output exponent and shifts required for a call to xs3_vect_complex_s32_nmacc().
 *
 * The logic for computing the shifts and exponents of `xs3_vect_complex_s32_nmacc()` is identical
 * to that for `xs3_vect_complex_s32_macc_prepare()`.
 *
 * This macro is provided as a convenience to developers and to make the code more readable.
 *
 * @see xs3_vect_complex_s32_macc_prepare(), xs3_vect_complex_s32_nmacc()
 *
 * @ingroup xs3_vect32_prepare
 */
#define xs3_vect_complex_s32_nmacc_prepare xs3_vect_complex_s32_macc_prepare


/**
 * @brief Obtain the output exponent and shifts required for a call to 
 * xs3_vect_complex_s32_conj_macc().
 *
 * The logic for computing the shifts and exponents of `xs3_vect_complex_s32_conj_macc()` is 
 * identical to that for `xs3_vect_complex_s32_macc_prepare()`.
 *
 * This macro is provided as a convenience to developers and to make the code more readable.
 *
 * @see xs3_vect_complex_s32_macc_prepare(), xs3_vect_complex_s32_conj_macc()
 *
 * @ingroup xs3_vect32_prepare
 */
#define xs3_vect_complex_s32_conj_macc_prepare xs3_vect_complex_s32_macc_prepare


/**
 * @brief Obtain the output exponent and shifts required for a call to 
 * xs3_vect_complex_s32_conj_nmacc().
 *
 * The logic for computing the shifts and exponents of `xs3_vect_complex_s32_conj_nmacc()` is 
 * identical to that for `xs3_vect_complex_s32_macc_prepare()`.
 *
 * This macro is provided as a convenience to developers and to make the code more readable.
 *
 * @see xs3_vect_complex_s32_macc_prepare(), xs3_vect_complex_s32_conj_nmacc()
 *
 * @ingroup xs3_vect32_prepare
 */
#define xs3_vect_complex_s32_conj_nmacc_prepare xs3_vect_complex_s32_macc_prepare


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
 * @operation{ 
 * &     v_k \leftarrow b_k \cdot 2^{-b\_shr}    \\
 * &     a_k \leftarrow \sqrt { {\left( Re\\{v_k\\} \right)}^2 + {\left( Im\\{v_k\\} \right)}^2 }
 * &       \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the complex 32-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} }, then the resulting 
 * vector @vector{a} are the real 32-bit mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where 
 * @math{a\_exp = b\_exp + b\_shr}.
 * 
 * The function xs3_vect_complex_s32_mag_prepare() can be used to obtain values for @math{a\_exp} and @math{b\_shr} based on 
 * the input exponent @math{b\_exp} and headroom @math{b\_hr}.
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
 * @see xs3_vect_complex_s32_mag_prepare
 * 
 * @ingroup xs3_vect32_func
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
 * @parblock
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
 * @endparblock
 * 
 * @param[out]  a_exp               Output exponent associated with output mantissa vector @vector{a}
 * @param[out]  b_shr               Signed arithmetic right-shift for @vector{b} used by xs3_vect_complex_s32_mag()
 * @param[in]   b_exp               Exponent associated with input mantissa vector @vector{b}
 * @param[in]   b_hr                Headroom of input mantissa vector @vector{b}
 * 
 * @see xs3_vect_complex_s32_mag()
 * 
 * @ingroup xs3_vect32_prepare
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
 * If @vector{b} are the complex 32-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} } and @math{c} is the 
 * complex 32-bit mantissa of floating-point value @math{c \cdot 2^{c\_exp}}, then the resulting vector @vector{a} are 
 * the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp + c\_exp + b\_shr + c\_shr}.
 * 
 * The function xs3_vect_complex_s32_mul_prepare() can be used to obtain values for @math{a\_exp}, @math{b\_shr} and 
 * @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and the input headrooms @math{b\_hr} and 
 * @math{c\_hr}.
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
 * @see xs3_vect_complex_s32_mul_prepare
 * 
 * @ingroup xs3_vect32_func
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
 * @parblock
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
 * @endparblock
 * 
 * @par Notes
 * @parblock
 * 
 * * Using the outputs of this function, an output mantissa which would otherwise be `INT32_MIN` will instead saturate 
 *   to `-INT32_MAX`. This is due to the symmetric saturation logic employed by the VPU and is a hardware feature. This 
 *   is a corner case which is usually unlikely and results in 1 LSb of error when it occurs.
 * @endparblock
 * 
 * @param[out]  a_exp               Exponent associated with output mantissa vector @vector{a}
 * @param[out]  b_shr               Signed arithmetic right-shift for @vector{b} used by xs3_vect_complex_s32_mul()
 * @param[out]  c_shr               Signed arithmetic right-shift for @vector{c} used by xs3_vect_complex_s32_mul()
 * @param[in]   b_exp               Exponent associated with input mantissa vector @vector{b}
 * @param[in]   c_exp               Exponent associated with input mantissa vector @vector{c}
 * @param[in]   b_hr                Headroom of input mantissa vector @vector{b}
 * @param[in]   c_hr                Headroom of input mantissa vector @vector{c}
 * 
 * @see xs3_vect_complex_s32_conj_mul, 
 *      xs3_vect_complex_s32_mul
 * 
 * @ingroup xs3_vect32_prepare
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
 * If @vector{b} are the complex 32-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} } and @math{c} is the 
 * complex 32-bit mantissa of floating-point value @math{c \cdot 2^{c\_exp}}, then the resulting vector @vector{a} are 
 * the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp + c\_exp + b\_shr + c\_shr}.
 * 
 * The function xs3_vect_complex_s32_real_mul_prepare() can be used to obtain values for 
 * @math{a\_exp}, @math{b\_shr} and @math{c\_shr} based on the input exponents @math{b\_exp} and 
 * @math{c\_exp} and the input headrooms @math{b\_hr} and @math{c\_hr}. 
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
 * @see xs3_vect_complex_s32_real_mul_prepare
 * 
 * @ingroup xs3_vect32_func
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
 * @parblock
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
 * @endparblock
 * 
 * @par Notes
 * @parblock
 * 
 * * Using the outputs of this function, an output mantissa which would otherwise be `INT32_MIN` will instead saturate 
 *   to `-INT32_MAX`. This is due to the symmetric saturation logic employed by the VPU and is a hardware feature. This 
 *   is a corner case which is usually unlikely and results in 1 LSb of error when it occurs.
 * @endparblock
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
 * 
 * @ingroup xs3_vect32_prepare
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
 * If @vector{b} are the complex 16-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} } and @math{c} is the 
 * complex 16-bit mantissa of floating-point value @math{c \cdot 2^{c\_exp}}, then the resulting vector @vector{a} are 
 * the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp + c\_exp + b\_shr + c\_shr}.
 * 
 * The function xs3_vect_complex_s32_real_scale_prepare() can be used to obtain values for @math{a\_exp}, @math{b\_shr} and 
 * @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and the input headrooms @math{b\_hr} and 
 * @math{c\_hr}. 
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
 * @ingroup xs3_vect32_func
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
 * @brief Obtain the output exponent and shifts required for a call to `xs3_vect_complex_s32_real_scale()`.
 * 
 * The logic for computing the shifts and exponents of `xs3_vect_complex_s32_real_scale()` is identical to that for
 * `xs3_vect_s32_mul()`.
 * 
 * This macro is provided as a convenience to developers and to make the code more readable.
 * 
 * @see xs3_vect_s32_mul_prepare()
 * 
 * @ingroup xs3_vect32_prepare
 */
#define xs3_vect_complex_s32_real_scale_prepare xs3_vect_s32_mul_prepare


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
 * If @vector{b} are the complex 32-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} } and @math{c} is the 
 * complex 32-bit mantissa of floating-point value @math{c \cdot 2^{c\_exp}}, then the resulting vector @vector{a} are 
 * the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp + c\_exp + b\_shr + c\_shr}.
 * 
 * The function xs3_vect_complex_s32_mul_prepare() can be used to obtain values for @math{a\_exp}, @math{b\_shr} and 
 * @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and the input headrooms @math{b\_hr} and 
 * @math{c\_hr}. 
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
 * @ingroup xs3_vect32_func
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
 * @parblock
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
 * @endparblock
 * 
 * @par Notes
 * @parblock
 * 
 * * Using the outputs of this function, an output mantissa which would otherwise be `INT32_MIN` will instead saturate 
 *   to `-INT32_MAX`. This is due to the symmetric saturation logic employed by the VPU and is a hardware feature. This 
 *   is a corner case which is usually unlikely and results in 1 LSb of error when it occurs.
 * @endparblock
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
 * 
 * @ingroup xs3_vect32_prepare
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
 * @operation{ 
 * &     a_k \leftarrow  b\_real + j\cdot b\_imag              \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1)   \\
 * &         \qquad\text{ where } j^2 = -1
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @math{b} is the mantissa of floating-point value @math{b \cdot 2^{b\_exp}}, then the output vector @vector{a} are
 * the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp}.
 * @endparblock
 * 
 * @param[out]  a           Complex output vector @vector{a}
 * @param[in]   b_real      Value to set real part of elements of @vector{a} to
 * @param[in]   b_imag      Value to set imaginary part of elements of @vector{a} to
 * @param[in]   length      Number of elements in @vector{a}
 * 
 * @exception ET_LOAD_STORE Raised if `a` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup xs3_vect32_func
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
 * @operation{
 * &     Re\\{a_k\\} \leftarrow sat_{32}(\lfloor Re\\{b_k\\} \cdot 2^{b\_shl} \rfloor)     \\
 * &     Im\\{a_k\\} \leftarrow sat_{32}(\lfloor Im\\{b_k\\} \cdot 2^{b\_shl} \rfloor)     \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the complex 32-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} }, then the resulting 
 * vector @vector{a} are the complex 32-bit mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where 
 * @math{\bar{a} = \bar{b} \cdot 2^{b\_shl}} and @math{a\_exp = b\_exp}.
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
 * @ingroup xs3_vect32_func
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
 * @operation{
 * &     Re\\{a_k\\} \leftarrow sat_{32}(\lfloor Re\\{b_k\\} \cdot 2^{-b\_shr} \rfloor)     \\
 * &     Im\\{a_k\\} \leftarrow sat_{32}(\lfloor Im\\{b_k\\} \cdot 2^{-b\_shr} \rfloor)     \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the complex 32-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} }, then the resulting 
 * vector @vector{a} are the complex 32-bit mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where 
 * @math{\bar{a} = \bar{b} \cdot 2^{-b\_shr}} and @math{a\_exp = b\_exp}.
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
 * @ingroup xs3_vect32_func
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
 * @operation{ 
 * &     b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
 * &     a_k \leftarrow ((Re\\{b_k'\\})^2 + (Im\\{b_k'\\})^2)\cdot 2^{-30}   \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1)
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the complex 32-bit mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} }, then the resulting 
 * vector @vector{a} are the real 32-bit mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where 
 * @math{a\_exp = 2 \cdot (b\_exp + b\_shr)}.
 * 
 * The function xs3_vect_complex_s32_squared_mag_prepare() can be used to obtain values for @math{a\_exp} and 
 * @math{b\_shr} based on the input exponent @math{b\_exp} and headroom @math{b\_hr}.
 * @endparblock
 * 
 * @param[out]  a           Complex output vector @vector{a}
 * @param[in]   b           Complex input vector @vector{b}
 * @param[in]   length      Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]   b_shr       Right-shift appled to @vector{b}
 * 
 * @exception ET_LOAD_STORE Raised if `a` or `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @see xs3_vect_complex_s32_squared_mag_prepare
 * 
 * @ingroup xs3_vect32_func
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
 * @parblock
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
 * @endparblock
 * 
 * @param[out]  a_exp               Output exponent associated with output mantissa vector @vector{a}
 * @param[out]  b_shr               Signed arithmetic right-shift for @vector{b} used by xs3_vect_complex_s32_squared_mag()
 * @param[in]   b_exp               Exponent associated with input mantissa vector @vector{b}
 * @param[in]   b_hr                Headroom of input mantissa vector @vector{b}
 * 
 * @see xs3_vect_complex_s32_squared_mag()
 * 
 * @ingroup xs3_vect32_prepare
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
 * If @vector{b} and @vector{c} are the complex 32-bit mantissas of BFP vectors @math{ \bar{b} \cdot 2^{b\_exp} } and 
 * @math{\bar{c} \cdot 2^{c\_exp}}, then the resulting vector @vector{a} are the complex 32-bit mantissas of BFP vector 
 * @math{\bar{a} \cdot 2^{a\_exp}}. 
 * 
 * In this case, @math{b\_shr} and @math{c\_shr} **must** be chosen so that 
 * @math{a\_exp = b\_exp + b\_shr = c\_exp + c\_shr}. Adding or subtracting mantissas only makes sense if they
 * are associated with the same exponent.
 * 
 * The function xs3_vect_complex_s32_sub_prepare() can be used to obtain values for @math{a\_exp}, @math{b\_shr} and 
 * @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and the input headrooms @math{b\_hr} and 
 * @math{c\_hr}.
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
 * @see xs3_vect_complex_s32_sub_prepare
 * 
 * @ingroup xs3_vect32_func
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
 * @brief Obtain the output exponent and shifts required for a call to `xs3_vect_complex_s32_sub()`.
 * 
 * The logic for computing the shifts and exponents of `xs3_vect_complex_s32_sub()` is identical to that for
 * `xs3_vect_s32_add()`.
 * 
 * This macro is provided as a convenience to developers and to make the code more readable.
 * 
 * @see xs3_vect_s32_add_prepare()
 * 
 * @ingroup xs3_vect32_prepare
 */
#define xs3_vect_complex_s32_sub_prepare xs3_vect_s32_add_prepare


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
 * @operation{ 
 * &     b_k' \leftarrow b_k \cdot 2^{-b\_shr}                                       \\
 * &     Re\\{a\\} \leftarrow \sum_{k=0}^{length-1} \left( Re\\{b_k'\\} \right)      \\
 * &     Im\\{a\\} \leftarrow \sum_{k=0}^{length-1} \left( Im\\{b_k'\\} \right)      
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then @math{a} is the complex 64-bit 
 * mantissa of floating-point value @math{a \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp + b\_shr}.
 * 
 * The function xs3_vect_complex_s32_sum_prepare() can be used to obtain values for @math{a\_exp} and @math{b\_shr} 
 * based on the input exponents @math{b\_exp} and @math{c\_exp} and the input headrooms @math{b\_hr} and @math{c\_hr}.
 * @endparblock
 * 
 * @par Additional Details
 * @parblock
 * 
 * Internally the sum accumulates into four separate complex 40-bit accumulators. These accumulators apply symmetric 
 * 40-bit saturation logic (with bounds @math{\pm 2^{39}-1}) with each added element. At the end, the 4 accumulators are
 * summed together into the 64-bit fields of `a`. No saturation logic is applied at this final step.
 * 
 * In the most extreme case, each @math{b_k} may be @math{-2^{31}}. @math{256} of these added into the same accumulator 
 * is @math{-2^{39}} which would saturate to @math{-2^{39}+1}, introducing 1 LSb of error (which may or may not be 
 * acceptable given a particular circumstance). The final result for each part then may be as large as 
 * @math{4\cdot(-2^{39}+1) = -2^{41}+4 }, each fitting into a 42-bit signed integer.
 * @endparblock
 * 
 * @param[out]  a           Complex sum @math{a}
 * @param[in]   b           Complex input vector @vector{b}.
 * @param[in]   length      Number of elements in vector @vector{b}.
 * @param[in]   b_shr       Right-shift appled to @vector{b}.
 * 
 * @exception ET_LOAD_STORE Raised if `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @see xs3_vect_complex_s32_sum_prepare
 * 
 * @ingroup xs3_vect32_func
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
 * @parblock
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
 * @endparblock
 * 
 * @param[out]  a_exp                   Exponent associated with output mantissa @math{a}
 * @param[out]  b_shr                   Signed arithmetic right-shift for @vector{b} used by xs3_vect_complex_s32_sum()
 * @param[in]   b_exp                   Exponent associated with input mantissa vector @vector{b}
 * @param[in]   b_hr                    Headroom of input mantissa vector @vector{b}
 * @param[in]   length                  Number of elements in @vector{b}
 * 
 * @see xs3_vect_complex_s32_sum
 * 
 * @ingroup xs3_vect32_prepare
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
 * @parblock
 * 
 * `x[]` represents the complex 32-bit vector @vector{x}, which is both an input to and an output of this function. 
 * `x[]` must begin at a word-aligned address.
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
 * @ingroup xs3_vect32_func
 */
C_API
void xs3_vect_complex_s32_tail_reverse(
    complex_s32_t x[],
    const unsigned length);
    

/**
 * @brief Get the complex conjugate of a complex 32-bit vector.
 * 
 * The complex conjugate of a complex scalar @math{z = x + yi} is @math{z^* = x - yi}.
 * This function computes the complex conjugate of each element of @vector{b} (negates the imaginary
 * part of each element) and places the result in @vector{a}.
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
 * @exception ET_LOAD_STORE Raised if `a` or `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup xs3_vect32_func
 */
C_API
void xs3_vect_complex_s32_conjugate(
    complex_s32_t a[],
    const complex_s32_t b[],
    const unsigned length);


#ifdef __XC__
}   //extern "C"
#endif

