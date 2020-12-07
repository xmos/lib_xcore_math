
#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "../../vect/vpu_helper.h"
#include "xs3_vpu_scalar_ops.h"






void xs3_vect_s16_inverse(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const unsigned scale)
{
    const int32_t dividend = 1 << scale;
    for(int k = 0; k < length; k++){
        a[k] = (dividend / b[k]);
    }
}





headroom_t xs3_vect_s32_inverse(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const unsigned scale)
{

    const int64_t d = (0x1LL << scale);

    for(int k = 0; k < length; k++){
        a[k] = d / b[k];
        
        // printf("0x%016llX / %ld = %ld\n", (uint64_t) d, b[k], a[k]);
    }

    return xs3_vect_s32_headroom(a, length);
}
