
#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "vpu_helper.h"



WEAK_FUNC
int16_t xs3_vect_s16_max(
    const int16_t b[],
    const unsigned length)
{
    int16_t cur_max = -0x8000;
    for(int k = 0; k < length; k++)
        cur_max = MAX(cur_max, b[k]);
    return cur_max;
}


WEAK_FUNC
int32_t xs3_vect_s32_max(
    const int32_t b[],
    const unsigned length)
{
    int32_t cur_max = -0x80000000;
    for(int k = 0; k < length; k++){
        cur_max = MAX(cur_max, b[k]);
    }
    return cur_max;
}


WEAK_FUNC
int16_t xs3_vect_s16_min(
    const int16_t b[],
    const unsigned length)
{
    int16_t cur_min = -0x8000;
    for(int k = 0; k < length; k++)
        cur_min = MIN(cur_min, b[k]);
    return cur_min;
}


WEAK_FUNC
int32_t xs3_vect_s32_min(
    const int32_t b[],
    const unsigned length)
{
    int32_t cur_min = -0x80000000;
    for(int k = 0; k < length; k++){
        cur_min = MIN(cur_min, b[k]);
    }
    return cur_min;
}



WEAK_FUNC
unsigned xs3_vect_s16_argmax(
    const int16_t b[],
    const unsigned length)
{

    unsigned res = 0;
    for(int k = 1; k < length; k++)
        res = (b[k] > b[res])? k : res;
    return res;
}

WEAK_FUNC
unsigned xs3_vect_s32_argmax(
    const int32_t b[],
    const unsigned length)
{
    unsigned res = 0;
    for(int k = 1; k < length; k++)
        res = (b[k] > b[res])? k : res;
    return res;
}


WEAK_FUNC
unsigned xs3_vect_s16_argmin(
    const int16_t b[],
    const unsigned length)
{
    unsigned res = 0;
    for(int k = 1; k < length; k++)
        res = (b[k] < b[res])? k : res;
    return res;
}

WEAK_FUNC
unsigned xs3_vect_s32_argmin(
    const int32_t b[],
    const unsigned length)
{
    unsigned res = 0;
    for(int k = 1; k < length; k++)
        res = (b[k] < b[res])? k : res;
    return res;
}





WEAK_FUNC
int32_t xs3_vect_s16_abs_sum(
    const int16_t b[],
    const unsigned length)
{
    int32_t acc = 0;
    for(int k = 0; k < length; k++){

        int16_t p = b[k];
        p = (p>=0)? p : -p;
        p = (p>=0)? p : 0x7FFF; // because -(-0x8000) == -0x8000
        acc += p;
        acc = MAX(-0x7FFFFFFF, acc);
    }
    return acc;
}


WEAK_FUNC
int64_t xs3_vect_s32_abs_sum(
    const int32_t b[],
    const unsigned length)
{
    int64_t acc = 0;
    for(int k = 0; k < length; k++){
        acc += (b[k] >= 0)? b[k] : -b[k];
    }
    return acc;
}


WEAK_FUNC
int32_t xs3_vect_s16_energy(
    const int16_t b[],
    const unsigned length,
    const right_shift_t b_shr)
{
    int64_t acc = 0;
    for(int k = 0; k < length; k++){
        int32_t t = b[k];
        t = (b_shr>=0)? (t>>b_shr) : (t<<-b_shr);
        acc += t*t;
        acc = MAX(-0x7FFFFFFF, MIN(0x7FFFFFFF, acc));
    }
    return (int32_t)acc;
}

WEAK_FUNC
int64_t xs3_vect_s32_energy(
    const int32_t b[],
    const unsigned length,
    const right_shift_t b_shr)
{
    int64_t acc = 0;
    for(int k = 0; k < length; k++){
        int64_t t = b[k];
        t = (b_shr>=0)? (t>>b_shr) : (t<<-b_shr);
        t = ((t*t)+(1<<29)) >> 30;
        acc += t;
        acc = MAX(-0x7FFFFFFFFFLL, MIN(0x7FFFFFFFFFLL, acc));
    }
    return acc;
}