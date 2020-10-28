
#ifndef XS3_BFP_FILTERS_LOW_H_
#define XS3_BFP_FILTERS_LOW_H_

#include "xs3_math_types.h"

#ifdef __XC__
extern "C" {
#endif


typedef struct {
    unsigned num_taps;
    unsigned head;
    right_shift_t shift;
    int32_t* coef;
    int32_t* state;
} xs3_fir_filter_s32_t;



void xs3_filter_fir_s32_init(
    xs3_fir_filter_s32_t* filter,
    int32_t* sample_buffer,
    const unsigned tap_count,
    const int32_t* coefficients,
    const right_shift_t shift);

void xs3_filter_fir_s32_add_sample(
    xs3_fir_filter_s32_t* filter,
    const int32_t new_sample);
    
int32_t xs3_filter_fir_s32(
    xs3_fir_filter_s32_t* filter,
    const int32_t new_sample);


    
typedef struct {
    unsigned num_taps;
    right_shift_t shift;
    int16_t* coef;
    int16_t* state;
} xs3_fir_filter_s16_t;

void xs3_filter_fir_s16_init(
    xs3_fir_filter_s16_t* filter,
    int16_t* sample_buffer,
    const unsigned tap_count,
    const int16_t* coefficients,
    const right_shift_t shift);

void xs3_filter_fir_s16_add_sample(
    xs3_fir_filter_s16_t* filter,
    const int16_t new_sample);
    
int16_t xs3_filter_fir_s16(
    xs3_fir_filter_s16_t* filter,
    const int16_t new_sample);



typedef struct {
    unsigned biquad_count;
    int32_t state[2][9]; // state[j][k] is the value x_k[j], i.e.  x[n-j] of the kth biquad. x[j][8] are outputs of 8th biquad
    int32_t coef[5][8];  // coefficients. coef[j][k] is for the kth biquad. j maps to b0,b1,b2,-a1,-a2.
} xs3_biquad_filter_s32_t;
    
int32_t xs3_filter_biquad_s32(
    xs3_biquad_filter_s32_t* filter,
    const int32_t new_sample);

int32_t xs3_filter_biquads_s32(
    xs3_biquad_filter_s32_t biquads[],
    const unsigned block_count,
    const int32_t new_sample);



#ifdef __XC__
}   //extern "C"
#endif

#endif //XS3_BFP_FILTERS_LOW_H_