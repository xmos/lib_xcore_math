

#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "../../../vect/vpu_helper.h"



#define MUL32(X, Y)     ((int32_t)(((((int64_t)(X)) * (Y)) + (1<<29)) >> 30))


int32_t xs3_filter_fir_s32(
    xs3_filter_fir_s32_t* filter,
    const int32_t new_sample)
{
    const unsigned head = filter->head;

    xs3_filter_fir_s32_add_sample(filter, new_sample);
    
    const unsigned N_A = filter->num_taps - head;
    const unsigned N_B = head;

    int64_t sum = 0;

    for(int i = 0; i < N_A; i++)
        sum += MUL32(filter->state[N_B + i], filter->coef[i]);

    for(int i = 0; i < N_B; i++)
        sum += MUL32(filter->state[i], filter->coef[N_A + i]);


    if(filter->shift >= 0)  sum = sum >>  filter->shift;
    else                    sum = sum << -filter->shift;

    return (int32_t)sum;
}
