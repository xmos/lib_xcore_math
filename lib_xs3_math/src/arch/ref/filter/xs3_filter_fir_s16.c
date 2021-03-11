// Copyright 2020 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "../../../vect/vpu_helper.h"



void xs3_push_sample_up_s16(
    int16_t* buffer,
    const unsigned length,
    const int16_t new_value)
{
    for(int i = length-1; i > 0; i--)
        buffer[i] = buffer[i-1];

    buffer[0] = new_value;
}


void xs3_push_sample_down_s16(
    int16_t* buffer,
    const unsigned length,
    const int16_t new_value)
{
    for(int i = 0; i < length-1; i++)
        buffer[i] = buffer[i+1];
    
    buffer[length-1] = new_value;
}




int16_t xs3_filter_fir_s16(
    xs3_filter_fir_s16_t* filter,
    const int16_t new_sample)
{
    xs3_filter_fir_s16_add_sample(filter, new_sample);

    int32_t sum = 0;

    for(int i = 0; i < filter->num_taps; i++){
        sum += filter->state[i] * filter->coef[i];
    }

    if(filter->shift >= 0)  sum = (sum + (1<<(filter->shift-1))) >> filter->shift;
    else                    sum <<= -filter->shift;

    return (int16_t) sum;
}
