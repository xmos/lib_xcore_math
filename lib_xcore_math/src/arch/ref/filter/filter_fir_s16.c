// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include <stdint.h>
#include <stdio.h>

#include "xmath/xmath.h"
#include "vpu_helper.h"



void filter_fir_s16_push_sample_up(
    int16_t* buffer,
    const unsigned length,
    const int16_t new_value)
{
    for(int i = length-1; i > 0; i--)
        buffer[i] = buffer[i-1];

    buffer[0] = new_value;
}


void filter_fir_s16_push_sample_down(
    int16_t* buffer,
    const unsigned length,
    const int16_t new_value)
{
    for(unsigned i = 0; i < length-1; i++)
        buffer[i] = buffer[i+1];
    
    buffer[length-1] = new_value;
}




int16_t filter_fir_s16(
    filter_fir_s16_t* filter,
    const int16_t new_sample)
{
    filter_fir_s16_add_sample(filter, new_sample);

    int32_t sum = 0;

    for(unsigned i = 0; i < filter->num_taps; i++){
        sum += filter->state[i] * filter->coef[i];
    }

    if(filter->shift >= 0)  sum = (sum + (1<<(filter->shift-1))) >> filter->shift;
    else                    sum <<= -filter->shift;

    return (int16_t) sum;
}
