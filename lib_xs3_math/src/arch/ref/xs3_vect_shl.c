// Copyright 2020 XMOS LIMITED. This Software is subject to the terms of the 
// XMOS Public License: Version 1

#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "../../vect/vpu_helper.h"
#include "xs3_vpu_scalar_ops.h"



headroom_t xs3_vect_s16_shl(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const int shl)
{
    for(int i = 0; i < length; i++){
        a[i] = vlashr16(b[i], -shl);
    }
    return xs3_vect_s16_headroom(a, length);
}




headroom_t xs3_vect_s32_shl(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const int shl)
{
    for(int i = 0; i < length; i++){
        a[i] = vlashr32(b[i], -shl);
    }
    return xs3_vect_s32_headroom(a, length);
}