// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>

#include "xmath/xmath.h"
#include "vpu_helper.h"
#include "xmath/xs3/vpu_scalar_ops.h"



headroom_t vect_complex_s32_conjugate(
    complex_s32_t a[],
    const complex_s32_t b[],
    const unsigned length)
{
    for(unsigned k = 0; k < length; k++){
        a[k].re = vlmul32(b[k].re,  0x40000000);
        a[k].im = vlmul32(b[k].im, -0x40000000);
    }

    return vect_complex_s32_headroom(a, length);
}



