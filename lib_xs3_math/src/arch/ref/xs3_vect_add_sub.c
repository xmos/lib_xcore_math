
#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "../../vect/vpu_helper.h"




headroom_t xs3_vect_s16_add(
    int16_t a[],
    const int16_t b[],
    const int16_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr)
{

    for(int k = 0; k < length; k++){
        
        int16_t B = ASHR(16)(b[k], b_shr);
        int16_t C = ASHR(16)(c[k], c_shr);

        a[k] = SAT(16)(((int32_t)B) + C);

    }

    return xs3_vect_s16_headroom(a, length);
}



headroom_t xs3_vect_s32_add(
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

        a[k] = SAT(32)(((int64_t)B) + C);
    }

    return xs3_vect_s32_headroom(a, length);
}





headroom_t xs3_vect_s16_sub(
    int16_t a[],
    const int16_t b[],
    const int16_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr)
{

    for(int k = 0; k < length; k++){
        
        int16_t B = ASHR(16)(b[k], b_shr);
        int16_t C = ASHR(16)(c[k], c_shr);

        a[k] = SAT(16)(((int32_t)B) - C);

    }

    return xs3_vect_s16_headroom(a, length);
}



headroom_t xs3_vect_s32_sub(
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

        a[k] = SAT(32)(((int64_t)B) - C);
    }

    return xs3_vect_s32_headroom(a, length);
}