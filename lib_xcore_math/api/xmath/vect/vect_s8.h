// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include "xmath/xmath_conf.h"
#include "xmath/types.h"
#include "xmath/util.h"

/**
 * @defgroup vect_s8_api   8-bit Vector Functions
 */

#ifdef __XC__
extern "C" {
#endif


/**
 * @brief Determine whether each element of a signed 8-bit input vector are negative.
 * 
 * Each element @math{a_k} of 8-bit output vector @vector{a} is set to 1 if the corresponding
 * element @math{b_k} of 8-bit input vector @vector{b} is negative, and is set to 0 otherwise.
 *
 * `a[]` represents the 8-bit output vector @vector{a}, with the element `a[k]` representing
 * @math{a_k}.
 *
 * `b[]` represents the 8-bit input vector @vector{b}, with the element `b[k]` representing
 * @math{b_k}.
 * 
 * `length` is the number of elements in `a[]` and `b[]`.
 * 
 * @operation{
 * &    a_k \leftarrow \begin{cases}
 *          1 & b_k < 0              \\ 
 *          0 & otherwise\end{cases}   \\
 * &        \qquad\text{ for }k\in 0\ ...\ (length-1)
 *  }
 * 
 * @param[out]  a       Output vector @vector{a}
 * @param[in]   b       Input vector @vector{b}
 * @param[in]   length  Number of elements in @vector{a} and @vector{b}
 * 
 * @exception ET_LOAD_STORE Raised if `a` or `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_s8_api
 */
C_API
void vect_s8_is_negative(
    int8_t a[],
    const int8_t b[],
    const unsigned length);
    


/**
 * @brief Multiply-accumulate an 8-bit matrix by an 8-bit vector into 32-bit accumulators.
 * 
 * This function multiplies an 8-bit @math{M \times N} matrix @vector{W} by an 8-bit 
 * @math{N}-element column vector @vector{v} and adds it to the 32-bit accumulator vector 
 * @vector{a}.
 * 
 * `accumulators` is the output vector @vector{a} to which the product @math{\bar W\times\bar v}
 * is accumulated. Note that the accumulators are encoded in a format native to the xcore VPU. 
 * To initialize the accumulator vector to zeros, just zero the memory.
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
 * @ingroup vect_s8_api
 */
C_API
void mat_mul_s8_x_s8_yield_s32 (
    split_acc_s32_t accumulators[],
    const int8_t matrix[],
    const int8_t input_vect[],
    const unsigned M_rows,
    const unsigned N_cols);




#ifdef __XC__
}   //extern "C"
#endif

