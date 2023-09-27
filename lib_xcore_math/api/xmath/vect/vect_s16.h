// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include "xmath/xmath_conf.h"
#include "xmath/types.h"
#include "xmath/util.h"

#include "xmath/vect/vect_s16_prepare.h"

/**
 * @defgroup vect_s16_api   16-bit Vector Functions
 */

#ifdef __XC__
extern "C" {
#endif


/**
 * Maximum bit-depth that can be calculated by vect_s16_sqrt().
 * 
 * @sa vect_s16_sqrt
 */
#define VECT_SQRT_S16_MAX_DEPTH     (15)


/** 
 * @brief Compute the element-wise absolute value of a 16-bit vector.
 * 
 * `a[]` and `b[]` represent the 16-bit vectors @vector{a} and @vector{b} respectively. Each must
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
 * @exception ET_LOAD_STORE Raised if `a` or `b` is not word-aligned (See @ref
 *            note_vector_alignment)
 * 
 * @ingroup vect_s16_api
 */
C_API
headroom_t vect_s16_abs(
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
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the returned
 * value @math{a} is the 32-bit mantissa of floating-point value @math{a \cdot 2^{a\_exp}}, where
 * @math{a\_exp = b\_exp}.
 * @endparblock
 * 
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   length      Number of elements in @vector{b}
 * 
 * @returns The 32-bit sum @math{a}
 * 
 * @exception ET_LOAD_STORE Raised if `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_s16_api
 */
C_API
int32_t vect_s16_abs_sum(
    const int16_t b[],
    const unsigned length);


/**
 * @brief Add one 16-bit BFP vector to another.
 * 
 * `a[]`, `b[]` and `c[]` represent the 16-bit vectors @vector{a}, @vector{b} and @vector{c}
 * respectively. Each must begin at a word-aligned address. This operation can be performed safely
 * in-place on `b[]` or `c[]`.
 *
 * `length` is the number of elements in each of the vectors.
 *
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b}
 * and @vector{c} respectively.
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
 * If @vector{b} and @vector{c} are the mantissas of BFP vectors @math{ \bar{b} \cdot 2^{b\_exp} }
 * and @math{\bar{c} \cdot 2^{c\_exp}}, then the resulting vector @vector{a} are the mantissas of
 * BFP vector @math{\bar{a} \cdot 2^{a\_exp}}. 
 *
 * In this case, @math{b\_shr} and @math{c\_shr} **must** be chosen so that 
 * @math{a\_exp = b\_exp + b\_shr = c\_exp + c\_shr}. Adding or subtracting mantissas only makes
 * sense if they are associated with the same exponent.
 *
 * The function vect_s16_add_prepare() can be used to obtain values for @math{a\_exp}, @math{b\_shr}
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
 * @exception ET_LOAD_STORE Raised if `a`, `b` or `c` is not word-aligned (See @ref
 *            note_vector_alignment)
 * 
 * @see vect_s16_add_prepare
 * 
 * @ingroup vect_s16_api
 */
C_API
headroom_t vect_s16_add(
    int16_t a[],
    const int16_t b[], 
    const int16_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);



/**
 * @brief Add a scalar to a 16-bit vector.
 * 
 * `a[]`, `b[]` represent the 16-bit mantissa vectors @vector{a} and @vector{b} respectively. Each
 * must begin at a word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `c` is the scalar @math{c} to be added to each element of @vector{b}.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` is the signed arithmetic right-shifts applied to each element of @vector{b}.
 * 
 * @operation{ 
 * &     b_k' = sat_{16}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)  \\
 * &     a_k \leftarrow sat_{16}\!\left( b_k' + c \right)        \\
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
 * The function vect_s16_add_scalar_prepare() can be used to obtain values for @math{a\_exp},
 * @math{b\_shr} and @math{c\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and
 * the input headrooms @math{b\_hr} and @math{c\_hr}.
 *
 * Note that @math{c\_shr} is an output of `vect_s16_add_scalar_prepare()`, but is not a
 * parameter to this function. The @math{c\_shr} produced by `vect_s16_add_scalar_prepare()` is
 * to be applied by the user, and the result passed as input `c`.
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
 * @see vect_s16_add_scalar_prepare()
 * 
 * @ingroup vect_s16_api
 */
C_API
headroom_t vect_s16_add_scalar(
    int16_t a[],
    const int16_t b[],
    const int16_t c,
    const unsigned length,
    const right_shift_t b_shr);


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
 * @returns @math{a}, the index of the maximum element of vector @vector{b}. If there is a tie for
 *          the maximum value, the lowest tying index is returned.
 * 
 * @exception ET_LOAD_STORE Raised if `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_s16_api
 */
C_API
unsigned vect_s16_argmax(
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
 * @returns @math{a}, the index of the minimum element of vector @vector{b}. If there is a tie for
 *          the minimum value, the lowest tying index is returned.
 * 
 * @exception ET_LOAD_STORE Raised if `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_s16_api
 */
C_API
unsigned vect_s16_argmin(
    const int16_t b[], 
    const unsigned length);


/**
 * @brief Clamp the elements of a 16-bit vector to a specified range.
 * 
 * `a[]` and `b[]` represent the 16-bit vectors @vector{a} and @vector{b} respectively. Each must
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
 * &     b_k' \leftarrow sat_{16}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor) \\
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
 * @ingroup vect_s16_api
 */
C_API
headroom_t vect_s16_clip(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const int16_t lower_bound,
    const int16_t upper_bound,
    const right_shift_t b_shr);


/**
 * @brief Compute the inner product of two 16-bit vectors.
 * 
 * `b[]` and `c[]` represent the 32-bit vectors @vector{a} and @vector{b} respectively. Each must
 * begin at a word-aligned address.
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
 * If @vector{b} and @vector{c} are the mantissas of the BFP vectors 
 * @math{ \bar{b} \cdot 2^{b\_exp}} and @math{\bar{c}\cdot 2^{c\_exp}}, then result @math{a} is the
 * mantissa of the result @math{a \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp + c\_exp}.
 *
 * If needed, the bit-depth of @math{a} can then be reduced to 16 or 32 bits to get a new result 
 * @math{a' \cdot 2^{a\_exp'}} where @math{a' = a \cdot 2^{-a\_shr}} and @math{a\_exp' = a\_exp +
 * a\_shr}.
 * @endparblock
 * 
 * @par Notes
 * @parblock
 * 
 * The sum @math{a} is accumulated simultaneously into 16 48-bit accumulators which are summed
 * together at the final step. So long as `length` is less than roughly 2 million, no overflow or
 * saturation of the resulting sum is possible. 
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
 * @ingroup vect_s16_api
 */
C_API
int64_t vect_s16_dot(
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
 * `b_shr` is the signed arithmetic right-shift applied to elements of @vector{b}. `b_shr` should be
 * chosen to avoid the possibility of saturation. See the note below.
 * 
 * @operation{
 * &     b_k' \leftarrow sat_{16}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)         \\
 * &     a \leftarrow \sum_{k=0}^{length-1} (b_k')^2
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the mantissas of the BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then
 * floating-point result is @math{a \cdot 2^{a\_exp}}, where the 32-bit mantissa @math{a} is
 * returned by this function, and @math{a\_exp = 2 \cdot (b\_exp + b\_shr) }.
 * @endparblock
 * 
 * @par Additional Details
 * @parblock
 * 
 * If @vector{b} has @math{b\_hr} bits of headroom, then each product @math{(b_k')^2} can be a
 * maximum of @math{ 2^{30 - 2 \cdot (b\_hr + b\_shr)}}. So long as `length` is less than @math{1 +
 * 2\cdot (b\_hr + b\_shr) }, such errors should not be possible. Each increase of @math{b\_shr} by
 * @math{1} doubles the number of elements that can be summed without risk of overflow.
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
 * @ingroup vect_s16_api
 */
C_API
int32_t vect_s16_energy(
    const int16_t b[],
    const unsigned length,
    const right_shift_t b_shr);


/**
 * @brief Calculate the headroom of a 16-bit vector.
 * 
 * The headroom of an N-bit integer is the number of bits that the integer's value may be
 * left-shifted without any information being lost. Equivalently, it is one less than the number of
 * leading sign bits.
 *
 * The headroom of an `int16_t` array is the minimum of the headroom of each of its `int16_t`
 * elements.
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
 * @see vect_s32_headroom,  
 *      vect_complex_s16_headroom, 
 *      vect_complex_s32_headroom
 * 
 * @ingroup vect_s16_api
 */
C_API
headroom_t vect_s16_headroom(
    const int16_t b[], 
    const unsigned length);


/**
 * @brief Compute the inverse of elements of a 16-bit vector.
 * 
 * `a[]` and `b[]` represent the 16-bit mantissa vectors @vector{a} and @vector{b} respectively.
 * This operation can be performed safely in-place on `b[]`.
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
 * The function vect_s16_inverse_prepare() can be used to obtain values for @math{a\_exp} and
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
 * @see vect_s16_inverse_prepare
 * 
 * @ingroup vect_s16_api
 */
C_API
void vect_s16_inverse(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const unsigned scale);


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
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the returned
 * value @math{a} is the 16-bit mantissa of floating-point value @math{a \cdot 2^{a\_exp}}, where
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
 * @ingroup vect_s16_api
 */
C_API
int16_t vect_s16_max(
    const int16_t b[], 
    const unsigned length);


/**
 * @brief Get the element-wise maximum of two 16-bit vectors.
 * 
 * `a[]`, `b[]` and `c[]` represent the 16-bit mantissa vectors @vector{a}, @vector{b} and
 * @vector{c} respectively. Each must begin at a word-aligned address. This operation can be
 * performed safely in-place on `b[]`, but @a not on `c[]`.
 *
 * `length` is the number of elements in each of the vectors.
 *
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b}
 * and @vector{c} respectively.
 * 
 * @operation{
 * &     b_k' \leftarrow sat_{16}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
 * &     c_k' \leftarrow sat_{16}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)     \\
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
 * @ingroup vect_s16_api
 */
C_API
headroom_t vect_s16_max_elementwise(
    int16_t a[],
    const int16_t b[],
    const int16_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


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
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the returned
 * value @math{a} is the 16-bit mantissa of floating-point value @math{a \cdot 2^{a\_exp}}, where
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
 * @ingroup vect_s16_api
 */
C_API
int16_t vect_s16_min(
    const int16_t b[], 
    const unsigned length);


/**
 * @brief Get the element-wise minimum of two 16-bit vectors.
 * 
 * `a[]`, `b[]` and `c[]` represent the 16-bit mantissa vectors @vector{a},
 * @vector{b} and @vector{c} respectively. Each must begin at a word-aligned
 * address. This operation can be performed safely in-place on `b[]`, but @a not
 * on `c[]`.
 *
 * `length` is the number of elements in each of the vectors.
 *
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each
 * element of @vector{b} and @vector{c} respectively.
 * 
 * @operation{
 * &     b_k' \leftarrow sat_{16}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
 * &     c_k' \leftarrow sat_{16}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)     \\
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
 * @ingroup vect_s16_api
 */
C_API
headroom_t vect_s16_min_elementwise(
    int16_t a[],
    const int16_t b[],
    const int16_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


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
 * @param[in]     bc_sat    Unsigned arithmetic right-shift applied to the products of elements
 *                          @math{b_k} and @math{c_k}
 *
 * @returns   Headroom of the output vector @vector{a}
 *
 * @exception ET_LOAD_STORE Raised if `acc`, `b` or `c` is not word-aligned (See @ref note_vector_alignment)
 *
 * @see vect_s16_macc_prepare
 *
 * @ingroup vect_s16_api
 */
C_API
headroom_t vect_s16_macc(
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
 * The function vect_complex_s16_nmacc_prepare() can be used to obtain values for @math{a\_exp},
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
 * @see vect_s16_nmacc_prepare
 *
 * @ingroup vect_s16_api
 */
C_API
headroom_t vect_s16_nmacc(
    int16_t acc[],
    const int16_t b[],
    const int16_t c[],
    const unsigned length,
    const right_shift_t acc_shr,
    const right_shift_t bc_sat);


/**
 * @brief Multiply two 16-bit vectors together element-wise.
 *
 * `a[]`, `b[]` and `c[]` represent the 16-bit vectors @vector{a}, @vector{b} and @vector{c}
 * respectively. Each must begin at a word-aligned address. This operation can be performed safely
 * in-place on `b[]` or `c[]`.
 *
 * `length` is the number of elements in each of the vectors.
 *
 * `a_shr` is an unsigned arithmetic right-shift applied to the 32-bit accumulators holding the
 * penultimate results.
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
 * If @vector{b} and @vector{c} are the mantissas of BFP vectors @math{ \bar{b} \cdot 2^{b\_exp} }
 * and @math{\bar{c} \cdot 2^{c\_exp}}, then the resulting vector @vector{a} are the mantissas of
 * BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where @math{a\_exp = b\_exp + c\_exp + a\_shr}.
 *
 * The function vect_s16_mul_prepare() can be used to obtain values for @math{a\_exp} and
 * @math{a\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and the input
 * headrooms @math{b\_hr} and @math{c\_hr}. 
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
 * @ingroup vect_s16_api
 */
C_API
headroom_t vect_s16_mul(
    int16_t a[],
    const int16_t b[],
    const int16_t c[],
    const unsigned length,
    const right_shift_t a_shr);


/** 
 * @brief Rectify the elements of a 16-bit vector.
 * 
 * Rectification ensures that all outputs are non-negative, changing negative values to 0.
 *
 * `a[]` and `b[]` represent the 16-bit vectors @vector{a} and @vector{b} respectively. Each must
 * begin at a word-aligned address. This operation can be performed safely in-place on `b[]`.
 *
 * `length` is the number of elements in each of the vectors.
 *
 *
 * Each output element `a[k]` is set to the value of the corresponding input element `b[k]` if it is
 * positive, and `a[k]` is set to zero otherwise. 
 * 
 * @operation{
 * &     a_k \leftarrow \begin{cases}
 *          b_k & b_k > 0             \\ 
 *          0 & b_k \leq 0\end{cases}   \\
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
 * @ingroup vect_s16_api
 */
C_API
headroom_t vect_s16_rect(
    int16_t a[],
    const int16_t b[],
    const unsigned length);


/**
 * @brief Multiply a 16-bit vector by a 16-bit scalar.
 * 
 * `a[]` and `b[]` represent the 16-bit vectors @vector{a} and @vector{b} respectively. Each must
 *  begin at a word-aligned address. This operation can be performed safely in-place on `b[]`.
 *
 * `length` is the number of elements in each of the vectors.
 *
 * `c` is the 16-bit scalar @math{c} by which elements of @vector{b} are multiplied.
 *
 * `a_shr` is an unsigned arithmetic right-shift applied to the 32-bit accumulators holding the
 * penultimate results.
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
 * If @vector{b} are the mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} } and @math{c} is
 * the mantissa of floating-point value @math{c \cdot 2^{c\_exp}}, then the resulting vector
 * @vector{a} are the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where 
 * @math{a\_exp = b\_exp + c\_exp + a\_shr}.
 *
 * The function vect_s16_scale_prepare() can be used to obtain values for @math{a\_exp} and
 * @math{a\_shr} based on the input exponents @math{b\_exp} and @math{c\_exp} and the input
 * headrooms @math{b\_hr} and @math{c\_hr}. 
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
 * @ingroup vect_s16_api
 */
C_API
headroom_t vect_s16_scale(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const int16_t c,
    const right_shift_t a_shr);


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
 * If @math{b} is the mantissa of floating-point value @math{b \cdot 2^{b\_exp}}, then the output
 * vector @vector{a} are the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where
 * @math{a\_exp = b\_exp}.
 * @endparblock
 * 
 * @param[out] a        Output vector @vector{a}
 * @param[in]  b        Input value @math{b}
 * @param[in]  length   Number of elements in vector @vector{a}
 * 
 * @exception ET_LOAD_STORE Raised if `a` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_s16_api
 */
C_API
void vect_s16_set(
    int16_t a[],
    const int16_t b,
    const unsigned length);


/**
 * @brief Left-shift the elements of a 16-bit vector by a specified number of bits.
 * 
 * `a[]` and `b[]` represent the 16-bit vectors @vector{a} and @vector{b} respectively. Each must
 * begin at a word-aligned address. This operation can be performed safely in-place on `b[]`.
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
 * If @vector{b} are the mantissas of a BFP vector @math{ \bar{b} \cdot 2^{b\_exp} }, then the
 * resulting vector @vector{a} are the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where 
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
 * @ingroup vect_s16_api
 */
C_API
headroom_t vect_s16_shl(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const left_shift_t b_shl);


/**
 * @brief Right-shift the elements of a 16-bit vector by a specified number of bits.
 * 
 * `a[]` and `b[]` represent the 16-bit vectors @vector{a} and @vector{b} respectively. Each must
 * begin at a word-aligned address. This operation can be performed safely in-place on `b[]`.
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
 * @ingroup vect_s16_api
 */
C_API
headroom_t vect_s16_shr(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const right_shift_t b_shr);


/**
 * @brief Compute the square roots of elements of a 16-bit vector.
 * 
 * `a[]` and `b[]` represent the 16-bit vectors @vector{a} and @vector{b} respectively. Each vector
 * must begin at a word-aligned address. This operation can be performed safely in-place on `b[]`.
 *
 * `length` is the number of elements in each of the vectors.
 *
 * `b_shr` is the signed arithmetic right-shift applied to elements of @vector{b}.
 *
 * `depth` is the number of most significant bits to calculate of each @math{a_k}. For example, a
 * `depth` value of 8 will only compute the 8 most significant byte of the result, with the
 * remaining byte as 0. The maximum value for this parameter is `VECT_SQRT_S16_MAX_DEPTH` (31). The
 * time cost of this operation is approximately proportional to the number of bits computed.
 * 
 * @operation{ 
 * &     b_k' \leftarrow sat_{16}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
 * &     a_k \leftarrow \begin{cases}
 *           \sqrt{ b_k' } & b_k' >= 0                                       \\
 *           0   &   otherwise\end{cases}                                    \\
 * &         \qquad\text{ for }k\in 0\ ...\ (length-1)                       \\
 * &         \qquad\text{ where } \sqrt{\cdot} \text{ computes the most significant } depth 
 *                       \text{ bits of the square root.}
 * }
 * 
 * @par Block Floating-Point
 * @parblock
 * 
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the resulting
 * vector @vector{a} are the mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}, where
 * @math{a\_exp = (b\_exp + b\_shr - 14)/2}.
 *
 * Note that because exponents must be integers, that means @math{b\_exp + b\_shr} **must be even**.
 *
 * The function vect_s16_sqrt_prepare() can be used to obtain values for @math{a\_exp} and
 * @math{b\_shr} based on the input exponent @math{b\_exp} and headroom @math{b\_hr}.
 * @endparblock
 * 
 * @par Notes
 * @parblock
 * * This function assumes roots are real. Negative input elements will result in corresponding
 *   outputs of 0.
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
 * @ingroup vect_s16_api
 */
C_API
headroom_t vect_s16_sqrt(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const right_shift_t b_shr,
    const unsigned depth);


/**
 * @brief Subtract one 16-bit BFP vector from another.
 * 
 * `a[]`, `b[]` and `c[]` represent the 16-bit vectors @vector{a}, @vector{b} and @vector{c}
 * respectively. Each must begin at a word-aligned address. This operation can be performed safely
 * in-place on `b[]` or `c[]`.
 *
 * `length` is the number of elements in each of the vectors.
 *
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b}
 * and @vector{c} respectively.
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
 * If @vector{b} and @vector{c} are the mantissas of BFP vectors @math{ \bar{b} \cdot 2^{b\_exp} }
 * and @math{\bar{c} \cdot 2^{c\_exp}}, then the resulting vector @vector{a} are the mantissas of
 * BFP vector @math{\bar{a} \cdot 2^{a\_exp}}. 
 *
 * In this case, @math{b\_shr} and @math{c\_shr} **must** be chosen so that 
 * @math{a\_exp = b\_exp + b\_shr = c\_exp + c\_shr}. Adding or subtracting mantissas only makes
 * sense if they are associated with the same exponent.
 *
 * The function vect_s16_sub_prepare() can be used to obtain values for @math{a\_exp}, @math{b\_shr}
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
 * @see vect_s16_sub_prepare
 * 
 * @ingroup vect_s16_api
 */
C_API
headroom_t vect_s16_sub(
    int16_t a[],
    const int16_t b[],
    const int16_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


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
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the returned
 * value @math{a} is the 32-bit mantissa of floating-point value @math{a \cdot 2^{a\_exp}}, where
 * @math{a\_exp = b\_exp}.
 * @endparblock
 * 
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   length      Number of elements in @vector{b}
 * 
 * @returns The 32-bit sum @math{a}
 * 
 * @exception ET_LOAD_STORE Raised if `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_s16_api
 */
C_API
int32_t vect_s16_sum(
    const int16_t b[],
    const unsigned length);


/**
 * @brief VPU Control Register initialization value.
 * 
 * This value is used with chunk_s16_accumulate(). It is the value `vpu_ctrl` should be
 * initialized to.
 * 
 * Setting the VPU control register to this value puts it in 16-bit mode and clears the headroom
 * register.
 */
#define VPU_INT16_CTRL_INIT   0x0100

/**
 * @brief Derive 16-bit headroom from VPU control register value.
 * 
 * Used in conjunction with `chunk_s16_accumulate()` to compute final headroom.
 */
#define VPU_INT16_HEADROOM_FROM_CTRL(X)   ((15)-(X & 0x1F))

/**
 * @brief Accumulate a 16-bit vector chunk into a 32-bit accumulator chunk.
 * 
 * 16-bit vector chunk @vector{b} is shifted and accumulated into 32-bit accumulator vector chunk
 * @vector{a} (`acc`). This function is used for efficiently accumulating multiple (possibly 
 * many) 16-bit vectors together. 
 * 
 * The accumulator vector @vector{a} stores its elements across two 16-bit vector chunks, which
 * corresponds to how the accumulators are stored internally across VPU registers `vD` and `vR`.
 * See @ref split_acc_s32_t for details about the accumulator structure.
 * 
 * The signed arithmetic right-shift `b_shr` is applied to @vector{b} prior to being 
 * accumulated into @vector{a}. When @vector{b} and @vector{a}, are the mantissas of block floating
 * point vectors, using `b_shr` allows those vectors to have different exponents. This is also 
 * important when this function is to be called periodically where each @vector{b} may have a 
 * different exponent.
 * 
 * `b_shr` must meet the condition `-14 <= b_shr <= 14` or the behavior of this function is 
 * undefined.
 * 
 * @operation{
 * &  a_k \leftarrow  a_k  +  floor( \frac{b_k}{2^{-\mathtt{b\_shr}}} )
 * }
 * 
 * @par VPU Control Value
 * 
 * The input `vpu_ctrl` tracks the VPU's control register state during accumulation. In particular,
 * it is used for keeping track of the headroom of the accumulator vector @vector{a}. When beginning
 * a sequence of accumulation calls, the value passed in should be initialized to
 * `VPU_INT16_CTRL_INIT`. On completion, this function returns the updated VPU control register
 * state, which should be passed in as `vpu_ctrl` on the next accumulation call.
 * 
 * The idea is that each call to this function processes only a single 'chunk' (in 16-bit mode, a
 * 16-element block) at a time, but the caller usually wants to know the headroom of a whole vector,
 * which may comprise many such chunks. So `vpu_ctrl` is a value which persists through each of 
 * these calls to track the whole vector.
 * 
 * Once all chunks have been accumulated, the `VPU_INT16_HEADROOM_FROM_CTRL()` macro can be used to
 * get the headroom of the accumulator vector. Note that this will produce a maximum value of `15`.
 * 
 * @par Accumulating Many Values
 * 
 * If many vector chunks @vector{b} are accumulated into the same accumulators (when using block
 * floating-point, it may be only a few accumulations if the exponent associated with @vector{b} is
 * significantly larger than that associated with @vector{a}), saturation becomes possible.
 * 
 * When saturation is possible, the user must monitor the headroom of @vector{a} (using the returned
 * value and `VPU_INT16_HEADROOM_FROM_CTRL()`) to detect when there is no further headroom. As long
 * as there is at least 1 bit of headroom, a call to this function cannot saturate.
 * 
 * Typically, when using block floating-point, this will be handled by:
 * 
 * * Converting @vector{a} to a standard vector of `int32_t` using vect_s32_merge_accs()
 * * Right-shift the values of @vector{a} using vect_s32_shr()
 * * Increment the exponent associated with @vector{a} by the same amount right-shifted
 * * Convert @vector{a} back into the split accumulator format using vect_s32_split_accs()
 * 
 * When accumulating, setting `b_shr` to the exponent associated with @vector{b} minus the exponent
 * associated with @vector{a} will automatically adjust for the new exponent of @vector{a}.
 * 
 * @param[inout]  acc       b
 * @param[in]     b         v
 * @param[in]     b_shr     v
 * @param[in]     vpu_ctrl  e
 * 
 * @returns Current state of VPU control register.
 * 
 * @exception ET_LOAD_STORE Raised if `acc` or `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_s16_api
 */
C_API
unsigned chunk_s16_accumulate(
    split_acc_s32_t* acc,
    const int16_t b[VPU_INT16_EPV],
    const right_shift_t b_shr,
    const unsigned vpu_ctrl);
    


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
 * If @vector{b} are the mantissas of BFP vector @math{\bar{b} \cdot 2^{b\_exp}}, then the resulting
 * vector @vector{a} are the 32-bit mantissas of BFP vector @math{\bar{a} \cdot 2^{a\_exp}}. If
 * @math{a\_exp = b\_exp - 8}, then this operation has effectively not changed the values
 * represented.
 * @endparblock
 * 
 * @par Notes
 * @parblock
 * * The multiplication by @math{2^8} is an artifact of the VPU's behavior. It turns out to be
 *   significantly more efficient to include the factor of @math{2^8}. If this is unwanted,
 *   vect_s32_shr() can be used with a `b_shr` value of 8 to remove the scaling afterwards.
 * * The headroom of output vector @vector{a} is not returned by this function. The headroom of the
 *   output is always 8 bits greater than the headroom of the input.
 * @endparblock
 * 
 * @param[out]  a           32-bit output vector @vector{a}
 * @param[in]   b           16-bit input vector @vector{b}
 * @param[in]   length      Number of elements in vectors @vector{a} and @vector{b}
 * 
 * @exception ET_LOAD_STORE Raised if `a` or `b` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_s16_api
 */
C_API
void vect_s16_to_vect_s32(
    int32_t a[],
    const int16_t b[],
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
 * @see vect_s16_extract_low_byte
 * 
 * @ingroup vect_s16_api
 */
C_API
void vect_s16_extract_high_byte(
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
 * @see vect_s16_extract_high_byte
 * 
 * @ingroup vect_s16_api
 */
C_API
void vect_s16_extract_low_byte(
    int8_t a[],
    const int16_t b[],
    const unsigned len);

#ifdef __XC__
}   //extern "C"
#endif

