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
 * @page page_xs3_vect_ch_pair_s32_h  xs3_vect_ch_pair_s32.h
 * 
 * This header contains functions implementing arithmetic operations on 32-bit channel-pair vectors, 
 * optimized for xCore XS3.
 * 
 * @note This header is included automatically through `xs3_math.h` or `bfp_math.h`.
 * 
 * @ingroup xs3_math_header_file
 */



/**
 * @brief Calculate the headroom of a 32-bit channel-pair vector.
 * 
 * The headroom of an N-bit integer is the number of bits that the integer's value may be left-shifted
 * without any information being lost. Equivalently, it is one less than the number of leading sign bits.
 * 
 * The headroom of a `ch_pair_s32_t` struct is the minimum of the headroom of each of its 16-bit fields,
 * `ch_a` and `ch_b`.
 * 
 * The headroom of a 32-bit channel-pair vector is the minimum of the headroom of each of its
 * `ch_pair_s32_t` elements.
 * 
 * This function efficiently traverses the elements of @vector{b} to determine its headroom.
 * 
 * `b[]` represents a 32-bit channel-pair vector @vector{b}. `b[]` must begin at a word-aligned address.
 * 
 * `length` is the number of elements in `b[]`.
 * 
 * @operation{
 *      a \leftarrow min\!\\{ HR_{32}\left(x_0\right), HR_{32}\left(x_1\right), ..., 
 *              HR_{32}\left(x_{length-1}\right) \\}
 * }
 * 
 * @param[in]   b       Input channel-pair vector @vector{b}
 * @param[in]   length  Number of elements in @vector{b}
 * 
 * @returns     @math{a}, the headroom of vector @vector{b}
 * 
 * @exception ET_LOAD_STORE Raised if `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @see xs3_vect_ch_pair_s16_headroom, 
 *      xs3_vect_s16_headroom, 
 *      xs3_vect_s32_headroom, 
 *      xs3_vect_complex_s16_headroom, 
 *      xs3_vect_complex_s32_headroom
 * 
 * @ingroup xs3_vect32_func
 */
C_API
headroom_t xs3_vect_ch_pair_s32_headroom(
    const ch_pair_s32_t b[],
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
 * @operation{ 
 * &     ChA\\{a_k\\} \leftarrow ch\_a \\
 * &     ChB\\{a_k\\} \leftarrow ch\_b \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If `ch_a` and `ch_b` are channel A and channel B mantissas of a floating-point channel-pair with exponent 
 * @math{b\_exp}, then the output channel-pair vector @vector{a} are the mantissas of BFP channel-pair vector 
 * @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp}.
 * @endparblock
 * 
 * @param[out]  a           Output channel-pair vector @vector{a}
 * @param[in]   ch_b        Value to set channel A of elements of @vector{a} to
 * @param[in]   ch_a        Value to set channel B of elements of @vector{a} to
 * @param[in]   length      Number of elements in vector{a}
 * 
 * @exception ET_LOAD_STORE Raised if `a` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @see xs3_vect_ch_pair_s16_set, 
 *      xs3_vect_s16_set, 
 *      xs3_vect_s32_set, 
 *      xs3_vect_complex_s16_set, 
 *      xs3_vect_complex_s32_set
 * 
 * @ingroup xs3_vect32_func
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
 * @operation{ 
 * &     ChA\\{a_k\\} \leftarrow sat_{32}(\lfloor ChA\\{b_k\\} \cdot {2^{b\_shl}} \rfloor) \\
 * &     ChB\\{a_k\\} \leftarrow sat_{32}(\lfloor ChB\\{b_k\\} \cdot {2^{b\_shl}} \rfloor) \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1)
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the mantissas of a BFP channel-pair vector @math{ \bar{b} \cdot 2^{b\_exp} }, then the resulting 
 * vector @vector{a} are the mantissas of BFP channel-pair vector @math{\bar{a} \cdot 2^{a\_exp}}, where 
 * @math{\bar{a} = \bar{b} \cdot 2^{b\_shl}} and @math{a\_exp = b\_exp}.
 * @endparblock
 * 
 * @param[out]  a           Output channel-pair vector @vector{a}
 * @param[in]   b           Input channel-pair vector @vector{b}
 * @param[in]   length      Number of elements in @vector{a} and @vector{b}
 * @param[in]   b_shl       Arithmetic left-shift applied to elements of @vector{b}
 * 
 * @returns     Headroom of output vector @vector{a}
 * 
 * @exception ET_LOAD_STORE Raised if `a` or `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @see xs3_vect_ch_pair_s32_shr
 * 
 * @ingroup xs3_vect32_func
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
 * @operation{ 
 * &     ChA\\{a_k\\} \leftarrow sat_{32}(\lfloor ChA\\{b_k\\} \cdot {2^{-b\_shr}} \rfloor) \\
 * &     ChB\\{a_k\\} \leftarrow sat_{32}(\lfloor ChB\\{b_k\\} \cdot {2^{-b\_shr}} \rfloor) \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1)
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the mantissas of a BFP channel-pair vector @math{ \bar{b} \cdot 2^{b\_exp} }, then the resulting 
 * vector @vector{a} are the mantissas of BFP channel-pair vector @math{\bar{a} \cdot 2^{a\_exp}}, where 
 * @math{\bar{a} = \bar{b} \cdot 2^{-b\_shr}} and @math{a\_exp = b\_exp}.
 * @endparblock
 * 
 * @param[out]  a           Output channel-pair vector @vector{a}
 * @param[in]   b           Input channel-pair vector @vector{b}
 * @param[in]   length      Number of elements in @vector{a} and @vector{b}
 * @param[in]   b_shr       Arithmetic right-shift applied to elements of @vector{b}
 * 
 * @returns     Headroom of output vector @vector{a}
 * 
 * @exception ET_LOAD_STORE Raised if `a` or `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @see xs3_vect_ch_pair_s32_shr
 * 
 * @ingroup xs3_vect32_func
 */
C_API
headroom_t xs3_vect_ch_pair_s32_shr(
    ch_pair_s32_t a[],
    const ch_pair_s32_t b[],
    const unsigned length,
    const right_shift_t b_shr);


#ifdef __XC__
}   //extern "C"
#endif

