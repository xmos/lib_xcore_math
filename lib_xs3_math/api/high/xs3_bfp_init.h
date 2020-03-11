
#ifndef XS3_BFP_INIT_H_
#define XS3_BFP_INIT_H_

#include <stdlib.h>
#include <stdint.h>

#include "xs3_math_types.h"

#ifdef __XC__
extern "C" {
#endif

/** Initialize a 16-bit BFP vector.
 * 
 * This function initializes each of the fields of ``a``.
 * 
 * If ``calc_hr`` is false, ``a->hr`` is initialized to 0. Otherwise, the actual
 * headroom of the first ``length`` elements of ``data`` is calculated and used
 * to initialize ``a->hr``.
 * 
 * \param a         BFP vector struct to initialize
 * \param data      int16_t buffer large enough to fit at least ``length`` elements
 * \param exp       Exponent of BFP vector
 * \param length    Size of BFP vector in elements
 * \param calc_hr   Boolean indicating whether the HR of ``data`` should be calculated
 */
void bfp_init_vect_s16(
    bfp_s16_t* a, 
    int16_t* data, 
    const exponent_t exp, 
    const unsigned length,
    const unsigned calc_hr);

/** Initialize a 32-bit BFP vector.
 * 
 * This function initializes each of the fields of ``a``.
 * 
 * If ``calc_hr`` is false, ``a->hr`` is initialized to 0. Otherwise, the actual
 * headroom of the first ``length`` elements of ``data`` is calculated and used
 * to initialize ``a->hr``.
 * 
 * \param a         BFP vector struct to initialize
 * \param data      int32_t buffer large enough to fit at least ``length`` elements
 * \param exp       Exponent of BFP vector
 * \param length    Size of BFP vector in elements
 * \param calc_hr   Boolean indicating whether the HR of ``data`` should be calculated
 */
void bfp_init_vect_s32(
    bfp_s32_t* a, 
    int32_t* data, 
    const exponent_t exp, 
    const unsigned length,
    const unsigned calc_hr);

/** Initialize a 16-bit complex BFP vector.
 * 
 * This function initializes each of the fields of ``a``.
 * 
 * If ``calc_hr`` is false, ``a->hr`` is initialized to 0. Otherwise, the actual
 * headroom of the first ``length`` elements of ``data`` is calculated and used
 * to initialize ``a->hr``.
 * 
 * \param a         BFP vector struct to initialize
 * \param real_data int16_t buffer large enough to fit at least ``length`` elements
 * \param imag_data int16_t buffer large enough to fit at least ``length`` elements
 * \param exp       Exponent of BFP vector
 * \param length    Size of BFP vector in elements
 * \param calc_hr   Boolean indicating whether the HR of ``data`` should be calculated
 */
void bfp_init_vect_complex_s16(
    bfp_complex_s16_t* a, 
    int16_t* real_data,
    int16_t* imag_data, 
    const exponent_t exp, 
    const unsigned length,
    const unsigned calc_hr);

/** Initialize a 32-bit complex BFP vector.
 * 
 * This function initializes each of the fields of ``a``.
 * 
 * If ``calc_hr`` is false, ``a->hr`` is initialized to 0. Otherwise, the actual
 * headroom of the first ``length`` elements of ``data`` is calculated and used
 * to initialize ``a->hr``.
 * 
 * \param a         BFP vector struct to initialize
 * \param data      complex_s32_t buffer large enough to fit at least ``length`` elements
 * \param exp       Exponent of BFP vector
 * \param length    Size of BFP vector in elements
 * \param calc_hr   Boolean indicating whether the HR of ``data`` should be calculated
 */
void bfp_init_vect_complex_s32(
    bfp_complex_s32_t* a, 
    complex_s32_t* data, 
    const exponent_t exp, 
    const unsigned length,
    const unsigned calc_hr);

/** Initialize a 16-bit BFP channel pair vector.
 * 
 * This function initializes each of the fields of ``a``.
 * 
 * If ``calc_hr`` is false, ``a->hr`` is initialized to 0. Otherwise, the actual
 * headroom of the first ``length`` elements of ``data`` is calculated and used
 * to initialize ``a->hr``.
 * 
 * \param a         BFP vector struct to initialize
 * \param data      ch_pair_s16_t buffer large enough to fit at least ``length`` elements
 * \param exp       Exponent of BFP vector
 * \param length    Size of BFP vector in elements
 * \param calc_hr   Boolean indicating whether the HR of ``data`` should be calculated
 */
void bfp_init_vect_ch_pair_s16(
    bfp_ch_pair_s16_t* a, 
    ch_pair_s16_t* data, 
    const exponent_t exp, 
    const unsigned length,
    const unsigned calc_hr);


/** Initialize a 32-bit BFP channel pair vector.
 * 
 * This function initializes each of the fields of ``a``.
 * 
 * If ``calc_hr`` is false, ``a->hr`` is initialized to 0. Otherwise, the actual
 * headroom of the first ``length`` elements of ``data`` is calculated and used
 * to initialize ``a->hr``.
 * 
 * \param a         BFP vector struct to initialize
 * \param data      ch_pair_s32_t buffer large enough to fit at least ``length`` elements
 * \param exp       Exponent of BFP vector
 * \param length    Size of BFP vector in elements
 * \param calc_hr   Boolean indicating whether the HR of ``data`` should be calculated
 */
void bfp_init_vect_ch_pair_s32(
    bfp_ch_pair_s32_t* a, 
    ch_pair_s32_t* data, 
    const exponent_t exp, 
    const unsigned length,
    const unsigned calc_hr);


/** Set all elements of a 16-bit BFP vector to a given value.
 * 
 * \param a         BFP vector to update
 * \param value     Value to set elements to
 * \param exp       Exponent associated with value
 */
void bfp_set_vect_s16(
    bfp_s16_t* a,
    const int16_t value,
    const exponent_t exp);

/** Set all elements of a 32-bit BFP vector to a given value.
 * 
 * \param a         BFP vector to update
 * \param value     Value to set elements to
 * \param exp       Exponent associated with value
 */
void bfp_set_vect_s32(
    bfp_s32_t* a,
    const int32_t value,
    const exponent_t exp);

/** Set all elements of a 16-bit complex BFP vector to a given value.
 * 
 * \param a         BFP vector to update
 * \param value     Value to set elements to
 * \param exp       Exponent associated with value
 */
void bfp_set_vect_complex_s16(
    bfp_complex_s16_t* a,
    const complex_s16_t value,
    const exponent_t exp);

/** Set all elements of a 32-bit complex BFP vector to a given value.
 * 
 * \param a         BFP vector to update
 * \param value     Value to set elements to
 * \param exp       Exponent associated with value
 */
void bfp_set_vect_complex_s32(
    bfp_complex_s32_t* a,
    const complex_s32_t value,
    const exponent_t exp);

/** Set all elements of a 16-bit BFP channel pair vector to a given value.
 * 
 * \param a         BFP vector to update
 * \param value     Value to set elements to
 * \param exp       Exponent associated with value
 */
void bfp_set_vect_ch_pair_s16(
    bfp_ch_pair_s16_t* a,
    const ch_pair_s16_t value,
    const exponent_t exp);

/** Set all elements of a 32-bit BFP channel pair vector to a given value.
 * 
 * \param a         BFP vector to update
 * \param value     Value to set elements to
 * \param exp       Exponent associated with value
 */
void bfp_set_vect_ch_pair_s32(
    bfp_ch_pair_s32_t* a,
    const ch_pair_s32_t value,
    const exponent_t exp);

    

#ifdef __XC__
}   //extern "C"
#endif

#endif //XS3_BFP_INIT_H_