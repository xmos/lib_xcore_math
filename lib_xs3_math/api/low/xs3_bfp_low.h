
#ifndef XS3_BFP_LOW_H_
#define XS3_BFP_LOW_H_

#ifdef __XC__
extern "C" {
#endif

/**
 * \brief Compute headroom of `int16_t` array `v`
 * 
 * \low_op{16, @f$ min(\ \ \ hr(v_0)\,\ hr(v_1)\,\ ...\,\ hr(v_\{N-1\})\ \ ) @f$ \n\tab where @f$hr(a)@f$ is the headroom of element @f$a@f$  }
 * 
 * \requires_word_alignment{v}
 * 
 * \param[in] v     Array of `int16_t`
 * \param[in] N     Number of elements in `v`
 * 
 * \return  Headroom of the array `v`
 */
headroom_t xs3_headroom_vect_s16(
    const int16_t* v, 
    const unsigned N);

/**
 * \brief Compute headroom of an `int32_t` array.
 * 
 * \low_op{32, @f$ min(\ \ \ hr(v_0)\,\ hr(v_1)\,\ ...\,\ hr(v_\{N-1\})\ \ ) @f$ \n\tab where @f$hr(a)@f$ is the headroom of element @f$a@f$  }
 * 
 * \requires_word_alignment{v}
 * 
 * \param[in] v     Array of `int32_t`
 * \param[in] N     Number of elements in `v`
 * 
 * \return  Headroom of the array `v`
 */
headroom_t xs3_headroom_vect_s32(
    const int32_t* v,
    const unsigned N);

/**
 * \brief Perform a signed, saturating arithmetic left shift of an `int16_t` vector.
 * 
 * \low_op{16, @f$a_k \leftarrow b_k \cdot 2^\{shl\} \qquad\text{ for }k\in 0\ ...\ (length-1)@f$ }
 * 
 * \par Performance
 * The performance of this function is specified as the number of thread-cycles between function
 * entrance and return (including instruction fetches).
 * \code
 *     if ( a == b )
 *         cycles = 3*(length >> 4) + K0;  //(TODO: K0 to be determined)
 *     else
 *         cycles = 4*(length >> 4) + K1   //(TODO: K1 to be determined)
 * \endcode
 * 
 * \safe_in_place{a,b}
 * \requires_word_alignment{a,b}
 * 
 * \note\li With `shl < 0`,  for each  `(1<<(-shl)) < v[k] < 0`, underflows will result in `-1` rather than `0`.
 * 
 * \warning\li To avoid saturation, never use a `shl` value larger than the current headroom of `b`.
 * 
 * \warning\li Unlike many operations, if the supplied value for `shl` is negative (i.e. performing an arithmetic
 *       right shift), the resulting value is truncated, rather than rounded. Rounding can be 
 *       accomplished by first adding `(1<<((-shl)-1))` to each element of the input vector if (and only 
 *       if) `shl` is negative.
 * 
 * \param[out] a         Output vector
 * \param[in] b         Input vector
 * \param[in] length    Number of elements in input (and output)
 * \param[in] shl       Number of bits to shift left
 * 
 * \return         The headroom of the output vector `a`
 */
headroom_t xs3_shl_vect_s16(
    int16_t* a,
    const int16_t* b,
    const unsigned length,
    const left_shift_t shl);

/**
 * \brief Perform a signed, saturating arithmetic left shift of an `int32_t` vector.
 * 
 * \low_op{32, @f$a_k \leftarrow b_k \cdot 2^\{shl\} \qquad\text{ for }k\in 0\ ...\ (length-1)@f$ }
 * 
 * \par Performance
 * The performance of this function is specified as the number of thread-cycles between function
 * entrance and return (including instruction fetches).
 * \code
 *     if ( a == b )
 *         cycles = 3*(length >> 4) + K0;  //(TODO: K0 to be determined)
 *     else
 *         cycles = 4*(length >> 4) + K1   //(TODO: K1 to be determined)
 * \endcode
 * 
 * \safe_in_place{a,b}
 * \requires_word_alignment{a,b}
 * 
 * \note\li With `shl < 0`,  for each  `(1<<(-shl)) < v[k] < 0`, underflows will result in `-1` rather than `0`.
 * 
 * \warning\li Unlike many operations, if the supplied value for `shl` is negative (i.e. performing an arithmetic
 *       right shift), the resulting value is truncated, rather than rounded. Rounding can be 
 *       accomplished by first adding `(1<<((-shl)-1))` to each element of the input vector if (and only 
 *       if) `shl` is negative.
 * 
 * \param[out] a        Output vector
 * \param[in] b         Input vector
 * \param[in] length    Number of elements in input (and output)
 * \param[in] shl       Number of bits to shift left
 * 
 * \return         The headroom of the output vector `a`
 */
headroom_t xs3_shl_vect_s32(
    int32_t* a,
    const int32_t* b,
    const unsigned length,
    const left_shift_t shl);

    
/**
 * \brief Add together two `int16_t` vectors.
 * 
 * Add together the two `int16_t` vectors `b[]` and `c[]`, placing the result in `a[]`. Each element of `b[]` 
 * or `c[]` has an arithmetic right shift of `b_shr` or `c_shr` bits (respectively) applied before the addition.
 * Negative values of `b_shr` and `c_shr` will left-shift.
 * 
 * \low_op{16, @f$a_k \leftarrow b_k \cdot 2^\{-b\_shr\} + c_k \cdot 2^\{-c\_shr\}\qquad\text{ for }k\in 0\ ...\ (length-1)@f$ }
 * 
 * \safe_in_place{a,b,c}
 * \requires_word_alignment{a,b,c}
 * 
 * \warning \li Both shifts (if they are negative) and the addition are saturating operations, and will saturate to the
 *       symmetric 16-bit range  (see: TODO).
 * 
 * \warning \li Where negative values underflow due to a right-shift, the resulting value is `-1`, rather than `0`. (see: TODO)
 * 
 * \param[out] a        Output vector
 * \param[in] b         Input vector 1
 * \param[in] c         Input vector 2
 * \param[in] length    Number of elements in `a`, `b` and `c`
 * \param[in] b_shr     Arithmetic right-shift applied to elements of `b[]`
 * \param[in] c_shr     Arithmetic right-shift applied to elements of `c[]`
 * 
 * \return  Headroom of output vector `a[]`
 * 
 * \see bfp_add_vect_s16()
 */
headroom_t xs3_add_vect_s16(
    int16_t* a,
    const int16_t* b, 
    const int16_t* c,
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);

/**
 * \brief Add together two `int32_t` vectors.
 * 
 * Add together the two `int32_t` vectors `b[]` and `c[]`, placing the result in `a[]`. Each element of `b[]` 
 * or `c[]` has an arithmetic right shift of `b_shr` or `c_shr` bits (respectively) applied before the addition.
 * Negative values of `b_shr` and `c_shr` will left-shift.
 * 
 * \low_op{32, @f$a_k \leftarrow b_k \cdot 2^\{-b\_shr\} + c_k \cdot 2^\{-c\_shr\}\qquad\text{ for }k\in 0\ ...\ (length-1)@f$ }
 * 
 * \safe_in_place{a,b,c}
 * \requires_word_alignment{a,b,c}
 * 
 * \warning \li Both shifts (if they are negative) and the addition are saturating operations, and will saturate to the
 *       symmetric 32-bit range  (see: TODO).
 * 
 * \warning \li Where negative values underflow due to a right-shift, the resulting value is `-1`, rather than `0`. (see: TODO)
 * 
 * \param[out] a        Output vector
 * \param[in] b         Input vector 1
 * \param[in] c         Input vector 2
 * \param[in] length    Number of elements in `a`, `b` and `c`
 * \param[in] b_shr     Arithmetic right-shift applied to elements of `b[]`
 * \param[in] c_shr     Arithmetic right-shift applied to elements of `c[]`
 * 
 * \return  Headroom of output vector `a[]`
 * 
 * \see bfp_add_vect_s32()
 */
headroom_t xs3_add_vect_s32(
    int32_t* a,
    const int32_t* b,
    const int32_t* c,
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * \brief Subtract one `int16_t` vector from another.
 * 
 * Subtract the `int16_t` vector `c[]` from the `int16_t` vector `b[]`, placing the result in `a[]`. Each 
 * element of `b[]` or `c[]` has an arithmetic right shift of `b_shr` or `c_shr` bits (respectively) applied 
 * before the subtraction. Negative values of `b_shr` and `c_shr` will left-shift.
 * 
 * \low_op{16, @f$a_k \leftarrow b_k \cdot 2^\{-b\_shr\} - c_k \cdot 2^\{-c\_shr\}\qquad\text{ for }k\in 0\ ...\ (length-1)@f$ }
 * 
 * \safe_in_place{a,b,c}
 * \requires_word_alignment{a,b,c}
 * 
 * \warning \li Both shifts (if they are negative) and the addition are saturating operations, and will saturate to the
 *       symmetric 16-bit range  (see: TODO).
 * 
 * \warning \li Where negative values underflow due to a right-shift, the resulting value is `-1`, rather than `0`. 
 *              (see: TODO)
 * 
 * \param[out] a        Output vector
 * \param[in] b         Input vector 1
 * \param[in] c         Input vector 2
 * \param[in] length    Number of elements in `a`, `b` and `c`
 * \param[in] b_shr     Arithmetic right-shift applied to elements of `b[]`
 * \param[in] c_shr     Arithmetic right-shift applied to elements of `c[]`
 * 
 * \return  Headroom of output vector `a[]`
 * 
 * \see bfp_sub_vect_s16()
 */
headroom_t xs3_sub_vect_s16(
    int16_t* a,
    const int16_t* b,
    const int16_t* c,
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * \brief Subtract one `int32_t` vector from another.
 * 
 * Subtract the `int32_t` vector `c[]` from the `int32_t` vector `b[]`, placing the result in `a[]`. Each 
 * element of `b[]` or `c[]` has an arithmetic right shift of `b_shr` or `c_shr` bits (respectively) applied 
 * before the subtraction. Negative values of `b_shr` and `c_shr` will left-shift.
 * 
 * \low_op{32, @f$a_k \leftarrow b_k \cdot 2^\{-b\_shr\} - c_k \cdot 2^\{-c\_shr\}\qquad\text{ for }k\in 0\ ...\ (length-1)@f$ }
 * 
 * \safe_in_place{a,b,c}
 * \requires_word_alignment{a,b,c}
 * 
 * \warning \li Both shifts (if they are negative) and the addition are saturating operations, and will saturate to the
 *       symmetric 32-bit range  (see: TODO).
 * 
 * \warning \li Where negative values underflow due to a right-shift, the resulting value is `-1`, rather than `0`. 
 *              (see: TODO)
 * 
 * \param[out] a        Output vector
 * \param[in] b         Input vector 1
 * \param[in] c         Input vector 2
 * \param[in] length    Number of elements in `a`, `b` and `c`
 * \param[in] b_shr     Arithmetic right-shift applied to elements of `b[]`
 * \param[in] c_shr     Arithmetic right-shift applied to elements of `c[]`
 * 
 * \return  Headroom of output vector `a`
 * 
 * \see bfp_sub_vect_s16()
 */
headroom_t xs3_sub_vect_s32(
    int32_t* a,
    const int32_t* b,
    const int32_t* c,
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * \brief Multiply one `int16_t` vector by another.
 * 
 * Multiply the `int16_t` vector `b` element-wise by the `int16_t` vector `c`, right-shift the 32-bit 
 * product by `a_shr` bits, and store the result in output vector `a`.
 * 
 * The final result is saturated to 16-bit bounds.
 * 
 * \low_op{16, @f$a_k \leftarrow \left(b_k \cdot c_k \right) \cdot 2^\{-a\_shr\} \qquad\text{ for }k\in 0\ ...\ (length-1)@f$ }
 * 
 * \safe_in_place{a,b,c}
 * \requires_word_alignment{a,b,c}
 * 
 * \param[out] a        Output vector
 * \param[in] b         Input vector 1
 * \param[in] c         Input vector 2
 * \param[in] length    Number of elements in `a`, `b` and `c`
 * \param[in] a_shr     Arithmetic right-shift applied to product of `b` and `c`
 * 
 * \return  Headroom of output vector `a`
 */
//! [xs3_mul_vect_s16]
headroom_t xs3_mul_vect_s16(
    int16_t* a,
    const int16_t* b,
    const int16_t* c,
    const unsigned length,
    const right_shift_t a_shr);
//! [xs3_mul_vect_s16]

/**
 * \brief Multiply one `int32_t` vector by another.
 * 
 * Multiply the `int32_t` vector `b`, arithmetically right-shifted `b_shr` bits, by the `int32_t` 
 * vector `c`, arithmetically right-shifted `c_shr` bits, and place the 64-bit product, arithmetically 
 * right-shifted another `30` bits, in output vector `a`.
 * 
 * If `b_shr` or `c_shr`are negative, a left-shift will occur instead of a right-shift. Left-shifts are 
 * saturating operations which saturate to 32-bit bounds. The result of the final right-shift is also 
 * saturated to 32-bit bounds.
 * 
 * \low_op{16, @f$a_k \leftarrow \left(b_k \cdot 2^\{-b\_shr\} \times c_k \cdot 2^\{-c\_shr\}\right) \cdot 2^\{-30\} \qquad\text{ for }k\in 0\ ...\ (length-1)@f$ }
 * 
 * \safe_in_place{a,b,c}
 * \requires_word_alignment{a,b,c}
 * 
 * \param[out] a        Output vector
 * \param[in] b         Input vector 1
 * \param[in] c         Input vector 2
 * \param[in] length    Number of elements in `a`, `b` and `c`
 * \param[in] b_shr     Arithmetic right-shift applied to elements of `b`
 * \param[in] c_shr     Arithmetic right-shift applied to elements of `c`
 * 
 * \return  Headroom of output vector `a`
 */
//! [xs3_mul_vect_s32]
headroom_t xs3_mul_vect_s32(
    int32_t* a,
    const int32_t* b,
    const int32_t* c,
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);
//! [xs3_mul_vect_s32]

/**
 * \brief Multiply one `int16_t` vector by a 16-bit scalar.
 * 
 * Multiply the `int16_t` vector `b`, arithmetically right-shifted `b_shr` bits, by `alpha`, 
 * and place the 32-bit result, arithmetically right-shifted another `14` bits, in output 
 * vector `a`.
 * 
 * If `b_shr` is negative, a left-shift will occur instead of a right-shift. Left-shifts are 
 * saturating operations which saturate to 16-bit bounds. The result of the final right-shift is also 
 * saturated to 16-bit bounds.
 * 
 * \low_op{16, @f$a_k \leftarrow \left(\alpha \cdot b_k \cdot 2^\{-b\_shr\}\right)\cdot 2^\{-14\} \qquad\text{ for }k\in 0\ ...\ (length-1)@f$ }
 * 
 * \safe_in_place{a,b}
 * \requires_word_alignment{a,b}
 * 
 * \param[out] a        Output vector
 * \param[in] b         Input vector 1
 * \param[in] length    Number of elements in `a` and `b`
 * \param[in] alpha     Scalar multiplier
 * \param[in] b_shr     Arithmetic right-shift applied to elements of `b`
 * 
 * \return  Headroom of output vector `a`
 */
headroom_t xs3_scalar_mul_vect_s16(
    int16_t* a,
    const int16_t* b,
    const unsigned length,
    const int16_t alpha,
    const right_shift_t b_shr);

/**
 * \brief Multiply one `int32_t` vector by a 32-bit scalar.
 * 
 * Multiply the `int32_t` vector `b`, arithmetically right-shifted `b_shr` bits, by `alpha`, 
 * and place the 64-bit result, arithmetically right-shifted another `30` bits, in output 
 * vector `a`.
 * 
 * If `b_shr` is negative, a left-shift will occur instead of a right-shift. Left-shifts are 
 * saturating operations which saturate to 16-bit bounds. The result of the final right-shift is also 
 * saturated to 32-bit bounds.
 * 
 * \low_op{16, @f$a_k \leftarrow \left(\alpha \cdot b_k \cdot 2^\{-b\_shr\}\right)\cdot 2^\{-30\} \qquad\text{ for }k\in 0\ ...\ (length-1)@f$ }
 * 
 * \safe_in_place{a,b}
 * \requires_word_alignment{a,b}
 * 
 * \param[out] a        Output vector
 * \param[in] b         Input vector 1
 * \param[in] length    Number of elements in `a` and `b`
 * \param[in] alpha     Scalar multiplier
 * \param[in] b_shr     Arithmetic right-shift applied to elements of `b`
 * 
 * \return  Headroom of output vector `a`
 */
headroom_t xs3_scalar_mul_vect_s32(
    int32_t* a,
    const int32_t* b,
    const unsigned length,
    const int32_t alpha,
    const right_shift_t b_shr);


/** 
 * \brief Compute the absolute value (element-wise) of an `int16_t` vector.
 * 
 * Each output element `a[k]` is set to the absolute value of the corresponding 
 * input element `b[k]` 
 * 
 * \low_op{16, @f$a_k \leftarrow \left| b_k \right|\qquad\text{ for }k\in 0\ ...\ (length-1)@f$ }
 * 
 * \requires_word_alignment{a,b}
 * \safe_in_place{a,b}
 * 
 * \param[out] a        Output vector
 * \param[in] b         Input vector
 * \param[in] length    Number of elements in vectors `a` and `b`
 * 
 * \return  Headroom of the output vector `a`
 */
headroom_t xs3_abs_vect_s16(
    int16_t* a,
    const int16_t* b,
    const unsigned length);


/** 
 * \brief Compute the absolute value (element-wise) of an `int32_t` vector.
 * 
 * Each output element `a[k]` is set to the absolute value of the corresponding 
 * input element `b[k]` 
 * 
 * \low_op{32, @f$a_k \leftarrow \saturating_op\{\left| b_k \right|\} \qquad\text{ for }k\in 0\ ...\ (length-1)@f$ }
 * 
 * \requires_word_alignment{a,b}
 * \safe_in_place{a,b}
 * 
 * \param[out] a        Output vector
 * \param[in] b         Input vector
 * \param[in] length    Number of elements in vectors `a` and `b`
 * 
 * \return  Headroom of the output vector `a`
 * 
 */
headroom_t xs3_abs_vect_s32(
    int32_t* a,
    const int32_t* b,
    const unsigned length);


/**
 * \brief Sum the elements of an `int16_t` vector.
 * 
 * Sums together the elements of the `int16_t` vector `b` and returns the result as an `int32_t`.
 * 
 * \low_op{16, @f$ \sum_\{k=0\}^\{length-1\} b_k     @f$ }
 * 
 * \requires_word_alignment{b}
 * 
 * \warning If `length > 65536` it is possible for saturation to occur on the 32-bit accumulator. Saturating
 *          additions are *not* associative, and so no guarantees are made with respect to the correctness of
 *          the result unless it is known *a priori* that no partial sums of elements from `b` (taken in any 
 *          order) will saturate. If more more than `65536` elements are to be summed, it is recommended that
 *          the user instead make multiple calls to `xs3_sum_s16()`, adding together partial sums of 
 *          subsequences of `b` in user code.
 * 
 * \param[in] b         Input vector
 * \param[in] length    Number of elements in vector `b`
 * 
 * \return  Sum of elements in `b`
 */
int32_t xs3_sum_s16(
    const int16_t* b,
    const unsigned length);


/**
 * \brief Sum the elements of an `int32_t` vector.
 * 
 * Sums together the elements of the `int32_t` vector `b` into a 40-bit saturating accumulator. The result
 * is returned as an `int64_t`.
 * 
 * \low_op{32, @f$ \sum_\{k=0\}^\{length-1\} b_k     @f$ }
 * 
 * \requires_word_alignment{b}
 * 
 * \warning If `length > 256` it is possible for saturation to occur on the 40-bit accumulator. Saturating
 *          additions are *not* associative, and so no guarantees are made with respect to the correctness of
 *          the result unless it is known *a priori* that no partial sums of elements from `b` (taken in any 
 *          order) will saturate. If more more than `256` elements are to be summed, it is recommended that
 *          the user instead make multiple calls to `xs3_sum_s32()`, adding together partial sums of 
 *          subsequences of `b` in user code.
 * 
 * \param[in] b         Input vector
 * \param[in] length    Number of elements in vector `b`
 * 
 * \return  Sum of elements in `b`
 */
int64_t xs3_sum_s32(
    const int32_t* b,
    const unsigned length);

/**
 * @brief Compute the dot product between two `int16_t` vectors.
 * 
 * Elements of the vector `b` are right-shifted `b_shr` bits and multiplied by the corresponding 
 * elements of vector `c` right-shifted `c_shr` bits. The element-wise products are added together
 * and the sum is right-shifted `sat` bits to produce the final 16-bit result.
 * 
 * Either `b_shr` and `c_shr` (or both) may be negative, in which case left-shifting occurs instead of right-shifting.
 * The shifts on the elements of `b` and `c` are saturating (if negative) and truncate rather than round (if positive).
 * Negative values for `sat` are treated as zeros. The final shift is rounding, and the result saturates to 16-bit
 * bounds.
 * 
 * The accumulator into which the element-wise products are accumulated is 32 bits wide and saturates to 32-bit
 * bounds. To avoid saturation, choose sufficiently large values for `b_shr` and `c_shr` such that saturation
 * of the accumulator is not possible.
 * 
 * All shifts are arithmetic.
 * 
 * @low_op{16, @f$ \left(\sum_\{k=0\}^\{length-1\} \{\left(b_k \cdot 2^\{-b\_shr\}\right)\times\left(c_k\cdot 2^\{-c\_shr\}\right)\}\right) \cdot 2^\{-sat\}    @f$ }
 * 
 * @requires_word_alignment{b,c}
 * 
 * @param[in] b         First input vector
 * @param[in] c         Second input vector
 * @param[in] length    Number of elements in vectors `b` and `c`
 * @param[in] b_shr     Right-shift applied to elements of `b`
 * @param[in] c_shr     Right-shift applied to elements of `c`
 * @param[in] sat       Right-shift applied to final sum
 * 
 * @return The dot product of `b` and `c`
 */
int32_t xs3_dot_s16(
    const int16_t* b,
    const int16_t* c,
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);

/**
 * \brief Compute the dot product between two `int32_t` vectors.
 * 
 * Elements of the vector `b` are right-shifted `b_shr` bits and multiplied by the corresponding 
 * elements of vector `c` right-shifted `c_shr` bits. The element-wise products are right-shifted
 * an additional 30 bits and are added together into a 40-bit accumulator. The 40-bit sum is right-shifted
 * `sat` bits to produce the final 32-bit result.
 * 
 * Either `b_shr` and `c_shr` (or both) may be negative (which may be useful if the corresponding vector has 
 * excessive headroom), in which case left-shifting occurs instead of right-shifting. The shifts on the 
 * elements of `b` and `c` are saturating (if negative) and truncate rather than round (if positive). Negative 
 * values for `sat` are treated as zeros. The final shift is rounding, and the result saturates to 32-bit
 * bounds.
 * 
 * The accumulator into which the element-wise products are accumulated is 40 bits wide and saturates to 40-bit
 * bounds. To avoid saturation, choose sufficiently large values for `b_shr` and `c_shr` such that saturation
 * of the accumulator is not possible.
 * 
 * All shifts are arithmetic.
 * 
 * \low_op{32, @f$ \left(\sum_\{k=0\}^\{length-1\} \{\left(\left(b_k \cdot 2^\{-b\_shr\}\right)\times\left(c_k\cdot 2^\{-c\_shr\}\right)\right)\cdot 2^\{-30\}\}\right) \cdot 2^\{-sat\}    @f$ }
 * 
 * \requires_word_alignment{b,c}
 * 
 * \param[in] b         First input vector
 * \param[in] c         Second input vector
 * \param[in] length    Number of elements in vectors `b` and `c`
 * \param[in] b_shr     Right-shift applied to elements of `b`
 * \param[in] c_shr     Right-shift applied to elements of `c`
 * \param[in] sat       Right-shift applied to final sum
 * 
 * \return The dot product of `b` and `c`
 */
int64_t xs3_dot_s32(
    const int32_t* b,
    const int32_t* c,
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);

/**
 * \brief Clamp the elements of an `int16_t` vector to a specified range.
 * 
 * Each element of `b` is right-shifted by `b_shr` and compared to `lower_bound`
 * and `upper_bound`. If the shifted value is less than `lower_bound`, the output
 * element will be `lower_bound`. If the shifted value is greater than `upper_bound`,
 * the output element will be `upper_bound`. Otherwise, the shifted value is output.
 * 
 * \low_op{16, @f$a_k \leftarrow \begin\{cases\}
 *                       lower\_bound & b_k \cdot 2^\{-b\_shr\} \lt lower\_bound \\
 *                       upper\_bound & b_k \cdot 2^\{-b\_shr\} \gt upper\_bound \\
 *                       b_k \cdot 2^\{-b\_shr\} & otherwise
 *                          \end\{cases\}
 *                                  \qquad\text{ for }k\in 0\ ...\ (length-1)@f$ }
 * 
 * \safe_in_place{a,b}
 * \requires_word_alignment{a,b}
 * 
 * \param[out]  a               Output vector
 * \param[in]   b               Input vector
 * \param[in]   length          Number of elements in vectors `a` and `b`
 * \param[in]   lower_bound     Lower bound of clipping range
 * \param[in]   upper_bound     Upper bound of clipping range
 * \param[in]   b_shr           Right-shift applied to elements of `b`
 * 
 * \return  Headroom of output vector `a`
 * 
 */
headroom_t xs3_clip_vect_s16(
    int16_t* a,
    const int16_t* b,
    const unsigned length,
    const int16_t lower_bound,
    const int16_t upper_bound,
    const right_shift_t b_shr);

/**
 * \brief Clamp the elements of an `int32_t` vector to a specified range.
 * 
 * Each element of `b` is right-shifted by `b_shr` and compared to `lower_bound`
 * and `upper_bound`. If the shifted value is less than `lower_bound`, the output
 * element will be `lower_bound`. If the shifted value is greater than `upper_bound`,
 * the output element will be `upper_bound`. Otherwise, the shifted value is output.
 * 
 * \low_op{16, @f$a_k \leftarrow \begin\{cases\}
 *                       lower\_bound & b_k \cdot 2^\{-b\_shr\} \lt lower\_bound \\
 *                       upper\_bound & b_k \cdot 2^\{-b\_shr\} \gt upper\_bound \\
 *                       b_k \cdot 2^\{-b\_shr\} & otherwise
 *                          \end\{cases\}
 *                                  \qquad\text{ for }k\in 0\ ...\ (length-1)@f$ }
 * 
 * \safe_in_place{a,b}
 * \requires_word_alignment{a,b}
 * 
 * \param[out]  a               Output vector
 * \param[in]   b               Input vector
 * \param[in]   length          Number of elements in vectors `a` and `b`
 * \param[in]   lower_bound     Lower bound of clipping range
 * \param[in]   upper_bound     Upper bound of clipping range
 * \param[in]   b_shr           Right-shift applied to elements of `b`
 * 
 * \return  Headroom of output vector `a`
 * 
 */
headroom_t xs3_clip_vect_s32(
    int32_t* a,
    const int32_t* b,
    const unsigned length,
    const int32_t lower_bound,
    const int32_t upper_bound,
    const right_shift_t b_shr);


/** 
 * \brief Rectify the elements of an `int16_t` vector.
 * 
 * Each output element `a[k]` is set to the value of the corresponding input element 
 * `b[k]` if it is positive, and `a[k]` is set to zero otherwise. 
 * 
 * \low_op{16, @f$a_k \leftarrow \begin\{cases\}b_k & b_k \gt 0 \\ 
 *                                  0 & b_k \leq 0\end\{cases\} 
 *                                  \qquad\text{ for }k\in 0\ ...\ (length-1)@f$ }
 * 
 * \safe_in_place{a,b}
 * \requires_word_alignment{a,b}
 * 
 * \param[out] a        Output vector
 * \param[in] b         Input vector
 * \param[in] length    Number of elements in vectors `a` and `b`
 * 
 * \return  Headroom of the output vector `a`
 */
headroom_t xs3_rect_vect_s16(
    int16_t* a,
    const int16_t* b,
    const unsigned length);


/** 
 * \brief Rectify the elements of an `int32_t` vector.
 * 
 * Each output element `a[k]` is set to the value of the corresponding input element 
 * `b[k]` if it is positive, and `a[k]` is set to zero otherwise. 
 * 
 * \low_op{32, @f$a_k \leftarrow \begin\{cases\}b_k & b_k \gt 0 \\ 
 *                                  0 & b_k \leq 0\end\{cases\} 
 *                                  \qquad\text{ for }k\in 0\ ...\ (length-1)@f$ }
 * 
 * \safe_in_place{a,b}
 * \requires_word_alignment{a,b}
 * 
 * \param[out] a        Output vector
 * \param[in] b         Input vector
 * \param[in] length    Number of elements in vectors `a` and `b`
 * 
 * \return  Headroom of the output vector `a`
 */
headroom_t xs3_rect_vect_s32(
    int32_t* a,
    const int32_t* b,
    const unsigned length);



/** 
 * \brief Convert an `int32_t` vector to an `int16_t` vector.
 * 
 * Each element of `b` is right-shifted `b_shr` bits (rounded towards positive infinity), 
 * saturated to 16-bit bounds and then written to `a`. 
 * 
 * \low_op{32, @f$a_k \leftarrow b_k \cdot 2^\{-b\_shr\}\qquad\text{ for }k\in 0\ ...\ (length-1)@f$ }
 * 
 * \safe_in_place{a,b}
 * \requires_word_alignment{a,b}
 * 
 * \note \li Underflows of negative values result in `-1` rather than `0`.
 * 
 * \note \li This function does not return headroom of the resulting vector. So long as the
 *       value of `b_shr` is chosen to avoid saturation, `a` should have `b_shr` more bits of 
 *       headroom than `b`.
 * 
 * \note \li To avoid saturation, `b_shr >= 16-b_hr` where `b_hr` is the initial headroom of
 *           the vector `b`.
 * 
 * \param[out] a        Output vector
 * \param[in]  b        Input vector
 * \param[in]  length   Number of elements in vectors `a` and `b`
 * \param[in]  b_shr    Right-shift to apply to elements of `b`
 */
void xs3_s32_to_s16(
    int16_t* a,
    const int32_t* b,
    const unsigned length,
    const right_shift_t b_shr);

    
/**
 * \brief Convert an `int16_t` vector to an `int32_t` vector.
 * 
 * Each element of the output vector `a[k]` is set equal to the corresponding element of the
 * input vector `b[k]` left-shifted 8 bits.
 * 
 * (The 8-bit left-shift is due to the ideosyncrasies of the XS3 VPU. It turns out to be much more
 *   efficient to include a factor of 2^8)
 * 
 * 
 * \low_op{32, @f$a_k \leftarrow b_k \cdot 2^\{8\} \qquad\text{ for }k\in 0\ ...\ (length-1)@f$ }
 * 
 * \requires_word_alignment{a,b}
 * 
 * \note \li This function does not return headroom of the resulting vector. The output vector
 *           will have 8 more bits of headroom than the input.
 * 
 * 
 * \param[out] a        Output vector
 * \param[in]  b        Input vector
 * \param[in]  length   Number of elements in vectors `a` and `b`
 */
void xs3_s16_to_s32(
    int32_t* a,
    const int16_t* b,
    const unsigned length);


// headroom_t xs3_sqrt_vect_s16(
//     int16_t* a,
//     const int16_t* b,
//     const unsigned length);


#ifdef __XC__
}   //extern "C"
#endif

#endif //XS3_BFP_LOW_H_