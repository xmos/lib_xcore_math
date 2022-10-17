// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include "xmath/xmath_conf.h"
#include "xmath/types.h"
#include "xmath/util.h"

/**
 * @defgroup vect_complex_s16_prepare_api   Complex 16-bit Vector Prepare Functions
 */

#ifdef __XC__
extern "C" {
#endif



/**
 * @brief Obtain the output exponent and shifts required for a call to `vect_complex_s16_add()`.
 *
 * The logic for computing the shifts and exponents of `vect_complex_s16_add()` is identical to that
 * for `vect_s32_add()`.
 *
 * This macro is provided as a convenience to developers and to make the code more readable.
 * 
 * @see vect_s32_add_prepare()
 * 
 * @ingroup vect_complex_s16_prepare_api
 */
#define vect_complex_s16_add_prepare vect_s32_add_prepare


/**
 * @brief Obtain the output exponent and shifts required for a call to
 * `vect_complex_s16_add_scalar()`.
 *
 * The logic for computing the shifts and exponents of `vect_complex_s16_add_scalar()` is identical
 * to that for `vect_s32_add()`.
 *
 * This macro is provided as a convenience to developers and to make the code more readable.
 * 
 * @see vect_s16_add_prepare()
 * 
 * @ingroup vect_complex_s16_prepare_api
 */
#define vect_complex_s16_add_scalar_prepare vect_s32_add_prepare


/**
 * @brief Obtain the output exponent and shifts required for a call to
 * `vect_complex_s16_conj_mul()`.
 *
 * The logic for computing the shifts and exponents of `vect_complex_s16_conj_mul()` is identical to
 * that for `vect_complex_s16_mul()`.
 *
 * This macro is provided as a convenience to developers and to make the code more readable.
 * 
 * @see vect_complex_s16_mul_prepare()
 * 
 * @ingroup vect_complex_s16_prepare_api
 */
#define vect_complex_s16_conj_mul_prepare vect_complex_s16_mul_prepare


/**
 * @brief Obtain the output exponent and shifts needed by vect_complex_s16_macc().
 *
 * This function is used in conjunction with vect_complex_s16_macc() to perform an element-wise
 * multiply-accumlate of complex 16-bit BFP vectors.
 *
 * This function computes `new_acc_exp` and `acc_shr` and `bc_sat`, which are selected to maximize
 * precision in the resulting accumulator vector without causing saturation of final or intermediate
 * values. Normally the caller will pass these outputs to their corresponding inputs of
 * vect_complex_s16_macc().
 *
 * `acc_exp` is the exponent associated with the accumulator mantissa vector @vector{a} prior to the
 * operation, whereas `new_acc_exp` is the exponent corresponding to the updated accumulator vector.
 *
 * `b_exp` and `c_exp` are the exponents associated with the complex input mantissa vectors
 * @vector{b} and @vector{c} respectively.
 *
 *
 * `acc_hr`, `b_hr` and `c_hr` are the headrooms of @vector{a}, @vector{b} and @vector{c}
 * respectively. If the headroom of any of these vectors is unknown, it can be obtained by calling
 * vect_complex_s16_headroom(). Alternatively, the value `0` can always be safely used (but may
 * result in reduced precision).
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
 *      right_shift_t acc_shr, bc_sat;
 *      vect_complex_s16_macc_prepare(&acc_exp, &acc_shr, &bc_sat, 
 *                                        acc_exp, b_exp, c_exp,
 *                                        acc_hr, b_hr, c_hr);
 * 
 *      // Modify results
 *      right_shift_t mant_shr = desired_exp - acc_exp;
 *      acc_exp += mant_shr;
 *      acc_shr += mant_shr;
 *      bc_sat  += mant_shr;
 *      
 *      // acc_shr and bc_sat may now be used in a call to vect_complex_s16_macc() 
 * @endcode
 *
 * When applying the above adjustment, the following conditions should be maintained:
 * * `bc_sat >= 0`  (`bc_sat` is an _unsigned_ right-shift)
 * * `acc_shr > -acc_hr`  (Shifting any further left may cause saturation)
 *
 * It is up to the user to ensure any such modification does not result in saturation or
 * unacceptable loss of precision.
 * @endparblock
 *
 *
 * @param[out]  new_acc_exp   Exponent associated with output mantissa vector @vector{a} (after macc)
 * @param[out]  acc_shr       Signed arithmetic right-shift used for @vector{a} in 
 *                            vect_complex_s16_macc()
 * @param[out]  bc_sat        Unsigned arithmetic right-shift applied to the product of elements
 *                            @math{b_k} and @math{c_k} in vect_complex_s16_macc()
 * @param[in]   acc_exp       Exponent associated with input mantissa vector @vector{a} (before macc)
 * @param[in]   b_exp         Exponent associated with input mantissa vector @vector{b}
 * @param[in]   c_exp         Exponent associated with input mantissa vector @vector{c}
 * @param[in]   acc_hr        Headroom of input mantissa vector @vector{a} (before macc)
 * @param[in]   b_hr          Headroom of input mantissa vector @vector{b}
 * @param[in]   c_hr          Headroom of input mantissa vector @vector{c}
 *
 * @see vect_complex_s16_macc
 *
 * @ingroup vect_complex_s16_prepare_api
 */
C_API
void vect_complex_s16_macc_prepare(
    exponent_t* new_acc_exp,
    right_shift_t* acc_shr,
    right_shift_t* bc_sat,
    const exponent_t acc_exp,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t acc_hr,
    const headroom_t b_hr,
    const headroom_t c_hr);


/**
 * @brief Obtain the output exponent and shifts required for a call to vect_complex_s16_nmacc().
 *
 * The logic for computing the shifts and exponents of `vect_complex_s16_nmacc()` is identical
 * to that for `vect_complex_s16_macc()`.
 *
 * This macro is provided as a convenience to developers and to make the code more readable.
 *
 * @see vect_complex_s16_macc_prepare(), vect_complex_s16_nmacc()
 *
 * @ingroup vect_complex_s16_prepare_api
 */
#define vect_complex_s16_nmacc_prepare vect_complex_s16_macc_prepare


/**
 * @brief Obtain the output exponent and shifts required for a call to vect_complex_s16_conj_macc().
 *
 * The logic for computing the shifts and exponents of `vect_complex_s16_conj_macc()` is identical
 * to that for `vect_complex_s16_macc()`.
 *
 * This macro is provided as a convenience to developers and to make the code more readable.
 *
 * @see vect_complex_s16_macc_prepare(), vect_complex_s16_conj_macc()
 *
 * @ingroup vect_complex_s16_prepare_api
 */
#define vect_complex_s16_conj_macc_prepare vect_complex_s16_macc_prepare


/**
 * @brief Obtain the output exponent and shifts required for a call to
 * vect_complex_s16_conj_nmacc().
 *
 * The logic for computing the shifts and exponents of `vect_complex_s16_conj_nmacc()` is
 * identical to that for `vect_complex_s16_macc()`.
 *
 * This macro is provided as a convenience to developers and to make the code more readable.
 *
 * @see vect_complex_s16_macc_prepare(), vect_complex_s16_conj_nmacc()
 *
 * @ingroup vect_complex_s16_prepare_api
 */
#define vect_complex_s16_conj_nmacc_prepare vect_complex_s16_macc_prepare

/**
 * @brief Obtain the output exponent and shifts required for a call to `vect_complex_s16_mag()`.
 *
 * The logic for computing the shifts and exponents of `vect_complex_s16_mag()` is identical to that
 * for `vect_complex_s32_mag()`.
 *
 * This macro is provided as a convenience to developers and to make the code more readable.
 * 
 * @see vect_complex_s32_mag_prepare()
 * 
 * @ingroup vect_complex_s16_prepare_api
 */
#define vect_complex_s16_mag_prepare vect_complex_s32_mag_prepare


/**
 * @brief Obtain the output exponent and output shift used by vect_complex_s16_mul() and
 * vect_complex_s16_conj_mul().
 *
 * This function is used in conjunction with vect_complex_s16_mul() to perform a complex
 * element-wise multiplication of two complex 16-bit BFP vectors.
 *
 * This function computes `a_exp` and `a_shr`.
 *
 * `a_exp` is the exponent associated with mantissa vector @vector{a}, and must be chosen to be
 * large enough to avoid overflow when elements of @vector{a} are computed. To maximize precision,
 * this function chooses `a_exp` to be the smallest exponent known to avoid saturation (see
 * exception below). The `a_exp` chosen by this function is derived from the exponents and headrooms
 * of associated with the input vectors.
 *
 * `a_shr` is the shift parameter required by vect_complex_s16_mul() to achieve the chosen output
 * exponent `a_exp`.
 *
 * `b_exp` and `c_exp` are the exponents associated with the input mantissa vectors @vector{b} and
 * @vector{c} respectively.
 *
 * `b_hr` and `c_hr` are the headroom of @vector{b} and @vector{c} respectively. If the headroom of
 * @vector{b} or @vector{c} is unknown, they can be obtained by calling vect_complex_s16_headroom().
 * Alternatively, the value `0` can always be safely used (but may result in reduced precision).
 * 
 * @par Adjusting Output Exponents
 * @parblock
 * 
 * If a specific output exponent `desired_exp` is needed for the result (e.g. for emulating
 * fixed-point arithmetic), the `a_shr` and `c_shr` produced by this function can be adjusted
 * according to the following:
 * \code{.c}
 *      exponent_t desired_exp = ...; // Value known a priori
 *      right_shift_t new_a_shr = a_shr + (desired_exp - a_exp);
 * \endcode
 * 
 * When applying the above adjustment, the following conditions should be maintained:
 * * `new_a_shr >= 0`
 *
 * Be aware that using smaller values than strictly necessary for `a_shr` can result in saturation,
 * and using larger values may result in unnecessary underflows or loss of precision.
 * @endparblock
 * 
 * @par Notes
 * @parblock
 * 
 * * Using the outputs of this function, an output mantissa which would otherwise be `INT16_MIN`
 *   will instead saturate to `-INT16_MAX`. This is due to the symmetric saturation logic employed
 *   by the VPU and is a hardware feature. This is a corner case which is usually unlikely and
 *   results in 1 LSb of error when it occurs.
 * @endparblock
 * 
 * @param[out]  a_exp               Exponent associated with output mantissa vector @vector{a}
 * @param[out]  a_shr               Unsigned arithmetic right-shift for @vector{b} used by vect_complex_s16_mul()
 * @param[in]   b_exp               Exponent associated with input mantissa vector @vector{b}
 * @param[in]   c_exp               Exponent associated with input mantissa vector @vector{c}
 * @param[in]   b_hr                Headroom of input mantissa vector @vector{b}
 * @param[in]   c_hr                Headroom of input mantissa vector @vector{c}
 * 
 * @see vect_complex_s16_conj_mul,
 *      vect_complex_s16_mul
 * 
 * @ingroup vect_complex_s16_prepare_api
 */
C_API
void vect_complex_s16_mul_prepare(
    exponent_t* a_exp,
    right_shift_t* a_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr);


/**
 * @brief Obtain the output exponent and output shift used by vect_complex_s16_real_mul().
 *
 * This function is used in conjunction with vect_complex_s16_real_mul() to perform a complex
 * element-wise multiplication of a complex 16-bit BFP vector by a real 16-bit vector.
 *
 * This function computes `a_exp` and `a_shr`.
 *
 * `a_exp` is the exponent associated with mantissa vector @vector{a}, and must be chosen to be
 * large enough to avoid overflow when elements of @vector{a} are computed. To maximize precision,
 * this function chooses `a_exp` to be the smallest exponent known to avoid saturation (see
 * exception below). The `a_exp` chosen by this function is derived from the exponents and headrooms
 * of associated with the input vectors.
 *
 * `a_shr` is the shift parameter required by vect_complex_s16_real_mul() to achieve the chosen
 * output exponent `a_exp`.
 *
 * `b_exp` and `c_exp` are the exponents associated with the input mantissa vectors @vector{b} and
 * @vector{c} respectively.
 *
 * `b_hr` and `c_hr` are the headroom of @vector{b} and @vector{c} respectively. If the headroom of
 * @vector{b} or @vector{c} is unknown, they can be obtained by calling vect_complex_s16_headroom().
 * Alternatively, the value `0` can always be safely used (but may result in reduced precision).
 * 
 * @par Adjusting Output Exponents
 * @parblock
 * 
 * If a specific output exponent `desired_exp` is needed for the result (e.g. for emulating
 * fixed-point arithmetic), the `a_shr` and `c_shr` produced by this function can be adjusted
 * according to the following:
 * \code{.c}
 *      exponent_t desired_exp = ...; // Value known a priori
 *      right_shift_t new_a_shr = a_shr + (desired_exp - a_exp);
 * \endcode
 * 
 * When applying the above adjustment, the following conditions should be maintained:
 * * `new_a_shr >= 0`
 *
 * Be aware that using smaller values than strictly necessary for `a_shr` can result in saturation,
 * and using larger values may result in unnecessary underflows or loss of precision.
 * @endparblock
 * 
 * @par Notes
 * @parblock
 * 
 * * Using the outputs of this function, an output mantissa which would otherwise be `INT16_MIN`
 *   will instead saturate to `-INT16_MAX`. This is due to the symmetric saturation logic employed
 *   by the VPU and is a hardware feature. This is a corner case which is usually unlikely and
 *   results in 1 LSb of error when it occurs.
 * @endparblock
 * 
 * @param[out]  a_exp               Exponent associated with output mantissa vector @vector{a}
 * @param[out]  a_shr               Unsigned arithmetic right-shift for @vector{a} used by 
 *                                  vect_complex_s16_real_mul()
 * @param[in]   b_exp               Exponent associated with input mantissa vector @vector{b}
 * @param[in]   c_exp               Exponent associated with input mantissa vector @vector{c}
 * @param[in]   b_hr                Headroom of input mantissa vector @vector{b}
 * @param[in]   c_hr                Headroom of input mantissa vector @vector{c}
 * 
 * @see vect_complex_s16_real_mul
 * 
 * @ingroup vect_complex_s16_prepare_api
 */
C_API
void vect_complex_s16_real_mul_prepare(
    exponent_t* a_exp,
    right_shift_t* a_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr);


/**
 * @brief Obtain the output exponent and shifts required for a call to
 * vect_complex_s16_real_scale().
 *
 * The logic for computing the shifts and exponents of `vect_complex_s16_real_scale()` is identical
 * to that for `vect_s32_scale()`.
 *
 * This macro is provided as a convenience to developers and to make the code more readable.
 *
 * @see vect_s16_scale_prepare()
 *
 * @ingroup vect_complex_s16_prepare_api
 */
#define vect_complex_s16_real_scale_prepare vect_s16_scale_prepare


/**
 * @brief Obtain the output exponent and shifts required for a call to `vect_complex_s16_scale()`.
 *
 * The logic for computing the shifts and exponents of `vect_complex_s16_scale()` is identical to
 * that for `vect_complex_s32_mul()`.
 *
 * This macro is provided as a convenience to developers and to make the code more readable.
 *
 * @see vect_complex_s16_mul_prepare()
 * 
 * @ingroup vect_complex_s16_prepare_api
 */
#define vect_complex_s16_scale_prepare vect_complex_s16_mul_prepare


/**
 * @brief Obtain the output exponent and input shift used by vect_complex_s16_squared_mag().
 *
 * This function is used in conjunction with vect_complex_s16_squared_mag() to compute the squared
 * magnitude of each element of a complex 16-bit BFP vector.
 *
 * This function computes `a_exp` and `a_shr`.
 *
 * `a_exp` is the exponent associated with mantissa vector @vector{a}, and is be chosen to maximize
 * precision when elements of @vector{a} are computed. The `a_exp` chosen by this function is
 * derived from the exponent and headroom associated with the input vector.
 *
 * `a_shr` is the shift parameter required by vect_complex_s16_mag() to achieve the chosen output
 * exponent `a_exp`.
 *
 * `b_exp` is the exponent associated with the input mantissa vector @vector{b}.
 *
 * `b_hr` is the headroom of @vector{b}. If the headroom of @vector{b} is unknown it can be
 * calculated using vect_complex_s16_headroom(). Alternatively, the value `0` can always be safely
 * used (but may result in reduced precision).
 *
 * @par Adjusting Output Exponents
 * @parblock
 * 
 * If a specific output exponent `desired_exp` is needed for the result (e.g. for emulating
 * fixed-point arithmetic), the `a_shr` produced by this function can be adjusted according to the
 * following:
 * \code{.c}
 *      exponent_t a_exp;
 *      right_shift_t a_shr;
 *      vect_s16_mul_prepare(&a_exp, &a_shr, b_exp, c_exp, b_hr, c_hr);
 *      exponent_t desired_exp = ...; // Value known a priori
 *      a_shr = a_shr + (desired_exp - a_exp);
 *      a_exp = desired_exp;
 * \endcode
 * 
 * When applying the above adjustment, the following condition should be maintained:
 * * `a_shr >= 0`
 *
 * Using larger values than strictly necessary for `a_shr` may result in unnecessary underflows or
 * loss of precision.
 * @endparblock
 * 
 * @param[out]  a_exp               Output exponent associated with output mantissa vector @vector{a}
 * @param[out]  a_shr               Unsigned arithmetic right-shift for @vector{a} used by 
 *                                  vect_complex_s16_squared_mag()
 * @param[in]   b_exp               Exponent associated with input mantissa vector @vector{b}
 * @param[in]   b_hr                Headroom of input mantissa vector @vector{b}
 * 
 * @see vect_complex_s16_squared_mag()
 * 
 * @ingroup vect_complex_s16_prepare_api
 */
C_API
void vect_complex_s16_squared_mag_prepare(
    exponent_t* a_exp,
    right_shift_t* a_shr,
    const exponent_t b_exp,
    const headroom_t b_hr);


/**
 * @brief Obtain the output exponent and shifts required for a call to `vect_complex_s16_sub()`.
 *
 * The logic for computing the shifts and exponents of `vect_complex_s16_sub()` is identical to that
 * for `vect_s32_add()`.
 *
 * This macro is provided as a convenience to developers and to make the code more readable.
 *
 * @see vect_s32_add_prepare()
 *
 * @ingroup vect_complex_s16_prepare_api
 */
#define vect_complex_s16_sub_prepare vect_s32_add_prepare


#ifdef __XC__
}   //extern "C"
#endif

