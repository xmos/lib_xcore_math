
#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "vpu_helper.h"



WEAK_FUNC
int16_t xs3_max_s16(
    const int16_t b[],
    const unsigned length)
{
    int16_t cur_max = -0x8000;
    for(int k = 0; k < length; k++)
        cur_max = MAX(cur_max, b[k]);
    return cur_max;
}


WEAK_FUNC
int32_t xs3_max_s32(
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
int16_t xs3_min_s16(
    const int16_t b[],
    const unsigned length)
{
    int16_t cur_min = -0x8000;
    for(int k = 0; k < length; k++)
        cur_min = MIN(cur_min, b[k]);
    return cur_min;
}


WEAK_FUNC
int32_t xs3_min_s32(
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
unsigned xs3_argmax_s16(
    const int16_t b[],
    const unsigned length)
{

    unsigned res = 0;
    for(int k = 1; k < length; k++)
        res = (b[k] > b[res])? k : res;
    return res;
}

WEAK_FUNC
unsigned xs3_argmax_s32(
    const int32_t b[],
    const unsigned length)
{
    unsigned res = 0;
    for(int k = 1; k < length; k++)
        res = (b[k] > b[res])? k : res;
    return res;
}


WEAK_FUNC
unsigned xs3_argmin_s16(
    const int16_t b[],
    const unsigned length)
{
    unsigned res = 0;
    for(int k = 1; k < length; k++)
        res = (b[k] < b[res])? k : res;
    return res;
}

WEAK_FUNC
unsigned xs3_argmin_s32(
    const int32_t b[],
    const unsigned length)
{
    unsigned res = 0;
    for(int k = 1; k < length; k++)
        res = (b[k] < b[res])? k : res;
    return res;
}





WEAK_FUNC
int32_t xs3_abs_sum_s16(
    const int16_t* b,
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
int64_t xs3_abs_sum_s32(
    const int32_t* b,
    const unsigned length)
{
    int64_t acc = 0;
    for(int k = 0; k < length; k++){
        acc += (b[k] >= 0)? b[k] : -b[k];
    }
    return acc;
}