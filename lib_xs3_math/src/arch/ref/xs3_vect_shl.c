
#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "../../vect/vpu_helper.h"




headroom_t xs3_vect_s16_shl(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const int shl)
{
    for(int i = 0; i < length; i++){
        int16_t B = b[i];
        a[i] = ASHR(16)(B, -shl);
    }
    return xs3_vect_s16_headroom(a, length);
}




headroom_t xs3_vect_s32_shl(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const int shl)
{
    for(int i = 0; i < length; i++){
        int32_t B = b[i];
        a[i] = ASHR(32)(B, -shl);
    }
    return xs3_vect_s32_headroom(a, length);
}