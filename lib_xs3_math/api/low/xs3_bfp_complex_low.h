
#ifndef XS3_BFP_COMPLEX_LOW_H_
#define XS3_BFP_COMPLEX_LOW_H_

#include "xs3_math_types.h"

#ifdef __XC__
extern "C" {
#endif


/**
 * Returns headroom
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
 */
headroom_t xs3_complex_mul_vect_complex_s16(
    int16_t* a_real,
    int16_t* a_imag,
    const int16_t* b_real,
    const int16_t* b_imag,
    const int16_t* c_real,
    const int16_t* c_imag,
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * Returns headroom
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
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * Returns headroom
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
    const right_shift_t b_shr);


/**
 * Returns headroom
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
 */
headroom_t xs3_complex_s16_to_complex_s32(
    complex_s32_t* a,
    const int16_t* b_real,
    const int16_t* b_imag,
    const unsigned length);

/**
 * 
 */
headroom_t xs3_complex_s32_to_complex_s16(
    int16_t* a_real,
    int16_t* a_imag,
    const complex_s32_t* b,
    const unsigned length,
    const right_shift_t sat);



/**
 * Returns headroom
 */
headroom_t xs3_squared_mag_vect_complex_s16(
    int16_t* a,
    const int16_t* b_real,
    const int16_t* b_imag,
    const unsigned length,
    const right_shift_t b_shr);

/**
 * Returns headroom
 */
headroom_t xs3_squared_mag_vect_complex_s32(
    int32_t* a,
    const complex_s32_t* b,
    const unsigned length,
    const right_shift_t b_shr);


/**
 * Returns headroom
 */
headroom_t xs3_mag_vect_complex_s16(
    int16_t* a,
    const int16_t* b_real,
    const int16_t* b_imag,
    const unsigned length,
    const right_shift_t b_shr);

/**
 * Returns headroom
 */
headroom_t xs3_mag_vect_complex_s32(
    int32_t* a,
    const complex_s32_t* b,
    const unsigned length,
    const right_shift_t b_shr);

    
/**
 * 
 */
void xs3_sum_complex_s16(
    int16_t* real,
    int16_t* imag,
    const int16_t* b_real,
    const int16_t* b_imag,
    const unsigned length,
    const right_shift_t sat);


/**
 * 
 */
void xs3_sum_complex_s32(
    int32_t* real,
    int32_t* imag,
    const complex_s32_t* b,
    const unsigned length,
    const right_shift_t sat);


#ifdef __XC__
}   //extern "C"
#endif

#endif //XS3_BFP_COMPLEX_LOW_H_