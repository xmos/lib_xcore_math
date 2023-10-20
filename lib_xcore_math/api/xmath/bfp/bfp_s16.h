// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include "xmath/types.h"

/**
 * @defgroup bfp_s16_api    16-bit Block Floating-Point API
 */

#ifdef __XC__
extern "C" {
#endif


/** 
 * @brief Initialize a 16-bit BFP vector.
 * 
 * This function initializes each of the fields of BFP vector `a`.
 *
 * `data` points to the memory buffer used to store elements of the vector, so it must be at least
 * `length * 2` bytes long, and must begin at a word-aligned address.
 *
 * `exp` is the exponent assigned to the BFP vector. The logical value associated with the `k`th
 * element of the vector after initialization is @math{ data_k \cdot 2^{exp} }.
 *
 * If `calc_hr` is false, `a->hr` is initialized to 0. Otherwise, the headroom of the the BFP vector
 * is calculated and used to initialize `a->hr`.
 * 
 * @param[out] a        BFP vector to initialize
 * @param[in] data      `int16_t` buffer used to back `a`
 * @param[in] exp       Exponent of BFP vector
 * @param[in] length    Number of elements in the BFP vector
 * @param[in] calc_hr   Boolean indicating whether the HR of the BFP vector should be calculated
 * 
 * @ingroup bfp_s16_api
 */
C_API
void bfp_s16_init(
    bfp_s16_t* a, 
    int16_t* data, 
    const exponent_t exp, 
    const unsigned length,
    const unsigned calc_hr);
    

/**
 * @brief Dynamically allocate a 16-bit BFP vector from the heap.
 * 
 * If allocation was unsuccessful, the `data` field of the returned vector will be NULL, and the
 * `length` field will be zero.  Otherwise, `data` will point to the allocated memory and the
 * `length` field will be the user-specified length. The `length` argument must not be zero.
 *
 * Neither the BFP exponent, headroom, nor the elements of the allocated mantissa vector are set by
 * this function. To set the BFP vector elements to a known value, use bfp_s16_set() on the retuned
 * BFP vector.
 *
 * BFP vectors allocated using this function must be deallocated using bfp_s16_dealloc() to avoid a
 * memory leak.
 *
 * To initialize a BFP vector using static memory allocation, use bfp_s16_init() instead.
 * 
 * @note Dynamic allocation of BFP vectors relies on allocation from the heap, and offers no
 *       guarantees about the execution time.  Use of this function in any time-critical 
 *       section of code is highly discouraged.
 * 
 * @param[in] length  The length of the BFP vector to be allocated (in elements)
 * 
 * @returns 16-bit BFP vector
 * 
 * @see bfp_s16_dealloc,
 *      bfp_s16_init
 * 
 * @ingroup bfp_s16_api
 */
C_API
bfp_s16_t bfp_s16_alloc(
    const unsigned length);


/**
 * @brief Deallocate a 16-bit BFP vector allocated by bfp_s16_alloc().
 * 
 * Use this function to free the heap memory allocated by bfp_s16_alloc().
 *
 * BFP vectors whose mantissa buffer was (successfully) dynamically allocated have a flag set which
 * indicates as much.  This function can safely be called on any bfp_s16_t which has not had its
 * `flags` or `data` manually manipulated, including:
 * * bfp_s16_t resulting from a successful call to bfp_s16_alloc()
 * * bfp_s16_t resulting from an unsuccessful call to bfp_s16_alloc()
 * * bfp_s16_t initialized with a call to bfp_s16_init()
 *
 * In the latter two cases, this function does nothing. In the former, the `data`, `length` and
 * `flags` fields of `vector` are cleared to zero.
 * 
 * @param[in] vector  BFP vector to be deallocated.
 * 
 * @see bfp_s16_alloc
 * 
 * @ingroup bfp_s16_api
 */
C_API
void bfp_s16_dealloc(
    bfp_s16_t* vector);


/**
 * @brief Set all elements of a 16-bit BFP vector to a specified value.
 * 
 * The exponent of `a` is set to `exp`, and each element's mantissa is set to `b`.
 * 
 * After performing this operation, all elements will represent the same value @math{b \cdot
 * 2^{exp}}.
 * 
 * `a` must have been initialized (see bfp_s16_init()).
 * 
 * @param[out] a         BFP vector to update
 * @param[in]  b         New value each mantissa is set to
 * @param[in]  exp       New exponent for the BFP vector
 * 
 * @ingroup bfp_s16_api
 */
C_API
void bfp_s16_set(
    bfp_s16_t* a,
    const int16_t b,
    const exponent_t exp);


/** 
 * @brief Get the headroom of a 16-bit BFP vector.
 *
 * The headroom of a vector is the number of bits its elements can be left-shifted without losing
 * any information. It conveys information about the range of values that vector may contain, which
 * is useful for determining how best to preserve precision in potentially lossy block
 * floating-point operations.
 *
 * In a BFP context, headroom applies to mantissas only, not exponents.
 *
 * In particular, if the 16-bit mantissa vector @vector{x} has @math{N} bits of headroom, then for
 * any element @math{x_k} of @vector{x}
 *
 * @math{-2^{15-N} \le x_k < 2^{15-N}}
 *
 * And for any element @math{X_k = x_k \cdot 2^{x\_exp}} of a complex BFP vector @vector{X}
 *
 * @math{-2^{15 + x\_exp - N} \le X_k < 2^{15 + x\_exp - N} }
 *
 * This function determines the headroom of `b`, updates `b->hr` with that value, and then returns
 * `b->hr`.
 *
 * @param   b         BFP vector to get the headroom of
 *
 * @returns    Headroom of BFP vector `b` 
 *
 * @ingroup bfp_s16_api
 */
C_API
headroom_t bfp_s16_headroom(
    bfp_s16_t* b);


/**
 * @brief Modify a 16-bit BFP vector to use a specified exponent.
 * 
 * This function forces BFP vector @vector{A} to use a specified exponent. The mantissa vector
 * @vector{a} will be bit-shifted left or right to compensate for the changed exponent.
 * 
 * This function can be used, for example, before calling a fixed-point arithmetic function to 
 * ensure the underlying mantissa vector has the needed Q-format. As another example, this may be
 * useful when communicating with peripheral devices (e.g. via I2S) that require sample data to
 * be in a specified format.
 * 
 * Note that this sets the _current_ encoding, and does not _fix_ the exponent permanently (i.e.
 * subsequent operations may change the exponent as usual).
 * 
 * If the required fixed-point Q-format is `QX.Y`, where `Y` is the number of fractional bits in the
 * resulting mantissas, then the associated exponent (and value for parameter `exp`) is `-Y`.
 * 
 * `a` points to input BFP vector @vector{A}, with mantissa vector @vector{a} and exponent
 * @math{a\_exp}. `a` is updated in place to produce resulting BFP vector @math{\bar{\tilde{A}}}
 * with mantissa vector @math{\bar{\tilde{a}}} and exponent @math{\tilde{a}\_exp}.
 * 
 * `exp` is @math{\tilde{a}\_exp}, the required exponent. @math{\Delta{}p = \tilde{a}\_exp - a\_exp}
 * is the required change in exponent.
 * 
 * If @math{\Delta{}p = 0}, the BFP vector is left unmodified.
 * 
 * If @math{\Delta{}p > 0}, the required exponent is larger than the current exponent and an
 * arithmetic right-shift of @math{\Delta{}p} bits is applied to the mantissas @vector{a}. When
 * applying a right-shift, precision may be lost by discarding the @math{\Delta{}p} least
 * significant bits.
 * 
 * If @math{\Delta{}p < 0}, the required exponent is smaller than the current exponent and a
 * left-shift of @math{\Delta{}p} bits is applied to the mantissas @vector{a}. When left-shifting,
 * saturation logic will be applied such that any element that can't be represented exactly with
 * the new exponent will saturate to the 16-bit saturation bounds.
 * 
 * The exponent and headroom of `a` are updated by this function.
 * 
 * @operation{
 * &    \Delta{}p = \tilde{a}\_exp - a\_exp \\
 * &    \tilde{a_k} \leftarrow sat_{16}( a_k \cdot 2^{-\Delta{}p} )   \\
 * &        \qquad\text{for } k \in 0\ ...\ (N-1)                     \\
 * &        \qquad\text{where } N \text{ is the length of } \bar{A} \text{ (in elements) }
 * }
 * 
 * @param[inout]  a     Input BFP vector @vector{A} / Output BFP vector @math{\bar{\tilde{A}}}
 * @param[in]     exp   The required exponent, @math{\tilde{a}\_exp}
 * 
 * @ingroup bfp_s16_api
 */
C_API
void bfp_s16_use_exponent(
    bfp_s16_t* a,
    const exponent_t exp);


/** 
 * @brief Apply a left-shift to the mantissas of a 16-bit BFP vector.
 * 
 * Each mantissa of input BFP vector @vector{B} is left-shifted `b_shl` bits and stored in the
 * corresponding element of output BFP vector @vector{A}.
 * 
 * This operation can be used to add or remove headroom from a BFP vector.
 * 
 * `b_shl` is the number of bits that each mantissa will be left-shifted. This shift is signed and
 * arithmetic, so negative values for `b_shl` will right-shift the mantissas.
 * 
 * `a` and `b` must have been initialized (see bfp_s16_init()), and must be the same length.
 * 
 * This operation can be performed safely in-place on `b`.
 * 
 * Note that this operation bypasses the logic protecting the caller from saturation or underflows.
 * Output values saturate to the symmetric 16-bit range (the open interval @math{(-2^{15},
 * 2^{15})}). To avoid saturation, `b_shl` should be no greater than the headroom of `b` (`b->hr`).
 * 
 * 
 * @operation{
 * &    a_k \leftarrow sat_{16}( \lfloor b_k \cdot 2^{b\_shl} \rfloor )     \\
 * &        \qquad\text{for } k \in 0\ ...\ (N-1)                           \\
 * &        \qquad\text{where } N \text{ is the length of } \bar{B}         \\
 * &        \qquad\text{  and } b_k \text{ and } a_k \text{ are the } k\text{th mantissas from } 
 *              \bar{B}\text{ and } \bar{A}\text{ respectively}
 * }
 * 
 * @param[out] a        Output BFP vector @vector{A}
 * @param[in]  b        Input BFP vector @vector{B}
 * @param[in]  b_shl    Signed arithmetic left-shift to be applied to mantissas of @vector{B}.
 * 
 * @ingroup bfp_s16_api
 */
C_API
void bfp_s16_shl(
    bfp_s16_t* a,
    const bfp_s16_t* b,
    const left_shift_t b_shl);


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
 * @operation{
 *      \bar{A} \leftarrow \bar{B} + \bar{C}  
 * }
 * 
 * @param[out] a     Output BFP vector @vector{A}
 * @param[in]  b     Input BFP vector @vector{B}
 * @param[in]  c     Input BFP vector @vector{C}
 * 
 * @ingroup bfp_s16_api
 */
C_API
void bfp_s16_add(
    bfp_s16_t* a, 
    const bfp_s16_t* b, 
    const bfp_s16_t* c);


/** 
 * @brief Add a scalar to a 16-bit BFP vector.
 * 
 * Add a real scalar @math{c} to input BFP vector @vector{B} and store the result in BFP vector
 * @vector{A}. 
 * 
 * `a`, and `b` must have been initialized (see bfp_s16_init()), and must be the same length.
 * 
 * This operation can be performed safely in-place on `b`.
 * 
 * @operation{
 * &     \bar{A} \leftarrow \bar{B} + c   \\
 * }
 * 
 * @param[out] a     Output BFP vector @vector{A}
 * @param[in]  b     Input BFP vector @vector{B}
 * @param[in]  c     Input scalar @math{c}
 * 
 * @ingroup bfp_s16_api
 */
C_API
void bfp_s16_add_scalar(
    bfp_s16_t* a, 
    const bfp_s16_t* b, 
    const float c);


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
 * @operation{
 *      \bar{A} \leftarrow \bar{B} - \bar{C}  
 * }
 * 
 * @param[out] a     Output BFP vector @vector{A}
 * @param[in]  b     Input BFP vector @vector{B}
 * @param[in]  c     Input BFP vector @vector{C}
 * 
 * @ingroup bfp_s16_api
 */
C_API
void bfp_s16_sub(
    bfp_s16_t* a, 
    const bfp_s16_t* b, 
    const bfp_s16_t* c);


/** 
 * @brief Multiply one 16-bit BFP vector by another element-wise.
 * 
 * Multiply each element of input BFP vector @vector{B} by the corresponding element of input BFP
 * vector @vector{C} and store the results in output BFP vector @vector{A}.
 *
 * `a`, `b` and `c` must have been initialized (see bfp_s16_init()), and must be the same length.
 * 
 * This operation can be performed safely in-place on `b` or `c`.
 * 
 * @operation{ 
 * &     A_k \leftarrow B_k \cdot C_k                    \\
 * &         \qquad\text{for } k \in 0\ ...\ (N-1)       \\
 * &         \qquad\text{where } N \text{ is the length of } \bar{B}\text{ and }\bar{C}
 * }
 * 
 * @param a     Output BFP vector @vector{A}
 * @param b     Input BFP vector @vector{B}
 * @param c     Input BFP vector @vector{C}
 * 
 * @ingroup bfp_s16_api
 */
C_API
void bfp_s16_mul(
    bfp_s16_t* a, 
    const bfp_s16_t* b, 
    const bfp_s16_t* c);


/**
 * @brief Multiply one 16-bit BFP vector by another element-wise and add the result to a third
 * vector.
 * 
 * @operation{
 * &    A_k \leftarrow A_k + B_k \cdot C_k              \\
 * &        \qquad\text{for } k \in 0\ ...\ (N-1)       \\
 * &        \qquad\text{where } N \text{ is the length of } \bar{B}\text{ and }\bar{C}
 * }
 * 
 * @param[inout]  acc   Input/Output accumulator BFP vector @vector{A}
 * @param[in]     b     Input BFP vector @vector{B}
 * @param[in]     c     Input BFP vector @vector{C}
 * 
 * @ingroup bfp_s16_api
 */
C_API
void bfp_s16_macc(
    bfp_s16_t* acc, 
    const bfp_s16_t* b, 
    const bfp_s16_t* c);


/**
 * @brief Multiply one 16-bit BFP vector by another element-wise and subtract the result from a
 * third vector.
 * 
 * @operation{
 * &    A_k \leftarrow A_k - B_k \cdot C_k              \\
 * &        \qquad\text{for } k \in 0\ ...\ (N-1)       \\
 * &        \qquad\text{where } N \text{ is the length of } \bar{B}\text{ and }\bar{C}
 * }
 * 
 * @param[inout]  acc   Input/Output accumulator BFP vector @vector{A}
 * @param[in]     b     Input BFP vector @vector{B}
 * @param[in]     c     Input BFP vector @vector{C}
 * 
 * @ingroup bfp_s16_api
 */
C_API
void bfp_s16_nmacc(
    bfp_s16_t* acc, 
    const bfp_s16_t* b, 
    const bfp_s16_t* c);


/** 
 * @brief Multiply a 16-bit BFP vector by a scalar.
 * 
 * Multiply input BFP vector @vector{B} by scalar @math{\alpha \cdot 2^{\alpha\_exp}} and store the
 * result in output BFP vector @vector{A}.
 *
 * `a` and `b` must have been initialized (see bfp_s16_init()), and must be the same length.
 *
 * `alpha` represents the scalar @math{\alpha \cdot 2^{\alpha\_exp}}, where @math{\alpha} is
 * `alpha.mant` and @math{\alpha\_exp} is `alpha.exp`.
 *
 * This operation can be performed safely in-place on `b`.
 * 
 * @operation{ 
 *      \bar{A} \leftarrow \bar{B} \cdot \left(\alpha \cdot 2^{\alpha\_exp}\right)
 * }
 * 
 * @param[out] a            Output BFP vector @vector{A}
 * @param[in]  b            Input BFP vector @vector{B}
 * @param[in]  alpha        Scalar by which @vector{B} is multiplied
 * 
 * @ingroup bfp_s16_api
 */
C_API
void bfp_s16_scale(
    bfp_s16_t* a, 
    const bfp_s16_t* b, 
    const float alpha);


/** 
 * @brief Get the absolute values of elements of a 16-bit BFP vector. 
 * 
 * Compute the absolute value of each element @math{B_k} of input BFP vector @vector{B} and store
 * the results in output BFP vector @vector{A}.
 * 
 * `a` and `b` must have been initialized (see bfp_s16_init()), and must be the same length.
 * 
 * This operation can be performed safely in-place on `b`.
 * 
 * @operation{ 
 * &     A_k \leftarrow \left| B_k \right|               \\
 * &         \qquad\text{for } k \in 0\ ...\ (N-1)       \\
 * &         \qquad\text{where } N \text{ is the length of } \bar{B}
 * }
 * 
 * @param[out] a     Output BFP vector @vector{A}
 * @param[in]  b     Input BFP vector @vector{B}
 * 
 * @ingroup bfp_s16_api
 */
C_API
void bfp_s16_abs(
    bfp_s16_t* a,
    const bfp_s16_t* b);


/** 
 * @brief Sum the elements of a 16-bit BFP vector.
 * 
 * Sum the elements of input BFP vector @vector{B} to get a result @math{A = a \cdot 2^{a\_exp}},
 * which is returned. The returned value has a 32-bit mantissa.
 * 
 * `b` must have been initialized (see bfp_s16_init()).
 * 
 * @operation{
 * &     A \leftarrow \sum_{k=0}^{N-1} \left( B_k \right)            \\
 * &         \qquad\text{where } N \text{ is the length of } \bar{B}
 * }
 * 
 * @param[in] b         Input BFP vector @vector{B}
 * 
 * @returns  @math{A}, the sum of elements of @vector{B}
 * 
 * @ingroup bfp_s16_api
 */
C_API
float_s32_t bfp_s16_sum(
    const bfp_s16_t* b);


/** 
 * @brief Compute the inner product of two 16-bit BFP vectors.
 * 
 * Adds together the element-wise products of input BFP vectors @vector{B} and @vector{C} for a
 * result @math{A = a \cdot 2^{a\_exp}}, where @math{a} is the 64-bit mantissa of the result and
 * @math{a\_exp} is its associated exponent. @math{A} is returned.
 * 
 * `b` and `c` must have been initialized (see bfp_s16_init()), and must be the same length.
 * 
 * @operation{
 * &     a \cdot 2^{a\_exp} \leftarrow \sum_{k=0}^{N-1} \left( B_k \cdot C_k \right)     \\
 * &         \qquad\text{where } N \text{ is the length of } \bar{B}\text{ and }\bar{C}
 * }
 * 
 * @param[in]  b        Input BFP vector @vector{B}
 * @param[in]  c        Input BFP vector @vector{C}
 * 
 * @returns     @math{A}, the inner product of vectors @vector{B} and @vector{C}
 * 
 * @ingroup bfp_s16_api
 */
C_API
float_s64_t bfp_s16_dot(
    const bfp_s16_t* b, 
    const bfp_s16_t* c);


/** 
 * Clamp the elements of a 16-bit BFP vector to a specified range.
 * 
 * Each element @math{A_k} of output BFP vector @vector{A} is set to the corresponding element
 * @math{B_k} of input BFP vector @vector{B} if it is in the range @math{ [ L \cdot 2^{bound\_exp},
 * U \cdot 2^{bound\_exp} ] }, otherwise it is set to the nearest value inside that range.
 * 
 * `a` and `b` must have been initialized (see bfp_s16_init()), and must be the same length.
 * 
 * This operation can be performed safely in-place on `b`.
 * 
 * 
 * @operation{
 * &     A_k \leftarrow \begin{cases}
 *           L \cdot 2^{bound\_exp}      &   B_k < L \cdot 2^{bound\_exp}  \\
 *           U \cdot 2^{bound\_exp}      &   B_k > U \cdot 2^{bound\_exp}  \\
 *           B_k                         &   otherwise
 *       \end{cases}                                     \\
 * &         \qquad\text{for } k \in 0\ ...\ (N-1)       \\
 * &         \qquad\text{where } N \text{ is the length of } \bar{B}
 * }
 * 
 * @param[out] a             Output BFP vector @vector{A}
 * @param[in]  b             Input BFP vector @vector{B}
 * @param[in]  lower_bound   Mantissa of the lower clipping bound, @math{L}
 * @param[in]  upper_bound   Mantissa of the upper clipping bound, @math{U}
 * @param[in]  bound_exp     Shared exponent of the clipping bounds
 * 
 * @ingroup bfp_s16_api
 */
C_API
void bfp_s16_clip(
    bfp_s16_t* a, 
    const bfp_s16_t* b, 
    const int16_t lower_bound, 
    const int16_t upper_bound, 
    const int bound_exp);


/** 
 * @brief Rectify a 16-bit BFP vector.
 * 
 * Each element @math{A_k} of output BFP vector @vector{A} is set to the corresponding element
 * @math{B_k} of input BFP vector @vector{B} if it is non-negative, otherwise it is set to @math{0}.
 * 
 * `a` and `b` must have been initialized (see bfp_s16_init()), and must be the same length.
 * 
 * This operation can be performed safely in-place on `b`.
 * 
 * @operation{
 * &     A_k \leftarrow \begin{cases}
 *           0       &   B_k < 0       \\
 *           B_k     &   otherwise
 *       \end{cases}                     \\
 * &     \qquad\text{for } k \in 0\ ...\ (N-1)       \\
 * &     \qquad\text{where } N \text{ is the length of } \bar{B}
 * }
 * 
 * @param[out] a     Output BFP vector @vector{A}
 * @param[in]  b     Input BFP vector @vector{B}
 * 
 * @ingroup bfp_s16_api
 */
C_API
void bfp_s16_rect(
    bfp_s16_t* a,
    const bfp_s16_t* b);


/** 
 * @brief Convert a 16-bit BFP vector into a 32-bit BFP vector.
 * 
 * Increases the bit-depth of each 16-bit element @math{B_k} of input BFP vector @vector{B} to 32
 * bits, and stores the 32-bit result in the corresponding element @math{A_k} of output BFP vector
 * @vector{A}.
 * 
 * `a` and `b` must have been initialized (see bfp_s16_init() and bfp_s32_init()), and must be the
 * same length.
 * 
 * @operation{
 * &     A_k \overset{32-bit}{\longleftarrow} B_k        \\
 * &         \qquad\text{for } k \in 0\ ...\ (N-1)       \\
 * &         \qquad\text{where } N \text{ is the length of } \bar{B}
 * }
 * 
 * @param[out] a     Output BFP vector @vector{A}
 * @param[in]  b     Input BFP vector @vector{B}
 * 
 * @ingroup bfp_s16_api
 **/
C_API
void bfp_s16_to_bfp_s32(
    bfp_s32_t* a,
    const bfp_s16_t* b);


/** 
 * @brief Get the square roots of elements of a 16-bit BFP vector.
 * 
 * Computes the square root of each element @math{B_k} of input BFP vector @vector{B} and stores the
 * results in output BFP vector @vector{A}.
 * 
 * `a` and `b` must have been initialized (see bfp_s16_init()), and must be the same length.
 * 
 * This operation can be performed safely in-place on `b`.
 * 
 * @operation{ 
 * &     A_k \leftarrow \sqrt{B_k}                       \\
 * &         \qquad\text{for } k \in 0\ ...\ (N-1)       \\
 * &         \qquad\text{where } N \text{ is the length of } \bar{B}
 * }
 * 
 * @par Notes
 * @parblock
 * * Only the `XMATH_BFP_SQRT_DEPTH_S16` (see xmath_conf.h) most significant bits of each result
 *   are computed.
 *
 * * This function only computes real roots. For any @math{B_k < 0}, the corresponding output
 *   @math{A_k} is set to @math{0}.
 * @endparblock
 * 
 * @param[out] a     Output BFP vector @vector{A}
 * @param[in]  b     Input BFP vector @vector{B}
 * 
 * @ingroup bfp_s16_api
 */
C_API
void bfp_s16_sqrt(
    bfp_s16_t* a,
    const bfp_s16_t* b);


/** 
 * @brief Get the inverses of elements of a 16-bit BFP vector.
 * 
 * Computes the inverse of each element @math{B_k} of input BFP vector @vector{B} and stores the
 * results in output BFP vector @vector{A}.
 * 
 * `a` and `b` must have been initialized (see bfp_s16_init()), and must be the same length.
 * 
 * This operation can be performed safely in-place on `b`.
 * 
 * @operation{ 
 * &     A_k \leftarrow B_k^{-1}                     \\
 * &         \qquad\text{for } k \in 0\ ...\ (N-1)   \\
 * &         \qquad\text{where } N \text{ is the length of } \bar{B}
 * }
 * 
 * @param[out] a     Output BFP vector @vector{A}
 * @param[in]  b     Input BFP vector @vector{B}
 * 
 * @ingroup bfp_s16_api
 */
C_API
void bfp_s16_inverse(
    bfp_s16_t* a,
    const bfp_s16_t* b);


/** 
 * @brief Sum the absolute values of elements of a 16-bit BFP vector.
 * 
 * Sum the absolute values of elements of input BFP vector @vector{B} for a result @math{A = a \cdot
 * 2^{a\_exp}}, where @math{a} is a 32-bit mantissa and @math{a\_exp} is its associated exponent.
 * @math{A} is returned.
 * 
 * `b` must have been initialized (see bfp_s16_init()).
 * 
 * @operation{
 * &     A \leftarrow \sum_{k=0}^{N-1} \left| A_k \right|            \\
 * &         \qquad\text{where } N \text{ is the length of } \bar{B}
 * }
 * 
 * @param[in] b         Input BFP vector @vector{B}
 * 
 * @returns  @math{A}, the sum of absolute values of elements of @vector{B}
 * 
 * @ingroup bfp_s16_api
 */
C_API
float_s32_t bfp_s16_abs_sum(
    const bfp_s16_t* b);


/** 
 * @brief Get the mean value of a 16-bit BFP vector.
 * 
 * Computes @math{A = a \cdot 2^{a\_exp}}, the mean value of elements of input BFP vector
 * @vector{B}, where @math{a} is the 16-bit mantissa of the result, and @math{a\_exp} is its
 * associated exponent. @math{A} is returned.
 * 
 * `b` must have been initialized (see bfp_s16_init()).
 * 
 * @operation{
 * &     A \leftarrow \frac{1}{N} \sum_{k=0}^{N-1} \left( B_k \right)     \\
 * &         \qquad\text{where } N \text{ is the length of } \bar{B}
 * }
 * 
 * @param[in]  b        Input BFP vector @vector{B}
 * 
 * @returns  @math{A}, the mean value of @vector{B}'s elements
 * 
 * @ingroup bfp_s16_api
 */
C_API
float bfp_s16_mean(
    const bfp_s16_t* b);


/** 
 * @brief Get the energy (sum of squared of elements) of a 16-bit BFP vector.
 * 
 * Computes @math{A = a \cdot 2^{a\_exp}}, the sum of squares of elements of input BFP vector
 * @vector{B}, where @math{a} is the 64-bit mantissa of the result, and @math{a\_exp} is its
 * associated exponent. @math{A} is returned.
 * 
 * `b` must have been initialized (see bfp_s16_init()).
 * 
 * @operation{
 * &     A \leftarrow \sum_{k=0}^{N-1} \left( B_k^2 \right)   \\
 * &         \qquad\text{where } N \text{ is the length of } \bar{B}
 * }
 * 
 * @param[in]  b        Input BFP vector @vector{B}
 * 
 * @returns  @math{A}, @vector{B}'s energy
 * 
 * @ingroup bfp_s16_api
 */
C_API
float_s64_t bfp_s16_energy(
    const bfp_s16_t* b);


/** 
 * @brief Get the RMS value of elements of a 16-bit BFP vector.
 * 
 * Computes @math{A = a \cdot 2^{a\_exp}}, the RMS value of elements of input BFP vector @vector{B},
 * where @math{a} is the 32-bit mantissa of the result, and @math{a\_exp} is its associated
 * exponent. @math{A} is returned.
 *
 * The RMS (root-mean-square) value of a vector is the square root of the sum of the squares of the
 * vector's elements.
 * 
 * `b` must have been initialized (see bfp_s16_init()).
 * 
 * @operation{
 * &     A \leftarrow \sqrt{\frac{1}{N}\sum_{k=0}^{N-1} \left( B_k^2 \right) }  \\
 * &         \qquad\text{where } N \text{ is the length of } \bar{B}
 * }
 * 
 * @param[in]  b        Input BFP vector @vector{B}
 * 
 * @returns  @math{A}, the RMS value of @vector{B}'s elements
 * 
 * @ingroup bfp_s16_api
 */
C_API
float_s32_t bfp_s16_rms(
    const bfp_s16_t* b);


/** 
 * @brief Get the maximum value of a 16-bit BFP vector.
 * 
 * Finds @math{A}, the maximum value among elements of input BFP vector @vector{B}. @math{A} is
 * returned by this function.
 * 
 * `b` must have been initialized (see bfp_s16_init()).
 * 
 * @operation{
 * &     A \leftarrow max\left(B_0\, B_1\, ...\, B_{N-1} \right)     \\
 * &         \qquad\text{where } N \text{ is the length of } \bar{B}
 * }
 * 
 * @param[in] b     Input vector
 * 
 * @returns     @math{A}, the value of @vector{B}'s maximum element
 * 
 * @ingroup bfp_s16_api
 */
C_API
float bfp_s16_max(
    const bfp_s16_t* b);


/**
 * @brief Get the element-wise maximum of two 16-bit BFP vectors.
 * 
 * Each element of output vector @vector{A} is set to the maximum of the 
 * corresponding elements in the input vectors @vector{B} and @vector{C}.
 * 
 * `a`, `b` and `c` must have been initialized (see bfp_s16_init()), and must be
 * the same length.
 * 
 * This operation can be performed safely in-place on `b`, but @a not on `c`.
 * 
 * @operation{ 
 * &     A_k \leftarrow max(B_k, C_k)                             \\
 * &         \qquad\text{for } k \in 0\ ...\ (N-1)                \\
 * &         \qquad\text{where } N \text{ is the length of } \bar{B}\text{ and }\bar{C}
 * }
 * 
 * @param a     Output BFP vector @vector{A}
 * @param b     Input BFP vector @vector{B}
 * @param c     Input BFP vector @vector{C}
 * 
 * @ingroup bfp_s16_api
 */
C_API
void bfp_s16_max_elementwise(
    bfp_s16_t* a, 
    const bfp_s16_t* b, 
    const bfp_s16_t* c);


/** 
 * @brief Get the minimum value of a 16-bit BFP vector.
 * 
 * Finds @math{A}, the minimum value among elements of input BFP vector @vector{B}. @math{A} is
 * returned by this function.
 * 
 * `b` must have been initialized (see bfp_s16_init()).
 * 
 * @operation{
 * &     A \leftarrow min\left(B_0\, B_1\, ...\, B_{N-1} \right)     \\
 * &         \qquad\text{where } N \text{ is the length of } \bar{B}
 * }
 * 
 * @param[in] b     Input vector
 * 
 * @returns     @math{A}, the value of @vector{B}'s minimum element
 * 
 * @ingroup bfp_s16_api
 */
C_API
float bfp_s16_min(
    const bfp_s16_t* b);


/**
 * @brief Get the element-wise minimum of two 16-bit BFP vectors.
 * 
 * Each element of output vector @vector{A} is set to the minimum of the 
 * corresponding elements in the input vectors @vector{B} and @vector{C}.
 * 
 * `a`, `b` and `c` must have been initialized (see bfp_s16_init()), and must be
 * the same length.
 * 
 * This operation can be performed safely in-place on `b`, but @a not on `c`.
 * 
 * @operation{ 
 * &     A_k \leftarrow min(B_k, C_k)                             \\
 * &         \qquad\text{for } k \in 0\ ...\ (N-1)                \\
 * &         \qquad\text{where } N \text{ is the length of } \bar{B}\text{ and }\bar{C}
 * }
 * 
 * @param a     Output BFP vector @vector{A}
 * @param b     Input BFP vector @vector{B}
 * @param c     Input BFP vector @vector{C}
 * 
 * @ingroup bfp_s16_api
 */
C_API
void bfp_s16_min_elementwise(
    bfp_s16_t* a, 
    const bfp_s16_t* b, 
    const bfp_s16_t* c);


/** 
 * @brief Get the index of the maximum value of a 16-bit BFP vector.
 * 
 * Finds @math{a}, the index of the maximum value among the elements of input BFP vector @vector{B}.
 * @math{a} is returned by this function.
 *
 * If `i` is the value returned, then the maximum value in @vector{B} is `ldexp(b->data[i],b->exp)`.
 * 
 * @operation{
 * &     a \leftarrow argmax_k\left(b_k\right)           \\
 * &         \qquad\text{for } k \in 0\ ...\ (N-1)       \\
 * &         \qquad\text{where } N \text{ is the length of } \bar{B}
 * }
 * 
 * @par Notes
 * @parblock
 * * If there is a tie for maximum value, the lowest tying index is returned.
 * @endparblock
 * 
 * @param[in] b     Input vector
 * 
 * @returns     @math{a}, the index of the maximum value from @vector{B}
 * 
 * @ingroup bfp_s16_api
 */
C_API
unsigned bfp_s16_argmax(
    const bfp_s16_t* b);


/** 
 * @brief Get the index of the minimum value of a 16-bit BFP vector.
 * 
 * Finds @math{a}, the index of the minimum value among the elements of input BFP vector @vector{B}.
 * @math{a} is returned by this function.
 *
 * If `i` is the value returned then the minimum value in @vector{B} is `ldexp(b->data[i], b->exp)`.
 * 
 * @operation{
 * &     a \leftarrow argmin_k\left(b_k\right)           \\
 * &         \qquad\text{for } k \in 0\ ...\ (N-1)       \\
 * &         \qquad\text{where } N \text{ is the length of } \bar{B}
 * }
 * 
 * @par Notes
 * @parblock
 * * If there is a tie for minimum value, the lowest tying index is returned.
 * @endparblock
 * 
 * @param[in] b     Input vector
 * 
 * @returns     @math{a}, the index of the minimum value from @vector{B}
 * 
 * @ingroup bfp_s16_api
 */
C_API
unsigned bfp_s16_argmin(
    const bfp_s16_t* b);


/**
 * @brief Accumulate a 16-bit BFP vector into a 32-bit accumulator vector.
 * 
 * This function is used for efficiently accumulating a series of 16-bit BFP vectors into a 32-bit 
 * vector. Each call to this function adds a BFP vector @vector{B} into the persistent 32-bit 
 * accumulator vector @vector{A}.
 * 
 * Eventually the value of @vector{A} will be needed for something other than simple accumulation,
 * which requires converting from the XS3-native split accumulator representation given by the 
 * @ref split_acc_s32_t struct, into a standard vector of `int32_t`. This can be accomplished
 * using vect_s32_merge_accs(). From there, the `int32_t` vector can be dropped to a 16-bit
 * vector with vect_s32_to_vect_s16() if needed.
 * 
 * Note, in order for this operation to work, @math{\mathtt{b\_exp} - \mathtt{a\_exp}} must be no 
 * greater than @math{14}.
 * 
 * @operation{
 *    \bar{A} \leftarrow \bar{A} + \bar{B}
 * }
 * 
 * @par Usage
 * 
 * Proper use of this function requires some book-keeping on the part of the caller. In particular,
 * the caller is responsible for tracking the exponent and monitoring the headroom of the 
 * accumulator vector @vector{A}.
 * 
 * To begin a sequence of accumulation, start by clearing the contents of @vector{A} to all zeros.
 * Then, an appropriate exponent for @vector{A} must be chosen. The only hard constraint is that the
 * accumulator exponent, @math{\mathtt{a\_exp}} must be within @math{14} of @vector{B}'s exponent,
 * @math{\mathtt{b\_exp}}. If @math{\mathtt{b\_exp}} is unknown, the caller may choose to wait until
 * the first @vector{B} is available before initializing @math{\mathtt{a\_exp}}.
 * 
 * As vectors are accumulated into @vector{A} with multiple calls to this function, it becomes 
 * possible for @vector{A} to saturate for some element. Each call to this function returns the 
 * headroom of @vector{A} (note: no more than 15 bits of headroom will be reported). If @vector{A} 
 * has at least 1 bit of headroom, then a call to this function is guarranteed not to saturate.
 * 
 * The larger @math{\mathtt{a\_exp}} is compared to each @math{\mathtt{b\_exp}}, the more 16-bit 
 * vectors can be accumulated before saturation becomes possible (and by virtue of that, the more 
 * efficiently accumulation can take place.). On the other hand, as long as 
 * @math{\mathtt{a\_exp} \le \mathtt{b\_exp}}, there is no precision loss during accumulation. It
 * is the responsibility of the caller to manage this trade-off.
 * 
 * If and when this function reports that @vector{A} has 0 headroom, if further accumulation is 
 * needed, the caller can handle this by increasing @math{\mathtt{a\_exp}}. Increasing 
 * @math{\mathtt{a\_exp}} will require that the contents of the mantissa vector @vector{a}  be
 * right-shifted to avoid corrupting the value of @vector{A}, making room for further accumulation
 * in the process. Shifting the split accumulators can be accomplished with a call to 
 * vect_split_acc_s32_shr().
 * 
 * Finally, when accumulation is complete or the accumulator values must be used elsewhere, the 
 * split accumulator vector can be converted to simple `int32_t` vector with a call to 
 * vect_s32_merge_accs().
 * 
 * 
 * @param[inout]  a       Mantissas of accumulator vector @vector{A}
 * @param[in]     a_exp   Exponent of accumulator vector @vector{A}
 * @param[in]     b       Input vector @vector{B}
 * 
 * @returns Headroom of @vector{A} (up to 15 bits)
 * 
 * @ingroup bfp_s16_api
 */
C_API
headroom_t bfp_s16_accumulate(
    split_acc_s32_t a[],
    const exponent_t a_exp,
    const bfp_s16_t* b);


#ifdef __XC__
}   //extern "C"
#endif