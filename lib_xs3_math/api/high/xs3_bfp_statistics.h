

#ifndef XS3_STATISTICS_H_
#define XS3_STATISTICS_H_

#include "xs3_math_types.h"

#ifdef __XC__
extern "C" {
#endif

/** Calculate the absolute sum of a 16-bit BFP vector.
 * 
 * Conceptually, the operation performed is:
 *      A <-  ( |B[0]| + |B[1]| + ... + |B[N-1]| )
 *        where A is a scalar
 *              B[] is a BFP vector of size N
 * 
 * \param a         Output. Mantissa of result
 * \param a_exp     Output. Exponent of result
 * \param b         Input BFP vector
 */
float bfp_abs_sum_s16(
    const bfp_s16_t* b);

/** Calculate the absolute sum of a 32-bit BFP vector.
 * 
 * Conceptually, the operation performed is:
 *      A <-  ( |B[0]| + |B[1]| + ... + |B[N-1]| )
 *        where A is a scalar
 *              B[] is a BFP vector of size N
 * 
 * \param a         Output. Mantissa of result
 * \param a_exp     Output. Exponent of result
 * \param b         Input BFP vector
 */
float bfp_abs_sum_s32(
    const bfp_s32_t* b);

/** Calculate the mean of a 16-bit BFP vector.
 * 
 * Conceptually, the operation performed is:
 *      A <-  ( B[0] + B[1] + ... + B[N-1] ) / N
 *        where A is a scalar
 *              B[] is a BFP vector of size N
 * 
 * \param a         Output. Mantissa of result
 * \param a_exp     Output. Exponent of result
 * \param b         Input BFP vector
 */
float bfp_mean_s16(
    const bfp_s16_t* b);

/** Calculate the mean of a 32-bit BFP vector.
 * 
 * Conceptually, the operation performed is:
 *      A <-  ( B[0] + B[1] + ... + B[N-1] ) / N
 *        where A is a scalar
 *              B[] is a BFP vector of size N
 * 
 * \param a         Output. Mantissa of result
 * \param a_exp     Output. Exponent of result
 * \param b         Input BFP vector
 */
float bfp_mean_s32(
    const bfp_s32_t* b);

/** Calculate the power of a 16-bit BFP vector.
 * 
 * Conceptually, the operation performed is:
 *      A <-  ( (B[0])^2 + (B[1])^2 + ... + (B[N-1])^2 )
 *        where A is a scalar
 *              B[] is a BFP vector of size N
 * 
 * \param a         Output. Mantissa of result
 * \param a_exp     Output. Exponent of result
 * \param b         Input BFP vector
 */
float bfp_energy_s16(
    const bfp_s16_t* b);

/** Calculate the power of a 32-bit BFP vector.
 * 
 * Conceptually, the operation performed is:
 *      A <-  ( (B[0])^2 + (B[1])^2 + ... + (B[N-1])^2 )
 *        where A is a scalar
 *              B[] is a BFP vector of size N
 * 
 * \param a         Output. Mantissa of result
 * \param a_exp     Output. Exponent of result
 * \param b         Input BFP vector
 */
float bfp_energy_s32(
    const bfp_s32_t* b);

/** Calculate the RMS of a 16-bit BFP vector.
 * 
 * Conceptually, the operation performed is:
 *      A <-  sqrt( ((B[0])^2 + (B[1])^2 + ... + (B[N-1])^2) / N )
 *        where A is a scalar
 *              B[] is a BFP vector of size N
 * 
 * \param a         Output. Mantissa of result
 * \param a_exp     Output. Exponent of result
 * \param b         Input BFP vector
 */
float bfp_rms_s16(
    const bfp_s16_t* b);

/** Calculate the RMS of a 32-bit BFP vector.
 * 
 * Conceptually, the operation performed is:
 *      A <-  sqrt( ((B[0])^2 + (B[1])^2 + ... + (B[N-1])^2) / N )
 *        where A is a scalar
 *              B[] is a BFP vector of size N
 * 
 * \param a         Output. Mantissa of result
 * \param a_exp     Output. Exponent of result
 * \param b         Input BFP vector
 */
float bfp_rms_s32(
    const bfp_s32_t* b);


/** Find the maximum value in a 16-bit integer vector.
 * 
 * Conceptually, the operation performed is:
 *      result <- max( A[0], A[1], A[N-1] )
 *        where A[] is an integer vector of size N
 * 
 * \param a         Input vector
 * \param length    Length of the vector ``a``
 * 
 * \returns         Maximum value in ``a``.
 */
float bfp_max_s16(
    const bfp_s16_t* b);

/** Find the maximum value in a 32-bit integer vector.
 * 
 * Conceptually, the operation performed is:
 *      result <- max( A[0], A[1], A[N-1] )
 *        where A[] is an integer vector of size N
 * 
 * \param a         Input vector
 * \param length    Length of the vector ``a``
 * 
 * \returns         Maximum value in ``a``.
 */
float bfp_max_s32(
    const bfp_s32_t* b);


/** Find the minimum value in a 16-bit integer vector.
 * 
 * Conceptually, the operation performed is:
 *      result <- min( A[0], A[1], A[N-1] )
 *        where A[] is an integer vector of size N
 * 
 * \param a         Input vector
 * \param length    Length of the vector ``a``
 * 
 * \returns         Minimum value in ``a``.
 */
float bfp_min_s16(
    const bfp_s16_t* b);

/** Find the minimum value in a 32-bit integer vector.
 * 
 * Conceptually, the operation performed is:
 *      result <- min( A[0], A[1], A[N-1] )
 *        where A[] is an integer vector of size N
 * 
 * \param a         Input vector
 * \param length    Length of the vector ``a``
 * 
 * \returns         Minimum value in ``a``.
 */
float bfp_min_s32(
    const bfp_s32_t* b);


/** Find the index of the maximum element of a 16-bit integer vector.
 * 
 * Conceptually, the operation performed is:
 *      result <- argmax[i]{ A[i] }
 *        where A[] is an integer vector of size N
 * 
 * \param a         Input vector
 * \param length    Length of the vector ``a``
 * 
 * \returns         Index of the maximum element of ``a``.
 */
unsigned bfp_argmax_s16(
    const bfp_s16_t* b);

/** Find the index of the maximum element of a 32-bit integer vector.
 * 
 * Conceptually, the operation performed is:
 *      result <- argmax[i]{ A[i] }
 *        where A[] is an integer vector of size N
 * 
 * \param a         Input vector
 * \param length    Length of the vector ``a``
 * 
 * \returns         Index of the maximum element of ``a``.
 */
unsigned bfp_argmax_s32(
    const bfp_s32_t* b);

/** Find the index of the minimum element of a 16-bit integer vector.
 * 
 * Conceptually, the operation performed is:
 *      result <- argmin[i]{ A[i] }
 *        where A[] is an integer vector of size N
 * 
 * \param a         Input vector
 * \param length    Length of the vector ``a``
 * 
 * \returns         Index of the minimum element of ``a``.
 */
unsigned bfp_argmin_s16(
    const bfp_s16_t* b);

/** Find the index of the minimum element of a 32-bit integer vector.
 * 
 * Conceptually, the operation performed is:
 *      result <- argmin[i]{ A[i] }
 *        where A[] is an integer vector of size N
 * 
 * \param a         Input vector
 * \param length    Length of the vector ``a``
 * 
 * \returns         Index of the minimum element of ``a``.
 */
unsigned bfp_argmin_s32(
    const bfp_s32_t* b);


#ifdef __XC__
}   //extern "C"
#endif

#endif //XS3_STATISTICS_H_