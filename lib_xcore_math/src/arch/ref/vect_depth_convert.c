// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>

#include "xmath/xmath.h"
#include "vpu_helper.h"
#include "xmath/xs3/vpu_scalar_ops.h"


void vect_s32_to_vect_s16(
    int16_t a[],
    const int32_t b[],
    const unsigned length,
    const right_shift_t b_shr)
{
    //ASM uses VDEPTH16, which has an implicit 16-bit right-shift. To make it more intuitive, b_shr is specified so
    // that the user doesn't have to care about that.
    const right_shift_t b_shr_mod = b_shr - 16;

    for(unsigned k = 0; k < length; k++){
        const int32_t B = vlashr32(b[k], b_shr_mod);
        a[k] = vdepth16_32(B);
    }
}




void vect_s16_to_vect_s32(
    int32_t a[],
    const int16_t b[],
    const unsigned length)
{
    for(unsigned k = 0; k < length; k++){
        int16_t B = b[k];
        a[k] = B << 8;
    }
}

