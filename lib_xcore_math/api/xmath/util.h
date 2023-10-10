// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include "xmath/types.h"

#include <stdio.h>
#include <assert.h>
#include <math.h>



/**
 * @defgroup util_macros  Miscellaneous Utility Functions and Macros
 */


#ifdef __XC__
extern "C" {
#endif


/**
 * @brief Takes the greater of arguments `A` and `B`, preferring `A` on equality.
 *
 * @note This is not safe from multiple evaluation of arguments.
 *
 * @param[in]   A   First input
 * @param[in]   B   Second input
 *
 * @returns  Maximum of the inputs.
 *
 * @ingroup util_macros
 */
#define MAX(A,B) (((A) >= (B))? (A) : (B))


/**
 * @brief Takes the lesser of arguments `A` and `B`, preferring `A` on equality.
 *
 * @note This is not safe from multiple evaluation of arguments.
 *
 * @param[in]   A   First input
 * @param[in]   B   Second input
 *
 * @returns  Minimum of the inputs.
 *
 * @ingroup util_macros
 */
#define MIN(A,B) (((A) <= (B))? (A) : (B))


/**
 * @brief Count leading sign bits of an `int8_t`.
 *
 * @param[in]   X   Input
 *
 * @returns Leading sign bits of `X`
 *
 * @ingroup util_macros
 */
#define CLS_S8(X)      (cls(X) - 24)


/**
 * @brief Count leading sign bits of an `int16_t`.
 *
 * @param[in]   X   Input
 *
 * @returns Leading sign bits of `X`
 *
 * @ingroup util_macros
 */
#define CLS_S16(X)      (cls(X) - 16)


/**
 * @brief Count leading sign bits of an `int32_t`.
 *
 * @param[in]   X   Input
 *
 * @returns Leading sign bits of `X`
 *
 * @ingroup util_macros
 */
#define CLS_S32(X)      (cls(X))


/**
 * @brief Count leading sign bits of an `int64_t`.
 *
 * @param[in]   X   Input
 *
 * @returns Leading sign bits of `X`
 *
 * @ingroup util_macros
 */
#define CLS_S64(X)    ( (cls((int32_t)(((int64_t)(X))>>32)) == 32)?                                             \
                        (cls((int32_t)(((int64_t)(X))>>16)) == 32)?    32 + cls((int32_t)(X))                   \
                                                                  :    16 + cls((int32_t)(((int64_t)(X))>>16))  \
                                                                  :    cls((int32_t)(((int64_t)(X))>>32))   )


/**
 * @brief Count leading sign bits of a `complex_s16_t`.
 *
 * The number of  leading sign bits for a complex integer is defined as the minimum of the number of
 * leading sign bits for its real part and for its imaginary part.
 *
 * @param[in] X   Input
 *
 * @returns Leading sign bits of `X`
 *
 * @ingroup util_macros
 */
#define CLS_C16(X)      (MIN(CLS_S16((X).re), CLS_S16((X).im)))

/**
 * @brief Count leading sign bits of a `complex_s32_t`.
 *
 * The number of  leading sign bits for a complex integer is defined as the minimum of the number of
 * leading sign bits for its real part and for its imaginary part.
 *
 * @param[in] X   Input
 *
 * @returns Leading sign bits of `X`
 *
 * @ingroup util_macros
 */
#define CLS_C32(X)      (MIN(CLS_S32(((int32_t)(X).re)), CLS_S32(((int32_t)(X).im))))


/**
 * @brief Get the headroom of an `int64_t`.
 *
 * @param[in] X   Input
 *
 * @returns Headroom of `X`
 *
 * @ingroup util_macros
 */
#define HR_S64(X)   (CLS_S64(X)-1)

/**
 * @brief Get the headroom of an `int32_t`.
 *
 * @param[in] X   Input
 *
 * @returns Headroom of `X`
 *
 * @ingroup util_macros
 */
#define HR_S32(X)   (CLS_S32(X)-1)

/**
 * @brief Get the headroom of an `int16_t`.
 *
 * @param[in] X   Input
 *
 * @returns Headroom of `X`
 *
 * @ingroup util_macros
 */
#define HR_S16(X)   (CLS_S16(((int16_t)X))-1)

/**
 * @brief Get the headroom of an `int8_t`.
 *
 * @param[in] X   Input
 *
 * @returns Headroom of `X`
 *
 * @ingroup util_macros
 */
#define HR_S8(X)   (CLS_S8(((int8_t)X))-1)

/**
 * @brief Get the headroom of a `complex_s32_t`.
 *
 * The headroom of a complex `N`-bit integer is the minimum of the headroom of each of its `N`-bit
 * real and imaginary parts.
 *
 * @param[in] X   Input
 *
 * @returns Headroom of `X`
 *
 * @ingroup util_macros
 */
#define HR_C32(X)   (CLS_C32(X)-1)

/**
 * @brief Get the headroom of a `complex_s16_t`.
 *
 * The headroom of a complex `N`-bit integer is the minimum of the headroom of each of its `N`-bit
 * real and imaginary parts.
 *
 * @param[in] X   Input
 *
 * @returns Headroom of `X`
 *
 * @ingroup util_macros
 */
#define HR_C16(X)   (CLS_C16(X)-1)


/**
 * @brief VPU-based memcpy implementation.
 *
 * Same as standard `memcpy()` except for an extra constraint that both `dst and `src` must be
 * word-aligned addresses.
 *
 * @param[out]  dst   Destination address
 * @param[in]   src   Source address
 * @param[in]   bytes Number of bytes to copy
 *
 * @ingroup util_macros
 */
void xs3_memcpy(
    void* dst,
    const void* src,
    unsigned bytes);


/**
 * @brief Count leading sign bits of `int32_t`.
 *
 * This function returns the number of most-significant bits in `a` which are equal to its sign bit.
 *
 * @note This is the total number of leading sign bits, not _redundant_ leading sign bits.
 *
 * @param[in] a Input value
 *
 * @returns Number of leading sign bits of `a`
 *
 * @ingroup util_macros
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
            int32_t mask = (1<<i);
            if((a | mask) != a) return 31-i;
        }
    }
    assert(0);
    return 0;

#endif //__XS3A__
}


/**
 * @brief Reverse the bits of an integer.
 *
 * This function returns takes an integer `index` and reverses the `bits` least-significant bits
 * to form a new integer which is returned. All more significant bits are ignored.
 *
 * This is useful for algorithms, such as the FFT, whose implementation requires reordering of
 * elements by reversing the bits of the indices.
 *
 * @param[in] index   Input value
 * @param[in] bits    The number of least-significant bits to reverse.
 *
 * @returns The `bits` LSb's of `index` reversed.
 *
 * @ingroup util_macros
 */
static inline unsigned n_bitrev(
    const unsigned index,
    const unsigned bits)
{
  unsigned rev_index = 0;
#ifdef __xcore__

  const unsigned shifted_index = index << (32 - bits);
  asm( "bitrev %0, %1" : "=r"(rev_index) : "r"(shifted_index) );

#else

  unsigned dex = index;
  
  for(unsigned i = 0; i < bits; i++, dex >>= 1){
      rev_index = ((rev_index<<1) | (dex & 0x1));
  }

#endif


  return rev_index;
}


#ifdef __XC__
} // extern "C"
#endif
