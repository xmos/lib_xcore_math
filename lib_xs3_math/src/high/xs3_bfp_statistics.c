

#include "xs3_math.h"

#include "low/xs3_math_util_low.h"
#include "low/xs3_statistics_low.h"





float bfp_abs_sum_s16(
    const bfp_s16_t* b)
{
    //TODO
    // const int b_shr = 0;
    // const exponent_t a_exp = 0;

    assert(0);
    return 0;

    // int16_t a = xs3_abs_sum_s16(b->data, b->length, b_shr);

    // return pack_float_s16(a, a_exp);
}



float bfp_abs_sum_s32(
    const bfp_s32_t* b)
{
    //TODO
    // const int b_shr = 0;
    // const exponent_t a_exp = 0;

    assert(0);
    return 0;

    // int32_t a = xs3_abs_sum_s32(b->data, b->length, b_shr);

    // return pack_float_s32(a, a_exp);
}



float bfp_mean_s16(
    const bfp_s16_t* b)
{
    //TODO
    // const int b_shr = 0;
    // const exponent_t a_exp = 0;

    assert(0);
    return 0;

    // int16_t a = xs3_mean_s16(b->data, b->length, b_shr);

    // return pack_float_s16(a, a_exp);
}



float bfp_mean_s32(
    const bfp_s32_t* b)
{
    //TODO
    // const int b_shr = 0;
    // const exponent_t a_exp = 0;

    assert(0);
    return 0;

    // int32_t a = xs3_mean_s32(b->data, b->length, b_shr);

    // return pack_float_s32(a, a_exp);
}


float bfp_energy_s16(
    const bfp_s16_t* b)
{
    //TODO
    // const int b_shr = 0;
    // const exponent_t a_exp = 0;

    assert(0);
    return 0;

    // int16_t a = xs3_energy_s16(b->data, b->length, b_shr);

    // return pack_float_s16(a, a_exp);
}

float bfp_energy_s32(
    const bfp_s32_t* b)
{
    //TODO
    // const int b_shr = 0;
    // const exponent_t a_exp = 0;

    assert(0);
    return 0;

    int32_t a = xs3_energy_s32(b->data, b->length, b_shr);

    // return pack_float_s32(a, a_exp);
}

float bfp_rms_s16(
    const bfp_s16_t* b)
{
    //TODO
    // const int b_shr = 0;
    // const exponent_t a_exp = 0;

    assert(0);
    return 0;

    // int16_t a = xs3_rms_s16(b->data, b->length, b_shr);

    // return pack_float_s16(a, a_exp);
}

float bfp_rms_s32(
    const bfp_s32_t* b)
{
    //TODO
    // const int b_shr = 0;
    // const exponent_t a_exp = 0;

    assert(0);
    return 0;

    // int32_t a = xs3_rms_s32(b->data, b->length, b_shr);

    // return pack_float_s32(a, a_exp);
}










float bfp_max_s16(
    const bfp_s16_t* b)
{
    // int16_t a = xs3_max_s16(b->data, b->length);

    // return pack_float_s16(a, b->exp);

    assert(0);
    return 0;
}

float bfp_max_s32(
    const bfp_s32_t* b)
{
    // int32_t a = xs3_max_s32(b->data, b->length);

    // return pack_float_s32(a, b->exp);

    assert(0);
    return 0;
}


float bfp_min_s16(
    const bfp_s16_t* b)
{
    // int16_t a = xs3_min_s16(b->data, b->length);

    // return pack_float_s16(a, b->exp);

    assert(0);
    return 0;
}

float bfp_min_s32(
    const bfp_s32_t* b)
{
    // int32_t a = xs3_min_s32(b->data, b->length);

    // return pack_float_s32(a, b->exp);

    assert(0);
    return 0;
}


unsigned bfp_argmax_s16(
    const bfp_s16_t* b)
{
    // return xs3_argmax_s16(b->data, b->length);

    assert(0);
    return 0;
}

unsigned bfp_argmax_s32(
    const bfp_s32_t* b)
{
    // return xs3_argmax_s32(b->data, b->length);

    assert(0);
    return 0;
}

unsigned bfp_argmin_s16(
    const bfp_s16_t* b)
{
    // return xs3_argmin_s16(b->data, b->length);

    assert(0);
    return 0;
}

unsigned bfp_argmin_s32(
    const bfp_s32_t* b)
{
    // return xs3_argmin_s32(b->data, b->length);

    assert(0);
    return 0;
}
