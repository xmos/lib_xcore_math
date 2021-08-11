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
 * @page page_xs3_vect_s16_h  xs3_vect_s16.h
 * 
 * This header contains functions implementing arithmetic operations on 16-bit vectors, optimized 
 * for xCore XS3.
 * 
 * @note This header is included automatically through `xs3_math.h` or `bfp_math.h`.
 * 
 * @ingroup xs3_math_header_file
 */

/**
 * Maximum bit-depth that can be calculated by xs3_vect_s16_sqrt().
 * 
 * @sa xs3_vect_s16_sqrt
 */
#define XS3_VECT_SQRT_S16_MAX_DEPTH     (15)


/** 
 * @brief Compute the element-wise absolute value of a 16-bit vector.
 * 
 * `a[]` and `b[]` represent the 16-bit vectors @vector{a} and @vector{b} respectively. Each must begin at a
 * word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * @operation{ 
 * &     a_k \leftarrow sat_{32}(\left| b_k \right|)         \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1)
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the output vector @vector{a} are
 * the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp}.
 * @endparblock
 * 
 * @param[out] a        Output vector @vector{a}
 * @param[in]  b        Input vector @vector{b}
 * @param[in]  length   Number of elements in vectors @vector{a} and @vector{b}
 * 
 * @returns     Headroom of the output vector @vector{a}.
 * 
 * @exception ET_LOAD_STORE Raised if `a` or `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup xs3_vect16_func
 */
C_API
headroom_t xs3_vect_s16_abs(
    int16_t a[],
    const int16_t b[],
    const unsigned length);


/**
 * @brief Compute the sum of the absolute values of elements of a 16-bit vector.
 * 
 * `b[]` represents the 16-bit vector @vector{b}. `b[]` must begin at a word-aligned address.
 * 
 * `length` is the number of elements in @vector{b}.
 * 
 * @operation{
 *      a \leftarrow \sum_{k=0}^{length-1} \left| b_k \right|
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the returned value @math{a} is 
 * the 32-bit mantissa of floating-point value @math{a \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp}.
 * @endparblock
 * 
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   length      Number of elements in @vector{b}
 * 
 * @returns The 32-bit sum @math{a}
 * 
 * @exception ET_LOAD_STORE Raised if `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup xs3_vect16_func
 */
C_API
int32_t xs3_vect_s16_abs_sum(
    const int16_t b[],
    const unsigned length);


/**
 * @brief Add one 16-bit BFP vector to another.
 * 
 * `a[]`, `b[]` and `c[]` represent the 16-bit vectors @vector{a}, @vector{b} and @vector{c} respectively. Each must 
 * begin at a word-aligned address. This operation can be performed safely in-place on `b[]` or `c[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b} and @vector{c} 
 * respectively.
 * 
 * @operation{ 
 * &     b_k' = sat_{16}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)  \\
 * &     c_k' = sat_{16}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)  \\
 * &     a_k \leftarrow sat_{16}\!\left( b_k' + c_k' \right)     \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} and @vector{c} are the mantissas of BFP vectors @math{ \bar{b} \cdot 2^{b\_exp} } and 
 * @math{\bar{c} \cdot 2^{c\_exp}}, then the resulting vector @vector{a} are the mantissas of BFP vector 
 * @math{\bar{a} \cdot 2^{a\_exp}}. 
 * 
 * In this case, @math{b\_shr} and @math{c\_shr} **must** be chosen so that 
 * @math{a\_exp = b\_exp + b\_shr = c\_exp + c\_shr}. Adding or subtracting mantissas only makes sense if they
 * are associated with the same exponent.
 * 
 * The function xs3_vect_s16_add_prepare() can be used to obtain values for @math{a\_exp}, @math{b\_shr} and 
 * @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and the input headrooms @math{b\_hr} and 
 * @math{c\_hr}.
 * @endparblock
 * 
 * @param[out]      a           Output vector @vector{a}
 * @param[in]       b           Input vector @vector{b}
 * @param[in]       c           Input vector @vector{c}
 * @param[in]       length      Number of elements in vectors @vector{a}, @vector{b} and @vector{c}
 * @param[in]       b_shr       Right-shift appled to @vector{b}
 * @param[in]       c_shr       Right-shift appled to @vector{c}
 * 
 * @returns     Headroom of the output vector @vector{a}.
 * 
 * @exception ET_LOAD_STORE Raised if `a`, `b` or `c` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @see xs3_vect_s16_add_prepare
 * 
 * @ingroup xs3_vect16_func
 */
C_API
headroom_t xs3_vect_s16_add(
    int16_t a[],
    const int16_t b[], 
    const int16_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Obtain the output exponent and shifts required for a call to `xs3_vect_s16_add()`.
 * 
 * The logic for computing the shifts and exponents of `xs3_vect_s16_add()` is identical to that for
 * `xs3_vect_s32_add()`.
 * 
 * This macro is provided as a convenience to developers and to make the code more readable.
 * 
 * @see xs3_vect_s32_add_prepare()
 * 
 * @ingroup xs3_vect16_prepare
 */
#define xs3_vect_s16_add_prepare xs3_vect_s32_add_prepare


/**
 * @brief Obtain the array index of the maximum element of a 16-bit vector.
 * 
 * `b[]` represents the 16-bit input vector @vector{b}. It must begin at a word-aligned address.
 * 
 * `length` is the number of elements in @vector{b}.
 * 
 * @operation{ 
 * &     a \leftarrow argmax_k\\{ b_k \\}     \\ 
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * }
 * 
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   length      Number of elemetns in @vector{b}
 * 
 * @returns @math{a}, the index of the maximum element of vector @vector{b}. If there is a tie for the maximum value, 
 *          the lowest tying index is returned.
 * 
 * @exception ET_LOAD_STORE Raised if `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup xs3_vect16_func
 */
C_API
unsigned xs3_vect_s16_argmax(
    const int16_t b[], 
    const unsigned length);


/**
 * @brief Obtain the array index of the minimum element of a 16-bit vector.
 * 
 * `b[]` represents the 16-bit input vector @vector{b}. It must begin at a word-aligned address.
 * 
 * `length` is the number of elements in @vector{b}.
 * 
 * @operation{ 
 * &     a \leftarrow argmin_k\\{ b_k \\}     \\ 
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * }
 * 
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   length      Number of elemetns in @vector{b}
 * 
 * @returns @math{a}, the index of the minimum element of vector @vector{b}. If there is a tie for the minimum value, 
 *          the lowest tying index is returned.
 * 
 * @exception ET_LOAD_STORE Raised if `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup xs3_vect16_func
 */
C_API
unsigned xs3_vect_s16_argmin(
    const int16_t b[], 
    const unsigned length);


/**
 * @brief Clamp the elements of a 16-bit vector to a specified range.
 * 
 * `a[]` and `b[]` represent the 16-bit vectors @vector{a} and @vector{b} respectively. Each must begin at a 
 * word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `lower_bound` and `upper_bound` are the lower and upper bounds of the clipping range respectively. These bounds are
 * checked for each element of @vector{b} only _after_ `b_shr` is applied.
 * 
 * `b_shr` is the signed arithmetic right-shift applied to elements of @vector{b} _before_ being compared to the upper
 * and lower bounds. 
 * 
 * If @vector{b} are the mantissas for a BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the exponent @math{a\_exp} of
 * the output BFP vector @math{\bar{a} \cdot 2^{a\_exp}} is given by @math{a\_exp = b\_exp + b\_shr}.
 * 
 * @operation{
 * &     b_k' \leftarrow sat_{16}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor) \\
 * &     a_k \leftarrow \begin\{cases\}
 *           lower\_bound & b_k' \le lower\_bound                        \\
 * &         upper\_bound & b_k' \ge upper\_bound                        \\
 * &         b_k' & otherwise \end\{cases\}                              \\
 * &     \qquad\text{ for }k\in 0\ ...\ (length-1)
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the output vector @vector{a} are
 * the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp + b\_shr}.
 * @endparblock
 * 
 * @param[out]  a               Output vector @vector{a}
 * @param[in]   b               Input vector @vector{b}
 * @param[in]   length          Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]   lower_bound     Lower bound of clipping range
 * @param[in]   upper_bound     Upper bound of clipping range
 * @param[in]   b_shr           Arithmetic right-shift applied to elements of @vector{b} prior to clipping
 * 
 * @returns  Headroom of output vector @vector{a}
 * 
 * @exception ET_LOAD_STORE Raised if `a` or `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup xs3_vect16_func
 */
C_API
headroom_t xs3_vect_s16_clip(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const int16_t lower_bound,
    const int16_t upper_bound,
    const right_shift_t b_shr);


/**
 * @brief Obtain the output exponent, input shift and modified bounds used by xs3_vect_s16_clip().
 * 
 * This function is used in conjunction with xs3_vect_s16_clip() to bound the elements of a 32-bit BFP vector to a
 * specified range.
 * 
 * This function computes `a_exp`, `b_shr`, `lower_bound` and `upper_bound`.
 * 
 * `a_exp` is the exponent associated with the 16-bit mantissa vector @vector{a} computed by xs3_vect_s32_clip().
 * 
 * `b_shr` is the shift parameter required by xs3_vect_s16_clip() to achieve the output exponent `a_exp`.
 * 
 * `lower_bound` and `upper_bound` are the 16-bit mantissas which indicate the lower and upper clipping bounds
 * respectively. The values are modified by this function, and the resulting values should be passed along to
 * xs3_vect_s16_clip().
 * 
 * `b_exp` is the exponent associated with the input mantissa vector @vector{b}.
 * 
 * `bound_exp` is the exponent associated with the bound mantissas `lower_bound` and `upper_bound` respectively.
 * 
 * `b_hr` is the headroom of @vector{b}. If unknown, it can be obtained using xs3_vect_s16_headroom(). Alternatively, 
 * the value `0` can always be safely used (but may result in reduced precision).
 * 
 * @param[out]    a_exp               Exponent associated with output mantissa vector @vector{a}
 * @param[out]    b_shr               Signed arithmetic right-shift for @vector{b} used by xs3_vect_s32_clip()
 * @param[inout]  lower_bound         Lower bound of clipping range
 * @param[inout]  upper_bound         Upper bound of clipping range
 * @param[in]     b_exp               Exponent associated with input mantissa vector @vector{b}
 * @param[in]     bound_exp           Exponent associated with clipping bounds `lower_bound` and `upper_bound`
 * @param[in]     b_hr                Headroom of input mantissa vector @vector{b}
 * 
 * @see xs3_vect_s16_clip
 * 
 * @ingroup xs3_vect16_prepare
 */
C_API
void xs3_vect_s16_clip_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    int16_t* lower_bound,
    int16_t* upper_bound,
    const exponent_t b_exp,
    const exponent_t bound_exp,
    const headroom_t b_hr);

/**
 * @brief Compute the inner product of two 16-bit vectors.
 * 
 * `b[]` and `c[]` represent the 32-bit vectors @vector{a} and @vector{b} respectively. Each must begin at a 
 * word-aligned address.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * @operation{
 *      a \leftarrow \sum_{k=0}^{length-1}\left( b_k \cdot c_k \right)
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} and @vector{c} are the mantissas of the BFP vectors @math{ \bar{b} \cdot 2^{b\_exp} } and 
 * @math{\bar{c}\cdot 2^{c\_exp}}, then result @math{a} is the mantissa of the result @math{a \cdot 2^{a\_exp}}, where
 * @math{a\_exp = b\_exp + c\_exp}.
 * 
 * If needed, the bit-depth of @math{a} can then be reduced to 16 or 32 bits to get a new result 
 * @math{a' \cdot 2^{a\_exp'}} where @math{a' = a \cdot 2^{-a\_shr}} and @math{a\_exp' = a\_exp + a\_shr}.
 * 
 * @todo I don't think there are currently any functions in this library to perform this bit-depth reduction in a 
 *       user-friendly way.
 * @endparblock
 * 
 * @par Notes
 * @parblock
 * 
 * The sum @math{a} is accumulated simultaneously into 16 48-bit accumulators which are summed together at the final
 * step. So long as `length` is less than roughly 2 million, no overflow or saturation of the resulting sum is possible. 
 * @endparblock
 * 
 * @param[in] b             Input vector @vector{b}
 * @param[in] c             Input vector @vector{c}
 * @param[in] length        Number of elements in vectors @vector{b} and @vector{c}
 * 
 * @returns @math{a}, the inner product of vectors @vector{b} and @vector{c}.
 * 
 * @exception ET_LOAD_STORE Raised if `b` or `c` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup xs3_vect16_func
 */
C_API
int64_t xs3_vect_s16_dot(
    const int16_t b[],
    const int16_t c[],
    const unsigned length);


/**
 * @brief Calculate the energy (sum of squares of elements) of a 16-bit vector.
 * 
 * `b[]` represents the 16-bit vector @vector{b}. `b[]` must begin at a word-aligned address.
 * 
 * `length` is the number of elements in @vector{b}.
 * 
 * `b_shr` is the signed arithmetic right-shift applied to elements of @vector{b}. `b_shr` should be chosen to avoid
 * the possibility of saturation. See the note below.
 * 
 * @operation{
 * &     b_k' \leftarrow sat_{16}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)         \\
 * &     a \leftarrow \sum_{k=0}^{length-1} (b_k')^2
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the mantissas of the BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then floating-point result is
 * @math{a \cdot 2^{a\_exp}}, where the 32-bit mantissa @math{a} is returned by this function, and 
 * @math{a\_exp = 2 \cdot (b\_exp + b\_shr) }.
 * @endparblock
 * 
 * @par Additional Details
 * @parblock
 * 
 * If @vector{b} has @math{b\_hr} bits of headroom, then each product @math{(b_k')^2} can be a maximum of 
 * @math{ 2^{30 - 2 \cdot (b\_hr + b\_shr)}}. So long as `length` is less than @math{1 + 2\cdot (b\_hr + b\_shr) }, 
 * such errors should not be possible. Each increase of @math{b\_shr} by @math{1} doubles the number of elements that
 * can be summed without risk of overflow.
 * 
 * If the caller's mantissa vector is longer than that, the full result can be found by calling this function multiple 
 * times for partial results on sub-sequences of the input, and adding the results in user code. 
 * 
 * In many situations the caller may have _a priori_ knowledge that saturation is impossible (or very nearly so), in 
 * which case this guideline may be disregarded. However, such situations are application-specific and are well beyond 
 * the scope of this documentation, and as such are left to the user's discretion.
 * @endparblock
 * 
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   length      Number of elements in @vector{b}
 * @param[in]   b_shr       Right-shift appled to @vector{b}
 * 
 * @returns 64-bit mantissa of vector @vector{b}'s energy
 * 
 * @exception ET_LOAD_STORE Raised if `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup xs3_vect16_func
 */
C_API
int32_t xs3_vect_s16_energy(
    const int16_t b[],
    const unsigned length,
    const right_shift_t b_shr);


/**
 * @brief Calculate the headroom of a 16-bit vector.
 * 
 * The headroom of an N-bit integer is the number of bits that the integer's value may be left-shifted
 * without any information being lost. Equivalently, it is one less than the number of leading sign bits.
 * 
 * The headroom of an `int16_t` array is the minimum of the headroom of each of its `int16_t` elements.
 * 
 * This function efficiently traverses the elements of `b[]` to determine its headroom.
 * 
 * `b[]` represents the 16-bit vector @vector{b}. `b[]` must begin at a word-aligned address.
 * 
 * `length` is the number of elements in `b[]`.
 * 
 * @operation{
 *      a \leftarrow min\!\\{ HR_{16}\left(x_0\right), HR_{16}\left(x_1\right), ..., 
 *              HR_{16}\left(x_{length-1}\right) \\}
 * }
 * 
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   length      The number of elements in vector @vector{b}
 * 
 * @returns     Headroom of vector @vector{b} 
 * 
 * @exception ET_LOAD_STORE Raised if `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @see xs3_vect_ch_pair_s16_headroom, 
 *      xs3_vect_ch_pair_s32_headroom, 
 *      xs3_vect_s32_headroom,  
 *      xs3_vect_complex_s16_headroom, 
 *      xs3_vect_complex_s32_headroom
 * 
 * @ingroup xs3_vect16_func
 */
C_API
headroom_t xs3_vect_s16_headroom(
    const int16_t b[], 
    const unsigned length);


/**
 * @brief Compute the inverse of elements of a 16-bit vector.
 * 
 * `a[]` and `b[]` represent the 16-bit mantissa vectors @vector{a} and @vector{b} respectively. This operation can be 
 * performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `scale` is a scaling parameter used to maximize the precision of the result.
 * 
 * @operation{
 * &     a_k \leftarrow \lfloor\frac{2^{scale}}{b_k}\rfloor      \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1)
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the resulting vector @vector{a}
 * are the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = scale - b\_exp}.
 * 
 * The function xs3_vect_s16_inverse_prepare() can be used to obtain values for @math{a\_exp} and @math{scale}.
 * @endparblock
 * 
 * @param[out]  a           Output vector @vector{a}
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   length      Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]   scale       Scale factor applied to dividend when computing inverse
 * 
 * @returns     Headroom of output vector @vector{a}
 * 
 * @see xs3_vect_s16_inverse_prepare
 * 
 * @ingroup xs3_vect16_func
 */
C_API
void xs3_vect_s16_inverse(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const unsigned scale);


/**
 * @brief Obtain the output exponent and scaling parameter used by xs3_vect_s16_inverse().
 * 
 * This function is used in conjunction with xs3_vect_s16_inverse() to compute the inverse of elements of a 16-bit
 * BFP vector.
 * 
 * This function computes `a_exp` and `scale`.
 * 
 * `a_exp` is the exponent associated with output mantissa vector @vector{a}, and must be chosen to avoid overflow in 
 * the smallest element of the input vector, which when inverted becomes the largest output element. To maximize 
 * precision, this function chooses `a_exp` to be the smallest exponent known to avoid saturation. The `a_exp` chosen
 * by this function is derived from the exponent and smallest element of the input vector.
 * 
 * `scale` is a scaling parameter used by xs3_vect_s16_inverse() to achieve the chosen output exponent.
 * 
 * `b[]` is the input mantissa vector @vector{b}.
 * 
 * `b_exp` is the exponent associated with the input mantissa vector @vector{b}.
 * 
 * `length` is the number of elements in @vector{b}.
 * 
 * @todo In lib_dsp, the inverse function has a floor, which prevents tiny values from completely dominating the output
 *       behavior. Perhaps I should include that?
 * 
 * @param[out]  a_exp       Exponent of output vector @vector{a}
 * @param[out]  scale       Scale factor to be applied when computing inverse
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   b_exp       Exponent of @vector{b}
 * @param[in]   length      Number of elements in vector @vector{b}
 * 
 * @see xs3_vect_s16_inverse
 * 
 * @ingroup xs3_vect16_prepare
 */
C_API
void xs3_vect_s16_inverse_prepare(
    exponent_t* a_exp,
    unsigned* scale,
    const int16_t b[],
    const exponent_t b_exp,
    const unsigned length);


/**
 * @brief Find the maximum value in a 16-bit vector.
 * 
 * `b[]` represents the 16-bit vector @vector{b}. It must begin at a word-aligned address.
 * 
 * `length` is the number of elements in @vector{b}.
 * 
 * @operation{ 
 *      max\\{ x_0, x_1, ..., x_{length-1} \\}
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the returned value @math{a} is 
 * the 16-bit mantissa of floating-point value @math{a \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp}.
 * @endparblock
 * 
 * @param[in]   b           Input vector @vector{b}     
 * @param[in]   length      Number of elements in @vector{b}
 * 
 * @returns     Maximum value from @vector{b}
 * 
 * @exception ET_LOAD_STORE Raised if `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup xs3_vect16_func
 */
C_API
int16_t xs3_vect_s16_max(
    const int16_t b[], 
    const unsigned length);


/**
 * @brief Find the minimum value in a 16-bit vector.
 * 
 * `b[]` represents the 16-bit vector @vector{b}. It must begin at a word-aligned address.
 * 
 * `length` is the number of elements in @vector{b}.
 * 
 * @operation{ 
 *      max\\{ x_0, x_1, ..., x_{length-1} \\}
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the returned value @math{a} is 
 * the 16-bit mantissa of floating-point value @math{a \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp}.
 * @endparblock
 * 
 * @param[in]   b           Input vector @vector{b}     
 * @param[in]   length      Number of elements in @vector{b}
 * 
 * @returns     Minimum value from @vector{b}
 * 
 * @exception ET_LOAD_STORE Raised if `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup xs3_vect16_func
 */
C_API
int16_t xs3_vect_s16_min(
    const int16_t b[], 
    const unsigned length);


/**
 * @brief Multiply one 16-bit vector element-wise by another, and add the result to an accumulator.
 *
 * `acc[]` represents the 16-bit accumulator mantissa vector @vector{a}. Each @math{a_k} is
 * `acc[k]`.
 *
 * `b[]` and `c[]` represent the 16-bit input mantissa vectors @vector{b} and @vector{c}, where each
 * @math{b_k} is `b[k]` and each @math{c_k} is `c[k]`.
 *
 * Each of the input vectors must begin at a word-aligned address.
 *
 * `length` is the number of elements in each of the vectors.
 *
 * `acc_shr` is the signed arithmetic right-shift applied to the accumulators @math{a_k} prior to
 * accumulation.
 *
 * `bc_sat` is the unsigned arithmetic right-shift applied to the product of @math{b_k} and
 * @math{c_k} before accumulation.
 *
 * @operation{
 * &     v_k \leftarrow round( sat_{16}( b_k \cdot c_k \cdot 2^{-bc\_sat} ) )   \\
 * &     \hat{a}_k \leftarrow sat_{16}( a_k \cdot 2^{-acc\_shr} )               \\
 * &     a_k \leftarrow sat_{16}( \hat{a}_k + v_k )                             \\
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
 * @math{  a\_exp + acc\_shr = b\_exp + c\_exp + bc\_sat }.
 *
 * The function xs3_vect_complex_s16_macc_prepare() can be used to obtain values for @math{a\_exp},
 * @math{acc\_shr} and @math{bc\_sat} based on the input exponents @math{a\_exp}, @math{b\_exp} and
 * @math{c\_exp} and the input headrooms @math{a\_hr}, @math{b\_hr} and @math{c\_hr}.
 * @endparblock
 *
 * @param[inout]  acc       Accumulator @vector{a}
 * @param[in]     b         Input vector @vector{b}
 * @param[in]     c         Input vector @vector{c}
 * @param[in]     length    Number of elements in vectors @vector{a}, @vector{b} and @vector{c}
 * @param[in]     acc_shr   Signed arithmetic right-shift applied to accumulator elements.
 * @param[in]     bc_sat    Unsigned arithmetic right-shift applied to the products of elements
 *                          @math{b_k} and @math{c_k}
 *
 * @returns   Headroom of the output vector @vector{a}
 *
 * @exception ET_LOAD_STORE Raised if `acc`, `b` or `c` is not word-aligned (See @ref note_vector_alignment)
 *
 * @see xs3_vect_s16_macc_prepare
 *
 * @ingroup xs3_vect16_func
 */
C_API
headroom_t xs3_vect_s16_macc(
    int16_t acc[],
    const int16_t b[],
    const int16_t c[],
    const unsigned length,
    const right_shift_t acc_shr,
    const right_shift_t bc_sat);


/**
 * @brief Multiply one 16-bit vector element-wise by another, and subtract the result from an
 * accumulator.
 *
 * `acc[]` represents the 16-bit accumulator mantissa vector @vector{a}. Each @math{a_k} is
 * `acc[k]`.
 *
 * `b[]` and `c[]` represent the 16-bit input mantissa vectors @vector{b} and @vector{c}, where each
 * @math{b_k} is `b[k]` and each @math{c_k} is `c[k]`.
 *
 * Each of the input vectors must begin at a word-aligned address.
 *
 * `length` is the number of elements in each of the vectors.
 *
 * `acc_shr` is the signed arithmetic right-shift applied to the accumulators @math{a_k} prior to
 * accumulation.
 *
 * `bc_sat` is the unsigned arithmetic right-shift applied to the product of @math{b_k} and
 * @math{c_k} before accumulation.
 *
 * @operation{
 * &     v_k \leftarrow round( sat_{16}( b_k \cdot c_k \cdot 2^{-bc\_sat} ) )   \\
 * &     \hat{a}_k \leftarrow sat_{16}( a_k \cdot 2^{-acc\_shr} )               \\
 * &     a_k \leftarrow sat_{16}( \hat{a}_k - v_k )                             \\
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
 * @math{  a\_exp + acc\_shr = b\_exp + c\_exp + bc\_sat }.
 *
 * The function xs3_vect_complex_s16_nmacc_prepare() can be used to obtain values for @math{a\_exp},
 * @math{acc\_shr} and @math{bc\_sat} based on the input exponents @math{a\_exp}, @math{b\_exp} and
 * @math{c\_exp} and the input headrooms @math{a\_hr}, @math{b\_hr} and @math{c\_hr}.
 * @endparblock
 *
 * @param[inout]  acc       Accumulator @vector{a}
 * @param[in]     b         Input vector @vector{b}
 * @param[in]     c         Input vector @vector{c}
 * @param[in]     length    Number of elements in vectors @vector{a}, @vector{b} and @vector{c}
 * @param[in]     acc_shr   Signed arithmetic right-shift applied to accumulator elements.
 * @param[in]     bc_sat    Unsigned arithmetic right-shift applied to the products of elements
 *                          @math{b_k} and @math{c_k}
 *
 * @returns   Headroom of the output vector @vector{a}
 *
 * @exception ET_LOAD_STORE Raised if `acc`, `b` or `c` is not word-aligned (See @ref note_vector_alignment)
 *
 * @see xs3_vect_s16_nmacc_prepare
 *
 * @ingroup xs3_vect16_func
 */
C_API
headroom_t xs3_vect_s16_nmacc(
    int16_t acc[],
    const int16_t b[],
    const int16_t c[],
    const unsigned length,
    const right_shift_t acc_shr,
    const right_shift_t bc_sat);


/**
 * @brief Obtain the output exponent and shifts needed by xs3_vect_s16_macc().
 *
 * This function is used in conjunction with xs3_vect_s16_macc() to perform an element-wise
 * multiply-accumlate of 16-bit BFP vectors.
 *
 * This function computes `new_acc_exp` and `acc_shr` and `bc_sat`, which are selected to maximize
 * precision in the resulting accumulator vector without causing saturation of final or intermediate
 * values. Normally the caller will pass these outputs to their corresponding inputs of
 * xs3_vect_s16_macc().
 *
 * `acc_exp` is the exponent associated with the accumulator mantissa vector @vector{a} prior to the
 * operation, whereas `new_acc_exp` is the exponent corresponding to the updated accumulator vector.
 *
 * `b_exp` and `c_exp` are the exponents associated with the complex input mantissa vectors
 * @vector{b} and @vector{c} respectively.
 *
 * `acc_hr`, `b_hr` and `c_hr` are the headrooms of @vector{a}, @vector{b} and @vector{c}
 * respectively. If the headroom of any of these vectors is unknown, it can be obtained by calling
 * xs3_vect_s16_headroom(). Alternatively, the value `0` can always be safely used (but may result
 * in reduced precision).
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
 *      right_shift_t acc_shr, bc_sat;
 *      xs3_vect_s16_macc_prepare(&acc_exp, &acc_shr, &bc_sat, 
 *                                acc_exp, b_exp, c_exp,
 *                                acc_hr, b_hr, c_hr);
 * 
 *      // Modify results
 *      right_shift_t mant_shr = desired_exp - acc_exp;
 *      acc_exp += mant_shr;
 *      acc_shr += mant_shr;
 *      bc_sat  += mant_shr;
 *      
 *      // acc_shr and bc_sat may now be used in a call to xs3_vect_s16_macc() 
 * @endcode
 *
 * When applying the above adjustment, the following conditions should be maintained:
 * * `bc_sat >= 0`  (`bc_sat` is an _unsigned_ right-shift)
 * * `acc_shr > -acc_hr`  (Shifting any further left may cause saturation)
 *
 * It is up to the user to ensure any such modification does not result in saturation or
 * unacceptable loss of precision.
 * @endparblock
 *
 *
 * @param[out]  new_acc_exp   Exponent associated with output mantissa vector @vector{a} (after macc)
 * @param[out]  acc_shr       Signed arithmetic right-shift used for @vector{a} in xs3_vect_s16_macc()
 * @param[out]  bc_sat        Unsigned arithmetic right-shift applied to the product of elements
 *                            @math{b_k} and @math{c_k} in xs3_vect_s16_macc()
 * @param[in]   acc_exp       Exponent associated with input mantissa vector @vector{a} (before macc)
 * @param[in]   b_exp         Exponent associated with input mantissa vector @vector{b}
 * @param[in]   c_exp         Exponent associated with input mantissa vector @vector{c}
 * @param[in]   acc_hr        Headroom of input mantissa vector @vector{a} (before macc)
 * @param[in]   b_hr          Headroom of input mantissa vector @vector{b}
 * @param[in]   c_hr          Headroom of input mantissa vector @vector{c}
 *
 * @see xs3_vect_s16_macc
 *
 * @ingroup xs3_vect16_prepare
 */
C_API
void xs3_vect_s16_macc_prepare(
    exponent_t* new_acc_exp,
    right_shift_t* acc_shr,
    right_shift_t* bc_sat,
    const exponent_t acc_exp,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t acc_hr,
    const headroom_t b_hr,
    const headroom_t c_hr);


/**
 * @brief Obtain the output exponent and shifts required for a call to xs3_vect_s16_nmacc().
 *
 * The logic for computing the shifts and exponents of `xs3_vect_s16_nmacc()` is identical
 * to that for `xs3_vect_s16_macc_prepare()`.
 *
 * This macro is provided as a convenience to developers and to make the code more readable.
 *
 * @see xs3_vect_s16_macc_prepare(), xs3_vect_s16_nmacc()
 *
 * @ingroup xs3_vect16_prepare
 */
#define xs3_vect_s16_nmacc_prepare  xs3_vect_s16_macc_prepare

/**
 * @brief Multiply two 16-bit vectors together element-wise.
 * 
 * `a[]`, `b[]` and `c[]` represent the 16-bit vectors @vector{a}, @vector{b} and @vector{c} respectively. 
 * Each must begin at a word-aligned address. This operation can be performed safely in-place on `b[]` or `c[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `a_shr` is an unsigned arithmetic right-shift applied to the 32-bit accumulators holding the penultimate results.
 * 
 * @operation{
 * &     a_k' \leftarrow b_k \cdot c_k                               \\
 * &     a_k \leftarrow sat_{16}(round(a_k' \cdot 2^{-a\_shr}))      \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1)
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} and @vector{c} are the mantissas of BFP vectors @math{ \bar{b} \cdot 2^{b\_exp} } and 
 * @math{\bar{c} \cdot 2^{c\_exp}}, then the resulting vector @vector{a} are the mantissas of BFP vector 
 * @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp + c\_exp + a\_shr}.
 * 
 * The function xs3_vect_s16_mul_prepare() can be used to obtain values for @math{a\_exp} and @math{a\_shr} based on the
 * input exponents @math{b\_exp} and @math{c\_exp} and the input headrooms @math{b\_hr} and @math{c\_hr}. 
 * @endparblock
 * 
 * @param[out] a        Output vector @vector{a}
 * @param[in]  b        Input vector @vector{b}
 * @param[in]  c        Input vector @vector{c}
 * @param[in]  length   Number of elements in vectors @vector{a}, @vector{b} and @vector{c}
 * @param[in]  a_shr    Right-shift appled to 32-bit products
 * 
 * @returns  Headroom of vector @vector{a}
 * 
 * @exception ET_LOAD_STORE Raised if `a`, `b` or `c` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup xs3_vect16_func
 */
//! [xs3_vect_s16_mul]
C_API
headroom_t xs3_vect_s16_mul(
    int16_t a[],
    const int16_t b[],
    const int16_t c[],
    const unsigned length,
    const right_shift_t a_shr);
//! [xs3_vect_s16_mul]


/**
 * @brief Obtain the output exponent and output shift used by xs3_vect_s16_mul().
 * 
 * This function is used in conjunction with xs3_vect_s16_mul() to perform an element-wise multiplication of two 16-bit
 * BFP vectors.
 * 
 * This function computes `a_exp` and `a_shr`.
 * 
 * `a_exp` is the exponent associated with mantissa vector @vector{a}, and must be chosen to be large enough to avoid 
 * overflow when elements of @vector{a} are computed. To maximize precision, this function chooses `a_exp` to be the 
 * smallest exponent known to avoid saturation (see exception below). The `a_exp` chosen by this function is derived 
 * from the exponents and headrooms of associated with the input vectors.
 * 
 * `a_shr` is an arithmetic right-shift applied by xs3_vect_complex_s16_mul() to the 32-bit products of input elements 
 * to achieve the chosen output exponent `a_exp`.
 * 
 * `b_exp` and `c_exp` are the exponents associated with the input mantissa vectors @vector{b} and @vector{c} 
 * respectively.
 * 
 * `b_hr` and `c_hr` are the headroom of @vector{b} and @vector{c} respectively. If the headroom of @vector{b} or 
 * @vector{c} is unknown, they can be obtained by calling xs3_vect_s16_headroom(). Alternatively, the value `0` can 
 * always be safely used (but may result in reduced precision).
 * 
 * @par Adjusting Output Exponents
 * @parblock
 * 
 * If a specific output exponent `desired_exp` is needed for the result (e.g. for emulating fixed-point arithmetic), the 
 * `a_shr` produced by this function can be adjusted according to the following:
 * \code{.c}
 *      exponent_t a_exp;
 *      right_shift_t a_shr;
 *      xs3_vect_s16_mul_prepare(&a_exp, &a_shr, b_exp, c_exp, b_hr, c_hr);
 *      exponent_t desired_exp = ...; // Value known a priori
 *      a_shr = a_shr + (desired_exp - a_exp);
 *      a_exp = desired_exp;
 * \endcode
 * 
 * When applying the above adjustment, the following conditions should be maintained:
 * * `a_shr >= 0`
 * 
 * Be aware that using a smaller value than strictly necessary for `a_shr` can result in saturation, and using larger 
 * values may result in unnecessary underflows or loss of precision.
 * @endparblock
 * 
 * @par Notes
 * @parblock
 * 
 * * Using the outputs of this function, an output mantissa which would otherwise be `INT16_MIN` will instead saturate 
 *   to `-INT16_MAX`. This is due to the symmetric saturation logic employed by the VPU and is a hardware feature. This 
 *   is a corner case which is usually unlikely and results in 1 LSb of error when it occurs.
 * @endparblock
 * 
 * @param[out]  a_exp       Exponent of output elements of xs3_vect_s16_mul()
 * @param[out]  a_shr       Right-shift supplied to xs3_vect_s16_mul()
 * @param[in]   b_exp       Exponent associated with @vector{b}
 * @param[in]   c_exp       Exponent associated with @vector{c}
 * @param[in]   b_hr        Headroom of @vector{b}
 * @param[in]   c_hr        Headroom of @vector{c}
 * 
 * @see xs3_vect_s16_mul
 * 
 * @ingroup xs3_vect16_prepare
 */
C_API
void xs3_vect_s16_mul_prepare(
    exponent_t* a_exp,
    right_shift_t* a_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr);


/** 
 * @brief Rectify the elements of a 16-bit vector.
 * 
 * Rectification ensures that all outputs are non-negative, changing negative values to 0.
 * 
 * `a[]` and `b[]` represent the 16-bit vectors @vector{a} and @vector{b} respectively. Each must begin at a
 * word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * 
 * Each output element `a[k]` is set to the value of the corresponding input element 
 * `b[k]` if it is positive, and `a[k]` is set to zero otherwise. 
 * 
 * @operation{
 * &     a_k \leftarrow \begin{cases}
 *          b_k & b_k \gt 0             \\ 
 * &         0 & b_k \leq 0\end{cases}   \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1)
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the output vector @vector{a} are
 * the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp}.
 * @endparblock
 * 
 * @param[out] a        Output vector @vector{a}
 * @param[in]  b        Input vector @vector{b}
 * @param[in]  length   Number of elements in vectors @vector{a} and @vector{b}
 * 
 * @returns     Headroom of the output vector @vector{a}.
 * 
 * @exception ET_LOAD_STORE Raised if `a` or `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup xs3_vect16_func
 */
C_API
headroom_t xs3_vect_s16_rect(
    int16_t a[],
    const int16_t b[],
    const unsigned length);


/**
 * @brief Multiply a 16-bit vector by a 16-bit scalar.
 * 
 * `a[]` and `b[]` represent the 16-bit vectors @vector{a} and @vector{b} respectively. Each must begin at a
 *  word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `c` is the 16-bit scalar @math{c} by which elements of @vector{b} are multiplied.
 * 
 * `a_shr` is an unsigned arithmetic right-shift applied to the 32-bit accumulators holding the penultimate results.
 * 
 * @operation{
 * &     a_k' \leftarrow b_k \cdot c                                 \\
 * &     a_k \leftarrow sat_{16}(round(a_k' \cdot 2^{-a\_shr}))      \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1)
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} } and @math{c} is the mantissa of
 * floating-point value @math{c \cdot 2^{c\_exp}}, then the resulting vector @vector{a} are the mantissas of BFP vector 
 * @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp + c\_exp + a\_shr}.
 * 
 * The function xs3_vect_s16_scale_prepare() can be used to obtain values for @math{a\_exp} and @math{a\_shr} based on 
 * the input exponents @math{b\_exp} and @math{c\_exp} and the input headrooms @math{b\_hr} and @math{c\_hr}. 
 * @endparblock
 * 
 * @param[out] a        Output vector @vector{a}
 * @param[in]  b        Input vector @vector{b}
 * @param[in]  c        Input vector @vector{c}
 * @param[in]  length   Number of elements in vectors @vector{a}, @vector{b} and @vector{c}
 * @param[in]  a_shr    Right-shift appled to 32-bit products
 * 
 * @returns  Headroom of vector @vector{a}
 * 
 * @exception ET_LOAD_STORE Raised if `a` or `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup xs3_vect16_func
 */
C_API
headroom_t xs3_vect_s16_scale(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const int16_t c,
    const right_shift_t a_shr);


/**
 * @brief Obtain the output exponent and output shift used by xs3_vect_s16_scale().
 * 
 * This function is used in conjunction with xs3_vect_s16_scale() to perform multiplication of a 16-bit BFP vector 
 * @math{\bar{b} \cdot 2^{b\_exp}} by a 16-bit scalar @math{c \cdot 2^{c\_exp}}. The result is another 16-bit BFP vector
 * @math{\bar{a} \cdot 2^{a\_exp}}.
 * 
 * This function computes `a_exp` and `a_shr`.
 * 
 * `a_exp` is the exponent associated with mantissa vector @vector{a}, and must be chosen to be large enough to avoid 
 * overflow when elements of @vector{a} are computed. To maximize precision, this function chooses `a_exp` to be the 
 * smallest exponent known to avoid saturation (see exception below). The `a_exp` chosen by this function is derived 
 * from the exponents and headrooms of associated with the inputs.
 * 
 * `a_shr` is an arithmetic right-shift applied by xs3_vect_complex_s16_scale() to the 32-bit products of input elements 
 * to achieve the chosen output exponent `a_exp`.
 * 
 * `b_exp` and `c_exp` are the exponents associated with @vector{b} and @math{c} 
 * respectively.
 * 
 * `b_hr` and `c_hr` are the headroom of @vector{b} and @math{c} respectively. If the headroom of @vector{b} or 
 * @math{c} are unknown, they can be obtained by calling xs3_vect_s16_headroom(). Alternatively, the value `0` can 
 * always be safely used (but may result in reduced precision).
 * 
 * @par Adjusting Output Exponents
 * @parblock
 * 
 * If a specific output exponent `desired_exp` is needed for the result (e.g. for emulating fixed-point arithmetic), the 
 * `a_shr` produced by this function can be adjusted according to the following:
 * \code{.c}
 *      exponent_t a_exp;
 *      right_shift_t a_shr;
 *      xs3_vect_s16_scale_prepare(&a_exp, &a_shr, b_exp, c_exp, b_hr, c_hr);
 *      exponent_t desired_exp = ...; // Value known a priori
 *      a_shr = a_shr + (desired_exp - a_exp);
 *      a_exp = desired_exp;
 * \endcode
 * 
 * When applying the above adjustment, the following conditions should be maintained:
 * * `a_shr >= 0`
 * 
 * Be aware that using a smaller value than strictly necessary for `a_shr` can result in saturation, and using larger 
 * values may result in unnecessary underflows or loss of precision.
 * @endparblock
 * 
 * @par Notes
 * @parblock
 * 
 * * Using the outputs of this function, an output mantissa which would otherwise be `INT16_MIN` will instead saturate 
 *   to `-INT16_MAX`. This is due to the symmetric saturation logic employed by the VPU and is a hardware feature. This 
 *   is a corner case which is usually unlikely and results in 1 LSb of error when it occurs.
 * @endparblock
 * 
 * @param[out]  a_exp       Exponent of output elements of xs3_vect_s16_scale()
 * @param[out]  a_shr       Right-shift supplied to xs3_vect_s16_scale()
 * @param[in]   b_exp       Exponent associated with @vector{b}
 * @param[in]   c_exp       Exponent associated with @vector{c}
 * @param[in]   b_hr        Headroom of @vector{b}
 * @param[in]   c_hr        Headroom of @vector{c}
 * 
 * @see xs3_vect_s16_scale
 * 
 * @ingroup xs3_vect16_prepare
 */
C_API
void xs3_vect_s16_scale_prepare(
    exponent_t* a_exp,
    right_shift_t* a_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr);

/** 
 * @brief Set all elements of a 16-bit vector to the specified value.
 * 
 * `a[]` represents the 16-bit vector @vector{a}. It must begin at a word-aligned address.
 * 
 * `b` is the value elements of @vector{a} are set to.
 * 
 * `length` is the number of elements in `a[]`.
 * 
 * @operation{
 * &     a_k \leftarrow b                                    \\
 * &         \qquad\text{for }k\in 0\ ...\ (length-1)
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @math{b} is the mantissa of floating-point value @math{b \cdot 2^{b\_exp}}, then the output vector @vector{a} are
 * the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp}.
 * @endparblock
 * 
 * @param[out] a        Output vector @vector{a}
 * @param[in]  b        Input value @math{b}
 * @param[in]  length   Number of elements in vector @vector{a}
 * 
 * @exception ET_LOAD_STORE Raised if `a` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup xs3_vect16_func
 */
C_API
void xs3_vect_s16_set(
    int16_t a[],
    const int16_t b,
    const unsigned length);


/**
 * @brief Left-shift the elements of a 16-bit vector by a specified number of bits.
 * 
 * `a[]` and `b[]` represent the 16-bit vectors @vector{a} and @vector{b} respectively. Each must begin at a 
 * word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in vectors @vector{a} and @vector{b}.
 * 
 * `b_shl` is the signed arithmetic left-shift applied to each element of @vector{b}. 
 * 
 * @operation{ 
 * &     a_k \leftarrow sat_{16}(\lfloor b_k \cdot 2^{b\_shl} \rfloor)       \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} }, then the resulting vector 
 * @vector{a} are the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where 
 * @math{\bar{a} = \bar{b} \cdot 2^{b\_shl}} and @math{a\_exp = b\_exp}.
 * @endparblock
 * 
 * @param[out]  a           Output vector @vector{a}
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   length      Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]   b_shl       Arithmetic left-shift applied to elements of @vector{b}
 * 
 * @returns     Headroom of output vector @vector{a}
 * 
 * @exception ET_LOAD_STORE Raised if `a` or `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup xs3_vect16_func
 */
C_API
headroom_t xs3_vect_s16_shl(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const left_shift_t b_shl);


/**
 * @brief Right-shift the elements of a 16-bit vector by a specified number of bits.
 * 
 * `a[]` and `b[]` represent the 16-bit vectors @vector{a} and @vector{b} respectively. Each must begin at a 
 * word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in vectors @vector{a} and @vector{b}.
 * 
 * `b_shr` is the signed arithmetic right-shift applied to each element of @vector{b}. 
 * 
 * @operation{ 
 * &     a_k \leftarrow sat_{16}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)      \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} }, then the resulting vector 
 * @vector{a} are the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where 
 * @math{\bar{a} = \bar{b} \cdot 2^{-b\_shr}} and @math{a\_exp = b\_exp}.
 * @endparblock
 * 
 * @param[out]  a           Output vector @vector{a}
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   length      Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]   b_shr       Arithmetic right-shift applied to elements of @vector{b}
 * 
 * @returns     Headroom of output vector @vector{a}
 * 
 * @exception ET_LOAD_STORE Raised if `a` or `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup xs3_vect16_func
 */
C_API
headroom_t xs3_vect_s16_shr(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const right_shift_t b_shr);


/**
 * @brief Compute the square roots of elements of a 16-bit vector.
 * 
 * `a[]` and `b[]` represent the 16-bit vectors @vector{a} and @vector{b} respectively. Each vector must begin at a 
 * word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` is the signed arithmetic right-shift applied to elements of @vector{b}.
 * 
 * `depth` is the number of most significant bits to calculate of each @math{a_k}. For example, a `depth` value of 8 
 * will only compute the 8 most significant byte of the result, with the remaining byte as 0. The maximum value for this 
 * parameter is `XS3_VECT_SQRT_S16_MAX_DEPTH` (31). The time cost of this operation is approximately proportional to the 
 * number of bits computed.
 * 
 * @operation{ 
 * &     b_k' \leftarrow sat_{16}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
 *       a_k \leftarrow \begin{cases}
 * &         \sqrt{ b_k' } & b_k' >= 0                                       \\
 * &         0   &   otherwise\end{cases}                                    \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1)                       \\
 * &         \qquad\text{ where } \sqrt{\cdot} \text{ computes the most significant } depth 
 *                       \text{ bits of the square root.}
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the resulting vector @vector{a}
 * are the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = (b\_exp + b\_shr - 14)/2}.
 * 
 * Note that because exponents must be integers, that means @math{b\_exp + b\_shr} **must be even**.
 * 
 * The function xs3_vect_s16_sqrt_prepare() can be used to obtain values for @math{a\_exp} and @math{b\_shr} based on 
 * the input exponent @math{b\_exp} and headroom @math{b\_hr}.
 * @endparblock
 * 
 * @par Notes
 * @parblock
 * * This function assumes roots are real. Negative input elements will result in corresponding outputs of 0.
 * @endparblock
 * 
 * @param[out]  a           Output vector @vector{a}
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   length      Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]   b_shr       Right-shift appled to @vector{b}
 * @param[in]   depth       Number of bits of each output value to compute
 * 
 * @returns     Headroom of output vector @vector{a}
 * 
 * @exception ET_LOAD_STORE Raised if `a` or `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup xs3_vect16_func
 */
C_API
headroom_t xs3_vect_s16_sqrt(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const right_shift_t b_shr,
    const unsigned depth);


/**
 * @brief Obtain the output exponent and shift parameter used by xs3_vect_s16_sqrt().
 * 
 * This function is used in conjunction withx xs3_vect_s16_sqrt() to compute the square root of elements of a 16-bit
 * BFP vector.
 * 
 * This function computes `a_exp` and `b_shr`.
 * 
 * `a_exp` is the exponent associated with output mantissa vector @vector{a}, and should be chosen to maximize the 
 * precision of the results. To that end, this function chooses `a_exp` to be the smallest exponent known to avoid 
 * saturation of the resulting mantissa vector @vector{a}. It is derived from the exponent and headroom of the input
 * BFP vector.
 * 
 * `b_shr` is the shift parameter required by xs3_vect_s16_sqrt() to achieve the chosen output exponent `a_exp`.
 * 
 * `b_exp` is the exponent associated with the input mantissa vector @vector{b}.
 * 
 * `b_hr` is the headroom of @vector{b}. If it is unknown, it can be obtained using xs3_vect_s16_headroom(). 
 * Alternatively, the value `0` can always be safely used (but may result in reduced precision).
 * 
 * @par Adjusting Output Exponents
 * @parblock
 * 
 * If a specific output exponent `desired_exp` is needed for the result (e.g. for emulating fixed-point arithmetic), the 
 * `b_shr` produced by this function can be adjusted according to the following:
 * \code{.c}
 *      exponent_t a_exp;
 *      right_shift_t b_shr;
 *      xs3_vect_s16_mul_prepare(&a_exp, &b_shr, b_exp, c_exp, b_hr, c_hr);
 *      exponent_t desired_exp = ...; // Value known a priori
 *      b_shr = b_shr + (desired_exp - a_exp);
 *      a_exp = desired_exp;
 * \endcode
 * 
 * When applying the above adjustment, the following condition should be maintained:
 * * `b_hr + b_shr >= 0`
 * 
 * Be aware that using smaller values than strictly necessary for `b_shr` can result in saturation, and using larger 
 * values may result in unnecessary underflows or loss of precision.
 * 
 * Also, if a larger exponent is used than necessary, a larger `depth` parameter (see xs3_vect_s16_sqrt()) will be 
 * required to achieve the same precision, as the results are computed bit by bit, starting with the most significant
 * bit.
 * @endparblock
 * 
 * @param[out]  a_exp       Exponent of outputs of xs3_vect_s16_sqrt()
 * @param[out]  b_shr       Right-shift to be applied to elements of @vector{b}
 * @param[in]   b_exp       Exponent of vector{b}
 * @param[in]   b_hr        Headroom of vector{b}
 * 
 * @see xs3_vect_s16_sqrt
 * 
 * @ingroup xs3_vect16_prepare
 */
C_API
void xs3_vect_s16_sqrt_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    const exponent_t b_exp,
    const right_shift_t b_hr);


/**
 * @brief Subtract one 16-bit BFP vector from another.
 * 
 * `a[]`, `b[]` and `c[]` represent the 16-bit vectors @vector{a}, @vector{b} and @vector{c} respectively. Each must 
 * begin at a word-aligned address. This operation can be performed safely in-place on `b[]` or `c[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b} and @vector{c} 
 * respectively.
 * 
 * @operation{ 
 * &     b_k' = sat_{16}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)  \\
 * &     c_k' = sat_{16}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)  \\
 * &     a_k \leftarrow sat_{16}\!\left( b_k' - c_k' \right)     \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} and @vector{c} are the mantissas of BFP vectors @math{ \bar{b} \cdot 2^{b\_exp} } and 
 * @math{\bar{c} \cdot 2^{c\_exp}}, then the resulting vector @vector{a} are the mantissas of BFP vector 
 * @math{\bar{a} \cdot 2^{a\_exp}}. 
 * 
 * In this case, @math{b\_shr} and @math{c\_shr} **must** be chosen so that 
 * @math{a\_exp = b\_exp + b\_shr = c\_exp + c\_shr}. Adding or subtracting mantissas only makes sense if they
 * are associated with the same exponent.
 * 
 * The function xs3_vect_s16_sub_prepare() can be used to obtain values for @math{a\_exp}, @math{b\_shr} and 
 * @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and the input headrooms @math{b\_hr} and 
 * @math{c\_hr}.
 * @endparblock
 * 
 * @param[out]      a           Output vector @vector{a}
 * @param[in]       b           Input vector @vector{b}
 * @param[in]       c           Input vector @vector{c}
 * @param[in]       length      Number of elements in vectors @vector{a}, @vector{b} and @vector{c}
 * @param[in]       b_shr       Right-shift appled to @vector{b}
 * @param[in]       c_shr       Right-shift appled to @vector{c}
 * 
 * @returns     Headroom of the output vector @vector{a}.
 * 
 * @exception ET_LOAD_STORE Raised if `a`, `b` or `c` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @see xs3_vect_s16_sub_prepare
 * 
 * @ingroup xs3_vect16_func
 */
C_API
headroom_t xs3_vect_s16_sub(
    int16_t a[],
    const int16_t b[],
    const int16_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Obtain the output exponent and shifts required for a call to `xs3_vect_s16_sub()`.
 * 
 * The logic for computing the shifts and exponents of `xs3_vect_s16_sub()` is identical to that for
 * `xs3_vect_s32_add()`.
 * 
 * This macro is provided as a convenience to developers and to make the code more readable.
 * 
 * @see xs3_vect_s32_add_prepare()
 * 
 * @ingroup xs3_vect16_prepare
 */
#define xs3_vect_s16_sub_prepare xs3_vect_s32_add_prepare


/**
 * @brief Get the sum of elements of a 16-bit vector.
 * 
 * `b[]` represents the 16-bit vector @vector{b}. `b[]` must begin at a word-aligned address.
 * 
 * `length` is the number of elements in @vector{b}.
 * 
 * @operation{
 *      a \leftarrow \sum_{k=0}^{length-1} b_k
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the returned value @math{a} is 
 * the 32-bit mantissa of floating-point value @math{a \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp}.
 * @endparblock
 * 
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   length      Number of elements in @vector{b}
 * 
 * @returns The 32-bit sum @math{a}
 * 
 * @exception ET_LOAD_STORE Raised if `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup xs3_vect16_func
 */
C_API
int32_t xs3_vect_s16_sum(
    const int16_t b[],
    const unsigned length);



#ifdef __XC__
}   //extern "C"
#endif

