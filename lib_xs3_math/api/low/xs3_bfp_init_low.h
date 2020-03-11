
#ifndef XS3_BFP_INIT_LOW_H_
#define XS3_BFP_INIT_LOW_H_

#include <stdlib.h>
#include <stdint.h>

#include "xs3_math_types.h"

#ifdef __XC__
extern "C" {
#endif

/** Set all elements of an `int16_t` array to the specified value.
 * 
 * \note \li `data` must be word-aligned.
 * 
 * \param[out] data     Array to set
 * \param[in] value     Value to set
 * \param[in] length    Number of elements in `data`
 */
void xs3_set_array_s16(
    int16_t data[],
    const int16_t value,
    const unsigned length);

/** Set all elements of an `int32_t` array to the specified value.
 * 
 * \note \li `data` must be word-aligned.
 * 
 * \param[out] data     Array to set
 * \param[in]  value    Value to set
 * \param[in]  length   Number of elements in `data`
 */
void xs3_set_array_s32(
    int32_t data[],
    const int32_t value,
    const unsigned length);

/** Set all elements of a `complex_s32_t` array to the specified value.
 * 
 * \note \li `data` must be word-aligned.
 * 
 * \param[out] data         Array to set
 * \param[in]  real_part    Real part of value to set
 * \param[in]  imag_part    Imaginary part of value to set
 * \param[in]  length       Number of elements in `data`
 */
void xs3_set_array_complex_s32(
    complex_s32_t data[],
    const int32_t real_part,
    const int32_t imag_part,
    const unsigned length);

#ifdef __XC__
}   //extern "C"
#endif

#endif //XS3_BFP_INIT_LOW_H_