
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
 * \foperation{16, @f$data_k \leftarrow value\qquad\text{ for }k\in 0\ ...\ (length-1)@f$ }
 * 
 * \requires_word_alignment{data}
 * 
 * \param[out] data     Array to set
 * \param[in] value     Value to set
 * \param[in] length    Number of elements in `data`
 */
void xs3_set_vect_s16(
    int16_t data[],
    const int16_t value,
    const unsigned length);

/** Set all elements of an `int32_t` array to the specified value.
 * 
 * \foperation{32, @f$data_k \leftarrow value\qquad\text{ for }k\in 0\ ...\ (length-1)@f$ }
 * 
 * \requires_word_alignment{data}
 * 
 * \param[out] data     Array to set
 * \param[in]  value    Value to set
 * \param[in]  length   Number of elements in `data`
 */
void xs3_set_vect_s32(
    int32_t data[],
    const int32_t value,
    const unsigned length);

/** Set all elements of a `complex_s32_t` array to the specified value.
 * 
 * \foperation{Complex&nbsp;32, @f$data_k \leftarrow real + i\cdot imag\qquad\text{ for }k\in 0\ ...\ (length-1)@f$ }
 * 
 * \requires_word_alignment{data}
 * 
 * \param[out] data     Array to set
 * \param[in]  real     Real part of value to set
 * \param[in]  imag     Imaginary part of value to set
 * \param[in]  length   Number of elements in `data`
 */
void xs3_set_vect_complex_s32(
    complex_s32_t data[],
    const int32_t real,
    const int32_t imag,
    const unsigned length);

#ifdef __XC__
}   //extern "C"
#endif

#endif //XS3_BFP_INIT_LOW_H_