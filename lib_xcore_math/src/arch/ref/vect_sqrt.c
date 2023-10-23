// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>

#include "xmath/xmath.h"
#include "vpu_helper.h"
#include "xmath/xs3/vpu_scalar_ops.h"


headroom_t vect_s16_sqrt(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const right_shift_t b_shr,
    const unsigned depth)
{
    for(unsigned i = 0; i < length; i++){

        int16_t target = vlashr16(b[i], b_shr);

        a[i] = 0;

        int16_t P = 0x4000;

        for(unsigned j = 0; j < depth; j++){

            int16_t tmp;

            tmp = vladd16(a[i], P);
            tmp = vlmul16(tmp, tmp);
            tmp = vlsub16(target, tmp);
            tmp = vsign16(tmp);
            tmp = vpos16(tmp);
            tmp = vlmul16(tmp, P);
            a[i] = vladd16(a[i], tmp);

            P = vlashr16(P, 1);
        }
    }

    return vect_s16_headroom(a, length);

}


headroom_t vect_s32_sqrt(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const right_shift_t b_shr,
    const unsigned depth)
{

    for(unsigned i = 0; i < length; i++){

        int32_t target = vlashr32(b[i], b_shr);

        a[i] = 0;

        int32_t P = 0x40000000;

        for(unsigned j = 0; j < depth; j++){

            int32_t tmp;

            tmp = vladd32(a[i], P);
            tmp = vlmul32(tmp, tmp);
            tmp = vlsub32(target, tmp);
            tmp = vsign32(tmp);
            tmp = vpos32(tmp);
            tmp = vlmul32(tmp, P);
            a[i] = vladd32(a[i], tmp);

            P = vlashr32(P, 1);
        }
    }

    return vect_s32_headroom(a, length);
}
