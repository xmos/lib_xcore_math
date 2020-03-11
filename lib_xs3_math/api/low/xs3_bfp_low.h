
#ifndef XS3_BFP_LOW_H_
#define XS3_BFP_LOW_H_

#ifdef __XC__
extern "C" {
#endif

/**
 * Compute headroom of an `int16_t` array.
 * 
 * \note `v` must be word-aligned.
 * 
 * \param[in] v         Array of `int16_t`
 * \param[in] length    Number of elements in `v`
 * 
 * \return  Headroom of the array `v`
 */
headroom_t xs3_cls_array_s16(
    const int16_t* v, 
    const unsigned length);

/**
 * Compute headroom of an `int32_t` array.
 * 
 * \note `v` must be word-aligned.
 * 
 * \param[in] v         Array of `int32_t`
 * \param[in] length    Number of elements in `v`
 * 
 * \return  Headroom of the array `v`
 */
headroom_t xs3_cls_array_s32(
    const int32_t* v,
    const unsigned length);

/**
 * Perform a signed, saturating arithmetic left shift of an `int16_t` vector.
 * 
 * \code
 *      a[:] <-- (b[:] << shl)
 * \endcode
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
 * \note\li Operation can safely be performed in-place if `a == b`. The function perfoms more efficiently 
 *       (both with respect to memory and compute time) when performed in-place.
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
 * Perform a signed, saturating arithmetic left shift of an `int32_t` vector.
 * 
 *      a[:] <-- (b[:] << shl)
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
 * \note\li Operation can safely be performed in-place if `a == b`. The function perfoms more efficiently 
 *       (both with respect to memory and compute time) when performed in-place.
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
 * Add together two `int16_t` vectors.
 * 
 *      a[:] <-- (b[:] >> b_shr) + (c[:] >> c_shr)
 * 
 * Add together the two `int16_t` vectors `b[]` and `c[]`, placing the result in `a[]`. Each element of `b[]` or `c[]` has an
 * arithmetic right shift of `b_shr` or `c_shr` bits (respectively) applied before the addition. Negative values of
 * `b_shr` and `c_shr` will left-shift.
 * 
 * The headroom of the resulting vector is returned. 
 * 
 * \note \li To reduce memory requirements this operation can safely be applied in-place on vector `b` or vector `c` by 
 *       passing the address of `b` or `c` as the output address `a`.
 * 
 * \warning \li Both shifts (if they are negative) and the addition are saturating operations, and will saturate to the
 *       symmetric 16-bit range  (see: TODO).
 * 
 * \warning \li Where negative values underflow due to a right-shift, the resulting value is `-1`, rather than `0`. (see: TODO)
 * 
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
 * Add together two `int32_t` vectors.
 * 
 *      a[:] <-- (b[:] >> b_shr) + (c[:] >> c_shr)
 * 
 * Add together the two `int32_t` vectors `b[]` and `c[]`, placing the result in `a[]`. Each element of `b[]` or `c[]` has an
 * arithmetic right shift of `b_shr` or `c_shr` bits (respectively) applied before the addition. Negative values of
 * `b_shr` and `c_shr` will left-shift.
 * 
 * The headroom of the resulting vector is returned. 
 * 
 * \note \li To reduce memory requirements this operation can safely be applied in-place on vector `b` or vector `c` by 
 *       passing the address of `b` or `c` as the output address `a`.
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
 * Subtract one `int16_t` vector from another.
 * 
 *      a[:] <-- (b[:] >> b_shr) - (c[:] >> c_shr)
 * 
 * Subtract the `int16_t` vector `c[]` from the `int16_t` vector `b[]`, placing the result in `a[]`. Each 
 * element of `b[]` or `c[]` has an arithmetic right shift of `b_shr` or `c_shr` bits (respectively) applied 
 * before the subtraction. Negative values of `b_shr` and `c_shr` will left-shift.
 * 
 * The headroom of the resulting vector is returned. 
 * 
 * \note \li To reduce memory requirements this operation can safely be applied in-place on vector `b` or vector `c` by 
 *       passing the address of `b` or `c` as the output address `a`.
 * 
 * \warning \li Both shifts (if they are negative) and the addition are saturating operations, and will saturate to the
 *       symmetric 16-bit range  (see: TODO).
 * 
 * \warning \li Where negative values underflow due to a right-shift, the resulting value is `-1`, rather than `0`. 
 *              (see: TODO)
 * 
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
 * Subtract one `int32_t` vector from another.
 * 
 *      a[:] <-- (b[:] >> b_shr) - (c[:] >> c_shr)
 * 
 * Subtract the `int32_t` vector `c[]` from the `int32_t` vector `b[]`, placing the result in `a[]`. Each 
 * element of `b[]` or `c[]` has an arithmetic right shift of `b_shr` or `c_shr` bits (respectively) applied 
 * before the subtraction. Negative values of `b_shr` and `c_shr` will left-shift.
 * 
 * The headroom of the resulting vector is returned. 
 * 
 * \note \li To reduce memory requirements this operation can safely be applied in-place on vector `b` or vector `c` by 
 *       passing the address of `b` or `c` as the output address `a`.
 * 
 * \warning \li Both shifts (if they are negative) and the addition are saturating operations, and will saturate to the
 *       symmetric 32-bit range  (see: TODO).
 * 
 * \warning \li Where negative values underflow due to a right-shift, the resulting value is `-1`, rather than `0`. 
 *              (see: TODO)
 * 
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
 * Returns headroom
 */
headroom_t xs3_abs_vect_s16(
    int16_t* a,
    const int16_t* b,
    const unsigned length);

/**
 * Returns headroom
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
 * 
 */
headroom_t xs3_rect_vect_s16(
    int16_t* a,
    const int16_t* b,
    const unsigned length);

/**
 * 
 */
headroom_t xs3_rect_vect_s32(
    int32_t* a,
    const int32_t* b,
    const unsigned length);



/**
 * 
 */
headroom_t xs3_s32_to_s16(
    int16_t* a,
    const int32_t* b,
    const unsigned length,
    const int sat);

    
/**
 * 
 */
headroom_t xs3_s16_to_s32(
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