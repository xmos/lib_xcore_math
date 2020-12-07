
#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "../../vect/vpu_helper.h"





void xs3_vect_s32_to_s16(
    int16_t a[],
    const int32_t b[],
    const unsigned length,
    const right_shift_t b_shr)
{
    const right_shift_t b_shr_mod = b_shr - 16;

    for(int k = 0; k < length; k++){
        int64_t B = ASHR(32)(b[k], b_shr_mod);

        a[k] = SAT(16)(ROUND_SHR(B, 16));
    }
}




void xs3_vect_s16_to_s32(
    int32_t a[],
    const int16_t b[],
    const unsigned length)
{
    for(int k = 0; k < length; k++){
        int16_t B = b[k];
        a[k] = B << 8;
    }
}

