// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include "xmath/xmath_conf.h"
#include "xmath/types.h"
#include "xmath/util.h"

#include "xmath/vect/vect_s16_prepare.h"

/**
 * @defgroup vect_s16_prepare_api   16-bit Vector Prepare Functions
 */

#ifdef __XC__
extern "C" {
#endif


/**
 * @brief Obtain the output exponent and shifts required for a call to `vect_s16_add()`.
 * 
 * The logic for computing the shifts and exponents of `vect_s16_add()` is identical to that for
 * `vect_s32_add()`.
 * 
 * This macro is provided as a convenience to developers and to make the code more readable.
 * 
 * @see vect_s32_add_prepare()
 * 
 * @ingroup vect_s16_prepare_api
 */
#define vect_s16_add_prepare vect_s32_add_prepare


/**
 * @brief Obtain the output exponent and shifts required for a call to `vect_s16_add_scalar()`.
 * 
 * The logic for computing the shifts and exponents of `vect_s16_add_scalar()` is identical to
 * that for `vect_s32_add()`.
 * 
 * This macro is provided as a convenience to developers and to make the code more readable.
 * 
 * @see vect_s32_add_prepare()
 * 
 * @ingroup vect_s16_prepare_api
 */
#define vect_s16_add_scalar_prepare vect_s32_add_prepare


/**
 * @brief Obtain the output exponent, input shift and modified bounds used by vect_s16_clip().
 * 
 * This function is used in conjunction with vect_s16_clip() to bound the elements of a 32-bit BFP
 * vector to a specified range.
 *
 * This function computes `a_exp`, `b_shr`, `lower_bound` and `upper_bound`.
 *
 * `a_exp` is the exponent associated with the 16-bit mantissa vector @vector{a} computed by
 * vect_s32_clip().
 *
 * `b_shr` is the shift parameter required by vect_s16_clip() to achieve the output exponent
 * `a_exp`.
 *
 * `lower_bound` and `upper_bound` are the 16-bit mantissas which indicate the lower and upper
 * clipping bounds respectively. The values are modified by this function, and the resulting values
 * should be passed along to vect_s16_clip().
 *
 * `b_exp` is the exponent associated with the input mantissa vector @vector{b}.
 *
 * `bound_exp` is the exponent associated with the bound mantissas `lower_bound` and `upper_bound`
 * respectively.
 *
 * `b_hr` is the headroom of @vector{b}. If unknown, it can be obtained using vect_s16_headroom().
 * Alternatively, the value `0` can always be safely used (but may result in reduced precision).
 * 
 * @param[out]    a_exp               Exponent associated with output mantissa vector @vector{a}
 * @param[out]    b_shr               Signed arithmetic right-shift for @vector{b} used by vect_s32_clip()
 * @param[inout]  lower_bound         Lower bound of clipping range
 * @param[inout]  upper_bound         Upper bound of clipping range
 * @param[in]     b_exp               Exponent associated with input mantissa vector @vector{b}
 * @param[in]     bound_exp           Exponent associated with clipping bounds `lower_bound` and `upper_bound`
 * @param[in]     b_hr                Headroom of input mantissa vector @vector{b}
 * 
 * @see vect_s16_clip
 * 
 * @ingroup vect_s16_prepare_api
 */
C_API
void vect_s16_clip_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    int16_t* lower_bound,
    int16_t* upper_bound,
    const exponent_t b_exp,
    const exponent_t bound_exp,
    const headroom_t b_hr);


/**
 * @brief Obtain the output exponent and scaling parameter used by vect_s16_inverse().
 * 
 * This function is used in conjunction with vect_s16_inverse() to compute the inverse of elements
 * of a 16-bit BFP vector.
 *
 * This function computes `a_exp` and `scale`.
 *
 * `a_exp` is the exponent associated with output mantissa vector @vector{a}, and must be chosen to
 * avoid overflow in the smallest element of the input vector, which when inverted becomes the
 * largest output element. To maximize precision, this function chooses `a_exp` to be the smallest
 * exponent known to avoid saturation. The `a_exp` chosen by this function is derived from the
 * exponent and smallest element of the input vector.
 *
 * `scale` is a scaling parameter used by vect_s16_inverse() to achieve the chosen output exponent.
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
 * @see vect_s16_inverse
 * 
 * @ingroup vect_s16_prepare_api
 */
C_API
void vect_s16_inverse_prepare(
    exponent_t* a_exp,
    unsigned* scale,
    const int16_t b[],
    const exponent_t b_exp,
    const unsigned length);


/**
 * @brief Obtain the output exponent and shifts needed by vect_s16_macc().
 *
 * This function is used in conjunction with vect_s16_macc() to perform an element-wise
 * multiply-accumlate of 16-bit BFP vectors.
 *
 * This function computes `new_acc_exp` and `acc_shr` and `bc_sat`, which are selected to maximize
 * precision in the resulting accumulator vector without causing saturation of final or intermediate
 * values. Normally the caller will pass these outputs to their corresponding inputs of
 * vect_s16_macc().
 *
 * `acc_exp` is the exponent associated with the accumulator mantissa vector @vector{a} prior to the
 * operation, whereas `new_acc_exp` is the exponent corresponding to the updated accumulator vector.
 *
 * `b_exp` and `c_exp` are the exponents associated with the complex input mantissa vectors
 * @vector{b} and @vector{c} respectively.
 *
 * `acc_hr`, `b_hr` and `c_hr` are the headrooms of @vector{a}, @vector{b} and @vector{c}
 * respectively. If the headroom of any of these vectors is unknown, it can be obtained by calling
 * vect_s16_headroom(). Alternatively, the value `0` can always be safely used (but may result
 * in reduced precision).
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
 *      vect_s16_macc_prepare(&acc_exp, &acc_shr, &bc_sat, 
 *                                acc_exp, b_exp, c_exp,
 *                                acc_hr, b_hr, c_hr);
 * 
 *      // Modify results
 *      right_shift_t mant_shr = desired_exp - acc_exp;
 *      acc_exp += mant_shr;
 *      acc_shr += mant_shr;
 *      bc_sat  += mant_shr;
 *      
 *      // acc_shr and bc_sat may now be used in a call to vect_s16_macc() 
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
 * @param[out]  acc_shr       Signed arithmetic right-shift used for @vector{a} in vect_s16_macc()
 * @param[out]  bc_sat        Unsigned arithmetic right-shift applied to the product of elements
 *                            @math{b_k} and @math{c_k} in vect_s16_macc()
 * @param[in]   acc_exp       Exponent associated with input mantissa vector @vector{a} (before macc)
 * @param[in]   b_exp         Exponent associated with input mantissa vector @vector{b}
 * @param[in]   c_exp         Exponent associated with input mantissa vector @vector{c}
 * @param[in]   acc_hr        Headroom of input mantissa vector @vector{a} (before macc)
 * @param[in]   b_hr          Headroom of input mantissa vector @vector{b}
 * @param[in]   c_hr          Headroom of input mantissa vector @vector{c}
 *
 * @see vect_s16_macc
 *
 * @ingroup vect_s16_prepare_api
 */
C_API
void vect_s16_macc_prepare(
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
 * @brief Obtain the output exponent and shifts required for a call to vect_s16_nmacc().
 *
 * The logic for computing the shifts and exponents of `vect_s16_nmacc()` is identical
 * to that for `vect_s16_macc_prepare()`.
 *
 * This macro is provided as a convenience to developers and to make the code more readable.
 *
 * @see vect_s16_macc_prepare(), vect_s16_nmacc()
 *
 * @ingroup vect_s16_prepare_api
 */
#define vect_s16_nmacc_prepare  vect_s16_macc_prepare


/**
 * @brief Obtain the output exponent and output shift used by vect_s16_mul().
 * 
 * This function is used in conjunction with vect_s16_mul() to perform an element-wise
 * multiplication of two 16-bit BFP vectors.
 *
 * This function computes `a_exp` and `a_shr`.
 *
 * `a_exp` is the exponent associated with mantissa vector @vector{a}, and must be chosen to be
 * large enough to avoid overflow when elements of @vector{a} are computed. To maximize precision,
 * this function chooses `a_exp` to be the smallest exponent known to avoid saturation (see
 * exception below). The `a_exp` chosen by this function is derived from the exponents and headrooms
 * of associated with the input vectors.
 *
 * `a_shr` is an arithmetic right-shift applied by vect_complex_s16_mul() to the 32-bit products of
 * input elements to achieve the chosen output exponent `a_exp`.
 *
 * `b_exp` and `c_exp` are the exponents associated with the input mantissa vectors @vector{b} and
 * @vector{c} respectively.
 *
 * `b_hr` and `c_hr` are the headroom of @vector{b} and @vector{c} respectively. If the headroom of
 * @vector{b} or @vector{c} is unknown, they can be obtained by calling vect_s16_headroom().
 * Alternatively, the value `0` can always be safely used (but may result in reduced precision).
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
 * When applying the above adjustment, the following conditions should be maintained:
 * * `a_shr >= 0`
 * 
 * Be aware that using a smaller value than strictly necessary for `a_shr` can result in saturation,
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
 * @param[out]  a_exp       Exponent of output elements of vect_s16_mul()
 * @param[out]  a_shr       Right-shift supplied to vect_s16_mul()
 * @param[in]   b_exp       Exponent associated with @vector{b}
 * @param[in]   c_exp       Exponent associated with @vector{c}
 * @param[in]   b_hr        Headroom of @vector{b}
 * @param[in]   c_hr        Headroom of @vector{c}
 * 
 * @see vect_s16_mul
 * 
 * @ingroup vect_s16_prepare_api
 */
C_API
void vect_s16_mul_prepare(
    exponent_t* a_exp,
    right_shift_t* a_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr);


/**
 * @brief Obtain the output exponent and output shift used by vect_s16_scale().
 * 
 * This function is used in conjunction with vect_s16_scale() to perform multiplication of a 16-bit
 * BFP vector @math{\bar{b} \cdot 2^{b\_exp}} by a 16-bit scalar @math{c \cdot 2^{c\_exp}}. The
 * result is another 16-bit BFP vector @math{\bar{a} \cdot 2^{a\_exp}}.
 *
 * This function computes `a_exp` and `a_shr`.
 *
 * `a_exp` is the exponent associated with mantissa vector @vector{a}, and must be chosen to be
 * large enough to avoid overflow when elements of @vector{a} are computed. To maximize precision,
 * this function chooses `a_exp` to be the smallest exponent known to avoid saturation (see
 * exception below). The `a_exp` chosen by this function is derived from the exponents and headrooms
 * of associated with the inputs.
 *
 * `a_shr` is an arithmetic right-shift applied by vect_complex_s16_scale() to the 32-bit products
 * of input elements to achieve the chosen output exponent `a_exp`.
 *
 * `b_exp` and `c_exp` are the exponents associated with @vector{b} and @math{c} respectively.
 *
 * `b_hr` and `c_hr` are the headroom of @vector{b} and @math{c} respectively. If the headroom of
 * @vector{b} or @math{c} are unknown, they can be obtained by calling vect_s16_headroom().
 * Alternatively, the value `0` can always be safely used (but may result in reduced precision).
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
 *      vect_s16_scale_prepare(&a_exp, &a_shr, b_exp, c_exp, b_hr, c_hr);
 *      exponent_t desired_exp = ...; // Value known a priori
 *      a_shr = a_shr + (desired_exp - a_exp);
 *      a_exp = desired_exp;
 * \endcode
 * 
 * When applying the above adjustment, the following conditions should be maintained:
 * * `a_shr >= 0`
 * 
 * Be aware that using a smaller value than strictly necessary for `a_shr` can result in saturation,
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
 * @param[out]  a_exp       Exponent of output elements of vect_s16_scale()
 * @param[out]  a_shr       Right-shift supplied to vect_s16_scale()
 * @param[in]   b_exp       Exponent associated with @vector{b}
 * @param[in]   c_exp       Exponent associated with @vector{c}
 * @param[in]   b_hr        Headroom of @vector{b}
 * @param[in]   c_hr        Headroom of @vector{c}
 * 
 * @see vect_s16_scale
 * 
 * @ingroup vect_s16_prepare_api
 */
C_API
void vect_s16_scale_prepare(
    exponent_t* a_exp,
    right_shift_t* a_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr);


/**
 * @brief Obtain the output exponent and shift parameter used by vect_s16_sqrt().
 * 
 * This function is used in conjunction withx vect_s16_sqrt() to compute the square root of elements
 * of a 16-bit BFP vector.
 *
 * This function computes `a_exp` and `b_shr`.
 *
 * `a_exp` is the exponent associated with output mantissa vector @vector{a}, and should be chosen
 * to maximize the precision of the results. To that end, this function chooses `a_exp` to be the
 * smallest exponent known to avoid saturation of the resulting mantissa vector @vector{a}. It is
 * derived from the exponent and headroom of the input BFP vector.
 *
 * `b_shr` is the shift parameter required by vect_s16_sqrt() to achieve the chosen output exponent
 * `a_exp`.
 *
 * `b_exp` is the exponent associated with the input mantissa vector @vector{b}.
 *
 * `b_hr` is the headroom of @vector{b}. If it is unknown, it can be obtained using
 * vect_s16_headroom(). Alternatively, the value `0` can always be safely used (but may result in
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
 * vect_s16_sqrt()) will be required to achieve the same precision, as the results are computed bit
 * by bit, starting with the most significant bit.
 * @endparblock
 * 
 * @param[out]  a_exp       Exponent of outputs of vect_s16_sqrt()
 * @param[out]  b_shr       Right-shift to be applied to elements of @vector{b}
 * @param[in]   b_exp       Exponent of vector{b}
 * @param[in]   b_hr        Headroom of vector{b}
 * 
 * @see vect_s16_sqrt
 * 
 * @ingroup vect_s16_prepare_api
 */
C_API
void vect_s16_sqrt_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    const exponent_t b_exp,
    const right_shift_t b_hr);


/**
 * @brief Obtain the output exponent and shifts required for a call to `vect_s16_sub()`.
 * 
 * The logic for computing the shifts and exponents of `vect_s16_sub()` is identical to that for
 * `vect_s32_add()`.
 * 
 * This macro is provided as a convenience to developers and to make the code more readable.
 * 
 * @see vect_s32_add_prepare()
 * 
 * @ingroup vect_s16_prepare_api
 */
#define vect_s16_sub_prepare vect_s32_add_prepare


#ifdef __XC__
}   //extern "C"
#endif

