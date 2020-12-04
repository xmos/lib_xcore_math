

#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "../vpu_helper.h"


void xs3_filter_fir_s32_init(
    xs3_fir_filter_s32_t* filter,
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


void xs3_filter_fir_s32_add_sample(
    xs3_fir_filter_s32_t* filter,
    const int32_t new_sample)
{
    filter->state[filter->head] = new_sample;
    
    if(filter->head == 0)   filter->head = filter->num_taps - 1;
    else                    filter->head = filter->head - 1;

    // for(int i = 0; i < filter->num_taps; i++)
    //     printf("%ld, ", filter->state[i]);
    // printf("\n");
}



#define MUL32(X, Y)     ((int32_t)(((((int64_t)(X)) * (Y)) + (1<<29)) >> 30))

WEAK_FUNC
int32_t xs3_filter_fir_s32(
    xs3_fir_filter_s32_t* filter,
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
