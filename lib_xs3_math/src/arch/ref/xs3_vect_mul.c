// Copyright 2021 XMOS LIMITED. This Software is subject to the terms of the 
// XMOS Public License: Version 1

#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "../../vect/vpu_helper.h"
#include "xs3_vpu_scalar_ops.h"






headroom_t xs3_vect_s16_mul(
    int16_t a[],
    const int16_t b[],
    const int16_t c[],
    const unsigned length,
    const right_shift_t a_shr)
{

    for(int k = 0; k < length; k++){
        const vpu_int16_acc_t acc = vlmacc16(0, b[k], c[k]);
        a[k] = vlsat16(acc, a_shr);
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
        const int32_t B = vlashr32(b[k], b_shr);
        const int32_t C = vlashr32(c[k], c_shr);
        a[k] = vlmul32(B, C);
    }

    return xs3_vect_s32_headroom(a, length);
}



headroom_t xs3_vect_s16_scale(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const int16_t c,
    const right_shift_t a_shr)
{
    for(int k = 0; k < length; k++){
        vpu_int16_acc_t acc = vlmacc16(0, b[k], c);
        a[k] = vlsat16(acc, a_shr);
    }

    return xs3_vect_s16_headroom(a, length);
}



headroom_t xs3_vect_s32_scale(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const int32_t c,
    const right_shift_t b_shr,
    const right_shift_t c_shr)
{
    int32_t C = vlashr32(c, c_shr);

    for(int k = 0; k < length; k++){
        int32_t B = vlashr32(b[k], b_shr);
        a[k] = vlmul32(B, C);
    }

    return xs3_vect_s32_headroom(a, length);
}