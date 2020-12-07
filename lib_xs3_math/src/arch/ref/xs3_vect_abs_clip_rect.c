
#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "../../vect/vpu_helper.h"




headroom_t xs3_vect_s16_abs(
    int16_t a[],
    const int16_t b[],
    const unsigned length)
{

    for(int k = 0; k < length; k++){
        int16_t B = SAT(16)(b[k]);
        a[k] = (B >= 0)? B : -B;
    }

    return xs3_vect_s16_headroom(a, length);
}



headroom_t xs3_vect_s32_abs(
    int32_t a[],
    const int32_t b[],
    const unsigned length)
{

    for(int k = 0; k < length; k++){
        int32_t B = SAT(32)(b[k]);
        a[k] = (B >= 0)? B : -B;
    }

    return xs3_vect_s32_headroom(a, length);
}





headroom_t xs3_vect_s16_clip(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const int16_t lower_bound,
    const int16_t upper_bound,
    const right_shift_t b_shr)
{
    for(int k = 0; k < length; k++){
        int16_t B = ASHR(16)(b[k], b_shr);
        a[k] = (B <= lower_bound)? lower_bound : (B >= upper_bound)? upper_bound : B;
    }

    return xs3_vect_s16_headroom(a, length);
}



headroom_t xs3_vect_s32_clip(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const int32_t lower_bound,
    const int32_t upper_bound,
    const right_shift_t b_shr)
{
    for(int k = 0; k < length; k++){
        int32_t B = ASHR(32)(b[k], b_shr);
        a[k] = (B <= lower_bound)? lower_bound : (B >= upper_bound)? upper_bound : B;
    }

    return xs3_vect_s32_headroom(a, length);
}



headroom_t xs3_vect_s16_rect(
    int16_t a[],
    const int16_t b[],
    const unsigned length)
{
    for(int k = 0; k < length; k++){
        int16_t B = b[k];
        a[k] = (B >= 0)? B : 0;
    }
    return xs3_vect_s16_headroom(a, length);
}



headroom_t xs3_vect_s32_rect(
    int32_t a[],
    const int32_t b[],
    const unsigned length)
{
    for(int k = 0; k < length; k++){
        int32_t B = b[k];
        a[k] = (B >= 0)? B : 0;
    }
    return xs3_vect_s32_headroom(a, length);
}