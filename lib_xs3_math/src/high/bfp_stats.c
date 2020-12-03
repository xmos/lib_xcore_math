

#include "xs3_math.h"

#include "low/xs3_math_util_low.h"
#include "low/xs3_statistics_low.h"





int32_t bfp_vect_s16_abs_sum(
    const bfp_s16_t* b)
{
    return xs3_vect_s16_abs_sum(b->data, b->length);
}



int64_t bfp_vect_s32_abs_sum(
    const bfp_s32_t* b)
{
    return xs3_vect_s32_abs_sum(b->data, b->length);
}



int16_t bfp_vect_s16_mean(
    exponent_t* a_exp,
    const bfp_s16_t* b)
{
    int32_t sum = xs3_vect_s16_sum(b->data, b->length);

    headroom_t hr = HR_S32(sum) + 32;
    int64_t sum64 = ((int64_t)sum) << hr;
    int64_t mean64 = sum64 / ((int)b->length);
    right_shift_t shr = MAX(0, 48 - HR_S64(mean64));

    if(shr > 0) 
        mean64 += 1 << (shr-1);

    int16_t res = mean64 >> shr;

    *a_exp = b->exp - hr + shr;

    return res;
}


int32_t bfp_vect_s32_mean(
    exponent_t* a_exp,
    const bfp_s32_t* b)
{
    int64_t sum = xs3_vect_s32_sum(b->data, b->length);
    
    headroom_t hr = HR_S64(sum);
    sum = sum << hr;
    int64_t mean = sum / ((int)b->length);
    right_shift_t shr = MAX(0, 32 - HR_S64(mean));

    if(shr > 0)
        mean += 1 << (shr-1);
    
    int32_t res = mean >> shr;

    *a_exp = b->exp - hr + shr;

    return res;

}

int64_t bfp_vect_s16_energy(
    exponent_t* a_exp,
    const bfp_s16_t* b)
{
    *a_exp = 2*b->exp;
    return xs3_vect_s16_dot(b->data, b->data, b->length, 2*b->hr);
}

int64_t bfp_vect_s32_energy(
    exponent_t* a_exp,
    const bfp_s32_t* b)
{
    right_shift_t b_shr;
    xs3_vect_s32_energy_calc_params(a_exp, &b_shr, b->length, b->exp, b->hr);

    assert(b_shr + ((int)b->hr) >= 0);
    return xs3_vect_s32_energy(b->data, b->length, b_shr);
}








int32_t bfp_vect_s16_rms(
    exponent_t* a_exp,
    const bfp_s16_t* b)
{
    exponent_t exp, len_inv_exp;
    const int64_t energy64 = bfp_vect_s16_energy(&exp, b);
    const int32_t energy32 = xs3_scalar_s64_to_s32(&exp, energy64, exp);
    const int32_t len_inv = xs3_inverse_s32(&len_inv_exp, b->length);
    const int32_t mean_energy = xs3_mul_s32(&exp, energy32, len_inv, exp, len_inv_exp);

    return xs3_sqrt_s32(a_exp, mean_energy, exp, XS3_BFP_SQRT_DEPTH_S32);
}

int32_t bfp_vect_s32_rms(
    exponent_t* a_exp,
    const bfp_s32_t* b)
{
    exponent_t exp, len_inv_exp;
    const int64_t energy64 = bfp_vect_s32_energy(&exp, b);
    const int32_t energy32 = xs3_scalar_s64_to_s32(&exp, energy64, exp);
    const int32_t len_inv = xs3_inverse_s32(&len_inv_exp, b->length);
    const int32_t mean_energy = xs3_mul_s32(&exp, energy32, len_inv, exp, len_inv_exp);

    return xs3_sqrt_s32(a_exp, mean_energy, exp, XS3_BFP_SQRT_DEPTH_S32);
}










int16_t bfp_vect_s16_max(
    const bfp_s16_t* b)
{
    return xs3_vect_s16_max(b->data, b->length);
}

int32_t bfp_vect_s32_max(
    const bfp_s32_t* b)
{
    return xs3_vect_s32_max(b->data, b->length);
}


int16_t bfp_vect_s16_min(
    const bfp_s16_t* b)
{
    return xs3_vect_s16_min(b->data, b->length);
}

int32_t bfp_vect_s32_min(
    const bfp_s32_t* b)
{
    return xs3_vect_s32_min(b->data, b->length);
}


unsigned bfp_vect_s16_argmax(
    const bfp_s16_t* b)
{
    return xs3_vect_s16_argmax(b->data, b->length);
}

unsigned bfp_vect_s32_argmax(
    const bfp_s32_t* b)
{
    return xs3_vect_s32_argmax(b->data, b->length);
}

unsigned bfp_vect_s16_argmin(
    const bfp_s16_t* b)
{
    return xs3_vect_s16_argmin(b->data, b->length);
}

unsigned bfp_vect_s32_argmin(
    const bfp_s32_t* b)
{
    return xs3_vect_s32_argmin(b->data, b->length);
}
