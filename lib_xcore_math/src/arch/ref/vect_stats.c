// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>

#include "xmath/xmath.h"
#include "vpu_helper.h"
#include "xmath/xs3/vpu_scalar_ops.h"


int16_t vect_s16_max(
    const int16_t b[],
    const unsigned length)
{
    int16_t cur_max = INT16_MIN;
    for(unsigned k = 0; k < length; k++)
        cur_max = MAX(cur_max, b[k]);
    return cur_max;
}


headroom_t vect_s16_max_elementwise(
    int16_t a[],
    const int16_t b[],
    const int16_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr)
{
  for(unsigned k = 0; k < length; k++)
    a[k] = MAX( vlashr16(b[k], b_shr), vlashr16(c[k], c_shr));
  
  return vect_s16_headroom(a, length);
}



int32_t vect_s32_max(
    const int32_t b[],
    const unsigned length)
{
    int32_t cur_max = INT32_MIN;
    for(unsigned k = 0; k < length; k++){
        cur_max = MAX(cur_max, b[k]);
    }
    return cur_max;
}


headroom_t vect_s32_max_elementwise(
    int32_t a[],
    const int32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr)
{
  for(unsigned k = 0; k < length; k++)
    a[k] = MAX(vlashr32(b[k], b_shr), vlashr32(c[k], c_shr));

  return vect_s32_headroom(a, length);
}



int16_t vect_s16_min(
    const int16_t b[],
    const unsigned length)
{
    int16_t cur_min = INT16_MAX;
    for(unsigned k = 0; k < length; k++)
        cur_min = MIN(cur_min, b[k]);
    return cur_min;
}


headroom_t vect_s16_min_elementwise(
    int16_t a[],
    const int16_t b[],
    const int16_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr)
{
  for(unsigned k = 0; k < length; k++)
    a[k] = MIN(vlashr16(b[k], b_shr), vlashr16(c[k], c_shr));
    
  return vect_s16_headroom(a, length);
}



int32_t vect_s32_min(
    const int32_t b[],
    const unsigned length)
{
    int32_t cur_min = INT32_MAX;
    for(unsigned k = 0; k < length; k++){
        cur_min = MIN(cur_min, b[k]);
    }
    return cur_min;
}


headroom_t vect_s32_min_elementwise(
    int32_t a[],
    const int32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr)
{
  for(unsigned k = 0; k < length; k++)
    a[k] = MIN(vlashr32(b[k], b_shr), vlashr32(c[k], c_shr));
    
  return vect_s32_headroom(a, length);
}




unsigned vect_s16_argmax(
    const int16_t b[],
    const unsigned length)
{

    unsigned res = 0;
    for(unsigned k = 1; k < length; k++)
        res = (b[k] > b[res])? k : res;
    return res;
}


unsigned vect_s32_argmax(
    const int32_t b[],
    const unsigned length)
{
    unsigned res = 0;
    for(unsigned k = 1; k < length; k++)
        res = (b[k] > b[res])? k : res;
    return res;
}



unsigned vect_s16_argmin(
    const int16_t b[],
    const unsigned length)
{
    unsigned res = 0;
    for(unsigned k = 1; k < length; k++)
        res = (b[k] < b[res])? k : res;
    return res;
}


unsigned vect_s32_argmin(
    const int32_t b[],
    const unsigned length)
{
    unsigned res = 0;
    for(unsigned k = 1; k < length; k++)
        res = (b[k] < b[res])? k : res;
    return res;
}






int32_t vect_s16_abs_sum(
    const int16_t b[],
    const unsigned length)
{
    vpu_int16_acc_t acc[VPU_INT16_ACC_PERIOD] = {0};

    for(unsigned k = 0; k < length; k++){
        const int j = k % VPU_INT16_ACC_PERIOD;

        acc[j] = vlmacc16(acc[j], -1, b[k]);

        int16_t tmp = vpos16(b[k]);
        acc[j] = vlmacc16(acc[j], 2, tmp);
    }

    return vadddr16(acc);
}



int64_t vect_s32_abs_sum(
    const int32_t b[],
    const unsigned length)
{
    vpu_int32_acc_t acc[VPU_INT32_ACC_PERIOD] = { 0 };

    for(unsigned k = 0; k < length; k++){ 
        const int j = k % VPU_INT32_ACC_PERIOD;
        acc[j] = vlmacc32(acc[j], b[k], vsign32(b[k]));
    }

    vpu_int32_acc_t total = 0;
    for(unsigned j = 0; j < VPU_INT32_ACC_PERIOD; j++)
        total += acc[j];

    // note: xcore assembly implementation adds together the 40-bit accumulators and doesn't attempt
    //       to apply any saturation logic.

    return total;
}



int32_t vect_s16_energy(
    const int16_t b[],
    const unsigned length,
    const right_shift_t b_shr)
{

    vpu_int16_acc_t acc[VPU_INT16_ACC_PERIOD] = {0};

    for(unsigned k = 0; k < length; k++){
        const int j = k % VPU_INT16_ACC_PERIOD;
        const int16_t B = vlashr16(b[k], b_shr);
        acc[j] = vlmacc16(acc[j], B, B);
    }

    return vadddr16(acc);
}


int64_t vect_s32_energy(
    const int32_t b[],
    const unsigned length,
    const right_shift_t b_shr)
{
    vpu_int32_acc_t acc[VPU_INT32_ACC_PERIOD] = {0};

    for(unsigned k = 0; k < length; k++){
        const int j = k % VPU_INT32_ACC_PERIOD;
        const int32_t B = vlashr32(b[k], b_shr);
        acc[j] = vlmacc32(acc[j], B, B);
    }

    vpu_int32_acc_t total = 0;
    for(unsigned j = 0; j < VPU_INT32_ACC_PERIOD; j++)
        total += acc[j];

    // note: xcore assembly implementation adds together the 40-bit accumulators and doesn't attempt
    //       to apply any saturation logic.

    return total;
}
