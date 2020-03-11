
#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "vpu_helper.h"



WEAK_FUNC
void xs3_set_array_s16(
    int16_t data[],
    const int16_t value,
    const unsigned length)
{
    for(int i = 0; i < length; i++)
        data[i] = value;
}



WEAK_FUNC
void xs3_set_array_s32(
    int32_t data[],
    const int32_t value,
    const unsigned length)
{
    for(int i = 0; i < length; i++)
        data[i] = value;
}


WEAK_FUNC
void xs3_set_array_complex_s32(
    complex_s32_t data[],
    const int32_t real_part,
    const int32_t imag_part,
    const unsigned length)
{
    for(int i = 0; i < length; i++){
        data[i].re = real_part;
        data[i].im = imag_part;
    }
}