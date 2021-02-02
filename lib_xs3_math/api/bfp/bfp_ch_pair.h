// Copyright 2021 XMOS LIMITED. This Software is subject to the terms of the 
// XMOS Public License: Version 1

#ifndef BFP_CH_PAIR_H_
#define BFP_CH_PAIR_H_

#include "xs3_math_types.h"


#ifdef __XC__
extern "C" {
#endif



/** 
 * @brief Get the headroom of a 16-bit BFP channel-pair vector.
 * 
 * The headroom of a channel-pair vector is the number of bits that each channel of each of its elements can be 
 * left-shifted without losing any information. It conveys information about the range of values that vector may 
 * contain, which is useful for determining how best to preserve precision in potentially lossy block floating-point 
 * operations.
 * 
 * In a BFP context, headroom applies to mantissas only, not exponents.
 * 
 * In particular, if the 16-bit mantissa vector @vector{x} has @math{N} bits of headroom, then for any element 
 * @math{x_k} of @vector{x}
 * 
 * @math{-2^{15-N} \le x_k \lt 2^{15-N}}
 * 
 * And for any element @math{X_k = x_k \cdot 2^{x\_exp}} of a BFP vector @vector{X}
 * 
 * @math{-2^{15 + x\_exp - N} \le X_k \lt 2^{15 + x\_exp - N} }
 * 
 * This function determines the headroom of `b`, updates `b->hr` with that value, and then returns `b->hr`.
 *
 * @param   b         BFP channel-pair vector to get the headroom of
 * 
 * @returns    Headroom of BFP channel-pair vector `b` 
 */
headroom_t bfp_ch_pair_s16_headroom(
    bfp_ch_pair_s16_t* b);



/** 
 * @brief Get the headroom of a 32-bit BFP channel-pair vector.
 * 
 * The headroom of a channel-pair vector is the number of bits that each channel of each of its elements can be 
 * left-shifted without losing any information. It conveys information about the range of values that vector may 
 * contain, which is useful for determining how best to preserve precision in potentially lossy block floating-point 
 * operations.
 * 
 * In a BFP context, headroom applies to mantissas only, not exponents.
 * 
 * In particular, if the 32-bit mantissa vector @vector{x} has @math{N} bits of headroom, then for any element 
 * @math{x_k} of @vector{x}
 * 
 * @math{-2^{31-N} \le x_k \lt 2^{31-N}}
 * 
 * And for any element @math{X_k = x_k \cdot 2^{x\_exp}} of a BFP vector @vector{X}
 * 
 * @math{-2^{31 + x\_exp - N} \le X_k \lt 2^{31 + x\_exp - N} }
 * 
 * This function determines the headroom of `b`, updates `b->hr` with that value, and then returns `b->hr`.
 *
 * @param   b         BFP channel-pair vector to get the headroom of
 * 
 * @returns    Headroom of BFP channel-pair vector `b` 
 */
headroom_t bfp_ch_pair_s32_headroom(
    bfp_ch_pair_s32_t* b);


/** 
 * @brief Apply a left-shift to the mantissas of a 16-bit BFP channel-pair vector.
 * 
 * Each channel-pair mantissa of input BFP vector @vector{B} is left-shifted `b_shl` bits and stored in the 
 * corresponding element of output BFP vector @vector{A}.
 * 
 * This operation can be used to add or remove headroom from a BFP vector.
 * 
 * `b_shr` is the number of bits that the channel A and channel B values of each mantissa will be left-shifted. This 
 * shift is signed and arithmetic, so negative values for `b_shl` will right-shift the mantissas.
 * 
 * `a` and `b` must have been initialized (see bfp_ch_pair_s16_init()), and must be the same length.
 * 
 * This operation can be performed safely in-place on `b`.
 * 
 * Note that this operation bypasses the logic protecting the caller from saturation or underflows. Output values 
 * saturate to the symmetric 16-bit range (@math{-2^{15} \lt \lt 2^{15}}). To avoid saturation, `b_shl` should be no
 * greater than the headroom of `b` (`b->hr`).
 * 
 * @bfp_op{16, @f$
 *      ChA\\{a_k\\} \leftarrow sat_{16}( \lfloor ChA\\{b_k\\} \cdot 2^{b\_shl} \rfloor )     \\
 *      ChB\\{a_k\\} \leftarrow sat_{16}( \lfloor ChB\\{b_k\\} \cdot 2^{b\_shl} \rfloor )     \\
 *          \qquad\text{for } k \in 0\ ...\ (N-1)                           \\
 *          \qquad\text{where } N \text{ is the length of } \bar{B}         \\
 *          \qquad\text{  and } b_k \text{ and } a_k \text{ are the } k\text{th mantissas from } 
 *              \bar{B}\text{ and } \bar{A}\text{ respectively}
 * @f$ }
 * 
 * @param[out] a        Output BFP channel-pair vector @vector{A}
 * @param[in]  b        Input BFP channel-pair vector @vector{B}
 * @param[in]  b_shl    Signed arithmetic left-shift to be applied to mantissas of @vector{B}.
 */
void bfp_ch_pair_s16_shl(
    bfp_ch_pair_s16_t* a,
    const bfp_ch_pair_s16_t* b,
    const left_shift_t shl);


/** 
 * @brief Apply a left-shift to the mantissas of a 32-bit BFP channel-pair vector.
 * 
 * Each channel-pair mantissa of input BFP vector @vector{B} is left-shifted `b_shl` bits and stored in the 
 * corresponding element of output BFP vector @vector{A}.
 * 
 * This operation can be used to add or remove headroom from a BFP vector.
 * 
 * `b_shr` is the number of bits that the channel A and channel B values of each mantissa will be left-shifted. This 
 * shift is signed and arithmetic, so negative values for `b_shl` will right-shift the mantissas.
 * 
 * `a` and `b` must have been initialized (see bfp_ch_pair_s32_init()), and must be the same length.
 * 
 * This operation can be performed safely in-place on `b`.
 * 
 * Note that this operation bypasses the logic protecting the caller from saturation or underflows. Output values 
 * saturate to the symmetric 32-bit range (@math{-2^{31} \lt \lt 2^{31}}). To avoid saturation, `b_shl` should be no
 * greater than the headroom of `b` (`b->hr`).
 * 
 * @bfp_op{32, @f$
 *      ChA\\{a_k\\} \leftarrow sat_{32}( \lfloor ChA\\{b_k\\} \cdot 2^{b\_shl} \rfloor )     \\
 *      ChB\\{a_k\\} \leftarrow sat_{32}( \lfloor ChB\\{b_k\\} \cdot 2^{b\_shl} \rfloor )     \\
 *          \qquad\text{for } k \in 0\ ...\ (N-1)                           \\
 *          \qquad\text{where } N \text{ is the length of } \bar{B}         \\
 *          \qquad\text{  and } b_k \text{ and } a_k \text{ are the } k\text{th mantissas from } 
 *              \bar{B}\text{ and } \bar{A}\text{ respectively}
 * @f$ }
 * 
 * @param[out] a        Output BFP channel-pair vector @vector{A}
 * @param[in]  b        Input BFP channel-pair vector @vector{B}
 * @param[in]  b_shl    Signed arithmetic left-shift to be applied to mantissas of @vector{B}.
 */
void bfp_ch_pair_s32_shl(
    bfp_ch_pair_s32_t* a,
    const bfp_ch_pair_s32_t* b,
    const left_shift_t shl);

#ifdef __XC__
}   //extern "C"
#endif

#endif //BFP_CH_PAIR_H_