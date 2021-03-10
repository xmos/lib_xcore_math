// Copyright 2020 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public License: Version 1.

#ifndef BFP_H_
#define BFP_H_

#include "xs3_math_types.h"


#ifdef __XC__
extern "C" {
#endif



/**
 * @page bfp_vector_functions16 16-bit BFP Functions
 * 
 * Below is a listing of the block floating-point API functions provided by this library that operate on 16-bit data.
 */

/**
 * @page bfp_vector_functions32 32-bit BFP Functions
 * 
 * Below is a listing of the block floating-point API functions provided by this library that operate on 32-bit data.
 */



/*
    Saturation Logic:

    Where specified, the VPU-enhanced arithmetic functions apply symmetric saturation logic, which is applied
    where results would otherwise overflow.

    The resulting integer range for N-bit saturation is   -(2^(N-1))+1  to  (2^(N-1))-1.

    8-bit:      (-127, 127)
    16-bit:     (-65535, 65535)
    32-bit:     (-2147483647, 2147483647)
*/



/** 
 * @brief Get the headroom of a 16-bit BFP vector.
 * 
 * The headroom of a vector is the number of bits its elements can be left-shifted without losing any information. It 
 * conveys information about the range of values that vector may contain, which is useful for determining how best to 
 * preserve precision in potentially lossy block floating-point operations.
 * 
 * In a BFP context, headroom applies to mantissas only, not exponents.
 * 
 * In particular, if the 16-bit mantissa vector @vector{x} has @math{N} bits of headroom, then for any element 
 * @math{x_k} of @vector{x}
 * 
 * @math{-2^{15-N} \le x_k \lt 2^{15-N}}
 * 
 * And for any element @math{X_k = x_k \cdot 2^{x\_exp}} of a complex BFP vector @vector{X}
 * 
 * @math{-2^{15 + x\_exp - N} \le X_k \lt 2^{15 + x\_exp - N} }
 * 
 * This function determines the headroom of `b`, updates `b->hr` with that value, and then returns `b->hr`.
 *
 * @param   b         BFP vector to get the headroom of
 * 
 * @returns    Headroom of BFP vector `b` 
 */
headroom_t bfp_s16_headroom(
    bfp_s16_t* a);


/** 
 * @brief Get the headroom of a 32-bit BFP vector.
 * 
 * The headroom of a vector is the number of bits its elements can be left-shifted without losing any information. It 
 * conveys information about the range of values that vector may contain, which is useful for determining how best to 
 * preserve precision in potentially lossy block floating-point operations.
 * 
 * In a BFP context, headroom applies to mantissas only, not exponents.
 * 
 * In particular, if the 32-bit mantissa vector @vector{x} has @math{N} bits of headroom, then for any element 
 * @math{x_k} of @vector{x}
 * 
 * @math{-2^{31-N} \le x_k \lt 2^{31-N}}
 * 
 * And for any element @math{X_k = x_k \cdot 2^{x\_exp}} of a complex BFP vector @vector{X}
 * 
 * @math{-2^{31 + x\_exp - N} \le X_k \lt 2^{31 + x\_exp - N} }
 * 
 * This function determines the headroom of `b`, updates `b->hr` with that value, and then returns `b->hr`.
 *
 * @param   b         BFP vector to get the headroom of
 * 
 * @returns    Headroom of BFP vector `b` 
 */
headroom_t bfp_s32_headroom(
    bfp_s32_t* a);


/** 
 * @brief Apply a left-shift to the mantissas of a 16-bit BFP vector.
 * 
 * Each mantissa of input BFP vector @vector{B} is left-shifted `b_shl` bits and stored in the corresponding element of
 * output BFP vector @vector{A}.
 * 
 * This operation can be used to add or remove headroom from a BFP vector.
 * 
 * `b_shl` is the number of bits that each mantissa will be left-shifted. This shift is signed and arithmetic, so 
 * negative values for `b_shl` will right-shift the mantissas.
 * 
 * `a` and `b` must have been initialized (see bfp_s16_init()), and must be the same length.
 * 
 * This operation can be performed safely in-place on `b`.
 * 
 * Note that this operation bypasses the logic protecting the caller from saturation or underflows. Output values 
 * saturate to the symmetric 16-bit range (@math{-2^{15} \lt \lt 2^{15}}). To avoid saturation, `b_shl` should be no
 * greater than the headroom of `b` (`b->hr`).
 * 
 * @bfp_op{16, @f$
 *      a_k \leftarrow sat_{16}( \lfloor b_k \cdot 2^{b\_shl} \rfloor )     \\
 *          \qquad\text{for } k \in 0\ ...\ (N-1)                           \\
 *          \qquad\text{where } N \text{ is the length of } \bar{B}         \\
 *          \qquad\text{  and } b_k \text{ and } a_k \text{ are the } k\text{th mantissas from } 
 *              \bar{B}\text{ and } \bar{A}\text{ respectively}
 * @f$ }
 * 
 * @param[out] a        Output BFP vector @vector{A}
 * @param[in]  b        Input BFP vector @vector{B}
 * @param[in]  b_shl    Signed arithmetic left-shift to be applied to mantissas of @vector{B}.
 */
void bfp_s16_shl(
    bfp_s16_t* a,
    const bfp_s16_t* b,
    const left_shift_t b_shl);


/** 
 * @brief Apply a left-shift to the mantissas of a 32-bit BFP vector.
 * 
 * Each mantissa of input BFP vector @vector{B} is left-shifted `b_shl` bits and stored in the corresponding element of
 * output BFP vector @vector{A}.
 * 
 * This operation can be used to add or remove headroom from a BFP vector.
 * 
 * `b_shl` is the number of bits that each mantissa will be left-shifted. This shift is signed and arithmetic, so 
 * negative values for `b_shl` will right-shift the mantissas.
 * 
 * `a` and `b` must have been initialized (see bfp_s32_init()), and must be the same length.
 * 
 * This operation can be performed safely in-place on `b`.
 * 
 * Note that this operation bypasses the logic protecting the caller from saturation or underflows. Output values 
 * saturate to the symmetric 32-bit range (@math{-2^{31} \lt \lt 2^{31}}). To avoid saturation, `b_shl` should be no
 * greater than the headroom of `b` (`b->hr`).
 * 
 * @bfp_op{32, @f$
 *      a_k \leftarrow sat_{32}( \lfloor b_k \cdot 2^{b\_shl} \rfloor )     \\
 *          \qquad\text{for } k \in 0\ ...\ (N-1)                           \\
 *          \qquad\text{where } N \text{ is the length of } \bar{B}         \\
 *          \qquad\text{  and } b_k \text{ and } a_k \text{ are the } k\text{th mantissas from } 
 *              \bar{B}\text{ and } \bar{A}\text{ respectively}
 * @f$ }
 * 
 * @param[out] a        Output BFP vector @vector{A}
 * @param[in]  b        Input BFP vector @vector{B}
 * @param[in]  b_shl    Signed arithmetic left-shift to be applied to mantissas of @vector{B}.
 */
void bfp_s32_shl(
    bfp_s32_t* a,
    const bfp_s32_t* b,
    const left_shift_t shl);



/** 
 * @brief Add two 16-bit BFP vectors together.
 * 
 * Add together two input BFP vectors @vector{B} and @vector{C} and store the result
 * in BFP vector @vector{A}. 
 * 
 * `a`, `b` and `c` must have been initialized (see bfp_s16_init()), and must be the same length.
 * 
 * This operation can be performed safely in-place on `b` or `c`.
 * 
 * @bfp_op{16, @f$ 
 *      \bar{A} \leftarrow \bar{B} + \bar{C}  
 * @f$ }
 * 
 * @param[out] a     Output BFP vector @vector{A}
 * @param[in]  b     Input BFP vector @vector{B}
 * @param[in]  c     Input BFP vector @vector{C}
 */
void bfp_s16_add(
    bfp_s16_t* a, 
    const bfp_s16_t* b, 
    const bfp_s16_t* c);


/** 
 * @brief Add two 32-bit BFP vectors together.
 * 
 * Add together two input BFP vectors @vector{B} and @vector{C} and store the result in BFP vector @vector{A}. 
 * 
 * `a`, `b` and `c` must have been initialized (see bfp_s32_init()), and must be the same length.
 * 
 * This operation can be performed safely in-place on `b` or `c`.
 * 
 * @bfp_op{32, @f$ 
 *      \bar{A} \leftarrow \bar{B} + \bar{C}  
 * @f$ }
 * 
 * @param[out] a     Output BFP vector @vector{A}
 * @param[in]  b     Input BFP vector @vector{B}
 * @param[in]  c     Input BFP vector @vector{C}
 */
void bfp_s32_add(
    bfp_s32_t* a, 
    const bfp_s32_t* b, 
    const bfp_s32_t* c);



/** 
 * @brief Subtract one 16-bit BFP vector from another.
 * 
 * Subtract input BFP vector @vector{C} from input BFP vector @vector{C} and store the result
 * in BFP vector @vector{A}. 
 * 
 * `a`, `b` and `c` must have been initialized (see bfp_s16_init()), and must be the same length.
 * 
 * This operation can be performed safely in-place on `b` or `c`.
 * 
 * @bfp_op{16, @f$ 
 *      \bar{A} \leftarrow \bar{B} - \bar{C}  
 * @f$ }
 * 
 * @param[out] a     Output BFP vector @vector{A}
 * @param[in]  b     Input BFP vector @vector{B}
 * @param[in]  c     Input BFP vector @vector{C}
 */
void bfp_s16_sub(
    bfp_s16_t* a, 
    const bfp_s16_t* b, 
    const bfp_s16_t* c);

/** 
 * @brief Subtract one 32-bit BFP vector from another.
 * 
 * Subtract input BFP vector @vector{C} from input BFP vector @vector{C} and store the result
 * in BFP vector @vector{A}. 
 * 
 * `a`, `b` and `c` must have been initialized (see bfp_s32_init()), and must be the same length.
 * 
 * This operation can be performed safely in-place on `b` or `c`.
 * 
 * @bfp_op{32, @f$ 
 *      \bar{A} \leftarrow \bar{B} - \bar{C}  
 * @f$ }
 * 
 * @param[out] a     Output BFP vector @vector{A}
 * @param[in]  b     Input BFP vector @vector{B}
 * @param[in]  c     Input BFP vector @vector{C}
 */
void bfp_s32_sub(
    bfp_s32_t* a, 
    const bfp_s32_t* b, 
    const bfp_s32_t* c);


/** 
 * @brief Multiply one 16-bit BFP vector by another element-wise.
 * 
 * Multiply each element of input BFP vector @vector{B} by the corresponding element of input BFP vector @vector{C} 
 * and store the results in output BFP vector @vector{A}.
 * 
 * `a`, `b` and `c` must have been initialized (see bfp_s16_init()), and must be the same length.
 * 
 * This operation can be performed safely in-place on `b` or `c`.
 * 
 * @bfp_op{16, @f$ 
 *      A_k \leftarrow B_k \cdot C_k                    \\
 *          \qquad\text{for } k \in 0\ ...\ (N-1)       \\
 *          \qquad\text{where } N \text{ is the length of } \bar{B}\text{ and }\bar{C}
 * @f$ }
 * 
 * @param a     Output BFP vector @vector{A}
 * @param b     Input BFP vector @vector{B}
 * @param c     Input BFP vector @vector{C}
 */
void bfp_s16_mul(
    bfp_s16_t* a, 
    const bfp_s16_t* b, 
    const bfp_s16_t* c);



/**
 * @brief Multiply one 32-bit BFP vector by another element-wise.
 * 
 * Multiply each element of input BFP vector @vector{B} by the corresponding element of input BFP vector @vector{C} 
 * and store the results in output BFP vector @vector{A}.
 * 
 * `a`, `b` and `c` must have been initialized (see bfp_s32_init()), and must be the same length.
 * 
 * This operation can be performed safely in-place on `b` or `c`.
 * 
 * @bfp_op{32, @f$ 
 *      A_k \leftarrow B_k \cdot C_k                            \\
 *          \qquad\text{for } k \in 0\ ...\ (N-1)       \\
 *          \qquad\text{where } N \text{ is the length of } \bar{B}\text{ and }\bar{C}
 * @f$ }
 * 
 * @param a     Output BFP vector @vector{A}
 * @param b     Input BFP vector @vector{B}
 * @param c     Input BFP vector @vector{C}
 */
void bfp_s32_mul(
    bfp_s32_t* a, 
    const bfp_s32_t* b, 
    const bfp_s32_t* c);


/** 
 * @brief Multiply a 16-bit BFP vector by a scalar.
 * 
 * Multiply input BFP vector @vector{B} by scalar @math{\alpha \cdot 2^{\alpha\_exp}} and store the result in output 
 * BFP vector @vector{A}.
 * 
 * `a` and `b` must have been initialized (see bfp_s16_init()), and must be the same length.
 * 
 * `alpha` represents the scalar @math{\alpha \cdot 2^{\alpha\_exp}}, where @math{\alpha} is `alpha.mant` and 
 * @math{\alpha\_exp} is `alpha.exp`.
 * 
 * This operation can be performed safely in-place on `b`.
 * 
 * @bfp_op{16, @f$
 *      \bar{A} \leftarrow \bar{B} \cdot \left(\alpha \cdot 2^{\alpha\_exp}\right)
 * @f$ }
 * 
 * @param[out] a            Output BFP vector @vector{A}
 * @param[in]  b            Input BFP vector @vector{B}
 * @param[in]  alpha        Scalar by which @vector{B} is multiplied
 */
void bfp_s16_scale(
    bfp_s16_t* a, 
    const bfp_s16_t* b, 
    const float_s16_t alpha);


/** 
 * @brief Multiply a 32-bit BFP vector by a scalar.
 * 
 * Multiply input BFP vector @vector{B} by scalar @math{\alpha \cdot 2^{\alpha\_exp}} and store the result in output 
 * BFP vector @vector{A}.
 * 
 * `a` and `b` must have been initialized (see bfp_s32_init()), and must be the same length.
 * 
 * `alpha` represents the scalar @math{\alpha \cdot 2^{\alpha\_exp}}, where @math{\alpha} is `alpha.mant` and 
 * @math{\alpha\_exp} is `alpha.exp`.
 * 
 * This operation can be performed safely in-place on `b`.
 * 
 * @bfp_op{32, @f$
 *      \bar{A} \leftarrow \bar{B} \cdot \left(\alpha \cdot 2^{\alpha\_exp}\right)
 * @f$ }
 * 
 * @param[out] a             Output BFP vector @vector{A}
 * @param[in]  b             Input BFP vector @vector{B}
 * @param[in]  alpha        Scalar by which @vector{B} is multiplied
 */
void bfp_s32_scale(
    bfp_s32_t* a, 
    const bfp_s32_t* b,
    const float_s32_t alpha);


/** 
 * @brief Get the absolute values of elements of a 16-bit BFP vector. 
 * 
 * Compute the absolute value of each element @math{B_k} of input BFP vector @vector{B} and store the results in output 
 * BFP vector @vector{A}.
 * 
 * `a` and `b` must have been initialized (see bfp_s16_init()), and must be the same length.
 * 
 * This operation can be performed safely in-place on `b`.
 * 
 * @bfp_op{16, @f$ 
 *      A_k \leftarrow \left| B_k \right|               \\
 *          \qquad\text{for } k \in 0\ ...\ (N-1)       \\
 *          \qquad\text{where } N \text{ is the length of } \bar{B}
 * @f$ }
 * 
 * @param[out] a     Output BFP vector @vector{A}
 * @param[in]  b     Input BFP vector @vector{B}
 */
void bfp_s16_abs(
    bfp_s16_t* a,
    const bfp_s16_t* b);


/** 
 * @brief Get the absolute values of elements of a 32-bit BFP vector. 
 * 
 * Compute the absolute value of each element @math{B_k} of input BFP vector @vector{B} and store the results in output 
 * BFP vector @vector{A}.
 * 
 * `a` and `b` must have been initialized (see bfp_s32_init()), and must be the same length.
 * 
 * This operation can be performed safely in-place on `b`.
 * 
 * @bfp_op{32, @f$ 
 *      A_k \leftarrow \left| B_k \right|               \\
 *          \qquad\text{for } k \in 0\ ...\ (N-1)       \\
 *          \qquad\text{where } N \text{ is the length of } \bar{B}
 * @f$ }
 * 
 * @param[out] a     Output BFP vector @vector{A}
 * @param[in]  b     Input BFP vector @vector{B}
 */
void bfp_s32_abs(
    bfp_s32_t* a,
    const bfp_s32_t* b);


/** 
 * @brief Sum the elements of a 16-bit BFP vector.
 * 
 * Sum the elements of input BFP vector @vector{B} to get a result @math{A = a \cdot 2^{a\_exp}}, which is returned. The
 * returned value has a 32-bit mantissa.
 * 
 * `b` must have been initialized (see bfp_s16_init()).
 * 
 * @bfp_op{16, @f$
 *      A \leftarrow \sum_{k=0}^{N-1} \left( B_k \right)            \\
 *          \qquad\text{where } N \text{ is the length of } \bar{B}
 * @f$ }
 * 
 * @param[in] b         Input BFP vector @vector{B}
 * 
 * @returns  @math{A}, the sum of elements of @vector{B}
 */
float_s32_t bfp_s16_sum(
    const bfp_s16_t* b);


/** 
 * @brief Sum the elements of a 32-bit BFP vector.
 * 
 * Sum the elements of input BFP vector @vector{B} to get a result @math{A = a \cdot 2^{a\_exp}}, which is returned. The
 * returned value has a 64-bit mantissa.
 * 
 * `b` must have been initialized (see bfp_s32_init()).
 * 
 * @bfp_op{32, @f$
 *      A \leftarrow \sum_{k=0}^{N-1} \left( B_k \right)            \\
 *          \qquad\text{where } N \text{ is the length of } \bar{B}
 * @f$ }
 * 
 * 
 * @param[in] b         Input BFP vector @vector{B}
 * 
 * @returns  @math{A}, the sum of elements of @vector{B}
 */
float_s64_t bfp_s32_sum(
    const bfp_s32_t* b);


/** 
 * @brief Compute the inner product of two 16-bit BFP vectors.
 * 
 * Adds together the element-wise products of input BFP vectors @vector{B} and @vector{C} for a result 
 * @math{A = a \cdot 2^{a\_exp}}, where @math{a} is the 64-bit mantissa of the result and @math{a\_exp} is its 
 * associated exponent. @math{A} is returned.
 * 
 * `b` and `c` must have been initialized (see bfp_s16_init()), and must be the same length.
 * 
 * @bfp_op{16, @f$
 *      a \cdot 2^{a\_exp} \leftarrow \sum_{k=0}^{N-1} \left( B_k \cdot C_k \right)     \\
 *          \qquad\text{where } N \text{ is the length of } \bar{B}\text{ and }\bar{C}
 * @f$ }
 * 
 * @param[in]  b        Input BFP vector @vector{B}
 * @param[in]  c        Input BFP vector @vector{C}
 * 
 * @returns     @math{A}, the inner product of vectors @vector{B} and @vector{C}
 */
float_s64_t bfp_s16_dot(
    const bfp_s16_t* b, 
    const bfp_s16_t* c);


/** 
 * @brief Compute the inner product of two 32-bit BFP vectors.
 * 
 * Adds together the element-wise products of input BFP vectors @vector{B} and @vector{C} for a result 
 * @math{A = a \cdot 2^{a\_exp}}, where @math{a} is the 64-bit mantissa of the result and @math{a\_exp} is its 
 * associated exponent. @math{A} is returned.
 * 
 * `b` and `c` must have been initialized (see bfp_s32_init()), and must be the same length.
 * 
 * @bfp_op{32, @f$
 *      a \cdot 2^{a\_exp} \leftarrow \sum_{k=0}^{N-1} \left( B_k \cdot C_k \right)     \\
 *          \qquad\text{where } N \text{ is the length of } \bar{B}\text{ and }\bar{C}
 * @f$ }
 * 
 * @param[in]  b        Input BFP vector @vector{B}
 * @param[in]  c        Input BFP vector @vector{C}
 * 
 * @returns     @math{A}, the inner product of vectors @vector{B} and @vector{C}
 */
float_s64_t bfp_s32_dot(
    const bfp_s32_t* b, 
    const bfp_s32_t* c);


/** 
 * Clamp the elements of a 16-bit BFP vector to a specified range.
 * 
 * Each element @math{A_k} of output BFP vector @vector{A} is set to the corresponding element @math{B_k} of input BFP
 * vector @vector{B} if it is in the range @math{ [ L \cdot 2^{bound\_exp}, U \cdot 2^{bound\_exp} ] }, otherwise it is
 * set to the nearest value inside that range.
 * 
 * `a` and `b` must have been initialized (see bfp_s16_init()), and must be the same length.
 * 
 * This operation can be performed safely in-place on `b`.
 * 
 * @bfp_op{16, @f$
 *      A_k \leftarrow \begin{cases}
 *          L \cdot 2^{bound\_exp}      &   B_k \lt L \cdot 2^{bound\_exp}  \\
 *          U \cdot 2^{bound\_exp}      &   B_k \gt U \cdot 2^{bound\_exp}  \\
 *          B_k                         &   otherwise
 *      \end{cases}                                     \\
 *          \qquad\text{for } k \in 0\ ...\ (N-1)       \\
 *          \qquad\text{where } N \text{ is the length of } \bar{B}
 * @f$ }
 * 
 * @param[out] a             Output BFP vector @vector{A}
 * @param[in]  b             Input BFP vector @vector{B}
 * @param[in]  lower_bound   Mantissa of the lower clipping bound, @math{L}
 * @param[in]  upper_bound   Mantissa of the upper clipping bound, @math{U}
 * @param[in]  bound_exp     Shared exponent of the clipping bounds
 */
void bfp_s16_clip(
    bfp_s16_t* a, 
    const bfp_s16_t* b, 
    const int16_t lower_bound, 
    const int16_t upper_bound, 
    const int bound_exp);


/** 
 * Clamp the elements of a 32-bit BFP vector to a specified range.
 * 
 * Each element @math{A_k} of output BFP vector @vector{A} is set to the corresponding element @math{B_k} of input BFP
 * vector @vector{B} if it is in the range @math{ [ L \cdot 2^{bound\_exp}, U \cdot 2^{bound\_exp} ] }, otherwise it is
 * set to the nearest value inside that range.
 * 
 * `a` and `b` must have been initialized (see bfp_s32_init()), and must be the same length.
 * 
 * This operation can be performed safely in-place on `b`.
 * 
 * @bfp_op{32, @f$
 *      A_k \leftarrow \begin{cases}
 *          L \cdot 2^{bound\_exp}      &   B_k \lt L \cdot 2^{bound\_exp}  \\
 *          U \cdot 2^{bound\_exp}      &   B_k \gt U \cdot 2^{bound\_exp}  \\
 *          B_k                         &   otherwise
 *      \end{cases}                                     \\
 *          \qquad\text{for } k \in 0\ ...\ (N-1)       \\
 *          \qquad\text{where } N \text{ is the length of } \bar{B}
 * @f$ }
 * 
 * @param[out] a             Output BFP vector @vector{A}
 * @param[in]  b             Input BFP vector @vector{B}
 * @param[in]  lower_bound   Mantissa of the lower clipping bound, @math{L}
 * @param[in]  upper_bound   Mantissa of the upper clipping bound, @math{U}
 * @param[in]  bound_exp     Shared exponent of the clipping bounds
 */
void bfp_s32_clip(
    bfp_s32_t* a, 
    const bfp_s32_t* b, 
    const int32_t lower_bound, 
    const int32_t upper_bound, 
    const int bound_exp);


/** 
 * @brief Rectify a 16-bit BFP vector.
 * 
 * Each element @math{A_k} of output BFP vector @vector{A} is set to the corresponding element @math{B_k} of input BFP
 * vector @vector{B} if it is non-negative, otherwise it is set to @math{0}.
 * 
 * `a` and `b` must have been initialized (see bfp_s16_init()), and must be the same length.
 * 
 * This operation can be performed safely in-place on `b`.
 * 
 * @bfp_op{16, @f$
 *      A_k \leftarrow \begin{cases}
 *          0       &   B_k \lt 0       \\
 *          B_k     &   otherwise
 *      \end{cases}                     \\
 *      \qquad\text{for } k \in 0\ ...\ (N-1)       \\
 *      \qquad\text{where } N \text{ is the length of } \bar{B}
 * @f$ }
 * 
 * @param[out] a     Output BFP vector @vector{A}
 * @param[in]  b     Input BFP vector @vector{B}
 */
void bfp_s16_rect(
    bfp_s16_t* a,
    const bfp_s16_t* b);


/** 
 * @brief Rectify a 32-bit BFP vector.
 * 
 * Each element @math{A_k} of output BFP vector @vector{A} is set to the corresponding element @math{B_k} of input BFP
 * vector @vector{B} if it is non-negative, otherwise it is set to @math{0}.
 * 
 * `a` and `b` must have been initialized (see bfp_s32_init()), and must be the same length.
 * 
 * This operation can be performed safely in-place on `b`.
 * 
 * @bfp_op{32, @f$
 *      A_k \leftarrow \begin{cases}
 *          0       &   B_k \lt 0       \\
 *          B_k     &   otherwise
 *      \end{cases}                     \\
 *      \qquad\text{for } k \in 0\ ...\ (N-1)       \\
 *      \qquad\text{where } N \text{ is the length of } \bar{B}
 * @f$ }
 * 
 * @param[out] a     Output BFP vector @vector{A}
 * @param[in]  b     Input BFP vector @vector{B}
 */
void bfp_s32_rect(
    bfp_s32_t* a, 
    const bfp_s32_t* b);

/** 
 * @brief Convert a 32-bit BFP vector into a 16-bit BFP vector.
 * 
 * Reduces the bit-depth of each 32-bit element @math{B_k} of input BFP vector @vector{B} to 16 bits, and stores the 
 * 16-bit result in the corresponding element @math{A_k} of output BFP vector @vector{A}.
 * 
 * `a` and `b` must have been initialized (see bfp_s32_init() and bfp_s16_init()), and must be the same length.
 * 
 * As much precision as possible will be retained.
 * 
 * @bfp_op{32, @f$
 *      A_k \overset{16-bit}{\longleftarrow} B_k        \\
 *          \qquad\text{for } k \in 0\ ...\ (N-1)       \\
 *          \qquad\text{where } N \text{ is the length of } \bar{B}
 * @f$ }
 * 
 * @param[out] a     Output BFP vector @vector{A}
 * @param[in]  b     Input BFP vector @vector{B}
 **/
void bfp_s32_to_s16(
    bfp_s16_t* a,
    const bfp_s32_t* b);

/** 
 * @brief Convert a 16-bit BFP vector into a 32-bit BFP vector.
 * 
 * Increases the bit-depth of each 16-bit element @math{B_k} of input BFP vector @vector{B} to 32 bits, and stores the 
 * 32-bit result in the corresponding element @math{A_k} of output BFP vector @vector{A}.
 * 
 * `a` and `b` must have been initialized (see bfp_s16_init() and bfp_s32_init()), and must be the same length.
 * 
 * @bfp_op{16, @f$
 *      A_k \overset{32-bit}{\longleftarrow} B_k        \\
 *          \qquad\text{for } k \in 0\ ...\ (N-1)       \\
 *          \qquad\text{where } N \text{ is the length of } \bar{B}
 * @f$ }
 * 
 * @param[out] a     Output BFP vector @vector{A}
 * @param[in]  b     Input BFP vector @vector{B}
 **/
void bfp_s16_to_s32(
    bfp_s32_t* a,
    const bfp_s16_t* b);


/** 
 * @brief Get the square roots of elements of a 16-bit BFP vector.
 * 
 * Computes the square root of each element @math{B_k} of input BFP vector @vector{B} and stores the results in output 
 * BFP vector @vector{A}.
 * 
 * `a` and `b` must have been initialized (see bfp_s16_init()), and must be the same length.
 * 
 * This operation can be performed safely in-place on `b`.
 * 
 * @bfp_op{16, @f$ 
 *      A_k \leftarrow \sqrt{B_k}                       \\
 *          \qquad\text{for } k \in 0\ ...\ (N-1)       \\
 *          \qquad\text{where } N \text{ is the length of } \bar{B}
 * @f$ }
 * 
 * @par Notes
 * 
 * * Only the `XS3_BFP_SQRT_DEPTH_S16` (see xs3_math_conf.h) most significant bits of each result are computed.
 * 
 * * This function only computes real roots. For any @math{B_k \lt 0}, the corresponding output @math{A_k} is set to 
 *   @math{0}.
 * 
 * @param[out] a     Output BFP vector @vector{A}
 * @param[in]  b     Input BFP vector @vector{B}
 */
void bfp_s16_sqrt(
    bfp_s16_t* a,
    const bfp_s16_t* b);


/** 
 * @brief Get the square roots of elements of a 32-bit BFP vector.
 * 
 * Computes the square root of each element @math{B_k} of input BFP vector @vector{B} and stores the results in output 
 * BFP vector @vector{A}.
 * 
 * `a` and `b` must have been initialized (see bfp_s32_init()), and must be the same length.
 * 
 * This operation can be performed safely in-place on `b`.
 * 
 * @bfp_op{32, @f$ 
 *      A_k \leftarrow \sqrt{B_k}                       \\
 *          \qquad\text{for } k \in 0\ ...\ (N-1)       \\
 *          \qquad\text{where } N \text{ is the length of } \bar{B}
 * @f$ }
 * 
 * @par Notes
 * 
 * * Only the `XS3_BFP_SQRT_DEPTH_S32` (see xs3_math_conf.h) most significant bits of each result are computed.
 * 
 * * This function only computes real roots. For any @math{B_k \lt 0}, the corresponding output @math{A_k} is set to 
 *   @math{0}.
 * 
 * @param[out] a     Output BFP vector @vector{A}
 * @param[in]  b     Input BFP vector @vector{B}
 */
void bfp_s32_sqrt(
    bfp_s32_t* a,
    const bfp_s32_t* b);


/** 
 * @brief Get the inverses of elements of a 16-bit BFP vector.
 * 
 * Computes the inverse of each element @math{B_k} of input BFP vector @vector{B} and stores the results in output 
 * BFP vector @vector{A}.
 * 
 * `a` and `b` must have been initialized (see bfp_s16_init()), and must be the same length.
 * 
 * This operation can be performed safely in-place on `b`.
 * 
 * @bfp_op{16, @f$ 
 *      A_k \leftarrow B_k^{-1}                     \\
 *          \qquad\text{for } k \in 0\ ...\ (N-1)   \\
 *          \qquad\text{where } N \text{ is the length of } \bar{B}
 * @f$ }
 * 
 * @param[out] a     Output BFP vector @vector{A}
 * @param[in]  b     Input BFP vector @vector{B}
 */
void bfp_s16_inverse(
    bfp_s16_t* a,
    const bfp_s16_t* b);


/** 
 * @brief Get the inverses of elements of a 32-bit BFP vector.
 * 
 * Computes the inverse of each element @math{B_k} of input BFP vector @vector{B} and stores the results in output 
 * BFP vector @vector{A}.
 * 
 * `a` and `b` must have been initialized (see bfp_s32_init()), and must be the same length.
 * 
 * This operation can be performed safely in-place on `b`.
 * 
 * @bfp_op{32, @f$ 
 *      A_k \leftarrow B_k^{-1}                     \\
 *          \qquad\text{for } k \in 0\ ...\ (N-1)   \\
 *          \qquad\text{where } N \text{ is the length of } \bar{B}
 * @f$ }
 * 
 * @param[out] a     Output BFP vector @vector{A}
 * @param[in]  b     Input BFP vector @vector{B}
 */
void bfp_s32_inverse(
    bfp_s32_t* a,
    const bfp_s32_t* b);

/** 
 * @brief Sum the absolute values of elements of a 16-bit BFP vector.
 * 
 * Sum the absolute values of elements of input BFP vector @vector{B} for a result @math{A = a \cdot 2^{a\_exp}}, where
 * @math{a} is a 32-bit mantissa and @math{a\_exp} is its associated exponent. @math{A} is returned.
 * 
 * `b` must have been initialized (see bfp_s16_init()).
 * 
 * @bfp_op{16, @f$
 *      A \leftarrow \sum_{k=0}^{N-1} \left| A_k \right|            \\
 *          \qquad\text{where } N \text{ is the length of } \bar{B}
 * @f$ }
 * 
 * @param[in] b         Input BFP vector @vector{B}
 * 
 * @returns  @math{A}, the sum of absolute values of elements of @vector{B}
 */
float_s32_t bfp_s16_abs_sum(
    const bfp_s16_t* b);

/** 
 * @brief Sum the absolute values of elements of a 32-bit BFP vector.
 * 
 * Sum the absolute values of elements of input BFP vector @vector{B} for a result @math{A = a \cdot 2^{a\_exp}}, where
 * @math{a} is a 64-bit mantissa and @math{a\_exp} is its associated exponent. @math{A} is returned.
 * 
 * `b` must have been initialized (see bfp_s32_init()).
 * 
 * @bfp_op{32, @f$
 *      A \leftarrow \sum_{k=0}^{N-1} \left| A_k \right|            \\
 *          \qquad\text{where } N \text{ is the length of } \bar{B}
 * @f$ }
 * 
 * @param[in] b         Input BFP vector @vector{B}
 * 
 * @returns  @math{A}, the sum of absolute values of elements of @vector{B}
 */
float_s64_t bfp_s32_abs_sum(
    const bfp_s32_t* b);


/** 
 * @brief Get the mean value of a 16-bit BFP vector.
 * 
 * Computes @math{A = a \cdot 2^{a\_exp}}, the mean value of elements of input BFP vector @vector{B}, where @math{a} is
 * the 16-bit mantissa of the result, and @math{a\_exp} is its associated exponent. @math{A} is returned.
 * 
 * `b` must have been initialized (see bfp_s16_init()).
 * 
 * @bfp_op{16, @f$
 *      A \leftarrow \frac{1}{N} \sum_{k=0}^{N-1} \left( B_k \right)     \\
 *          \qquad\text{where } N \text{ is the length of } \bar{B}
 * @f$ }
 * 
 * @param[in]  b        Input BFP vector @vector{B}
 * 
 * @returns  @math{A}, the mean value of @vector{B}'s elements
 */
float_s16_t bfp_s16_mean(
    const bfp_s16_t* b);


/** 
 * @brief Get the mean value of a 32-bit BFP vector.
 * 
 * Computes @math{A = a \cdot 2^{a\_exp}}, the mean value of elements of input BFP vector @vector{B}, where @math{a} is
 * the 32-bit mantissa of the result, and @math{a\_exp} is its associated exponent. @math{A} is returned.
 * 
 * `b` must have been initialized (see bfp_s32_init()).
 * 
 * @bfp_op{32, @f$
 *      A \leftarrow \frac{1}{N} \sum_{k=0}^{N-1} \left( B_k \right)     \\
 *          \qquad\text{where } N \text{ is the length of } \bar{B}
 * @f$ }
 * 
 * @param[in]  b        Input BFP vector @vector{B}
 * 
 * @returns  @math{A}, the mean value of @vector{B}'s elements
 */
float_s32_t bfp_s32_mean(
    const bfp_s32_t* b);


/** 
 * @brief Get the energy (sum of squared of elements) of a 16-bit BFP vector.
 * 
 * Computes @math{A = a \cdot 2^{a\_exp}}, the sum of squares of elements of input BFP vector @vector{B}, where @math{a} 
 * is the 64-bit mantissa of the result, and @math{a\_exp} is its associated exponent. @math{A} is returned.
 * 
 * `b` must have been initialized (see bfp_s16_init()).
 * 
 * @bfp_op{16, @f$
 *      A \leftarrow \sum_{k=0}^{N-1} \left( B_k^2 \right)   \\
 *          \qquad\text{where } N \text{ is the length of } \bar{B}
 * @f$ }
 * 
 * @param[in]  b        Input BFP vector @vector{B}
 * 
 * @returns  @math{A}, @vector{B}'s energy
 */
float_s64_t bfp_s16_energy(
    const bfp_s16_t* b);


/** 
 * @brief Get the energy (sum of squared of elements) of a 32-bit BFP vector.
 * 
 * Computes @math{A = a \cdot 2^{a\_exp}}, the sum of squares of elements of input BFP vector @vector{B}, where @math{a} 
 * is the 64-bit mantissa of the result, and @math{a\_exp} is its associated exponent. @math{A} is returned.
 * 
 * `b` must have been initialized (see bfp_s32_init()).
 * 
 * @bfp_op{32, @f$
 *      A \leftarrow \sum_{k=0}^{N-1} \left( B_k^2 \right)   \\
 *          \qquad\text{where } N \text{ is the length of } \bar{B}
 * @f$ }
 * 
 * @param[in]  b        Input BFP vector @vector{B}
 * 
 * @returns  @math{A}, @vector{B}'s energy
 */
float_s64_t bfp_s32_energy(
    const bfp_s32_t* b);


/** 
 * @brief Get the RMS value of elements of a 16-bit BFP vector.
 * 
 * Computes @math{A = a \cdot 2^{a\_exp}}, the RMS value of elements of input BFP vector @vector{B}, where @math{a} 
 * is the 32-bit mantissa of the result, and @math{a\_exp} is its associated exponent. @math{A} is returned.
 * 
 * The RMS (root-mean-square) value of a vector is the square root of the sum of the squares of the vector's elements.
 * 
 * `b` must have been initialized (see bfp_s16_init()).
 * 
 * @bfp_op{16, @f$
 *      A \leftarrow \sqrt{\frac{1}{N}\sum_{k=0}^{N-1} \left( B_k^2 \right) }  \\
 *          \qquad\text{where } N \text{ is the length of } \bar{B}
 * @f$ }
 * 
 * @param[in]  b        Input BFP vector @vector{B}
 * 
 * @returns  @math{A}, the RMS value of @vector{B}'s elements
 */
float_s32_t bfp_s16_rms(
    const bfp_s16_t* b);


/** 
 * @brief Get the RMS value of elements of a 32-bit BFP vector.
 * 
 * Computes @math{A = a \cdot 2^{a\_exp}}, the RMS value of elements of input BFP vector @vector{B}, where @math{a} 
 * is the 32-bit mantissa of the result, and @math{a\_exp} is its associated exponent. @math{A} is returned.
 * 
 * The RMS (root-mean-square) value of a vector is the square root of the sum of the squares of the vector's elements.
 * 
 * `b` must have been initialized (see bfp_s32_init()).
 * 
 * @bfp_op{32, @f$
 *      A \leftarrow \sqrt{\frac{1}{N}\sum_{k=0}^{N-1} \left( B_k^2 \right) }  \\
 *          \qquad\text{where } N \text{ is the length of } \bar{B}
 * @f$ }
 * 
 * @param[in]  b        Input BFP vector @vector{B}
 * 
 * @returns  @math{A}, the RMS value of @vector{B}'s elements
 */
float_s32_t bfp_s32_rms(
    const bfp_s32_t* b);


/** 
 * @brief Get the maximum value of a 16-bit BFP vector.
 * 
 * Finds @math{A}, the maximum value among elements of input BFP vector @vector{B}. @math{A} is returned by this 
 * function.
 * 
 * `b` must have been initialized (see bfp_s16_init()).
 * 
 * @bfp_op{16, @f$
 *      A \leftarrow max\left(B_0\, B_1\, ...\, B_{N-1} \right)     \\
 *          \qquad\text{where } N \text{ is the length of } \bar{B}
 * @f$ }
 * 
 * @param[in] b     Input vector
 * 
 * @returns     @math{A}, the value of @vector{B}'s maximum element
 */
float_s16_t bfp_s16_max(
    const bfp_s16_t* b);


/** 
 * @brief Get the maximum value of a 32-bit BFP vector.
 * 
 * Finds @math{A}, the maximum value among elements of input BFP vector @vector{B}. @math{A} is returned by this 
 * function.
 * 
 * `b` must have been initialized (see bfp_s32_init()).
 * 
 * @bfp_op{32, @f$
 *      A \leftarrow max\left(B_0\, B_1\, ...\, B_{N-1} \right)     \\
 *          \qquad\text{where } N \text{ is the length of } \bar{B}
 * @f$ }
 * 
 * @param[in] b     Input vector
 * 
 * @returns     @math{A}, the value of @vector{B}'s maximum element
 */
float_s32_t bfp_s32_max(
    const bfp_s32_t* b);


/** 
 * @brief Get the minimum value of a 16-bit BFP vector.
 * 
 * Finds @math{A}, the minimum value among elements of input BFP vector @vector{B}. @math{A} is returned by this 
 * function.
 * 
 * `b` must have been initialized (see bfp_s16_init()).
 * 
 * @bfp_op{16, @f$
 *      A \leftarrow min\left(B_0\, B_1\, ...\, B_{N-1} \right)     \\
 *          \qquad\text{where } N \text{ is the length of } \bar{B}
 * @f$ }
 * 
 * @param[in] b     Input vector
 * 
 * @returns     @math{A}, the value of @vector{B}'s minimum element
 */
float_s16_t bfp_s16_min(
    const bfp_s16_t* b);


/** 
 * @brief Get the minimum value of a 32-bit BFP vector.
 * 
 * Finds @math{A}, the minimum value among elements of input BFP vector @vector{B}. @math{A} is returned by this 
 * function.
 * 
 * `b` must have been initialized (see bfp_s32_init()).
 * 
 * @bfp_op{32, @f$
 *      A \leftarrow min\left(B_0\, B_1\, ...\, B_{N-1} \right)     \\
 *          \qquad\text{where } N \text{ is the length of } \bar{B}
 * @f$ }
 * 
 * @param[in] b     Input vector
 * 
 * @returns     @math{A}, the value of @vector{B}'s minimum element
 */
float_s32_t bfp_s32_min(
    const bfp_s32_t* b);


/** 
 * @brief Get the index of the maximum value of a 16-bit BFP vector.
 * 
 * Finds @math{a}, the index of the maximum value among the elements of input BFP vector @vector{B}. @math{a} is 
 * returned by this function.
 * 
 * If `i` is the value returned, then the maximum value in @vector{B} is `ldexp(b->data[i], b->exp)`.
 * 
 * @bfp_op{16, @f$
 *      a \leftarrow argmax_k\left(b_k\right)           \\
 *          \qquad\text{for } k \in 0\ ...\ (N-1)       \\
 *          \qquad\text{where } N \text{ is the length of } \bar{B}
 * @f$ }
 * 
 * @par Notes
 * 
 * * If there is a tie for maximum value, the lowest tying index is returned.
 * 
 * @param[in] b     Input vector
 * 
 * @returns     @math{a}, the index of the maximum value from @vector{B}
 */
unsigned bfp_s16_argmax(
    const bfp_s16_t* b);


/** 
 * @brief Get the index of the maximum value of a 32-bit BFP vector.
 * 
 * Finds @math{a}, the index of the maximum value among the elements of input BFP vector @vector{B}. @math{a} is 
 * returned by this function.
 * 
 * If `i` is the value returned, then the maximum value in @vector{B} is `ldexp(b->data[i], b->exp)`.
 * 
 * @bfp_op{32, @f$
 *      a \leftarrow argmax_k\left(b_k\right)           \\
 *          \qquad\text{for } k \in 0\ ...\ (N-1)       \\
 *          \qquad\text{where } N \text{ is the length of } \bar{B}
 * @f$ }
 * 
 * @par Notes
 * 
 * * If there is a tie for maximum value, the lowest tying index is returned.
 * 
 * @param[in] b     Input vector
 * 
 * @returns     @math{a}, the index of the maximum value from @vector{B}
 */
unsigned bfp_s32_argmax(
    const bfp_s32_t* b);


/** 
 * @brief Get the index of the minimum value of a 16-bit BFP vector.
 * 
 * Finds @math{a}, the index of the minimum value among the elements of input BFP vector @vector{B}. @math{a} is 
 * returned by this function.
 * 
 * If `i` is the value returned, then the minimum value in @vector{B} is `ldexp(b->data[i], b->exp)`.
 * 
 * @bfp_op{16, @f$
 *      a \leftarrow argmin_k\left(b_k\right)           \\
 *          \qquad\text{for } k \in 0\ ...\ (N-1)       \\
 *          \qquad\text{where } N \text{ is the length of } \bar{B}
 * @f$ }
 * 
 * @par Notes
 * 
 * * If there is a tie for minimum value, the lowest tying index is returned.
 * 
 * @param[in] b     Input vector
 * 
 * @returns     @math{a}, the index of the minimum value from @vector{B}
 */
unsigned bfp_s16_argmin(
    const bfp_s16_t* b);


/** 
 * @brief Get the index of the minimum value of a 32-bit BFP vector.
 * 
 * Finds @math{a}, the index of the minimum value among the elements of input BFP vector @vector{B}. @math{a} is 
 * returned by this function.
 * 
 * If `i` is the value returned, then the minimum value in @vector{B} is `ldexp(b->data[i], b->exp)`.
 * 
 * @bfp_op{32, @f$
 *      a \leftarrow argmin_k\left(b_k\right)           \\
 *          \qquad\text{for } k \in 0\ ...\ (N-1)       \\
 *          \qquad\text{where } N \text{ is the length of } \bar{B}
 * @f$ }
 * 
 * @par Notes
 * 
 * * If there is a tie for minimum value, the lowest tying index is returned.
 * 
 * @param[in] b     Input vector
 * 
 * @returns     @math{a}, the index of the minimum value from @vector{B}
 */
unsigned bfp_s32_argmin(
    const bfp_s32_t* b);


#ifdef __XC__
}   //extern "C"
#endif

#endif //BFP_H_