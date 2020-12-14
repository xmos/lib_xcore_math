#ifndef XS3_VECT_S32_H_
#define XS3_VECT_S32_H_

#include "xs3_math_conf.h"
#include "xs3_math_types.h"
#include "xs3_util.h"

#ifdef __XC__
extern "C" {
#endif

/**
 * Maximum bit-depth that can be calculated by xs3_vect_s32_sqrt().
 * 
 * @see xs3_vect_s32_sqrt
 */
#define XS3_VECT_SQRT_S32_MAX_DEPTH     (31)


/**
 * @brief Calculate the output exponent and input shifts to add or subtract two 16- or 32-bit BFP vectors.
 * 
 * When adding or subtracting two BFP vectors (which may have different exponents), the operand elements may 
 * need to be scaled by a power of 2 (i.e. bit-shifted) prior to the operation. This process can also be used
 * to avoid any overflow or saturation.
 * 
 * This function is used to calculate the output exponent and operand shift parameter values required to 
 * compute the element-wise sum @math{ \bar A = \bar B + \bar C } or difference @math{ \bar A = \bar B - \bar C } 
 * of two BFP vectors @math{\bar B} and @math{\bar C}.
 * 
 * 
 * Call this to compute the requierd parameters for:
 * The outputs of this function `b_shr` and `c_shr` can be used as the shift values for:
 * * xs3_vect_s16_add()
 * * xs3_vect_s32_add()
 * * xs3_vect_s16_sub()
 * * xs3_vect_s32_sub()
 * * xs3_vect_complex_s16_add()
 * * xs3_vect_complex_s32_add()
 * * xs3_vect_complex_s16_sub()
 * * xs3_vect_complex_s32_sub()
 * 
 * The output `a_exp` is the exponent associated with the result computed by those functions.
 * 
 * If a specific output exponent `desired_exp` is needed for the result, the `b_shr` and `c_shr` 
 * produced by this function can be adjusted according to the following:
 * \code{.c}
 *      exponent_t desired_exp = ...; // Value known a priori
 *      right_shift_t new_b_shr = b_shr + (desired_exp - a_exp);
 *      right_shift_t new_c_shr = c_shr + (desired_exp - a_exp);
 * \endcode
 * Note that using smaller values than strictly necessary for `b_shr` and `c_shr` can result in saturation, 
 * and using larger values may result in unnecessary underflows or loss of precision.
 * 
 * If @math{B_{hr}} or @math{C_{hr}} are unknown, they can be calculated using xs3_vect_s16_headroom() 
 * or xs3_vect_s32_headroom(), or the value `0` can be supplied, which will account for the worst case.
 * 
 * The `allow_saturation` parameter, if `0`, specifies that this function should detect and avoid the possibility
 * that any element of the output vector @math{\bar A} may saturate. This is usually unnecessary. In the worst
 * case, saturation will cause 1 LSb of error.
 * 
 * @param[out] a_exp    Exponent @math{A_{exp}} of the result vector @math{\bar A}
 * @param[out] b_shr    Right-shift to be applied to vector @math{\bar B}
 * @param[out] c_shr    Right-shift to be applied to vector @math{\bar C}
 * @param[in]  b_exp    Exponent @math{B_{exp}} associated with @math{\bar B}
 * @param[in]  c_exp    Exponent @math{C_{exp}} associated with @math{\bar C}
 * @param[in]  b_hr     Headroom @math{B_{hr}} associated with @math{\bar B}
 * @param[in]  c_hr     Headroom @math{B_{hr}} associated with @math{\bar C}
 * @param[in]  allow_saturation  Whether to avoid corner-case saturation.
 * 
 * @see xs3_vect_s16_add
 * @see xs3_vect_s32_add
 * @see xs3_vect_s16_sub
 * @see xs3_vect_s32_sub
 * @see xs3_vect_complex_s16_add
 * @see xs3_vect_complex_s32_add
 * @see xs3_vect_complex_s16_sub
 * @see xs3_vect_complex_s32_sub
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


/**
 * @brief Calculate the headroom of a 32-bit channel-pair vector.
 * 
 * The headroom of an N-bit integer is the number of bits that the integer's value may be left-shifted
 * without any information being lost. Equivalently, it is one less than the number of leading sign bits.
 * 
 * The headroom of a `ch_pair_s32_t` struct is the minimum of the headroom of each of its 32-bit fields,
 * `ch_a` and `ch_b`.
 * 
 * The headroom of a 32-bit channel-pair vector is the minimum of the headroom of each of its `ch_pair_s32_t`
 * elements.
 * 
 * This function efficiently traverses the elements of @vector{x} to determine its headroom.
 * 
 * @par Parameter Details
 * 
 * `x[]` represents a 32-bit channel-pair vector @vector{x}. `x[]` must begin at a word-aligned address.
 * 
 * `length` is the number of elements in `x[]`.
 * 
 * @low_op{32, @f$
 *      min\!\\{ HR_{32}\left(x_0\right), HR_{32}\left(x_1\right), ..., HR_{32}\left(x_{length-1}\right) \\}
 * @f$ }
 * 
 * @param[in]   x       Input channel-pair vector @vector{x}
 * @param[in]   length  Number of elements in @vector{x}
 * 
 * @returns     Headroom of vector @vector{x}
 * 
 * @see xs3_vect_ch_pair_s16_headroom
 * @see xs3_vect_s16_headroom
 * @see xs3_vect_s32_headroom
 * @see xs3_vect_complex_s16_headroom
 * @see xs3_vect_complex_s32_headroom
 */
headroom_t xs3_vect_ch_pair_s32_headroom(
    const ch_pair_s32_t x[],
    const unsigned length);


/**
 * @brief Set each element of a 32-bit channel-pair vector to a specified value.
 * 
 * Each `x[k].ch_a` is set to `ch_a`, and each `x[k].ch_b` is set to `ch_b` (for `0 < k <= length`).
 * 
 * @par Parameter Details
 * 
 * `x[]` represents a 32-bit vector @vector{x}. `x[]` must begin at a word-aligned address.
 * 
 * `ch_a` and `ch_b` are respectively the channel A and channel B values to which each element will be set.
 * 
 * `length` is the number of elements in `x[]`.
 * 
 * @low_op{32, @f$ 
 *      ChA\\{x_k\\} \leftarrow ch\_a \\
 *      ChB\\{x_k\\} \leftarrow ch\_b \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
 * 
 * @par Word-alignment Required
 *      `x[]` must begin at a word-aligned (4 byte) address.
 * 
 * @param[out]  x           Output channel-pair vector @vector{x}
 * @param[in]   ch_b        Value to set channel A of elements of @vector{x} to
 * @param[in]   ch_a        Value to set channel B of elements of @vector{x} to
 * @param[in]   length      Number of elements in vector{x}
 * 
 * @see xs3_vect_ch_pair_s16_set
 * @see xs3_vect_s16_set
 * @see xs3_vect_s32_set
 * @see xs3_vect_complex_s16_set
 * @see xs3_vect_complex_s32_set
 */
void xs3_vect_ch_pair_s32_set(
    ch_pair_s32_t x[],
    const int32_t ch_a,
    const int32_t ch_b,
    const unsigned length);


/**
 * @brief Left-shift each element of a 32-bit channel-pair vector by a specified number of bits.
 * 
 * Left-shift each element of @vector{b} by @math{b\_shl} bits, placing the result in the corresponding
 * element of @vector{a}. This is equivalent to multiplying each element of @vector{b} by @math{2^{b\_shl}}.
 * 
 * @par Parameter Details
 * 
 * `a[]` and `b[]` represent the 32-bit mantissa vectors @vector{a} and @vector{b} respectively. Each must begin at a
 * word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in @vector{a} and @vector{b}.
 * 
 * `b_shl` is the signed arithmetic left-shift applied to each element of @vector{b}.
 * 
 * @low_op{32, @f$ 
 *      ChA\\{a_k\\} \leftarrow sat_{32}(\lfloor ChA\\{b_k\\} \cdot {2^{b\_shl}} \rfloor) \\
 *      ChB\\{a_k\\} \leftarrow sat_{32}(\lfloor ChB\\{b_k\\} \cdot {2^{b\_shl}} \rfloor) \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)
 * @f$ }
 * 
 * @param[out]  a           Output channel-pair vector @vector{a}
 * @param[in]   b           Input channel-pair vector @vector{b}
 * @param[in]   length      Number of elements in @vector{a} and @vector{b}
 * @param[in]   b_shl       Number of bits @math{b\_shl} to left-shift the elements of @vector{b}
 * 
 * @returns     Headroom of output vector @vector{a}
 * 
 * @see xs3_vect_ch_pair_s32_shr
 */
headroom_t xs3_vect_ch_pair_s32_shl(
    ch_pair_s32_t a[],
    const ch_pair_s32_t b[],
    const unsigned length,
    const left_shift_t b_shl);


/**
 * @brief Right-shift each element of a 32-bit channel-pair vector by a specified number of bits.
 * 
 * Right-shift each element of @vector{b} by @math{b\_shr} bits, placing the result in the corresponding
 * element of @vector{a}. This is equivalent to dividing each element of @vector{b} by @math{2^{b\_shr}}.
 * 
 * @par Parameter Details
 * 
 * `a[]` and `b[]` represent the 32-bit mantissa vectors @vector{a} and @vector{b} respectively. Each must begin at a
 * word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in @vector{a} and @vector{b}.
 * 
 * `b_shr` is the signed arithmetic right-shift applied to each element of @vector{b}.
 * 
 * @low_op{32, @f$ 
 *      ChA\\{a_k\\} \leftarrow sat_{32}(\lfloor ChA\\{b_k\\} \cdot {2^{-b\_shr}} \rfloor) \\
 *      ChB\\{a_k\\} \leftarrow sat_{32}(\lfloor ChB\\{b_k\\} \cdot {2^{-b\_shr}} \rfloor) \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)
 * @f$ }
 * 
 * @note This function saturates the output elements to the symmetric 32-bit range.
 * 
 * @par Safe In-place Computation
 *      This function may safely operate in-place on `b[]`.
 * 
 * @par Word-alignment Required
 *      `a[]` and `b[]` must each begin at a word-aligned (4 byte) address.
 * 
 * @param[out]  a           Complex output vector @vector{a}
 * @param[in]   b           Complex input vector @vector{b}
 * @param[in]   length      Number of elements in @vector{a} and @vector{b}
 * @param[in]   b_shr       Number of bits @math{b\_shr} to right-shift the elements of @vector{b}
 * 
 * @returns     Headroom of output vector @vector{a}
 * 
 * @see xs3_vect_ch_pair_s32_shl
 */
headroom_t xs3_vect_ch_pair_s32_shr(
    ch_pair_s32_t a[],
    const ch_pair_s32_t b[],
    const unsigned length,
    const right_shift_t b_shr);


/**
 * @brief Obtain the shift params used by xs3_vect_complex_s32_mag() and xs3_vect_complex_s16_mag().
 * 
 * @param[out]  a_exp               Exponent of output of xs3_vect_complex_s32_mag()
 * @param[out]  b_shr               Right-shift to be applied to elements of @vector{b}
 * @param[in]   b_exp               Exponent of @vector{b}
 * @param[in]   b_hr                Headroom of @vector{b}
 * @param[in]   allow_saturation    Whether xs3_vect_complex_s32_mag() is allowed to saturate
 * 
 * @see xs3_vect_complex_s16_mag()
 * @see xs3_vect_complex_s32_mag()
 */
void xs3_vect_complex_mag_calc_params(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    const exponent_t b_exp,
    const headroom_t b_hr,
    const unsigned allow_saturation);


/**
 * @brief Add together two complex 32-bit vectors element-wise.
 * 
 * xs3_vect_complex_s32_add() and xs3_vect_add_sub_calc_params() together represent the following BFP operation: 
 * 
 * @math{ \bar{a} \cdot 2^{a\_exp} \leftarrow \left( \bar{b} \cdot 2^{b\_exp} \right) 
 *                                          + \left( \bar{c} \cdot 2^{c\_exp} \right) }
 * 
 * xs3_vect_add_sub_calc_params() should be called first to compute the shift parameters @math{b\_shr} and @math{c\_shr} 
 * used by this function, as well as output exponent @math{a\_exp}. This function is then used to compute the resulting 
 * mantissa vector @vector{a}.
 * 
 * @par Parameter Details
 * 
 * `a[]`, `b[]` and `c[]` represent the complex 32-bit mantissa vectors @vector{a}, @vector{b} and @vector{c}  
 * respectively. Each must begin at a word-aligned address. This operation can be performed safely in-place on `b[]` or
 * `c[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b} and @vector{c} 
 * respectively.
 * 
 * @low_op{32, @f$ 
 *      b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
 *      c_k' \leftarrow sat_{32}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)     \\
 *      Re\\{a_k\\} \leftarrow Re\\{b_k'\\} + Re\\{c_k'\\}                  \\
*       Im\\{a_k\\} \leftarrow Im\\{b_k'\\} + Im\\{c_k'\\}                  \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
 * 
 * @param[out]      a           Complex output vector @vector{a}
 * @param[in]       b           Complex input vector @vector{b}
 * @param[in]       c           Complex input vector @vector{c}
 * @param[in]       length      Number of elements in vectors @vector{a}, @vector{b} and @vector{c}
 * @param[in]       b_shr       Right-shift applied to @vector{b}
 * @param[in]       c_shr       Right-shift applied to @vector{c}
 * 
 * @returns     Headroom of output vector @vector{a}.
 * 
 * @see xs3_vect_add_sub_calc_params
 */
headroom_t xs3_vect_complex_s32_add(
    complex_s32_t a[],
    const complex_s32_t b[],
    const complex_s32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Multiply one complex 32-bit vector element-wise by the complex conjugate of another.
 * 
 * xs3_vect_complex_s32_conj_mul() and xs3_vect_complex_s32_mul_calc_params() together represent the following BFP
 * operation:
 * 
 * @math{ a_k \cdot 2^{a\_exp} \leftarrow \left( b_k \cdot 2^{b\_exp} \right) 
 *                                  \cdot \left( c_k \cdot 2^{c\_exp} \right)^*  \\
 *          \qquad\text{where } (\cdot)^* \text{ is the complex conjugate}
 * }
 * 
 * xs3_vect_complex_s32_mul_calc_params() should be called first to compute the shift parameters @math{b\_shr} and
 * @math{c\_shr} used by this function, as well as output exponent @math{a\_exp}. This function is then used to compute
 * the resulting mantissa vector @vector{a}.
 * 
 * @par Parameter Details
 * 
 * `a[]`, `b[]` and `c[]` represent the 32-bit mantissa vectors @vector{a}, @vector{b} and @vector{c} respectively. 
 * Each must begin at a word-aligned address. This operation can be performed safely in-place on `b[]` or `c[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b} and @vector{c} 
 * respectively.
 * 
 * @low_op{32, @f$ 
 *      b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)                     \\
 *      c_k' \leftarrow sat_{32}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)                     \\
 *      Re\\{a_k\\} \leftarrow \left( Re\\{b_k'\\} \cdot Re\\{c_k'\\} 
 *                                  + Im\\{b_k'\\} \cdot Im\\{c_k'\\} \right) \cdot 2^{-30} \\
 *      Im\\{a_k\\} \leftarrow \left( Im\\{b_k'\\} \cdot Re\\{c_k'\\} 
 *                                  - Re\\{b_k'\\} \cdot Im\\{c_k'\\} \right) \cdot 2^{-30} \\
 *      \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
 * 
 * @param[out] a        Complex output vector @vector{a}
 * @param[in]  b        Complex input vector @vector{b}
 * @param[in]  c        Complex input vector @vector{c}
 * @param[in]  length   Number of elements in vectors @vector{a}, @vector{b}, and @vector{c}
 * @param[in]  b_shr    Right-shift appled to @vector{b}
 * @param[in]  c_shr    Right-shift appled to @vector{c}
 * 
 * @return      Headroom of the output vector @vector{a}
 * 
 * @see xs3_vect_complex_s32_mul_calc_params
 */
headroom_t xs3_vect_complex_s32_conj_mul(
    complex_s32_t a[],
    const complex_s32_t b[],
    const complex_s32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Calculate the headroom of a complex 32-bit array.
 * 
 * The headroom of an N-bit integer is the number of bits that the integer's value may be left-shifted
 * without any information being lost. Equivalently, it is one less than the number of leading sign bits.
 * 
 * The headroom of a `complex_s32_t` struct is the minimum of the headroom of each of its 32-bit fields,
 * `re` and `im`.
 * 
 * The headroom of a `complex_s32_t` array is the minimum of the headroom of each of its `complex_s32_t`
 * elements.
 * 
 * This function efficiently traverses the elements of @vector{x} to determine its headroom.
 * 
 * @par Parameter Details
 * 
 * `x[]` represents the complex 32-bit vector @vector{x}. `x[]` must begin at a word-aligned address.
 * 
 * `length` is the number of elements in `x[]`.
 * 
 * @low_op{32, @f$
 *      min\!\\{ HR_{32}\left(x_0\right), HR_{32}\left(x_1\right), ..., HR_{32}\left(x_{length-1}\right) \\}
 * @f$ }
 * 
 * @param[in]   x       Complex input vector @vector{x}
 * @param[in]   length  Number of elements in @vector{x}
 * 
 * @returns     Headroom of vector @vector{x} 
 * 
 * @see xs3_vect_ch_pair_s16_headroom
 * @see xs3_vect_ch_pair_s32_headroom
 * @see xs3_vect_s16_headroom
 * @see xs3_vect_s32_headroom
 * @see xs3_vect_complex_s16_headroom
 */
headroom_t xs3_vect_complex_s32_headroom(
    const complex_s32_t x[], 
    const unsigned length);


/**
 * @brief Compute the magnitude of each element of a complex 32-bit vector.
 * 
 * xs3_vect_complex_s32_mag() and xs3_vect_complex_mag_calc_params() together represent the following BFP operation:
 * 
 * @math{ a_k \cdot 2^{a\_exp} \leftarrow \left| b_k \cdot 2^{b\_exp} \right|   \\
 *      \qquad\text{where } \left|\cdot\right| \text{ is the complex magnitude operator}
 * }
 * 
 * @par Parameter Details
 * 
 * `a[]` represents the real 32-bit output mantissa vector @vector{a}.
 * 
 * `b[]` represents the complex 32-bit input mantissa vector @vector{b}.
 * 
 * `a[]` and `b[]` must each begin at a word-aligned address.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` is the signed arithmetic right-shift applied to elements of @vector{b}.
 * 
 * `rot_table` must point to a pre-computed table of complex vectors used in calculating the magnitudes. `table_rows` is
 * the number of rows in the table. This library is distributed with a default version of the required rotation table. 
 * The following symbols can be used to refer to it in user code:
 * 
 * @code
 *     const extern unsigned rot_table32_rows;
 *     const extern complex_s32_t rot_table32[30][4];
 * @endcode
 * 
 * Faster computation (with reduced precision) can be achieved by generating a smaller version of the table. A python
 * script is provided to generate this table.
 * @todo Point to documentation page on generating this table.
 * 
 * @low_op{32, @f$ 
 *      v_k \leftarrow b_k \cdot 2^{-b\_shr}    \\
 *      a_k \leftarrow \sqrt { {\left( Re\\{v_k\\} \right)}^2 + {\left( Im\\{v_k\\} \right)}^2 }
 *        \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
 * 
 * @param[out]  a           Real output vector @vector{a}
 * @param[in]   b           Complex input vector @vector{b}
 * @param[in]   length      Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]   b_shr       Right-shift appled to @vector{b}
 * @param[in]   rot_table   Pre-computed rotation table required for calculating magnitudes
 * @param[in]   table_rows  Number of rows in `rot_table`
 * 
 * @returns     Headroom of the output vector @vector{a}.
 * 
 * @see xs3_vect_complex_mag_calc_params
 */
headroom_t xs3_vect_complex_s32_mag(
    int32_t a[],
    const complex_s32_t b[],
    const unsigned length,
    const right_shift_t b_shr,
    const complex_s32_t* rot_table,
    const unsigned table_rows);


/**
 * @brief Multiply one complex 32-bit vector element-wise by another.
 * 
 * xs3_vect_complex_s32_mul() and xs3_vect_complex_s32_mul_calc_params() together represent the following BFP
 * operation:
 * 
 * @math{ a_k \cdot 2^{a\_exp} \leftarrow \left( b_k \cdot 2^{b\_exp} \right) 
 *                                  \cdot \left( c_k \cdot 2^{c\_exp} \right) }
 * 
 * xs3_vect_complex_s32_mul_calc_params() should be called first to compute the shift parameters @math{b\_shr} and
 * @math{c\_shr} used by this function, as well as output exponent @math{a\_exp}. This function is then used to compute
 * the resulting mantissa vector @vector{a}.
 * 
 * @par Parameter Details
 * 
 * `a[]`, `b[]` and `c[]` represent the 32-bit mantissa vectors @vector{a}, @vector{b} and @vector{c} respectively. 
 * Each must begin at a word-aligned address. This operation can be performed safely in-place on `b[]` or `c[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b} and @vector{c} 
 * respectively.
 * 
 * @low_op{32, @f$ 
 *      b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)                     \\
 *      c_k' \leftarrow sat_{32}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)                     \\
 *      Re\\{a_k\\} \leftarrow \left( Re\\{b_k'\\} \cdot Re\\{c_k'\\} 
 *                                  - Im\\{b_k'\\} \cdot Im\\{c_k'\\} \right) \cdot 2^{-30} \\
 *      Im\\{a_k\\} \leftarrow \left( Im\\{b_k'\\} \cdot Re\\{c_k'\\} 
 *                                  + Re\\{b_k'\\} \cdot Im\\{c_k'\\} \right) \cdot 2^{-30} \\
 *      \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
 * 
 * @param[out] a        Complex output vector @vector{a}
 * @param[in]  b        Complex input vector @vector{b}
 * @param[in]  c        Complex input vector @vector{c}
 * @param[in]  length   Number of elements in vectors @vector{a}, @vector{b}, and @vector{c}
 * @param[in]  b_shr    Right-shift appled to @vector{b}
 * @param[in]  c_shr    Right-shift appled to @vector{c}
 * 
 * @return      Headroom of the output vector @vector{a}
 * 
 * @see xs3_vect_complex_s32_mul_calc_params
 */
headroom_t xs3_vect_complex_s32_mul(
    complex_s32_t a[],
    const complex_s32_t b[],
    const complex_s32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Obtain the shift params used by xs3_vect_complex_s32_mul()
 * 
 * @param[out]  a_exp               Exponent of output of xs3_vect_complex_s32_mul()
 * @param[out]  b_shr               Right-shift to be applied to elements of @vector{b}
 * @param[out]  c_shr               Right-shift to be applied to elements of @vector{c}
 * @param[in]   b_exp               Exponent of @vector{b}
 * @param[in]   c_exp               Exponent of @vector{c}
 * @param[in]   b_hr                Headroom of @vector{b}
 * @param[in]   c_hr                Headroom of @vector{c}
 * @param[in]   allow_saturation    Whether xs3_vect_complex_s32_mul() is allowed to saturate
 * 
 * @see xs3_vect_complex_s32_mul
 */
void xs3_vect_complex_s32_mul_calc_params(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    right_shift_t* c_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr,
    const unsigned allow_saturation);


/**
 * @brief Multiply a complex 32-bit vector element-wise by a real 32-bit vector.
 * 
 * xs3_vect_complex_s32_real_mul() and xs3_vect_complex_s32_real_mul_calc_params() together represent the following BFP
 * operation:
 * 
 * @math{ a_k \cdot 2^{a\_exp} \leftarrow \left( b_k \cdot 2^{b\_exp} \right) 
 *                                  \cdot \left( c_k \cdot 2^{c\_exp} \right) }
 * 
 * xs3_vect_complex_s32_real_mul_calc_params() should be called first to compute the shift parameters @math{b\_shr} and
 * @math{c\_shr} used by this function, as well as output exponent @math{a\_exp}. This function is then used to compute
 * the resulting mantissa vector @vector{a}.
 * 
 * @par Parameter Details
 * 
 * `a[]` and `b[]` represent the complex 32-bit mantissa vectors @vector{a} and @vector{b} respectively. 
 * 
 * `c[]` represents the real 32-bit mantissa vector @vector{c}.
 * 
 * `a[]`, `b[]`, and `c[]` each must begin at a word-aligned address. This operation can be performed safely in-place on 
 * `b[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b} and @vector{c} 
 * respectively.
 * 
 * @low_op{32, @f$ 
 *      b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)      \\
 *      c_k' \leftarrow sat_{32}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)      \\
 *      Re\\{a_k\\} \leftarrow \left( Re\\{b_k'\\} \cdot c_k' \right) \cdot 2^{-30} \\
 *      Im\\{a_k\\} \leftarrow \left( Im\\{b_k'\\} \cdot c_k' \right) \cdot 2^{-30} \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)
 * @f$ }
 * 
 * @param[out] a        Complex output vector @vector{a}.
 * @param[in]  b        Complex input vector @vector{b}.
 * @param[in]  c        Real input vector @vector{c}.
 * @param[in]  length   Number of elements in vectors @vector{a}, @vector{b}, and @vector{c}.
 * @param[in]  b_shr    Right-shift appled to @vector{b}.
 * @param[in]  c_shr    Right-shift appled to @vector{c}.
 * 
 * @returns     Headroom of the output vector @vector{a}.
 * 
 * @see xs3_vect_complex_s32_real_mul_calc_params
 */
headroom_t xs3_vect_complex_s32_real_mul(
    complex_s32_t a[],
    const complex_s32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Obtain the shift params used by xs3_vect_complex_s32_real_mul().
 * 
 * @param[out]  a_exp               Exponent of output from xs3_vect_complex_s32_real_mul()
 * @param[out]  b_shr               Arithmetic right-shift to be applied to elements of vector @vector{b}
 * @param[out]  c_shr               Arithmetic right-shift to be applied to elements of vector @vector{c}
 * @param[in]   b_exp               Exponent of @vector{b}
 * @param[in]   c_exp               Exponent of @vector{c}
 * @param[in]   b_hr                Headroom of @vector{b}
 * @param[in]   c_hr                Headroom of @vector{c}
 * @param[in]   allow_saturation    Whether xs3_vect_complex_s32_real_mul() is allowed to saturate
 * 
 * @see xs3_vect_complex_s32_real_mul
 */
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
 * @brief Multiply a complex 32-bit vector by a real scalar.
 * 
 * xs3_vect_complex_s32_real_scale() and xs3_vect_complex_s32_real_mul_calc_params() together represent the following 
 * BFP operation:
 * 
 * @math{ a_k \cdot 2^{a\_exp} \leftarrow \left( b_k \cdot 2^{b\_exp} \right) \cdot \left( c \cdot 2^{c\_exp} \right) }
 * 
 * xs3_vect_complex_s32_real_mul_calc_params() should be called first to compute the shift parameters @math{b\_shr} and
 * @math{c\_shr}, as well as the output exponent @math{a\_exp}. @math{b\_shr} is used by this function, and 
 * @math{c\_shr} is applied in user code prior to calling this function. This function is then used to compute the 
 * resulting mantissa vector @vector{a}.
 * 
 * @par Parameter Details
 * 
 * `a[]` and `b[]` represent the complex 32-bit mantissa vectors @vector{a} and @vector{b} respectively. 
 * 
 * `c` represents the real 32-bit scale factor @math{c}.
 * 
 * `a[]` and `b[]` each must begin at a word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` is the signed arithmetic right-shift applied to each element of @vector{b}.
 * 
 * @low_op{32, @f$
 *      b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
 *      Re\\{a_k\\} \leftarrow Re\\{b_k'\\} \cdot c                         \\
 *      Im\\{a_k\\} \leftarrow Im\\{b_k'\\} \cdot c                         \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)
 * @f$ }
 * 
 * @todo To make things simpler I should just have this function take `c_shr` as a parameter so that the user can't 
 *       accidentally apply it incorrectly.
 * 
 * @param[out]  a           Complex output vector @vector{a}
 * @param[in]   b           Complex input vector @vector{b}
 * @param[in]   c           Complex input vector @vector{c}
 * @param[in]   length      Number of elements in vectors @vector{a}, @vector{b}, and @vector{c}
 * @param[in]   b_shr       Right-shift applied to @vector{b}
 * 
 * @returns     Headroom of the output vector @vector{a}.
 */
headroom_t xs3_vect_complex_s32_real_scale(
    complex_s32_t a[],
    const complex_s32_t b[],
    const int32_t c,
    const unsigned length,
    const right_shift_t b_shr);


/**
 * @brief Multiply a complex 32-bit vector by a complex 32-bit scalar.
 * 
 * xs3_vect_complex_s32_scale() and xs3_vect_complex_s32_scale_calc_params() together represent the following 
 * BFP operation:
 * 
 * @math{ a_k \cdot 2^{a\_exp} \leftarrow \left( b_k \cdot 2^{b\_exp} \right) \cdot \left( c \cdot 2^{c\_exp} \right) }
 * 
 * xs3_vect_complex_s32_scale_calc_params() should be called first to compute the shift parameters @math{b\_shr} and
 * @math{c\_shr}, as well as the output exponent @math{a\_exp}. @math{b\_shr} is used by this function, and 
 * @math{c\_shr} is applied in user code prior to calling this function. This function is then used to compute the 
 * resulting mantissa vector @vector{a}.
 * 
 * @par Parameter Details
 * 
 * `a[]` and `b[]` represent the complex 32-bit mantissa vectors @vector{a} and @vector{b} respectively. 
 * 
 * `c` represents the complex 32-bit scale factor @math{c}.
 * 
 * `a[]` and `b[]` each must begin at a word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` is the signed arithmetic right-shift applied to each element of @vector{b}.
 * 
 * @low_op{32, @f$ 
 *      b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)                     \\
 *      Re\\{a_k\\} \leftarrow \left( Re\\{v_k\\} \cdot Re\\{c\\} 
 *                                  - Im\\{v_k\\} \cdot Im\\{c\\} \right) \cdot 2^{-30}     \\
 *      Im\\{a_k\\} \leftarrow \left( Re\\{v_k\\} \cdot Im\\{c\\} 
 *                                  + Im\\{v_k\\} \cdot Re\\{c\\} \right) \cdot 2^{-30}     \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)
 * @f$ }
 * 
 * @todo To make things simpler I should just have this function take `c_shr` as a parameter so that the user can't 
 *       accidentally apply it incorrectly.
 * 
 * @param[out] a        Complex output vector @vector{a}.
 * @param[in]  b        Complex input vector @vector{b}.
 * @param[in]  c_real   Real part of @math{c}
 * @param[in]  c_imag   Imaginary part of @math{c}
 * @param[in]  length   Number of elements in vectors @vector{a} and @vector{b}.
 * @param[in]  b_shr    Right-shift appled to @vector{b}.
 * 
 * @returns     Headroom of the output vector @vector{a}.
 */
headroom_t xs3_vect_complex_s32_scale(
    complex_s32_t a[],
    const complex_s32_t b[],
    const int32_t c_real,
    const int32_t c_imag,
    const unsigned length,
    const right_shift_t b_shr);


/**
 * @brief Obtain shift params used by xs3_vect_complex_s32_scale()
 * 
 * @param[out]  a_exp               Exponent of output from xs3_vect_complex_s32_scale()
 * @param[out]  b_shr               Arithmetic right-shift to be applied to elements of @vector{b}
 * @param[out]  alpha_shr           Arithmetic right-shift to be applied to @math{\alpha}
 * @param[in]   b_exp               Exponent of @vector{b}
 * @param[in]   alpha_exp           Exponent of @math{\alpha}
 * @param[in]   b_hr                Headroom of @vector{b}
 * @param[in]   alpha_hr            Headroom of @math{\alpha}
 * @param[in]   allow_saturation    Whether xs3_vect_complex_s32_scale() is allowed to saturate
 * 
 * @see xs3_vect_complex_s32_scale
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
 * @brief Set each element of a complex 32-bit vector to a specified value.
 * 
 * @par Parameter Details
 * 
 * `x[]` represents a complex 32-bit vector @vector{x}. `x[]` must begin at a word-aligned address.
 * 
 * `real` and `imag` are the real and imaginary parts to which each element will be set.
 * 
 * `length` is the number of elements in `x[]`.
 * 
 * @low_op{32, @f$ 
 *      x_k \leftarrow  real + j\cdot imag              \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)   \\
 *          \qquad\text{ where } j^2 = -1
 *  @f$ }
 * 
 * @param[out]  x           Complex output vector @vector{x}
 * @param[in]   real        Value to set real part of elements of @vector{x} to
 * @param[in]   imag        Value to set imaginary part of elements of @vector{x} to
 * @param[in]   length      Number of elements in @vector{x}
 */
void xs3_vect_complex_s32_set(
    complex_s32_t x[],
    const int32_t real,
    const int32_t imag,
    const unsigned length);


/**
 * @brief Left-shift each element of a complex 32-bit vector by a specified number of bits.
 * 
 * @par Parameter Details
 * 
 * `a[]` and `b[]` represent the complex 32-bit mantissa vectors @vector{a} and @vector{b} respectively. Each must begin 
 * at a word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in @vector{a} and @vector{b}.
 * 
 * `b_shl` is the signed arithmetic left-shift applied to each element of @vector{b}.
 * 
 * @low_op{32, @f$
 *      Re\\{a_k\\} \leftarrow sat_{32}(\lfloor Re\\{b_k\\} \cdot 2^{b\_shl} \rfloor)     \\
 *      Im\\{a_k\\} \leftarrow sat_{32}(\lfloor Im\\{b_k\\} \cdot 2^{b\_shl} \rfloor)     \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
 * 
 * @param[out]  a           Complex output vector @vector{a}
 * @param[in]   b           Complex input vector @vector{b}
 * @param[in]   length      Number of elements in vector @vector{b}
 * @param[in]   b_shl       Left-shift applied to @vector{b} 
 * 
 * @returns     Headroom of the output vector @vector{a}
 */
headroom_t xs3_vect_complex_s32_shl(
    complex_s32_t a[],
    const complex_s32_t b[],
    const unsigned length,
    const left_shift_t b_shl);


/**
 * @brief Right-shift each element of a complex 32-bit vector by a specified number of bits.
 * 
 * @par Parameter Details
 * 
 * `a[]` and `b[]` represent the complex 32-bit mantissa vectors @vector{a} and @vector{b} respectively. Each must begin 
 * at a word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in @vector{a} and @vector{b}.
 * 
 * `b_shr` is the signed arithmetic right-shift applied to each element of @vector{b}.
 * 
 * @low_op{32, @f$
 *      Re\\{a_k\\} \leftarrow sat_{32}(\lfloor Re\\{b_k\\} \cdot 2^{-b\_shr} \rfloor)     \\
 *      Im\\{a_k\\} \leftarrow sat_{32}(\lfloor Im\\{b_k\\} \cdot 2^{-b\_shr} \rfloor)     \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
 * 
 * @param[out]  a           Complex output vector @vector{a}
 * @param[in]   b           Complex input vector @vector{b}
 * @param[in]   length      Number of elements in vector @vector{b}
 * @param[in]   b_shr       Right-shift applied to @vector{b} 
 * 
 * @returns     Headroom of the output vector @vector{a}
 */
headroom_t xs3_vect_complex_s32_shr(
    complex_s32_t a[],
    const complex_s32_t b[],
    const unsigned length,
    const right_shift_t shr);


/**
 * @brief Computes the squared magnitudes of elements of a complex 32-bit vector.
 * 
 * xs3_vect_complex_s32_squared_mag() and xs3_vect_complex_s32_squared_mag_calc_params() together represent the 
 * following BFP operation:
 * 
 * @math{ a_k \cdot 2^{a\_exp} \leftarrow  \left|b_k \cdot 2^{b\_exp} \right|^2   }
 * 
 * xs3_vect_complex_s32_squared_mag_calc_params() should be called first to compute the shift parameter @math{b\_shr}
 * used by this function, as well as output exponent @math{a\_exp}. This function is then used to compute the resulting
 * mantissa vector @vector{a}.
 * 
 * @par Parameter Details
 * 
 * `a[]` represents the complex 32-bit mantissa vector @vector{a}. `b[]` represents the real 32-bit mantissa vector
 * @vector{b}. Each must begin at a word-aligned address.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` is the signed arithmetic right-shift appled to each element of @vector{b}.
 * 
 * @low_op{32, @f$ 
 *      b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
 *      a_k \leftarrow ((Re\\{b_k'\\})^2 + (Im\\{b_k'\\})^2)\cdot 2^{-30}   \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)
 * @f$ }
 * 
 * @param[out]  a           Complex output vector @vector{a}
 * @param[in]   b           Complex input vector @vector{b}
 * @param[in]   length      Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]   b_shr       Right-shift appled to @vector{b}
 * 
 * @see xs3_vect_complex_s32_squared_mag_calc_params
 */
headroom_t xs3_vect_complex_s32_squared_mag(
    int32_t a[],
    const complex_s32_t b[],
    const unsigned length,
    const right_shift_t b_shr);


/**
 * @brief Obtain the shift parameters used by xs3_vect_complex_s32_squared_mag.
 * 
 * @param[out]  a_exp                   Exponent of output of xs3_vect_complex_s32_squared_mag()
 * @param[out]  b_shr                   Right-shift to be applied to elements of @vector{b}
 * @param[in]   b_exp                   Exponent of @vector{b}
 * @param[in]   b_hr                    Headroom of @vector{b}
 * @param[in]   allow_saturation        Indicates whether xs3_vect_complex_s32_squared_mag() should be allowed to 
 *                                      saturate
 * 
 * @see xs3_vect_complex_s32_squared_mag
 */
void xs3_vect_complex_s32_squared_mag_calc_params(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    const exponent_t b_exp,
    const headroom_t b_hr,
    const unsigned allow_saturation);


/**
 * @brief Subtract one complex 32-bit vector from another.
 * 
 * xs3_vect_complex_s32_sub() and xs3_vect_add_sub_calc_params() together represent the following BFP operation: 
 * 
 * @math{ \bar{a} \cdot 2^{a\_exp} \leftarrow \left( \bar{b} \cdot 2^{b\_exp} \right) 
 *                                          - \left( \bar{c} \cdot 2^{c\_exp} \right) }
 * 
 * xs3_vect_add_sub_calc_params() should be called first to compute the shift parameters @math{b\_shr} and @math{c\_shr} 
 * used by this function, as well as output exponent @math{a\_exp}. This function is then used to compute the resulting 
 * mantissa vector @vector{a}.
 * 
 * @par Parameter Details
 * 
 * `a[]`, `b[]` and `c[]` represent the complex 32-bit mantissa vectors @vector{a}, @vector{b} and @vector{c}  
 * respectively. Each must begin at a word-aligned address. This operation can be performed safely in-place on `b[]` or
 * `c[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b} and @vector{c} 
 * respectively.
 * 
 * @low_op{32, @f$ 
 *      b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
 *      c_k' \leftarrow sat_{32}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)     \\
 *      Re\\{a_k\\} \leftarrow Re\\{b_k'\\} - Re\\{c_k'\\}                  \\
*       Im\\{a_k\\} \leftarrow Im\\{b_k'\\} - Im\\{c_k'\\}                  \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
 * 
 * @param[out]      a           Complex output vector @vector{a}
 * @param[in]       b           Complex input vector @vector{b}
 * @param[in]       c           Complex input vector @vector{c}
 * @param[in]       length      Number of elements in vectors @vector{a}, @vector{b} and @vector{c}
 * @param[in]       b_shr       Right-shift applied to @vector{b}
 * @param[in]       c_shr       Right-shift applied to @vector{c}
 * 
 * @returns     Headroom of output vector @vector{a}.
 * 
 * @see xs3_vect_add_sub_calc_params
 */
headroom_t xs3_vect_complex_s32_sub(
    complex_s32_t a[], 
    const complex_s32_t b[],
    const complex_s32_t c[],
    const unsigned length, 
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Compute the sum of elements of a complex 32-bit vector.
 * 
 * xs3_vect_complex_s32_sum() and xs3_vect_complex_s32_sum_calc_params() together represent the following BFP operation:
 * 
 * @math{ a \cdot 2^{a\_exp} \leftarrow \sum_{k=0}^{length-1} \left( b_k \cdot 2^{b\_exp} \right) }
 * 
 * xs3_vect_complex_s32_sum_calc_params() should be called first to compute the shift parameter @math{b\_shr} which is
 * used by this function, as well as output exponent @math{a\_exp}. This function is then used to compute the resulting
 * mantissa.
 * 
 * @par Parameter Details
 * 
 * `a` is the complex 64-bit mantissa of the resulting sum.
 * 
 * `b[]` represents the complex 32-bit mantissa vector @vector{b}. `b[]` must begin at a word-aligned address.
 * 
 * `length` is the number of elements in @vector{b}.
 * 
 * `b_shr` is the **unsigned** arithmetic right-shift appled to each element of @vector{b}. `b_shr` _cannot_ be 
 * negative.
 * 
 * @low_op{32, @f$ 
 *      b_k' \leftarrow b_k \cdot 2^{-b\_shr}                                       \\
 *      Re\\{a\\} \leftarrow \sum_{k=0}^{length-1} \left( Re\\{b_k'\\} \right)      \\
 *      Im\\{a\\} \leftarrow \sum_{k=0}^{length-1} \left( Im\\{b_k'\\} \right)      
 * @f$ }
 * 
 * @par Additional Details
 * 
 * Internally the sum accumulates into four separate complex 40-bit accumulators. These accumulators apply symmetric 
 * 40-bit saturation logic (with bounds @math{\pm 2^{39}-1}) with each added element. At the end, the 4 accumulators are
 * summed together into the 64-bit fields of `a`. No saturation logic is applied at this final step.
 * 
 * In the most extreme case, each @math{b_k} may be @math{-2^{31}}. @math{256} of these added into the same accumulator 
 * is @math{-2^{39}} which would saturate to @math{-2^{39}+1}, introducing 1 LSb of error (which may or may not be 
 * acceptable given a particular circumstance). The final result for each part then may be as large as 
 * @math{4\cdot(-2^{39}+1) = -2^{41}+4 }, each fitting into a 42-bit signed integer.
 * 
 * @param[out]  a           Complex sum @math{a}
 * @param[in]   b           Complex input vector @vector{b}.
 * @param[in]   length      Number of elements in vector @vector{b}.
 * @param[in]   b_shr       Right-shift appled to @vector{b}.
 * 
 * @see xs3_vect_complex_s32_sum_calc_params
 */
void xs3_vect_complex_s32_sum(
    complex_s64_t* a,
    const complex_s32_t b[],
    const unsigned length,
    const right_shift_t b_shr);


/**
 * @brief Obtain the shift parameters used by xs3_vect_complex_s32_sum().
 * 
 * @param[out]  a_exp                   Exponent of output from xs3_vect_complex_sum()
 * @param[out]  b_shr                   Right-shift to be applied to elements of @vector{b}
 * @param[in]   b_exp                   Exponent of @vector{b}
 * @param[in]   b_hr                    Headroom of @vector{b}
 * @param[in]   length                  Number of elements in @vector{b}
 * @param[in]   allow_saturation        Indicates whether xs3_vect_complex_s32_sum() should be allowed to saturate
 * 
 * @see xs3_vect_complex_s32_sum
 */
void xs3_vect_complex_s32_sum_calc_params(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    const exponent_t b_exp,
    const headroom_t b_hr,
    const unsigned length,
    const unsigned allow_saturation);


/**
 * @brief Reverses the order of the tail of a complex 32-bit vector.
 * 
 * Reverses the order of elements in the tail of the complex 32-bit vector @vector{x}. The tail of @vector{x}, in this 
 * context, is all elements of @vector{x} except for @math{x_0}. In other words, the first element @math{x_0} remains 
 * where it is, and the remaining @math{length-1} elements are rearranged to have their order reversed.
 * 
 * This function is used when performing a forward or inverse FFT on a single sequence of real values (i.e. the mono 
 * FFT), and operates in-place on `x[]`.
 * 
 * @par Parameter Details
 * 
 * `x[]` represents the complex 32-bit vector @vector{x}, which is both an input to and an output of this function. 
 * `x[]` must begin at a word-aligned address.
 * 
 * `length` is the number of elements in @vector{x}.
 * 
 * @low_op{32, @f$
 *      x_0 \leftarrow x_0              \\
 *      x_k \leftarrow x_{length - k}   \\
 *          \qquad\text{ for }k\in 1\ ...\ (length-1) 
 * @f$ }
 * 
 * @param[inout]    x           Complex vector to have its tail reversed.
 * @param[in]       length      Number of elements in @vector{x}
 * 
 * @see bfp_fft_forward_mono
 * @see bfp_fft_inverse_mono
 */
void xs3_vect_complex_s32_tail_reverse(
    complex_s32_t x[],
    const unsigned length);


/** 
 * @brief Convert a complex 32-bit vector into a complex 16-bit vector.
 * 
 * This function converts a complex 32-bit mantissa vector @vector{b} into a complex 16-bit mantissa vector @vector{a}.
 * Conceptually, the output BFP vector @math{\bar{a}\cdot 2^{a\_exp}} represents the same value as the input BFP vector
 * @math{\bar{b}\cdot 2^{b\_exp}}, only with a reduced bit-depth.
 * 
 * In most cases @math{b\_shr} should be @math{16 - b\_hr}, where @math{b\_hr} is the headroom of the 32-bit input
 * mantissa vector @vector{b}. The output exponent @math{a\_exp} will then be given by
 * 
 * @math{ a\_exp = b\_exp + b\_shr }
 * 
 * @par Parameter Details
 * 
 * `a_real[]` and `a_imag[]` together represent the complex 16-bit output mantissa vector @vector{a}, with the real part
 * of each @math{a_k} going in `a_real[]` and the imaginary part going in `a_imag[]`.
 * 
 * `b[]` represents the complex 32-bit mantissa vector @vector{b}.
 * 
 * `a_real[]`, `a_imag[]` and `b[]` must each begin at a word-aligned address.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` is the signed arithmetic right-shift applied to elements of @vector{b}.
 * 
 * @low_op{32, @f$ 
 *      b_k' \leftarrow sat_{16}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
 *      Re\\{a_k\\} \leftarrow Re\\{b_k'\\}                                 \\
 *      Im\\{a_k\\} \leftarrow Im\\{b_k'\\}                                 \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
 * 
 * @param[out]  a_real      Real part of complex output vector @vector{a}.
 * @param[out]  a_imag      Imaginary part of complex output vector @vector{a}.
 * @param[in]   b           Complex input vector @vector{b}.
 * @param[in]   length      Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]   b_shr       Right-shift appled to @vector{b}.
 * 
 * @see xs3_vect_s32_to_s16
 * @see xs3_vect_complex_s16_to_complex_s32
 */
void xs3_vect_complex_s32_to_complex_s16(
    int16_t a_real[],
    int16_t a_imag[],
    const complex_s32_t b[],
    const unsigned length,
    const right_shift_t b_shr);


/** 
 * @brief Compute the element-wise absolute value of a 32-bit vector.
 * 
 * xs3_vect_s32_abs() represents the following BFP operation:
 * 
 * @math{ 
 *      a_k \cdot 2^{a\_exp} \leftarrow \left|b_k \cdot 2^{b\_exp}\right|       \\
 *          \qquad\text{with } a\_exp = b\_exp
 * }
 * 
 * @par Parameter Details
 * 
 * `a[]` and `b[]` represent teh 32-bit mantissa vectors @vector{a} and @vector{b} respectively. Each must begin at a
 * word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * @low_op{32, @f$ 
 *      a_k \leftarrow sat_{32}(\left| b_k \right|)         \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)
 * @f$ }
 * 
 * @param[out] a        Output vector @vector{a}
 * @param[in]  b        Input vector @vector{b}
 * @param[in]  length   Number of elements in vectors @vector{a} and @vector{b}
 * 
 * @returns     Headroom of the output vector @vector{a}.
 */
headroom_t xs3_vect_s32_abs(
    int32_t a[],
    const int32_t b[],
    const unsigned length);


/**
 * @brief Compute the sum of the absolute values of elements of a 32-bit vector.
 * 
 * xs3_vect_s32_abs_sum() represents the following BFP operation:
 * 
 * @math{   
 *      a \cdot 2^{a\_exp} \leftarrow \sum_{k=0}^{length-1} \left| b_k \right| \cdot 2^{b\_exp}     \\
 *          \qquad\text{with } a\_exp = b\_exp
 * }
 * 
 * @par Parameter Details
 * 
 * `b[]` represents the 32-bit mantissa vector @vector{b}. `b[]` must begin at a word-aligned address.
 * 
 * `length` is the number of elements in @vector{b}.
 * 
 * @low_op{32, @f$
 *      \sum_{k=0}^{length-1} sat_{32}(\left| b_k \right|)
 * @f$ }
 * 
 * @par Additional Details
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
 * 
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   length      Number of elements in @vector{b}
 * 
 * @returns The 64-bit sum @math{a}
 */
int64_t xs3_vect_s32_abs_sum(
    const int32_t b[],
    const unsigned length);


/**
 * @brief Add together two 32-bit vectors.
 * 
 * xs3_vect_s32_add() and xs3_vect_add_sub_calc_params() together represent the following BFP operation: 
 * 
 * @math{ \bar{a} \cdot 2^{a\_exp} \leftarrow \left( \bar{b} \cdot 2^{b\_exp} \right) 
 *                                          + \left( \bar{c} \cdot 2^{c\_exp} \right) }
 * 
 * xs3_vect_add_sub_calc_params() should be called first to compute the shift parameters @math{b\_shr} and @math{c\_shr} 
 * used by this function, as well as output exponent @math{a\_exp}. This function is then used to compute the resulting 
 * mantissa vector @vector{a}.
 * 
 * @par Parameter Details
 * 
 * `a[]`, `b[]` and `c[]` represent the 32-bit mantissa vectors @vector{a}, @vector{b} and @vector{c} respectively. 
 * Each must begin at a word-aligned address. This operation can be performed safely in-place on `b[]` or `c[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b} and @vector{c} 
 * respectively.
 * 
 * @low_op{32, @f$ 
 *      b_k' = sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)  \\
 *      c_k' = sat_{32}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)  \\
 *      a_k \leftarrow sat_{32}\!\left( b_k' + c_k' \right)     \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
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
 * @see xs3_vect_add_sub_calc_params
 */
headroom_t xs3_vect_s32_add(
    int32_t a[],
    const int32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Obtain the array index of the maximum element of a 32-bit vector.
 * 
 * @par Parameter Details
 * 
 * `x[]` represents the 32-bit mantissa vector @vector{x}. It must begin at a word-aligned address.
 * 
 * `length` is the number of elements in @vector{x}.
 * 
 * @low_op{32, @f$ 
 *      argmax_k\\{ x_k \\}     \\ 
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
 * 
 * @param[in]   x           Input vector @vector{x}
 * @param[in]   length      Number of elemetns in @vector{x}
 * 
 * @returns Index of the maximum element of mantissa vector @vector{x}. If there is a tie for the maximum value, the 
 * lowest tying index is returned.
 */
unsigned xs3_vect_s32_argmax(
    const int32_t x[],
    const unsigned length);


/**
 * @brief Obtain the array index of the minimum element of a 32-bit vector.
 * 
 * @par Parameter Details
 * 
 * `x[]` represents the 32-bit mantissa vector @vector{x}. It must begin at a word-aligned address.
 * 
 * `length` is the number of elements in @vector{x}.
 * 
 * @low_op{32, @f$ 
 *      argmin_k\\{ x_k \\}     \\ 
 *              \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
 * 
 * @param[in]   x           Input vector @vector{x}
 * @param[in]   length      Number of elements in @vector{x}
 * 
 * @returns Index of the minimum element of mantissa vector @vector{x}. If there is a tie for the minimum value, the 
 * lowest tying index is returned.
 */
unsigned xs3_vect_s32_argmin(
    const int32_t x[],
    const unsigned length);


/**
 * @brief Clamp the elements of a 32-bit vector to a specified range.
 * 
 * xs3_vect_s32_clip() represents the following BFP operation:
 * 
 * @math{   a_k \cdot 2^{a\_exp} \leftarrow min\!\left( U, max\!\left( L, b_k \cdot 2^{b\_exp} \right)\right)   \\
 *              \qquad\text{ for }k\in 0\ ...\ (length-1)                               \\
 *              \qquad\text{ where } L = lower\_bound \cdot 2^{b\_exp+b\_shr},          \\
 *              \qquad\text{       } U = upper\_bound \cdot 2^{b\_exp+b\_shr},          \\
 *              \qquad\text{ and } a\_exp = b\_exp  
 * }
 * 
 * @par Parameter Details
 * 
 * `a[]` and `b[]` represent the 32-bit mantissa vectors @vector{a} and @vector{b} respectively. Each must begin at a 
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
 * @low_op{32, @f$
 *      b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
 *      a_k \leftarrow 
 *        \begin\{cases\}
 *          lower\_bound & b_k' \le lower\_bound     \\
 *          upper\_bound & b_k' \ge upper\_bound     \\
 *          b_k' & otherwise
 *        \end\{cases\}                                                 \\
 *      \qquad\text{ for }k\in 0\ ...\ (length-1)
 * @f$ }
 * 
 * @param[out]  a               Output vector @vector{a}
 * @param[in]   b               Input vector @vector{b}
 * @param[in]   length          Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]   lower_bound     Lower bound of clipping range
 * @param[in]   upper_bound     Upper bound of clipping range
 * @param[in]   b_shr           Arithmetic right-shift applied to elements of `b`
 * 
 * @returns  Headroom of output vector @vector{a}
 */
headroom_t xs3_vect_s32_clip(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const int32_t lower_bound,
    const int32_t upper_bound,
    const right_shift_t b_shr);


/**
 * @brief Compute the inner product between two 32-bit vectors.
 * 
 * xs3_vect_s32_dot() and xs3_vect_s32_dot_calc_params() together represent the following BFP operation:
 * 
 * @math{ a \cdot 2^{a\_exp} \leftarrow 
 *      \sum_{k=0}^{length-1} \left( (b_k \cdot 2^{b\_exp}) \cdot (c_k \cdot 2^{c\_exp}) \right) }
 * 
 * xs3_vect_s32_dot_calc_params() should be called first to compute the shift parameters @math{b\_shr} and @math{c\_shr}
 * used by this function, as well as output exponent @math{a\_exp}. This function is then used to compute the resulting
 * mantissa vector @vector{a}.
 * 
 * @par Parameter Details
 * 
 * `b[]` and `c[]` represent the 32-bit mantissa vectors @vector{b} and @vector{c} respectively. Each must begin at a 
 * word-aligned address.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b} and @vector{c} 
 * respectively.
 * 
 * \low_op{32, @f$ 
 *      b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)                     \\
 *      c_k' \leftarrow sat_{32}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)                     \\
 *      a \leftarrow \sum_{k=0}^{length-1}\left(round( b_k' \cdot c_k' \cdot 2^{-30} ) \right)   \\
 *      \qquad\text{where } a \text{ is returned}
 * @f$ }
 * 
 * @par Additional Details
 * 
 * The 30-bit rounding right-shift applied to each of the 64-bit products @math{b_k \cdot c_k} is a feature of the 
 * hardware and cannot be avoided. As such, if the input vectors @vector{b} and @vector{c} together have too much 
 * headroom (i.e. @math{b\_hr + c\_hr}), the sum may effectively vanish. To avoid this situation, negative values of 
 * `b_shr` and `c_shr` may be used (with the stipulation that @math{b\_shr \ge -b\_hr} and @math{c\_shr \ge -c\_hr} if
 * saturation of @math{b_k'} and @math{c_k'} is to be avoided). The less headroom @math{b_k'} and @math{c_k'} have, the
 * greater the precision of the final result.
 * 
 * Internally, each product @math{(b_k' \cdot c_k' \cdot 2^{-30})} accumulates into one of eight 40-bit accumulators 
 * (which are all used simultaneously) which apply symmetric 40-bit saturation logic (with bounds @math{\approx 2^{39})
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
 * 
 * @param[in] b             Input vector @vector{b}
 * @param[in] c             Input vector @vector{c}
 * @param[in] length        Number of elements in vectors @vector{b} and @vector{c}
 * @param[in] b_shr         Right-shift appled to @vector{b}
 * @param[in] c_shr         Right-shift appled to @vector{c}
 * 
 * @returns The inner product of vectors @vector{b} and @vector{c}, scaled as indicated above.
 */
int64_t xs3_vect_s32_dot(
    const int32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Obtain the shift parameters used by xs3_vect_s32_dot().
 * 
 * @param[out]  a_exp               Exponent of output of xs3_vect_s32_dot()
 * @param[out]  b_shr               Right-shift to be applied to elements of @vector{b}
 * @param[out]  c_shr               Right-shift to be applied to elements of @vector{c}
 * @param[in]   b_exp               Exponent of @vector{b}
 * @param[in]   c_exp               Exponent of @vector{c}
 * @param[in]   b_hr                Headroom of @vector{b}
 * @param[in]   c_hr                Headroom of @vector{c}
 * @param[in]   length              Number of elements in vectors @vector{b} and @vector{c}
 * @param[in]   allow_saturation    Whether xs3_vect_s32_dot() may saturate
 * 
 * @see xs3_vect_s32_dot
 */
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
 * @brief Calculate the energy (sum of squares of elements) of a 32-bit vector.
 * 
 * xs3_vect_s32_energy() and xs3_vect_s32_energy_calc_params() together represent the following BFP operation:
 * 
 * @math{ a \cdot 2^{a\_exp} \leftarrow \sum_{k=0}^{length-1} \left( b_k \cdot 2^{b\_exp} \right)^2 }
 * 
 * xs3_vect_s32_energy_calc_params() should be called first to compute the shift parameter @math{b\_shr} which is used
 * by this function, as well as output exponent @math{a\_exp}. This function is then used to compute the resulting
 * mantissa @math{a}, which is returned.
 * 
 * @par Parameter Details
 * 
 * `b[]` represents the 32-bit mantissa vector @vector{b}. `b[]` must begin at a word-aligned address.
 * 
 * `length` is the number of elements in @vector{b}.
 * 
 * `b_shr` is the signed arithmetic right-shift applied to elements of @vector{b}.
 * 
 * @low_op{32, @f$
 *      b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)                 \\
 *      a \leftarrow \sum_{k=0}^{length-1} round((b_k')^2 \cdot 2^{-30})
 * @f$ }
 * 
 * @par Additional Details
 * 
 * The 30-bit rounding right-shift applied to each element of the 64-bit products @math{(b_k')^2} is a feature of the 
 * hardware and cannot be avoided. As such, if the input vector @vector{b} has too much headroom (i.e. 
 * @math{2\cdot b\_hr}), the sum may effectively vanish. To avoid this situation, negative values of `b_shr` may be used
 * (with the stipulation that @math{b\_shr \ge -b\_hr} if satuartion of @math{b_k'} is to be avoided). The less headroom
 * @math{b_k'} has, the greater the precision of the final result.
 * 
 * Internally, each product @math{(b_k')^2 \cdot 2^{-30}} accumulates into one of eight 40-bit accumulators (which are 
 * all used simultaneously) which apply symmetric 40-bit saturation logic (with bounds @math{\approx 2^{39}) with each
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
 * 
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   length      Number of elements in @vector{b}
 * @param[in]   b_shr       Right-shift appled to @vector{b}
 * 
 * @returns 64-bit mantissa of vector @vector{b}'s energy
 */
int64_t xs3_vect_s32_energy(
    const int32_t b[],
    const unsigned length,
    const right_shift_t b_shr);


/**
 * @brief Obtain the shift parameters used by xs3_vect_s32_energy().
 * 
 * @param[out]  a_exp       Exponent of outputs of xs3_vect_s32_energy()
 * @param[out]  b_shr       Right-shift to be applied to elements of @vector{b}
 * @param[in]   length      Number of elements in vector @vector{b}
 * @param[in]   b_exp       Exponent of vector{b}
 * @param[in]   b_hr        Headroom of vector{b}
 * 
 * @see xs3_vect_s32_energy
 */
void xs3_vect_s32_energy_calc_params(
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
 * @par Parameter Details
 * 
 * `x[]` represents the 32-bit vector @vector{x}. `x[]` must begin at a word-aligned address.
 * 
 * `length` is the number of elements in `x[]`.
 * 
 * 
 * @low_op{32, @f$
 *      min\!\\{ HR_{32}\left(x_0\right), HR_{32}\left(x_1\right), ..., HR_{32}\left(x_{length-1}\right) \\}
 * @f$ }
 * 
 * @param[in]   x           Input vector @vector{x}
 * @param[in]   length      The number of elements in `x[]`
 * 
 * @returns     Headroom of vector @vector{x} 
 * 
 * @see xs3_vect_ch_pair_s16_headroom
 * @see xs3_vect_ch_pair_s32_headroom
 * @see xs3_vect_s16_headroom
 * @see xs3_vect_complex_s16_headroom
 * @see xs3_vect_complex_s32_headroom
 */
headroom_t xs3_vect_s32_headroom(
    const int32_t x[],
    const unsigned length);


/**
 * @brief Compute the inverse of elements of a 32-bit vector.
 * 
 * xs3_vect_s32_inverse() and xs3_vect_s32_inverse_calc_params() together represent the following BFP operation:
 * 
 * @math{ a_k \cdot 2^{a\_exp} \leftarrow \frac{1}{b_k \cdot 2^{b\_exp}} }
 * 
 * xs3_vect_complex_s32_inverse_calc_params() should be called first to compute the shift parameter @math{b\_shr} used
 * by this function, as well as output exponent @math{a\_exp}. This function is then used to compute the resulting 
 * mantissa vector @vector{a}.
 * 
 * @par Parameter Details
 * 
 * `a[]` and `b[]` represent the 32-bit mantissa vectors @vector{a} and @vector{b} respectively. Each vectors must begin
 * at a word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `scale` is a scaling parameter used to maximize the precision of the result.
 * 
 * @low_op{32, @f$
 *      a_k \leftarrow \lfloor\frac{2^{scale}}{b_k}\rfloor      \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)
 * @f$ }
 * 
 * @param[out]  a           Output vector @vector{a}
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   length      Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]   scale       Scale factor applied to dividend when computing inverse
 * 
 * @returns     Headroom of output vector @vector{a}
 */
headroom_t xs3_vect_s32_inverse(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const unsigned scale);


/**
 * @brief Obtain the shift parameters used by xs3_vect_s32_inverse().
 * 
 * @par Word-alignment Required
 *      `b[]` must begin at a word-aligned (4 byte) address.
 * 
 * @param[out]  a_exp       Exponent of output vector @vector{a}
 * @param[out]  scale       Scale factor to be applied when computing inverse
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   b_exp       Exponent of @vector{b}
 * @param[in]   length      Number of elements in vector @vector{b}
 * 
 * @see xs3_vect_s32_inverse
 */
void xs3_vect_s32_inverse_calc_params(
    exponent_t* a_exp,
    unsigned* scale,
    const int32_t b[],
    const exponent_t b_exp,
    const unsigned length);


/**
 * @brief Find the maximum value in a 32-bit vector.
 * 
 * @par Parameter Details
 * 
 * `x[]` represents the 32-bit mantissa vector @vector{x}. It must begin at a word-aligned address.
 * 
 * `length` is the number of elements in @vector{x}.
 * 
 * @low_op{32, @f$ 
 *      max\\{ x_0, x_1, ..., x_{length-1} \\}
 * @f$ }
 * 
 * @param[in]   x           Input vector @vector{x}     
 * @param[in]   length      Number of elements in @vector{x}
 * 
 * @returns     Maximum value from @vector{x}
 */
int32_t xs3_vect_s32_max(
    const int32_t x[],
    const unsigned length);


/**
 * @brief Find the minimum value in a 32-bit vector.
 * 
 * @par Parameter Details
 * 
 * `x[]` represents the 32-bit mantissa vector @vector{x}. It must begin at a word-aligned address.
 * 
 * `length` is the number of elements in @vector{x}.
 * 
 * @low_op{32, @f$ 
 *      min\\{ x_0, x_1, ..., x_{length-1} \\}
 * @f$ }
 * 
 * @param[in]   x           Input vector @vector{x}     
 * @param[in]   length      Number of elements in @vector{x}
 * 
 * @returns     Minimum value from @vector{x}
 */
int32_t xs3_vect_s32_min(
    const int32_t x[],
    const unsigned length);


/**
 * @brief Multiply one 32-bit vector element-wise by another.
 * 
 * xs3_vect_s32_mul() and xs3_s32_mul_calc_params() together represent the following BFP operation:
 * 
 * @math{ a_k \cdot 2^{a\_exp} \leftarrow (b_k \cdot 2^{b\_exp}) \cdot (c_k \cdot 2^{c\_exp}) \cdot 2^{-30} }
 * 
 * xs3_vect_s32_mul_calc_params() should be called first to compute the shift parameters @math{b\_shr} and @math{c\_shr}
 * used by this function, as well as output exponent @math{a\_exp}. This function is then used to compute the resulting
 * mantissa vector @vector{a}.
 * 
 * @par Parameter Details
 * 
 * `a[]`, `b[]` and `c[]` represent the 32-bit mantissa vectors @vector{a}, @vector{b} and @vector{c} respectively. 
 * Each must begin at a word-aligned address. This operation can be performed safely in-place on `b[]` or `c[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b} and @vector{c} 
 * respectively.
 * 
 * \low_op{32, @f$
 *      b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
 *      c_k' \leftarrow sat_{32}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)     \\
 *      a_k \leftarrow sat_{32}(round(b_k' \cdot c_k' \cdot 2^{-30}))       \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)
 * @f$ }
 * 
 * @param[out] a        Output vector @vector{a}
 * @param[in]  b        Input vector @vector{b}
 * @param[in]  c        Input vector @vector{c}
 * @param[in]  length   Number of elements in vectors @vector{a}, @vector{b} and @vector{c}
 * @param[in]  b_shr    Right-shift appled to @vector{b}
 * @param[in]  c_shr    Right-shift appled to @vector{c}
 * 
 * @returns  Headroom of vector @vector{a}
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
 * @brief Obtain the shift parameters used by xs3_vect_s32_mul().
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
 * @brief Rectify the elements of a 32-bit vector.
 * 
 * xs3_vect_s32_rect() represents the following BFP operation:
 * 
 * @math{ a_k \cdot 2^{a\_exp} \leftarrow 
 *          \begin{cases}
 *              b_k \cdot 2^{b\_exp} & b_k \cdot 2^{b\_exp} \gt 0 \\
 *              0 & otherwise
 *          \end{cases}     \\
 *       \qquad\text{where }a\_exp = b\_exp
 * }
 * 
 * The output exponent @math{a\_exp} 
 * 
 * @par Parameter Details
 * 
 * `a[]` and `b[]` represent the 32-bit mantissa vectors @vector{a} and @vector{b} respectively. Each must begin at a 
 * word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * \low_op{32, @f$
 *      a_k \leftarrow 
 *          \begin\{cases\}
 *              b_k & b_k \gt 0 \\ 
 *              0 & b_k \leq 0
 *      \end\{cases\}           \\
 *      \qquad\text{ for }k\in 0\ ...\ (length-1)
 * @f$ }
 * 
 * @par Safe In-place Computation
 *      This function may safely operate in-place on `b[]`.
 * 
 * @par Word-alignment Required
 *      `a[]` and `b[]` must each begin at a word-aligned (4 byte) address.
 * 
 * @param[out]  a       Output vector @vector{a}
 * @param[in]   b       Input vector @vector{b}
 * @param[in]   length  Number of elements in vectors @vector{a} and @vector{b}
 * 
 * @returns  Headroom of the output vector @vector{a}
 */
headroom_t xs3_vect_s32_rect(
    int32_t a[],
    const int32_t b[],
    const unsigned length);


/**
 * @brief Multiply a 32-bit vector by a scalar.
 * 
 * xs3_vect_s32_scale() and xs3_vect_s32_mul_calc_params() together represent the following BFP operation:
 * 
 * @math{ a_k \cdot 2^{a\_exp} \leftarrow (b_k \cdot 2^{b\_exp}) \cdot (c \cdot 2^{c\_exp}) \cdot 2^{-30} }
 * 
 * xs3_vect_s32_mul_calc_params() should be called first to compute the shift parameters @math{b\_shr} and 
 * @math{c\_shr}, as well as the output exponent @math{a\_exp}. @math{b\_shr} is used by this function and @math{c\_exp}
 * is applied to @math{c} in user code. This function is then used to compute the resulting mantissa vector @vector{a}.
 * 
 * @todo To mitigate the possibility of user error, this function should just take @math{c\_shr} directly as a parameter 
 * and apply the shift itself.
 * 
 * @par Parameter Details
 * 
 * `a[]` and `b[]`represent the 32-bit mantissa vectors @vector{a} and @vector{b} respectively. Each must begin at a 
 * word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `c` is the 32-bit scalar @math{c} by which each element of @vector{b} is multiplied.
 * 
 * `b_shr` is the signed arithmetic right-shift applied to each element of @vector{b}.
 * 
 * \low_op{32, @f$
 *      b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
 *      a_k  \leftarrow sat_{32}(round(c \cdot b_k' \cdot 2^{-30}))         \\
 *           \qquad\text{ for }k\in 0\ ...\ (length-1)
 * @f$ }
 * 
 * @param[out]  a           Output vector @vector{a}
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   length      Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]   c           Scalar to be multiplied by elements of @vector{b}
 * @param[in]   b_shr       Right-shift appled to @vector{b}
 * 
 * @returns  Headroom of output vector @vector{a}
 * 
 * @see xs3_vect_s32_mul_calc_params
 */
headroom_t xs3_vect_s32_scale(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const int32_t c,
    const right_shift_t b_shr);


/** 
 * @brief Set all elements of a 32-bit vector to the specified value.
 * 
 * xs3_vect_s32_set() represents the following BFP operation:
 * 
 * @math{ a_k \cdot 2^{a\_exp} \leftarrow b \cdot 2^{b\_exp}    \\
 *          \qquad\text{where } a\_exp = b\_exp }
 * 
 * @par Parameter Details
 * 
 * `a[]` represents the 32-bit output vector @vector{a}. `a[]` must begin at a word-aligned address.
 * 
 * `b` is the new value to set each element of @vector{a} to.
 * 
 * @low_op{32, @f$
 *      a_k \leftarrow b    \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)
 * @f$ }
 * 
 * @param[out]  a           Output vector @vector{a}
 * @param[in]   b           New value for the elements of @vector{a}
 * @param[in]   length      Number of elements in @vector{a}
 */
void xs3_vect_s32_set(
    int32_t a[],
    const int32_t b,
    const unsigned length);


/**
 * @brief Left-shift the elements of a 32-bit vector by a specified number of bits.
 * 
 * `a[]` and `b[]` represent the 32-bit integer vectors @vector{a} and @vector{b} respectively. Each must begin at a 
 * word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in @vector{a} and @vector{b}.
 * 
 * `b_shl` is the signed arithmetic left-shift applied to each element of @vector{b}. 
 * 
 * @low_op{32, @f$ 
 *      a_k \leftarrow sat_{32}(\lfloor b_k \cdot 2^{b\_shl} \rfloor)     \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
 * 
 * @param[out]  a           Output vector @vector{a}
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   length      Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]   b_shl       Arithmetic left-shift applied to elements of @vector{b}
 * 
 * @returns     Headroom of output vector @vector{a}
 */
headroom_t xs3_vect_s32_shl(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const left_shift_t b_shl);

    
/**
 * @brief Right-shift the elements of a 32-bit vector by a specified number of bits.
 * 
 * @par Parameter Details
 * 
 * `a[]` and `b[]` represent the 32-bit integer vectors @vector{a} and @vector{b} respectively. Each must begin at a 
 * word-aligned address. This operation can be performed safely in-place on `b[]`.
 * 
 * `length` is the number of elements in @vector{a} and @vector{b}.
 * 
 * `b_shr` is the signed arithmetic right-shift applied to each element of @vector{b}. 
 * 
 * @low_op{32, @f$ 
 *      a_k \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)      \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
 * 
 * @param[out]  a           Output vector @vector{a}
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   length      Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]   b_shr       Arithmetic right-shift applied to elements of @vector{b}
 * 
 * @returns     Headroom of output vector @vector{a}
 */
headroom_t xs3_vect_s32_shr(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const right_shift_t shr);


/**
 * @brief Compute the square root of elements of a 32-bit vector.
 * 
 * xs3_vect_s32_sqrt() and xs3_vect_s32_sqrt_calc_params() together represent the following BFP operation:
 * 
 * @math{ a_k \cdot 2^{a\_exp} \leftarrow \sqrt{ b_k \cdot 2^{b\_exp}  }}
 * 
 * xs3_vect_s32_sqrt_calc_params() should be called first to compute the shift parameter @math{b\_shr} which is used by
 * this function, as well as output exponent @math{a\_exp}. This function is then used to compute the resulting mantissa
 * vector @vector{a}.
 * 
 * @par Parameter Details
 * 
 * `a[]` and `b[]` represent the 32-bit mantissa vectors @vector{a} and @vector{b} respectively. Each vectors must begin
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
 * @low_op{32, @f$ 
 *      b_k' \leftarrow sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)     \\
 *      a_k \leftarrow \sqrt{ b_k' }                                        \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)                       \\
 *          \qquad\text{ where } sqrt() \text{ computes the first } depth \text{ bits of the square root.}
 * @f$ }
 * 
 * @param[out]  a           Output vector @vector{a}
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   length      Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]   b_shr       Right-shift appled to @vector{b}
 * @param[in]   depth       Number of bits of each output value to compute
 * 
 * @returns     Headroom of output vector @vector{a}
 */
headroom_t xs3_vect_s32_sqrt(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const right_shift_t b_shr,
    const unsigned depth);


/**
 * @brief Obtain the shift parameters used by xs3_vect_s32_sqrt().
 * 
 * @param[out]  a_exp       Exponent of output elements from xs3_vect_s32_sqrt()
 * @param[out]  b_shr       Right-shift appled to @vector{b}
 * @param[in]   b_exp       Exponent of input vector @vector{b}
 * @param[in]   b_hr        Headroom of input vector @vector{b}
 * 
 * @see xs3_vect_s32_sqrt
 */
void xs3_vect_s32_sqrt_calc_params(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    const exponent_t b_exp,
    const right_shift_t b_hr);


/**
 * @brief Subtract one 32-bit vector from another.
 * 
 * xs3_vect_s32_sub() and xs3_vect_add_sub_calc_params() together represent the following BFP operation: 
 * 
 * @math{ \bar{a} \cdot 2^{a\_exp} \leftarrow \left( \bar{b} \cdot 2^{b\_exp} \right) 
 *                                          - \left( \bar{c} \cdot 2^{c\_exp} \right) }
 * 
 * xs3_vect_add_sub_calc_params() should be called first to compute the shift parameters @math{b\_shr} and @math{c\_shr} 
 * used by this function, as well as output exponent @math{a\_exp}. This function is then used to compute the resulting 
 * mantissa vector @vector{a}.
 * 
 * @par Parameter Details
 * 
 * `a[]`, `b[]` and `c[]` represent the 32-bit mantissa vectors @vector{a}, @vector{b} and @vector{c} 
 * respectively. Each must begin at a word-aligned address. This operation can be performed safely in-place on `b[]` or
 * `c[]`.
 * 
 * `length` is the number of elements in each of the vectors.
 * 
 * `b_shr` and `c_shr` are the signed arithmetic right-shifts applied to each element of @vector{b} and @vector{c} 
 * respectively.
 * 
 * @low_op{32, @f$ 
 *      b_k' = sat_{32}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)  \\
 *      c_k' = sat_{32}(\lfloor c_k \cdot 2^{-c\_shr} \rfloor)  \\
 *      a_k \leftarrow sat_{32}\!\left( b_k' - c_k' \right)     \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
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
 * @see xs3_vect_add_sub_calc_params
 */
headroom_t xs3_vect_s32_sub(
    int32_t a[],
    const int32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Sum the elements of a 32-bit vector.
 * 
 * xs3_vect_s32_sum() represents the following BFP operation:
 * 
 * @math{ a \cdot 2^{a\_exp} \leftarrow \sum_{k=0}^{length-1} \left( b_k \cdot 2^{b\_exp} \right)   \\
 *          \qquad\text{where } a\_exp = b\_exp
 * }
 * 
 * @par Parameter Details
 * 
 * `b[]` represents the 32-bit mantissa vector @vector{b}. `b[]` must begin at a word-aligned address.
 * 
 * `length` is the number of elements in @vector{b}.
 * 
 * \low_op{32, @f$ 
 *      a \leftarrow \sum_\{k=0\}^\{length-1\} b_k     
 * @f$ }
 * 
 * @par Additional Details
 * 
 * Internally, each element accumulates into one of eight 40-bit accumulators (which are all used simultaneously) which 
 * apply symmetric 40-bit saturation logic (with bounds @math{\approx 2^{39}) with each value added. The saturating 
 * arithmetic employed is _not associative_ and no indication is given if saturation occurs at an intermediate step. To 
 * avoid the possibility of saturation errors, `length` should be no greater than @math{2^{11+b\_hr}}, where 
 * @math{b\_hr} is the headroom of @vector{b}.
 * 
 * If the caller's mantissa vector is longer than that, the full result can be found by calling this function multiple 
 * times for partial results on sub-sequences of the input, and adding the results in user code. 
 * 
 * In many situations the caller may have _a priori_ knowledge that saturation is impossible (or very nearly so), in 
 * which case this guideline may be disregarded. However, such situations are application-specific and are well beyond 
 * the scope of this documentation, and as such are left to the user's discretion.
 * 
 * @param[in]   b         Input vector @vector{b}
 * @param[in]   length    Number of elements in vector @vector{b}
 * 
 * @returns  64-bit mantissa of the sum, @math{a}.
 */
int64_t xs3_vect_s32_sum(
    const int32_t b[],
    const unsigned length);


/** 
 * @brief Convert a 32-bit vector to a 16-bit vector.
 * 
 * This function converts a 32-bit mantissa vector @vector{b} into a 16-bit mantissa vector @vector{a}. Conceptually,
 * the output BFP vector @math{\bar{a}\cdot 2^{a\_exp}} represents the same values as the input BFP vector
 * @math{\bar{b}\cdot 2^{b\_exp}}, only with a reduced bit-depth.
 * 
 * In most cases @math{b\_shr} should be @math{16 - b\_hr}, where @math{b\_hr} is the headroom of the 32-bit input
 * mantissa vector @vector{b}. 
 * 
 * The output exponent @math{a\_exp} will be given by
 * 
 * @math{ a\_exp = b\_exp + b\_shr }
 * 
 * @par Parameter Details
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
 * 
 * \low_op{32, @f$
 *      a_k \leftarrow sat_{16}(\lfloor b_k \cdot 2^{-b\_shr} \rfloor)      \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) 
 * @f$ }
 * 
 * @param[out]  a        Output vector @vector{a}
 * @param[in]   b        Input vector @vector{b}
 * @param[in]   length   Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]   b_shr    Right-shift appled to @vector{b}
 * 
 * @see xs3_vect_s16_to_s32
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