
#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "vpu_helper.h"



WEAK_FUNC
headroom_t xs3_shl_vect_s16(
    int16_t* a,
    const int16_t* b,
    const unsigned length,
    const int shl)
{
    for(int i = 0; i < length; i++){
        int16_t B = b[i];
        a[i] = ASHR(16)(B, -shl);
    }
    return xs3_cls_array_s16(a, length);
}



WEAK_FUNC
headroom_t xs3_shl_vect_s32(
    int32_t* a,
    const int32_t* b,
    const unsigned length,
    const int shl)
{
    for(int i = 0; i < length; i++){
        int32_t B = b[i];
        a[i] = ASHR(32)(B, -shl);
    }
    return xs3_cls_array_s32(a, length);
}