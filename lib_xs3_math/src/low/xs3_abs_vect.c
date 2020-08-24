
#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "vpu_helper.h"



WEAK_FUNC
headroom_t xs3_abs_vect_s16(
    int16_t* a,
    const int16_t* b,
    const unsigned length)
{

    for(int k = 0; k < length; k++){
        int16_t B = SAT(16)(b[k]);
        a[k] = (B >= 0)? B : -B;
    }

    return xs3_headroom_vect_s16(a, length);
}


WEAK_FUNC
headroom_t xs3_abs_vect_s32(
    int32_t* a,
    const int32_t* b,
    const unsigned length)
{

    for(int k = 0; k < length; k++){
        int32_t B = SAT(32)(b[k]);
        a[k] = (B >= 0)? B : -B;
    }

    return xs3_headroom_vect_s32(a, length);
}