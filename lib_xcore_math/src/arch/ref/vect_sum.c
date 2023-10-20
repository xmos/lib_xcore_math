// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>

#include "xmath/xmath.h"
#include "vpu_helper.h"
#include "xmath/xs3/vpu_scalar_ops.h"
#include "vpu_const_vects.h"


const int32_t one_q30 = 0x40000000;

int32_t vect_s16_sum(
    const int16_t b[],
    const unsigned length)
{
    vpu_int16_acc_t acc = 0;
    for(unsigned k = 0; k < length; k++){
        acc = vlmacc16(acc, b[k], 1);
    }

    return acc;
}



int64_t vect_s32_sum(
    const int32_t b[],
    const unsigned length)
{
    vpu_int32_acc_t acc = 0;
    for(unsigned k = 0; k < length; k++){
        acc = vlmacc32(acc, b[k], one_q30);
    }

    return acc;
}
