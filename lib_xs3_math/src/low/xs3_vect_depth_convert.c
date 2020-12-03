
#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "vpu_helper.h"



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


WEAK_FUNC
void xs3_vect_s32_to_s16(
    int16_t a[],
    const int32_t b[],
    const unsigned length,
    const right_shift_t b_shr)
{
    const right_shift_t b_shr_mod = b_shr - 16;

    for(int k = 0; k < length; k++){
        int64_t B = ASHR(32)(b[k], b_shr_mod);

        a[k] = SAT(16)(ROUND_SHR(B, 16));
    }
}



WEAK_FUNC
void xs3_vect_s16_to_s32(
    int32_t a[],
    const int16_t b[],
    const unsigned length)
{
    for(int k = 0; k < length; k++){
        int16_t B = b[k];
        a[k] = B << 8;
    }
}

