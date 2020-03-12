
#ifndef XS3_BFP_STATISTICS_LOW_H_
#define XS3_BFP_STATISTICS_LOW_H_

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
 * \param[in] b         Input vector
 * \param[in] length    Length of input vector
 * \param[in] b_shr     Right-shift applied to elements of `b`
 * 
 * \return  Absolute sum
 */
int16_t xs3_abs_sum_s16(
    const int16_t* b,
    const unsigned length,
    const int b_shr);

/** Calculate the absolute sum of a 32-bit BFP vector.
 * 
 * Conceptually, the operation performed is:
 *      A <-  ( |B[0]| + |B[1]| + ... + |B[N-1]| )
 *        where A is a scalar
 *              B[] is a BFP vector of size N
 * 
 * \param[in] b         Input vector
 * \param[in] length    Length of input vector
 * \param[in] b_shr     Right-shift applied to elements of `b`
 * 
 * \return  Absolute sum
 */
int32_t xs3_abs_sum_s32(
    const int32_t* b,
    const unsigned length,
    const int b_shr);

/** Calculate the mean of a 16-bit BFP vector.
 * 
 * Conceptually, the operation performed is:
 *      A <-  ( B[0] + B[1] + ... + B[N-1] ) / N
 *        where A is a scalar
 *              B[] is a BFP vector of size N
 * 
 * \param[in] b         Input vector
 * \param[in] length    Length of input vector
 * \param[in] b_shr     Right-shift applied to elements of `b`
 * 
 * \return  Mean
 */
int16_t xs3_mean_s16(
    const int16_t* b,
    const unsigned length,
    const int b_shr);

/** Calculate the mean of a 32-bit BFP vector.
 * 
 * Conceptually, the operation performed is:
 *      A <-  ( B[0] + B[1] + ... + B[N-1] ) / N
 *        where A is a scalar
 *              B[] is a BFP vector of size N
 * 
 * \param[in] b         Input vector
 * \param[in] length    Length of input vector
 * \param[in] b_shr     Right-shift applied to elements of `b`
 * 
 * \return  Mean
 */
int32_t xs3_mean_s32(
    const int32_t* b,
    const unsigned length,
    const int b_shr);

/** Calculate the energy of a 16-bit BFP vector.
 * 
 * Conceptually, the operation performed is:
 *      A <-  ( (B[0])^2 + (B[1])^2 + ... + (B[N-1])^2 )
 *        where A is a scalar
 *              B[] is a BFP vector of size N
 * 
 * \param[in] b         Input vector
 * \param[in] length    Length of input vector
 * \param[in] b_shr     Right-shift applied to elements of `b`
 * 
 * \return  Energy
 */
int16_t xs3_energy_s16(
    const int16_t* b,
    const unsigned length,
    const int b_shr);

/** Calculate the energy of a 32-bit BFP vector.
 * 
 * Conceptually, the operation performed is:
 *      A <-  ( (B[0])^2 + (B[1])^2 + ... + (B[N-1])^2 )
 *        where A is a scalar
 *              B[] is a BFP vector of size N
 * 
 * \param[in] b         Input vector
 * \param[in] length    Length of input vector
 * \param[in] b_shr     Right-shift applied to elements of `b`
 * 
 * \return  Energy
 */
int32_t xs3_energy_s32(
    const int32_t* b,
    const unsigned length,
    const int b_shr);

/** Calculate the RMS of a 16-bit BFP vector.
 * 
 * Conceptually, the operation performed is:
 *      A <-  sqrt( ((B[0])^2 + (B[1])^2 + ... + (B[N-1])^2) / N )
 *        where A is a scalar
 *              B[] is a BFP vector of size N
 * 
 * \param[in] b         Input vector
 * \param[in] length    Length of input vector
 * \param[in] b_shr     Right-shift applied to elements of `b`
 * 
 * \return  RMS
 */
int16_t xs3_rms_s16(
    const int16_t* b,
    const unsigned length,
    const int b_shr);

/** Calculate the RMS of a 32-bit BFP vector.
 * 
 * Conceptually, the operation performed is:
 *      A <-  sqrt( ((B[0])^2 + (B[1])^2 + ... + (B[N-1])^2) / N )
 *        where A is a scalar
 *              B[] is a BFP vector of size N
 * 
 * \param[in] b         Input vector
 * \param[in] length    Length of input vector
 * \param[in] b_shr     Right-shift applied to elements of `b`
 * 
 * \return  RMS
 */
int32_t xs3_rms_s32(
    const int32_t* b,
    const unsigned length,
    const int b_shr);


/** Find the maximum value in a 16-bit integer vector.
 * 
 * Conceptually, the operation performed is:
 *      result <- max( A[0], A[1], A[N-1] )
 *        where A[] is an integer vector of size N
 * 
 * \param[in] a         Input vector
 * \param[in] length    Length of input vector
 * 
 * \return  Maximum value of `a`
 */
int16_t xs3_max_s16(
    const int16_t a[], 
    const unsigned length);

/** Find the maximum value in a 32-bit integer vector.
 * 
 * Conceptually, the operation performed is:
 *      result <- max( A[0], A[1], A[N-1] )
 *        where A[] is an integer vector of size N
 * 
 * \param[in] a         Input vector
 * \param[in] length    Length of input vector
 * 
 * \return  Maximum value of `a`
 */
int32_t xs3_max_s32(
    const int32_t a[],
    const unsigned length);


/** Find the minimum value in a 16-bit integer vector.
 * 
 * Conceptually, the operation performed is:
 *      result <- min( A[0], A[1], A[N-1] )
 *        where A[] is an integer vector of size N
 * 
 * \param[in] a         Input vector
 * \param[in] length    Length of input vector
 * 
 * \return  Minimum value of `a`
 */
int16_t xs3_min_s16(
    const int16_t a[], 
    const unsigned length);

/** Find the minimum value in a 32-bit integer vector.
 * 
 * Conceptually, the operation performed is:
 *      result <- min( A[0], A[1], A[N-1] )
 *        where A[] is an integer vector of size N
 * 
 * \param[in] a         Input vector
 * \param[in] length    Length of input vector
 * 
 * \return  Minimum value of `a`
 */
int32_t xs3_min_s32(
    const int32_t a[],
    const unsigned length);


/** Find the index of the maximum element of a 16-bit integer vector.
 * 
 * Conceptually, the operation performed is:
 *      result <- argmax[i]{ A[i] }
 *        where A[] is an integer vector of size N
 * 
 * \param a         Input vector
 * \param length    Length of the vector `a`
 * 
 * \return  Index of the maximum element of `a`.
 */
unsigned xs3_argmax_s16(
    const int16_t a[], 
    const unsigned length);

/** Find the index of the maximum element of a 32-bit integer vector.
 * 
 * Conceptually, the operation performed is:
 *      result <- argmax[i]{ A[i] }
 *        where A[] is an integer vector of size N
 * 
 * \param a         Input vector
 * \param length    Length of the vector `a`
 * 
 * \return         Index of the maximum element of `a`
 */
unsigned xs3_argmax_s32(
    const int32_t a[],
    const unsigned length);

/** Find the index of the minimum element of a 16-bit integer vector.
 * 
 * Conceptually, the operation performed is:
 *      result <- argmin[i]{ A[i] }
 *        where A[] is an integer vector of size N
 * 
 * \param a         Input vector
 * \param length    Length of the vector `a`
 *  
 * \return  Index of the minimum element of `a`.
 */
unsigned xs3_argmin_s16(
    const int16_t a[], 
    const unsigned length);

/** Find the index of the minimum element of a 32-bit integer vector.
 * 
 * Conceptually, the operation performed is:
 *      result <- argmin[i]{ A[i] }
 *        where A[] is an integer vector of size N
 * 
 * \param a         Input vector
 * \param length    Length of the vector `a`
 * 
 * \return         Index of the minimum element of `a`.
 */
unsigned xs3_argmin_s32(
    const int32_t a[],
    const unsigned length);



#ifdef __XC__
}   //extern "C"
#endif

#endif //XS3_BFP_STATISTICS_LOW_H_