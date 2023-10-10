// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// XMOS Public License: Version 1

#include <stdint.h>
#include <stdio.h>

#include "xmath/xmath.h"
#include "vpu_helper.h"
#include "xmath/xs3/vpu_scalar_ops.h"


headroom_t vect_s16_macc(
    int16_t acc[],
    const int16_t b[],
    const int16_t c[],
    const unsigned length,
    const right_shift_t acc_shr,
    const right_shift_t bc_shr)
{
    for(unsigned k = 0; k < length; k++){
        acc[k] = vlashr16(acc[k], acc_shr);
        const vpu_int16_acc_t tmp = vlmacc16(0, b[k], c[k]);
        acc[k] = vladd16(acc[k], vlsat16(tmp, bc_shr));
    }

    return vect_s16_headroom(acc, length);
}

headroom_t vect_s16_nmacc(
    int16_t acc[],
    const int16_t b[],
    const int16_t c[],
    const unsigned length,
    const right_shift_t acc_shr,
    const right_shift_t bc_shr)
{

    for(unsigned k = 0; k < length; k++){
        acc[k] = vlashr16(acc[k], acc_shr);
        const vpu_int16_acc_t tmp = vlmacc16(0, b[k], c[k]);
        acc[k] = vlsub16(acc[k], vlsat16(tmp, bc_shr));
    }

    return vect_s16_headroom(acc, length);
}



headroom_t vect_s32_macc(
    int32_t acc[],
    const int32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t acc_shr,
    const right_shift_t b_shr,
    const right_shift_t c_shr)
{

    for(unsigned k = 0; k < length; k++){
        acc[k] = vlashr32(acc[k], acc_shr);
        const int32_t B = vlashr32(b[k], b_shr);
        const int32_t C = vlashr32(c[k], c_shr);
        acc[k] = vladd32(acc[k], vlmul32(B, C));
    }

    return vect_s32_headroom(acc, length);
}


headroom_t vect_s32_nmacc(
    int32_t acc[],
    const int32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t acc_shr,
    const right_shift_t b_shr,
    const right_shift_t c_shr)
{

    for(unsigned k = 0; k < length; k++){
        acc[k] = vlashr32(acc[k], acc_shr);
        const int32_t B = vlashr32(b[k], b_shr);
        const int32_t C = vlashr32(c[k], c_shr);
        acc[k] = vlsub32(acc[k], vlmul32(B, C));
    }

    return vect_s32_headroom(acc, length);
}

