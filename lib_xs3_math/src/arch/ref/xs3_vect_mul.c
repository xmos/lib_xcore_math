
#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "../../vect/vpu_helper.h"






headroom_t xs3_vect_s16_mul(
    int16_t a[],
    const int16_t b[],
    const int16_t c[],
    const unsigned length,
    const right_shift_t a_shr)
{

    for(int k = 0; k < length; k++){
        
        int16_t B = b[k];
        int16_t C = c[k];

        int32_t P = ((int32_t)(B))*C;

        a[k] = SAT(16)(ROUND_SHR(P, a_shr));
    }

    return xs3_vect_s16_headroom(a, length);
}



headroom_t xs3_vect_s32_mul(
    int32_t a[],
    const int32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr)
{

    for(int k = 0; k < length; k++){
        
        int32_t B = ASHR(32)(b[k], b_shr);
        int32_t C = ASHR(32)(c[k], c_shr);

        int64_t P = ((int64_t)B)*C;

        a[k] = SAT(32)(ROUND_SHR(P, 30));
    }

    return xs3_vect_s32_headroom(a, length);
}



headroom_t xs3_vect_s16_scale(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const int16_t alpha,
    const right_shift_t b_shr)
{

    for(int k = 0; k < length; k++){
        
        int16_t B = ASHR(16)(b[k], b_shr);
        int32_t P = ((int32_t)(B))*alpha;
        a[k] = SAT(16)(ROUND_SHR(P, 14));
    }

    return xs3_vect_s16_headroom(a, length);
}



headroom_t xs3_vect_s32_scale(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const int32_t alpha,
    const right_shift_t b_shr)
{

    for(int k = 0; k < length; k++){
        
        int32_t B = ASHR(32)(b[k], b_shr);
        int64_t P = ((int64_t)(B))*alpha;
        a[k] = SAT(32)(ROUND_SHR(P, 30));
    }

    return xs3_vect_s32_headroom(a, length);
}