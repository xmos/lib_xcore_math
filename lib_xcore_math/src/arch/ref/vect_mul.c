// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>

#include "xmath/xmath.h"
#include "vpu_helper.h"
#include "xmath/xs3/vpu_scalar_ops.h"


headroom_t vect_s16_mul(
    int16_t a[],
    const int16_t b[],
    const int16_t c[],
    const unsigned length,
    const right_shift_t a_shr)
{

    for(unsigned k = 0; k < length; k++){
        const vpu_int16_acc_t acc = vlmacc16(0, b[k], c[k]);
        a[k] = vlsat16(acc, a_shr);
    }

    return vect_s16_headroom(a, length);
}



headroom_t vect_s32_mul(
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
        a[k] = vlmul32(B, C);
    }

    return vect_s32_headroom(a, length);
}

headroom_t vect_s16_scale(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const int16_t c,
    const right_shift_t a_shr)
{
    for(unsigned k = 0; k < length; k++){
        vpu_int16_acc_t acc = vlmacc16(0, b[k], c);
        a[k] = vlsat16(acc, a_shr);
    }

    return vect_s16_headroom(a, length);
}



headroom_t vect_s32_scale(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const int32_t c,
    const right_shift_t b_shr,
    const right_shift_t c_shr)
{
    int32_t C = vlashr32(c, c_shr);

    for(unsigned k = 0; k < length; k++){
        int32_t B = vlashr32(b[k], b_shr);
        a[k] = vlmul32(B, C);
    }

    return vect_s32_headroom(a, length);
}
