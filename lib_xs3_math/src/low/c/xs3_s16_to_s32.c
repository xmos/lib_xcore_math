
#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "vpu_helper.h"



WEAK_FUNC
void xs3_s16_to_s32(
    int32_t* a,
    const int16_t* b,
    const unsigned length)
{
    for(int k = 0; k < length; k++){
        int16_t B = b[k];
        a[k] = B << 8;
    }
}

