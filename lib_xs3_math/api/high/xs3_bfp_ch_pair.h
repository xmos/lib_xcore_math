
#ifndef XS3_BFP_CH_PAIR_H_
#define XS3_BFP_CH_PAIR_H_

#include "xs3_math_types.h"


#ifdef __XC__
extern "C" {
#endif

/** Count the leading sign bits of a 16-bit channel pair.
 * 
 * The leading sign bits of a channel pair is the lesser of the leading sign
 * bits among its two channels.
 *
 * \param a    Input channel pair 
 * 
 * \returns    Number of leading sign bits of ``a``. 
 */
headroom_t xs3_cls_ch_pair_s16(
    const ch_pair_s16_t a);


/** Count the leading sign bits of a 32-bit channel pair.
 * 
 * The leading sign bits of a channel pair is the lesser of the leading sign
 * bits among its two channels.
 *
 * \param a    Input channel pair 
 * 
 * \returns    Number of leading sign bits of ``a``. 
 */
headroom_t xs3_cls_ch_pair_s32(
    const ch_pair_s32_t a);



/** Count the leading sign bits of a 16-bit channel pair BFP vector.
 * 
 * The number of leading sign bits of a sequence is the minimum of
 * that among its elements.
 *
 * \param a         input BFP vector
 * 
 * \returns    Number of leading sign bits of ``a``. 
 */
headroom_t bfp_cls_vect_ch_pair_s16(
    bfp_ch_pair_s16_t* a);

/** Count the leading sign bits of a 32-bit channel pair BFP vector.
 * 
 * The number of leading sign bits of a sequence is the minimum of
 * that among its elements.
 *
 * \param a         input BFP vector
 * 
 * \returns    Number of leading sign bits of ``a``. 
 */
headroom_t bfp_cls_vect_ch_pair_s32(
    bfp_ch_pair_s32_t* a);

    
/** Multiply a 16-bit BFP channel pair vector by a power of 2 by shifting mantissas.
 * 
 * Conceptually, the operation performed is:
 *      A[i] <- B[i] * 2^(shift)
 *        for each index i < length
 *        where A[] and B[] are channel pair vectors
 *              shift is an integer
 * 
 * This operation can be used to remove headroom from a BFP vector.
 * 
 * The operation saturates to 16-bit bounds.
 * 
 * It is safe to supply the same ``ch_pair_s16_t*`` for ``a`` and ``b``.
 * 
 * \param a     Output BFP vector
 * \param b     Input BFP vector
 * \param exp   Exponent
 */
void bfp_ldexp_vect_ch_pair_s16(
    bfp_ch_pair_s16_t* a,
    const bfp_ch_pair_s16_t* b,
    const exponent_t exp);
    
/** Multiply a 32-bit BFP channel pair vector by a power of 2 by shifting mantissas.
 * 
 * Conceptually, the operation performed is:
 *      A[i] <- B[i] * 2^(shift)
 *        for each index i < length
 *        where A[] and B[] are channel pair vectors
 *              shift is an integer
 * 
 * This operation can be used to remove headroom from a BFP vector.
 * 
 * The operation saturates to 32-bit bounds.
 * 
 * It is safe to supply the same ``ch_pair_s32_t*`` for ``a`` and ``b``.
 * 
 * \param a     Output BFP vector
 * \param b     Input BFP vector
 * \param exp   Exponent
 */
void bfp_ldexp_vect_ch_pair_s32(
    bfp_ch_pair_s32_t* a,
    const bfp_ch_pair_s32_t* b,
    const exponent_t exp);

#ifdef __XC__
}   //extern "C"
#endif

#endif //XS3_BFP_CH_PAIR_H_