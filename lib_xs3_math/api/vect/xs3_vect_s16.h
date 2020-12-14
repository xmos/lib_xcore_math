#ifndef XS3_VECT_S16_H_
#define XS3_VECT_S16_H_

#include "xs3_math_conf.h"
#include "xs3_math_types.h"
#include "xs3_util.h"

#ifdef __XC__
extern "C" {
#endif


/**
 * Maximum bit-depth that can be calculated by xs3_vect_s16_sqrt().
 * 
 * @sa xs3_vect_s16_sqrt
 */
#define XS3_VECT_SQRT_S16_MAX_DEPTH     (15)


/**
 * @brief Calculate the headroom of a 16-bit channel-pair vector.
 * 
 * The headroom of an N-bit integer is the number of bits that the integer's value may be left-shifted
 * without any information being lost. Equivalently, it is one less than the number of leading sign bits.
 * 
 * The headroom of a `ch_pair_s16_t` struct is the minimum of the headroom of each of its 16-bit fields,
 * `ch_a` and `ch_b`.
 * 
 * The headroom of a 16-bit channel-pair vector is the minimum of the headroom of each of its `ch_pair_s16_t`
 * elements.
 * 
 * This function efficiently traverses the elements of `a[]` to determine its headroom.
 * 
 * @param[in]   x       The 16-bit channel-pair vector to compute the headroom of
 * @param[in]   length  The number of elements in `a[]`
 * 
 * @returns     The headroom of `a[]` 
 * 
 * @sa xs3_vect_ch_pair_s32_headroom
 * @sa xs3_vect_s16_headroom
 * @sa xs3_vect_s32_headroom
 * @sa xs3_vect_complex_s16_headroom
 * @sa xs3_vect_complex_s32_headroom
 */
headroom_t xs3_vect_ch_pair_s16_headroom(
    const ch_pair_s16_t x[],
    const unsigned length);


/**
 * @brief Set the elements of a 16-bit channel-pair vector to a specified value.
 * 
 * Each `x[k].ch_a` is set to `ch_a`, and each `x[k].ch_b` is set to `ch_b` (for `0 < k <= length`).
 * 
 * @low_op{16, @f$ 
 *      ChA\\{x_k\\} \leftarrow ch\_a \\
 *      ChB\\{x_k\\} \leftarrow ch\_b \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) 
 *  @f$ }
 * 
 * @param[out]  x       The 16-bit channel-pair vector whose elements are to be set
 * @param[in]   ch_a    The value to set each `x[k].ch_a` to
 * @param[in]   ch_b    The value to set each `x[k].ch_b` to
 * @param[in]   length  The number of elements of `x[]`
 * 
 * @sa xs3_vect_ch_pair_s16_set
 * @sa xs3_vect_s16_set
 * @sa xs3_vect_s32_set
 * @sa xs3_vect_complex_s16_set
 * @sa xs3_vect_complex_s32_set
 */
void xs3_vect_ch_pair_s16_set(
    ch_pair_s16_t x[],
    const int16_t ch_a,
    const int16_t ch_b,
    const unsigned length);


/**
 * @brief Left-shift each element of a 16-bit channel-pair vector by a specified number of bits.
 * 
 * Left-shift each element of @vector{b} by @math{b\_shl} bits, placing the result in the corresponding
 * element of @vector{a}. This is equivalent to multiplying each element of @vector{b} by @math{2^{b\_shl}}.
 * 
 * The parameters `a[]` and `b[]` represent vectors @vector{a} and @vector{b} respectively.
 * 
 * `length` is the length of @vector{b} and @vector{a}, in elements.
 * 
 * `b_shl` is the number of bits to left-shift each element of @vector{b}. If `b_shl` is negative, the
 * elements of @vector{b} are right-shifted instead.
 * 
 * This function returns the headroom of output vector @vector{a}.
 * 
 * @low_op{32, @f$ ChA\\{a_k\\} floor\!\left( \leftarrow ChA\\{b_k\\} \cdot {2^{b\_shl}} \right) \\
 *                 ChB\\{a_k\\} floor\!\left( \leftarrow ChB\\{b_k\\} \cdot {2^{b\_shl}} \right) \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) @f$}
 * 
 * @note This function saturates the output elements to the symmetric 32-bit range.
 * @par
 * @note This function can safely operate in-place if `a` and `b` point to the same vector.
 * 
 * @param[out]  a           The output vector @vector{a}
 * @param[in]   b           The input vector @vector{b}
 * @param[in]   length      The number of elements in @vector{a} and @vector{b}
 * @param[in]   b_shl       The number of bits @math{b\_shl} to left-shift the elements of @vector{b}
 * 
 * @returns     The headroom of `a[]`
 * 
 * @sa xs3_vect_ch_pair_s32_shr
 */
headroom_t xs3_vect_ch_pair_s16_shl(
    ch_pair_s16_t a[],
    const ch_pair_s16_t b[],
    const unsigned length,
    const left_shift_t shl);


headroom_t xs3_vect_ch_pair_s16_shr(
    ch_pair_s16_t a[],
    const ch_pair_s16_t b[],
    const unsigned length,
    const right_shift_t shr);


headroom_t xs3_vect_complex_s16_add(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const int16_t c_real[],
    const int16_t c_imag[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


void xs3_vect_complex_s16_mul_prepare(
    exponent_t* a_exp,
    right_shift_t* a_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr,
    const unsigned allow_saturation);


/**
 * @brief Compute complex product of two complex 16-bit vectors.
 * 
 * Multiply the vector @vector{b} element-wise with the vector @vector{c} and divide the product by 
 * @math{2^{-sat}}, placing the result in the complex vector @vector{a}.
 * 
 * The 16-bit elements of @vector{b} and @vector{c} are multiplied to produce 32-bit values for the real and imaginary
 * part of the result. Those 32-bit values are divided by @math{2^{sat}} and finally saturated to the symmetric 16-bit 
 * range (see TODO) for the final result. The division rounds away from zero.
 * 
 * The parameters `a_real[]` and `a_imag[]` together represent the complex output vector @vector{a}, with the former 
 * representing the real part of its elements and the latter representing the imaginary parts.
 * 
 * Similarly, `b_real[]` and `b_imag[]` together represent the complex input vector @vector{b}, and the parameters
 * `c_real[]` and `c_imag[]` together represent the complex input vector @vector{c}.
 * 
 * `sat` is the right-shift applied to the products of @vector{b} and @vector{c}, as @math{2^{-sat}}. `sat` can only
 * be used to apply a right shift of the products, so `sat` must be non-negative.
 * 
 * `length` is the number of elements in vectors @vector{a}, @vector{b} and @vector{c}.
 * 
 * This function returns the headroom of the output vector @vector{a}. The headroom of a complex 16-bit vector is the 
 * minimum of the headroom between its real and imaginary parts (i.e. `a_real[]` and `a_imag[]`).
 * 
 * @low_op{16, @f$ Re\\{a_k\\} \leftarrow \left(Re\\{b_k\\}\cdot Re\\{c_k\\}-Im\\{b_k\\}\cdot Im\\{c_k\\}\right) 
 *          \cdot 2^{-sat} \\
 *          Im\\{a_k\\} \leftarrow \left(Re\\{b_k\\}\cdot Im\\{c_k\\}+Im\\{b_k\\}\cdot Re\\{c_k\\}\right)
 *          \cdot 2^{-sat} \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) @f$}
 * 
 * @par Maximizing Precision:
 *      TODO - Guidelines for keeping maximal precision with this function.
 * 
 * @par Safe In-Place Computation:
 *      The output of this function can be safely computed in-place if the output pointers `a_real[]` and `a_imag[]` are
 *      equal to the input pointers `b_real[]`, `b_imag[]`, `c_real[]` or `c_imag`. Using in-place operations may 
 *      reduce peak memory requirements.
 * 
 * @par Word Alignment Required:
 *      Parameters `a_real[]`, `a_imag[]`, `b_real[]`, `b_imag[]`, `c_real[]` and `c_imag[]` must begin at a 
 *      word-aligned (4-byte) address. For more about alignment requirements, see \ref vector_alignment.
 * 
 * @param[out] a_real   Real part of output vector @vector{a}.
 * @param[out] a_imag   Imaginary part of output vector @vector{a}.
 * @param[in]  b_real   Real part of input vector @vector{b}.
 * @param[in]  b_imag   Imaginary part of input vector @vector{b}.
 * @param[in]  c_real   Real part of input vector @vector{c}.
 * @param[in]  c_real   Imaginary part of input vector @vector{c}.
 * @param[in]  length   Number of elements in vectors @vector{a}, @vector{b}, and @vector{c}.
 * @param[in]  sat      Right-shift applied to products.
 * 
 * @return The headroom of the output vector @vector{a} is returned.
 */
headroom_t xs3_vect_complex_s16_mul(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const int16_t c_real[],
    const int16_t c_imag[],
    const unsigned length,
    const right_shift_t sat);


/**
 * @brief Compute the complex product of a complex 16-bit vector and the complex conjugate of another.
 * 
 * Multiply the vector @vector{b} element-wise with the complex conjugate of vector @vector{c} and divide the product by 
 * @math{2^{-sat}}, placing the result in the complex vector @vector{a}.
 * 
 * The 16-bit elements of @vector{b} and the complex conjugate of @vector{c} are multiplied to produce 32-bit values for 
 * the real and imaginary part of the result. Those 32-bit values are divided by @math{2^{sat}} and finally saturated to 
 * the symmetric 16-bit range (see TODO) for the final result. The division rounds away from zero.
 * 
 * The parameters `a_real[]` and `a_imag[]` together represent the complex output vector @vector{a}, with the former 
 * representing the real part of its elements and the latter representing the imaginary parts.
 * 
 * Similarly, `b_real[]` and `b_imag[]` together represent the complex input vector @vector{b}, and the parameters
 * `c_real[]` and `c_imag[]` together represent the complex input vector @vector{c}.
 * 
 * `sat` is the right-shift applied to the products of @vector{b} and @vector{c}, as @math{2^{-sat}}. `sat` can only
 * be used to apply a right shift of the products, so `sat` must be non-negative.
 * 
 * `length` is the number of elements in vectors @vector{a}, @vector{b} and @vector{c}.
 * 
 * This function returns the headroom of the output vector @vector{a}. The headroom of a complex 16-bit vector is the 
 * minimum of the headroom between its real and imaginary parts (i.e. `a_real[]` and `a_imag[]`).
 * 
 * @low_op{16, @f$ Re\\{a_k\\} \leftarrow \left(Re\\{b_k\\}\cdot Re\\{c_k\\} + Im\\{b_k\\}\cdot Im\\{c_k\\}\right) 
 *          \cdot 2^{-sat} \\
 *          Im\\{a_k\\} \leftarrow \left(Im\\{b_k\\}\cdot Re\\{c_k\\} - Re\\{b_k\\}\cdot Im\\{c_k\\}\right)
 *          \cdot 2^{-sat} \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) @f$}
 * 
 * @par Maximizing Precision:
 *      TODO - Guidelines for keeping maximal precision with this function.
 * 
 * @par Safe In-Place Computation:
 *      The output of this function can be safely computed in-place if the output pointers `a_real[]` and `a_imag[]` are
 *      equal to the input pointers `b_real[]`, `b_imag[]`, `c_real[]` or `c_imag`. Using in-place operations may 
 *      reduce peak memory requirements.
 * 
 * @par Word Alignment Required:
 *      Parameters `a_real[]`, `a_imag[]`, `b_real[]`, `b_imag[]`, `c_real[]` and `c_imag[]` must begin at a 
 *      word-aligned (4-byte) address. For more about alignment requirements, see \ref vector_alignment.
 * 
 * @param[out] a_real   Real part of output vector @vector{a}.
 * @param[out] a_imag   Imaginary part of output vector @vector{a}.
 * @param[in]  b_real   Real part of input vector @vector{b}.
 * @param[in]  b_imag   Imaginary part of input vector @vector{b}.
 * @param[in]  c_real   Real part of input vector @vector{c}.
 * @param[in]  c_real   Imaginary part of input vector @vector{c}.
 * @param[in]  length   Number of elements in vectors @vector{a}, @vector{b}, and @vector{c}.
 * @param[in]  sat      Right-shift applied to products.
 * 
 * @return The headroom of the output vector @vector{a} is returned.
 */
headroom_t xs3_vect_complex_s16_conj_mul(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const int16_t c_real[],
    const int16_t c_imag[],
    const unsigned length,
    const right_shift_t sat);

/**
 * @brief Get headroom of complex 16-bit vector.
 * 
 */
headroom_t xs3_vect_complex_s16_headroom(
    const int16_t a_real[],
    const int16_t a_imag[],
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
headroom_t xs3_vect_complex_s16_mag(
    int16_t a[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const unsigned length,
    const right_shift_t b_shr,
    const int16_t* rot_table,
    const unsigned table_rows);




/**
 * 
 * TODO: useful documentation info in the function's body. Copy some of that.
 * 
 */
void xs3_vect_complex_s16_real_mul_prepare(
    exponent_t* a_exp,
    right_shift_t* sat,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr,
    const unsigned allow_saturation);

/**
 * @brief Multiply a complex 16-bit vector by a real 16-bit vector.
 * 
 * Multiply the complex vector @vector{b} element-wise with the real vector @vector{c} and divide the product by 
 * @math{2^{-sat}}, placing the result in the complex vector @vector{a}. 
 * 
 * The 16-bit elements of @vector{b} and @vector{c} are multiplied for a 32-bit product. The 32-bit product is divided
 * by @math{2^{sat}} and finally saturated to the symmetric 16-bit range (see TODO) for the final result. The division
 * rounds away from zero.
 * 
 * The parameters `a_real[]` and `a_imag[]` together represent the complex output vector @vector{a}, with the former 
 * representing the real part of its elements and the latter representing the imaginary parts.
 * 
 * Similarly, `b_real[]` and `b_imag[]` together represent the complex input vector @vector{b}.
 * 
 * The parameter `c_real[]` represents the real input vector @vector{c}.
 * 
 * `sat` is the right-shift applied to the products of @vector{b} and @vector{c}, as @math{2^{-sat}}. `sat` can only
 * be used to apply a right shift of the products, so `sat` must be non-negative.
 * 
 * `length` is the number of elements in vectors @vector{a}, @vector{b} and @vector{c}.
 * 
 * This function returns the headroom of the output vector @vector{a}. The headroom of a complex 16-bit vector is the 
 * minimum of the headroom between its real and imaginary parts (i.e. `a_real[]` and `a_imag[]`).
 * 
 * @low_op{16, @f$ Re\\{a_k\\} \leftarrow Re\\{b_k\\} \cdot c_k \cdot 2^{-sat} \\
 *                 Im\\{a_k\\} \leftarrow Im\\{b_k\\} \cdot c_k \cdot 2^{-sat} \\
 *                      \qquad\text{ for }k\in 0\ ...\ (length-1)@f$ }
 * 
 * @par Maximizing Precision:
 *      TODO - Guidelines for keeping maximal precision with this function.
 * 
 * @par Safe In-Place Computation:
 *      The output of this function can be safely computed in-place if the output pointers `a_real[]` and `a_imag[]` are
 *      equal to the input pointers `b_real[]`, `b_imag[]` or `c_real[]`. Using in-place operations may reduce peak 
 *      memory requirements.
 * 
 * @par Word Alignment Required:
 *      Parameters `a_real[]`, `a_imag[]`, `b_real[]`, `b_imag[]`, and `c_real[]` must begin at a word-aligned (4-byte)
 *      address. For more about alignment requirements, see \ref vector_alignment.
 * 
 * @param[out] a_real   Real part of output vector @vector{a}.
 * @param[out] a_imag   Imaginary part of output vector @vector{a}.
 * @param[in]  b_real   Real part of input vector @vector{b}.
 * @param[in]  b_imag   Imaginary part of input vector @vector{b}.
 * @param[in]  c_real   Input vector @vector{c}.
 * @param[in]  length   Number of elements in vectors @vector{a}, @vector{b}, and @vector{c}.
 * @param[in]  sat      Right-shift applied to products.
 * 
 * @return The headroom of the output vector @vector{a} is returned.
 */
headroom_t xs3_vect_complex_s16_real_mul(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const int16_t c_real[],
    const unsigned length,
    const right_shift_t sat);

/**
 * @todo No need for separate low-level 16-bit real scalar multiplication of complex vector. Although, I suppose we 
 *       could speed it up a little if we had a dedicated assembly function for this... currently the inner loop of
 *       xs3_vect_s16_scale() is 4 instructions (with no FNOPs). But we have separate arrays for real and imag in
 *       16-bit vectors, so we go through it twice per complex element. If it was a dedicated function, we could do both
 *       the real and imag parts in a single iteration, saving one branch instruction... but, if it ends up adding an
 *       FNOP, then it hasn't really sped it up. There would only be 7 dual-issue instructions in the loop though... at
 *       least after the first pass it seems like it should be able to hold all of them in the instruction buffer 
 *       (which theoretically fits 8?)
 */
headroom_t xs3_vect_complex_s16_real_scale(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const int16_t c,
    const unsigned length,
    const right_shift_t sat);


/**
 * @brief Compute complex product of a complex 16-bit vector with a complex 16-bit scalar.
 * 
 * Multiply the vector @vector{b} with the scalar @math{c} and divide the product by 
 * @math{2^{-sat}}, placing the result in the complex vector @vector{a}.
 * 
 * Each 16-bit element of @vector{b} is multiplied by @math{c} to produce a 32-bit value for the real and imaginary
 * part of the result. Those 32-bit values are divided by @math{2^{sat}} and finally saturated to the symmetric 16-bit 
 * range (see TODO) for the final result. The division rounds away from zero.
 * 
 * The parameters `a_real[]` and `a_imag[]` together represent the complex output vector @vector{a}, with the former 
 * representing the real part of its elements and the latter representing the imaginary parts.
 * 
 * Similarly, `b_real[]` and `b_imag[]` together represent the complex input vector @vector{b}. 
 * 
 * The parameters `c_real` and `c_imag` together represent the complex input scalar @math{c}.
 * 
 * `sat` is the right-shift applied to the products of @vector{b} and @math{c}, as @math{2^{-sat}}. `sat` can only
 * be used to apply a right shift of the products, so `sat` must be non-negative.
 * 
 * `length` is the number of elements in vectors @vector{a} and @vector{b}.
 * 
 * This function returns the headroom of the output vector @vector{a}. The headroom of a complex 16-bit vector is the 
 * minimum of the headroom between its real and imaginary parts (i.e. `a_real[]` and `a_imag[]`).
 * 
 * @low_op{16, @f$ Re\\{a_k\\} \leftarrow \left(Re\\{b_k\\}\cdot Re\\{c\\}-Im\\{b_k\\}\cdot Im\\{c\\}\right) 
 *          \cdot 2^{-sat} \\
 *          Im\\{a_k\\} \leftarrow \left(Re\\{b_k\\}\cdot Im\\{c\\}+Im\\{b_k\\}\cdot Re\\{c\\}\right)
 *          \cdot 2^{-sat} \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) @f$}
 * 
 * @par Maximizing Precision:
 *      TODO - Guidelines for keeping maximal precision with this function.
 * 
 * @par Safe In-Place Computation:
 *      The output of this function can be safely computed in-place if the output pointers `a_real[]` and `a_imag[]` are
 *      equal to the input pointers `b_real[]` or `b_imag[]`. Using in-place operations may reduce peak memory 
 *      requirements.
 * 
 * @par Word Alignment Required:
 *      Parameters `a_real[]`, `a_imag[]`, `b_real[]`, and `b_imag[]` must begin at a word-aligned (4-byte) address. 
 *      For more about alignment requirements, see \ref vector_alignment.
 * 
 * @param[out] a_real   Real part of output vector @vector{a}.
 * @param[out] a_imag   Imaginary part of output vector @vector{a}.
 * @param[in]  b_real   Real part of input vector @vector{b}.
 * @param[in]  b_imag   Imaginary part of input vector @vector{b}.
 * @param[in]  c_real   Real part of input scalar @math{c}.
 * @param[in]  c_real   Imaginary part of input scalar @math{c}.
 * @param[in]  length   Number of elements in vectors @vector{a} and @vector{b}.
 * @param[in]  sat      Right-shift applied to products.
 * 
 * @return The headroom of the output vector @vector{a} is returned.
 */
headroom_t xs3_vect_complex_s16_scale(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const int16_t c_real,
    const int16_t c_imag,
    const unsigned length,
    const right_shift_t sat);



/** Set all elements of a `complex_s32_t` array to the specified value.
 * 
 * \low_op{Complex&nbsp;32, @f$data_k \leftarrow real + i\cdot imag\qquad\text{ for }k\in 0\ ...\ (length-1)@f$ }
 * 
 * \requires_word_alignment{data}
 * 
 * \param[out] data     Array to set
 * \param[in]  real     Real part of value to set
 * \param[in]  imag     Imaginary part of value to set
 * \param[in]  length   Number of elements in `data`
 */
void xs3_vect_complex_s16_set(
    int16_t real[],
    int16_t imag[],
    const int16_t real_value,
    const int16_t imag_value,
    const unsigned length);


headroom_t xs3_vect_complex_s16_shl(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const unsigned length,
    const left_shift_t shl);

headroom_t xs3_vect_complex_s16_shr(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const unsigned length,
    const right_shift_t shr);


/**
 * @todo
 */
void xs3_vect_complex_s16_squared_mag_prepare(
    exponent_t* a_exp,
    right_shift_t* sat,
    const exponent_t b_exp,
    const headroom_t b_hr,
    const unsigned allow_saturation);


/**
 * @brief Compute the squared magnitudes of elements of a complex 16-bit vector.
 * 
 * The parameter `a[]` represents the vector of squared magnitudes @vector{a}.
 * 
 * The parameters `b_real[]` and `b_imag[]` together represent the complex input vector @vector{b}.
 * 
 * The parameter `length` is the number of elements in each of the vectors.
 * 
 * The parameter `sat` is the right-shift applied to each result to avoid saturation. `sat` can only be used to apply a 
 * right-shift of the products, so `sat` must be non-negative.
 * 
 * @low_op{16, @f$ a_k \leftarrow ((Re\\{b_k\\})^2 + (Im\\{b_k\\})^2)\cdot 2^{-sat} \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)@f$}
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
headroom_t xs3_vect_complex_s16_squared_mag(
    int16_t a[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const unsigned length,
    const right_shift_t sat);


/**
 * @todo
 */
headroom_t xs3_vect_complex_s16_sub(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const int16_t c_real[],
    const int16_t c_imag[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);
    
    
    
/**
 * @brief Compute the sum of elements of a complex 16-bit vector.
 * 
 * This function computes the 32-bit complex sum of the elements of vector @vector{b}. The result is returned 
 * as a `complex_s32_t`.
 * 
 * @low_op{16, @f$ \sum_{k=0}^{length-1} \left( Re\\{b_k\\} + j\cdot Im\\{b_k\\} \right) 
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)   @f$}
 * 
 * @param[in] b_real    The real part of input vector @vector{b}.
 * @param[in] b_imag    The imaginary part of input vector @vector{b}.
 * @param[in] length    The number of elements in vector @vector{b}.
 * 
 * @return The complex 32-bit sum.
 */
complex_s32_t xs3_vect_complex_s16_sum(
    const int16_t b_real[],
    const int16_t b_imag[],
    const unsigned length);

/**
 * @brief Convert a complex 16-bit vector into a complex 32-bit vector.
 * 
 * Each element of the input vector @vector{b} is assigned to the corresponding element of output vector @vector{b}. No
 * scaling is applied by this operation.
 * 
 * The parameter `a[]` represents the 32-bit output vector @vector{a}.
 * 
 * The parameters `b_real[]` and `b_imag[]` together represent the complex input vector @vector{b}.
 * 
 * The parameter `length` is the number of elements in each of the vectors.
 * 
 * @low_op{16, @f$ Re\\{a_k\\} \leftarrow Re\\{b_k\\} \\
 *                 Im\\{a_k\\} \leftarrow Im\\{b_k\\} \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)@f$}
 * 
 * @note The headroom of the output vector @vector{a} is 16 more than that of the input vector @vector{b}.
 * 
 * @param[out] a        Complex output vector @vector{a}.
 * @param[in]  b_real   Real part of complex input vector @vector{b}.
 * @param[in]  b_imag   Imaginary part of complex input vector @vector{b}.
 * @param[in]  length   Number of elements in vectors @vector{a} and @vector{b}
 */
void xs3_vect_complex_s16_to_complex_s32(
    complex_s32_t a[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const unsigned length);


/** Calculate the absolute sum of a 16-bit BFP vector.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
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
int32_t xs3_vect_s16_abs_sum(
    const int16_t b[],
    const unsigned length);


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
 * \see bfp_s16_add()
 */
headroom_t xs3_vect_s16_add(
    int16_t a[],
    const int16_t b[], 
    const int16_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/** Find the index of the maximum element of a 16-bit integer vector.
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
 * \return  Index of the maximum element of `a`.
 */
unsigned xs3_vect_s16_argmax(
    const int16_t a[], 
    const unsigned length);

/** Find the index of the minimum element of a 16-bit integer vector.
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
 * \return  Index of the minimum element of `a`.
 */
unsigned xs3_vect_s16_argmin(
    const int16_t a[], 
    const unsigned length);

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

/** Calculate the energy of a 16-bit BFP vector.
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
int32_t xs3_vect_s16_energy(
    const int16_t b[],
    const unsigned length,
    const right_shift_t b_shr);




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


void xs3_vect_s16_inverse_prepare(
    exponent_t* a_exp,
    unsigned* scale,
    const int16_t b[],
    const exponent_t b_exp,
    const unsigned length);

void xs3_vect_s16_inverse(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const unsigned scale);


/** Find the maximum value in a 16-bit integer vector.
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
int16_t xs3_vect_s16_max(
    const int16_t a[], 
    const unsigned length);


/** Find the minimum value in a 16-bit integer vector.
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
int16_t xs3_vect_s16_min(
    const int16_t a[], 
    const unsigned length);



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
void xs3_vect_s16_mul_prepare(
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



void xs3_vect_s16_scale_prepare(
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
headroom_t xs3_vect_s16_scale(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const int16_t alpha,
    const right_shift_t b_shr);

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


headroom_t xs3_vect_s16_shr(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const right_shift_t shr);



void xs3_vect_s16_sqrt_prepare(
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
 * \see bfp_s16_sub()
 */
headroom_t xs3_vect_s16_sub(
    int16_t a[],
    const int16_t b[],
    const int16_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


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

#endif //XS3_VECT_S16_H_