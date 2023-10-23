// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include <stdint.h>
#include <stdio.h>

#include "xmath/xmath.h"

void filter_fir_s16_push_sample_up(
    int16_t* buffer,
    const unsigned length,
    const int16_t new_value);

void filter_fir_s16_push_sample_down(
    int16_t* buffer,
    const unsigned length,
    const int16_t new_value);

void filter_fir_s16_init(
    filter_fir_s16_t* filter,
    int16_t* sample_buffer,
    const unsigned tap_count,
    const int16_t* coefficients,
    const right_shift_t shift)
{
    assert(tap_count != 0);
    filter->num_taps = tap_count;
    filter->shift = shift;
    filter->coef = (int16_t*) coefficients;
    filter->state = sample_buffer;
}


void filter_fir_s16_add_sample(
    filter_fir_s16_t* filter,
    const int16_t new_sample)
{
    filter_fir_s16_push_sample_up(filter->state, filter->num_taps, new_sample); 
}



void filter_fir_s32_init(
    filter_fir_s32_t* filter,
    int32_t* sample_buffer,
    const unsigned tap_count,
    const int32_t* coefficients,
    const right_shift_t shift)
{
    assert(tap_count != 0);
    filter->num_taps = tap_count;
    filter->head = tap_count-1;
    filter->shift = shift;
    filter->coef = (int32_t*) coefficients;
    filter->state = sample_buffer;
}


void filter_fir_s32_add_sample(
    filter_fir_s32_t* filter,
    const int32_t new_sample)
{
    filter->state[filter->head] = new_sample;
    
    if(filter->head == 0)   filter->head = filter->num_taps - 1;
    else                    filter->head = filter->head - 1;
}



int32_t filter_biquads_s32(
    filter_biquad_s32_t biquads[],
    const unsigned block_count,
    const int32_t new_sample)
{
    int32_t smp = new_sample;

    for(unsigned i = 0; i < block_count; i++)
        smp = filter_biquad_s32(&biquads[i], smp);
    
    return smp;
}
