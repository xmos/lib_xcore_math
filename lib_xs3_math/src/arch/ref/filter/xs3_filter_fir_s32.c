// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "../../../vect/vpu_helper.h"

#include "xs3_vpu_scalar_ops.h"



int32_t xs3_filter_fir_s32(
    xs3_filter_fir_s32_t* filter,
    const int32_t new_sample)
{
    const unsigned head = filter->head;

    xs3_filter_fir_s32_add_sample(filter, new_sample);
    
    const unsigned N_A = filter->num_taps - head;
    const unsigned N_B = head;

    vpu_int32_acc_t acc = 0;

    for(int i = 0; i < N_A; i++)
        acc = vlmacc32(acc, filter->state[N_B + i], filter->coef[i]);

    for(int i = 0; i < N_B; i++)
        acc = vlmacc32(acc, filter->state[i], filter->coef[N_A + i]);


    if(filter->shift >= 0){
        acc += (1 << (filter->shift-1));
        acc = acc >> filter->shift;
    } else {
        acc = acc << (-filter->shift);
    }

    return SAT(32)(acc);

}