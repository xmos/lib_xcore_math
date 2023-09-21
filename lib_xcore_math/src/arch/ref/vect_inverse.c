// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>

#include "xmath/xmath.h"
#include "../../vect/vpu_helper.h"
#include "xmath/xs3/vpu_scalar_ops.h"


void vect_s16_inverse(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const unsigned scale)
{
    const int32_t dividend = 1 << scale;
    for(unsigned k = 0; k < length; k++){
        a[k] = (dividend / b[k]);
    }
}

headroom_t vect_s32_inverse(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const unsigned scale)
{

    const int64_t d = (0x1LL << scale);

    for(unsigned k = 0; k < length; k++){
        a[k] = d / b[k];
        
        // printf("0x%016llX / %ld = %ld\n", (uint64_t) d, b[k], a[k]);
    }

    return vect_s32_headroom(a, length);
}
