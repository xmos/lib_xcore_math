
#ifndef XS3_BFP_COMPLEX_H_
#define XS3_BFP_COMPLEX_H_

#include "xs3_math_types.h"

#ifdef __XC__
extern "C" {
#endif




/** Count the leading sign bits of a 16-bit complex BFP vector.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * The number of leading sign bits of a sequence is the minimum of
 * that among its elements.
 * 
 * Updates ``a->hr``.
 *
 * \param a         input BFP vector
 * 
 * \returns    Number of leading sign bits of ``a``. 
 */
headroom_t bfp_vect_complex_s16_headroom(
    bfp_complex_s16_t* a);


/** Count the leading sign bits of a 32-bit complex BFP vector.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * The number of leading sign bits of a sequence is the minimum of
 * that among its elements.
 *
 * Updates ``a->hr``.
 * 
 * \param a         input BFP vector
 * 
 * \returns    Number of leading sign bits of ``a``. 
 */
headroom_t bfp_vect_complex_s32_headroom(
    bfp_complex_s32_t* a);



/** Apply a left-shift to the elements of a complex 16-bit BFP vector.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Conceptually, the operation performed is:
 *      A[i] <- B[i] * 2^(shift)
 *        for each index i < length
 *        where A[] and B[] are complex BFP vectors
 *              shift is an integer
 * 
 * This operation can be used to remove headroom from a BFP vector.
 * 
 * The operation saturates to 16-bit bounds.
 * 
 * \safe_in_place{a,b}
 * 
 * \param a     Output BFP vector
 * \param b     Input BFP vector
 * \param shl   Number of bits to left shift
 */
void bfp_vect_complex_s16_shl(
    bfp_complex_s16_t* a,
    const bfp_complex_s16_t* b,
    const left_shift_t shl);
    
/** Apply a left-shift to the elements of a complex 32-bit BFP vector.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Conceptually, the operation performed is:
 *      A[i] <- B[i] * 2^(shift)
 *        for each index i < length
 *        where A[] and B[] are complex BFP vectors
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
void bfp_vect_complex_s32_shl(
    bfp_complex_s32_t* a,
    const bfp_complex_s32_t* b,
    const left_shift_t shl);




/** Multiply a 16-bit complex BFP vector by a 16-bit real BFP vector.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Conceptually, the operation performed is:
 *      A[i] <- B[i] * C[i]
 *        for each index i in B[]
 *        where A[] and B[] are complex vectors
 *              C[] is a real vector
 * 
 * This operation saturates to 16-bit bounds.
 * 
 * ``a->data`` must already be initialized to a valid memory buffer.
 * It is safe to supply the same ``bfp_complex_s16_t*`` for ``a`` and ``b``.
 * 
 * \param a     Output BFP vector
 * \param b     Input complex BFP vector
 * \param c     Input real BFP vector
 */
void bfp_vect_complex_s16_mul(
    bfp_complex_s16_t* a, 
    const bfp_complex_s16_t* b, 
    const bfp_s16_t* c);

/** Multiply a 32-bit complex BFP vector by a 32-bit real BFP vector.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Conceptually, the operation performed is:
 *      A[i] <- B[i] * C[i]
 *        for each index i in B[]
 *        where A[] and B[] are complex vectors
 *              C[] is a real vector
 * 
 * This operation saturates to 32-bit bounds.
 * 
 * ``a->data`` must already be initialized to a valid memory buffer.
 * It is safe to supply the same ``bfp_complex_s32_t*`` for ``a`` and ``b``.
 * 
 * \param a     Output BFP vector
 * \param b     Input complex BFP vector
 * \param c     Input real BFP vector
 */
void bfp_vect_complex_s32_mul(
    bfp_complex_s32_t* a, 
    const bfp_complex_s32_t* b, 
    const bfp_s32_t* c);



/** Multiply a 16-bit complex BFP vector by another 16-bit complex BFP vector.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Conceptually, the operation performed is:
 *      A[i] <- B[i] * C[i]
 *        for each index i in B[]
 *        where A[], B[] and C[] are complex vectors.
 * 
 * This operation saturates to 16-bit bounds.
 * 
 * ``a->data`` must already be initialized to a valid memory buffer.
 * It is safe to supply the same ``bfp_complex_s16_t*`` for any of the inputs.
 * 
 * \param a     Output BFP vector
 * \param b     Input complex BFP vector 1
 * \param c     Input complex BFP vector 2
 */
void bfp_vect_complex_s16_complex_mul(
    bfp_complex_s16_t* a, 
    const bfp_complex_s16_t* b, 
    const bfp_complex_s16_t* c);





/** Multiply a 32-bit complex BFP vector by another 32-bit complex BFP vector.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Conceptually, the operation performed is:
 *      A[i] <- B[i] * C[i]
 *        for each index i in B[]
 *        where A[], B[] and C[] are complex vectors.
 * 
 * This operation saturates to 32-bit bounds.
 * 
 * ``a->data`` must already be initialized to a valid memory buffer.
 * It is safe to supply the same ``bfp_complex_s32_t*`` for any of the inputs.
 * 
 * \param a     Output BFP vector
 * \param b     Input complex BFP vector 1
 * \param c     Input complex BFP vector 2
 */
void bfp_vect_complex_s32_complex_mul(
    bfp_complex_s32_t* a, 
    const bfp_complex_s32_t* b, 
    const bfp_complex_s32_t* c);

/** Conjugate multiply a 16-bit complex BFP vector by another 16-bit complex BFP vector.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Conceptually, the operation performed is:
 *      A[i] <- B[i] * conjugate(C[i])
 *        for each index i in B[]
 *        where A[], B[] and C[] are complex vectors.
 * 
 * This operation saturates to 16-bit bounds.
 * 
 * ``a->data`` must already be initialized to a valid memory buffer.
 * It is safe to supply the same ``bfp_complex_s16_t*`` for any of the inputs.
 * 
 * \param a     Output complex BFP vector
 * \param b     Input complex BFP vector 1
 * \param c     Input complex BFP vector 2
 */
void bfp_vect_complex_s16_complex_conj_mul(
    bfp_complex_s16_t* a, 
    const bfp_complex_s16_t* b, 
    const bfp_complex_s16_t* c);

/** Conjugate multiply a 32-bit complex BFP vector by another 32-bit complex BFP vector.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Conceptually, the operation performed is:
 *      A[i] <- B[i] * conjugate(C[i])
 *        for each index i in B[]
 *        where A[], B[] and C[] are complex vectors.
 * 
 * This operation saturates to 32-bit bounds.
 * 
 * ``a->data`` must already be initialized to a valid memory buffer.
 * It is safe to supply the same ``bfp_complex_s32_t*`` for any of the inputs.
 * 
 * \param a     Output complex BFP vector
 * \param b     Input complex BFP vector 1
 * \param c     Input complex BFP vector 2
 */
void bfp_vect_complex_s32_complex_conj_mul(
    bfp_complex_s32_t* a, 
    const bfp_complex_s32_t* b, 
    const bfp_complex_s32_t* c);

/** Multiply a 16-bit complex BFP vector by a 16-bit real scalar.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Conceptually, the operation performed is:
 *      A[] <- B[] * C
 *        where A[] and B[] are complex vectors
 *              C is a real scalar
 * 
 * This operation saturates to 16-bit bounds.
 * 
 * ``a->data`` must already be initialized to a valid memory buffer.
 * ``a`` and ``b`` may safely point to the same complex BFP vector.
 * 
 * \param a         Output complex BFP vector
 * \param b         Input complex BFP vector
 * \param c         Real scalar input
 */
void bfp_vect_complex_s16_scalar_mul(
    bfp_complex_s16_t* a, 
    const bfp_complex_s16_t* b, 
    const int16_t alpha_mant,
    const exponent_t alpha_exp);

/** Multiply a 32-bit complex BFP vector by a 32-bit real scalar.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Conceptually, the operation performed is:
 *      A[] <- B[] * C
 *        where A[] and B[] are complex vectors
 *              C is a real scalar
 * 
 * This operation saturates to 32-bit bounds.
 * 
 * ``a->data`` must already be initialized to a valid memory buffer.
 * ``a`` and ``b`` may safely point to the same complex BFP vector.
 * 
 * \param a         Output complex BFP vector
 * \param b         Input complex BFP vector
 * \param c         Real scalar input
 */
void bfp_vect_complex_s32_scalar_mul(
    bfp_complex_s32_t* a, 
    const bfp_complex_s32_t* b, 
    const int32_t alpha_mant,
    const exponent_t alpha_exp);

/** Multiply a 16-bit complex BFP vector by a 16-bit complex scalar.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Conceptually, the operation performed is:
 *      A[] <- B[] * C
 *        where A[] and B[] are a complex vectors
 *              C is a complex scalar
 * 
 * This operation saturates to 16-bit bounds.
 * 
 * ``a->data`` must already be initialized to a valid memory buffer.
 * ``a`` and ``b`` may safely point to the same complex BFP vector.
 * 
 * \param a         Output complex BFP vector
 * \param b         Input complex BFP vector
 * \param c         Complex scalar input
 */
void bfp_vect_complex_s16_complex_scal_mul(
    bfp_complex_s16_t* a, 
    const bfp_complex_s16_t* b, 
    const complex_s16_t alpha_mant,
    const exponent_t alpha_exp);

/** Multiply a 32-bit complex BFP vector by a 32-bit complex scalar.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Conceptually, the operation performed is:
 *      A[] <- B[] * C
 *        where A[] and B[] are a complex vectors
 *              C is a complex scalar
 * 
 * This operation saturates to 32-bit bounds.
 * 
 * ``a->data`` must already be initialized to a valid memory buffer.
 * ``a`` and ``b`` may safely point to the same complex BFP vector.
 * 
 * \param a         Output complex BFP vector
 * \param b         Input complex BFP vector
 * \param c         Complex scalar input
 */
void bfp_vect_complex_s32_complex_scal_mul(
    bfp_complex_s32_t* a, 
    const bfp_complex_s32_t* b, 
    const complex_s32_t alpha_mant,
    const exponent_t alpha_exp);

/** Add one 16-bit complex BFP vector to another element-wise.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Conceptually, the operation performed is:
 *      A[] <- B[] + C[]
 *        where A[], B[] and C[] are complex vectors.
 * 
 * This operation saturates to 16-bit bounds.
 * 
 * ``a->data`` must already be initialized to a valid memory buffer.
 * Any of ``a``, ``b``, and ``c`` may safely point to the same complex BFP vector.
 * 
 * \param a         Output complex BFP vector
 * \param b         Input complex BFP vector 1
 * \param c         Input complex BFP vector 2
 */
void bfp_vect_complex_s16_add(
    bfp_complex_s16_t* a, 
    const bfp_complex_s16_t* b, 
    const bfp_complex_s16_t* c);

/** Add one 32-bit complex BFP vector to another element-wise.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Conceptually, the operation performed is:
 *      A[] <- B[] + C[]
 *        where A[], B[] and C[] are complex vectors.
 * 
 * This operation saturates to 32-bit bounds.
 * 
 * ``a->data`` must already be initialized to a valid memory buffer.
 * Any of ``a``, ``b``, and ``c`` may safely point to the same complex BFP vector.
 * 
 * \param a         Output complex BFP vector
 * \param b         Input complex BFP vector 1
 * \param c         Input complex BFP vector 2
 */
void bfp_vect_complex_s32_add(
    bfp_complex_s32_t* a, 
    const bfp_complex_s32_t* b, 
    const bfp_complex_s32_t* c);

/** Subtract one 16-bit complex BFP vector from another element-wise.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Conceptually, the operation performed is:
 *      A[] <- B[] - C[]
 *        where A[], B[] and C[] are complex vectors.
 * 
 * This operation saturates to 16-bit bounds.
 * 
 * ``a->data`` must already be initialized to a valid memory buffer.
 * Any of ``a``, ``b``, and ``c`` may safely point to the same complex BFP vector.
 * 
 * \param a         Output BFP vector
 * \param b         Input operand 1
 * \param c         Input operand 2
 */
void bfp_vect_complex_s16_sub(
    bfp_complex_s16_t* a, 
    const bfp_complex_s16_t* b, 
    const bfp_complex_s16_t* c);

/** Subtract one 32-bit complex BFP vector from another element-wise.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Conceptually, the operation performed is:
 *      A[] <- B[] - C[]
 *        where A[], B[] and C[] are complex vectors.
 * 
 * This operation saturates to 32-bit bounds.
 * 
 * ``a->data`` must already be initialized to a valid memory buffer.
 * Any of ``a``, ``b``, and ``c`` may safely point to the same complex BFP vector.
 * 
 * \param a         Output BFP vector
 * \param b         Input operand 1
 * \param c         Input operand 2
 */
void bfp_vect_complex_s32_sub(
    bfp_complex_s32_t* a, 
    const bfp_complex_s32_t* b,
    const bfp_complex_s32_t* c);

/** Convert a 16-bit complex BFP vector to a 32-bit complex BFP vector.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Conceptually, the operation performed is:
 *      A[] <- B[]
 *        where A[] is a 32-bit complex vector
 *              B[] is a 16-bit complex vector
 * 
 * No precision is lost in this operation.
 * 
 * ``a->data`` must already be initialized to a valid memory buffer.
 * 
 * \param a         Output 32-bit complex BFP vector
 * \param b         Input 16-bit complex BFP vector
 */
void bfp_vect_complex_s16_to_complex_s32(
    bfp_complex_s32_t* a, 
    const bfp_complex_s16_t* b);

/** Convert a 32-bit complex BFP vector to a 16-bit complex BFP vector.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Conceptually, the operation performed is:
 *      A[] <- B[]
 *        where A[] is a 16-bit complex vector
 *              B[] is a 32-bit complex vector
 * 
 * To preserve as much precision as possible, headroom is removed from ``b`` as the conversion occurs.
 * 
 * Each element's real and imaginary parts are rounded to their nearest representable 16-bit values.
 * 
 * ``a->data`` must already be initialized to a valid memory buffer.
 * 
 * \param a         Output 16-bit complex BFP vector
 * \param b         Input 32-bit complex BFP vector
 */
void bfp_vect_complex_s32_to_complex_s16(
    bfp_complex_s16_t* a, 
    const bfp_complex_s32_t* b);

    

/** Compute the squared magnitude of each element of a 16-bit complex BFP vector.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Conceptually, the operation performed is:
 *      A[i] <- ( (B[i].re)^2 + (B[i].im)^2 )
 *        for each index i of B[]
 *        where A[] is a real vector
 *              B[] is a complex vector
 * 
 * ``a->data`` must already be initialized to a valid memory buffer.
 * 
 * \param a     Output real BFP vector
 * \param b     Input complex BFP vector
 */
void bfp_vect_complex_s16_squared_mag(
    bfp_s16_t* a, 
    const bfp_complex_s16_t* b);

    

/** Compute the squared magnitude of each element of a 32-bit complex BFP vector.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Conceptually, the operation performed is:
 *      A[i] <- ( (B[i].re)^2 + (B[i].im)^2 )
 *        for each index i of B[]
 *        where A[] is a real vector
 *              B[] is a complex vector
 * 
 * ``a->data`` must already be initialized to a valid memory buffer.
 * 
 * \param a     Output real BFP vector
 * \param b     Input complex BFP vector
 */
void bfp_vect_complex_s32_squared_mag(
    bfp_s32_t* a, 
    const bfp_complex_s32_t* b);

/** Compute the magnitude of each element of a 16-bit complex BFP vector.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Conceptually, the operation performed is:
 *      A[i] <- sqrt( (B[i].re)^2 + (B[i].im)^2 )
 *        for each index i of B[]
 *        where A[] is a real vector
 *              B[] is a complex vector
 * 
 * ``a->data`` must already be initialized to a valid memory buffer.
 * 
 * \param a     Output real BFP vector
 * \param b     Input complex BFP vector
 */
void bfp_vect_complex_s16_mag(
    bfp_s16_t* a, 
    const bfp_complex_s16_t* b);

/** Compute the magnitude of each element of a 32-bit complex BFP vector.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Conceptually, the operation performed is:
 *      A[i] <- sqrt( (B[i].re)^2 + (B[i].im)^2 )
 *        for each index i of B[]
 *        where A[] is a real vector
 *              B[] is a complex vector
 * 
 * ``a->data`` must already be initialized to a valid memory buffer.
 * 
 * \param a     Output real BFP vector
 * \param b     Input complex BFP vector
 */
void bfp_vect_complex_s32_mag(
    bfp_s32_t* a, 
    const bfp_complex_s32_t* b);

/** Sum the elements of a 16-bit complex BFP vector.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Conceptually, the operation performed is:
 *      A <- sum{ B[i] }
 *        for each index i of B[]
 *        where A is a complex scalar
 *              B[] is a complex vector
 * 
 * \param b     Input complex BFP vector.
 * 
 * \returns     Complex sum of vector elements.
 */
complex_s32_t bfp_vect_complex_s16_sum(
    const bfp_complex_s16_t* b);

/** Sum the elements of a 32-bit complex BFP vector.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Conceptually, the operation performed is:
 *      A <- sum{ B[i] }
 *        for each index i of B[]
 *        where A is a complex scalar
 *              B[] is a complex vector
 * 
 * \param b     Input complex BFP vector.
 * 
 * \returns     Complex sum of vector elements
 */
complex_s64_t bfp_vect_complex_s32_sum( 
    exponent_t* a_exp,
    const bfp_complex_s32_t* b);


#ifdef __XC__
}   //extern "C"
#endif

#endif //XS3_BFP_COMPLEX_H_