
#ifndef BFP_H_
#define BFP_H_

#include "xs3_math_types.h"


#ifdef __XC__
extern "C" {
#endif

/*
    Saturation Logic:

    Where specified, the VPU-enhanced arithmetic functions apply symmetric saturation logic, which is applied
    where results would otherwise overflow.

    The resulting integer range for N-bit saturation is   -(2^(N-1))+1  to  (2^(N-1))-1.

    8-bit:      (-127, 127)
    16-bit:     (-65535, 65535)
    32-bit:     (-2147483647, 2147483647)
*/


/*
    Exponent Calculation / Headroom Minimization:

    In many of the arithmetic functions, an output exponent is chosen prior to actually performing element-by-element
    operations. The exponent is chosen to be the minimum exponent which guarantees no overflow/saturation will occur.**

    This is intended to minimize headroom, but in most situations cannot guarantee zero headroom in the result.

    ** Because of the symmetric saturation logic, a single LSb of saturation may occur, which is considered to be an
    acceptable trade-off for an extra bit of precision.

*/


/** Get the headroom of a 16-bit signed integer.
 *
 * @param[in] a    Input value 
 * 
 * @return    Number of leading sign bits of `a`. 
 */
headroom_t xs3_headroom_s16(
    const int16_t a);

/** Get the headroom of a 32-bit signed integer.
 * 
 * @param[in] a    Input value 
 * 
 * @return    Number of leading sign bits of `a`. 
 */
headroom_t xs3_headroom_s32(
    const int32_t a);

/** Get the headroom of a 64-bit signed integer.
 * 
 * IMPLEMENTATION_NOTE: Simplest way is just to check whether hte top word is 0 or -1,
 * then CLS whichever word that suggests needs it.
 * 
 * @param[in] a    Input value 
 * 
 * @return    Number of leading sign bits of `a`. 
 */
headroom_t xs3_headroom_s64(
    const int64_t a);



//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


/** Count the leading sign bits of a 16-bit BFP vector.
 * 
 * The number of leading sign bits of a sequence is the minimum of
 * that among its elements.
 *
 * @param[in] a         input BFP vector
 * 
 * @return    Number of leading sign bits of `a`. 
 */
headroom_t bfp_s16_headroom(
    bfp_s16_t* a);


/** Count the leading sign bits of a 32-bit BFP vector.
 * 
 * The number of leading sign bits of a sequence is the minimum of
 * that among its elements.
 *
 * @param[in] a         input BFP vector
 * 
 * @return    Number of leading sign bits of `a`. 
 */
headroom_t bfp_s32_headroom(
    bfp_s32_t* a);


/** 
 * @brief Apply a left-shift to the elements of a 16-bit BFP vector.
 * 
 * This operation can be used to remove headroom from a BFP vector.Each mantissa of input BFP vector 
 * @bfp_vec{B} is left-shifted `shl` bits and stored in output BFP vector @bfp_vec{A}.
 * 
 * If the `XS3_BFP_DEBUG_CHECK_LENGTHS` configuration option is enabled (see \ref compile_time_options), this   
 * function will assert that @bfp_vec{A} and @bfp_vec{B} have the same length. Otherwise, no assertion is made and
 * the length of @bfp_vec{A} is updated to the length of @bfp_vec{B}. In either case, the length of 
 * @bfp_vec{B} dictates the number of elements to be updated.
 * 
 * The operation saturates to 16-bit bounds.
 * 
 * Both `bfp_s16_t` vectors must already have been initialized using `bfp_s16_init()`.
 * 
 * @bfp_op{16, @f$ \bar A \leftarrow \bar B \cdot 2^\{shl\}  @f$ }
 * 
 * \safe_in_place{a,b}
 * 
 * @param[out] a     Output BFP vector @bfp_vec{A}
 * @param[in]  b     Input BFP vector @bfp_vec{B}
 * @param[in]  shl   Number of bits to left shift
 */
void bfp_s16_shl(
    bfp_s16_t* a,
    const bfp_s16_t* b,
    const left_shift_t shl);

/** 
 * @brief Apply a left-shift to the elements of a 32-bit BFP vector.
 * 
 * This operation can be used to remove headroom from a BFP vector.Each mantissa of input BFP vector 
 * @bfp_vec{B} is left-shifted `shl` bits and stored in output BFP vector @bfp_vec{A}.
 * 
 * If the `XS3_BFP_DEBUG_CHECK_LENGTHS` configuration option is enabled (see \ref compile_time_options), this   
 * function will assert that @bfp_vec{A} and @bfp_vec{B} have the same length. Otherwise, no assertion is made and
 * the length of @bfp_vec{A} is updated to the length of @bfp_vec{B}. In either case, the length of 
 * @bfp_vec{B} dictates the number of elements to be updated.
 * 
 * The operation saturates to 32-bit bounds.
 * 
 * Both `bfp_s32_t` vectors must already have been initialized using `bfp_s32_init()`.
 * 
 * @bfp_op{32, @f$ \bar A \leftarrow \bar B \cdot 2^\{shl\}  @f$ }
 * 
 * \safe_in_place{a,b}
 * 
 * @param[out] a     Output BFP vector @bfp_vec{A}
 * @param[in]  b     Input BFP vector @bfp_vec{B}
 * @param[in]  shl   Number of bits to left shift
 */
void bfp_s32_shl(
    bfp_s32_t* a,
    const bfp_s32_t* b,
    const left_shift_t shl);



/** 
 * @brief Add two 16-bit BFP vectors together.
 * 
 * Add together two input BFP vectors @bfp_vec{B} and @bfp_vec{C} and store the result
 * in BFP vector @bfp_vec{A}. 
 * 
 * The headroom, exponent, mantissas and (possibly) length of @bfp_vec{A} will each be updated by this 
 * function. @f$A_{exp}@f$, the exponent associated with @bfp_vec{A}, is chosen to avoid the possibility of 
 * saturating output mantissas (with a possible corner case exception; more below). Underflow, when it occurs, 
 * happens to the operands _prior_ to the addition (see \ref arithmetic_underflow). 
 * 
 * If the `XS3_BFP_DEBUG_CHECK_LENGTHS` configuration option is enabled (see \ref compile_time_options), this   
 * function will assert that all three BFP vectors have the same length. Otherwise, no assertion is made and
 * the length of @bfp_vec{A} is updated to the length of @bfp_vec{B}. In either case, the length of 
 * @bfp_vec{B} dictates the number of elements to be updated.
 * 
 * If the `XS3_BFP_ALLOW_SATURATION` configuration option (see \ref compile_time_options) is disabled
 * (enabled by default), this function will detect and avoid corner case saturation (see \ref saturation).
 *  
 * All three `bfp_s16_t` vectors (including the one to which `a` points) must already have been initialized 
 * using `bfp_s16_init()`.
 * 
 * @bfp_op{16, @f$ \bar A \leftarrow \bar B + \bar C  @f$ }
 * 
 * \safe_in_place{a,b,c}
 * 
 * @param[out] a     Output BFP vector @bfp_vec{A}
 * @param[in]  b     Input BFP vector @bfp_vec{B}
 * @param[in]  c     Input BFP vector @bfp_vec{C}
 */
void bfp_s16_add(
    bfp_s16_t* a, 
    const bfp_s16_t* b, 
    const bfp_s16_t* c);


/** 
 * @brief Add two 32-bit BFP vectors together.
 * 
 * Add together two input BFP vectors @bfp_vec{B} and @bfp_vec{C} and store the result
 * in BFP vector @bfp_vec{A}. 
 * 
 * The headroom, exponent, mantissas and (possibly) length of @bfp_vec{A} will each be updated by this 
 * function. @math{A_{exp}}, the exponent associated with @bfp_vec{A}, is chosen to avoid the possibility of 
 * saturating output mantissas (with a possible corner case exception; more below). Underflow, when it occurs, 
 * happens to the operands _prior_ to the addition (see \ref arithmetic_underflow). 
 * 
 * If the `XS3_BFP_DEBUG_CHECK_LENGTHS` configuration option is enabled (see \ref compile_time_options), this
 * function will assert that all three BFP vectors have the same length. Otherwise, no assertion is made and
 * the length of @bfp_vec{A} is updated to the length of @bfp_vec{B}. In either case, the length of 
 * @bfp_vec{B} dictates the number of elements to be updated.
 * 
 * If the `XS3_BFP_ALLOW_SATURATION` configuration option (see \ref compile_time_options) is disabled
 * (enabled by default), this function will detect and avoid corner case saturation (see \ref saturation).
 *  
 * All three `bfp_s32_t` vectors (including the one to which `a` points) must already have been initialized 
 * using `bfp_s32_init()`.
 * 
 * @bfp_op{32, @f$ \bar A \leftarrow \bar B + \bar C  @f$ }
 * 
 * \safe_in_place{a,b,c}
 * 
 * @param[out] a     Output BFP vector @bfp_vec{A}
 * @param[in]  b     Input BFP vector @bfp_vec{B}
 * @param[in]  c     Input BFP vector @bfp_vec{C}
 */
void bfp_s32_add(
    bfp_s32_t* a, 
    const bfp_s32_t* b, 
    const bfp_s32_t* c);



/** 
 * @brief Subtract one 16-bit BFP vector from another.
 * 
 * Subtract input BFP vector @bfp_vec{C} from input BFP vector @bfp_vec{V} and store the result
 * in output BFP vector @bfp_vec{A}. 
 * 
 * The headroom, exponent, mantissas and (possibly) length of @bfp_vec{A} will each be updated by this 
 * function. @f$A_{exp}@f$, the exponent associated with @bfp_vec{A}, is chosen to avoid the possibility of 
 * saturating output mantissas. Underflow, when it occurs, happens to the operands _prior_ to the addition 
 * (see \ref arithmetic_underflow). 
 * 
 * If the `XS3_BFP_DEBUG_CHECK_LENGTHS` configuration option is enabled (see \ref compile_time_options), this   
 * function will assert that all three BFP vectors have the same length. Otherwise, no assertion is made and
 * the length of @bfp_vec{A} is updated to the length of @bfp_vec{B}. In either case, the length of 
 * @bfp_vec{B} dictates the number of elements to be updated.
 *  
 * All three `bfp_s16_t` vectors (including the one to which `a` points) must already have been initialized 
 * using `bfp_s16_init()`.
 * 
 * @bfp_op{16, @f$ \bar A \leftarrow \bar B - \bar C  @f$ }
 * 
 * \safe_in_place{a,b,c}
 * 
 * @param[out] a     Output BFP vector @bfp_vec{A}
 * @param[in]  b     Input BFP vector @bfp_vec{B}
 * @param[in]  c     Input BFP vector @bfp_vec{C}
 */
void bfp_s16_sub(
    bfp_s16_t* a, 
    const bfp_s16_t* b, 
    const bfp_s16_t* c);

/** 
 * @brief Subtract one 32-bit BFP vector from another.
 * 
 * Subtract input BFP vector @bfp_vec{C} from input BFP vector @bfp_vec{V} and store the result
 * in output BFP vector @bfp_vec{A}. 
 * 
 * The headroom, exponent, mantissas and (possibly) length of @bfp_vec{A} will each be updated by this 
 * function. @f$A_{exp}@f$, the exponent associated with @bfp_vec{A}, is chosen to avoid the possibility of 
 * saturating output mantissas. Underflow, when it occurs, happens to the operands _prior_ to the addition 
 * (see \ref arithmetic_underflow). 
 * 
 * If the `XS3_BFP_DEBUG_CHECK_LENGTHS` configuration option is enabled (see \ref compile_time_options), this   
 * function will assert that all three BFP vectors have the same length. Otherwise, no assertion is made and
 * the length of @bfp_vec{A} is updated to the length of @bfp_vec{B}. In either case, the length of 
 * @bfp_vec{B} dictates the number of elements to be updated.
 *  
 * All three `bfp_s32_t` vectors (including the one to which `a` points) must already have been initialized 
 * using `bfp_s32_init()`.
 * 
 * @bfp_op{32, @f$ \bar A \leftarrow \bar B - \bar C  @f$ }
 * 
 * \safe_in_place{a,b,c}
 * 
 * @param[out] a     Output BFP vector @bfp_vec{A}
 * @param[in]  b     Input BFP vector @bfp_vec{B}
 * @param[in]  c     Input BFP vector @bfp_vec{C}
 */
void bfp_s32_sub(
    bfp_s32_t* a, 
    const bfp_s32_t* b, 
    const bfp_s32_t* c);


/** 
 * @brief Multiply one 16-bit BFP vector by another.
 * 
 * Multiply input BFP vector @bfp_vec{B} by input BFP vector @bfp_vec{C} and store the result
 * in output BFP vector @bfp_vec{A}.
 * 
 * The headroom, exponent, mantissas and (possibly) length of @bfp_vec{A} will each be updated by this 
 * function. @f$A_{exp}@f$, the exponent associated with @bfp_vec{A}, is chosen to avoid the possibility of 
 * saturating output mantissas. 
 * 
 * If the `XS3_BFP_DEBUG_CHECK_LENGTHS` configuration option is enabled (see \ref compile_time_options), this   
 * function will assert that all three BFP vectors have the same length. Otherwise, no assertion is made and
 * the length of @bfp_vec{A} is updated to the length of @bfp_vec{B}. In either case, the length of 
 * @bfp_vec{B} dictates the number of elements to be updated.
 *  
 * All three `bfp_s16_t` vectors must already have been initialized using `bfp_s16_init()`.
 * 
 * @bfp_op{16, @f$ A_k \leftarrow B_k \cdot C_k  \qquad\text{ for } k\in 0 .. \left(B_\{length\}-1\right)@f$ }
 * 
 * \safe_in_place{a,b,c}
 * 
 * @param a     Output BFP vector @bfp_vec{A}
 * @param b     Input BFP vector @bfp_vec{B}
 * @param c     Input BFP vector @bfp_vec{C}
 */
void bfp_s16_mul(
    bfp_s16_t* a, 
    const bfp_s16_t* b, 
    const bfp_s16_t* c);



/**
 * @brief Multiply one 32-bit BFP vector by another.
 * 
 * Multiply input BFP vector @bfp_vec{B} by input BFP vector @bfp_vec{C} and store the result
 * in output BFP vector @bfp_vec{A}.
 * 
 * The headroom, exponent, mantissas and (possibly) length of @bfp_vec{A} will each be updated by this 
 * function. @f$A_{exp}@f$, the exponent associated with @bfp_vec{A}, is chosen to avoid the possibility of 
 * saturating output mantissas. 
 * 
 * If the `XS3_BFP_DEBUG_CHECK_LENGTHS` configuration option is enabled (see \ref compile_time_options), this   
 * function will assert that all three BFP vectors have the same length. Otherwise, no assertion is made and
 * the length of @bfp_vec{A} is updated to the length of @bfp_vec{B}. In either case, the length of 
 * @bfp_vec{B} dictates the number of elements to be updated.
 *  
 * All three `bfp_s32_t` vectors must already have been initialized using `bfp_s32_init()`.
 * 
 * @bfp_op{32, @f$ A_k \leftarrow B_k \cdot C_k  \qquad\text{ for } k\in 0 .. \left(B_\{length\}-1\right)@f$ }
 * 
 * \safe_in_place{a,b,c}
 * 
 * @param[out] a     Output BFP vector @bfp_vec{A}
 * @param[in]  b     Input BFP vector @bfp_vec{B}
 * @param[in]  c     Input BFP vector @bfp_vec{C}
 */
void bfp_s32_mul(
    bfp_s32_t* a, 
    const bfp_s32_t* b, 
    const bfp_s32_t* c);


/** 
 * @brief Multiply a 16-bit BFP vector by a scalar.
 * 
 * Multiply input BFP vector @bfp_vec{B} by scalar @f$\alpha\cdot 2^{\alpha_{exp}}@f$ and store the result
 * in output BFP vector @bfp_vec{A}.
 * 
 * The headroom, exponent, mantissas and (possibly) length of @bfp_vec{A} will each be updated by this 
 * function. @f$A_{exp}@f$, the exponent associated with @bfp_vec{A}, is chosen to avoid the possibility of 
 * saturating output mantissas. 
 * 
 * If the `XS3_BFP_DEBUG_CHECK_LENGTHS` configuration option is enabled (see \ref compile_time_options), this   
 * function will assert that both @bfp_vec{A} and @bfp_vec{B} have the same length. Otherwise, no assertion is made and
 * the length of @bfp_vec{A} is updated to the length of @bfp_vec{B}. In either case, the length of 
 * @bfp_vec{B} dictates the number of elements to be updated.
 *  
 * Both `bfp_s16_t` must already have been initialized using `bfp_s16_init()`.
 * 
 * @bfp_op{16, @f$ \bar A\leftarrow \alpha \bar B@f$ }
 * 
 * \safe_in_place{a,b,c}
 * 
 * @param[out] a             Output BFP vector @bfp_vec{A}
 * @param[in]  b             Input BFP vector @bfp_vec{B}
 * @param[in]  alpha_mant    Input BFP vector @bfp_vec{C}
 * @param[in]  alpha_exp     Input BFP vector @bfp_vec{C}
 */
void bfp_s16_scale(
    bfp_s16_t* a, 
    const bfp_s16_t* b, 
    const int16_t alpha_mant,
    const exponent_t alpha_exp);

/** 
 * @brief Multiply a 32-bit BFP vector by a scalar.
 * 
 * Multiply input BFP vector @bfp_vec{B} by scalar @f$\alpha\cdot 2^{\alpha_{exp}}@f$ and store the result
 * in output BFP vector @bfp_vec{A}.
 * 
 * The headroom, exponent, mantissas and (possibly) length of @bfp_vec{A} will each be updated by this 
 * function. @f$A_{exp}@f$, the exponent associated with @bfp_vec{A}, is chosen to avoid the possibility of 
 * saturating output mantissas. 
 * 
 * If the `XS3_BFP_DEBUG_CHECK_LENGTHS` configuration option is enabled (see \ref compile_time_options), this   
 * function will assert that both @bfp_vec{A} and @bfp_vec{B} have the same length. Otherwise, no assertion is made and
 * the length of @bfp_vec{A} is updated to the length of @bfp_vec{B}. In either case, the length of 
 * @bfp_vec{B} dictates the number of elements to be updated.
 *  
 * Both `bfp_s32_t` must already have been initialized using `bfp_s32_init()`.
 * 
 * @bfp_op{32, @f$ \bar A\leftarrow \alpha \bar B@f$ }
 * 
 * \safe_in_place{a,b,c}
 * 
 * @param[out] a             Output BFP vector @bfp_vec{A}
 * @param[in]  b             Input BFP vector @bfp_vec{B}
 * @param[in]  alpha_mant    Input BFP vector @bfp_vec{C}
 * @param[in]  alpha_exp     Input BFP vector @bfp_vec{C}
 */
void bfp_s32_scale(
    bfp_s32_t* a, 
    const bfp_s32_t* b,
    const int32_t alpha_mant,
    const exponent_t alpha_exp);


/** 
 * @brief Take the absolute value of a 16-bit BFP vector.
 * 
 * Compute the absolute value of each element @f$B_k@f$ of input BFP vector @bfp_vec{B} and store the
 * results in output BFP vector @bfp_vec{A}.
 * 
 * The headroom, exponent, mantissas and (possibly) length of @bfp_vec{A} will each be updated by this 
 * function.
 * 
 * If the `XS3_BFP_DEBUG_CHECK_LENGTHS` configuration option is enabled (see \ref compile_time_options), this
 * function will assert that all three BFP vectors have the same length. Otherwise, no assertion is made and
 * the length of @bfp_vec{A} is updated to the length of @bfp_vec{B}. In either case, the length of 
 * @bfp_vec{B} dictates the number of elements to be updated.
 * 
 * If the `XS3_BFP_ALLOW_SATURATION` configuration option (see \ref compile_time_options) is disabled
 * (enabled by default), this function will detect and avoid corner case saturation (see \ref saturation).
 * The corner case condition is detected if `b->hr == 0`, as a mantissa value of `-0x4000` would saturate 
 * to `0x3FFF`.
 *  
 * Both `bfp_s16_t` vectors must already have been initialized using `bfp_s16_init()`.
 * 
 * @bfp_op{16, @f$ a_k \leftarrow \left|b_k\right|  \qquad\text{ for } k\in 0 .. \left(B_\{length\}-1\right)@f$ }
 * 
 * \safe_in_place{a,b}
 * 
 * @param[out] a     Output BFP vector @bfp_vec{A}
 * @param[in]  b     Input BFP vector @bfp_vec{B}
 */
void bfp_s16_abs(
    bfp_s16_t* a,
    const bfp_s16_t* b);

/** 
 * @brief Take the absolute value of a 32-bit BFP vector.
 * 
 * Compute the absolute value of each element @f$B_k@f$ of input BFP vector @bfp_vec{B} and store the
 * results in output BFP vector @bfp_vec{A}.
 * 
 * The headroom, exponent, mantissas and (possibly) length of @bfp_vec{A} will each be updated by this 
 * function.
 * 
 * If the `XS3_BFP_DEBUG_CHECK_LENGTHS` configuration option is enabled (see \ref compile_time_options), this
 * function will assert that all three BFP vectors have the same length. Otherwise, no assertion is made and
 * the length of @bfp_vec{A} is updated to the length of @bfp_vec{B}. In either case, the length of 
 * @bfp_vec{B} dictates the number of elements to be updated.
 * 
 * If the `XS3_BFP_ALLOW_SATURATION` configuration option (see \ref compile_time_options) is disabled
 * (enabled by default), this function will detect and avoid corner case saturation (see \ref saturation).
 * The corner case condition is detected if `b->hr == 0`, as a mantissa value of `-0x40000000` would saturate 
 * to `0x3FFFFFFF`.
 *  
 * Both `bfp_s32_t` vectors must already have been initialized using `bfp_s32_init()`.
 * 
 * @bfp_op{32, @f$ a_k \leftarrow \left|b_k\right|  \qquad\text{ for } k\in 0 .. \left(B_\{length\}-1\right)@f$ }
 * 
 * \safe_in_place{a,b}
 * 
 * @param[out] a     Output BFP vector @bfp_vec{A}
 * @param[in]  b     Input BFP vector @bfp_vec{B}
 */
void bfp_s32_abs(
    bfp_s32_t* a,
    const bfp_s32_t* b);

/** 
 * @brief Sum the mantissas of a 16-bit BFP vector.
 * 
 * The 16-bit mantissas of the input BFP vector @bfp_vec{B} are summed to produce a 32-bit result, @f$\alpha@f$,
 * which is returned. @f$\alpha@f$ taken with the input vector's exponent (i.e. @f${\alpha}\cdot 2^{B_{exp}} @f$) 
 * is the sum of the represented values of @bfp_vec{B}.
 * 
 * 
 * @param[in] b         Input BFP vector @bfp_vec{B}
 * 
 * @return  Sum of mantissas of @bfp_vec{B}
 */
int32_t bfp_s16_sum(
    const bfp_s16_t* b);

/** 
 * @brief Sum the mantissas of a 32-bit BFP vector.
 * 
 * The 32-bit mantissas of the input BFP vector @bfp_vec{B} are summed to produce a 64-bit result, @f$\alpha@f$,
 * which is returned. @f$\alpha@f$ taken with the input vector's exponent (i.e. @f${\alpha}\cdot 2^{B_{exp}} @f$) 
 * is the sum of the represented values of @bfp_vec{B}.
 * 
 * @bfp_op{32, @f$ \left(\sum_\{k=0\}^\{B_\{length\}-1\} b_k \right)\cdot 2^\{-B_\{exp\}\}  @f$ }
 * 
 * @param[in] b         Input BFP vector @bfp_vec{B}
 * 
 * @return  Sum of mantissas of @bfp_vec{B}
 */
int64_t bfp_s32_sum(
    const bfp_s32_t* b);


/** 
 * @brief Compute the inner product of two 16-bit BFP vectors.
 * 
 * Adds together the element-wise products of input BFP vectors @bfp_vec{B} and @bfp_vec{C} for a resulting 32-bit
 * mantissa and an exponent. The mantissa is returned; the exponent is output through `a_exp`.
 * 
 * @bfp_op{16, @f$ A\cdot 2^\{A_\{exp\}\}\longleftarrow \sum_\{k=0\}^\{B_\{length\}-1\} \left( B_k\cdot C_k\cdot B_{exp}\cdot C_{exp} \right) @f$}
 * 
 * 
 * @param[out] a_exp    Output exponent
 * @param[in]  b        Input BFP vector @bfp_vec{B}
 * @param[in]  c        Input BFP vector @bfp_vec{C}
 * 
 * @return The 32-bit mantissa of the inner product.
 */
int64_t bfp_s16_dot(
    exponent_t* a_exp,
    const bfp_s16_t* b, 
    const bfp_s16_t* c);

/** 
 * @brief Compute the inner product of two 32-bit BFP vectors.
 * 
 * Adds together the element-wise products of input BFP vectors @bfp_vec{B} and @bfp_vec{C} for a resulting 40-bit
 * mantissa and an exponent. The mantissa is returned; the exponent is output through `a_exp`.
 * 
 * @bfp_op{32, @f$ A\cdot 2^\{A_\{exp\}\}\longleftarrow \sum_\{k=0\}^\{B_\{length\}-1\} \left( B_k\cdot C_k\cdot B_{exp}\cdot C_{exp} \right) @f$}
 * 
 * 
 * @param[out] a_exp    Output exponent
 * @param[in]  b        Input BFP vector @bfp_vec{B}
 * @param[in]  c        Input BFP vector @bfp_vec{C}
 * 
 * @return The 40-bit mantissa of the inner product.
 */
int64_t bfp_s32_dot(
    exponent_t* a_exp,
    const bfp_s32_t* b, 
    const bfp_s32_t* c);


/** Clip the elements of a 16-bit BFP vector to a specified range.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Conceptually, the operation performed is:
 *      A[i] <-  min( U,  max( L,  B[i] ) )
 *        for each index i in B[]
 *        where A[] and B[] are BFP vectors
 *              U is a sclar; the range's upper bound
 *              L is a scalar; the range's lower bound
 * 
 * `a->data` must already be initialized to a valid memory buffer.
 * It is safe to supply the same `bfp_s16_t*` for `a` and `b`.
 * 
 * @param[out] a             Output BFP vector
 * @param[in]  b             Input BFP vector
 * @param[in]  lower_bound   Mantissa of the lower clipping bound
 * @param[in]  upper_bound   Mantissa of the upper clipping bound
 * @param[in]  bound_exp     Shared exponent of the clipping bounds
 */
void bfp_s16_clip(
    bfp_s16_t* a, 
    const bfp_s16_t* b, 
    const int16_t lower_bound, 
    const int16_t upper_bound, 
    const int bound_exp);

/** Clip the elements of a 32-bit BFP vector to a specified range.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * Conceptually, the operation performed is:
 *      A[i] <-  min( U,  max( L,  B[i] ) )
 *        for each index i in B[]
 *        where A[] and B[] are BFP vectors
 *              U is a sclar; the range's upper bound
 *              L is a scalar; the range's lower bound
 * 
 * `a->data` must already be initialized to a valid memory buffer.
 * It is safe to supply the same `bfp_s32_t*` for `a` and `b`.
 * 
 * @param[out] a             Output BFP vector
 * @param[in]  b             Input BFP vector
 * @param[in]  lower_bound   Mantissa of the lower clipping bound
 * @param[in]  upper_bound   Mantissa of the upper clipping bound
 * @param[in]  bound_exp     Shared exponent of the clipping bounds
 */
void bfp_s32_clip(
    bfp_s32_t* a, 
    const bfp_s32_t* b, 
    const int32_t lower_bound, 
    const int32_t upper_bound, 
    const int bound_exp);


/** 
 * @brief Rectify a 16-bit BFP vector.
 * 
 * Rectify each element @f$B_k@f$ of input BFP vector @bfp_vec{B} and store the results in output BFP 
 * vector @bfp_vec{A}. The headroom, exponent, mantissas and (possibly) length of @bfp_vec{A} will each
 * be updated by this function.
 * 
 * If the `XS3_BFP_DEBUG_CHECK_LENGTHS` configuration option is enabled (see \ref compile_time_options), this
 * function will assert that all three BFP vectors have the same length. Otherwise, no assertion is made and
 * the length of @bfp_vec{A} is updated to the length of @bfp_vec{B}. In either case, the length of 
 * @bfp_vec{B} dictates the number of elements to be updated.
 *  
 * Both `bfp_s16_t` vectors must already have been initialized using `bfp_s16_init()`.
 * 
 * @bfp_op{16, @f$ a_k \leftarrow \begin\{cases\}b_k & b_k \gt 0 \\ 
 *                                  0 & b_k \leq 0\end\{cases\} 
 *                                  \qquad\text{ for }k\in 0\ ...\left(B_\{length\}-1\right)@f$ }
 * 
 * \safe_in_place{a,b}
 * 
 * @param[out] a     Output BFP vector @bfp_vec{A}
 * @param[in]  b     Input BFP vector @bfp_vec{B}
 */
void bfp_s16_rect(
    bfp_s16_t* a,
    const bfp_s16_t* b);


/** 
 * @brief Rectify a 32-bit BFP vector.
 * 
 * Rectify each element @f$B_k@f$ of input BFP vector @bfp_vec{B} and store the results in output BFP 
 * vector @bfp_vec{A}. The headroom, exponent, mantissas and (possibly) length of @bfp_vec{A} will each
 * be updated by this function.
 * 
 * If the `XS3_BFP_DEBUG_CHECK_LENGTHS` configuration option is enabled (see \ref compile_time_options), this
 * function will assert that all three BFP vectors have the same length. Otherwise, no assertion is made and
 * the length of @bfp_vec{A} is updated to the length of @bfp_vec{B}. In either case, the length of 
 * @bfp_vec{B} dictates the number of elements to be updated.
 *  
 * Both `bfp_s16_t` vectors must already have been initialized using `bfp_s16_init()`.
 * 
 * @bfp_op{32, @f$ a_k \leftarrow \begin\{cases\}b_k & b_k \gt 0 \\ 
 *                                  0 & b_k \leq 0\end\{cases\} 
 *                                  \qquad\text{ for }k\in 0\ ...\left(B_\{length\}-1\right)@f$ }
 * 
 * \safe_in_place{a,b}
 * 
 * @param[out] a     Output BFP vector @bfp_vec{A}
 * @param[in]  b     Input BFP vector @bfp_vec{B}
 */
void bfp_s32_rect(
    bfp_s32_t* a, 
    const bfp_s32_t* b);

/** 
 * @brief Convert a 32-bit BFP vector into a 16-bit BFP vector.
 * 
 * Reduces the bit-depth of each 32-bit element of input BFP vector @bfp_vec{B} to 16 bits, and store the 
 * 16 bit result in output BFP vector @bfp_vec{A}. The headroom, exponent, mantissas and (possibly) length 
 * of @bfp_vec{A} will each be updated by this function.
 * 
 * If the `XS3_BFP_DEBUG_CHECK_LENGTHS` configuration option is enabled (see \ref compile_time_options), this
 * function will assert that all three BFP vectors have the same length. Otherwise, no assertion is made and
 * the length of @bfp_vec{A} is updated to the length of @bfp_vec{B}. In either case, the length of 
 * @bfp_vec{B} dictates the number of elements to be updated.
 * 
 * If the `XS3_BFP_ALLOW_SATURATION` configuration option (see \ref compile_time_options) is disabled
 * (enabled by default), this function will ensure there is 1 bit of headroom in the resulting vector.
 * Otherwise all headroom is removed.
 *  
 * Vectors @bfp_vec{A} and vector @bfp_vec{B} must already have been initialized using 
 * `bfp_s16_init()` and `bfp_s32_init()` respectively.
 * 
 * @bfp_op{32, @f$ \bar A \overset\{16-bit\}\{\longleftarrow\} \bar B@f$ }
 * 
 * \safe_in_place{a,b}
 * 
 * @param[out] a     Output BFP vector @bfp_vec{A}
 * @param[in]  b     Input BFP vector @bfp_vec{B}
 **/
void bfp_s32_to_s16(
    bfp_s16_t* a,
    const bfp_s32_t* b);

/** 
 * @brief Convert a 16-bit BFP vector into a 32-bit BFP vector.
 * 
 * Increases the bit-depth of each 16-bit element of input BFP vector @bfp_vec{B} to 32 bits, and store the 
 * 32 bit result in output BFP vector @bfp_vec{A}. The headroom, exponent, mantissas and (possibly) length 
 * of @bfp_vec{A} will each be updated by this function.
 * 
 * The headroom of the resulting BFP vector will always be 8 bits larger than the input.
 * 
 * If the `XS3_BFP_DEBUG_CHECK_LENGTHS` configuration option is enabled (see \ref compile_time_options), this
 * function will assert that all three BFP vectors have the same length. Otherwise, no assertion is made and
 * the length of @bfp_vec{A} is updated to the length of @bfp_vec{B}. In either case, the length of 
 * @bfp_vec{B} dictates the number of elements to be updated.
 *  
 * Vectors @bfp_vec{A} and vector @bfp_vec{B} must already have been initialized using 
 * `bfp_s32_init()` and `bfp_s16_init()` respectively.
 * 
 * @bfp_op{16, @f$ \bar A \overset\{32-bit\}\{\longleftarrow\} \bar B@f$ }
 * 
 * @param[out] a     Output BFP vector @bfp_vec{A}
 * @param[in]  b     Input BFP vector @bfp_vec{B}
 **/
void bfp_s16_to_s32(
    bfp_s32_t* a,
    const bfp_s16_t* b);



void bfp_s16_sqrt(
    bfp_s16_t* a,
    const bfp_s16_t* b);


void bfp_s32_sqrt(
    bfp_s32_t* a,
    const bfp_s32_t* b);


void bfp_s16_inverse(
    bfp_s16_t* a,
    const bfp_s16_t* b);


void bfp_s32_inverse(
    bfp_s32_t* a,
    const bfp_s32_t* b);


/** Calculate the absolute sum of a 16-bit BFP vector.
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
 * \param b     Input vector
 * 
 * \return  Absolute sum
 */
int32_t bfp_s16_abs_sum(
    const bfp_s16_t* b);

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
 * \param b     Input vector
 * 
 * \return  Absolute sum
 */
int64_t bfp_s32_abs_sum(
    const bfp_s32_t* b);

/** Calculate the mean of a 16-bit BFP vector.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Conceptually, the operation performed is:
 *      A <-  ( B[0] + B[1] + ... + B[N-1] ) / N
 *        where A is a scalar
 *              B[] is a BFP vector of size N
 * 
 * \param b     Input vector
 * 
 * \return  Mean
 */
int16_t bfp_s16_mean(
    exponent_t* a_exp,
    const bfp_s16_t* b);

/** Calculate the mean of a 32-bit BFP vector.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Conceptually, the operation performed is:
 *      A <-  ( B[0] + B[1] + ... + B[N-1] ) / N
 *        where A is a scalar
 *              B[] is a BFP vector of size N
 * 
 * \param b     Input vector
 * 
 * \return  Mean
 */
int32_t bfp_s32_mean(
    exponent_t* a_exp,
    const bfp_s32_t* b);
    
/** Calculate the power of a 16-bit BFP vector.
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
 * \param b     Input vector
 * 
 * \return  Energy
 */
int64_t bfp_s16_energy(
    exponent_t* a_exp,
    const bfp_s16_t* b);


/** Calculate the power of a 32-bit BFP vector.
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
 * \param b     Input vector
 * 
 * \return  Energy
 */
int64_t bfp_s32_energy(
    exponent_t* a_exp,
    const bfp_s32_t* b);

/** Calculate the RMS of a 16-bit BFP vector.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Conceptually, the operation performed is:
 *      A <-  sqrt( ((B[0])^2 + (B[1])^2 + ... + (B[N-1])^2) / N )
 *        where A is a scalar
 *              B[] is a BFP vector of size N
 * 
 * \param b     Input vector
 * 
 * \return  RMS 
 */
int32_t bfp_s16_rms(
    exponent_t* a_exp,
    const bfp_s16_t* b);

/** Calculate the RMS of a 32-bit BFP vector.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Conceptually, the operation performed is:
 *      A <-  sqrt( ((B[0])^2 + (B[1])^2 + ... + (B[N-1])^2) / N )
 *        where A is a scalar
 *              B[] is a BFP vector of size N
 * 
 * \param b     Input vector
 * 
 * \return  RMS
 */
int32_t bfp_s32_rms(
    exponent_t* a_exp,
    const bfp_s32_t* b);


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
 * \param b     Input vector
 * 
 * \return  Max value
 */
int16_t bfp_s16_max(
    const bfp_s16_t* b);

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
 * \param b     Input vector
 * 
 * \return  Max value
 */
int32_t bfp_s32_max(
    const bfp_s32_t* b);


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
 * \param b     Input vector
 * 
 * \return  Min value
 */
int16_t bfp_s16_min(
    const bfp_s16_t* b);

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
 * \param b     Input vector
 * 
 * \return  Min value
 */
int32_t bfp_s32_min(
    const bfp_s32_t* b);


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
 * \param b     Input vector
 * 
 * \return  Index of the maximum element of `b`.
 */
unsigned bfp_s16_argmax(
    const bfp_s16_t* b);

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
 * \param b     Input vector
 * 
 * \return  Index of the maximum element of `b`.
 */
unsigned bfp_s32_argmax(
    const bfp_s32_t* b);

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
 * \param b     Input vector
 * 
 * \return  Index of the minimum element of `b`.
 */
unsigned bfp_s16_argmin(
    const bfp_s16_t* b);

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
 * \param b     Input vector
 * 
 * \return  Index of the minimum element of `b`.
 */
unsigned bfp_s32_argmin(
    const bfp_s32_t* b);


#ifdef __XC__
}   //extern "C"
#endif

#endif //BFP_H_