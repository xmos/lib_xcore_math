
#ifndef XS3_BFP_INIT_H_
#define XS3_BFP_INIT_H_

#include <stdlib.h>
#include <stdint.h>

#include "xs3_math_types.h"

#ifdef __XC__
extern "C" {
#endif

/** 
 * \brief Initialize a 16-bit BFP vector.
 * 
 * This function initializes each of the fields of `a`.
 * 
 * `data` points to the memory buffer used to store elements of the vector, so it must be at least
 * `length * 2` bytes long.
 * 
 * `exp` is the exponent assigned to the BFP vector. The logical value associated with the `k`th element
 * of the vector after initialization will be @f$ data_k\cdot2^{exp} @f$.
 * 
 * If `calc_hr` is false, `a->hr` is initialized to 0. Otherwise, the headroom of the the BFP vector 
 * is calculated and used to initialize `a->hr`.
 * 
 * \requires_word_alignment{data}
 * 
 * \param a         BFP vector to initialize
 * \param data      `int16_t` buffer used to back `a`
 * \param exp       Exponent of BFP vector
 * \param length    Number of elements in the BFP vector
 * \param calc_hr   Boolean indicating whether the HR of the BFP vector should be calculated
 */
void bfp_vect_s16_init(
    bfp_s16_t* a, 
    int16_t* data, 
    const exponent_t exp, 
    const unsigned length,
    const unsigned calc_hr);

/** 
 * \brief Initialize a 32-bit BFP vector.
 * 
 * This function initializes each of the fields of `a`.
 * 
 * `data` points to the memory buffer used to store elements of the vector, so it must be at least
 * `length * 4` bytes long.
 * 
 * `exp` is the exponent assigned to the BFP vector. The logical value associated with the `k`th element
 * of the vector after initialization will be @f$ data_k\cdot2^{exp} @f$.
 * 
 * If `calc_hr` is false, `a->hr` is initialized to 0. Otherwise, the headroom of the the BFP vector 
 * is calculated and used to initialize `a->hr`.
 * 
 * \requires_word_alignment{data}
 * 
 * \param a         BFP vector to initialize
 * \param data      `int32_t` buffer used to back `a`
 * \param exp       Exponent of BFP vector
 * \param length    Number of elements in the BFP vector
 * \param calc_hr   Boolean indicating whether the HR of the BFP vector should be calculated
 */
void bfp_vect_s32_init(
    bfp_s32_t* a, 
    int32_t* data, 
    const exponent_t exp, 
    const unsigned length,
    const unsigned calc_hr);

/** 
 * \brief Initialize a 16-bit complex BFP vector.
 * 
 * This function initializes each of the fields of `a`.
 * 
 * Unlike `bfp_complex_s32_t`, 16-bit BFP vectors use separate buffers to store the real and imaginary
 * parts of the vector. `real_data` points to the memory buffer used to store real elements of the vector.
 * `imag_data` points to the memory buffer used to store imaginary elements of the vector. Each must be
 * at least `length * 2` bytes long.
 * 
 * `exp` is the exponent assigned to the BFP vector. The logical value associated with the `k`th element
 * of the vector after initialization will be @f$ \left(real\_data_k + i\cdot imag\_data_k \right)\cdot2^{exp} @f$.
 * 
 * If `calc_hr` is false, `a->hr` is initialized to 0. Otherwise, the headroom of the the BFP vector 
 * is calculated and used to initialize `a->hr`.
 * 
 * \requires_word_alignment{real_data,imag_data}
 * 
 * \param a         BFP vector to initialize
 * \param real_data `int16_t` buffer used to back the real part of `a`
 * \param imag_data `int16_t` buffer used to back the imaginary part of `a`
 * \param exp       Exponent of BFP vector
 * \param length    Number of elements in BFP vector
 * \param calc_hr   Boolean indicating whether the HR of the BFP vector should be calculated
 */
void bfp_vect_complex_s16_init(
    bfp_complex_s16_t* a, 
    int16_t* real_data,
    int16_t* imag_data, 
    const exponent_t exp, 
    const unsigned length,
    const unsigned calc_hr);

/** 
 * \brief Initialize a 32-bit complex BFP vector.
 * 
 * This function initializes each of the fields of `a`.
 * 
 * Unlike `bfp_complex_s16_t`, 32-bit BFP vectors use a single buffer to store the real and imaginary
 * parts of the vector, such that the imaginary part of element `k` follows the real part of element `k`
 * in memory. `data` points to the memory buffer used to store real elements of the vector, and must be
 * at least `length * 8` bytes long.
 * 
 * `exp` is the exponent assigned to the BFP vector. The logical value associated with the `k`th complex
 * element of the vector after initialization will be @f$ \left(data_{2k} + i\cdot data_{2k+1} \right)\cdot2^{exp} @f$.
 * 
 * If `calc_hr` is false, `a->hr` is initialized to 0. Otherwise, the headroom of the the BFP vector 
 * is calculated and used to initialize `a->hr`.
 * 
 * \requires_word_alignment{data}
 * 
 * \param a         BFP vector struct to initialize
 * \param data      `complex_s32_t` buffer used to back `a`
 * \param exp       Exponent of BFP vector
 * \param length    Number of elements in BFP vector
 * \param calc_hr   Boolean indicating whether the HR of the BFP vector should be calculated
 */
void bfp_vect_complex_s32_init(
    bfp_complex_s32_t* a, 
    complex_s32_t* data, 
    const exponent_t exp, 
    const unsigned length,
    const unsigned calc_hr);

/** 
 * \brief Initialize a 16-bit channel pair BFP vector.
 * 
 * This function initializes each of the fields of `a`.
 * 
 * `data` points to the memory buffer used to store elements of the vector. Two values comprise each element
 * of the vector, called channel A and channel B. The channel A and B values corresponding to the same vector
 * element are stored at adjacent locations in memory. The memory buffer at which `data` points must be at 
 * least `length * 4` bytes long.
 * 
 * `exp` is the exponent assigned to the BFP vector. The logical value associated with the `k`th channel A
 * value will be @f$ data_{2k} \cdot 2^{exp} @f$. The logical value associated with the `k`th channel B
 * value will be @f$ data_{2k+1} \cdot 2^{exp} @f$.
 * 
 * If `calc_hr` is false, `a->hr` is initialized to 0. Otherwise, the headroom of the the BFP vector 
 * is calculated and used to initialize `a->hr`.
 * 
 * \requires_word_alignment{data}
 * 
 * \param a         BFP vector struct to initialize
 * \param data      `ch_pair_s16_t` buffer used to back `a`
 * \param exp       Exponent of BFP vector
 * \param length    Number of elements in BFP vector
 * \param calc_hr   Boolean indicating whether the HR of the BFP vector should be calculated
 */
void bfp_vect_ch_pair_s16_init(
    bfp_ch_pair_s16_t* a, 
    ch_pair_s16_t* data, 
    const exponent_t exp, 
    const unsigned length,
    const unsigned calc_hr);


/** 
 * \brief Initialize a 32-bit channel pair BFP vector.
 * 
 * This function initializes each of the fields of `a`.
 * 
 * `data` points to the memory buffer used to store elements of the vector. Two values comprise each element
 * of the vector, called channel A and channel B. The channel A and B values corresponding to the same vector
 * element are stored at adjacent locations in memory. The memory buffer at which `data` points must be at 
 * least `length * 8` bytes long.
 * 
 * `exp` is the exponent assigned to the BFP vector. The logical value associated with the `k`th channel A
 * value will be @f$ data_{2k} \cdot 2^{exp} @f$. The logical value associated with the `k`th channel B
 * value will be @f$ data_{2k+1} \cdot 2^{exp} @f$.
 * 
 * If `calc_hr` is false, `a->hr` is initialized to 0. Otherwise, the headroom of the the BFP vector 
 * is calculated and used to initialize `a->hr`.
 * 
 * \requires_word_alignment{data}
 * 
 * \param a         BFP vector struct to initialize
 * \param data      `ch_pair_s32_t` buffer used to back `a`
 * \param exp       Exponent of BFP vector
 * \param length    Number of elements in BFP vector
 * \param calc_hr   Boolean indicating whether the HR of the BFP vector should be calculated
 */
void bfp_vect_ch_pair_s32_init(
    bfp_ch_pair_s32_t* a, 
    ch_pair_s32_t* data, 
    const exponent_t exp, 
    const unsigned length,
    const unsigned calc_hr);


/**
 * \brief Set elements of 16-bit BFP vector `a` to a specified value.
 * 
 * The exponent of `a` is set to `exp`, and each element's mantissa is set to `alpha`.
 * 
 * After performing this operation, all elements will represent the same value @f$ \alpha\cdot 2^{exp} @f$.
 * 
 * \param a         BFP vector to update
 * \param alpha     New value each element's mantissa is set to
 * \param exp       New exponent for the BFP vector
 */
void bfp_vect_s16_set(
    bfp_s16_t* a,
    const int16_t alpha,
    const exponent_t exp);

/** 
 * \brief Set elements of 32-bit BFP vector `a` to a specified value.
 * 
 * The exponent of `a` is set to `exp`, and each element's mantissa is set to `alpha`.
 * 
 * After performing this operation, all elements will represent the same value @f$ \alpha\cdot 2^{exp} @f$.
 * 
 * \param a         BFP vector to update
 * \param alpha     New value each element's mantissa is set to
 * \param exp       New exponent for the BFP vector
 */
void bfp_vect_s32_set(
    bfp_s32_t* a,
    const int32_t alpha,
    const exponent_t exp);

/** 
 * \brief Set elements of complex 16-bit BFP vector `a` to a specified value.
 * 
 * The exponent of `a` is set to `exp`. The real part of each element's mantissa is set to `z.re`, and 
 * the imaginary part of each element's mantissa is set to `z.im`.
 * 
 * After performing this operation, all elements will represent the same complex value @f$ \left(\alpha + i\cdot\beta
 *      \right)\cdot 2^{exp} @f$, where @f$z = \alpha + i\cdot\beta@f$.
 * 
 * \param a         BFP vector to update
 * \param z         New value each element's mantissa is set to
 * \param exp       New exponent for the BFP vector
 */
void bfp_vect_complex_s16_set(
    bfp_complex_s16_t* a,
    const complex_s16_t z,
    const exponent_t exp);

/** 
 * \brief Set elements of complex 32-bit BFP vector `a` to a specified value.
 * 
 * The exponent of `a` is set to `exp`. The real part of each element's mantissa is set to `z.re`, and 
 * the imaginary part of each element's mantissa is set to `z.im`.
 * 
 * After performing this operation, all elements will represent the same complex value @f$ \left(\alpha + i\cdot\beta
 *      \right)\cdot 2^{exp} @f$, where @f$z = \alpha + i\cdot\beta@f$.
 * 
 * \param a         BFP vector to update
 * \param z     New value each element's mantissa is set to
 * \param exp       New exponent for the BFP vector
 */
void bfp_vect_complex_s32_set(
    bfp_complex_s32_t* a,
    const complex_s32_t z,
    const exponent_t exp);

/** 
 * \brief Set elements of 16-bit BFP channel-pair vector `a` to a specified value.
 * 
 * The exponent of `a` is set to `exp`. The channel A part of each element's mantissa is set to `z.ch_a`, and 
 * the channel B part of each element's mantissa is set to `z.ch_b`.
 * 
 * After performing this operation, all channel A values will represent @f$ \alpha\cdot 2^{exp} @f$, and 
 * channel B values will represent @f$ \beta\cdot 2^{exp} @f$ where @f$z = \alpha + i\cdot\beta@f$.
 * 
 * \param a         BFP vector to update
 * \param z         New value each element's mantissa is set to
 * \param exp       New exponent for the BFP vector
 */
void bfp_vect_ch_pair_s16_set(
    bfp_ch_pair_s16_t* a,
    const ch_pair_s16_t z,
    const exponent_t exp);

/** 
 * \brief Set elements of 32-bit BFP channel-pair vector `a` to a specified value.
 * 
 * The exponent of `a` is set to `exp`. The channel A part of each element's mantissa is set to `z.ch_a`, and 
 * the channel B part of each element's mantissa is set to `z.ch_b`.
 * 
 * After performing this operation, all channel A values will represent @f$ \alpha\cdot 2^{exp} @f$, and 
 * channel B values will represent @f$ \beta\cdot 2^{exp} @f$ where @f$z = \alpha + i\cdot\beta@f$.
 * 
 * \param a         BFP vector to update
 * \param z         New value each element's mantissa is set to
 * \param exp       New exponent for the BFP vector
 */
void bfp_vect_ch_pair_s32_set(
    bfp_ch_pair_s32_t* a,
    const ch_pair_s32_t z,
    const exponent_t exp);

    

#ifdef __XC__
}   //extern "C"
#endif

#endif //XS3_BFP_INIT_H_