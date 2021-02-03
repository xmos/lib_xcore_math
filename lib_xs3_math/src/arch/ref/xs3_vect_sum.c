// Copyright 2020 XMOS LIMITED. This Software is subject to the terms of the 
// XMOS Public License: Version 1

#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "../../vect/vpu_helper.h"
#include "xs3_vpu_scalar_ops.h"
#include "../../vect/vpu_const_vects.h"


const int32_t one_q30 = 0x40000000;

int32_t xs3_vect_s16_sum(
    const int16_t b[],
    const unsigned length)
{
    vpu_int16_acc_t acc = 0;
    for(int k = 0; k < length; k++){
        acc = vlmacc16(acc, b[k], 1);
    }

    return acc;
}



int64_t xs3_vect_s32_sum(
    const int32_t b[],
    const unsigned length)
{
    vpu_int32_acc_t acc = 0;
    for(int k = 0; k < length; k++){
        acc = vlmacc32(acc, b[k], one_q30);
    }

    return acc;
}