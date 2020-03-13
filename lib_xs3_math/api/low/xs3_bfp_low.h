
#ifndef XS3_BFP_LOW_H_
#define XS3_BFP_LOW_H_

#ifdef __XC__
extern "C" {
#endif

/**
 * \brief Compute headroom of `int16_t` array `v`
 * 
 * \foperation{16, @f$ min(\ \ \ hr(v_0)\,\ hr(v_1)\,\ ...\,\ hr(v_\{N-1\})\ \ ) @f$ \n\tab where @f$hr(a)@f$ is the headroom of element @f$a@f$  }
 * 
 * \requires_word_alignment{v}
 * 
 * \param[in] v     Array of `int16_t`
 * \param[in] N     Number of elements in `v`
 * 
 * \return  Headroom of the array `v`
 */
headroom_t xs3_cls_array_s16(
    const int16_t* v, 
    const unsigned N);

/**
 * \brief Compute headroom of an `int32_t` array.
 * 
 * \foperation{32, @f$ min(\ \ \ hr(v_0)\,\ hr(v_1)\,\ ...\,\ hr(v_\{N-1\})\ \ ) @f$ \n\tab where @f$hr(a)@f$ is the headroom of element @f$a@f$  }
 * 
 * \requires_word_alignment{v}
 * 
 * \param[in] v     Array of `int32_t`
 * \param[in] N     Number of elements in `v`
 * 
 * \return  Headroom of the array `v`
 */
headroom_t xs3_cls_array_s32(
    const int32_t* v,
    const unsigned N);

/**
 * \brief Perform a signed, saturating arithmetic left shift of an `int16_t` vector.
 * 
 * \foperation{16, @f$a_k \leftarrow b_k \cdot 2^\{shl\} \qquad\text{ for }k\in 0\ ...\ (length-1)@f$ }
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
    const int shl);

/**
 * \brief Perform a signed, saturating arithmetic left shift of an `int32_t` vector.
 * 
 * \foperation{32, @f$a_k \leftarrow b_k \cdot 2^\{shl\} \qquad\text{ for }k\in 0\ ...\ (length-1)@f$ }
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
    const int shl);

    
/**
 * \brief Add together two `int16_t` vectors.
 * 
 * Add together the two `int16_t` vectors `b[]` and `c[]`, placing the result in `a[]`. Each element of `b[]` 
 * or `c[]` has an arithmetic right shift of `b_shr` or `c_shr` bits (respectively) applied before the addition.
 * Negative values of `b_shr` and `c_shr` will left-shift.
 * 
 * \foperation{16, @f$a_k \leftarrow b_k \cdot 2^\{-b\_shr\} + c_k \cdot 2^\{-c\_shr\}\qquad\text{ for }k\in 0\ ...\ (length-1)@f$ }
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
    const int b_shr,
    const int c_shr);

/**
 * \brief Add together two `int32_t` vectors.
 * 
 * Add together the two `int32_t` vectors `b[]` and `c[]`, placing the result in `a[]`. Each element of `b[]` 
 * or `c[]` has an arithmetic right shift of `b_shr` or `c_shr` bits (respectively) applied before the addition.
 * Negative values of `b_shr` and `c_shr` will left-shift.
 * 
 * \foperation{32, @f$a_k \leftarrow b_k \cdot 2^\{-b\_shr\} + c_k \cdot 2^\{-c\_shr\}\qquad\text{ for }k\in 0\ ...\ (length-1)@f$ }
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
    const int b_shr,
    const int c_shr);


/**
 * \brief Subtract one `int16_t` vector from another.
 * 
 * Subtract the `int16_t` vector `c[]` from the `int16_t` vector `b[]`, placing the result in `a[]`. Each 
 * element of `b[]` or `c[]` has an arithmetic right shift of `b_shr` or `c_shr` bits (respectively) applied 
 * before the subtraction. Negative values of `b_shr` and `c_shr` will left-shift.
 * 
 * \foperation{16, @f$a_k \leftarrow b_k \cdot 2^\{-b\_shr\} - c_k \cdot 2^\{-c\_shr\}\qquad\text{ for }k\in 0\ ...\ (length-1)@f$ }
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
    const int b_shr,
    const int c_shr);


/**
 * \brief Subtract one `int32_t` vector from another.
 * 
 * Subtract the `int32_t` vector `c[]` from the `int32_t` vector `b[]`, placing the result in `a[]`. Each 
 * element of `b[]` or `c[]` has an arithmetic right shift of `b_shr` or `c_shr` bits (respectively) applied 
 * before the subtraction. Negative values of `b_shr` and `c_shr` will left-shift.
 * 
 * \foperation{32, @f$a_k \leftarrow b_k \cdot 2^\{-b\_shr\} - c_k \cdot 2^\{-c\_shr\}\qquad\text{ for }k\in 0\ ...\ (length-1)@f$ }
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
 * \return  Headroom of output vector `a[]`
 * 
 * \see bfp_sub_vect_s16()
 */
headroom_t xs3_sub_vect_s32(
    int32_t* a,
    const int32_t* b,
    const int32_t* c,
    const unsigned length,
    const int b_shr,
    const int c_shr);


/**
 * Returns headroom
 */
headroom_t xs3_mul_vect_s16(
    int16_t* a,
    const int16_t* b,
    const int16_t* c,
    const unsigned length,
    const int b_shr,
    const int c_shr);

/**
 * Returns headroom
 */
headroom_t xs3_mul_vect_s32(
    int32_t* a,
    const int32_t* b,
    const int32_t* c,
    const unsigned length,
    const int b_shr,
    const int c_shr);

/**
 * Returns headroom
 */
headroom_t xs3_scalar_mul_vect_s16(
    int16_t* a,
    const int16_t* b,
    const int16_t c,
    const unsigned length,
    const int b_shr);

/**
 * Returns headroom
 */
headroom_t xs3_scalar_mul_vect_s32(
    int32_t* a,
    const int32_t* b,
    const int32_t c,
    const unsigned length,
    const int b_shr);


/** 
 * \brief Compute the absolute value (element-wise) of an `int16_t` vector.
 * 
 * Each output element `a[k]` is set to the absolute value of the corresponding 
 * input element `b[k]` 
 * 
 * \foperation{16, @f$a_k \leftarrow \left| b_k \right|\qquad\text{ for }k\in 0\ ...\ (length-1)@f$ }
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
 * \foperation{32, @f$a_k \leftarrow \left| b_k \right|\qquad\text{ for }k\in 0\ ...\ (length-1)@f$ }
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
 * 
 */
int16_t xs3_sum_s16(
    const int16_t* b,
    const unsigned length,
    const int sat);


/**
 * 
 */
int32_t xs3_sum_s32(
    const int32_t* b,
    const unsigned length,
    const int sat);

/**
 * 
 */
int16_t xs3_dot_s16(
    const int16_t* b,
    const int16_t* c,
    const unsigned length,
    const int b_shr,
    const int c_shr,
    const int sat);

/**
 * 
 */
int32_t xs3_dot_s32(
    const int32_t* b,
    const int32_t* c,
    const unsigned length,
    const int b_shr,
    const int c_shr,
    const int sat);

/**
 * 
 */
headroom_t xs3_clip_vect_s16(
    int16_t* a,
    const int16_t* b,
    const unsigned length,
    const int16_t lower_bound,
    const int16_t upper_bound,
    const int b_shr);

/**
 * 
 */
headroom_t xs3_clip_vect_s32(
    int32_t* a,
    const int32_t* b,
    const unsigned length,
    const int32_t lower_bound,
    const int32_t upper_bound,
    const int b_shr);


/** 
 * \brief Rectify the elements of an `int16_t` vector.
 * 
 * Each output element `a[k]` is set to the value of the corresponding input element 
 * `b[k]` if it is positive, and `a[k]` is set to zero otherwise. 
 * 
 * \foperation{16, @f$a_k \leftarrow \begin\{cases\}b_k & b_k \gt 0 \\ 
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
 * \foperation{32, @f$a_k \leftarrow \begin\{cases\}b_k & b_k \gt 0 \\ 
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
 * \foperation{32, @f$a_k \leftarrow b_k \cdot 2^\{-b\_shr\}\qquad\text{ for }k\in 0\ ...\ (length-1)@f$ }
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
 * \note \li To avoid saturation, `b_shr >= -b_hr` where `b_hr` is the initial headroom of
 *           the vector `b`.
 * 
 * 
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
    const int b_shr);

    
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
 * \foperation{32, @f$a_k \leftarrow b_k \cdot 2^\{8\} \qquad\text{ for }k\in 0\ ...\ (length-1)@f$ }
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