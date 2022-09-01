// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include "xs3_math_conf.h"
#include "xs3_math_types.h"
#include "xs3_util.h"
#include "xs3_vpu_info.h"

#ifdef __XC__
extern "C" {
#endif


/**
 * @page page_xs3_chunk_s32_h  xs3_chunk_s32.h
 * 
 * This header contains functions implementing arithmetic operations on 32-bit vector "chunks", 
 * optimized for xCore XS3. A "chunk" is an 8-element array of values, enough to fill a single
 * VPU vector register.
 * 
 * Chunk functions are highly optimized computational building blocks that can be used either on
 * their own or as components of other functions.
 * 
 * @note This header is included automatically through `xs3_math.h` or `bfp_math.h`.
 * 
 * @ingroup xs3_math_header_file
 */


/**
 * @defgroup xs3_chunk32_func   32-bit Chunk (8-Element Vector) Functions
 */


/**
 * @brief Compute the inner product between two vector chunks.
 * 
 * This function computes the inner product of two vector chunks, @vector{b} and @vector{c}. 
 * 
 * Conceptually, elements of @vector{b} may have any number of fractional bits (int, fixed-point, 
 * mantissas of a BFP vector) so long as they're all the same. Elements of @vector{c} are Q2.30
 * fixed-point values.  Given that, the returned value @math{a} will have the same number of 
 * fractional bits as @vector{b}.
 * 
 * Only the lowest 32 bits of the sum @math{a} are returned.
 * 
 * @operation{
 * &    a \leftarrow  \sum_{k=0}^{\mathtt{VPU\_INT32\_EPV}-1} \left(  round\left( \frac{b_k\cdot{}c_k}{2^{30}} \right) \right)
 * }
 * 
 * @param[in]   b   Input chunk @vector{b}
 * @param[in]   c   Input chunk @vector{c}
 * 
 * @returns @math{a}
 * 
 * @ingroup xs3_chunk32_func
 */
C_API
int32_t xs3_chunk_s32_dot(
    const int32_t b[VPU_INT32_EPV],
    const q2_30 c[VPU_INT32_EPV]);


/**
 * @brief Compute the natural log of a vector chunk of 32-bit values.
 * 
 * This function computes the natural logarithm of each of the 8 elements in vector chunk
 * @vector{b}. The result is returned as an 8-element chunk @vector{a} of Q8.24 values.
 * 
 * `b_exp` is the exponent associated with elements of @vector{b}.
 * 
 * Any input @math{b_k \le 0} will result in a corresponding output @math{a_k = \mathtt{INT32_MIN}}.
 * 
 * @operation{
 * &    a_k  \leftarrow          \
 *      \begin{cases}  log(b_k\cdot{}2^{\mathtt{b\_exp}})   & b_k \gt 0  \\
 *                     \mathtt{INT32\_MIN}  & \text{otherwise} \\
 *      \end{cases} \\ 
 * &    \qquad\text{for }k \in \{0..\mathtt{VPU\_INT32\_EPV}-1\} 
 * }
 * 
 * @param[out]  a       Output vector chunk @vector{a}
 * @param[in]   b       Input vector chunk @vector{b}
 * @param[in]   b_exp   Exponent associated with @vector{b}
 * 
 * @exception ET_LOAD_STORE Raised if `b` or `a` is not double word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup xs3_chunk32_func
 */
C_API
void xs3_chunk_s32_log(
    q8_24 a[VPU_INT32_EPV],
    const int32_t b[VPU_INT32_EPV],
    const exponent_t b_exp);


/**
 * @brief Compute the natural log of a vector chunk of `float_s32_t`.
 * 
 * This function computes the natural logarithm of each of the `VPU_INT32_EPV` elements in vector
 * chunk @vector{b}. The result is returned as an 8-element chunk @vector{a} of Q8.24 values.
 * 
 * Any input @math{b_k \le 0} will result in a corresponding output @math{a_k = \mathtt{INT32_MIN}}.
 * 
 * @operation{
 * &    a_k  \leftarrow          \
 *      \begin{cases}  log(b_k)   & b_k \gt 0  \\
 *                     \mathtt{INT32\_MIN}  & \text{otherwise} \\
 *      \end{cases} \\ 
 * &    \qquad\text{for }k \in \{0..\mathtt{VPU\_INT32\_EPV}-1\} 
 * }
 * 
 * @param[out]  a   Output vector chunk @vector{a}
 * @param[in]   b   Input vector chunk @vector{b}
 * 
 * @exception ET_LOAD_STORE Raised if `b` or `a` is not double word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup xs3_chunk32_func
 */
C_API
void xs3_chunk_float_s32_log(
    q8_24 a[VPU_INT32_EPV],
    const float_s32_t b[VPU_INT32_EPV]);


/**
 * @brief Compute a power series on a vector chunk of Q2.30 values.
 * 
 * This function is used to compute a power series summation on a vector chunk 
 * (`VPU_INT32_EPV`-element vector) @vector{b}. @vector{b} contains Q2.30 values. @vector{c} is a 
 * vector containing coefficients to be multiplied by powers of @vector{b}, and may have any 
 * associated exponent. The output is vector chunk @vector{a} and has the same exponent as 
 * @vector{c}.
 * 
 * `c[]` is an array with shape `(term_count, VPU_INT32_EPV)`, where the second axis contains the 
 * same value replicated across all `VPU_INT32_EPV` elements. That is, `c[k][i] = c[k][j]` for `i` 
 * and `j` in `0..(VPU_INT32_EPV-1)`. This is for performance reasons. (For the purpose of this 
 * explanation, @vector{c} is considered to be single-dimensional, without redundancy.)
 * 
 * @operation{
 * &    b_{k,0} = 2^{30}                                                                   \\
 * &    b_{k,i} = round\left(\frac{b_{k,i-1}\cdot{}b_k}{2^{30}}\right)\qquad\text{for }i \in \{1..(N-1)\}    \\
 * &    a_k \leftarrow  \sum_{i=0}^{N-1}  round\left( \frac{b_{k,i}\cdot c_i}{2^{30}}  \right) \\
 * &    \qquad\text{for }k \in \{0..\mathtt{VPU\_INT32\_EPV}-1\}
 * }
 * 
 * @param[out]  a           Output vector chunk @vector{a}
 * @param[in]   b           Input vector chunk @vector{b}
 * @param[in]   c           Coefficient vector @vector{c}
 * @param[in]   term_count  Number of power series terms, @math{N}
 * 
 * @ingroup xs3_chunk32_func
 */
C_API
void xs3_chunk_q30_power_series(
    int32_t a[VPU_INT32_EPV],
    const q2_30 b[VPU_INT32_EPV],
    const int32_t c[],
    const unsigned term_count);


/**
 * @brief Compute @math{e^b} on a vector chunk of Q2.30 values.
 * 
 * This function computes @math{e^{b_k}} for each element of a vector chunk (`VPU_INT32_EPV`-element 
 * vector) @vector{b} of Q2.30 values near @math{0}. The result is computed using the power series 
 * approximation of @math{e^x} near zero. It is recommended that this function only be used for 
 * @math{ -0.5 \le b_k\cdot{}2^{-30} \le 0.5}.
 * 
 * The output vector chunk @vector{a} is also in a Q2.30 format.
 * 
 * @operation{
 * &    a_k \leftarrow e^{b_k\cdot{}2^{-30}}    \\
 * &    \qquad\text{for }k \in \{0..\mathtt{VPU\_INT32\_EPV}\}
 * }
 * 
 * @param[out]  a   Output vector chunk @vector{a}
 * @param[in]   b   Input vector chunk @vector{b}
 * 
 * @ingroup xs3_chunk32_func
 */
C_API
void xs3_chunk_q30_exp_small(
    q2_30 a[VPU_INT32_EPV],
    const q2_30 b[VPU_INT32_EPV]);


#ifdef __XC__
}   //extern "C"
#endif

