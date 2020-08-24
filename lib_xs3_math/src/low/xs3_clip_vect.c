
#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "vpu_helper.h"



WEAK_FUNC
headroom_t xs3_clip_vect_s16(
    int16_t* a,
    const int16_t* b,
    const unsigned length,
    const int16_t lower_bound,
    const int16_t upper_bound,
    const right_shift_t b_shr)
{
    for(int k = 0; k < length; k++){
        int16_t B = ASHR(16)(b[k], b_shr);
        a[k] = (B <= lower_bound)? lower_bound : (B >= upper_bound)? upper_bound : B;
    }

    return xs3_headroom_vect_s16(a, length);
}


WEAK_FUNC
headroom_t xs3_clip_vect_s32(
    int32_t* a,
    const int32_t* b,
    const unsigned length,
    const int32_t lower_bound,
    const int32_t upper_bound,
    const right_shift_t b_shr)
{
    for(int k = 0; k < length; k++){
        int32_t B = ASHR(32)(b[k], b_shr);
        a[k] = (B <= lower_bound)? lower_bound : (B >= upper_bound)? upper_bound : B;
    }

    return xs3_headroom_vect_s32(a, length);
}