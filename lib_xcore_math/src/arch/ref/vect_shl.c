// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>

#include "xmath/xmath.h"
#include "vpu_helper.h"
#include "xmath/xs3/vpu_scalar_ops.h"


headroom_t vect_s16_shl(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const int shl)
{
    for(unsigned i = 0; i < length; i++){
        a[i] = vlashr16(b[i], -shl);
    }
    return vect_s16_headroom(a, length);
}




headroom_t vect_s32_shl(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const int shl)
{
    for(unsigned i = 0; i < length; i++){
        a[i] = vlashr32(b[i], -shl);
    }
    return vect_s32_headroom(a, length);
}
