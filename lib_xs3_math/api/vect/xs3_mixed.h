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
 * @brief Multiply-accumulate an 8-bit matrix by an 8-bit vector into 32-bit accumulators.
 * 
 * This function multiplies an 8-bit @math{M \times N} matrix @vector{W} by an 8-bit 
 * @math{N}-element column vector @vector{v} and adds it to the 32-bit accumulator vector 
 * @vector{a}.
 * 
 * `accumulators` is the output vector @vector{a} to which the product @math{\bar W\times\barv}
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
 * `M_rows` and `N_rows` are the dimensions @math{M} and @math{N} of matrix @vector{W}. @{M} 
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
 * `M_rows` and `N_rows` are the dimensions @math{M} and @math{N} of matrix @vector{W}. @{M} 
 * must be a multiple of 16, and @math{N} must be a multiple of 32.
 * 
 * The result of this multiplication is exact, so long as saturation does not occur.
 * 
 * @param[inout]  output        The output vector @vector{a}
 * @param[in]     matrix        The weight matrix @vector{W}
 * @param[in]     input_vect    The input vector @vector{v}
 * @param[in]     M_rows        The number of rows @math{M} in matrix @vector{W}
 * @param[in]     N_cols        The number of columns @math{N} in matrix @vector{W}
 */
C_API
void xs3_mat_mul_s8_x_s16_yield_s32 (
    int32_t output[],
    const int8_t weights[],
    const int16_t input[],
    const unsigned M_rows,
    const unsigned N_cols,
    int8_t scratch[]);


#ifdef __XC__
}   //extern "C"
#endif

