
#ifndef XS3_BFP_COMPLEX_LOW_H_
#define XS3_BFP_COMPLEX_LOW_H_

#include "xs3_math_types.h"

#ifdef __XC__
extern "C" {
#endif


/**
 * Returns headroom
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * astew: Is this actually doable with the VPU?? Each element of b is 8 bytes (real, imag), where
 *        each element of c is 4 bytes.
 */
headroom_t xs3_mul_vect_complex_s16(
    int16_t* a_real,
    int16_t* a_imag,
    const int16_t* b_real,
    const int16_t* b_imag,
    const int16_t* c,
    const unsigned length,
    const right_shift_t sat);


/**
 * Returns headroom
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * astew: Is this actually doable with the VPU?? Each element of b is 8 bytes (real, imag), where
 *        each element of c is 4 bytes.
 */
headroom_t xs3_mul_vect_complex_s32(
    complex_s32_t* a,
    const complex_s32_t* b,
    const int32_t* c,
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * Returns headroom
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See @ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * @safe_in_place{a,b}
 * 
 * @note Intermediate saturation may occur unexpectedly `b_imag[k] == -0x8000` for any `k` in vector `b`. (As elements
 *       of `b_imag[]` are multiplied by `-1`)
 * 
 * @note It is _not_ safe to operate in-place on vector `c`.
 * 
 */
headroom_t xs3_complex_mul_vect_complex_s16(
    int16_t* a_real,
    int16_t* a_imag,
    const int16_t* b_real,
    const int16_t* b_imag,
    const int16_t* c_real,
    const int16_t* c_imag,
    const unsigned length,
    const right_shift_t sat);


/**
 * Returns headroom
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 */
headroom_t xs3_complex_mul_vect_complex_s32(
    complex_s32_t* a,
    const complex_s32_t* b,
    const complex_s32_t* c,
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * Returns headroom
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * astew: Is this actually doable with the VPU?? The VCMI and VCMR instructions don't work in 16-bit mode!
 * 
 * Maybe what is needed for 16-bit complex BFP is *separate* buffers for the real and imaginary parts.
 */
headroom_t xs3_complex_conj_mul_vect_complex_s16(
    int16_t* a_real,
    int16_t* a_imag,
    const int16_t* b_real,
    const int16_t* b_imag,
    const int16_t* c_real,
    const int16_t* c_imag,
    const unsigned length,
    const right_shift_t sat);


/**
 * Returns headroom
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 */
headroom_t xs3_complex_conj_mul_vect_complex_s32(
    complex_s32_t* a,
    const complex_s32_t* b,
    const complex_s32_t* c,
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * Returns headroom
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * astew: Is this actually doable with the VPU?? The VCMI and VCMR instructions don't work in 16-bit mode!
 */
headroom_t xs3_complex_scal_mul_vect_complex_s16(
    int16_t* a_real,
    int16_t* a_imag,
    const int16_t* b_real,
    const int16_t* b_imag,
    const int16_t c_real,
    const int16_t c_imag,
    const unsigned length,
    const right_shift_t sat);


/**
 * Returns headroom
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * astew: Is this actually doable with the VPU?? The VCMI and VCMR instructions don't work in 16-bit mode!
 */
headroom_t xs3_complex_scal_mul_vect_complex_s32(
    complex_s32_t* a,
    const complex_s32_t* b,
    const int32_t c_real,
    const int32_t c_imag,
    const unsigned length,
    const right_shift_t b_shr);


/**
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 */
void xs3_complex_s16_to_complex_s32(
    complex_s32_t* a,
    const int16_t* b_real,
    const int16_t* b_imag,
    const unsigned length);

/**
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 */
void xs3_complex_s32_to_complex_s16(
    int16_t* a_real,
    int16_t* a_imag,
    const complex_s32_t* b,
    const unsigned length,
    const right_shift_t b_shr);



/**
 * Returns headroom
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 */
headroom_t xs3_squared_mag_vect_complex_s16(
    int16_t* a,
    const int16_t* b_real,
    const int16_t* b_imag,
    const unsigned length,
    const right_shift_t b_shr);

/**
 * Returns headroom
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 */
headroom_t xs3_squared_mag_vect_complex_s32(
    int32_t* a,
    const complex_s32_t* b,
    const unsigned length,
    const right_shift_t b_shr);


/**
 * Returns headroom
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 */
headroom_t xs3_mag_vect_complex_s16(
    int16_t* a,
    const int16_t* b_real,
    const int16_t* b_imag,
    const unsigned length,
    const right_shift_t b_shr,
    const int16_t* rot_table,
    const unsigned table_rows);

/**
 * Returns headroom
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 */
headroom_t xs3_mag_vect_complex_s32(
    int32_t* a,
    const complex_s32_t* b,
    const unsigned length,
    const right_shift_t b_shr,
    const complex_s32_t* rot_table,
    const unsigned table_rows);

    
/**
 * 
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 */
complex_s16_t xs3_sum_complex_s16(
    const int16_t* b_real,
    const int16_t* b_imag,
    const unsigned length,
    const right_shift_t sat);


/**
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 */
complex_s32_t xs3_sum_complex_s32(
    const complex_s32_t* b,
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t sat);




/**
 * @brief Reverses the order of the tail of a complex 32-bit vector.
 * 
 * Reverses the tail of the complex 32-bit vector @vect{X}. In other words, the first element @math{X[0]} remains where 
 * it is, and the remaining @math{N-1} elements are rearranged to have their order reversed.
 * 
 * @note This function is used when performing a mono FFT.
 * @par
 * @note Saturation logic may be applied to the real or imaginary parts of elements (@ref saturation).
 * 
 * @param[in]   x   The vector to have its tail reversed.
 * @param[in]   N   The length of `x`.
 */
void xs3_tail_reverse_complex_s32(
    complex_s32_t x[],
    const unsigned N);



/**
 * @brief Get headroom of complex 32-bit vector.
 * 
 */
static headroom_t xs3_headroom_vect_complex_s32(
    const complex_s32_t a[], 
    const unsigned length)
{
    return xs3_headroom_vect_s32((int32_t*)a, 2*length);
}

/**
 * @brief Adds one complex 32-bit vector to another.
 * 
 */
static headroom_t xs3_add_vect_complex_s32(
    complex_s32_t a[], 
    const complex_s32_t b[],
    const complex_s32_t c[],
    const unsigned length, 
    const right_shift_t b_shr,
    const right_shift_t c_shr)
{
    return xs3_add_vect_s32((int32_t*)a, (int32_t*)b, (int32_t*)c, 2*length, b_shr, c_shr);
}
    
    
/**
 * @brief Subtracts one complex 32-bit vector from another.
 * 
 */
static headroom_t xs3_sub_vect_complex_s32(
    complex_s32_t a[], 
    const complex_s32_t b[],
    const complex_s32_t c[],
    const unsigned length, 
    const right_shift_t b_shr,
    const right_shift_t c_shr)
{
    return xs3_sub_vect_s32((int32_t*)a, (int32_t*)b, (int32_t*)c, 2*length, b_shr, c_shr);
}


#ifdef __XC__
}   //extern "C"
#endif

#endif //XS3_BFP_COMPLEX_LOW_H_