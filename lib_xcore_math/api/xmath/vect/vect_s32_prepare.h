// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include "xmath/xmath_conf.h"
#include "xmath/types.h"
#include "xmath/util.h"

/**
 * @defgroup vect_s32_prepare_api   32-bit Vector Prepare Functions
 */

#ifdef __XC__
extern "C" {
#endif


/**
 * @brief Obtain the output exponent and input shifts to add or subtract two 16- or 32-bit BFP
 *        vectors.
 *
 * The block floating-point functions in this library which add or subtract vectors are of the
 * general form:
 * 
 * @math{ 
 *      \bar{a} \cdot 2^{a\_exp} = \bar{b}\cdot 2^{b\_exp} \pm \bar{c}\cdot 2^{c\_exp} }
 * }
 * 
 * @vector{b} and @vector{c} are the input mantissa vectors with exponents @math{b\_exp} and
 * @math{c\_exp}, which are shared by each element of their respective vectors. @vector{a} is the
 * output mantissa vector with exponent 
 * @math{a\_exp}. Two additional properties, @math{b\_hr} and @math{c\_hr}, which are the headroom
 * of mantissa vectors @vector{b} and @vector{c} respectively, are required by this function.
 *
 * In order to avoid any overflows in the output mantissas, the output exponent @math{a\_exp} must
 * be chosen such that the largest (in the sense of absolute value) possible output mantissa will
 * fit into the allotted space (e.g. 32 bits for vect_s32_add()). Once @math{a\_exp} is chosen, the
 * input bit-shifts @math{b\_shr} and @math{c\_shr} are calculated to achieve that resulting
 * exponent.
 *
 * This function chooses @math{a\_exp} to be the minimum exponent known to avoid overflows, given
 * the input exponents (@math{b\_exp} and @math{c\_exp}) and input headroom (@math{b\_hr} and
 * @math{c\_hr}).
 * 
 * This function is used calculate the output exponent and input bit-shifts for each of the
 * following functions:
 * * vect_s16_add()
 * * vect_s32_add()
 * * vect_s16_sub()
 * * vect_s32_sub()
 * * vect_complex_s16_add()
 * * vect_complex_s32_add()
 * * vect_complex_s16_sub()
 * * vect_complex_s32_sub()
 * 
 * @par Adjusting Output Exponents
 * @parblock
 * 
 * If a specific output exponent `desired_exp` is needed for the result (e.g. for emulating
 * fixed-point arithmetic), the `b_shr` and `c_shr` produced by this function can be adjusted
 * according to the following:
 * \code{.c}
 *      exponent_t desired_exp = ...; // Value known a priori
 *      right_shift_t new_b_shr = b_shr + (desired_exp - a_exp);
 *      right_shift_t new_c_shr = c_shr + (desired_exp - a_exp);
 * \endcode
 * 
 * When applying the above adjustment, the following conditions should be maintained:
 * * `b_hr + b_shr >= 0`
 * * `c_hr + c_shr >= 0`
 * 
 * Be aware that using smaller values than strictly necessary for `b_shr` and `c_shr` can result in
 * saturation, and using larger values may result in unnecessary underflows or loss of precision.
 * @endparblock
 * 
 * @par Notes
 * @parblock
 * 
 * * If @math{b\_hr} or @math{c\_hr} are unknown, they can be calculated using the appropriate
 *   headroom function (e.g. vect_complex_s16_headroom() for complex 16-bit vectors) or the value
 *   `0` can always be safely used (but may result in reduced precision).
 * @endparblock
 *
 * @param[out] a_exp    Output exponent associated with output mantissa vector @vector{a}
 * @param[out] b_shr    Signed arithmetic right-shift to be applied to elements of @vector{b}. Used
 *                      by the function which computes the output mantissas @vector{a}
 * @param[out] c_shr    Signed arithmetic right-shift to be applied to elements of @vector{c}. Used
 *                      by the function which computes the output mantissas @vector{a}
 * @param[in]  b_exp    Exponent of BFP vector @vector{b}
 * @param[in]  c_exp    Exponent of BFP vector @vector{c}
 * @param[in]  b_hr     Headroom of BFP vector @vector{b} 
 * @param[in]  c_hr     Headroom of BFP vector @vector{c}
 * 
 * @see vect_s16_add, 
 *      vect_s32_add, 
 *      vect_s16_sub, 
 *      vect_s32_sub, 
 *      vect_complex_s16_add, 
 *      vect_complex_s32_add, 
 *      vect_complex_s16_sub, 
 *      vect_complex_s32_sub
 * 
 * @ingroup vect_s32_prepare_api
 */
C_API
void vect_s32_add_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    right_shift_t* c_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr);


/**
 * @brief Obtain the output exponent and shifts required for a call to `vect_s32_add_scalar()`.
 * 
 * The logic for computing the shifts and exponents of `vect_s32_add_scalar()` is identical to
 * that for `vect_s32_add()`.
 * 
 * This macro is provided as a convenience to developers and to make the code more readable.
 * 
 * @see vect_s32_add_prepare()
 * 
 * @ingroup vect_s32_prepare_api
 */
#define vect_s32_add_scalar_prepare vect_s32_add_prepare

/**
 * @brief Obtain the output exponent, input shift and modified bounds used by vect_s32_clip().
 * 
 * This function is used in conjunction with vect_s32_clip() to bound the elements of a 32-bit BFP
 * vector to a specified range.
 *
 * This function computes `a_exp`, `b_shr`, `lower_bound` and `upper_bound`.
 *
 * `a_exp` is the exponent associated with the 32-bit mantissa vector @vector{a} computed by
 * vect_s32_clip().
 *
 * `b_shr` is the shift parameter required by vect_s32_clip() to achieve the output exponent
 * `a_exp`.
 *
 * `lower_bound` and `upper_bound` are the 32-bit mantissas which indicate the lower and upper
 * clipping bounds respectively. The values are modified by this function, and the resulting values
 * should be passed along to vect_s32_clip().
 *
 * `b_exp` is the exponent associated with the input mantissa vector @vector{b}.
 *
 * `bound_exp` is the exponent associated with the bound mantissas `lower_bound` and `upper_bound`
 * respectively.
 *
 * `b_hr` is the headroom of @vector{b}. If unknown, it can be obtained using vect_s32_headroom().
 * Alternatively, the value `0` can always be safely used (but may result in reduced precision).
 * 
 * @param[out]    a_exp               Exponent associated with output mantissa vector @vector{a}
 * @param[out]    b_shr               Signed arithmetic right-shift for @vector{b} used by
 *                                    vect_s32_clip()
 * @param[inout]  lower_bound         Lower bound of clipping range
 * @param[inout]  upper_bound         Upper bound of clipping range
 * @param[in]     b_exp               Exponent associated with input mantissa vector @vector{b}
 * @param[in]     bound_exp           Exponent associated with clipping bounds `lower_bound` and
 *                                    `upper_bound`
 * @param[in]     b_hr                Headroom of input mantissa vector @vector{b}
 * 
 * @see vect_s32_clip
 * 
 * @ingroup vect_s32_prepare_api
 */
C_API
void vect_s32_clip_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    int32_t* lower_bound,
    int32_t* upper_bound,
    const exponent_t b_exp,
    const exponent_t bound_exp,
    const headroom_t b_hr);


/**
 * @brief Obtain the output exponent and input shift used by vect_s32_dot().
 * 
 * This function is used in conjunction with vect_s32_dot() to compute the inner product of two
 * 32-bit BFP vectors.
 *
 * This function computes `a_exp`, `b_shr` and `c_shr`.
 *
 * `a_exp` is the exponent associated with the 64-bit mantissa @math{a} returned by vect_s32_dot(),
 * and must be chosen to be large enough to avoid saturation when @math{a} is computed. To maximize
 * precision, this function chooses `a_exp` to be the smallest exponent known to avoid saturation
 * (see exception below). The `a_exp` chosen by this function is derived from the exponents and
 * headrooms associated with the input vectors.
 *
 * `b_shr` and `c_shr` are the shift parameters required by vect_s32_dot() to achieve the chosen
 * output exponent `a_exp`.
 *
 * `b_exp` and `c_exp` are the exponents associated with the input mantissa vectors @vector{b} and
 * @vector{c} respectively.
 *
 * `b_hr` and `c_hr` are the headroom of @vector{b} and @vector{c} respectively. If either is
 * unknown, they can be obtained using vect_s32_headroom(). Alternatively, the value `0` can always
 * be safely used (but may result in reduced precision).
 *
 * `length` is the number of elements in the input mantissa vectors @vector{b} and @vector{c}.
 * 
 * @par Adjusting Output Exponents
 * @parblock
 * 
 * If a specific output exponent `desired_exp` is needed for the result (e.g. for emulating
 * fixed-point arithmetic), the `b_shr` and `c_shr` produced by this function can be adjusted
 * according to the following:
 * \code{.c}
 *      exponent_t desired_exp = ...; // Value known a priori
 *      right_shift_t new_b_shr = b_shr + (desired_exp - a_exp);
 *      right_shift_t new_c_shr = c_shr + (desired_exp - a_exp);
 * \endcode
 * 
 * When applying the above adjustment, the following conditions should be maintained:
 * * `b_hr + b_shr >= 0`
 * * `c_hr + c_shr >= 0`
 * 
 * Be aware that using smaller values than strictly necessary for `b_shr` or `c_shr` can result in
 * saturation, and using larger values may result in unnecessary underflows or loss of precision.
 * @endparblock
 * 
 * @param[out]  a_exp               Exponent associated with output mantissa @math{a}
 * @param[out]  b_shr               Signed arithmetic right-shift for @vector{b} used by
 *                                  vect_s32_dot()
 * @param[out]  c_shr               Signed arithmetic right-shift for @vector{c} used by
 *                                  vect_s32_dot()
 * @param[in]   b_exp               Exponent associated with input mantissa vector @vector{b}
 * @param[in]   c_exp               Exponent associated with input mantissa vector @vector{b}
 * @param[in]   b_hr                Headroom of input mantissa vector @vector{b}
 * @param[in]   c_hr                Headroom of input mantissa vector @vector{b}
 * @param[in]   length              Number of elements in vectors @vector{b} and @vector{c}
 * 
 * @see vect_s32_dot
 * 
 * @ingroup vect_s32_prepare_api
 */
C_API
void vect_s32_dot_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    right_shift_t* c_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr,
    const unsigned length);


/**
 * @brief Obtain the output exponent and input shift used by vect_s32_energy().
 * 
 * This function is used in conjunction with vect_s32_energy() to compute the inner product of a
 * 32-bit BFP vector with itself.
 *
 * This function computes `a_exp` and `b_shr`.
 *
 * `a_exp` is the exponent associated with the 64-bit mantissa @math{a} returned by
 * vect_s32_energy(), and must be chosen to be large enough to avoid saturation when @math{a} is
 * computed. To maximize precision, this function chooses `a_exp` to be the smallest exponent known
 * to avoid saturation (see exception below). The `a_exp` chosen by this function is derived from
 * the exponent and headroom associated with the input vector.
 *
 * `b_shr` is the shift parameter required by vect_s32_energy() to achieve the chosen output
 * exponent `a_exp`.
 *
 * `b_exp` is the exponent associated with the input mantissa vector @vector{b}.
 *
 * `b_hr` is the headroom of @vector{b}. If it is unknown, it can be obtained using
 * vect_s32_headroom(). Alternatively, the value `0` can always be safely used (but may result in
 * reduced precision).
 *
 * `length` is the number of elements in the input mantissa vector @vector{b}.
 * 
 * @par Adjusting Output Exponents
 * @parblock
 * 
 * If a specific output exponent `desired_exp` is needed for the result (e.g. for emulating
 * fixed-point arithmetic), the `b_shr` produced by this function can be adjusted according to the
 * following:
 * \code{.c}
 *      exponent_t desired_exp = ...; // Value known a priori
 *      right_shift_t new_b_shr = b_shr + (desired_exp - a_exp);
 * \endcode
 * 
 * When applying the above adjustment, the following condition should be maintained:
 * * `b_hr + b_shr >= 0`
 * 
 * Be aware that using smaller values than strictly necessary for `b_shr` can result in saturation,
 * and using larger values may result in unnecessary underflows or loss of precision.
 * @endparblock
 * 
 * @param[out]  a_exp       Exponent of outputs of vect_s32_energy()
 * @param[out]  b_shr       Right-shift to be applied to elements of @vector{b}
 * @param[in]   length      Number of elements in vector @vector{b}
 * @param[in]   b_exp       Exponent of vector{b}
 * @param[in]   b_hr        Headroom of vector{b}
 * 
 * @see vect_s32_energy
 * 
 * @ingroup vect_s32_prepare_api
 */
C_API
void vect_s32_energy_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    const unsigned length,
    const exponent_t b_exp,
    const headroom_t b_hr);


/**
 * @brief Obtain the output exponent and scale used by vect_s32_inverse().
 * 
 * This function is used in conjunction with vect_s32_inverse() to compute the inverse of elements
 * of a 32-bit BFP vector.
 *
 * This function computes `a_exp` and `scale`.
 *
 * `a_exp` is the exponent associated with output mantissa vector @vector{a}, and must be chosen to
 * avoid overflow in the smallest element of the input vector, which when inverted becomes the
 * largest output element. To maximize precision, this function chooses `a_exp` to be the smallest
 * exponent known to avoid saturation. The `a_exp` chosen by this function is derived from the
 * exponent and smallest element of the input vector.
 *
 * `scale` is a scaling parameter used by vect_s32_inverse() to achieve the chosen output exponent.
 *
 * `b[]` is the input mantissa vector @vector{b}.
 *
 * `b_exp` is the exponent associated with the input mantissa vector @vector{b}.
 * 
 * `length` is the number of elements in @vector{b}.
 * 
 * @param[out]  a_exp       Exponent of output vector @vector{a}
 * @param[out]  scale       Scale factor to be applied when computing inverse
 * @param[in]   b           Input vector @vector{b}
 * @param[in]   b_exp       Exponent of @vector{b}
 * @param[in]   length      Number of elements in vector @vector{b}
 * 
 * @see vect_s32_inverse
 * 
 * @ingroup vect_s32_prepare_api
 */
C_API
void vect_s32_inverse_prepare(
    exponent_t* a_exp,
    unsigned* scale,
    const int32_t b[],
    const exponent_t b_exp,
    const unsigned length);


/**
 * @brief Obtain the output exponent and shifts needed by vect_s32_macc().
 *
 * This function is used in conjunction with vect_s32_macc() to perform an element-wise
 * multiply-accumlate of 32-bit BFP vectors.
 *
 * This function computes `new_acc_exp`, `acc_shr`, `b_shr` and `c_shr`, which are selected to
 * maximize precision in the resulting accumulator vector without causing saturation of final or
 * intermediate values. Normally the caller will pass these outputs to their corresponding inputs of
 * vect_s32_macc().
 *
 * `acc_exp` is the exponent associated with the accumulator mantissa vector @vector{a} prior to the
 * operation, whereas `new_acc_exp` is the exponent corresponding to the updated accumulator vector.
 *
 * `b_exp` and `c_exp` are the exponents associated with the complex input mantissa vectors
 * @vector{b} and @vector{c} respectively.
 *
 * `acc_hr`, `b_hr` and `c_hr` are the headrooms of @vector{a}, @vector{b} and @vector{c}
 * respectively. If the headroom of any of these vectors is unknown, it can be obtained by calling
 * vect_s32_headroom(). Alternatively, the value `0` can always be safely used (but may result in
 * reduced precision).
 *
 * @par Adjusting Output Exponents
 * @parblock
 *
 * If a specific output exponent `desired_exp` is needed for the result (e.g. for emulating
 * fixed-point arithmetic), the `acc_shr` and `bc_sat` produced by this function can be adjusted
 * according to the following: 
 *
 * @code{.c}
 *      // Presumed to be set somewhere
 *      exponent_t acc_exp, b_exp, c_exp;
 *      headroom_t acc_hr, b_hr, c_hr;
 *      exponent_t desired_exp;
 *      
 *      ...
 * 
 *      // Call prepare
 *      right_shift_t acc_shr, b_shr, c_shr;
 *      vect_s32_macc_prepare(&acc_exp, &acc_shr, &b_shr, &c_shr, 
 *                                acc_exp, b_exp, c_exp,
 *                                acc_hr, b_hr, c_hr);
 * 
 *      // Modify results
 *      right_shift_t mant_shr = desired_exp - acc_exp;
 *      acc_exp += mant_shr;
 *      acc_shr += mant_shr;
 *      b_shr  += mant_shr;
 *      c_shr  += mant_shr;
 *      
 *      // acc_shr, b_shr and c_shr may now be used in a call to vect_s32_macc() 
 * @endcode
 *
 * When applying the above adjustment, the following conditions should be maintained:
 * * `acc_shr > -acc_hr`  (Shifting any further left may cause saturation)
 * * `b_shr => -b_hr`  (Shifting any further left may cause saturation)
 * * `c_shr => -c_hr`  (Shifting any further left may cause saturation)
 *
 * It is up to the user to ensure any such modification does not result in saturation or
 * unacceptable loss of precision.
 * @endparblock
 *
 *
 * @param[out]  new_acc_exp   Exponent associated with output mantissa vector @vector{a} (after
 *                            macc)
 * @param[out]  acc_shr       Signed arithmetic right-shift used for @vector{a} in vect_s32_macc()
 * @param[out]  b_shr         Signed arithmetic right-shift used for @vector{b} in vect_s32_macc()
 * @param[out]  c_shr         Signed arithmetic right-shift used for @vector{c} in vect_s32_macc()
 * @param[in]   acc_exp       Exponent associated with input mantissa vector @vector{a} (before
 *                            macc)
 * @param[in]   b_exp         Exponent associated with input mantissa vector @vector{b}
 * @param[in]   c_exp         Exponent associated with input mantissa vector @vector{c}
 * @param[in]   acc_hr        Headroom of input mantissa vector @vector{a} (before macc)
 * @param[in]   b_hr          Headroom of input mantissa vector @vector{b}
 * @param[in]   c_hr          Headroom of input mantissa vector @vector{c}
 *
 * @see vect_s32_macc
 *
 * @ingroup vect_s32_prepare_api
 */
C_API
void vect_s32_macc_prepare(
    exponent_t* new_acc_exp,
    right_shift_t* acc_shr,
    right_shift_t* b_shr,
    right_shift_t* c_shr,
    const exponent_t acc_exp,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t acc_hr,
    const headroom_t b_hr,
    const headroom_t c_hr);


/**
 * @brief Obtain the output exponent and shifts required for a call to vect_s32_nmacc().
 *
 * The logic for computing the shifts and exponents of `vect_s32_nmacc()` is identical
 * to that for `vect_s32_macc_prepare()`.
 *
 * This macro is provided as a convenience to developers and to make the code more readable.
 *
 * @see vect_s32_macc_prepare(), vect_s32_nmacc()
 *
 * @ingroup vect_s32_prepare_api
 */
#define vect_s32_nmacc_prepare  vect_s32_macc_prepare


/**
 * @brief Obtain the output exponent and input shifts used by vect_s32_mul().
 *
 * This function is used in conjunction with vect_s32_mul() to perform an element-wise
 * multiplication of two 32-bit BFP vectors.
 *
 * This function computes `a_exp`, `b_shr`, `c_shr`.
 *
 * `a_exp` is the exponent associated with mantissa vector @vector{a}, and must be chosen to be
 * large enough to avoid overflow when elements of @vector{a} are computed. To maximize precision,
 * this function chooses `a_exp` to be the smallest exponent known to avoid saturation (see
 * exception below). The `a_exp` chosen by this function is derived from the exponents and headrooms
 * of associated with the input vectors.
 *
 * `b_shr` and `c_shr` are the shift parameters required by vect_complex_s32_mul() to achieve the
 * chosen output exponent `a_exp`.
 *
 * `b_exp` and `c_exp` are the exponents associated with the input mantissa vectors @vector{b} and
 * @vector{c} respectively.
 *
 * `b_hr` and `c_hr` are the headroom of @vector{b} and @vector{c} respectively. If the headroom of
 * @vector{b} or 
 * @vector{c} is unknown, they can be obtained by calling vect_s32_headroom(). Alternatively, the
 * value `0` can always be safely used (but may result in reduced precision).
 * 
 * @par Adjusting Output Exponents
 * @parblock
 * 
 * If a specific output exponent `desired_exp` is needed for the result (e.g. for emulating
 * fixed-point arithmetic), the `b_shr` and `c_shr` produced by this function can be adjusted
 * according to the following:
 * \code{.c}
 *      exponent_t desired_exp = ...; // Value known a priori
 *      right_shift_t new_b_shr = b_shr + (desired_exp - a_exp);
 *      right_shift_t new_c_shr = c_shr + (desired_exp - a_exp);
 * \endcode
 * 
 * When applying the above adjustment, the following conditions should be maintained:
 * * `b_hr + b_shr >= 0`
 * * `c_hr + c_shr >= 0`
 * 
 * Be aware that using smaller values than strictly necessary for `b_shr` and `c_shr` can result in
 * saturation, and using larger values may result in unnecessary underflows or loss of precision.
 * @endparblock
 * 
 * @par Notes
 * @parblock
 * 
 * * Using the outputs of this function, an output mantissa which would otherwise be `INT32_MIN`
 *   will instead saturate to `-INT32_MAX`. This is due to the symmetric saturation logic employed
 *   by the VPU and is a hardware feature. This is a corner case which is usually unlikely and
 *   results in 1 LSb of error when it occurs.
 * @endparblock
 * 
 * 
 * @param[out]  a_exp       Exponent of output elements of vect_s32_mul()
 * @param[out]  b_shr       Right-shift to be applied to elements of @vector{b}
 * @param[out]  c_shr       Right-shift to be applied to elemetns of @vector{c}
 * @param[in]   b_exp       Exponent of @vector{b}
 * @param[in]   c_exp       Exponent of @vector{c}
 * @param[in]   b_hr        Headroom of @vector{b}
 * @param[in]   c_hr        Headroom of @vector{c}
 * 
 * @see vect_s32_mul
 * 
 * @ingroup vect_s32_prepare_api
 */
C_API
void vect_s32_mul_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    right_shift_t* c_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr);


/**
 * @brief Obtain the output exponent and shifts required for a call to `vect_s32_scale()`.
 * 
 * The logic for computing the shifts and exponents of `vect_s32_scale()` is identical to that for
 * `vect_s32_mul()`.
 * 
 * This macro is provided as a convenience to developers and to make the code more readable.
 * 
 * @see vect_s32_mul_prepare()
 * 
 * @ingroup vect_s32_prepare_api
 */
#define vect_s32_scale_prepare vect_s32_mul_prepare


/**
 * @brief Obtain the output exponent and shift parameter used by vect_s32_sqrt().
 * 
 * This function is used in conjunction withx vect_s32_sqrt() to compute the square root of elements
 * of a 32-bit BFP vector.
 *
 * This function computes `a_exp` and `b_shr`.
 *
 * `a_exp` is the exponent associated with output mantissa vector @vector{a}, and should be chosen
 * to maximize the precision of the results. To that end, this function chooses `a_exp` to be the
 * smallest exponent known to avoid saturation of the resulting mantissa vector @vector{a}. It is
 * derived from the exponent and headroom of the input BFP vector.
 *
 * `b_shr` is the shift parameter required by vect_s32_sqrt() to achieve the chosen output exponent
 * `a_exp`.
 *
 * `b_exp` is the exponent associated with the input mantissa vector @vector{b}.
 *
 * `b_hr` is the headroom of @vector{b}. If it is unknown, it can be obtained using
 * vect_s32_headroom(). Alternatively, the value `0` can always be safely used (but may result in
 * reduced precision).
 * 
 * @par Adjusting Output Exponents
 * @parblock
 * 
 * If a specific output exponent `desired_exp` is needed for the result (e.g. for emulating
 * fixed-point arithmetic), the `b_shr` produced by this function can be adjusted according to the
 * following:
 * \code{.c}
 *      exponent_t a_exp;
 *      right_shift_t b_shr;
 *      vect_s16_mul_prepare(&a_exp, &b_shr, b_exp, c_exp, b_hr, c_hr);
 *      exponent_t desired_exp = ...; // Value known a priori
 *      b_shr = b_shr + (desired_exp - a_exp);
 *      a_exp = desired_exp;
 * \endcode
 * 
 * When applying the above adjustment, the following condition should be maintained:
 * * `b_hr + b_shr >= 0`
 * 
 * Be aware that using smaller values than strictly necessary for `b_shr` can result in saturation,
 * and using larger values may result in unnecessary underflows or loss of precision.
 *
 * Also, if a larger exponent is used than necessary, a larger `depth` parameter (see
 * vect_s32_sqrt()) will be required to achieve the same precision, as the results are computed bit
 * by bit, starting with the most significant bit.
 * @endparblock
 * 
 * @param[out]  a_exp       Exponent of outputs of vect_s32_sqrt()
 * @param[out]  b_shr       Right-shift to be applied to elements of @vector{b}
 * @param[in]   b_exp       Exponent of vector{b}
 * @param[in]   b_hr        Headroom of vector{b}
 * 
 * @see vect_s32_sqrt
 * 
 * @ingroup vect_s32_prepare_api
 */
C_API
void vect_s32_sqrt_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    const exponent_t b_exp,
    const right_shift_t b_hr);


/**
 * @brief Obtain the output exponent and shifts required for a call to `vect_s32_sub()`.
 * 
 * The logic for computing the shifts and exponents of `vect_s32_sub()` is identical to that for
 * `vect_s32_add()`.
 * 
 * This macro is provided as a convenience to developers and to make the code more readable.
 * 
 * @see vect_s32_add_prepare()
 * 
 * @ingroup vect_s32_prepare_api
 */
#define vect_s32_sub_prepare vect_s32_add_prepare


/**
 * @brief Obtain the output exponent and input shifts required to perform a
 *        binary add-like operation.
 * 
 * This function computes the output exponent and input shifts required for 
 * BFP operations which take two vectors as input, where the operation is
 * "add-like".  
 * 
 * Here, "add-like" operations are loosely defined as those which require input
 * vectors to share an exponent before their mantissas can be meaningfully used
 * to perform that operation. 
 * 
 * For example, consider adding @math{ 3 \cdot 2^{x} + 4 \cdot 2^{y} }. If
 * @math{x = y}, then the mantissas can be added directly to get a meaningful 
 * result @math{ (3+4) \cdot 2^{x} }. If @math{x \ne y} however, adding the
 * mantissas together is meaningless.  Before the mantissas can be added in this
 * case, one or both of the input mantissas must be shifted so that the
 * representations correspond to the same exponent.  Likewise, similar logic
 * applies to binary comparisons.
 * 
 * This is in contrast to a "multiply-like" operation, which does not have this
 * same requirement (e.g. 
 * @math{a \cdot 2^x \cdot b \cdot 2^y = ab \cdot 2^{x+y}}, regardless of 
 * whether @math{x=y}).
 * 
 * For a general operation like:
 * 
 * @math{ 
 *      \bar{a} \cdot 2^{a\_exp} = \bar{b}\cdot 2^{b\_exp} 
 *                          \oplus \bar{c}\cdot 2^{c\_exp} 
 * }
 * 
 * @vector{b} and @vector{c} are the input mantissa vectors with exponents
 * @math{b\_exp} and @math{c\_exp}, which are shared by each element of their
 * respective vectors. @vector{a} is the output mantissa vector with exponent 
 * @math{a\_exp}. Two additional properties, @math{b\_hr} and @math{c\_hr},
 * which are the headroom of mantissa vectors @vector{b} and @vector{c}
 * respectively, are required by this function.
 * 
 * In addition to @math{a\_exp}, this function computes @math{b\_shr} and 
 * @math{c\_shr}, signed arithmetic right-shifts applied to the mantissa vectors
 * @vector{b} and @vector{c} so that the add-like @math{\oplus} operation can
 * be applied.
 * 
 * This function chooses @math{a\_exp} to be the minimum exponent which can be
 * used to express both @vector{B} and @vector{C} without saturation of their
 * mantissas, and which leaves both @vector{b} and @vector{c} with at least 
 * `extra_operand_hr` bits of headroom. The shifts @math{b\_shr} and 
 * @math{c\_shr} are derived from @math{a\_exp} using @math{b\_exp} and 
 * @math{c\_exp}.
 * 
 * @par Adjusting Output Exponents
 * @parblock
 * 
 * If a specific output exponent `desired_exp` is needed for the result (e.g.
 * for emulating fixed-point arithmetic), the `b_shr` and `c_shr` produced by
 * this function can be adjusted according to the following:
 * \code{.c}
 *      exponent_t desired_exp = ...; // Value known a priori
 *      right_shift_t new_b_shr = b_shr + (desired_exp - a_exp);
 *      right_shift_t new_c_shr = c_shr + (desired_exp - a_exp);
 * \endcode
 * 
 * When applying the above adjustment, the following conditions should be
 * maintained:
 * * `b_hr + b_shr >= 0`
 * * `c_hr + c_shr >= 0`
 * 
 * Be aware that using smaller values than strictly necessary for `b_shr` and
 * `c_shr` can result in saturation, and using larger values may result in
 * unnecessary underflows or loss of precision.
 * @endparblock
 * 
 * @par Notes
 * @parblock
 * 
 * * If @math{b\_hr} or @math{c\_hr} are unknown, they can be calculated using
 *   the appropriate headroom function (e.g. vect_complex_s16_headroom() for
 *   complex 16-bit vectors) or the value `0` can always be safely used (but may
 *   result in reduced precision).
 * @endparblock
 *
 * @param[out] a_exp    Output exponent associated with output mantissa vector @vector{a}
 * @param[out] b_shr    Signed arithmetic right-shift to be applied to elements of @vector{b}. Used
 *                      by the function which computes the output mantissas @vector{a}
 * @param[out] c_shr    Signed arithmetic right-shift to be applied to elements of @vector{c}. Used
 *                      by the function which computes the output mantissas @vector{a}
 * @param[in]  b_exp    Exponent of BFP vector @vector{b}
 * @param[in]  c_exp    Exponent of BFP vector @vector{c}
 * @param[in]  b_hr     Headroom of BFP vector @vector{b} 
 * @param[in]  c_hr     Headroom of BFP vector @vector{c}
 * @param[in]  extra_operand_hr  The minimum amount of headroom that will be left in the mantissa
 *                      vectors following the arithmetic right-shift, as required by some
 *                      operations.
 * 
 * @ingroup vect_s32_prepare_api
 */
C_API
void vect_2vec_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    right_shift_t* c_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr,
    const headroom_t extra_operand_hr);


#ifdef __XC__
}   //extern "C"
#endif

