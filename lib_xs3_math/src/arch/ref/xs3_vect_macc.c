// Copyright 2020 XMOS LIMITED. This Software is subject to the terms of the 
// XMOS Public License: Version 1

#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "../../vect/vpu_helper.h"
#include "xs3_vpu_scalar_ops.h"






headroom_t xs3_vect_s16_macc(
    int16_t acc[],
    const int16_t b[],
    const int16_t c[],
    const unsigned length,
    const right_shift_t a_shr)
{

    for(int k = 0; k < length; k++){
        const vpu_int16_acc_t tmp = vlmacc16(0, b[k], c[k]);
        acc[k] = vladd16(acc[k], vlsat16(tmp, a_shr));
    }

    return xs3_vect_s16_headroom(acc, length);
}

headroom_t xs3_vect_s16_nmacc(
    int16_t acc[],
    const int16_t b[],
    const int16_t c[],
    const unsigned length,
    const right_shift_t a_shr)
{

    for(int k = 0; k < length; k++){
        const vpu_int16_acc_t tmp = vlmacc16(0, b[k], c[k]);
        acc[k] = vlsub16(acc[k], vlsat16(tmp, a_shr));
    }

    return xs3_vect_s16_headroom(acc, length);
}



headroom_t xs3_vect_s32_macc(
    int32_t acc[],
    const int32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t acc_shr,
    const right_shift_t b_shr,
    const right_shift_t c_shr)
{

    for(int k = 0; k < length; k++){
        const int32_t B = vlashr32(b[k], b_shr);
        const int32_t C = vlashr32(c[k], c_shr);
        acc[k] = vladd32(acc[k], vlmul32(B, C));
    }

    return xs3_vect_s32_headroom(acc, length);
}


headroom_t xs3_vect_s32_nmacc(
    int32_t acc[],
    const int32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t acc_shr,
    const right_shift_t b_shr,
    const right_shift_t c_shr)
{

    for(int k = 0; k < length; k++){
        const int32_t B = vlashr32(b[k], b_shr);
        const int32_t C = vlashr32(c[k], c_shr);
        acc[k] = vlsub32(acc[k], vlmul32(B, C));
    }

    return xs3_vect_s32_headroom(acc, length);
}

