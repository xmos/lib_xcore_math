

#ifndef XS3_UTIL_H_
#define XS3_UTIL_H_

#include "xs3_math_types.h"

#include <stdio.h>
#include <assert.h>
#include <math.h>

#define ASSERT(COND)    assert(COND)

#ifdef __XC__
extern "C" {
#endif

// Note: These are not safe from multiple evaluation of arguments
#define MAX(A,B) (((A) >= (B))? (A) : (B))
#define MIN(A,B) (((A) <= (B))? (A) : (B))

/**
 * Perform an arithmetic right-shift with a signed shift. i.e. negative values of `SHR` will left-shift.
 */ 
#define SIGNED_ASHR(VAL, SHR)  (((SHR) >= 0)? ((VAL) >> (SHR)) : ((VAL) << (-(SHR))))

/**
 * \brief Count leading sign bits of an `int16_t`.
 * 
 * \sa cls()
 */
#define CLS_S16(X)      (cls(X) - 16)

/**
 * \brief Count leading sign bits of an `int32_t`.
 * 
 * \sa cls()
 */
#define CLS_S32(X)      (cls(X))

/**
 * @todo
 * 
 * Might be a more efficient way to do this.
 */
#define CLS_S64(X)    ( (cls((int32_t)(((int64_t)(X))>>32)) == 32)?                                             \
                        (cls((int32_t)(((int64_t)(X))>>16)) == 32)?    32 + cls((int32_t)(X))                   \
                                                                  :    16 + cls((int32_t)(((int64_t)(X))>>16))  \
                                                                  :    cls((int32_t)(((int64_t)(X))>>32))   )





/**
 * \brief Count leading sign bits of a `complex_s16_t`.
 * 
 * The number of  leading sign bits for a complex integer is defined as 
 * the minimum of the number of leading sign bits for its real part and
 * for its imaginary part.
 * 
 * \sa cls()
 */
#define CLS_C16(X)      (MIN(CLS_S16((X).re), CLS_S16((X).im)))

/**
 * \brief Count leading sign bits of a `complex_s32_t`.
 * 
 * The number of  leading sign bits for a complex integer is defined as 
 * the minimum of the number of leading sign bits for its real part and
 * for its imaginary part.
 * 
 * \sa cls()
 */
#define CLS_C32(X)      (MIN(CLS_S32(((int32_t)(X).re)), CLS_S32(((int32_t)(X).im))))


#define HR_S64(X)   (CLS_S64(X)-1)

/**
 * \brief Get the headroom of an `int32_t`.
 */
#define HR_S32(X)   (CLS_S32(X)-1)

/**
 * \brief Get the headroom of an `int32_t`.
 */
#define HR_S16(X)   (CLS_S16(((int16_t)X))-1)

/**
 * \brief Get the headroom of a `complex_s32_t`.
 */
#define HR_C32(X)   (CLS_C32(X)-1)

/**
 * \brief Get the headroom of a `complex_s16_t`.
 */
#define HR_C16(X)   (CLS_C16(X)-1)

/**
 * \brief Count leading sign bits of `int32_t`.
 * 
 * This function returns the number of most-significant bits
 * in `a` which are equal to its sign bit.
 * 
 * \param[in] a Input value
 * 
 * \return Number of leading sign bits
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

int32_t xs3_scalar_s64_to_s32(
    exponent_t* a_exp,
    const int64_t b,
    const exponent_t b_exp);

int16_t xs3_scalar_s32_to_s16(
    exponent_t* a_exp,
    const int32_t b,
    const exponent_t b_exp);
    
int32_t xs3_scalar_s16_to_s32(
    exponent_t* a_exp,
    const int16_t b,
    const exponent_t b_exp,
    const unsigned remove_hr);


#define XS3_SQRT_S32_MAX_DEPTH     (31)

int32_t xs3_sqrt_s32(
    exponent_t* a_exp,
    const int32_t B,
    const exponent_t b_exp,
    const unsigned depth);


int32_t xs3_inverse_s32(
    exponent_t* a_exp,
    const int32_t b);
    
int32_t xs3_mul_s32(
    exponent_t* a_exp,
    const int32_t b,
    const int32_t c,
    const exponent_t b_exp,
    const exponent_t c_exp);


/**
 * \brief Get the size of a number.
 * 
 * This function reports the size of the number as the number of bits required
 * to store unsigned integer `N`. This is equivalent to @f$ceil\left(log_2\left(N\right)\right)@f$.
 * 
 * \param N     Number to get the size of
 * 
 * \return Number of bits required to `N`
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