// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include <stdint.h>

#include "xs3_math_conf.h"
#include "xs3_math_types.h"
#include "xs3_util.h"

#ifdef __XC__
extern "C" {
#endif


/**
 * @page page_xs3_mixed_h  xs3_mixed.h
 * 
 * This header contains XS3-optimized functions implementing arithmetic operations where the input(s)
 * and output vectors are not of the same bit-depth.
 * 
 * @note This header is included automatically through `xs3_math.h` or `bfp_math.h`.
 * 
 * @ingroup xs3_math_header_file
 */



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
 * @parblock
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
 * @operation{
 * &     a_k \leftarrow sat_{16}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)      \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * }
 * @endparblock
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the 32-bit mantissas of a BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the resulting 
 * vector @vector{a} are the 16-bit mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where
 * @math{a\_exp = b\_exp + b\_shr}.
 * @endparblock
 * 
 * @param[out]  a        Output vector @vector{a}
 * @param[in]   b        Input vector @vector{b}
 * @param[in]   length   Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]   b_shr    Right-shift appled to @vector{b}
 * 
 * @see xs3_vect_s16_to_s32
 * 
 * @exception ET_LOAD_STORE Raised if `a` or `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup xs3_mixed_vect_func
 */
C_API
void xs3_vect_s32_to_s16(
    int16_t a[],
    const int32_t b[],
    const unsigned length,
    const right_shift_t b_shr);


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
 * @operation{
 * &     a_k \leftarrow b_k \cdot 2^{8}                  \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1)
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the resulting vector @vector{a}
 * are the 32-bit mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}. If @math{a\_exp = b\_exp - 8}, then this
 * operation has effectively not changed the values represented.
 * @endparblock
 * 
 * @par Notes
 * @parblock
 * * The multiplication by @math{2^8} is an artifact of the VPU's behavior. It turns out to be significantly more
 *   efficient to include the factor of @math{2^8}. If this is unwanted, xs3_vect_s32_shr() can be used with a `b_shr`
 *   value of 8 to remove the scaling afterwards.
 * * The headroom of output vector @vector{a} is not returned by this function. The headroom of the output is always
 *   8 bits greater than the headroom of the input.
 * @endparblock
 * 
 * @param[out]  a           32-bit output vector @vector{a}
 * @param[in]   b           16-bit input vector @vector{b}
 * @param[in]   length      Number of elements in vectors @vector{a} and @vector{b}
 * 
 * @exception ET_LOAD_STORE Raised if `a` or `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup xs3_mixed_vect_func
 */
C_API
void xs3_vect_s16_to_s32(
    int32_t a[],
    const int16_t b[],
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
 * @parblock
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
 * If @vector{b} are the complex 32-bit mantissas of a BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the resulting 
 * vector @vector{a} are the complex 16-bit mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where
 * @math{a\_exp = b\_exp + b\_shr}.
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
 * @see xs3_vect_s32_to_s16, 
 *      xs3_vect_complex_s16_to_complex_s32
 * 
 * @ingroup xs3_mixed_vect_func
 */
C_API
void xs3_vect_complex_s32_to_complex_s16(
    int16_t a_real[],
    int16_t a_imag[],
    const complex_s32_t b[],
    const unsigned length,
    const right_shift_t b_shr);


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
 * @operation{ 
 * &     Re\\{a_k\\} \leftarrow Re\\{b_k\\}              \\
 * &     Im\\{a_k\\} \leftarrow Im\\{b_k\\}              \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1)
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the complex 16-bit mantissas of a BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the resulting 
 * vector @vector{a} are the complex 32-bit mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where
 * @math{a\_exp = b\_exp}.
 * @endparblock
 * 
 * @par Notes
 * @parblock
 * * The headroom of output vector @vector{a} is not returned by this function. The headroom of the output is always
 *   16 bits greater than the headroom of the input.
 * @endparblock
 * 
 * @param[out] a        Complex output vector @vector{a}.
 * @param[in]  b_real   Real part of complex input vector @vector{b}.
 * @param[in]  b_imag   Imaginary part of complex input vector @vector{b}.
 * @param[in]  length   Number of elements in vectors @vector{a} and @vector{b}
 * 
 * @exception ET_LOAD_STORE Raised if `a` is not double-word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup xs3_mixed_vect_func
 */
C_API
void xs3_vect_complex_s16_to_complex_s32(
    complex_s32_t a[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const unsigned length);


/**
 * @brief Extract an 8-bit vector containing the most significant byte of a 16-bit vector.
 * 
 * This is a utility function used, for example, in optimizing mixed-width products. The
 * most significant byte of each element is extracted (without rounding or saturation) and
 * inserted into the output vector.
 * 
 * @param[out]  a     8-bit output vector @vector{a}
 * @param[in]   b     16-bit input vector @vector{b}
 * @param[in]   len   The number of elements in @vector{a} and @vector{b}
 * 
 * @exception ET_LOAD_STORE Raised if `a` or `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @see xs3_vect_s16_extract_low_byte
 * 
 * @ingroup xs3_mixed_vect_func
 */
C_API
void xs3_vect_s16_extract_high_byte(
    int8_t a[],
    const int16_t b[],
    const unsigned len);


/**
 * @brief Extract an 8-bit vector containing the least significant byte of a 16-bit vector.
 * 
 * This is a utility function used, for example, in optimizing mixed-width products. The
 * least significant byte of each element is extracted (without rounding or saturation) and
 * inserted into the output vector.
 * 
 * @param[out]  a     8-bit output vector @vector{a}
 * @param[in]   b     16-bit input vector @vector{b}
 * @param[in]   len   The number of elements in @vector{a} and @vector{b}
 * 
 * @exception ET_LOAD_STORE Raised if `a` or `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @see xs3_vect_s16_extract_high_byte
 * 
 * @ingroup xs3_mixed_vect_func
 */
C_API
void xs3_vect_s16_extract_low_byte(
    int8_t a[],
    const int16_t b[],
    const unsigned len);


/**
 * @brief Multiply-accumulate an 8-bit matrix by an 8-bit vector into 32-bit accumulators.
 * 
 * This function multiplies an 8-bit @math{M \times N} matrix @vector{W} by an 8-bit 
 * @math{N}-element column vector @vector{v} and adds it to the 32-bit accumulator vector 
 * @vector{a}.
 * 
 * `accumulators` is the output vector @vector{a} to which the product @math{\bar W\times\bar v}
 * is accumulated. Note that the accumulators are encoded in a format native to the XS3 VPU. 
 * To initialize the accumulator vector to zeros, just zero the memory.
 * 
 * `matrix` is the matrix @vector{W}. 
 * 
 * `input_vect` is the vector @vector{v}.
 * 
 * `matrix` and `input_vect` must both begin at a word-aligned 
 * offsets.
 * 
 * `M_rows` and `N_rows` are the dimensions @math{M} and @math{N} of matrix @vector{W}. @math{M} 
 * must be a multiple of 16, and @math{N} must be a multiple of 32.
 * 
 * The result of this multiplication is exact, so long as saturation does not occur.
 * 
 * @param[inout]  accumulators  The accumulator vector @vector{a}
 * @param[in]     matrix        The weight matrix @vector{W}
 * @param[in]     input_vect    The input vector @vector{v}
 * @param[in]     M_rows        The number of rows @math{M} in matrix @vector{W}
 * @param[in]     N_cols        The number of columns @math{N} in matrix @vector{W}
 * 
 * @exception ET_LOAD_STORE Raised if `matrix` or `input_vect` is not word-aligned 
 *            (See @ref note_vector_alignment)
 * 
 * @ingroup xs3_mixed_vect_func
 */
C_API
void xs3_mat_mul_s8_x_s8_yield_s32 (
    xs3_split_acc_s32_t accumulators[],
    const int8_t matrix[],
    const int8_t input_vect[],
    const unsigned M_rows,
    const unsigned N_cols);


/**
 * @brief Multiply an 8-bit matrix by a 16-bit vetor for a 32-bit result vector.
 * 
 * This function multiplies an 8-bit @math{M \times N} matrix @vector{W} by a 16-bit 
 * @math{N}-element column vector @vector{v} and returns the result as a 32-bit @math{M}-element
 * vector @vector{a}.
 * 
 * `output` is the output vector @vector{a}.
 * 
 * `matrix` is the matrix @vector{W}. 
 * 
 * `input_vect` is the vector @vector{v}.
 * 
 * `matrix` and `input_vect` must both begin at a word-aligned 
 * offsets.
 * 
 * `M_rows` and `N_rows` are the dimensions @math{M} and @math{N} of matrix @vector{W}. @math{M} 
 * must be a multiple of 16, and @math{N} must be a multiple of 32.
 * 
 * `scratch` is a pointer to a word-aligned buffer that this function may use to store 
 * intermediate results. This buffer must be at least @math{N} bytes long.
 * 
 * The result of this multiplication is exact, so long as saturation does not occur.
 * 
 * @param[inout]  output        The output vector @vector{a}
 * @param[in]     matrix        The weight matrix @vector{W}
 * @param[in]     input_vect    The input vector @vector{v}
 * @param[in]     M_rows        The number of rows @math{M} in matrix @vector{W}
 * @param[in]     N_cols        The number of columns @math{N} in matrix @vector{W}
 * @param[in]     scratch       Scratch buffer required by this function.
 * 
 * @exception ET_LOAD_STORE Raised if `matrix` or `input_vect` is not word-aligned 
 *            (See @ref note_vector_alignment)
 * 
 * @ingroup xs3_mixed_vect_func
 */
C_API
void xs3_mat_mul_s8_x_s16_yield_s32 (
    int32_t output[],
    const int8_t matrix[],
    const int16_t input_vect[],
    const unsigned M_rows,
    const unsigned N_cols,
    int8_t scratch[]);


/**
 * @brief Add a scalar to a vector.
 * 
 * Add a scalar to a vector. This works for 8, 16 or 32 bits, real or complex.
 * 
 * `length_bytes` is the total number of bytes to be output. So, for 16-bit vectors, `length_bytes`
 * is twice the number of elements, whereas for complex 32-bit vectors, `length_bytes` is 8 times
 * the number of elements.
 * 
 * `c` and `d` are the values that populate the internal buffer to be added to the input vector as
 * follows:  Internally an 8 word (32 byte) buffer is allocated (on the stack).  Even-indexed words
 * are populated with `c` and odd-indexed words are populated with `d`.  For real vectors, `c` and
 * `d` should be the same value -- the reason for `d` is to allow this same function to work for
 * complex 32-bit vectors.  This also means that for 16-bit vectors, the value to be added needs to
 * be duplicated in both the higher 2 bytes and lower 2 bytes of the word.
 * 
 * `mode_bits` should be `0x0000` for 32-bit mode, `0x0100` for 16-bit mode or `0x0200` for 8-bit
 * mode.
 * 
 * @ingroup xs3_mixed_vect_func
 */
C_API
unsigned xs3_vect_sXX_add_scalar(
    int32_t a[],
    const int32_t b[],
    const unsigned length_bytes,
    const int32_t c,
    const int32_t d,
    const right_shift_t b_shr,
    const unsigned mode_bits);

#ifdef __XC__
}   //extern "C"
#endif

