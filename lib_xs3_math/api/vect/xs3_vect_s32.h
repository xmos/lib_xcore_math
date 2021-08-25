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
 * @page page_xs3_vect_s32_h  xs3_vect_s32.h
 * 
 * This header contains functions implementing arithmetic operations on 32-bit vectors, optimized 
 * for xCore XS3.
 * 
 * @note This header is included automatically through `xs3_math.h` or `bfp_math.h`.
 * 
 * @ingroup xs3_math_header_file
 */


/**
 * Maximum bit-depth that can be calculated by xs3_vect_s32_sqrt().
 * 
 * @see xs3_vect_s32_sqrt
 */
#define XS3_VECT_SQRT_S32_MAX_DEPTH     (31)


/**
 * @brief Copy one 32-bit vector to another.
 * 
 * This function is effectively a constrained version of `memcpy`.
 * 
 * With the constraints below met, this function should be modestly faster than `memcpy`.
 * 
 * `a[]` is the output vector to which elements are copied.
 * 
 * `b[]` is the input vector from which elements are copied.
 * 
 * `a` and `b` each must begin at a word-aligned address.
 * 
 * `length` is the number of elements to be copied. `length` must be a multiple of 8.
 * 
 * @operation{
 * &    a_k \leftarrow b_k                              \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1)
 * }
 * 
 * @param[out]  a         Output vector @vector{a}
 * @param[in]   b         Input vector @vector{b}
 * @param[in]   length    Number of elements in @vector{a} and @vector{b}
 * 
 * @returns   Headroom of output vector @vector{a}
 * 
 * @exception ET_LOAD_STORE Raised if `a` or `b` is not word-aligned (See @ref note_vector_alignment)
 *
 * @ingroup xs3_vect32_func
 */
C_API
headroom_t xs3_vect_s32_copy(
    int32_t a[],
    const int32_t b[],
    const unsigned length);


/** 
 * @brief Compute the element-wise absolute value of a 32-bit vector.
 * 
 * `a[]` and `b[]` represent the 32-bit vectors @vector{a} and @vector{b} respectively. Each must begin at a
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
 * @ingroup xs3_vect32_func
 */
C_API
headroom_t xs3_vect_s32_abs(
    int32_t a[],
    const int32_t b[],
    const unsigned length);


/**
 * @brief Compute the sum of the absolute values of elements of a 32-bit vector.
 * 
 * `b[]` represents the 32-bit mantissa vector @vector{b}. `b[]` must begin at a word-aligned address.
 * 
 * `length` is the number of elements in @vector{b}.
 * 
 * @operation{
 *      \sum_{k=0}^{length-1} sat_{32}(\left| b_k \right|)
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the returned value @math{a} is 
 * the 64-bit mantissa of floating-point value @math{a \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp}.
 * @endparblock
 * 
 * @par Additional Details
 * @parblock
 * 
 * Internally the sum accumulates into 8 separate 40-bit accumulators. These accumulators apply symmetric 40-bit 
 * saturation logic (with bounds @math{\pm (2^{39}-1)}) with each added element. At the end, the 8 accumulators are summed
 * together into the 64-bit value @math{a} which is returned by this function. No saturation logic is applied at this
 * final step.
 * 
 * Because symmetric 32-bit saturation logic is applied when computing the absolute value, in the corner case where 
 * each element is `INT32_MIN`, each of the 8 accumulators can accumulate @math{256} elements before saturation is
 * possible. Therefore, with @math{b\_hr} bits of headroom, no saturation of intermediate results is possible with fewer 
 * than @math{2^{11 + b\_hr}} elements in @vector{b}.
 * 
 * If the length of @vector{b} is greater than @math{2^{11 + b\_hr}}, the sum can be computed piece-wise in several 
 * calls to this function, with the partial results summed in user code.
 * @endparblock
 * 
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   length      Number of elements in @vector{b}
 * 
 * @returns The 64-bit sum @math{a}
 * 
 * @exception ET_LOAD_STORE Raised if `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup xs3_vect32_func
 */
C_API
int64_t xs3_vect_s32_abs_sum(
    const int32_t b[],
    const unsigned length);


/**
 * @brief Add together two 32-bit vectors.
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
 * &     b_k' = sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)  \\
 * &     c_k' = sat_{32}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)  \\
 * &     a_k \leftarrow sat_{32}\!\left( b_k' + c_k' \right)     \\
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
 * The function xs3_vect_s32_add_prepare() can be used to obtain values for @math{a\_exp}, @math{b\_shr} and 
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
 * @see xs3_vect_s32_prepare()
 * 
 * @ingroup xs3_vect32_func
 */
C_API
headroom_t xs3_vect_s32_add(
    int32_t a[],
    const int32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Obtain the output exponent and input shifts to add or subtract two 16- or 32-bit BFP vectors.
 * 
 * The block floating-point functions in this library which add or subtract vectors are of the general form:
 * 
 * @math{ 
 *      \bar{a} \cdot 2^{a\_exp} = \bar{b}\cdot 2^{b\_exp} \pm \bar{c}\cdot 2^{c\_exp} }
 * }
 * 
 * @vector{b} and @vector{c} are the input mantissa vectors with exponents @math{b\_exp} and @math{c\_exp}, which are 
 * shared by each element of their respective vectors. @vector{a} is the output mantissa vector with exponent 
 * @math{a\_exp}. Two additional properties, @math{b\_hr} and @math{c\_hr}, which are the 
 * headroom of mantissa vectors @vector{b} and @vector{c} respectively, are required by this function.
 * 
 * In order to avoid any overflows in the output mantissas, the output exponent @math{a\_exp} must be chosen such that 
 * the largest (in the sense of absolute value) possible output mantissa will fit into the allotted space (e.g. 32 bits
 * for xs3_vect_s32_add()). Once @math{a\_exp} is chosen, the input bit-shifts @math{b\_shr} and @math{c\_shr} are 
 * calculated to achieve that resulting exponent.
 * 
 * This function chooses @math{a\_exp} to be the minimum exponent known to avoid overflows, given the input exponents
 * (@math{b\_exp} and @math{c\_exp}) and input headroom (@math{b\_hr} and @math{c\_hr}).
 * 
 * This function is used calculate the output exponent and input bit-shifts for each of the following functions:
 * * xs3_vect_s16_add()
 * * xs3_vect_s32_add()
 * * xs3_vect_s16_sub()
 * * xs3_vect_s32_sub()
 * * xs3_vect_complex_s16_add()
 * * xs3_vect_complex_s32_add()
 * * xs3_vect_complex_s16_sub()
 * * xs3_vect_complex_s32_sub()
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
 * * If @math{b\_hr} or @math{c\_hr} are unknown, they can be calculated using the appropriate headroom function 
 * (e.g. xs3_vect_complex_s16_headroom() for complex 16-bit vectors) or the value `0` can always be safely used (but may 
 * result in reduced precision).
 * @endparblock
 *
 * @param[out] a_exp    Output exponent associated with output mantissa vector @vector{a}
 * @param[out] b_shr    Signed arithmetic right-shift to be applied to elements of @vector{b}. Used by the function 
 *                      which computes the output mantissas @vector{a}
 * @param[out] c_shr    Signed arithmetic right-shift to be applied to elements of @vector{c}. Used by the function 
 *                      which computes the output mantissas @vector{a}
 * @param[in]  b_exp    Exponent of BFP vector @vector{b}
 * @param[in]  c_exp    Exponent of BFP vector @vector{c}
 * @param[in]  b_hr     Headroom of BFP vector @vector{b} 
 * @param[in]  c_hr     Headroom of BFP vector @vector{c}
 * 
 * @see xs3_vect_s16_add, 
 *      xs3_vect_s32_add, 
 *      xs3_vect_s16_sub, 
 *      xs3_vect_s32_sub, 
 *      xs3_vect_complex_s16_add, 
 *      xs3_vect_complex_s32_add, 
 *      xs3_vect_complex_s16_sub, 
 *      xs3_vect_complex_s32_sub
 * 
 * @ingroup xs3_vect32_prepare
 */
C_API
void xs3_vect_s32_add_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    right_shift_t* c_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr);


/**
 * @brief Obtain the array index of the maximum element of a 32-bit vector.
 * 
 * `b[]` represents the 32-bit input vector @vector{b}. It must begin at a word-aligned address.
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
 */
C_API
unsigned xs3_vect_s32_argmax(
    const int32_t b[],
    const unsigned length);


/**
 * @brief Obtain the array index of the minimum element of a 32-bit vector.
 * 
 * `b[]` represents the 32-bit input vector @vector{b}. It must begin at a word-aligned address.
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
 * @ingroup xs3_vect32_func
 */
C_API
unsigned xs3_vect_s32_argmin(
    const int32_t b[],
    const unsigned length);


/**
 * @brief Clamp the elements of a 32-bit vector to a specified range.
 * 
 * `a[]` and `b[]` represent the 32-bit vectors @vector{a} and @vector{b} respectively. Each must begin at a 
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
 * &     b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor) \\
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
 * @ingroup xs3_vect32_func
 */
C_API
headroom_t xs3_vect_s32_clip(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const int32_t lower_bound,
    const int32_t upper_bound,
    const right_shift_t b_shr);

/**
 * @brief Obtain the output exponent, input shift and modified bounds used by xs3_vect_s32_clip().
 * 
 * This function is used in conjunction with xs3_vect_s32_clip() to bound the elements of a 32-bit BFP vector to a
 * specified range.
 * 
 * This function computes `a_exp`, `b_shr`, `lower_bound` and `upper_bound`.
 * 
 * `a_exp` is the exponent associated with the 32-bit mantissa vector @vector{a} computed by xs3_vect_s32_clip().
 * 
 * `b_shr` is the shift parameter required by xs3_vect_s32_clip() to achieve the output exponent `a_exp`.
 * 
 * `lower_bound` and `upper_bound` are the 32-bit mantissas which indicate the lower and upper clipping bounds
 * respectively. The values are modified by this function, and the resulting values should be passed along to
 * xs3_vect_s32_clip().
 * 
 * `b_exp` is the exponent associated with the input mantissa vector @vector{b}.
 * 
 * `bound_exp` is the exponent associated with the bound mantissas `lower_bound` and `upper_bound` respectively.
 * 
 * `b_hr` is the headroom of @vector{b}. If unknown, it can be obtained using xs3_vect_s32_headroom(). Alternatively, 
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
 * @see xs3_vect_s32_clip
 * 
 * @ingroup xs3_vect32_prepare
 */
C_API
void xs3_vect_s32_clip_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    int32_t* lower_bound,
    int32_t* upper_bound,
    const exponent_t b_exp,
    const exponent_t bound_exp,
    const headroom_t b_hr);


/**
 * @brief Compute the inner product between two 32-bit vectors.
 * 
 * `b[]` and `c[]` represent the 32-bit mantissa vectors @vector{b} and @vector{c} respectively. Each must begin at a 
 * word-aligned address.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b} and @vector{c} 
 * respectively.
 * 
 * @operation{
 * &     b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)                     \\
 * &     c_k' \leftarrow sat_{32}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)                     \\
 * &     a \leftarrow \sum_{k=0}^{length-1}\left(round( b_k' \cdot c_k' \cdot 2^{-30} ) \right)   \\
 * &     \qquad\text{where } a \text{ is returned}
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} and @vector{c} are the mantissas of the BFP vectors @math{ \bar{b} \cdot 2^{b\_exp} } and 
 * @math{\bar{c}\cdot 2^{c\_exp}}, then result @math{a} is the 64-bit mantissa of the result @math{a \cdot 2^{a\_exp}}, 
 * where @math{a\_exp = b\_exp + c\_exp + b\_shr + c\_shr + 30}.
 * 
 * If needed, the bit-depth of @math{a} can then be reduced to 32 bits to get a new result 
 * @math{a' \cdot 2^{a\_exp'}} where @math{a' = a \cdot 2^{-a\_shr}} and @math{a\_exp' = a\_exp + a\_shr}.
 * 
 * The function xs3_vect_s32_dot_prepare() can be used to obtain values for @math{a\_exp}, @math{b\_shr} and 
 * @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and the input headrooms @math{b\_hr} and 
 * @math{c\_hr}.
 * @endparblock
 * 
 * @par Additional Details
 * @parblock
 * 
 * The 30-bit rounding right-shift applied to each of the 64-bit products @math{b_k \cdot c_k} is a feature of the 
 * hardware and cannot be avoided. As such, if the input vectors @vector{b} and @vector{c} together have too much 
 * headroom (i.e. @math{b\_hr + c\_hr}), the sum may effectively vanish. To avoid this situation, negative values of 
 * `b_shr` and `c_shr` may be used (with the stipulation that @math{b\_shr \ge -b\_hr} and @math{c\_shr \ge -c\_hr} if
 * saturation of @math{b_k'} and @math{c_k'} is to be avoided). The less headroom @math{b_k'} and @math{c_k'} have, the
 * greater the precision of the final result.
 * 
 * Internally, each product @math{(b_k' \cdot c_k' \cdot 2^{-30})} accumulates into one of eight 40-bit accumulators 
 * (which are all used simultaneously) which apply symmetric 40-bit saturation logic (with bounds @math{\approx 2^{39}})
 * with each value added. The saturating arithmetic employed is _not associative_ and no indication is given if 
 * saturation occurs at an intermediate step. To avoid satuation errors, `length` should be no greater than 
 * @math{2^{10+b\_hr+c\_hr}}, where @math{b\_hr} and @math{c\_hr} are the headroom of @vector{b} and @vector{c} 
 * respectively.
 * 
 * If the caller's mantissa vectors are longer than that, the full inner product can be found by calling this function 
 * multiple times for partial inner products on sub-sequences of the input vectors, and adding the results in user code. 
 * 
 * In many situations the caller may have _a priori_ knowledge that saturation is impossible (or very nearly so), in 
 * which case this guideline may be disregarded. However, such situations are application-specific and are well beyond 
 * the scope of this documentation, and as such are left to the user's discretion.
 * @endparblock
 * 
 * @param[in] b             Input vector @vector{b}
 * @param[in] c             Input vector @vector{c}
 * @param[in] length        Number of elements in vectors @vector{b} and @vector{c}
 * @param[in] b_shr         Right-shift appled to @vector{b}
 * @param[in] c_shr         Right-shift appled to @vector{c}
 * 
 * @returns The inner product of vectors @vector{b} and @vector{c}, scaled as indicated above.
 * 
 * @exception ET_LOAD_STORE Raised if `b` or `c` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup xs3_vect32_func
 */
C_API
int64_t xs3_vect_s32_dot(
    const int32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Obtain the output exponent and input shift used by xs3_vect_s32_dot().
 * 
 * This function is used in conjunction with xs3_vect_s32_dot() to compute the inner product of two 32-bit BFP vectors.
 * 
 * This function computes `a_exp`, `b_shr` and `c_shr`.
 * 
 * `a_exp` is the exponent associated with the 64-bit mantissa @math{a} returned by xs3_vect_s32_dot(), and must be 
 * chosen to be large enough to avoid saturation when @math{a} is computed. To maximize precision, this function chooses
 * `a_exp` to be the smallest exponent known to avoid saturation (see exception below). The `a_exp` chosen by this 
 * function is derived from the exponents and headrooms associated with the input vectors.
 * 
 * `b_shr` and `c_shr` are the shift parameters required by xs3_vect_s32_dot() to achieve the chosen output exponent 
 * `a_exp`.
 * 
 * `b_exp` and `c_exp` are the exponents associated with the input mantissa vectors @vector{b} and @vector{c} 
 * respectively.
 * 
 * `b_hr` and `c_hr` are the headroom of @vector{b} and @vector{c} respectively. If either is unknown, they can be 
 * obtained using xs3_vect_s32_headroom(). Alternatively, the value `0` can always be safely used (but may result in 
 * reduced precision).
 * 
 * `length` is the number of elements in the input mantissa vectors @vector{b} and @vector{c}.
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
 * Be aware that using smaller values than strictly necessary for `b_shr` or `c_shr` can result in saturation, and using 
 * larger values may result in unnecessary underflows or loss of precision.
 * @endparblock
 * 
 * @param[out]  a_exp               Exponent associated with output mantissa @math{a}
 * @param[out]  b_shr               Signed arithmetic right-shift for @vector{b} used by xs3_vect_s32_dot()
 * @param[out]  c_shr               Signed arithmetic right-shift for @vector{c} used by xs3_vect_s32_dot()
 * @param[in]   b_exp               Exponent associated with input mantissa vector @vector{b}
 * @param[in]   c_exp               Exponent associated with input mantissa vector @vector{b}
 * @param[in]   b_hr                Headroom of input mantissa vector @vector{b}
 * @param[in]   c_hr                Headroom of input mantissa vector @vector{b}
 * @param[in]   length              Number of elements in vectors @vector{b} and @vector{c}
 * 
 * @see xs3_vect_s32_dot
 * 
 * @ingroup xs3_vect32_prepare
 */
C_API
void xs3_vect_s32_dot_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    right_shift_t* c_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr,
    const unsigned length);


/**
 * @brief Calculate the energy (sum of squares of elements) of a 32-bit vector.
 * 
 * `b[]` represents the 32-bit mantissa vector @vector{b}. `b[]` must begin at a word-aligned address.
 * 
 * `length` is the number of elements in @vector{b}.
 * 
 * `b_shr` is the signed arithmetic right-shift applied to elements of @vector{b}.
 * 
 * @operation{
 * &     b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)                 \\
 * &     a \leftarrow \sum_{k=0}^{length-1} round((b_k')^2 \cdot 2^{-30})
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the mantissas of the BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then floating-point result is
 * @math{a \cdot 2^{a\_exp}}, where the 64-bit mantissa @math{a} is returned by this function, and 
 * @math{a\_exp = 30 + 2 \cdot (b\_exp + b\_shr) }.
 * 
 * The function xs3_vect_s32_energy_prepare() can be used to obtain values for @math{a\_exp}, @math{b\_shr} and 
 * @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and the input headrooms @math{b\_hr} and 
 * @math{c\_hr}.
 * @endparblock
 * 
 * @par Additional Details
 * @parblock
 * 
 * The 30-bit rounding right-shift applied to each element of the 64-bit products @math{(b_k')^2} is a feature of the 
 * hardware and cannot be avoided. As such, if the input vector @vector{b} has too much headroom (i.e. 
 * @math{2\cdot b\_hr}), the sum may effectively vanish. To avoid this situation, negative values of `b_shr` may be used
 * (with the stipulation that @math{b\_shr \ge -b\_hr} if satuartion of @math{b_k'} is to be avoided). The less headroom
 * @math{b_k'} has, the greater the precision of the final result.
 * 
 * Internally, each product @math{(b_k')^2 \cdot 2^{-30}} accumulates into one of eight 40-bit accumulators (which are 
 * all used simultaneously) which apply symmetric 40-bit saturation logic (with bounds @math{\approx 2^{39}}) with each
 * value added. The saturating arithmetic employed is _not associative_ and no indication is given if saturation occurs
 * at an intermediate step. To avoid saturation errors, `length` should be no greater than @math{2^{10+2\cdotb\_hr}}, 
 * where @math{b\_hr} is the headroom of @vector{b}.
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
 * @ingroup xs3_vect32_func
 */
C_API
int64_t xs3_vect_s32_energy(
    const int32_t b[],
    const unsigned length,
    const right_shift_t b_shr);


/**
 * @brief Obtain the output exponent and input shift used by xs3_vect_s32_energy().
 * 
 * This function is used in conjunction with xs3_vect_s32_energy() to compute the inner product of a 32-bit BFP vector
 * with itself.
 * 
 * This function computes `a_exp` and `b_shr`.
 * 
 * `a_exp` is the exponent associated with the 64-bit mantissa @math{a} returned by xs3_vect_s32_energy(), and must be 
 * chosen to be large enough to avoid saturation when @math{a} is computed. To maximize precision, this function chooses
 * `a_exp` to be the smallest exponent known to avoid saturation (see exception below). The `a_exp` chosen by this 
 * function is derived from the exponent and headroom associated with the input vector.
 * 
 * `b_shr` is the shift parameter required by xs3_vect_s32_energy() to achieve the chosen output exponent `a_exp`.
 * 
 * `b_exp` is the exponent associated with the input mantissa vector @vector{b}.
 * 
 * `b_hr` is the headroom of @vector{b}. If it is unknown, it can be obtained using xs3_vect_s32_headroom(). 
 * Alternatively, the value `0` can always be safely used (but may result in reduced precision).
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
 * When applying the above adjustment, the following condition should be maintained:
 * * `b_hr + b_shr >= 0`
 * 
 * Be aware that using smaller values than strictly necessary for `b_shr` can result in saturation, and using larger 
 * values may result in unnecessary underflows or loss of precision.
 * @endparblock
 * 
 * @param[out]  a_exp       Exponent of outputs of xs3_vect_s32_energy()
 * @param[out]  b_shr       Right-shift to be applied to elements of @vector{b}
 * @param[in]   length      Number of elements in vector @vector{b}
 * @param[in]   b_exp       Exponent of vector{b}
 * @param[in]   b_hr        Headroom of vector{b}
 * 
 * @see xs3_vect_s32_energy
 * 
 * @ingroup xs3_vect32_prepare
 */
C_API
void xs3_vect_s32_energy_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    const unsigned length,
    const exponent_t b_exp,
    const headroom_t b_hr);


/**
 * @brief Calculate the headroom of a 32-bit vector.
 * 
 * The headroom of an N-bit integer is the number of bits that the integer's value may be left-shifted
 * without any information being lost. Equivalently, it is one less than the number of leading sign bits.
 * 
 * The headroom of an `int32_t` array is the minimum of the headroom of each of its `int32_t` elements.
 * 
 * This function efficiently traverses the elements of `a[]` to determine its headroom.
 * 
 * `x[]` represents the 32-bit vector @vector{x}. `x[]` must begin at a word-aligned address.
 * 
 * `length` is the number of elements in `x[]`.
 * 
 * 
 * @operation{
 *      min\!\\{ HR_{32}\left(x_0\right), HR_{32}\left(x_1\right), ..., HR_{32}\left(x_{length-1}\right) \\}
 * }
 * 
 * @param[in]   x           Input vector @vector{x}
 * @param[in]   length      The number of elements in `x[]`
 * 
 * @returns     Headroom of vector @vector{x} 
 * 
 * @exception ET_LOAD_STORE Raised if `x` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @see xs3_vect_s16_headroom, 
 *      xs3_vect_complex_s16_headroom, 
 *      xs3_vect_complex_s32_headroom
 * 
 * @ingroup xs3_vect32_func
 */
C_API
headroom_t xs3_vect_s32_headroom(
    const int32_t x[],
    const unsigned length);


/**
 * @brief Compute the inverse of elements of a 32-bit vector.
 * 
 * `a[]` and `b[]` represent the 32-bit mantissa vectors @vector{a} and @vector{b} respectively. Each vector must begin
 * at a word-aligned address. This operation can be performed safely in-place on `b[]`.
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
 * The function xs3_vect_s32_inverse_prepare() can be used to obtain values for @math{a\_exp} and @math{scale}.
 * @endparblock
 * 
 * @param[out]  a           Output vector @vector{a}
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   length      Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]   scale       Scale factor applied to dividend when computing inverse
 * 
 * @returns     Headroom of output vector @vector{a}
 * 
 * @exception ET_LOAD_STORE Raised if `a` or `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @see xs3_vect_s32_inverse_prepare
 * 
 * @ingroup xs3_vect32_func
 */
C_API
headroom_t xs3_vect_s32_inverse(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const unsigned scale);


/**
 * @brief Obtain the output exponent and scale used by xs3_vect_s32_inverse().
 * 
 * This function is used in conjunction with xs3_vect_s32_inverse() to compute the inverse of elements of a 32-bit
 * BFP vector.
 * 
 * This function computes `a_exp` and `scale`.
 * 
 * `a_exp` is the exponent associated with output mantissa vector @vector{a}, and must be chosen to avoid overflow in 
 * the smallest element of the input vector, which when inverted becomes the largest output element. To maximize 
 * precision, this function chooses `a_exp` to be the smallest exponent known to avoid saturation. The `a_exp` chosen
 * by this function is derived from the exponent and smallest element of the input vector.
 * 
 * `scale` is a scaling parameter used by xs3_vect_s32_inverse() to achieve the chosen output exponent.
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
 * @see xs3_vect_s32_inverse
 * 
 * @ingroup xs3_vect32_prepare
 */
C_API
void xs3_vect_s32_inverse_prepare(
    exponent_t* a_exp,
    unsigned* scale,
    const int32_t b[],
    const exponent_t b_exp,
    const unsigned length);


/**
 * @brief Find the maximum value in a 32-bit vector.
 * 
 * `b[]` represents the 32-bit vector @vector{b}. It must begin at a word-aligned address.
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
 * the 32-bit mantissa of floating-point value @math{a \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp}.
 * @endparblock
 * 
 * @param[in]   b           Input vector @vector{b}     
 * @param[in]   length      Number of elements in @vector{b}
 * 
 * @returns     Maximum value from @vector{b}
 * 
 * @exception ET_LOAD_STORE Raised if `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup xs3_vect32_func
 */
C_API
int32_t xs3_vect_s32_max(
    const int32_t b[],
    const unsigned length);


/**
 * @brief Find the minimum value in a 32-bit vector.
 * 
 * `b[]` represents the 32-bit vector @vector{b}. It must begin at a word-aligned address.
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
 * the 32-bit mantissa of floating-point value @math{a \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp}.
 * @endparblock
 * 
 * @param[in]   b           Input vector @vector{b}     
 * @param[in]   length      Number of elements in @vector{b}
 * 
 * @returns     Minimum value from @vector{b}
 * 
 * @exception ET_LOAD_STORE Raised if `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup xs3_vect32_func
 */
C_API
int32_t xs3_vect_s32_min(
    const int32_t b[],
    const unsigned length);


/**
 * @brief Multiply one 32-bit vector element-wise by another.
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
 * &     b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
 * &     c_k' \leftarrow sat_{32}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)     \\
 * &     a_k \leftarrow sat_{32}(round(b_k' \cdot c_k' \cdot 2^{-30}))       \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1)
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} and @vector{c} are the mantissas of BFP vectors @math{ \bar{b} \cdot 2^{b\_exp} } and 
 * @math{\bar{c} \cdot 2^{c\_exp}}, then the resulting vector @vector{a} are the mantissas of BFP vector 
 * @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp + c\_exp + b\_shr + c\_shr + 30}.
 * 
 * The function xs3_vect_s32_mul_prepare() can be used to obtain values for @math{a\_exp}, @math{b\_shr} and 
 * @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and the input headrooms @math{b\_hr} and 
 * @math{c\_hr}. 
 * @endparblock
 * 
 * @param[out] a        Output vector @vector{a}
 * @param[in]  b        Input vector @vector{b}
 * @param[in]  c        Input vector @vector{c}
 * @param[in]  length   Number of elements in vectors @vector{a}, @vector{b} and @vector{c}
 * @param[in]  b_shr    Right-shift appled to @vector{b}
 * @param[in]  c_shr    Right-shift appled to @vector{c}
 * 
 * @returns  Headroom of vector @vector{a}
 * 
 * @exception ET_LOAD_STORE Raised if `a`, `b` or `c` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup xs3_vect32_func
 */
//! [xs3_vect_s32_mul]
C_API
headroom_t xs3_vect_s32_mul(
    int32_t a[],
    const int32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);
//! [xs3_vect_s32_mul]


/**
 * @brief Multiply one 32-bit vector element-wise by another, and add the result to an accumulator.
 *
 * `acc[]` represents the 32-bit accumulator mantissa vector @vector{a}. Each @math{a_k} is 
 * `acc[k]`.
 *
 * `b[]` and `c[]` represent the 32-bit input mantissa vectors @vector{b} and @vector{c}, where each
 * @math{b_k} is `b[k]` and each @math{c_k} is `c[k]`.
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
 * &     v_k \leftarrow round( sat_{32}( \tilde{b}_k \cdot \tilde{c}_k \cdot
 *                                        2^{-30} ) )                           \\
 * &     a_k \leftarrow sat_{32}( \tilde{a}_k + v_k )                           \\
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
 * @param[in]     b_shr     Signed arithmetic right-shift applied to elements of @vector{b}
 * @param[in]     c_shr     Signed arithmetic right-shift applied to elements of @vector{c}
 * 
 * @returns   Headroom of the output vector @vector{a}
 *
 * @exception ET_LOAD_STORE Raised if `acc`, `b` or `c` is not word-aligned (See @ref note_vector_alignment)
 *
 * @see xs3_vect_s32_macc_prepare
 *
 * @ingroup xs3_vect32_func
 */
C_API
headroom_t xs3_vect_s32_macc(
    int32_t acc[],
    const int32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t acc_shr,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Multiply one 32-bit vector element-wise by another, and subtract the result from an 
 * accumulator.
 *
 * `acc[]` represents the 32-bit accumulator mantissa vector @vector{a}. Each @math{a_k} is 
 * `acc[k]`.
 *
 * `b[]` and `c[]` represent the 32-bit input mantissa vectors @vector{b} and @vector{c}, where each
 * @math{b_k} is `b[k]` and each @math{c_k} is `c[k]`.
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
 * &     v_k \leftarrow round( sat_{32}( \tilde{b}_k \cdot \tilde{c}_k \cdot
 *                                        2^{-30} ) )                           \\
 * &     a_k \leftarrow sat_{32}( \tilde{a}_k - v_k )                           \\
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
 * @param[in]     b_shr     Signed arithmetic right-shift applied to elements of @vector{b}
 * @param[in]     c_shr     Signed arithmetic right-shift applied to elements of @vector{c}
 * 
 * @returns   Headroom of the output vector @vector{a}
 *
 * @exception ET_LOAD_STORE Raised if `acc`, `b` or `c` is not word-aligned (See @ref note_vector_alignment)
 *
 * @see xs3_vect_s32_nmacc_prepare
 *
 * @ingroup xs3_vect32_func
 */
C_API
headroom_t xs3_vect_s32_nmacc(
    int32_t acc[],
    const int32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t acc_shr,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Obtain the output exponent and shifts needed by xs3_vect_s32_macc().
 *
 * This function is used in conjunction with xs3_vect_s32_macc() to perform an element-wise
 * multiply-accumlate of 32-bit BFP vectors.
 *
 * This function computes `new_acc_exp`, `acc_shr`, `b_shr` and `c_shr`, which are selected to
 * maximize precision in the resulting accumulator vector without causing saturation of final or
 * intermediate values. Normally the caller will pass these outputs to their corresponding inputs of
 * xs3_vect_s32_macc().
 *
 * `acc_exp` is the exponent associated with the accumulator mantissa vector @vector{a} prior to the
 * operation, whereas `new_acc_exp` is the exponent corresponding to the updated accumulator vector.
 *
 * `b_exp` and `c_exp` are the exponents associated with the complex input mantissa vectors
 * @vector{b} and @vector{c} respectively.
 *
 * `acc_hr`, `b_hr` and `c_hr` are the headrooms of @vector{a}, @vector{b} and @vector{c}
 * respectively. If the headroom of any of these vectors is unknown, it can be obtained by calling
 * xs3_vect_s32_headroom(). Alternatively, the value `0` can always be safely used (but may result
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
 *      right_shift_t acc_shr, b_shr, c_shr;
 *      xs3_vect_s32_macc_prepare(&acc_exp, &acc_shr, &b_shr, &c_shr, 
 *                                acc_exp, b_exp, c_exp,
 *                                acc_hr, b_hr, c_hr);
 * 
 *      // Modify results
 *      right_shift_t mant_shr = desired_exp - acc_exp;
 *      acc_exp += mant_shr;
 *      acc_shr += mant_shr;
 *      b_shr  += mant_shr;
 *      c_shr  += mant_shr;
 *      
 *      // acc_shr, b_shr and c_shr may now be used in a call to xs3_vect_s32_macc() 
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
 * @param[out]  acc_shr       Signed arithmetic right-shift used for @vector{a} in xs3_vect_s32_macc()
 * @param[out]  b_shr         Signed arithmetic right-shift used for @vector{b} in xs3_vect_s32_macc()
 * @param[out]  c_shr         Signed arithmetic right-shift used for @vector{c} in xs3_vect_s32_macc()
 * @param[in]   acc_exp       Exponent associated with input mantissa vector @vector{a} (before macc)
 * @param[in]   b_exp         Exponent associated with input mantissa vector @vector{b}
 * @param[in]   c_exp         Exponent associated with input mantissa vector @vector{c}
 * @param[in]   acc_hr        Headroom of input mantissa vector @vector{a} (before macc)
 * @param[in]   b_hr          Headroom of input mantissa vector @vector{b}
 * @param[in]   c_hr          Headroom of input mantissa vector @vector{c}
 *
 * @see xs3_vect_s32_macc
 *
 * @ingroup xs3_vect32_prepare
 */
C_API
void xs3_vect_s32_macc_prepare(
    exponent_t* new_acc_exp,
    right_shift_t* acc_shr,
    right_shift_t* b_shr,
    right_shift_t* c_shr,
    const exponent_t acc_exp,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t acc_hr,
    const headroom_t b_hr,
    const headroom_t c_hr);


/**
 * @brief Obtain the output exponent and shifts required for a call to xs3_vect_s32_nmacc().
 *
 * The logic for computing the shifts and exponents of `xs3_vect_s32_nmacc()` is identical
 * to that for `xs3_vect_s32_macc_prepare()`.
 *
 * This macro is provided as a convenience to developers and to make the code more readable.
 *
 * @see xs3_vect_s32_macc_prepare(), xs3_vect_s32_nmacc()
 *
 * @ingroup xs3_vect32_prepare
 */
#define xs3_vect_s32_nmacc_prepare  xs3_vect_s32_macc_prepare


/**
 * @brief Obtain the output exponent and input shifts used by xs3_vect_s32_mul().
 * 
 * This function is used in conjunction with xs3_vect_s32_mul() to perform an element-wise multiplication of two 32-bit
 * BFP vectors.
 * 
 * This function computes `a_exp`, `b_shr`, `c_shr`.
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
 * @vector{c} is unknown, they can be obtained by calling xs3_vect_s32_headroom(). Alternatively, the value `0` can 
 * always be safely used (but may result in reduced precision).
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
 * 
 * @param[out]  a_exp       Exponent of output elements of xs3_vect_s32_mul()
 * @param[out]  b_shr       Right-shift to be applied to elements of @vector{b}
 * @param[out]  c_shr       Right-shift to be applied to elemetns of @vector{c}
 * @param[in]   b_exp       Exponent of @vector{b}
 * @param[in]   c_exp       Exponent of @vector{c}
 * @param[in]   b_hr        Headroom of @vector{b}
 * @param[in]   c_hr        Headroom of @vector{c}
 * 
 * @see xs3_vect_s32_mul
 * 
 * @ingroup xs3_vect32_prepare
 */
C_API
void xs3_vect_s32_mul_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    right_shift_t* c_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr);


/** 
 * @brief Rectify the elements of a 32-bit vector.
 * 
 * `a[]` and `b[]` represent the 32-bit mantissa vectors @vector{a} and @vector{b} respectively. Each must begin at a 
 * word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * @operation{
 * &     a_k \leftarrow 
 *           \begin\{cases\}
 *               b_k & b_k \gt 0 \\ 
 * &             0 & b_k \leq 0
 *       \end\{cases\}           \\
 * &     \qquad\text{ for }k\in 0\ ...\ (length-1)
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the output vector @vector{a} are
 * the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp}.
 * @endparblock
 * 
 * @param[out]  a       Output vector @vector{a}
 * @param[in]   b       Input vector @vector{b}
 * @param[in]   length  Number of elements in vectors @vector{a} and @vector{b}
 * 
 * @returns  Headroom of the output vector @vector{a}
 * 
 * @exception ET_LOAD_STORE Raised if `a` or `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup xs3_vect32_func
 */
C_API
headroom_t xs3_vect_s32_rect(
    int32_t a[],
    const int32_t b[],
    const unsigned length);


/**
 * @brief Multiply a 32-bit vector by a scalar.
 * 
 * `a[]` and `b[]`represent the 32-bit mantissa vectors @vector{a} and @vector{b} respectively. Each must begin at a 
 * word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `c` is the 32-bit scalar @math{c} by which each element of @vector{b} is multiplied.
 * 
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b} and to @math{c}.
 * 
 * @operation{
 * &     b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
 * &     a_k  \leftarrow sat_{32}(round(c \cdot b_k' \cdot 2^{-30}))         \\
 * &          \qquad\text{ for }k\in 0\ ...\ (length-1)
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} } and @math{c} is the mantissa of
 * floating-point value @math{c \cdot 2^{c\_exp}}, then the resulting vector @vector{a} are the mantissas of BFP vector 
 * @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp + c\_exp + b\_shr + c\_shr + 30}.
 * 
 * The function xs3_vect_s32_scale_prepare() can be used to obtain values for @math{a\_exp}, @math{b\_shr} and 
 * @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and the input headrooms @math{b\_hr} and 
 * @math{c\_hr}. 
 * @endparblock
 * 
 * @param[out]  a           Output vector @vector{a}
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   length      Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]   c           Scalar to be multiplied by elements of @vector{b}
 * @param[in]   b_shr       Right-shift appled to @vector{b}
 * @param[in]   c_shr       Right-shift applied to @math{c}
 * 
 * @returns  Headroom of output vector @vector{a}
 * 
 * @exception ET_LOAD_STORE Raised if `a` or `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @see xs3_vect_s32_scale_prepare
 * 
 * @ingroup xs3_vect32_func
 */
C_API
headroom_t xs3_vect_s32_scale(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const int32_t c,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Obtain the output exponent and shifts required for a call to `xs3_vect_s32_scale()`.
 * 
 * The logic for computing the shifts and exponents of `xs3_vect_s32_scale()` is identical to that for
 * `xs3_vect_s32_mul()`.
 * 
 * This macro is provided as a convenience to developers and to make the code more readable.
 * 
 * @see xs3_vect_s32_mul_prepare()
 * 
 * @ingroup xs3_vect32_prepare
 */
#define xs3_vect_s32_scale_prepare xs3_vect_s32_mul_prepare



/** 
 * @brief Set all elements of a 32-bit vector to the specified value.
 * 
 * `a[]` represents the 32-bit output vector @vector{a}. `a[]` must begin at a word-aligned address.
 * 
 * `b` is the new value to set each element of @vector{a} to.
 * 
 * @operation{
 * &     a_k \leftarrow b    \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1)
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @math{b} is the mantissa of floating-point value @math{b \cdot 2^{b\_exp}}, then the output vector @vector{a} are
 * the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp}.
 * @endparblock
 * 
 * @param[out]  a           Output vector @vector{a}
 * @param[in]   b           New value for the elements of @vector{a}
 * @param[in]   length      Number of elements in @vector{a}
 * 
 * @exception ET_LOAD_STORE Raised if `a` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup xs3_vect32_func
 */
C_API
void xs3_vect_s32_set(
    int32_t a[],
    const int32_t b,
    const unsigned length);


/**
 * @brief Left-shift the elements of a 32-bit vector by a specified number of bits.
 * 
 * `a[]` and `b[]` represent the 32-bit vectors @vector{a} and @vector{b} respectively. Each must begin at a 
 * word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in vectors @vector{a} and @vector{b}.
 * 
 * `b_shl` is the signed arithmetic left-shift applied to each element of @vector{b}. 
 * 
 * @operation{ 
 * &     a_k \leftarrow sat_{32}(\lfloor b_k \cdot 2^{b\_shl} \rfloor)       \\
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
 * @ingroup xs3_vect32_func
 */
C_API
headroom_t xs3_vect_s32_shl(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const left_shift_t b_shl);


/**
 * @brief Right-shift the elements of a 32-bit vector by a specified number of bits.
 * 
 * `a[]` and `b[]` represent the 32-bit vectors @vector{a} and @vector{b} respectively. Each must begin at a 
 * word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in vectors @vector{a} and @vector{b}.
 * 
 * `b_shr` is the signed arithmetic right-shift applied to each element of @vector{b}. 
 * 
 * @operation{ 
 * &     a_k \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)      \\
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
 * @ingroup xs3_vect32_func
 */
C_API
headroom_t xs3_vect_s32_shr(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const right_shift_t b_shr);


/**
 * @brief Compute the square root of elements of a 32-bit vector.
 * 
 * `a[]` and `b[]` represent the 32-bit mantissa vectors @vector{a} and @vector{b} respectively. Each vector must begin
 * at a word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` is the signed arithmetic right-shift applied to elements of @vector{b}.
 * 
 * `depth` is the number of most significant bits to calculate of each @math{a_k}. For example, a `depth` value of 8 
 * will only compute the 8 most significant byte of the result, with the remaining 3 bytes as 0. The maximum value
 * for this parameter is `XS3_VECT_SQRT_S32_MAX_DEPTH` (31). The time cost of this operation is approximately 
 * proportional to the number of bits computed.
 * 
 * @operation{ 
 * &     b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
 * &     a_k \leftarrow \sqrt{ b_k' }                                        \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1)                       \\
 * &         \qquad\text{ where } sqrt() \text{ computes the first } depth \text{ bits of the square root.}
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the resulting vector @vector{a}
 * are the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = (b\_exp + b\_shr - 30)/2}.
 * 
 * Note that because exponents must be integers, that means @math{b\_exp + b\_shr} **must be even**.
 * 
 * The function xs3_vect_s32_sqrt_prepare() can be used to obtain values for @math{a\_exp} and @math{b\_shr} based on 
 * the input exponent @math{b\_exp} and headroom @math{b\_hr}.
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
 * @ingroup xs3_vect32_func
 */
C_API
headroom_t xs3_vect_s32_sqrt(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const right_shift_t b_shr,
    const unsigned depth);


/**
 * @brief Obtain the output exponent and shift parameter used by xs3_vect_s32_sqrt().
 * 
 * This function is used in conjunction withx xs3_vect_s32_sqrt() to compute the square root of elements of a 32-bit
 * BFP vector.
 * 
 * This function computes `a_exp` and `b_shr`.
 * 
 * `a_exp` is the exponent associated with output mantissa vector @vector{a}, and should be chosen to maximize the 
 * precision of the results. To that end, this function chooses `a_exp` to be the smallest exponent known to avoid 
 * saturation of the resulting mantissa vector @vector{a}. It is derived from the exponent and headroom of the input
 * BFP vector.
 * 
 * `b_shr` is the shift parameter required by xs3_vect_s32_sqrt() to achieve the chosen output exponent `a_exp`.
 * 
 * `b_exp` is the exponent associated with the input mantissa vector @vector{b}.
 * 
 * `b_hr` is the headroom of @vector{b}. If it is unknown, it can be obtained using xs3_vect_s32_headroom(). 
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
 * Also, if a larger exponent is used than necessary, a larger `depth` parameter (see xs3_vect_s32_sqrt()) will be 
 * required to achieve the same precision, as the results are computed bit by bit, starting with the most significant
 * bit.
 * @endparblock
 * 
 * @param[out]  a_exp       Exponent of outputs of xs3_vect_s32_sqrt()
 * @param[out]  b_shr       Right-shift to be applied to elements of @vector{b}
 * @param[in]   b_exp       Exponent of vector{b}
 * @param[in]   b_hr        Headroom of vector{b}
 * 
 * @see xs3_vect_s32_sqrt
 * 
 * @ingroup xs3_vect32_prepare
 */
C_API
void xs3_vect_s32_sqrt_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    const exponent_t b_exp,
    const right_shift_t b_hr);


/**
 * @brief Subtract one 32-bit vector from another.
 * 
 * `a[]`, `b[]` and `c[]` represent the 32-bit mantissa vectors @vector{a}, @vector{b} and
 * @vector{c} respectively. Each must begin at a word-aligned address. This operation can be
 * performed safely in-place on `b[]` or `c[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b}
 * and @vector{c} respectively.
 * 
 * @operation{ 
 * &     b_k' = sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)  \\
 * &     c_k' = sat_{32}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)  \\
 * &     a_k \leftarrow sat_{32}\!\left( b_k' - c_k' \right)     \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} and @vector{c} are the mantissas of BFP vectors @math{ \bar{b} \cdot 2^{b\_exp} }
 * and @math{\bar{c} \cdot 2^{c\_exp}}, then the resulting vector @vector{a} are the mantissas of
 * BFP vector @math{\bar{a} \cdot 2^{a\_exp}}. 
 *
 * In this case, @math{b\_shr} and @math{c\_shr} **must** be chosen so that @math{a\_exp = b\_exp +
 * b\_shr = c\_exp + c\_shr}. Adding or subtracting mantissas only makes sense if they are
 * associated with the same exponent.
 * 
 * The function xs3_vect_s32_sub_prepare() can be used to obtain values for @math{a\_exp},
 * @math{b\_shr} and  * @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp}
 * and the input headrooms @math{b\_hr} and @math{c\_hr}.
 * @endparblock
 * 
 * @param[out] a        Output vector @vector{a}
 * @param[in] b         Input vector @vector{b}
 * @param[in] c         Input vector @vector{c}
 * @param[in] length    Number of elements in vectors @vector{a}, @vector{b} and @vector{c}
 * @param[in] b_shr     Right-shift appled to @vector{b}
 * @param[in] c_shr     Right-shift appled to @vector{c}
 * 
 * @returns  Headroom of output vector @vector{a}
 * 
 * @exception ET_LOAD_STORE Raised if `a`, `b` or `c` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @see xs3_vect_s32_sub_prepare
 * 
 * @ingroup xs3_vect32_func
 */
C_API
headroom_t xs3_vect_s32_sub(
    int32_t a[],
    const int32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Obtain the output exponent and shifts required for a call to `xs3_vect_s32_sub()`.
 * 
 * The logic for computing the shifts and exponents of `xs3_vect_s32_sub()` is identical to that for
 * `xs3_vect_s32_add()`.
 * 
 * This macro is provided as a convenience to developers and to make the code more readable.
 * 
 * @see xs3_vect_s32_add_prepare()
 * 
 * @ingroup xs3_vect32_prepare
 */
#define xs3_vect_s32_sub_prepare xs3_vect_s32_add_prepare

/**
 * @brief Sum the elements of a 32-bit vector.
 * 
 * `b[]` represents the 32-bit mantissa vector @vector{b}. `b[]` must begin at a word-aligned address.
 * 
 * `length` is the number of elements in @vector{b}.
 * 
 * @operation{
 *      a \leftarrow \sum_\{k=0\}^\{length-1\} b_k     
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the returned value @math{a} is 
 * the 64-bit mantissa of floating-point value @math{a \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp}.
 * @endparblock
 * 
 * @par Additional Details
 * @parblock
 * 
 * Internally, each element accumulates into one of eight 40-bit accumulators (which are all used
 * simultaneously) which apply symmetric 40-bit saturation logic (with bounds @math{\approx 2^{39}})
 * with each value added. The saturating arithmetic employed is _not associative_ and no indication
 * is given if saturation occurs at an intermediate step. To avoid the possibility of saturation
 * errors, `length` should be no greater than @math{2^{11+b\_hr}}, where 
 * @math{b\_hr} is the headroom of @vector{b}.
 * 
 * If the caller's mantissa vector is longer than that, the full result can be found by calling this
 * function multiple times for partial results on sub-sequences of the input, and adding the results
 * in user code. 
 *
 * In many situations the caller may have _a priori_ knowledge that saturation is impossible (or
 * very nearly so), in which case this guideline may be disregarded. However, such situations are
 * application-specific and are well beyond the scope of this documentation, and as such are left to
 * the user's discretion.
 * @endparblock
 * 
 * @param[in]   b         Input vector @vector{b}
 * @param[in]   length    Number of elements in vector @vector{b}
 * 
 * @returns  64-bit mantissa of the sum, @math{a}.
 * 
 * @exception ET_LOAD_STORE Raised if `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup xs3_vect32_func
 */
C_API
int64_t xs3_vect_s32_sum(
    const int32_t b[],
    const unsigned length);


/**
 * @brief Interleave the elements of two vectors into a single vector.
 *
 * Elements of 32-bit input vectors @vector{b} and @vector{c} are interleaved into 32-bit output
 * vector @vector{a}. Each element of @vector{b} has a right-shift of @math{b\_shr} applied, and
 * each element of @vector{c} has a right-shift of @math{c\_shr} applied.
 *
 * Alternatively (and equivalently), this function can be conceived of as taking two real vectors
 * @vector{b} and @vector{c} and forming a new complex vector @vector{a} where @math{\bar{a} =
 * \bar{b} + i\cdot\bar{c}}.
 *
 * If vectors @vector{b} and @vector{c} each have @math{N} elements, then the resulting @vector{a}
 * will have either @math{2N} `int32_t` elements or (equivalently) @math{N} `complex_s32_t` elements
 * (and must have space for such). 
 *
 * Each element @math{b_k} of @vector{b} will end up as end up as element @math{a_{2k}} of
 * @vector{a} (with the bit-shift applied). Each element @math{c_k} will end up as element
 * @math{a_{2k+1}} of @vector{a}.
 *
 * `a[]` is the output vector @vector{a}.
 *
 * `b[]` and `c[]` are the input vectors @vector{b} and @vector{c} respectively.
 *
 * `a`, `b` and `c` must each begin at a double word-aligned (8 byte) address. (see @ref
 * DWORD_ALIGNED).
 *
 * `length` is the number @math{N} of `int32_t` elements in @vector{b} and @vector{c}.
 *
 * `b_shr` is the signed arithmetic right-shift applied to elements of @vector{b}.
 *
 * `c_shr` is the signed arithmetic right-shift applied to elements of @vector{c}.
 *
 * @operation{
 * &     Re\{a_{k}\} \leftarrow sat_{32}( b_k \cdot 2^{-b\_shr}                 \\
 * &     Im\{a_{k}\} \leftarrow sat_{32}( c_k \cdot 2^{-c\_shr}                 \\
 * &         \qquad\text{ for }k\in 0\ ...\ (N-1) 
 * }
 * 
 * @param[out]  a       Output vector @vector{a}
 * @param[in]   b       Input vector @vector{b}
 * @param[in]   c       Input vector @vector{c}
 * @param[in]   length  Number of elements @math{N} in vectors @vector{a}, @vector{b} and @vector{c}
 * @param[in]   b_shr   Signed arithmetic right-shift applied to elements of @vector{b}
 * @param[in]   c_shr   Signed arithmetic right-shift applied to elements of @vector{c}
 *
 * @exception ET_LOAD_STORE Raised if `a`, `b` or `c` is not double word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup xs3_vect32_func
 */
C_API
void xs3_vect_s32_zip(
    complex_s32_t a[],
    const int32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Deinterleave the real and imaginary parts of a complex 32-bit vector into two separate
 * vectors.
 *
 * Complex 32-bit input vector @vector{c} has its real and imaginary parts (which correspond to the
 * even and odd-indexed elements, if reinterpreted as an `int32_t` array) split apart to create real
 * 32-bit output vectors @vector{a} and @vector{b}, such that  @math{\bar{a} = Re\{\bar{c}\}} and
 * @math{\bar{b} = Im\{\bar{c}\}}.
 *
 * `a[]` and `b[]` are the real output vectors @vector{a} and @vector{b} which receive the real and
 * imaginary parts respectively of @vector{c}. `a` and `b` must each begin at a word-aligned
 * address.
 *
 * `c[]` is the complex input vector @vector{c}. `c` must begin at a double word-aligned address.
 *
 * `length` is the number @math{N} of `int32_t` elements in @vector{a} and @vector{b} and the number
 * of `complex_s32_t` in @vector{c}.
 *
 * @operation{
 * &    a_k = Re\{c_k}                              \\
 * &    b_k = Im\{c_k}                              \\
 * &         \qquad\text{ for }k\in 0\ ...\ (N-1) 
 * }
 *
 * @param[out]  a       Output vector @vector{a}
 * @param[out]  b       Output vector @vector{b}
 * @param[in]   c       Input vector @vector{c}
 * @param[in]   length  The number of elements @math{N} in vectors @vector{a}, @vector{b} and @vector{c}
 *
 * @exception ET_LOAD_STORE Raised if `a` or `b` is not word-aligned (See @ref note_vector_alignment)
 * @exception ET_LOAD_STORE Raised if `c` is not double word-aligned (See @ref note_vector_alignment)
 *
 * @ingroup xs3_vect32_func
 */
C_API
void xs3_vect_s32_unzip(
    int32_t a[],
    int32_t b[],
    const complex_s32_t c[],
    const unsigned length);

#ifdef __XC__
}   //extern "C"
#endif

