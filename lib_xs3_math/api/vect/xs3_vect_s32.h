#ifndef XS3_VECT_S32_H_
#define XS3_VECT_S32_H_

#include "xs3_math_conf.h"
#include "xs3_math_types.h"
#include "xs3_util.h"

#ifdef __XC__
extern "C" {
#endif

#define XS3_VECT_SQRT_S32_MAX_DEPTH     (31)






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
void xs3_vect_add_sub_calc_params(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    right_shift_t* c_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr,
    const unsigned allow_saturation);


headroom_t xs3_vect_ch_pair_s32_headroom(
    const ch_pair_s32_t a[],
    const unsigned length);

void xs3_vect_ch_pair_s32_set(
    ch_pair_s32_t data[],
    const int32_t ch_a,
    const int32_t ch_b,
    const unsigned length);

headroom_t xs3_vect_ch_pair_s32_shl(
    ch_pair_s32_t a[],
    const ch_pair_s32_t b[],
    const unsigned length,
    const left_shift_t shl);

headroom_t xs3_vect_ch_pair_s32_shr(
    ch_pair_s32_t a[],
    const ch_pair_s32_t b[],
    const unsigned length,
    const right_shift_t shr);


/**
 * @todo 
 */
void xs3_vect_complex_mag_calc_params(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    const exponent_t b_exp,
    const headroom_t b_hr,
    const unsigned allow_saturation);


headroom_t xs3_vect_complex_s32_add(
    complex_s32_t a[],
    const complex_s32_t b[],
    const complex_s32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


void xs3_vect_complex_s32_complex_mul_calc_params(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    right_shift_t* c_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr,
    const unsigned allow_saturation);

/**
 * @brief Compute the complex product of two complex 32-bit vectors.
 * 
 * Multiply the vector @vector{b} element-wise with the vector @vector{c}, placing the final result in the 
 * vector @vector{a}. This function saturates the output elements to the symmetric 32-bit range. To avoid 
 * saturation, 3 scaling factors are applied during the operation. 
 * 
 * Prior to computing their product, the elements of @vector{b} and @vector{c} are respectively scaled by the factors 
 * @math{2^{-b\_shr}} and @math{2^{-c\_shr}}. This can be used to scale the elements up or down (without rounding). Note
 * that the results of this scaling are also saturated, and so in general neither input vector should be scaled up by
 * more than its headroom.
 * 
 * Finally, a scale factor of @math{2^{-30}} is applied to the 64-bit products of elements. Rounding is applied with
 * this scaling.
 * 
 * The parameters `a[]`, `b[]` and `c[]` represent the vectors @vector{a}, @vector{b} and @vector{c} respectively.
 * 
 * The parameter `length` is the number of elements in each of the vectors.
 * 
 * The parameters `b_shr` and `c_shr` define the scaling factors applied to elements of @vector{b} and @vector{c} 
 * respectively.
 * 
 * This function returns the headroom of the output vector @vector{a}.
 * 
 * @low_op{32, @f$ Re\\{a_k\\} \leftarrow \left( \frac{Re\\{b_k\\}}{2^{b\_shr}} \cdot \frac{Re\\{c_k\\}}{2^{c\_shr}} 
 *              - \frac{Im\\{b_k\\}}{2^{b\_shr}} \cdot \frac{Im\\{c_k\\}}{2^{c\_shr}} \right) \cdot 2^{-30} \\
 *          Im\\{a_k\\} \leftarrow \left( \frac{Re\\{b_k\\}}{2^{b\_shr}} \cdot \frac{Im\\{c_k\\}}{2^{c\_shr}} 
 *              + \frac{Im\\{b_k\\}}{2^{b\_shr}} \cdot \frac{Re\\{c_k\\}}{2^{c\_shr}} \right) \cdot 2^{-30} \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) @f$}
 * 
 * @par Maximizing Precision:
 *      TODO - Guidelines for keeping maximal precision with this function.
 * 
 * @par Safe In-Place Computation:
 *      The output of this function can be safely computed in-place if the output `a[]` is equal to the input pointer 
 *      `b[]`. Using in-place operations may reduce peak memory requirements.
 * 
 * @par Word Alignment Required:
 *      Parameters `a[]`, `b[]`, and `c[]` must begin at a word-aligned (4-byte) address. For more about alignment 
 *      requirements, see \ref vector_alignment.
 * 
 * @param[out] a        Complex output vector @vector{a}.
 * @param[in]  b        Complex input vector @vector{b}.
 * @param[in]  c        Complex input vector @vector{c}.
 * @param[in]  length   Number of elements in vectors @vector{a}, @vector{b}, and @vector{c}.
 * @param[in]  b_shr    Right-shift applied to elements of @vector{b}.
 * @param[in]  c_shr    Right-shift applied to elements of @vector{c}.
 * 
 * @return The headroom of the output vector @vector{a} is returned.
 */
headroom_t xs3_vect_complex_s32_complex_mul(
    complex_s32_t a[],
    const complex_s32_t b[],
    const complex_s32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);



/**
 * @brief Compute the complex product of a complex 32-bit vector with the complex conjugate of another.
 * 
 * Multiply the vector @vector{b} element-wise with the complex conjugate of vector @vector{c}, placing the final result 
 * in the vector @vector{a}. This function saturates the output elements to the symmetric 32-bit range. To avoid 
 * saturation, 3 scaling factors are applied during the operation. 
 * 
 * Prior to computing their product, the elements of @vector{b} and @vector{c^*} are respectively scaled by the factors 
 * @math{2^{-b\_shr}} and @math{2^{-c\_shr}}. This can be used to scale the elements up or down (without rounding). Note
 * that the results of this scaling are also saturated, and so in general neither input vector should be scaled up by
 * more than its headroom.
 * 
 * Finally, a scale factor of @math{2^{-30}} is applied to the 64-bit products of elements. Rounding is applied with
 * this scaling.
 * 
 * The parameters `a[]`, `b[]` and `c[]` represent the vectors @vector{a}, @vector{b} and @vector{c} respectively.
 * 
 * The parameter `length` is the number of elements in each of the vectors.
 * 
 * The parameters `b_shr` and `c_shr` define the scaling factors applied to elements of @vector{b} and @vector{c} 
 * respectively.
 * 
 * This function returns the headroom of the output vector @vector{a}.
 * 
 * @low_op{32, @f$ Re\\{a_k\\} \leftarrow \left( \frac{Re\\{b_k\\}}{2^{b\_shr}} \cdot \frac{Re\\{c_k\\}}{2^{c\_shr}} 
 *              + \frac{Im\\{b_k\\}}{2^{b\_shr}} \cdot \frac{Im\\{c_k\\}}{2^{c\_shr}} \right) \cdot 2^{-30} \\
 *          Im\\{a_k\\} \leftarrow \left( \frac{Im\\{b_k\\}}{2^{b\_shr}} \cdot \frac{Re\\{c_k\\}}{2^{c\_shr}} 
 *              - \frac{Re\\{b_k\\}}{2^{b\_shr}} \cdot \frac{Im\\{c_k\\}}{2^{c\_shr}} \right) \cdot 2^{-30} \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) @f$}
 * 
 * @par Maximizing Precision:
 *      TODO - Guidelines for keeping maximal precision with this function.
 * 
 * @par Safe In-Place Computation:
 *      The output of this function can be safely computed in-place if the output `a[]` is equal to the input pointer 
 *      `b[]`. Using in-place operations may reduce peak memory requirements.
 * 
 * @par Word Alignment Required:
 *      Parameters `a[]`, `b[]`, and `c[]` must begin at a word-aligned (4-byte) address. For more about alignment 
 *      requirements, see \ref vector_alignment.
 * 
 * @param[out] a        Complex output vector @vector{a}.
 * @param[in]  b        Complex input vector @vector{b}.
 * @param[in]  c        Complex input vector @vector{c}.
 * @param[in]  length   Number of elements in vectors @vector{a}, @vector{b}, and @vector{c}.
 * @param[in]  b_shr    Right-shift applied to elements of @vector{b}.
 * @param[in]  c_shr    Right-shift applied to elements of @vector{c}.
 * 
 * @return The headroom of the output vector @vector{a} is returned.
 */
headroom_t xs3_vect_complex_s32_conj_mul(
    complex_s32_t a[],
    const complex_s32_t b[],
    const complex_s32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);



/**
 * @brief Get headroom of complex 32-bit vector.
 * 
 */
headroom_t xs3_vect_complex_s32_headroom(
    const complex_s32_t a[], 
    const unsigned length);

/**
 * Returns headroom
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 */
headroom_t xs3_vect_complex_s32_mag(
    int32_t a[],
    const complex_s32_t b[],
    const unsigned length,
    const right_shift_t b_shr,
    const complex_s32_t* rot_table,
    const unsigned table_rows);



void xs3_vect_complex_s32_real_mul_calc_params(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    right_shift_t* c_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr,
    const unsigned allow_saturation);

/**
 * @brief Multiply a complex 32-bit vector by a real 32-bit vector.
 * 
 * Multiply the complex vector @vector{b} element-wise with the real vector @vector{c}, placing the final result in the 
 * complex vector @vector{a}. This function saturates the output elements to the symmetric 32-bit range. To avoid 
 * saturation, 3 scaling factors are applied during the operation. 
 * 
 * Prior to computing their product, the elements of @vector{b} and @vector{c} are respectively scaled by the factors 
 * @math{2^{-b\_shr}} and @math{2^{-c\_shr}}. This can be used to scale the elements up or down (without rounding). Note
 * that the results of this scaling are also saturated, and so in general neither input vector should be scaled up by
 * more than its headroom.
 * 
 * Finally, a scale factor of @math{2^{-30}} is applied to the 64-bit products of elements. Rounding is applied with
 * this scaling.
 * 
 * The parameters `a[]`, `b[]` and `c[]` represent the vectors @vector{a}, @vector{b} and @vector{c} respectively.
 * 
 * The parameter `length` is the number of elements in each of the vectors.
 * 
 * The parameters `b_shr` and `c_shr` define the scaling factors applied to elements of @vector{b} and @vector{c} 
 * respectively.
 * 
 * This function returns the headroom of the output vector @vector{a}.
 * 
 * @low_op{32, @f$ Re\\{a_k\\} \leftarrow \left( \frac{Re\\{b_k\\}}{2^{b\_shr}}\right) \left(\frac{c_k}{2^{c\_shr}}\right) \cdot 2^{-30} \\
 *                 Im\\{a_k\\} \leftarrow \left( \frac{Im\\{b_k\\}}{2^{b\_shr}}\right) \left(\frac{c_k}{2^{c\_shr}}\right) \cdot 2^{-30} \\
 *              \qquad\text{ for }k\in 0\ ...\ (length-1)@f$ }
 * 
 * @par Maximizing Precision:
 *      TODO - Guidelines for keeping maximal precision with this function.
 * 
 * @par Safe In-Place Computation:
 *      The output of this function can be safely computed in-place if the output `a[]` is equal to the input pointer 
 *      `b[]`. Using in-place operations may reduce peak memory requirements.
 * 
 * @par Word Alignment Required:
 *      Parameters `a[]`, `b[]`, and `c[]` must begin at a word-aligned (4-byte) address. For more about alignment 
 *      requirements, see \ref vector_alignment.
 * 
 * @param[out] a        Complex output vector @vector{a}.
 * @param[in]  b        Complex input vector @vector{b}.
 * @param[in]  c        Real input vector @vector{c}.
 * @param[in]  length   Number of elements in vectors @vector{a}, @vector{b}, and @vector{c}.
 * @param[in]  b_shr    Right-shift applied to elements of @vector{b}.
 * @param[in]  c_shr    Right-shift applied to elements of @vector{c}.
 * 
 * @return The headroom of the output vector @vector{a} is returned.
 */
headroom_t xs3_vect_complex_s32_real_mul(
    complex_s32_t a[],
    const complex_s32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);

/**
 * @todo
 */
headroom_t xs3_vect_complex_s32_real_scale(
    complex_s32_t a[],
    const complex_s32_t b[],
    const int32_t c,
    const unsigned length,
    const right_shift_t b_shr);


/**
 * 
 * 
 */
void xs3_vect_complex_s32_scale_calc_params(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    right_shift_t* alpha_shr,
    const exponent_t b_exp,
    const exponent_t alpha_exp,
    const headroom_t b_hr,
    const headroom_t alpha_hr,
    const unsigned allow_saturation);

/**
 * @brief Compute complex product of a complex 32-bit vector with a complex 32-bit scalar.
 * 
 * Multiply the vector @vector{b} with the scalar @math{c}, placing the final result in the 
 * vector @vector{a}. This function saturates the output elements to the symmetric 32-bit range. To avoid 
 * saturation, 2 scaling factors are applied during the operation. 
 * 
 * Prior to computing their product, the elements of @vector{b} are scaled by the factor @math{2^{-b\_shr}}. This can 
 * be used to scale the elements up or down (without rounding). Note that the results of this scaling are also 
 * saturated, and so in general @vector{b} should not be scaled up by more than its headroom.
 * 
 * Finally, a scale factor of @math{2^{-30}} is applied to the 64-bit products of elements. Rounding is applied with
 * this scaling.
 * 
 * The parameters `a[]` and `b[]` represent the vectors @vector{a} and @vector{b} respectively.
 * 
 * The parameters `c_real` and `c_imag` represent the real and imaginary parts of the complex scalar @math{c}.
 * 
 * The parameter `length` is the number of elements in each of the vectors.
 * 
 * The parameter `b_shr` defines the scaling factor applied to elements of @vector{b}.
 * 
 * This function returns the headroom of the output vector @vector{a}.
 * 
 * @low_op{32, @f$ Re\\{a_k\\} \leftarrow \left( \frac{Re\\{b_k\\}}{2^{b\_shr}} \cdot Re\\{c\\} 
 *              - \frac{Im\\{b_k\\}}{2^{b\_shr}} \cdot Im\\{c\\} \right) \cdot 2^{-30} \\
 *          Im\\{a_k\\} \leftarrow \left( \frac{Re\\{b_k\\}}{2^{b\_shr}} \cdot Im\\{c\\} 
 *              + \frac{Im\\{b_k\\}}{2^{b\_shr}} \cdot Re\\{c\\} \right) \cdot 2^{-30} \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) @f$}
 * 
 * @par Maximizing Precision:
 *      TODO - Guidelines for keeping maximal precision with this function.
 * 
 * @par Safe In-Place Computation:
 *      The output of this function can be safely computed in-place if the output `a[]` is equal to the input pointer 
 *      `b[]`. Using in-place operations may reduce peak memory requirements.
 * 
 * @par Word Alignment Required:
 *      Parameters `a[]` and `b[]` must begin at a word-aligned (4-byte) address. For more about alignment 
 *      requirements, see \ref vector_alignment.
 * 
 * @param[out] a        Complex output vector @vector{a}.
 * @param[in]  b        Complex input vector @vector{b}.
 * @param[in]  c_real   Real part of @math{c}
 * @param[in]  c_imag   Imaginary part of @math{c}
 * @param[in]  length   Number of elements in vectors @vector{a} and @vector{b}.
 * @param[in]  b_shr    Right-shift applied to elements of @vector{b}.
 * 
 * @return The headroom of the output vector @vector{a} is returned.
 */
headroom_t xs3_vect_complex_s32_scale(
    complex_s32_t a[],
    const complex_s32_t b[],
    const int32_t c_real,
    const int32_t c_imag,
    const unsigned length,
    const right_shift_t b_shr);



void xs3_vect_complex_s32_set(
    complex_s32_t data[],
    const int32_t real_part,
    const int32_t imag_part,
    const unsigned length);

headroom_t xs3_vect_complex_s32_shl(
    complex_s32_t a[],
    const complex_s32_t b[],
    const unsigned length,
    const left_shift_t shl);




headroom_t xs3_vect_complex_s32_shr(
    complex_s32_t a[],
    const complex_s32_t b[],
    const unsigned length,
    const right_shift_t shr);

/**
 * @todo
 */
void xs3_vect_complex_s32_squared_mag_calc_params(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    const exponent_t b_exp,
    const headroom_t b_hr,
    const unsigned allow_saturation);

/**
 * @brief Computes the squared magnitudes of elements of a complex 32-bit vector.
 * 
 * Computes the squared magnitudes of each element of the input vector @vector{b}. The results are output as vector
 * @vector{a}. Results are saturated to symmetric 32-bit bounds. To avoid saturation and underflow, 2 scaling factors
 * are applied during this operation.
 * 
 * The first scaling is given by `b_shr` (scales by @math{2^{b\_shr}}) and occurs on the real and imaginary parts of 
 * each element of @vector{b} prior to squaring them. The second scale is by the constant factor @math{2^{-30}} and 
 * happens after the real and imaginary parts are summed.
 * 
 * The parameter `a[]` represents the vector of squared magnitudes @vector{a}.
 * 
 * The parameter `b` represents the complex input vector @vector{b}.
 * 
 * The parameter `length` is the number of elements in each of the vectors.
 * 
 * The parameter `b_shr` is the right-shift applied to each element of @vector{b} to avoid saturation and underflow.
 * 
 * @low_op{16, @f$ 
 *                  v_k = b_k \cdot 2^{-b\_shr} \\
 *                  a_k \leftarrow ((Re\\{v_k\\})^2 + (Im\\{v_k\\})^2)\cdot 2^{-30} \\
 *                      \qquad\text{ for }k\in 0\ ...\ (length-1)
 *             @f$}
 * 
 * @par Maximizing Precision:
 *      TODO - Guidelines for keeping maximal precision with this function.
 * 
 * @param[out] a        The output vector @vector{a}.
 * @param[in]  b_real   The real part of input vector @vector{b}.
 * @param[in]  b_imag   The imaginary part of input vector @vector{b}.
 * @param[in]  length   The number of elements in vector @vector{b}.
 * @param[in]  sat      The right-shift applied to the results.
 */
headroom_t xs3_vect_complex_s32_squared_mag(
    int32_t a[],
    const complex_s32_t b[],
    const unsigned length,
    const right_shift_t b_shr);

/**
 * @brief Subtracts one complex 32-bit vector from another.
 * 
 */
headroom_t xs3_vect_complex_s32_sub(
    complex_s32_t a[], 
    const complex_s32_t b[],
    const complex_s32_t c[],
    const unsigned length, 
    const right_shift_t b_shr,
    const right_shift_t c_shr);



void xs3_vect_complex_s32_sum_calc_params(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    const exponent_t b_exp,
    const headroom_t b_hr,
    const unsigned length,
    const unsigned allow_saturation);


/**
 * @brief Compute the sum of elements of a complex 32-bit vector.
 * 
 * This function computes the 40-bit complex sum of elements of vector @vector{b}. Prior to being added, each element 
 * is scaled down by a factor @math{2^{b\_shr}}. This function does not support scaling up the elements of @vector{b}, 
 * so `b_shr` must be non-negative. The sum saturates at symmetric 40-bit boundaries. The result is output through
 * @math{a}, with the real and imaginary parts sign-extended to 64 bits.
 * 
 * @low_op{32, @f$ a \leftarrow \sum_{k=0}^{length-1} \left( \frac{Re\\{b_k\\}}{2^{b\_shr}} 
 *                                                  + j\cdot \frac{Im\\{b_k\\}}{2^{b\_shr}} \right) 
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)   @f$}
 * 
 * @par Maximizing Precision:
 *      TODO - Guidelines for keeping maximal precision with this function.
 * 
 * @param[out] a        The result @math{a}.
 * @param[in]  b        The complex input vector @vector{b}.
 * @param[in]  length   The number of elements in vector @vector{b}.
 * @param[in]  b_shr    The right-shift applied to elements of @vector{b}.
 */
void xs3_vect_complex_s32_sum(
    complex_s64_t* a,
    const complex_s32_t b[],
    const unsigned length,
    const right_shift_t b_shr);




/**
 * @brief Reverses the order of the tail of a complex 32-bit vector.
 * 
 * Reverses the tail of the complex 32-bit vector @vect{X}. In other words, the first element @math{X[0]} remains where 
 * it is, and the remaining @math{N-1} elements are rearranged to have their order reversed.
 * 
 * @note This function is used when performing a mono FFT.
 * @par
 * @note Saturation logic may be applied to the real or imaginary parts of elements (@ref saturation).
 * 
 * @param[in]   x   The vector to have its tail reversed.
 * @param[in]   N   The length of `x`.
 */
void xs3_vect_complex_s32_tail_reverse(
    complex_s32_t x[],
    const unsigned N);

/** 
 * @brief Convert a complex 32-bit vector into a complex 16-bit vector.
 * 
 * Each element of the input vector @vector{b} scaled by @math{2^{-b\_shr}} abd assigned to the corresponding element 
 * of output vector @vector{b}.
 * 
 * The parameters `a_real[]` and `a_imag[]` together represent the complex 16-bit output vector @vector{b}.
 * 
 * The parameter `b[]` represents the complex 32-bit input vector @vector{a}.
 * 
 * The parameter `length` is the number of elements in each of the vectors.
 * 
 * @low_op{32, @f$ Re\\{a_k\\} \leftarrow \frac{Re\\{b_k\\}}{2^{b\_shr}} \\
 *                 Im\\{a_k\\} \leftarrow \frac{Im\\{b_k\\}}{2^{b\_shr}} \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) @f$}
 * 
 * @par Maximizing Precision:
 *      To maintain the greatest possible precision, the value provided for `b_shr` should be in the range
 *      @math{-hr \le b\_shr \le 16 - hr} where @math{hr} is the headroom of vector @vector{b}.
 * 
 * @param[out] a_real   Real part of complex output vector @vector{b}.
 * @param[out] a_imag   Imaginary part of complex output vector @vector{b}.
 * @param[in]  b        Complex input vector @vector{a}.
 * @param[in]  length   Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]  b_shr    Right-shift applied to elements of @vector{b}.
 * 
 */
void xs3_vect_complex_s32_to_complex_s16(
    int16_t a_real[],
    int16_t a_imag[],
    const complex_s32_t b[],
    const unsigned length,
    const right_shift_t b_shr);

/** Calculate the absolute sum of a 32-bit BFP vector.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Conceptually, the operation performed is:
 *      A <-  ( |B[0]| + |B[1]| + ... + |B[N-1]| )
 *        where A is a scalar
 *              B[] is a BFP vector of size N
 * 
 * \param[in] b         Input vector
 * \param[in] length    Length of input vector
 * \param[in] b_shr     Right-shift applied to elements of `b`
 * 
 * \return  Absolute sum
 */
int64_t xs3_vect_s32_abs_sum(
    const int32_t b[],
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
 * \see bfp_s32_add()
 */
headroom_t xs3_vect_s32_add(
    int32_t a[],
    const int32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);

/** Find the index of the maximum element of a 32-bit integer vector.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Conceptually, the operation performed is:
 *      result <- argmax[i]{ A[i] }
 *        where A[] is an integer vector of size N
 * 
 * \param a         Input vector
 * \param length    Length of the vector `a`
 * 
 * \return         Index of the maximum element of `a`
 */
unsigned xs3_vect_s32_argmax(
    const int32_t a[],
    const unsigned length);

/** Find the index of the minimum element of a 32-bit integer vector.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Conceptually, the operation performed is:
 *      result <- argmin[i]{ A[i] }
 *        where A[] is an integer vector of size N
 * 
 * \param a         Input vector
 * \param length    Length of the vector `a`
 * 
 * \return         Index of the minimum element of `a`.
 */
unsigned xs3_vect_s32_argmin(
    const int32_t a[],
    const unsigned length);

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




void xs3_vect_s32_energy_calc_params(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    const unsigned length,
    const exponent_t b_exp,
    const headroom_t b_hr);
    

right_shift_t xs3_vect_s32_energy_calc_shift(
    const unsigned length,
    const headroom_t b_hr);

/** Calculate the energy of a 32-bit BFP vector.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Conceptually, the operation performed is:
 *      A <-  ( (B[0])^2 + (B[1])^2 + ... + (B[N-1])^2 )
 *        where A is a scalar
 *              B[] is a BFP vector of size N
 * 
 * \param[in] b         Input vector
 * \param[in] length    Length of input vector
 * \param[in] b_shr     Right-shift applied to elements of `b`
 * 
 * \return  Energy
 */
int64_t xs3_vect_s32_energy(
    const int32_t b[],
    const unsigned length,
    const right_shift_t b_shr);

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

/** Find the maximum value in a 32-bit integer vector.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Conceptually, the operation performed is:
 *      result <- max( A[0], A[1], A[N-1] )
 *        where A[] is an integer vector of size N
 * 
 * \param[in] a         Input vector
 * \param[in] length    Length of input vector
 * 
 * \return  Maximum value of `a`
 */
int32_t xs3_vect_s32_max(
    const int32_t a[],
    const unsigned length);

/** Find the minimum value in a 32-bit integer vector.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Conceptually, the operation performed is:
 *      result <- min( A[0], A[1], A[N-1] )
 *        where A[] is an integer vector of size N
 * 
 * \param[in] a         Input vector
 * \param[in] length    Length of input vector
 * 
 * \return  Minimum value of `a`
 */
int32_t xs3_vect_s32_min(
    const int32_t a[],
    const unsigned length);



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
headroom_t xs3_vect_s32_scale(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const int32_t alpha,
    const right_shift_t b_shr);

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

    

headroom_t xs3_vect_s32_shr(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const right_shift_t shr);





void xs3_vect_s32_sqrt_calc_params(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    const exponent_t b_exp,
    const right_shift_t b_hr);

headroom_t xs3_vect_s32_sqrt(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const right_shift_t b_shr,
    const unsigned depth);

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
 * \see bfp_s16_sub()
 */
headroom_t xs3_vect_s32_sub(
    int32_t a[],
    const int32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);

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




#ifdef __XC__
}   //extern "C"
#endif

#endif //XS3_VECT_S32_H_