
#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "vpu_helper.h"



WEAK_FUNC
int32_t xs3_sum_s16(
    const int16_t* b,
    const unsigned length)
{
    int32_t acc = 0;
    for(int k = 0; k < length; k++){
        acc += b[k];
    }

    return acc;
}


WEAK_FUNC
int64_t xs3_sum_s32(
    const int32_t* b,
    const unsigned length)
{
    int64_t acc = 0;
    for(int k = 0; k < length; k++){
        acc += b[k];
    }

    return acc;
}