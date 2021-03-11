// Copyright 2020 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "../../vect/vpu_helper.h"
#include "xs3_vpu_scalar_ops.h"





headroom_t xs3_vect_s16_sqrt(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const right_shift_t b_shr,
    const unsigned depth)
{
    

    for(int i = 0; i < length; i++){

        int16_t target = vlashr16(b[i], b_shr);

        a[i] = 0;

        int16_t P = 0x4000;

        for(int j = 0; j < depth; j++){

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

    return xs3_vect_s16_headroom(a, length);

}


headroom_t xs3_vect_s32_sqrt(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const right_shift_t b_shr,
    const unsigned depth)
{

    for(int i = 0; i < length; i++){

        int32_t target = vlashr32(b[i], b_shr);

        a[i] = 0;

        int32_t P = 0x40000000;

        for(int j = 0; j < depth; j++){

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

    return xs3_vect_s32_headroom(a, length);
}