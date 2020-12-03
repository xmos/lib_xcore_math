
#ifndef XS3_BFP_LOW_H_
#define XS3_BFP_LOW_H_

#ifdef __XC__
extern "C" {
#endif



/** Set all elements of an `int16_t` array to the specified value.
 * 
 * \low_op{16, @f$data_k \leftarrow value\qquad\text{ for }k\in 0\ ...\ (length-1)@f$ }
 * 
 * \requires_word_alignment{data}
 * 
 * \param[out] data     Array to set
 * \param[in] value     Value to set
 * \param[in] length    Number of elements in `data`
 */
void xs3_vect_s16_set(
    int16_t data[],
    const int16_t value,
    const unsigned length);

/** Set all elements of an `int32_t` array to the specified value.
 * 
 * \low_op{32, @f$data_k \leftarrow value\qquad\text{ for }k\in 0\ ...\ (length-1)@f$ }
 * 
 * \requires_word_alignment{data}
 * 
 * \param[out] data     Array to set
 * \param[in]  value    Value to set
 * \param[in]  length   Number of elements in `data`
 */
void xs3_vect_s32_set(
    int32_t data[],
    const int32_t value,
    const unsigned length);




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
headroom_t xs3_vect_s16_headroom(
    const int16_t v[], 
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
headroom_t xs3_vect_s32_headroom(
    const int32_t v[],
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
headroom_t xs3_vect_s16_shl(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const left_shift_t shl);

    
#ifdef __XC__

  // For some reason I can't get the static inline functions to compile when included
  // from a .xc file. There's probably some fix I don't know about. This is temporary.
  // @todo Make these work from XC.

#else    
static inline headroom_t xs3_vect_s16_shr(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const right_shift_t shr)
{
    return xs3_vect_s16_shl(a, b, length, -shr);
}

#endif //__XC__

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
headroom_t xs3_vect_s32_shl(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const left_shift_t shl);

    
    
#ifdef __XC__

  // For some reason I can't get the static inline functions to compile when included
  // from a .xc file. There's probably some fix I don't know about. This is temporary.
  // @todo Make these work from XC.

#else    

static inline headroom_t xs3_vect_s32_shr(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const right_shift_t shr)
{
    return xs3_vect_s32_shl(a, b, length, -shr);
}

#endif //__XC__



/**
 * @brief Calculate the output exponent and input shifts needed to perform a
 * BFP vector addition.
 * 
 * This function is used to calcular the output exponent and operand shift parameter values 
 * required to compute the element-wise sum @f$\bar A = \bar B - \bar C@f$ of BFP vectors 
 * @f$\bar B@f$ and @f$\bar C@f$
 * 
 * Logic is identical for `bfp_s16_t`, `bfp_s32_t`, `bfp_complex_s16_t`, `bfp_complex_s32_t`.
 * 
 * The `allow_saturation` parameter is used to specify whether to check for the possibility of 
 * corner-case saturation. For an explanation of corner-case saturation, see \ref saturation.
 * Corner-case saturation is avoided if `allow_saturation` is zero.
 * 
 * For BFP vector addition, corner-case saturation may occur when BFP vectors `B` and `C` have
 * similar magnitudes (in the sense of their @f$L\infty@f$-norm). Specifically, for input BFP 
 * vectors @f$B@f$ and @f$C@f$, the corner case will be detected and avoided when 
 * @f$\left(B_{exp}-B_{hr}\right) = \left(C_{exp}-C_{hr}\right)@f$.
 * 
 * The outputs of this function `b_shr` and `c_shr` can be used with `xs3_vect_s16_add()` and 
 * `xs3_vect_s32_add()` as the shift values for that function's corresponding parameters. The output 
 * `a_exp` is the exponent associated with the result computed by those functions.
 * 
 * If a specific output exponent `desired_exp` is needed for the result, the `b_shr` and `c_shr` 
 * produced by this function can be adjusted according to the following:
 * \code{.c}
 *      exponent_t desired_exp = ...; // Value known a priori
 *      right_shift_t new_b_shr = b_shr + (desired_exp - a_exp);
 *      right_shift_t new_c_shr = c_shr + (desired_exp - a_exp);
 * \endcode
 * Note that using smaller values than necessary for `b_shr` and `c_shr` can result in saturation, 
 * and using larger values may result in unnecessary underflows.
 * 
 * @param[out] a_exp    Exponent @f$A_{exp}@f$ of the result vector @f$\bar A@f$
 * @param[out] b_shr    Right-shift to be applied to vector @f$\bar B@f$
 * @param[out] c_shr    Right-shift to be applied to vector @f$\bar C@f$
 * @param[in]  b_exp    Exponent @f$B_{exp}@f$ associated with @f$\bar B@f$
 * @param[in]  c_exp    Exponent @f$C_{exp}@f$ associated with @f$\bar C@f$
 * @param[in]  b_hr     Headroom @f$B_{hr}@f$ associated with @f$\bar B@f$
 * @param[in]  c_hr     Headroom @f$B_{hr}@f$ associated with @f$\bar B@f$
 * @param[in]  allow_saturation  Whether to avoid corner-case saturation.
 * 
 * \sa xs3_vect_s16_add
 * \sa xs3_vect_s32_add
 * \sa xs3_vect_s16_sub
 * \sa xs3_vect_s32_sub
 */
void xs3_add_sub_vect_calc_params(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    right_shift_t* c_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr,
    const unsigned allow_saturation);
    
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
 * \see bfp_vect_s16_add()
 */
headroom_t xs3_vect_s16_add(
    int16_t a[],
    const int16_t b[], 
    const int16_t c[],
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
 * \see bfp_vect_s32_add()
 */
headroom_t xs3_vect_s32_add(
    int32_t a[],
    const int32_t b[],
    const int32_t c[],
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
 * \see bfp_vect_s16_sub()
 */
headroom_t xs3_vect_s16_sub(
    int16_t a[],
    const int16_t b[],
    const int16_t c[],
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
 * \see bfp_vect_s16_sub()
 */
headroom_t xs3_vect_s32_sub(
    int32_t a[],
    const int32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);



/**
 * @brief Calculate the exponent and output shift for `xs3_vect_s16_mul`.
 * 
 * @param[out] a_exp
 * @param[out] a_shr
 * @param[in]  b_exp
 * @param[in]  c_exp
 * @param[in]  b_hr
 * @param[in]  c_hr
 */
void xs3_vect_s16_mul_calc_params(
    exponent_t* a_exp,
    right_shift_t* a_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr);


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
//! [xs3_vect_s16_mul]
headroom_t xs3_vect_s16_mul(
    int16_t a[],
    const int16_t b[],
    const int16_t c[],
    const unsigned length,
    const right_shift_t a_shr);
//! [xs3_vect_s16_mul]



/**
 * @brief Calculate the exponent and input shifts for `xs3_vect_s32_mul`.
 * 
 * @param[out] a_exp
 * @param[out] b_shr
 * @param[out] c_shr
 * @param[in]  b_exp
 * @param[in]  c_exp
 * @param[in]  b_hr
 * @param[in]  c_hr
 */
void xs3_vect_s32_mul_calc_params(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    right_shift_t* c_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr);

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
//! [xs3_vect_s32_mul]
headroom_t xs3_vect_s32_mul(
    int32_t a[],
    const int32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);
//! [xs3_vect_s32_mul]



void xs3_vect_s16_scalar_mul_calc_params(
    exponent_t* a_exp,
    right_shift_t* sat,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr,
    const unsigned allow_saturation);


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
headroom_t xs3_vect_s16_scalar_mul(
    int16_t a[],
    const int16_t b[],
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
headroom_t xs3_vect_s32_scalar_mul(
    int32_t a[],
    const int32_t b[],
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
headroom_t xs3_vect_s16_abs(
    int16_t a[],
    const int16_t b[],
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
headroom_t xs3_vect_s32_abs(
    int32_t a[],
    const int32_t b[],
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
 *          order) will saturate. If more than `65536` elements are to be summed, it is recommended that
 *          the user instead make multiple calls to `xs3_vect_s16_sum()`, adding together partial sums of 
 *          subsequences of `b` in user code.
 * 
 * \param[in] b         Input vector
 * \param[in] length    Number of elements in vector `b`
 * 
 * \return  Sum of elements in `b`
 */
int32_t xs3_vect_s16_sum(
    const int16_t b[],
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
 *          order) will saturate. If more than `256` elements are to be summed, it is recommended that
 *          the user instead make multiple calls to `xs3_vect_s32_sum()`, adding together partial sums of 
 *          subsequences of `b` in user code.
 * 
 * \param[in] b         Input vector
 * \param[in] length    Number of elements in vector `b`
 * 
 * \return  Sum of elements in `b`
 */
int64_t xs3_vect_s32_sum(
    const int32_t b[],
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
int64_t xs3_vect_s16_dot(
    const int16_t b[],
    const int16_t c[],
    const unsigned length,
    const headroom_t bc_hr);


void xs3_vect_s32_dot_calc_params(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    right_shift_t* c_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr,
    const unsigned length,
    const unsigned allow_saturation);


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
int64_t xs3_vect_s32_dot(
    const int32_t b[],
    const int32_t c[],
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
headroom_t xs3_vect_s16_clip(
    int16_t a[],
    const int16_t b[],
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
headroom_t xs3_vect_s32_clip(
    int32_t a[],
    const int32_t b[],
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
headroom_t xs3_vect_s16_rect(
    int16_t a[],
    const int16_t b[],
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
headroom_t xs3_vect_s32_rect(
    int32_t a[],
    const int32_t b[],
    const unsigned length);


#define XS3_VECT_SQRT_S16_MAX_DEPTH     (15)
#define XS3_VECT_SQRT_S32_MAX_DEPTH     (31)

void xs3_vect_s16_sqrt_calc_params(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    const exponent_t b_exp,
    const right_shift_t b_hr);

void xs3_vect_s32_sqrt_calc_params(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    const exponent_t b_exp,
    const right_shift_t b_hr);

headroom_t xs3_vect_s16_sqrt(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const right_shift_t b_shr,
    const unsigned depth);

headroom_t xs3_vect_s32_sqrt(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const right_shift_t b_shr,
    const unsigned depth);


void xs3_vect_s16_inverse_calc_params(
    exponent_t* a_exp,
    unsigned* scale,
    const int16_t b[],
    const exponent_t b_exp,
    const unsigned length);

void xs3_vect_s32_inverse_calc_params(
    exponent_t* a_exp,
    unsigned* scale,
    const int32_t b[],
    const exponent_t b_exp,
    const unsigned length);

headroom_t xs3_vect_s32_inverse(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const unsigned scale);

void xs3_vect_s16_inverse(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const unsigned scale);



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
void xs3_vect_s32_to_s16(
    int16_t a[],
    const int32_t b[],
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
void xs3_vect_s16_to_s32(
    int32_t a[],
    const int16_t b[],
    const unsigned length);



#ifdef __XC__
}   //extern "C"
#endif

#endif //XS3_BFP_LOW_H_