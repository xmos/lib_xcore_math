

#ifndef XS3_UTIL_H_
#define XS3_UTIL_H_

#include "xs3_math_types.h"

#include <stdio.h>
#include <assert.h>
#include <math.h>

#ifdef __XC__
extern "C" {
#endif

/**
 * @brief Takes the greater of arguments `A` and `B`, preferring `A` on equality.
 * 
 * @note This is not safe from multiple evaluation of arguments.
 */
#define MAX(A,B) (((A) >= (B))? (A) : (B))

/**
 * @brief Takes the lesser of arguments `A` and `B`, preferring `A` on equality.
 * 
 * @note This is not safe from multiple evaluation of arguments.
 */
#define MIN(A,B) (((A) <= (B))? (A) : (B))

/**
 * @brief Perform an arithmetic right-shift with a signed shift. i.e. negative values of `SHR` will left-shift.
 * 
 * @note Neither rounding nor saturation logic is applied.
 * 
 * @todo Is this necessary? It's only used in a couple places and probably shouldn't be.
 */ 
#define SIGNED_ASHR(VAL, SHR)  (((SHR) >= 0)? ((VAL) >> (SHR)) : ((VAL) << (-(SHR))))

/**
 * @brief Count leading sign bits of an `int16_t`.
 */
#define CLS_S16(X)      (cls(X) - 16)

/**
 * @brief Count leading sign bits of an `int32_t`.
 */
#define CLS_S32(X)      (cls(X))

/**
 * @brief Count leading sign bits of an `int64_t`.
 */
#define CLS_S64(X)    ( (cls((int32_t)(((int64_t)(X))>>32)) == 32)?                                             \
                        (cls((int32_t)(((int64_t)(X))>>16)) == 32)?    32 + cls((int32_t)(X))                   \
                                                                  :    16 + cls((int32_t)(((int64_t)(X))>>16))  \
                                                                  :    cls((int32_t)(((int64_t)(X))>>32))   )





/**
 * @brief Count leading sign bits of a `complex_s16_t`.
 * 
 * The number of  leading sign bits for a complex integer is defined as the minimum of the number of leading sign bits 
 * for its real part and for its imaginary part.
 */
#define CLS_C16(X)      (MIN(CLS_S16((X).re), CLS_S16((X).im)))

/**
 * @brief Count leading sign bits of a `complex_s32_t`.
 * 
 * The number of  leading sign bits for a complex integer is defined as the minimum of the number of leading sign bits 
 * for its real part and for its imaginary part.
 */
#define CLS_C32(X)      (MIN(CLS_S32(((int32_t)(X).re)), CLS_S32(((int32_t)(X).im))))


/**
 * @brief Get the headroom of an `int64_t`.
 */
#define HR_S64(X)   (CLS_S64(X)-1)

/**
 * @brief Get the headroom of an `int32_t`.
 */
#define HR_S32(X)   (CLS_S32(X)-1)

/**
 * @brief Get the headroom of an `int32_t`.
 */
#define HR_S16(X)   (CLS_S16(((int16_t)X))-1)

/**
 * @brief Get the headroom of a `complex_s32_t`.
 */
#define HR_C32(X)   (CLS_C32(X)-1)

/**
 * @brief Get the headroom of a `complex_s16_t`.
 */
#define HR_C16(X)   (CLS_C16(X)-1)

/**
 * @brief Count leading sign bits of `int32_t`.
 * 
 * This function returns the number of most-significant bits
 * in `a` which are equal to its sign bit.
 * 
 * @param[in] a Input value
 * 
 * @returns Number of leading sign bits
 */
static inline unsigned cls(
    const int32_t a)
{
#ifdef __XS3A__
    unsigned res;
    asm( "cls %0, %1" : "=r"(res) : "r"(a) );
    return res;
#else
    if(a == 0 || a == -1)
        return 32;
    
    if( a > 0 ){
        for(int i = 30; i >= 0; i--){
            if(a & (1<<i)) return 31-i;
        }
    } else {
        for(int i = 30; i >= 0; i--){
            unsigned mask = (1<<i);
            if((a | mask) != a) return 31-i;
        }
    }
    assert(0);
    return 0;

#endif //__XS3A__
}

/**
 * @brief Convert a 64-bit floating-point scalar to a 32-bit floating-point scalar.
 * 
 * Converts a 64-bit floating-point scalar, represented by the 64-bit mantissa `b` and exponent `b_exp`, into a 32-bit
 * floating-point scalar, represented by the 32-bit returned mantissa and output exponent `a_exp`.
 * 
 * @param[out] a_exp    Output exponent
 * @param[in]  b        64-bit input mantissa
 * @param[in]  b_exp    Input exponent
 * 
 * @returns     32-bit output mantissa
 */
int32_t xs3_scalar_s64_to_s32(
    exponent_t* a_exp,
    const int64_t b,
    const exponent_t b_exp);

/**
 * @brief Convert a 32-bit floating-point scalar to a 16-bit floating-point scalar.
 * 
 * Converts a 32-bit floating-point scalar, represented by the 32-bit mantissa `b` and exponent `b_exp`, into a 16-bit
 * floating-point scalar, represented by the 16-bit returned mantissa and output exponent `a_exp`.
 * 
 * @param[out] a_exp    Output exponent
 * @param[in]  b        32-bit input mantissa
 * @param[in]  b_exp    Input exponent
 * 
 * @returns     16-bit output mantissa
 */
int16_t xs3_scalar_s32_to_s16(
    exponent_t* a_exp,
    const int32_t b,
    const exponent_t b_exp);

/**
 * @brief Convert a 16-bit floating-point scalar to a 32-bit floating-point scalar.
 * 
 * Converts a 16-bit floating-point scalar, represented by the 16-bit mantissa `b` and exponent `b_exp`, into a 32-bit
 * floating-point scalar, represented by the 32-bit returned mantissa and output exponent `a_exp`.
 * 
 * `remove_hr`, if nonzero, indicates that the output mantissa should have no headroom. Otherwise, the output mantissa
 * will be the same as the input mantissa.
 * 
 * @param[out] a_exp        Output exponent
 * @param[in]  b            16-bit input mantissa
 * @param[in]  b_exp        Input exponent
 * @param[in]  remove_hr    Whether to remove headroom in output
 * 
 * @returns     32-bit output mantissa
 */
int32_t xs3_scalar_s16_to_s32(
    exponent_t* a_exp,
    const int16_t b,
    const exponent_t b_exp,
    const unsigned remove_hr);


#define XS3_SQRT_S32_MAX_DEPTH     (31)

/**
 * @brief Compute the square root of a 32-bit floating-point scalar.
 * 
 * @param[out] a_exp
 * @param[in]  B
 * @param[in]  b_exp
 * @param[in]  depth
 * 
 * @returns     
 */
int32_t xs3_sqrt_s32(
    exponent_t* a_exp,
    const int32_t B,
    const exponent_t b_exp,
    const unsigned depth);

/**
 * @brief Compute the inverse of a 32-bit floating-point scalar.
 * 
 * @param[out] a_exp
 * @param[in]  b
 * 
 * @returns     
 */
int32_t xs3_inverse_s32(
    exponent_t* a_exp,
    const int32_t b);

/**
 * @brief Compute the product of two 32-bit floating-point scalars.
 * 
 * @param[out] a_exp
 * @param[in]  b
 * @param[in]  c
 * @param[in]  b_exp
 * @param[in]  c_exp
 * 
 * @returns 
 */
int32_t xs3_mul_s32(
    exponent_t* a_exp,
    const int32_t b,
    const int32_t c,
    const exponent_t b_exp,
    const exponent_t c_exp);


/**
 * @brief Get the size of a number.
 * 
 * This function reports the size of the number as the number of bits required
 * to store unsigned integer `N`. This is equivalent to @f$ceil\left(log_2\left(N\right)\right)@f$.
 * 
 * @param[in] N     Number to get the size of
 * 
 * @returns     Number of bits required to store `N`
 */
static inline unsigned ceil_log2(
    unsigned N)
{
    if(N == 0)
        return 0;
    
    return 32-cls(N-1);
}
#ifdef __XC__
} // extern "C" 
#endif

#endif //XS3_UTIL_H_