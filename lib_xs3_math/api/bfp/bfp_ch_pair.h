
#ifndef BFP_CH_PAIR_H_
#define BFP_CH_PAIR_H_

#include "xs3_math_types.h"


#ifdef __XC__
extern "C" {
#endif



/** Count the leading sign bits of a 16-bit channel pair BFP vector.
 * 
 * The number of leading sign bits of a sequence is the minimum of
 * that among its elements.
 *
 * \param a         input BFP vector
 * 
 * \returns    Number of leading sign bits of ``a``. 
 */
headroom_t bfp_ch_pair_s16_headroom(
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
headroom_t bfp_ch_pair_s32_headroom(
    bfp_ch_pair_s32_t* a);

    
/** Apply a left-shift to the elements of a 16-bit channel pair BFP vector.
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
 * \param shl   Number of bits to left shift
 */
void bfp_ch_pair_s16_shl(
    bfp_ch_pair_s16_t* a,
    const bfp_ch_pair_s16_t* b,
    const left_shift_t shl);
    
/** Apply a left-shift to the elements of a 32-bit channel pair BFP vector.
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
 * \safe_in_place{a,b}
 * 
 * \param a     Output BFP vector
 * \param b     Input BFP vector
 * \param shl   Number of bits to left shift
 */
void bfp_ch_pair_s32_shl(
    bfp_ch_pair_s32_t* a,
    const bfp_ch_pair_s32_t* b,
    const left_shift_t shl);

#ifdef __XC__
}   //extern "C"
#endif

#endif //BFP_CH_PAIR_H_