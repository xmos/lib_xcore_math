// Copyright 2020 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public License: Version 1.

#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "../../vect/vpu_helper.h"
#include "xs3_vpu_scalar_ops.h"


static const int32_t one_q30 = 0x40000000;


int16_t xs3_vect_s16_max(
    const int16_t b[],
    const unsigned length)
{
    int16_t cur_max = INT16_MIN;
    for(int k = 0; k < length; k++)
        cur_max = MAX(cur_max, b[k]);
    return cur_max;
}



int32_t xs3_vect_s32_max(
    const int32_t b[],
    const unsigned length)
{
    int32_t cur_max = INT32_MIN;
    for(int k = 0; k < length; k++){
        cur_max = MAX(cur_max, b[k]);
    }
    return cur_max;
}



int16_t xs3_vect_s16_min(
    const int16_t b[],
    const unsigned length)
{
    int16_t cur_min = INT16_MAX;
    for(int k = 0; k < length; k++)
        cur_min = MIN(cur_min, b[k]);
    return cur_min;
}



int32_t xs3_vect_s32_min(
    const int32_t b[],
    const unsigned length)
{
    int32_t cur_min = INT32_MAX;
    for(int k = 0; k < length; k++){
        cur_min = MIN(cur_min, b[k]);
    }
    return cur_min;
}




unsigned xs3_vect_s16_argmax(
    const int16_t b[],
    const unsigned length)
{

    unsigned res = 0;
    for(int k = 1; k < length; k++)
        res = (b[k] > b[res])? k : res;
    return res;
}


unsigned xs3_vect_s32_argmax(
    const int32_t b[],
    const unsigned length)
{
    unsigned res = 0;
    for(int k = 1; k < length; k++)
        res = (b[k] > b[res])? k : res;
    return res;
}



unsigned xs3_vect_s16_argmin(
    const int16_t b[],
    const unsigned length)
{
    unsigned res = 0;
    for(int k = 1; k < length; k++)
        res = (b[k] < b[res])? k : res;
    return res;
}


unsigned xs3_vect_s32_argmin(
    const int32_t b[],
    const unsigned length)
{
    unsigned res = 0;
    for(int k = 1; k < length; k++)
        res = (b[k] < b[res])? k : res;
    return res;
}






int32_t xs3_vect_s16_abs_sum(
    const int16_t b[],
    const unsigned length)
{
    vpu_int16_acc_t acc[VPU_INT16_ACC_PERIOD] = {0};

    for(int k = 0; k < length; k++){
        const int j = k % VPU_INT16_ACC_PERIOD;
        int16_t B = vlmul16(b[k], vsign16(b[k]));
        acc[j] = vlmacc16(acc[j], B, 1);
    }

    return vadddr16(acc);
}



int64_t xs3_vect_s32_abs_sum(
    const int32_t b[],
    const unsigned length)
{
    vpu_int32_acc_t acc[VPU_INT32_ACC_PERIOD] = { 0 };

    for(int k = 0; k < length; k++){ 
        const int j = k % VPU_INT32_ACC_PERIOD;
        acc[j] = vlmacc32(acc[j], b[k], vsign32(b[k]));
    }

    vpu_int32_acc_t total = 0;
    for(int j = 0; j < VPU_INT32_ACC_PERIOD; j++)
        total += acc[j];

    // note: xcore assembly implementation adds together the 40-bit accumulators and doesn't attempt
    //       to apply any saturation logic.

    return total;
}



int32_t xs3_vect_s16_energy(
    const int16_t b[],
    const unsigned length,
    const right_shift_t b_shr)
{

    vpu_int16_acc_t acc[VPU_INT16_ACC_PERIOD] = {0};

    for(int k = 0; k < length; k++){
        const int j = k % VPU_INT16_ACC_PERIOD;
        const int16_t B = vlashr16(b[k], b_shr);
        acc[j] = vlmacc16(acc[j], B, B);
    }

    return vadddr16(acc);
}


int64_t xs3_vect_s32_energy(
    const int32_t b[],
    const unsigned length,
    const right_shift_t b_shr)
{
    vpu_int32_acc_t acc[VPU_INT32_ACC_PERIOD] = {0};

    for(int k = 0; k < length; k++){
        const int j = k % VPU_INT32_ACC_PERIOD;
        const int32_t B = vlashr32(b[k], b_shr);
        acc[j] = vlmacc32(acc[j], B, B);
    }

    vpu_int32_acc_t total = 0;
    for(int j = 0; j < VPU_INT32_ACC_PERIOD; j++)
        total += acc[j];

    // note: xcore assembly implementation adds together the 40-bit accumulators and doesn't attempt
    //       to apply any saturation logic.

    return total;
}