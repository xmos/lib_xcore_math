// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include <stdint.h>
#include <stdio.h>

#include "xmath/xmath.h"
#include "vpu_helper.h"




#define MUL32(X, Y)     ((int32_t)(((((int64_t)(X)) * (Y)) + (1<<29)) >> 30))


int32_t filter_biquad_s32(
    filter_biquad_s32_t* filter,
    const int32_t new_sample)
{

    int64_t accs[8] = { 0 };

    // -a2 * y[n-2]
    for(int i = 0; i < 8; i++)
        accs[i] += MUL32(filter->state[1][i+1], filter->coef[4][i]);
    
    // -a1 * y[n-1]
    for(int i = 0; i < 8; i++)
        accs[i] += MUL32(filter->state[0][i+1], filter->coef[3][i]);
        
    // b2 * x[n-2]
    for(int i = 0; i < 8; i++)
        accs[i] += MUL32(filter->state[1][i], filter->coef[2][i]);

    // b1 * x[n-1]
    for(int i = 0; i < 8; i++)
        accs[i] += MUL32(filter->state[0][i], filter->coef[1][i]);

    //Before dealing with b0 * x[n], we need to move some memory around.
    for(int i = 0; i < 9; i++)
        filter->state[1][i] = filter->state[0][i];

    // And calculate each new output
    filter->state[0][0] = new_sample;
    for(unsigned i = 0; i < filter->biquad_count; i++){
        accs[i] += MUL32(filter->state[0][i], filter->coef[0][i]);
        
        // The output is the input to the next biquad
        filter->state[0][i+1] = (int32_t) accs[i];
    }
    
    return filter->state[0][filter->biquad_count];
}
