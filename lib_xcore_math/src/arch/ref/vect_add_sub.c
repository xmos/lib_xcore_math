// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>

#include "xmath/xmath.h"
#include "xmath/xs3/vpu_scalar_ops.h"




headroom_t vect_s16_add(
    int16_t a[],
    const int16_t b[],
    const int16_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr)
{
    for(unsigned k = 0; k < length; k++){        
        const int16_t B = vlashr16(b[k], b_shr);
        const int16_t C = vlashr16(c[k], c_shr);
        a[k] = vladd16(B, C);
    }

    return vect_s16_headroom(a, length);
}



headroom_t vect_s32_add(
    int32_t a[],
    const int32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr)
{

    for(unsigned k = 0; k < length; k++){
        const int32_t B = vlashr32(b[k], b_shr);
        const int32_t C = vlashr32(c[k], c_shr);
        a[k] = vladd32(B, C);
    }

    return vect_s32_headroom(a, length);
}





headroom_t vect_s16_sub(
    int16_t a[],
    const int16_t b[],
    const int16_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr)
{

    for(unsigned k = 0; k < length; k++){
        const int16_t B = vlashr16(b[k], b_shr);
        const int16_t C = vlashr16(c[k], c_shr);
        a[k] = vlsub16(B, C);
    }

    return vect_s16_headroom(a, length);
}



headroom_t vect_s32_sub(
    int32_t a[],
    const int32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr)
{

    for(unsigned k = 0; k < length; k++){
        const int32_t B = vlashr32(b[k], b_shr);
        const int32_t C = vlashr32(c[k], c_shr);
        a[k] = vlsub32(B, C);
    }

    return vect_s32_headroom(a, length);
}
