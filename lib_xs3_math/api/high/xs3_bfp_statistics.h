

#ifndef XS3_STATISTICS_H_
#define XS3_STATISTICS_H_

#include "xs3_math_types.h"

#ifdef __XC__
extern "C" {
#endif

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
int32_t bfp_abs_sum_s16(
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
int64_t bfp_abs_sum_s32(
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
int16_t bfp_mean_s16(
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
int32_t bfp_mean_s32(
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
int64_t bfp_energy_s16(
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
int64_t bfp_energy_s32(
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
int32_t bfp_rms_s16(
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
int32_t bfp_rms_s32(
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
int16_t bfp_max_s16(
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
int32_t bfp_max_s32(
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
int16_t bfp_min_s16(
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
int32_t bfp_min_s32(
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
unsigned bfp_argmax_s16(
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
unsigned bfp_argmax_s32(
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
unsigned bfp_argmin_s16(
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
unsigned bfp_argmin_s32(
    const bfp_s32_t* b);


#ifdef __XC__
}   //extern "C"
#endif

#endif //XS3_STATISTICS_H_