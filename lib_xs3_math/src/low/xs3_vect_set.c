
#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "vpu_helper.h"



WEAK_FUNC
void xs3_vect_s16_set(
    int16_t data[],
    const int16_t value,
    const unsigned length)
{
    for(int i = 0; i < length; i++)
        data[i] = value;
}



WEAK_FUNC
void xs3_vect_s32_set(
    int32_t data[],
    const int32_t value,
    const unsigned length)
{
    for(int i = 0; i < length; i++)
        data[i] = value;
}


WEAK_FUNC
void xs3_vect_complex_s32_set(
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







void xs3_vect_complex_s16_set(
    int16_t real[],
    int16_t imag[],
    const int16_t real_value,
    const int16_t imag_value,
    const unsigned length)
{
    xs3_vect_s16_set(real, real_value, length);
    xs3_vect_s16_set(imag, imag_value, length);
}


void xs3_vect_ch_pair_s16_set(
    ch_pair_s16_t data[],
    const int16_t ch_a,
    const int16_t ch_b,
    const unsigned length)
{
    union {
        int32_t s32;
        ch_pair_s16_t cp16;
    } tmp;

    tmp.cp16.ch_a = ch_a;
    tmp.cp16.ch_b = ch_b;
    
    xs3_vect_s32_set((int32_t*) data, tmp.s32, length);
}



void xs3_vect_ch_pair_s32_set(
    ch_pair_s32_t data[],
    const int32_t ch_a,
    const int32_t ch_b,
    const unsigned length)
{
    xs3_vect_complex_s32_set((complex_s32_t*) data, ch_a, ch_b, length);
}