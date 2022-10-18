// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include <stdint.h>

#include "xmath/xmath_conf.h"
#include "xmath/types.h"
#include "xmath/util.h"


/**
 * @defgroup vect_mixed_api   Vector functions which operate on a mix of input bit-depths
 */

#ifdef __XC__
extern "C" {
#endif


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
 * `matrix` and `input_vect` must both begin at a word-aligned offsets.
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
 * @ingroup vect_mixed_api
 */
C_API
void mat_mul_s8_x_s16_yield_s32 (
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
 * @ingroup vect_mixed_api
 */
C_API
unsigned vect_sXX_add_scalar(
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

