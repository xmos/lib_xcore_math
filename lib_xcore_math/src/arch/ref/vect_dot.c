// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

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



int64_t vect_s32_dot(
    const int32_t b[],
    const int32_t c[],
    const unsigned length,
    const int b_shr,
    const int c_shr)
{

    vpu_int32_acc_t accs[VPU_INT32_EPV] = {0};

    for(unsigned k = 0; k < length; k++){

        int j = k % VPU_INT32_EPV;

        int32_t B = vlashr32(b[k], b_shr);
        int32_t C = vlashr32(c[k], c_shr);

        accs[j] = vlmacc32(accs[j], B, C);
    }

    int64_t res = 0;

    for(int k = 0; k < VPU_INT32_EPV; k++){
        res += accs[k];
    }

    return res;
}
