// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include "xmath/xmath_conf.h"
#include "xmath/types.h"
#include "xmath/util.h"

#include "xmath/vect/vect_s32_prepare.h"

/**
 * @defgroup vect_s32_api   32-bit Vector Functions
 */

#ifdef __XC__
extern "C" {
#endif



/**
 * Maximum bit-depth that can be calculated by vect_s32_sqrt().
 * 
 * @see vect_s32_sqrt
 * 
 * @ingroup vect_s32_api
 */
#define VECT_SQRT_S32_MAX_DEPTH     (31)


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
 * @ingroup vect_s32_api
 */
C_API
headroom_t vect_s32_copy(
    int32_t a[],
    const int32_t b[],
    const unsigned length);


/** 
 * @brief Compute the element-wise absolute value of a 32-bit vector.
 * 
 * `a[]` and `b[]` represent the 32-bit vectors @vector{a} and @vector{b} respectively. Each must
 * begin at a word-aligned address. This operation can be performed safely in-place on `b[]`.
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
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the output
 * vector @vector{a} are the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where
 * @math{a\_exp = b\_exp}.
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
 * @ingroup vect_s32_api
 */
C_API
headroom_t vect_s32_abs(
    int32_t a[],
    const int32_t b[],
    const unsigned length);


/**
 * @brief Compute the sum of the absolute values of elements of a 32-bit vector.
 * 
 * `b[]` represents the 32-bit mantissa vector @vector{b}. `b[]` must begin at a word-aligned
 * address.
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
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the returned
 * value @math{a} is the 64-bit mantissa of floating-point value @math{a \cdot 2^{a\_exp}}, where
 * @math{a\_exp = b\_exp}.
 * @endparblock
 * 
 * @par Additional Details
 * @parblock
 * 
 * Internally the sum accumulates into 8 separate 40-bit accumulators. These accumulators apply
 * symmetric 40-bit saturation logic (with bounds @math{\pm (2^{39}-1)}) with each added element. At
 * the end, the 8 accumulators are summed together into the 64-bit value @math{a} which is returned
 * by this function. No saturation logic is applied at this final step.
 *
 * Because symmetric 32-bit saturation logic is applied when computing the absolute value, in the
 * corner case where each element is `INT32_MIN`, each of the 8 accumulators can accumulate
 * @math{256} elements before saturation is possible. Therefore, with @math{b\_hr} bits of headroom,
 * no saturation of intermediate results is possible with fewer than @math{2^{11 + b\_hr}} elements
 * in @vector{b}.
 *
 * If the length of @vector{b} is greater than @math{2^{11 + b\_hr}}, the sum can be computed
 * piece-wise in several calls to this function, with the partial results summed in user code.
 * @endparblock
 * 
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   length      Number of elements in @vector{b}
 * 
 * @returns The 64-bit sum @math{a}
 * 
 * @exception ET_LOAD_STORE Raised if `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_s32_api
 */
C_API
int64_t vect_s32_abs_sum(
    const int32_t b[],
    const unsigned length);


/**
 * @brief Add together two 32-bit vectors.
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
 * &     a_k \leftarrow sat_{32}\!\left( b_k' + c_k' \right)     \\
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
 * In this case, @math{b\_shr} and @math{c\_shr} **must** be chosen so that 
 * @math{a\_exp = b\_exp + b\_shr = c\_exp + c\_shr}. Adding or subtracting mantissas only makes
 * sense if they are associated with the same exponent.
 *
 * The function vect_s32_add_prepare() can be used to obtain values for @math{a\_exp}, @math{b\_shr}
 * and @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and the input
 * headrooms @math{b\_hr} and @math{c\_hr}.
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
 * @see vect_s32_add_prepare()
 * 
 * @ingroup vect_s32_api
 */
C_API
headroom_t vect_s32_add(
    int32_t a[],
    const int32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Add a scalar to a 32-bit vector.
 * 
 * `a[]`, `b[]` represent the 32-bit mantissa vectors @vector{a} and @vector{b} respectively. Each
 * must begin at a word-aligned address. This operation can be performed safely in-place on `b[]`.
 *
 * `c` is the scalar @math{c} to be added to each element of @vector{b}.
 *
 * `length` is the number of elements in each of the vectors.
 *
 * `b_shr` is the signed arithmetic right-shift applied to each element of @vector{b}.
 * 
 * @operation{ 
 * &     b_k' = sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)  \\
 * &     a_k \leftarrow sat_{32}\!\left( b_k' + c \right)        \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If elements of @vector{b} are the mantissas of BFP vector @math{ \bar{b} \cdot 2^{b\_exp} }, and 
 * @math{c} is the mantissa of floating-point value @math{c \cdot 2^{c\_exp}}, then the resulting
 * vector @vector{a} are the mantissas of BFP vector 
 * @math{\bar{a} \cdot 2^{a\_exp}}. 
 *
 * In this case, @math{b\_shr} and @math{c\_shr} **must** be chosen so that 
 * @math{a\_exp = b\_exp + b\_shr = c\_exp + c\_shr}. Adding or subtracting mantissas only makes
 * sense if they are associated with the same exponent.
 *
 * The function vect_s32_add_scalar_prepare() can be used to obtain values for @math{a\_exp},
 * @math{b\_shr} and 
 * @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and the input
 * headrooms @math{b\_hr} and 
 * @math{c\_hr}.
 *
 * Note that @math{c\_shr} is an output of `vect_s32_add_scalar_prepare()`, but is not a parameter
 * to this function. The @math{c\_shr} produced by `vect_s32_add_scalar_prepare()` is to be applied
 * by the user, and the result passed as input `c`.
 * @endparblock
 * 
 * @param[out]      a           Output vector @vector{a}
 * @param[in]       b           Input vector @vector{b}
 * @param[in]       c           Input scalar @math{c}
 * @param[in]       length      Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]       b_shr       Right-shift appled to @vector{b}
 * 
 * @returns     Headroom of the output vector @vector{a}.
 * 
 * @exception ET_LOAD_STORE Raised if `a` or `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @see vect_s32_add_scalar_prepare()
 * 
 * @ingroup vect_s32_api
 */
C_API
headroom_t vect_s32_add_scalar(
    int32_t a[],
    const int32_t b[],
    const int32_t c,
    const unsigned length,
    const right_shift_t b_shr);


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
 * @returns @math{a}, the index of the maximum element of vector @vector{b}. If there is a tie for
 *          the maximum value, the lowest tying index is returned.
 * 
 * @exception ET_LOAD_STORE Raised if `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_s32_api
 */
C_API
unsigned vect_s32_argmax(
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
 * @returns @math{a}, the index of the minimum element of vector @vector{b}. If there is a tie for
 *          the minimum value, the lowest tying index is returned.
 * 
 * @exception ET_LOAD_STORE Raised if `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_s32_api
 */
C_API
unsigned vect_s32_argmin(
    const int32_t b[],
    const unsigned length);


/**
 * @brief Clamp the elements of a 32-bit vector to a specified range.
 * 
 * `a[]` and `b[]` represent the 32-bit vectors @vector{a} and @vector{b} respectively. Each must
 * begin at a word-aligned address. This operation can be performed safely in-place on `b[]`.
 *
 * `length` is the number of elements in each of the vectors.
 *
 * `lower_bound` and `upper_bound` are the lower and upper bounds of the clipping range
 * respectively. These bounds are checked for each element of @vector{b} only _after_ `b_shr` is
 * applied.
 *
 * `b_shr` is the signed arithmetic right-shift applied to elements of @vector{b} _before_ being
 * compared to the upper and lower bounds.
 *
 * If @vector{b} are the mantissas for a BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the
 * exponent @math{a\_exp} of the output BFP vector @math{\bar{a} \cdot 2^{a\_exp}} is given by
 * @math{a\_exp = b\_exp + b\_shr}.
 * 
 * @operation{
 * &     b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor) \\
 * &     a_k \leftarrow \begin\{cases\}
 *           lower\_bound & b_k' \le lower\_bound                        \\
 *           upper\_bound & b_k' \ge upper\_bound                        \\
 *           b_k' & otherwise \end\{cases\}                              \\
 * &     \qquad\text{ for }k\in 0\ ...\ (length-1)
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the output
 * vector @vector{a} are the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where
 * @math{a\_exp = b\_exp + b\_shr}.
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
 * @ingroup vect_s32_api
 */
C_API
headroom_t vect_s32_clip(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const int32_t lower_bound,
    const int32_t upper_bound,
    const right_shift_t b_shr);


/**
 * @brief Compute the inner product between two 32-bit vectors.
 * 
 * `b[]` and `c[]` represent the 32-bit mantissa vectors @vector{b} and @vector{c} respectively.
 * Each must begin at a word-aligned address.
 *
 * `length` is the number of elements in each of the vectors.
 *
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b}
 * and @vector{c} respectively.
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
 * If @vector{b} and @vector{c} are the mantissas of the BFP vectors @math{ \bar{b} \cdot 2^{b\_exp}
 * } and @math{\bar{c}\cdot 2^{c\_exp}}, then result @math{a} is the 64-bit mantissa of the result
 * @math{a \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp + c\_exp + b\_shr + c\_shr + 30}.
 *
 * If needed, the bit-depth of @math{a} can then be reduced to 32 bits to get a new result 
 * @math{a' \cdot 2^{a\_exp'}} where @math{a' = a \cdot 2^{-a\_shr}} and @math{a\_exp' = a\_exp +
 * a\_shr}.
 *
 * The function vect_s32_dot_prepare() can be used to obtain values for @math{a\_exp}, @math{b\_shr}
 * and @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and the input
 * headrooms @math{b\_hr} and @math{c\_hr}.
 * @endparblock
 * 
 * @par Additional Details
 * @parblock
 * 
 * The 30-bit rounding right-shift applied to each of the 64-bit products @math{b_k \cdot c_k} is a
 * feature of the hardware and cannot be avoided. As such, if the input vectors @vector{b} and
 * @vector{c} together have too much headroom (i.e. @math{b\_hr + c\_hr}), the sum may effectively
 * vanish. To avoid this situation, negative values of `b_shr` and `c_shr` may be used (with the
 * stipulation that @math{b\_shr \ge -b\_hr} and @math{c\_shr \ge -c\_hr} if saturation of
 * @math{b_k'} and @math{c_k'} is to be avoided). The less headroom @math{b_k'} and @math{c_k'}
 * have, the greater the precision of the final result.
 *
 * Internally, each product @math{(b_k' \cdot c_k' \cdot 2^{-30})} accumulates into one of eight
 * 40-bit accumulators (which are all used simultaneously) which apply symmetric 40-bit saturation
 * logic (with bounds @math{\approx 2^{39}}) with each value added. The saturating arithmetic
 * employed is _not associative_ and no indication is given if saturation occurs at an intermediate
 * step. To avoid satuation errors, `length` should be no greater than 
 * @math{2^{10+b\_hr+c\_hr}}, where @math{b\_hr} and @math{c\_hr} are the headroom of @vector{b} and
 * @vector{c} respectively.
 *
 * If the caller's mantissa vectors are longer than that, the full inner product can be found by
 * calling this function multiple times for partial inner products on sub-sequences of the input
 * vectors, and adding the results in user code. 
 *
 * In many situations the caller may have _a priori_ knowledge that saturation is impossible (or
 * very nearly so), in which case this guideline may be disregarded. However, such situations are
 * application-specific and are well beyond the scope of this documentation, and as such are left to
 * the user's discretion.
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
 * @ingroup vect_s32_api
 */
C_API
int64_t vect_s32_dot(
    const int32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Calculate the energy (sum of squares of elements) of a 32-bit vector.
 * 
 * `b[]` represents the 32-bit mantissa vector @vector{b}. `b[]` must begin at a word-aligned
 * address.
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
 * If @vector{b} are the mantissas of the BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then
 * floating-point result is @math{a \cdot 2^{a\_exp}}, where the 64-bit mantissa @math{a} is
 * returned by this function, and @math{a\_exp = 30 + 2 \cdot (b\_exp + b\_shr) }.
 *
 * The function vect_s32_energy_prepare() can be used to obtain values for @math{a\_exp},
 * @math{b\_shr} and @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and
 * the input headrooms @math{b\_hr} and @math{c\_hr}.
 * @endparblock
 * 
 * @par Additional Details
 * @parblock
 * 
 * The 30-bit rounding right-shift applied to each element of the 64-bit products @math{(b_k')^2} is
 * a feature of the hardware and cannot be avoided. As such, if the input vector @vector{b} has too
 * much headroom (i.e. @math{2\cdot b\_hr}), the sum may effectively vanish. To avoid this
 * situation, negative values of `b_shr` may be used (with the stipulation that @math{b\_shr \ge
 * -b\_hr} if satuartion of @math{b_k'} is to be avoided). The less headroom @math{b_k'} has, the
 * greater the precision of the final result.
 *
 * Internally, each product @math{(b_k')^2 \cdot 2^{-30}} accumulates into one of eight 40-bit
 * accumulators (which are all used simultaneously) which apply symmetric 40-bit saturation logic
 * (with bounds @math{\approx 2^{39}}) with each value added. The saturating arithmetic employed is
 * _not associative_ and no indication is given if saturation occurs at an intermediate step. To
 * avoid saturation errors, `length` should be no greater than @math{2^{10+2\cdot b\_hr}}, where
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
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   length      Number of elements in @vector{b}
 * @param[in]   b_shr       Right-shift appled to @vector{b}
 * 
 * @returns 64-bit mantissa of vector @vector{b}'s energy
 * 
 * @exception ET_LOAD_STORE Raised if `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_s32_api
 */
C_API
int64_t vect_s32_energy(
    const int32_t b[],
    const unsigned length,
    const right_shift_t b_shr);


/**
 * @brief Calculate the headroom of a 32-bit vector.
 * 
 * The headroom of an N-bit integer is the number of bits that the integer's value may be
 * left-shifted without any information being lost. Equivalently, it is one less than the number of
 * leading sign bits.
 *
 * The headroom of an `int32_t` array is the minimum of the headroom of each of its `int32_t`
 * elements.
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
 * @see vect_s16_headroom, 
 *      vect_complex_s16_headroom, 
 *      vect_complex_s32_headroom
 * 
 * @ingroup vect_s32_api
 */
C_API
headroom_t vect_s32_headroom(
    const int32_t x[],
    const unsigned length);


/**
 * @brief Compute the inverse of elements of a 32-bit vector.
 * 
 * `a[]` and `b[]` represent the 32-bit mantissa vectors @vector{a} and @vector{b} respectively.
 * Each vector must begin at a word-aligned address. This operation can be performed safely in-place
 * on `b[]`.
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
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the resulting
 * vector @vector{a} are the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where
 * @math{a\_exp = scale - b\_exp}.
 *
 * The function vect_s32_inverse_prepare() can be used to obtain values for @math{a\_exp} and
 * @math{scale}.
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
 * @see vect_s32_inverse_prepare
 * 
 * @ingroup vect_s32_api
 */
C_API
headroom_t vect_s32_inverse(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const unsigned scale);


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
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the returned
 * value @math{a} is the 32-bit mantissa of floating-point value @math{a \cdot 2^{a\_exp}}, where
 * @math{a\_exp = b\_exp}.
 * @endparblock
 * 
 * @param[in]   b           Input vector @vector{b}     
 * @param[in]   length      Number of elements in @vector{b}
 * 
 * @returns     Maximum value from @vector{b}
 * 
 * @exception ET_LOAD_STORE Raised if `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_s32_api
 */
C_API
int32_t vect_s32_max(
    const int32_t b[],
    const unsigned length);


/**
 * @brief Get the element-wise maximum of two 32-bit vectors.
 * 
 * `a[]`, `b[]` and `c[]` represent the 32-bit mantissa vectors @vector{a},
 * @vector{b} and @vector{c} respectively. Each must begin at a word-aligned address. This operation
 * can be performed safely in-place on `b[]`, but @a not on `c[]`.
 *
 * `length` is the number of elements in each of the vectors.
 *
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b}
 * and @vector{c} respectively.
 *
 * @operation{
 * &     b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
 * &     c_k' \leftarrow sat_{32}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)     \\
 * &     a_k \leftarrow max(b_k', c_k')                                      \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1)
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} and @vector{c} are the mantissas of BFP vectors @math{ \bar{b} \cdot 2^{b\_exp} }
 * and @math{\bar{c} \cdot 2^{c\_exp}}, then the resulting vector @vector{a} are the mantissas of
 * BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp + b\_shr = c\_exp +
 * c\_shr}.
 *
 * The function vect_2vec_prepare() can be used to obtain values for @math{a\_exp}, @math{b\_shr}
 * and @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and the input
 * headrooms @math{b\_hr} and @math{c\_hr}. 
 * @endparblock
 * 
 * @warning For correct operation, this function requires at least 1 bit of 
 *          headroom in each mantissa vector @a after the shifts have been 
 *          applied.
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
 * @ingroup vect_s32_api
 */
C_API
headroom_t vect_s32_max_elementwise(
    int32_t a[],
    const int32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


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
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the returned
 * value @math{a} is the 32-bit mantissa of floating-point value @math{a \cdot 2^{a\_exp}}, where
 * @math{a\_exp = b\_exp}.
 * @endparblock
 * 
 * @param[in]   b           Input vector @vector{b}     
 * @param[in]   length      Number of elements in @vector{b}
 * 
 * @returns     Minimum value from @vector{b}
 * 
 * @exception ET_LOAD_STORE Raised if `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_s32_api
 */
C_API
int32_t vect_s32_min(
    const int32_t b[],
    const unsigned length);


/**
 * @brief Get the element-wise minimum of two 32-bit vectors.
 * 
 * `a[]`, `b[]` and `c[]` represent the 32-bit mantissa vectors @vector{a},
 * @vector{b} and @vector{c} respectively. Each must begin at a word-aligned address. This operation
 * can be performed safely in-place on `b[]`, but @a not on `c[]`.
 *
 * `length` is the number of elements in each of the vectors.
 *
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b}
 * and @vector{c} respectively.
 * 
 * @operation{
 * &     b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
 * &     c_k' \leftarrow sat_{32}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)     \\
 * &     a_k \leftarrow min(b_k', c_k')                                      \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1)
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} and @vector{c} are the mantissas of BFP vectors @math{ \bar{b} \cdot 2^{b\_exp} }
 * and @math{\bar{c} \cdot 2^{c\_exp}}, then the resulting vector @vector{a} are the mantissas of
 * BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp + b\_shr = c\_exp +
 * c\_shr}.
 *
 * The function vect_2vec_prepare() can be used to obtain values for @math{a\_exp}, @math{b\_shr}
 * and @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and the input
 * headrooms @math{b\_hr} and @math{c\_hr}. 
 * @endparblock
 * 
 * @warning For correct operation, this function requires at least 1 bit of 
 *          headroom in each mantissa vector @a after the shifts have been 
 *          applied.
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
 * @ingroup vect_s32_api
 */
C_API
headroom_t vect_s32_min_elementwise(
    int32_t a[],
    const int32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Multiply one 32-bit vector element-wise by another.
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
 * &     b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
 * &     c_k' \leftarrow sat_{32}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)     \\
 * &     a_k \leftarrow sat_{32}(round(b_k' \cdot c_k' \cdot 2^{-30}))       \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1)
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} and @vector{c} are the mantissas of BFP vectors @math{ \bar{b} \cdot 2^{b\_exp} }
 * and @math{\bar{c} \cdot 2^{c\_exp}}, then the resulting vector @vector{a} are the mantissas of
 * BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp + c\_exp + b\_shr +
 * c\_shr + 30}.
 *
 * The function vect_s32_mul_prepare() can be used to obtain values for @math{a\_exp}, @math{b\_shr}
 * and @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and the input
 * headrooms @math{b\_hr} and @math{c\_hr}. 
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
 * @ingroup vect_s32_api
 */
C_API
headroom_t vect_s32_mul(
    int32_t a[],
    const int32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


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
 * The function vect_complex_s16_macc_prepare() can be used to obtain values for @math{a\_exp},
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
 * @see vect_s32_macc_prepare
 *
 * @ingroup vect_s32_api
 */
C_API
headroom_t vect_s32_macc(
    int32_t acc[],
    const int32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t acc_shr,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Multiply one 32-bit vector element-wise by another, and subtract the result from an 
 *        accumulator.
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
 * The function vect_complex_s16_macc_prepare() can be used to obtain values for @math{a\_exp},
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
 * @see vect_s32_nmacc_prepare
 *
 * @ingroup vect_s32_api
 */
C_API
headroom_t vect_s32_nmacc(
    int32_t acc[],
    const int32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t acc_shr,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/** 
 * @brief Rectify the elements of a 32-bit vector.
 * 
 * `a[]` and `b[]` represent the 32-bit mantissa vectors @vector{a} and @vector{b} respectively.
 * Each must begin at a word-aligned address. This operation can be performed safely in-place on
 * `b[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * @operation{
 * &     a_k \leftarrow 
 *           \begin\{cases\}
 *               b_k & b_k > 0 \\ 
 *               0 & b_k \leq 0
 *       \end\{cases\}           \\
 * &     \qquad\text{ for }k\in 0\ ...\ (length-1)
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the output
 * vector @vector{a} are the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where
 * @math{a\_exp = b\_exp}.
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
 * @ingroup vect_s32_api
 */
C_API
headroom_t vect_s32_rect(
    int32_t a[],
    const int32_t b[],
    const unsigned length);


/**
 * @brief Multiply a 32-bit vector by a scalar.
 * 
 * `a[]` and `b[]`represent the 32-bit mantissa vectors @vector{a} and @vector{b} respectively. Each
 * must begin at a word-aligned address. This operation can be performed safely in-place on `b[]`.
 *
 * `length` is the number of elements in each of the vectors.
 *
 * `c` is the 32-bit scalar @math{c} by which each element of @vector{b} is multiplied.
 *
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b}
 * and to @math{c}.
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
 * If @vector{b} are the mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} } and @math{c} is
 * the mantissa of floating-point value @math{c \cdot 2^{c\_exp}}, then the resulting vector
 * @vector{a} are the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp =
 * b\_exp + c\_exp + b\_shr + c\_shr + 30}.
 *
 * The function vect_s32_scale_prepare() can be used to obtain values for @math{a\_exp},
 * @math{b\_shr} and @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and
 * the input headrooms @math{b\_hr} and @math{c\_hr}. 
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
 * @see vect_s32_scale_prepare
 * 
 * @ingroup vect_s32_api
 */
C_API
headroom_t vect_s32_scale(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const int32_t c,
    const right_shift_t b_shr,
    const right_shift_t c_shr);



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
 * If @math{b} is the mantissa of floating-point value @math{b \cdot 2^{b\_exp}}, then the output
 * vector @vector{a} are the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where
 * @math{a\_exp = b\_exp}.
 * @endparblock
 * 
 * @param[out]  a           Output vector @vector{a}
 * @param[in]   b           New value for the elements of @vector{a}
 * @param[in]   length      Number of elements in @vector{a}
 * 
 * @exception ET_LOAD_STORE Raised if `a` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_s32_api
 */
C_API
void vect_s32_set(
    int32_t a[],
    const int32_t b,
    const unsigned length);


/**
 * @brief Left-shift the elements of a 32-bit vector by a specified number of bits.
 * 
 * `a[]` and `b[]` represent the 32-bit vectors @vector{a} and @vector{b} respectively. Each must
 * begin at a word-aligned address. This operation can be performed safely in-place on `b[]`.
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
 * If @vector{b} are the mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} }, then the
 * resulting vector @vector{a} are the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}},
 * where @math{\bar{a} = \bar{b} \cdot 2^{b\_shl}} and @math{a\_exp = b\_exp}.
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
 * @ingroup vect_s32_api
 */
C_API
headroom_t vect_s32_shl(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const left_shift_t b_shl);


/**
 * @brief Right-shift the elements of a 32-bit vector by a specified number of bits.
 * 
 * `a[]` and `b[]` represent the 32-bit vectors @vector{a} and @vector{b} respectively. Each must
 * begin at a word-aligned address. This operation can be performed safely in-place on `b[]`.
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
 * If @vector{b} are the mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} }, then the
 * resulting vector @vector{a} are the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}},
 * where @math{\bar{a} = \bar{b} \cdot 2^{-b\_shr}} and @math{a\_exp = b\_exp}.
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
 * @ingroup vect_s32_api
 */
C_API
headroom_t vect_s32_shr(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const right_shift_t b_shr);


/**
 * @brief Compute the square root of elements of a 32-bit vector.
 * 
 * `a[]` and `b[]` represent the 32-bit mantissa vectors @vector{a} and @vector{b} respectively.
 * Each vector must begin at a word-aligned address. This operation can be performed safely in-place
 * on `b[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` is the signed arithmetic right-shift applied to elements of @vector{b}.
 * 
 * `depth` is the number of most significant bits to calculate of each @math{a_k}. For example, a
 * `depth` value of 8 will only compute the 8 most significant byte of the result, with the
 * remaining 3 bytes as 0. The maximum value for this parameter is `VECT_SQRT_S32_MAX_DEPTH` (31).
 * The time cost of this operation is approximately proportional to the number of bits computed.
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
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the resulting
 * vector @vector{a} are the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where
 * @math{a\_exp = (b\_exp + b\_shr - 30)/2}.
 *
 * Note that because exponents must be integers, that means @math{b\_exp + b\_shr} **must be even**.
 *
 * The function vect_s32_sqrt_prepare() can be used to obtain values for @math{a\_exp} and
 * @math{b\_shr} based on the input exponent @math{b\_exp} and headroom @math{b\_hr}.
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
 * @ingroup vect_s32_api
 */
C_API
headroom_t vect_s32_sqrt(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const right_shift_t b_shr,
    const unsigned depth);


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
 * The function vect_s32_sub_prepare() can be used to obtain values for @math{a\_exp},
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
 * @see vect_s32_sub_prepare
 * 
 * @ingroup vect_s32_api
 */
C_API
headroom_t vect_s32_sub(
    int32_t a[],
    const int32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


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
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the returned
 * value @math{a} is the 64-bit mantissa of floating-point value @math{a \cdot 2^{a\_exp}}, where
 * @math{a\_exp = b\_exp}.
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
 * @ingroup vect_s32_api
 */
C_API
int64_t vect_s32_sum(
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
 * `a`, `b` and `c` must each begin at a double word-aligned (8 byte) address. (see
 * `DWORD_ALIGNED`).
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
 * @ingroup vect_s32_api
 */
C_API
void vect_s32_zip(
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
 * &    a_k = Re\\{c_k\\}                              \\
 * &    b_k = Im\\{c_k\\}                              \\
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
 * @ingroup vect_s32_api
 */
C_API
void vect_s32_unzip(
    int32_t a[],
    int32_t b[],
    const complex_s32_t c[],
    const unsigned length);


/**
 * @brief Convolve a 32-bit vector with a short kernel.
 * 
 * 32-bit input vector @vector{x} is convolved with a short fixed-point kernel @vector{b} to produce
 * 32-bit output vector @vector{y}.  In other words, this function applies the @math{K}th-order FIR
 * filter with coefficients given by @vector{b} to the input signal @vector{x}.  The convolution is
 * "valid" in the sense that no output elements are emitted where the filter taps extend beyond the
 * bounds of the input vector, resulting in an output vector @vector{y} with fewer elements.
 * 
 * The maximum filter order @math{K} supported by this function is @math{7}.
 * 
 * `y[]` is the output vector @vector{y}.  If input @vector{x} has @math{N} elements, and the filter
 * has @math{K} elements, then @vector{y} has @math{N-2P} elements, where 
 * @math{P = \lfloor K / 2 \rfloor}.
 * 
 * `x[]` is the input vector @vector{x} with length @math{N}.
 * 
 * `b_q30[]` is the vector @vector{b} of filter coefficients. The coefficients of @vector{b} are
 * encoded in a Q2.30 fixed-point format. The effective value of the @math{i}th coefficient is then
 * @math{b_i \cdot 2^{-30}}.
 * 
 * `x_length` is the length @math{N} of @vector{x} in elements.
 * 
 * `b_length` is the length @math{K} of @vector{b} in elements (i.e. the number of filter taps).
 * `b_length` must be one of @math{ \\{ 1, 3, 5, 7 \\} }. 
 *
 *
 * @operation{
 * &    y_k \leftarrow  \sum_{l=0}^{K-1} (x_{(k+l)} \cdot b_l \cdot 2^{-30} )   \\
 * &         \qquad\text{ for }k\in 0\ ...\ (N-2P)                              \\
 * &         \qquad\text{ where }P = \lfloor K/2 \rfloor 
 * }
 * 
 * @par Additional Details
 * @parblock
 * 
 * To avoid the possibility of saturating any output elements, @vector{b} may be constrained such
 * that @math{ \sum_{i=0}^{K-1} \left|b_i\right| \leq 2^{30} }.
 * 
 * This operation can be applied safely in-place on `x[]`.
 * 
 * @endparblock
 *
 * @param[out]  y           Output vector @vector{y}
 * @param[in]   x           Input vector @vector{x}
 * @param[in]   b_q30       Filter coefficient vector @vector{b}
 * @param[in]   x_length    The number of elements @math{N} in vector @vector{x}
 * @param[in]   b_length    The number of elements @math{K} in @vector{b}
 *
 * @exception ET_LOAD_STORE Raised if `x` or `y` or `b_q30` is not word-aligned (See @ref note_vector_alignment)
 *
 * @ingroup vect_s32_api
 */
C_API
headroom_t vect_s32_convolve_valid(
    int32_t y[],
    const int32_t x[],
    const int32_t b_q30[],
    const unsigned x_length,
    const unsigned b_length);


/**
 * @brief Supported padding modes for convolutions in "same" mode.
 * 
 * @see vect_s32_convolve_same(), bfp_s32_convolve_same()
 * 
 * @ingroup vect_s32_api
 */
typedef enum {
  /**
   * Vector is reflected at its boundaries, such that
   * 
   *  @math{ \tilde{x}_i \begin\{cases\}
   *           x_{-i} & i < 0                           \\
   *           x_{2N - 2 - i} & i \ge N                   \\
   *           x_i & otherwise 
   *          \end\{cases\} }
   * 
   * For example, if the length @math{N} of input vector @vector{x} is @math{7} and the order 
   * @math{K} of the filter is @math{5}, then
   * 
   * @math{ \bar{x} = [x_0, x_1, x_2, x_3, x_4, x_5, x_6] }
   * 
   * @math{ \tilde{x} = [x_2, x_1, x_0, x_1, x_2, x_3, x_4, x_5, x_6, x_5, x_4] }
   * 
   * Note that by convention the first element of @math{\tilde{x}} is considered to be at index 
   * @math{-P}, where @math{P = \lfloor K/2 \rfloor}.
   */
  PAD_MODE_REFLECT = (INT32_MAX-0),

  /**
   * Vector is padded using the value of the bounding elements.
   * 
   *  @math{ \tilde{x}_i \begin\{cases\}
   *           x_{0} & i < 0            \\
   *           x_{N-1} & i \ge N          \\
   *           x_i & otherwise 
   *          \end\{cases\} }
   * 
   * For example, if the length @math{N} of input vector @vector{x} is @math{7} and the order 
   * @math{K} of the filter is @math{5}, then
   * 
   * @math{ \bar{x} = [x_0, x_1, x_2, x_3, x_4, x_5, x_6] }
   * 
   * @math{ \tilde{x} = [x_0, x_0, x_0, x_1, x_2, x_3, x_4, x_5, x_6, x_6, x_6] }
   * 
   * Note that by convention the first element of @math{\tilde{x}} is considered to be at index 
   * @math{-P}, where @math{P = \lfloor K/2 \rfloor}.
   */
  PAD_MODE_EXTEND  = (INT32_MAX-1),

  /**
   * Vector is padded with zeroes.
   * 
   *  @math{ \tilde{x}_i \begin\{cases\}
   *           0 & i < 0            \\
   *           0 & i \ge N            \\
   *           x_i & otherwise 
   *          \end\{cases\} }
   * 
   * For example, if the length @math{N} of input vector @vector{x} is @math{7} and the order 
   * @math{K} of the filter is @math{5}, then
   * 
   * @math{ \bar{x} = [x_0, x_1, x_2, x_3, x_4, x_5, x_6] }
   * 
   * @math{ \tilde{x} = [0, 0, x_0, x_1, x_2, x_3, x_4, x_5, x_6, 0, 0] }
   * 
   * Note that by convention the first element of @math{\tilde{x}} is considered to be at index 
   * @math{-P}, where @math{P = \lfloor K/2 \rfloor}.
   */
  PAD_MODE_ZERO = 0,
} pad_mode_e;


/**
 * @brief Convolve a 32-bit vector with a short kernel.
 * 
 * 32-bit input vector @vector{x} is convolved with a short fixed-point kernel @vector{b} to produce
 * 32-bit output vector @vector{y}.  In other words, this function applies the @math{K}th-order FIR
 * filter with coefficients given by @vector{b} to the input signal @vector{x}.  The convolution
 * mode is "same" in that the input vector is effectively padded such that the input and output
 * vectors are the same length.  The padding behavior is one of those given by @ref pad_mode_e.
 * 
 * The maximum filter order @math{K} supported by this function is @math{7}.
 * 
 * `y[]` and `x[]` are the output and input vectors @vector{y} and @vector{x} respectively.
 * 
 * `b_q30[]` is the vector @vector{b} of filter coefficients. The coefficients of @vector{b} are
 * encoded in a Q2.30 fixed-point format. The effective value of the @math{i}th coefficient is then
 * @math{b_i \cdot 2^{-30}}.
 * 
 * `x_length` is the length @math{N} of @vector{x} and @vector{y} in elements.
 * 
 * `b_length` is the length @math{K} of @vector{b} in elements (i.e. the number of filter taps).
 * `b_length` must be one of @math{ \\{ 1, 3, 5, 7 \\} }. 
 *  
 * `padding_mode` is one of the values from the @ref pad_mode_e enumeration. The padding mode 
 * indicates the filter input values for filter taps that have extended beyond the bounds of the
 * input vector @vector{x}. See @ref pad_mode_e for a list of supported padding modes and associated
 * behaviors.
 *
 * @operation{
 * &    \tilde{x}_i = \begin\{cases\}
 *           \text{determined by padding mode} & i < 0                                  \\
 *           \text{determined by padding mode} & i \ge N                                  \\
 *           x_i & otherwise \end\{cases\}                                                \\
 * &    y_k \leftarrow  \sum_{l=0}^{K-1} (\tilde{x}_{(k+l-P)} \cdot b_l \cdot 2^{-30} )   \\
 * &         \qquad\text{ for }k\in 0\ ...\ (N-2P)                                        \\
 * &         \qquad\text{ where }P = \lfloor K/2 \rfloor 
 * }
 * 
 * 
 * @par Additional Details
 * @parblock
 * 
 * To avoid the possibility of saturating any output elements, @vector{b} may be constrained such
 * that @math{ \sum_{i=0}^{K-1} \left|b_i\right| \leq 2^{30} }.
 * @endparblock
 * 
 * @note Unlike vect_s32_convolve_valid(), this operation _cannot_ be performed safely in-place
 * on `x[]`
 *
 * @param[out]  y               Output vector @vector{y}
 * @param[in]   x               Input vector @vector{x}
 * @param[in]   b_q30           Filter coefficient vector @vector{b}
 * @param[in]   x_length        The number of elements @math{N} in vector @vector{x}
 * @param[in]   b_length        The number of elements @math{K} in @vector{b}
 * @param[in]   padding_mode    The padding mode to be applied at signal boundaries
 *
 * @exception ET_LOAD_STORE Raised if `x` or `y` or `b_q30` is not word-aligned (See @ref note_vector_alignment)
 *
 * @ingroup vect_s32_api
 */
C_API
headroom_t vect_s32_convolve_same(
    int32_t y[],
    const int32_t x[],
    const int32_t b_q30[],
    const unsigned x_length,
    const unsigned b_length,
    const pad_mode_e padding_mode);


/**
 * @brief Merge a vector of split 32-bit accumulators into a vector of int32_t's.
 * 
 * Convert a vector of @ref split_acc_s32_t into a vector of `int32_t`. This is useful when
 * a function (e.g. `mat_mul_s8_x_s8_yield_s32`) outputs a vector of accumulators in the XS3
 * VPU's native split 32-bit format, which has the upper half of each accumulator in the first 32
 * bytes and the lower half in the following 32 bytes.
 * 
 * This function is most efficient (in terms of cycles/accumulator) when `length` is a multiple of
 * 16. In any case, `length` will be rounded up such that a multiple of 16 accumulators will always
 * be merged.
 * 
 * This function can safely merge accumulators in-place.
 * 
 * @param[out]  a       Output vector of int32_t
 * @param[in]   b       Input vector of split_acc_s32_t
 * @param[in]   length  Number of accumulators to merge
 * 
 * @exception ET_LOAD_STORE Raised if `b` or `a` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_s32_api
 */
C_API
void vect_s32_merge_accs(
    int32_t a[],
    const split_acc_s32_t b[],
    const unsigned length);


/**
 * @brief Split a vector of `int32_t`'s into a vector of `split_acc_s32_t`.
 * 
 * Convert a vector of `int32_t` into a vector of @ref split_acc_s32_t, the native format
 * for the XS3 VPU's 32-bit accumulators. This is useful when a function (e.g.
 * `mat_mul_s8_x_s8_yield_s32`) takes in a vector of accumulators in that native format.
 * 
 * This function is most efficient (in terms of cycles/accumulator) when `length` is a multiple of
 * 16. In any case, `length` will be rounded up such that a multiple of 16 accumulators will always
 * be split.
 * 
 * This function can safely split accumulators in-place.
 * 
 * @param[out]  a       Output vector of split_acc_s32_t 
 * @param[in]   b       Input vector of int32_t
 * @param[in]   length  Number of accumulators to split
 * 
 * @exception ET_LOAD_STORE Raised if `b` or `a` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_s32_api
 */
C_API
void vect_s32_split_accs(
    split_acc_s32_t a[],
    const int32_t b[],
    const unsigned length);

    
/**
 * @brief Apply a right-shift to the elements of a 32-bit split accumulator vector.
 * 
 * This function may be used in conjunction with chunk_s16_accumulate() or bfp_s16_accumulate()
 * to avoid saturation of accumulators.
 * 
 * This function updates @vector{a} in-place.
 * 
 * @param[inout]  a       Accumulator vector @vector{a}
 * @param[in]     length  Number of elements of @vector{a}
 * @param[in]     shr     Number of bits to right-shift the elements of @vector{a}
 *  
 * @exception ET_LOAD_STORE Raised if `a` is not double-word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_s32_api
 */
C_API
void vect_split_acc_s32_shr(
    split_acc_s32_t a[],
    const unsigned length,
    const right_shift_t shr);


/**
 * @brief Compute a power series sum on a vector of Q2.30 values.
 * 
 * This function is used to compute a power series summation on a vector @vector{b}. @vector{b} 
 * contains Q2.30 values. @vector{c} is a vector containing coefficients to be multiplied by powers 
 * of @vector{b}, and may have any associated exponent. The output is vector @vector{a} and has the 
 * same exponent as @vector{c}.
 * 
 * `c[]` is an array with shape `(term_count, VPU_INT32_EPV)`, where the second axis contains the 
 * same value replicated across all `VPU_INT32_EPV` elements. That is, `c[k][i] = c[k][j]` for `i` 
 * and `j` in `0..(VPU_INT32_EPV-1)`. This is for performance reasons. (For the purpose of this 
 * explanation, @vector{c} is considered to be single-dimensional, without redundancy.)
 * 
 * @operation{
 * &    b_{k,0} = 2^{30}                                                                    \\
 * &    b_{k,i} = round\left(\frac{b_{k,i-1}\cdot{}b_k}{2^{30}}\right)                      \\
 * &    \qquad\text{for }i \in \{1..(N-1)\}                                                 \\
 * &    a_k \leftarrow  \sum_{i=0}^{N-1}  round\left( \frac{b_{k,i}\cdot c_i}{2^{30}}  \right) \\
 * &    \qquad\text{for }k \in \{0..\mathtt{length}-1\}
 * }
 * 
 * @param[out]  a           Output vector @vector{a}
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   c           Coefficient vector @vector{c}
 * @param[in]   term_count  Number of power series terms, @math{N}
 * @param[in]   length      Number of elements in vectors @vector{a} and @vector{b}
 * 
 * @ingroup vect_s32_api
 */
C_API
void vect_q30_power_series(
    int32_t a[],
    const q2_30 b[],
    const int32_t c[],
    const unsigned term_count,
    const unsigned length);
 

/**
 * @brief Compute the logarithm (in the specified base) of a vector of `float_s32_t`.
 * 
 * This function computes the logarithm of a vector @vector{b} of `float_s32_t` values. The base of
 * the computed logarithm is given by parameter `inv_ln_base_q30`. The result is written to output
 * @vector{a}, a vector of Q8.24 values.
 * 
 * If the desired base is @math{D}, then `inv_ln_base_q30`, represented here by @math{R}, should be 
 * @math{\mathtt{Q30}\left(\frac{1}{ln\left(D\right)}\right)}. That is: the inverse of the natural
 * logarithm of the desired base, expressed as a Q2.30 value. Typically the desired base is known
 * at compile time, so this value will usually be a precomputed constant.
 * 
 * The resulting @math{a_k} for @math{b_k \le 0} is undefined.
 * 
 * @operation{
 * &    a_k \leftarrow  log_{D}\left(b_k\right)   \\
 * &    \qquad\text{for }k \in \{0..\mathtt{length}-1\}
 * }
 * 
 * @param[out]  a         Output Q8.24 vector @vector{a}
 * @param[in]   b         Input vector @vector{b}
 * @param[in]   inv_ln_base_q30   Coefficient @math{R} converting from natural log to desired base @math{D}
 * @param[in]   length    Number of elements in vectors @vector{a} and @vector{b}
 * 
 * @exception ET_LOAD_STORE Raised if `b` or `a` is not double word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_s32_api
 */
C_API
void vect_float_s32_log_base(
    q8_24 a[],
    const float_s32_t b[],
    const q2_30 inv_ln_base_q30,
    const unsigned length);


/**
 * @brief Compute the natural logarithm of a vector of `float_s32_t`.
 * 
 * This function computes the natural logarithm of a vector @vector{b} of `float_s32_t` values. The 
 * result is written to output @vector{a}, a vector of Q8.24 values.
 * 
 * The resulting @math{a_k} for @math{b_k \le 0} is undefined.
 * 
 * @operation{
 * &    a_k \leftarrow  ln\left(b_k\right)   \\
 * &    \qquad\text{for }k \in \{0..\mathtt{length}-1\}
 * }
 * 
 * @param[out]  a         Output Q8.24 vector @vector{a}
 * @param[in]   b         Input vector @vector{b}
 * @param[in]   length    Number of elements in vectors @vector{a} and @vector{b}
 * 
 * @exception ET_LOAD_STORE Raised if `b` or `a` is not double word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_s32_api
 */
C_API
void vect_float_s32_log(
    q8_24 a[],
    const float_s32_t b[],
    const unsigned length);


/**
 * @brief Compute the base 2 logarithm of a vector of `float_s32_t`.
 * 
 * This function computes the base 2 logarithm of a vector @vector{b} of `float_s32_t` values. The 
 * result is written to output @vector{a}, a vector of Q8.24 values.
 * 
 * The resulting @math{a_k} for @math{b_k \le 0} is undefined.
 * 
 * @operation{
 * &    a_k \leftarrow  log_2\left(b_k\right)   \\
 * &    \qquad\text{for }k \in \{0..\mathtt{length}-1\}
 * }
 * 
 * @param[out]  a         Output Q8.24 vector @vector{a}
 * @param[in]   b         Input vector @vector{b}
 * @param[in]   length    Number of elements in vectors @vector{a} and @vector{b}
 * 
 * @exception ET_LOAD_STORE Raised if `b` or `a` is not double word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_s32_api
 */
C_API
void vect_float_s32_log2(
    q8_24 a[],
    const float_s32_t b[],
    const unsigned length);


/**
 * @brief Compute the base 10 logarithm of a vector of `float_s32_t`.
 * 
 * This function computes the base 10 logarithm of a vector @vector{b} of `float_s32_t` values. The 
 * result is written to output @vector{a}, a vector of Q8.24 values.
 * 
 * The resulting @math{a_k} for @math{b_k \le 0} is undefined.
 * 
 * @operation{
 * &    a_k \leftarrow  log_{10}\left(b_k\right)   \\
 * &    \qquad\text{for }k \in \{0..\mathtt{length}-1\}
 * }
 * 
 * @param[out]  a         Output Q8.24 vector @vector{a}
 * @param[in]   b         Input vector @vector{b}
 * @param[in]   length    Number of elements in vectors @vector{a} and @vector{b}
 * 
 * @exception ET_LOAD_STORE Raised if `b` or `a` is not double word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_s32_api
 */
C_API
void vect_float_s32_log10(
    q8_24 a[],
    const float_s32_t b[],
    const unsigned length);


/**
 * @brief Compute the logarithm (in the specified base) of a block floating-point vector.
 * 
 * This function computes the logarithm of the block floating-point vector 
 * @math{\bar{b}\cdot 2^{b\_exp}}. The base of the computed logarithm is given by parameter 
 * `inv_ln_base_q30`. The result is written to output @vector{a}, a vector of Q8.24 values.
 * 
 * If the desired base is @math{D}, then `inv_ln_base_q30`, represented here by @math{R}, should be 
 * @math{\mathtt{Q30}\left(\frac{1}{ln\left(D\right)}\right)}. That is: the inverse of the natural
 * logarithm of the desired base, expressed as a Q2.30 value. Typically the desired base is known
 * at compile time, so this value will usually be a precomputed constant.
 * 
 * The resulting @math{a_k} for @math{b_k \le 0} is undefined.
 * 
 * @operation{
 * &    a_k \leftarrow  log_{D}\left(b_k\cdot 2^{b\_exp}\right)   \\
 * &    \qquad\text{for }k \in \{0..\mathtt{length}-1\}
 * }
 * 
 * @param[out]  a         Output Q8.24 vector @vector{a}
 * @param[in]   b         Input mantissa vector @vector{b}
 * @param[in]   b_exp     Exponent associated with @vector{b}
 * @param[in]   inv_ln_base_q30   Coefficient @math{R} converting from natural log to desired base @math{D}
 * @param[in]   length    Number of elements in vectors @vector{a} and @vector{b}
 * 
 * @exception ET_LOAD_STORE Raised if `b` or `a` is not double word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_s32_api
 */
C_API
void vect_s32_log_base(
    q8_24 a[],
    const int32_t b[],
    const exponent_t b_exp,
    const q2_30 inv_ln_base_q30,
    const unsigned length);


/**
 * @brief Compute the natural logarithm of a block floating-point vector.
 * 
 * This function computes the natural logarithm of the block floating-point vector 
 * @math{\bar{b}\cdot 2^{b\_exp}}. The result is written to output @vector{a}, a vector of Q8.24 
 * values.
 * 
 * The resulting @math{a_k} for @math{b_k \le 0} is undefined.
 * 
 * @operation{
 * &    a_k \leftarrow  ln\left(b_k\cdot 2^{b\_exp}\right)   \\
 * &    \qquad\text{for }k \in \{0..\mathtt{length}-1\}
 * }
 * 
 * @param[out]  a         Output Q8.24 vector @vector{a}
 * @param[in]   b         Input mantissa vector @vector{b}
 * @param[in]   b_exp     Exponent associated with @vector{b}
 * @param[in]   length    Number of elements in vectors @vector{a} and @vector{b}
 * 
 * @exception ET_LOAD_STORE Raised if `b` or `a` is not double word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_s32_api
 */
C_API
void vect_s32_log(
    q8_24 a[],
    const int32_t b[],
    const exponent_t b_exp,
    const unsigned length);


/**
 * @brief Compute the base 2 logarithm of a block floating-point vector.
 * 
 * This function computes the base 2 logarithm of the block floating-point vector 
 * @math{\bar{b}\cdot 2^{b\_exp}}. The result is written to output @vector{a}, a vector of Q8.24 
 * values.
 * 
 * The resulting @math{a_k} for @math{b_k \le 0} is undefined.
 * 
 * @operation{
 * &    a_k \leftarrow  log_2\left(b_k\cdot 2^{b\_exp}\right)   \\
 * &    \qquad\text{for }k \in \{0..\mathtt{length}-1\}
 * }
 * 
 * @param[out]  a         Output Q8.24 vector @vector{a}
 * @param[in]   b         Input mantissa vector @vector{b}
 * @param[in]   b_exp     Exponent associated with @vector{b}
 * @param[in]   length    Number of elements in vectors @vector{a} and @vector{b}
 * 
 * @exception ET_LOAD_STORE Raised if `b` or `a` is not double word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_s32_api
 */
C_API
void vect_s32_log2(
    q8_24 a[],
    const int32_t b[],
    const exponent_t b_exp,
    const unsigned length);


/**
 * @brief Compute the base 10 logarithm of a block floating-point vector.
 * 
 * This function computes the base 10 logarithm of the block floating-point vector 
 * @math{\bar{b}\cdot 2^{b\_exp}}. The result is written to output @vector{a}, a vector of Q8.24 
 * values.
 * 
 * The resulting @math{a_k} for @math{b_k \le 0} is undefined.
 * 
 * @operation{
 * &    a_k \leftarrow  log_{10}\left(b_k\cdot 2^{b\_exp}\right)   \\
 * &    \qquad\text{for }k \in \{0..\mathtt{length}-1\}
 * }
 * 
 * @param[out]  a         Output Q8.24 vector @vector{a}
 * @param[in]   b         Input mantissa vector @vector{b}
 * @param[in]   b_exp     Exponent associated with @vector{b}
 * @param[in]   length    Number of elements in vectors @vector{a} and @vector{b}
 * 
 * @exception ET_LOAD_STORE Raised if `b` or `a` is not double word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_s32_api
 */
C_API
void vect_s32_log10(
    q8_24 a[],
    const int32_t b[],
    const exponent_t b_exp,
    const unsigned length);


/**
 * @brief Compute @math{e^x} for Q2.30 value near @math{0}.
 * 
 * This function computes @math{e^{b_k \cdot 2^{-30}}} for each @math{b_k} in input vector 
 * @vector{b}. The results are placed in output vector @vector{a} as Q2.30 values.
 * 
 * This function is meant to compute @math{e^x} for values of @math{x} in the interval 
 * @math{ \left[-0.5, 0.5\right] }. The error grows quickly outside of this range.
 * 
 * @operation{
 * &  a_k \leftarrow  \frac{ e^{b_k \cdot 2^{-30}} }{ 2^{30} } \\
 * &      \qquad\text{for }k \in \{0..(\mathtt{length}-1)\}
 * }
 * 
 * @param[out]  a       Output vector @vector{a}
 * @param[in]   b       Input vector @vector{b}
 * @param[in]   length  Number of elements in vectors @vector{a} and @vector{b}
 * 
 * @ingroup vect_s32_api
 */
C_API
void vect_q30_exp_small(
    q2_30 a[],
    const q2_30 b[],
    const unsigned length);


/** 
 * @brief Convert a 32-bit vector to a 16-bit vector.
 * 
 * This function converts a 32-bit mantissa vector @vector{b} into a 16-bit mantissa vector
 * @vector{a}. Conceptually, the output BFP vector @math{\bar{a}\cdot 2^{a\_exp}} represents the
 * same values as the input BFP vector @math{\bar{b}\cdot 2^{b\_exp}}, only with a reduced 
 * bit-depth.
 *
 * In most cases @math{b\_shr} should be @math{16 - b\_hr}, where @math{b\_hr} is the headroom of
 * the 32-bit input mantissa vector @vector{b}. 
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
 * @endparblock
 * 
 * @operation{
 * &     a_k \leftarrow sat_{16}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)      \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the 32-bit mantissas of a BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the
 * resulting vector @vector{a} are the 16-bit mantissas of BFP vector @math{\bar{a} \cdot
 * 2^{a\_exp}}, where @math{a\_exp = b\_exp + b\_shr}.
 * @endparblock
 * 
 * @param[out]  a        Output vector @vector{a}
 * @param[in]   b        Input vector @vector{b}
 * @param[in]   length   Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]   b_shr    Right-shift appled to @vector{b}
 * 
 * @see vect_s16_to_vect_s32
 * 
 * @exception ET_LOAD_STORE Raised if `a` or `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_s32_api
 */
C_API
void vect_s32_to_vect_s16(
    int16_t a[],
    const int32_t b[],
    const unsigned length,
    const right_shift_t b_shr);


/** 
 * @brief Convert a 32-bit BFP vector into a vector of IEEE754 single-precision floats.
 * 
 * This function converts a 32-bit mantissa vector and exponent @math{\bar b \cdot 2^{b\_exp}} 
 * into a vector of 32-bit IEEE754 single-precision floating-point elements @vector{a}. 
 * Conceptually, the elements of output vector @vector{a} represent the same values as those of
 * the input vector.
 * 
 * Because IEEE754 single-precision floats hold fewer mantissa bits, this operation may result in
 * a loss of precision for some elements.
 * 
 * @operation{ 
 * &     a_k \leftarrow b_k \cdot 2^{b\_exp}     \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * }
 * 
 * @par Parameter Details
 * @parblock
 * 
 * `a[]` represents the output IEEE754 float vector @vector{a}.
 * 
 * `b[]` represents the 32-bit input mantissa vector @vector{b}.
 * 
 * `a[]` and `b[]` must each begin at a double-word-aligned address.
 * 
 * `b[]` can be safely updated in-place.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_exp` is the exponent associated with the input vector @vector{b}.
 * @endparblock
 * 
 * @param[out]  a        Output vector @vector{a}
 * @param[in]   b        Input vector @vector{b}
 * @param[in]   length   Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]   b_exp    Exponent @math{b\_exp} of input vector @vector{b}
 * 
 * @see vect_f32_to_vect_s32
 * 
 * @exception ET_LOAD_STORE Raised if `a` or `b` is not double-word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_f32_api
 */
C_API
void vect_s32_to_vect_f32(
    float a[],
    const int32_t b[], 
    const unsigned length, 
    const exponent_t b_exp);


#ifdef __XC__
}   //extern "C"
#endif

