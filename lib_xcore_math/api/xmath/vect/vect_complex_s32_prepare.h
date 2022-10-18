// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include "xmath/xmath_conf.h"
#include "xmath/types.h"
#include "xmath/util.h"

/**
 * @defgroup vect_complex_s32_prepare_api   Complex 32-bit Vector Prepare Functions
 */

#ifdef __XC__
extern "C" {
#endif


/**
 * @brief Obtain the output exponent and shifts required for a call to `vect_complex_s32_add()`.
 *
 * The logic for computing the shifts and exponents of `vect_complex_s32_add()` is identical to that
 * for `vect_s32_add()`.
 *
 * This macro is provided as a convenience to developers and to make the code more coherent.
 * 
 * @see vect_s32_add_prepare()
 * 
 * @ingroup vect_complex_s32_prepare_api
 */
#define vect_complex_s32_add_prepare vect_s32_add_prepare


/**
 * @brief Obtain the output exponent and shifts required for a call to
 * `vect_complex_s32_add_scalar()`.
 *
 * The logic for computing the shifts and exponents of `vect_complex_s32_add_scalar()` is identical
 * to that for `vect_s32_add()`.
 *
 * This macro is provided as a convenience to developers and to make the code more readable.
 * 
 * @see vect_s32_add_prepare()
 * 
 * @ingroup vect_complex_s32_prepare_api
 */
#define vect_complex_s32_add_scalar_prepare vect_s32_add_prepare


/**
 * @brief Obtain the output exponent and shifts required for a call to
 * `vect_complex_s32_conj_mul()`.
 *
 * The logic for computing the shifts and exponents of `vect_complex_s32_conj_mul()` is identical to
 * that for `vect_complex_s32_mul()`.
 *
 * This macro is provided as a convenience to developers and to make the code more readable.
 * 
 * @see vect_complex_s32_mul_prepare()
 * 
 * @ingroup vect_complex_s32_prepare_api
 */
#define vect_complex_s32_conj_mul_prepare vect_complex_s32_mul_prepare


/**
 * @brief Obtain the output exponent and shifts needed by vect_complex_s32_macc().
 *
 * This function is used in conjunction with vect_complex_s32_macc() to perform an element-wise
 * multiply-accumlate of 32-bit BFP vectors.
 *
 * This function computes `new_acc_exp`, `acc_shr`, `b_shr` and `c_shr`, which are selected to
 * maximize precision in the resulting accumulator vector without causing saturation of final or
 * intermediate values. Normally the caller will pass these outputs to their corresponding inputs of
 * vect_complex_s32_macc().
 *
 * `acc_exp` is the exponent associated with the accumulator mantissa vector @vector{a} prior to the
 * operation, whereas `new_acc_exp` is the exponent corresponding to the updated accumulator vector.
 *
 * `b_exp` and `c_exp` are the exponents associated with the complex input mantissa vectors
 * @vector{b} and @vector{c} respectively.
 *
 * `acc_hr`, `b_hr` and `c_hr` are the headrooms of @vector{a}, @vector{b} and @vector{c}
 * respectively. If the headroom of any of these vectors is unknown, it can be obtained by calling
 * vect_complex_s32_headroom(). Alternatively, the value `0` can always be safely used (but may
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
 *      right_shift_t acc_shr, b_shr, c_shr;
 *      vect_complex_s32_macc_prepare(&acc_exp, &acc_shr, &b_shr, &c_shr, 
 *                                        acc_exp, b_exp, c_exp,
 *                                        acc_hr, b_hr, c_hr);
 * 
 *      // Modify results
 *      right_shift_t mant_shr = desired_exp - acc_exp;
 *      acc_exp += mant_shr;
 *      acc_shr += mant_shr;
 *      b_shr  += mant_shr;
 *      c_shr  += mant_shr;
 *      
 *      // acc_shr, b_shr and c_shr may now be used in a call to vect_complex_s32_macc() 
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
 * @param[out]  new_acc_exp   Exponent associated with output mantissa vector @vector{a} (after macc)
 * @param[out]  acc_shr       Signed arithmetic right-shift used for @vector{a} in  
 *                            vect_complex_s32_macc()
 * @param[out]  b_shr         Signed arithmetic right-shift used for @vector{b} in  
 *                            vect_complex_s32_macc()
 * @param[out]  c_shr         Signed arithmetic right-shift used for @vector{c} in  
 *                            vect_complex_s32_macc()
 * @param[in]   acc_exp       Exponent associated with input mantissa vector @vector{a} (before macc)
 * @param[in]   b_exp         Exponent associated with input mantissa vector @vector{b}
 * @param[in]   c_exp         Exponent associated with input mantissa vector @vector{c}
 * @param[in]   acc_hr        Headroom of input mantissa vector @vector{a} (before macc)
 * @param[in]   b_hr          Headroom of input mantissa vector @vector{b}
 * @param[in]   c_hr          Headroom of input mantissa vector @vector{c}
 *
 * @see vect_complex_s32_macc
 *
 * @ingroup vect_complex_s32_prepare_api
 */
C_API
void vect_complex_s32_macc_prepare(
    exponent_t* new_acc_exp,
    right_shift_t* acc_shr,
    right_shift_t* b_shr,
    right_shift_t* c_shr,
    const exponent_t acc_exp,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const exponent_t acc_hr,
    const headroom_t b_hr,
    const headroom_t c_hr);


/**
 * @brief Obtain the output exponent and shifts required for a call to vect_complex_s32_nmacc().
 *
 * The logic for computing the shifts and exponents of `vect_complex_s32_nmacc()` is identical
 * to that for `vect_complex_s32_macc_prepare()`.
 *
 * This macro is provided as a convenience to developers and to make the code more readable.
 *
 * @see vect_complex_s32_macc_prepare(), vect_complex_s32_nmacc()
 *
 * @ingroup vect_complex_s32_prepare_api
 */
#define vect_complex_s32_nmacc_prepare vect_complex_s32_macc_prepare


/**
 * @brief Obtain the output exponent and shifts required for a call to 
 * vect_complex_s32_conj_macc().
 *
 * The logic for computing the shifts and exponents of `vect_complex_s32_conj_macc()` is 
 * identical to that for `vect_complex_s32_macc_prepare()`.
 *
 * This macro is provided as a convenience to developers and to make the code more readable.
 *
 * @see vect_complex_s32_macc_prepare(), vect_complex_s32_conj_macc()
 *
 * @ingroup vect_complex_s32_prepare_api
 */
#define vect_complex_s32_conj_macc_prepare vect_complex_s32_macc_prepare


/**
 * @brief Obtain the output exponent and shifts required for a call to 
 * vect_complex_s32_conj_nmacc().
 *
 * The logic for computing the shifts and exponents of `vect_complex_s32_conj_nmacc()` is 
 * identical to that for `vect_complex_s32_macc_prepare()`.
 *
 * This macro is provided as a convenience to developers and to make the code more readable.
 *
 * @see vect_complex_s32_macc_prepare(), vect_complex_s32_conj_nmacc()
 *
 * @ingroup vect_complex_s32_prepare_api
 */
#define vect_complex_s32_conj_nmacc_prepare vect_complex_s32_macc_prepare

/**
 * @brief Obtain the output exponent and input shift used by vect_complex_s32_mag() and
 * vect_complex_s16_mag().
 *
 * This function is used in conjunction with vect_complex_s32_mag() to compute the magnitude of each
 * element of a complex 32-bit BFP vector.
 *
 * This function computes `a_exp` and `b_shr`.
 *
 * `a_exp` is the exponent associated with mantissa vector @vector{a}, and is be chosen to maximize
 * precision when elements of @vector{a} are computed. The `a_exp` chosen by this function is
 * derived from the exponent and headroom associated with the input vector.
 *
 * `b_shr` is the shift parameter required by vect_complex_s32_mag() to achieve the chosen output
 * exponent `a_exp`.
 *
 * `b_exp` is the exponent associated with the input mantissa vector @vector{b}.
 *
 * `b_hr` is the headroom of @vector{b}. If the headroom of @vector{b} is unknown it can be
 * calculated using vect_complex_s32_headroom(). Alternatively, the value `0` can always be safely
 * used (but may result in reduced precision).
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
 * Using larger values than strictly necessary for `b_shr` may result in unnecessary underflows or
 * loss of precision.
 * @endparblock
 * 
 * @param[out]  a_exp               Output exponent associated with output mantissa vector @vector{a}
 * @param[out]  b_shr               Signed arithmetic right-shift for @vector{b} used by vect_complex_s32_mag()
 * @param[in]   b_exp               Exponent associated with input mantissa vector @vector{b}
 * @param[in]   b_hr                Headroom of input mantissa vector @vector{b}
 * 
 * @see vect_complex_s32_mag()
 * 
 * @ingroup vect_complex_s32_prepare_api
 */
C_API
void vect_complex_s32_mag_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    const exponent_t b_exp,
    const headroom_t b_hr);


/**
 * @brief Obtain the output exponent and input shifts used by vect_complex_s32_mul() and
 * vect_complex_s32_conj_mul().
 *
 * This function is used in conjunction with vect_complex_s32_mul() to perform a complex
 * element-wise multiplication of two complex 32-bit BFP vectors.
 *
 * This function computes `a_exp`, `b_shr` and `c_shr`.
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
 * @vector{b} or @vector{c} is unknown, they can be obtained by calling vect_complex_s32_headroom().
 * Alternatively, the value `0` can always be safely used (but may result in reduced precision).
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
 * @param[out]  a_exp               Exponent associated with output mantissa vector @vector{a}
 * @param[out]  b_shr               Signed arithmetic right-shift for @vector{b} used by vect_complex_s32_mul()
 * @param[out]  c_shr               Signed arithmetic right-shift for @vector{c} used by vect_complex_s32_mul()
 * @param[in]   b_exp               Exponent associated with input mantissa vector @vector{b}
 * @param[in]   c_exp               Exponent associated with input mantissa vector @vector{c}
 * @param[in]   b_hr                Headroom of input mantissa vector @vector{b}
 * @param[in]   c_hr                Headroom of input mantissa vector @vector{c}
 * 
 * @see vect_complex_s32_conj_mul, 
 *      vect_complex_s32_mul
 * 
 * @ingroup vect_complex_s32_prepare_api
 */
C_API
void vect_complex_s32_mul_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    right_shift_t* c_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr);


/**
 * @brief Obtain the output exponent and input shifts used by vect_complex_s32_real_mul().
 * 
 * This function is used in conjunction with vect_complex_s32_real_mul() to perform a the
 * element-wise multiplication of complex 32-bit BFP vector by a real 32-bit BFP vector.
 *
 * This function computes `a_exp`, `b_shr` and `c_shr`.
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
 * @vector{b} or @vector{c} is unknown, they can be obtained by calling vect_complex_s32_headroom().
 * Alternatively, the value `0` can always be safely used (but may result in reduced precision).
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
 * @param[out]  a_exp               Output exponent associated with @vector{a}
 * @param[out]  b_shr               Signed arithmetic right-shift for @vector{b} used by vect_complex_s32_real_mul()
 * @param[out]  c_shr               Signed arithmetic right-shift for @vector{c} used by vect_complex_s32_real_mul()
 * @param[in]   b_exp               Exponent associated with @vector{b}
 * @param[in]   c_exp               Exponent associated with @vector{c}
 * @param[in]   b_hr                Headroom of mantissa vector @vector{b}
 * @param[in]   c_hr                Headroom of mantissa vector @vector{c}
 * 
 * @see vect_complex_s32_real_mul
 * 
 * @ingroup vect_complex_s32_prepare_api
 */
C_API
void vect_complex_s32_real_mul_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    right_shift_t* c_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr);


/**
 * @brief Obtain the output exponent and shifts required for a call to
 * `vect_complex_s32_real_scale()`.
 *
 * The logic for computing the shifts and exponents of `vect_complex_s32_real_scale()` is identical
 * to that for `vect_s32_mul()`.
 *
 * This macro is provided as a convenience to developers and to make the code more readable.
 * 
 * @see vect_s32_mul_prepare()
 * 
 * @ingroup vect_complex_s32_prepare_api
 */
#define vect_complex_s32_real_scale_prepare vect_s32_mul_prepare


/**
 * @brief Obtain the output exponent and input shifts used by vect_complex_s32_scale().
 * 
 * This function is used in conjunction with vect_complex_s32_scale() to perform a complex
 * multiplication of a complex 32-bit BFP vector by a complex 32-bit scalar.
 *
 * This function computes `a_exp`, `b_shr` and `c_shr`.
 *
 * `a_exp` is the exponent associated with mantissa vector @vector{a}, and must be chosen to be
 * large enough to avoid overflow when elements of @vector{a} are computed. To maximize precision,
 * this function chooses `a_exp` to be the smallest exponent known to avoid saturation (see
 * exception below). The `a_exp` chosen by this function is derived from the exponents and headrooms
 * associated with the input vectors.
 *
 * `b_shr` and `c_shr` are the shift parameters required by vect_complex_s32_mul() to achieve the
 * chosen output exponent `a_exp`.
 *
 * `b_exp` and `c_exp` are the exponents associated with the input mantissa vectors @vector{b} and
 * @vector{c} respectively.
 *
 * `b_hr` and `c_hr` are the headroom of @vector{b} and @vector{c} respectively. If the headroom of
 * @vector{b} or @vector{c} is unknown, they can be obtained by calling vect_complex_s32_headroom().
 * Alternatively, the value `0` can always be safely used (but may result in reduced precision).
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
 * @param[out]  a_exp               Exponent associated with output mantissa vector @vector{a}
 * @param[out]  b_shr               Signed arithmetic right-shift for @vector{b} used by vect_complex_s32_scale()
 * @param[out]  c_shr               Signed arithmetic right-shift for @vector{c} used by vect_complex_s32_scale()
 * @param[in]   b_exp               Exponent associated with input mantissa vector @vector{b}
 * @param[in]   c_exp               Exponent associated with input mantissa vector @vector{c}
 * @param[in]   b_hr                Headroom of input mantissa vector @vector{b}
 * @param[in]   c_hr                Headroom of input mantissa vector @vector{c}
 * 
 * @see vect_complex_s32_scale
 * 
 * @ingroup vect_complex_s32_prepare_api
 */
C_API
void vect_complex_s32_scale_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    right_shift_t* c_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr);


/**
 * @brief Obtain the output exponent and input shift used by vect_complex_s32_squared_mag().
 * 
 * This function is used in conjunction with vect_complex_s32_squared_mag() to compute the squared
 * magnitude of each element of a complex 32-bit BFP vector.
 *
 * This function computes `a_exp` and `b_shr`.
 *
 * `a_exp` is the exponent associated with mantissa vector @vector{a}, and is be chosen to maximize
 * precision when elements of @vector{a} are computed. The `a_exp` chosen by this function is
 * derived from the exponent and headroom associated with the input vector.
 *
 * `b_shr` is the shift parameter required by vect_complex_s32_mag() to achieve the chosen output
 * exponent `a_exp`.
 *
 * `b_exp` is the exponent associated with the input mantissa vector @vector{b}.
 *
 * `b_hr` is the headroom of @vector{b}. If the headroom of @vector{b} is unknown it can be
 * calculated using vect_complex_s32_headroom(). Alternatively, the value `0` can always be safely
 * used (but may result in reduced precision).
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
 * Using larger values than strictly necessary for `b_shr` may result in unnecessary underflows or
 * loss of precision.
 * @endparblock
 * 
 * @param[out]  a_exp               Output exponent associated with output mantissa vector @vector{a}
 * @param[out]  b_shr               Signed arithmetic right-shift for @vector{b} used by vect_complex_s32_squared_mag()
 * @param[in]   b_exp               Exponent associated with input mantissa vector @vector{b}
 * @param[in]   b_hr                Headroom of input mantissa vector @vector{b}
 * 
 * @see vect_complex_s32_squared_mag()
 * 
 * @ingroup vect_complex_s32_prepare_api
 */
C_API
void vect_complex_s32_squared_mag_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    const exponent_t b_exp,
    const headroom_t b_hr);


/**
 * @brief Obtain the output exponent and shifts required for a call to `vect_complex_s32_sub()`.
 * 
 * The logic for computing the shifts and exponents of `vect_complex_s32_sub()` is identical to that
 * for `vect_s32_add()`.
 *
 * This macro is provided as a convenience to developers and to make the code more readable.
 * 
 * @see vect_s32_add_prepare()
 * 
 * @ingroup vect_complex_s32_prepare_api
 */
#define vect_complex_s32_sub_prepare vect_s32_add_prepare


/**
 * @brief Obtain the output exponent and input shift used by vect_complex_s32_sum().
 * 
 * This function is used in conjunction with vect_complex_s32_sum() to compute the sum of elements
 * of a complex 32-bit BFP vector.
 *
 * This function computes `a_exp` and `b_shr`.
 *
 * `a_exp` is the exponent associated with the 64-bit mantissa @math{a} returned by
 * vect_complex_s32_sum(), and must be chosen to be large enough to avoid saturation when @math{a}
 * is computed. To maximize precision, this function chooses `a_exp` to be the smallest exponent
 * known to avoid saturation (see exception below). The `a_exp` chosen by this function is derived
 * from the exponents and headrooms associated with the input vector.
 *
 * `b_shr` is the shift parameter required by vect_complex_s32_sum() to achieve the chosen output
 * exponent `a_exp`.
 *
 * `b_exp` is the exponent associated with the input mantissa vector @vector{b}.
 *
 * `b_hr` is the headroom of @vector{b}. If the headroom of @vector{b} is unknown it can be
 * calculated using vect_complex_s32_headroom(). Alternatively, the value `0` can always be safely
 * used (but may result in reduced precision).
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
 * When applying the above adjustment, the following conditions should be maintained:
 * * `b_hr + b_shr >= 0`
 *
 * Be aware that using smaller values than strictly necessary for `b_shr` can result in saturation,
 * and using larger values may result in unnecessary underflows or loss of precision.
 * @endparblock
 * 
 * @param[out]  a_exp                   Exponent associated with output mantissa @math{a}
 * @param[out]  b_shr                   Signed arithmetic right-shift for @vector{b} used by vect_complex_s32_sum()
 * @param[in]   b_exp                   Exponent associated with input mantissa vector @vector{b}
 * @param[in]   b_hr                    Headroom of input mantissa vector @vector{b}
 * @param[in]   length                  Number of elements in @vector{b}
 * 
 * @see vect_complex_s32_sum
 * 
 * @ingroup vect_complex_s32_prepare_api
 */
C_API
void vect_complex_s32_sum_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    const exponent_t b_exp,
    const headroom_t b_hr,
    const unsigned length);


#ifdef __XC__
}   //extern "C"
#endif

