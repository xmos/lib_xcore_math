// Copyright 2020 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "xs3_vpu_scalar_ops.h"



int32_t xs3_mul_s32(
    exponent_t* a_exp,
    const int32_t b,
    const int32_t c,
    const exponent_t b_exp,
    const exponent_t c_exp)
{
    int64_t p = ((int64_t)b) * c;

    const headroom_t p_hr = HR_S64(p);

    const right_shift_t p_shr = MAX( 0, 32 - ((int)p_hr));

    *a_exp = b_exp + c_exp + p_shr;

    if(p_shr)
        p = p + (1LL << (p_shr-1));

    p >>= p_shr;

    if(p == 0x80000000){
        p >>= 1;
        *a_exp += 1;
    }

    return (int32_t) p;
}




int32_t xs3_scalar_s64_to_s32(
    exponent_t* a_exp,
    const int64_t b,
    const exponent_t b_exp)
{
    const headroom_t b_hr = HR_S64(b);
    const right_shift_t shr = MAX( 0, (int)(32-b_hr) );
    *a_exp = b_exp + shr;
    return b >> shr;
}


int16_t xs3_scalar_s32_to_s16(
    exponent_t* a_exp,
    const int32_t b,
    const exponent_t b_exp)
{
    const headroom_t b_hr = HR_S32(b);
    const right_shift_t shr = MAX( 0, (int)(16-b_hr) );
    *a_exp = b_exp + shr;
    return b >> shr;
}
    
int32_t xs3_scalar_s16_to_s32(
    exponent_t* a_exp,
    const int16_t b,
    const exponent_t b_exp,
    const unsigned remove_hr)
{
    const left_shift_t shl = remove_hr? (16+HR_S16(b)) : 0;
    *a_exp = b_exp - shl;
    return ((int32_t)b) << shl;
}



int32_t xs3_inverse_s32(
    exponent_t* a_exp,
    const int32_t b)
{
    const headroom_t b_hr = HR_S32(b);

    const unsigned scale = 60 - b_hr;

    const int64_t dividend = 1LL << scale;

    *a_exp = -scale;

    return dividend / b;

}

int16_t min_abs_s16(
    const int16_t b[], 
    const unsigned length)
{
    int16_t m = INT16_MAX;

    for(int i = 0; i < length; i++){
        int16_t tmp = vlmul16(b[i], vsign16(b[i]));
        m = MIN(m, tmp);
    }

    return m;
}

int32_t min_abs_s32(
    const int32_t b[], 
    const unsigned length)
{
    int32_t m = INT32_MAX;

    for(int i = 0; i < length; i++){
        int32_t tmp = vlmul32(b[i], vsign32(b[i]));
        m = MIN(m, tmp);
    }

    return m;
}

