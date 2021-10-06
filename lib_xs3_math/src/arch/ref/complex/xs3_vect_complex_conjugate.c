// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "../../../vect/vpu_helper.h"
#include "xs3_vpu_scalar_ops.h"



headroom_t xs3_vect_complex_s32_conjugate(
    complex_s32_t a[],
    const complex_s32_t b[],
    const unsigned length)
{
    for(int k = 0; k < length; k++){
        a[k].re = vlmul32(b[k].re,  0x40000000);
        a[k].im = vlmul32(b[k].im, -0x40000000);
    }

    return xs3_vect_complex_s32_headroom(a, length);
}



