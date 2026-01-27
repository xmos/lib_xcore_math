// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#if defined (__VX4B__)

#include <stdint.h>
#include <stdio.h>

#include "xmath/xmath.h"
#include "vpu_helper.h"
#include "xmath/xs3/vpu_scalar_ops.h"



int64_t vect_s16_dot(
    const int16_t b[],
    const int16_t c[],
    const unsigned length)
{
    //#warn vect_s16_dot is not yet optimised for vx4b.
    
    // Note: instead of using the 32-bit accumulators for this, the assembly version of this function implements
    //       makeshift 48-bit accumulators, which is why this is using a 64-bit int for accumulation.
    vpu_int32_acc_t acc = 0;

    const int64_t upper_sat_bound = 0x7FFFFFFFFFFFLL;
    const int64_t lower_sat_bound = -upper_sat_bound;

    for(unsigned k = 0; k < length; k++){
        acc += vlmacc16(0, b[k], c[k]);
        acc = MAX(lower_sat_bound, MIN(upper_sat_bound, acc));
    }

    return acc;
}

#endif