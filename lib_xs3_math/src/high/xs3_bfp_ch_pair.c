

#include "xs3_math.h"

#include "low/xs3_math_util_low.h"
#include "low/xs3_bfp_low.h"

#define ASSERT(COND)    assert(COND)



headroom_t xs3_cls_ch_pair_s16(
    const ch_pair_s16_t a)
{
    complex_s16_t* b = (complex_s16_t*) &a;
    return CLS_C16( *b);
}


headroom_t xs3_cls_ch_pair_s32(
    const ch_pair_s32_t a)
{
    complex_s32_t* b = (complex_s32_t*) &a;
    return CLS_C32( *b);
}



headroom_t bfp_cls_vect_ch_pair_s16(
    bfp_ch_pair_s16_t* a)
{
    a->hr = xs3_cls_array_s16((int16_t*)a->data, 2 * a->length);
    return a->hr;
}


headroom_t bfp_cls_vect_ch_pair_s32(
    bfp_ch_pair_s32_t* a)
{
    a->hr = xs3_cls_array_s32((int32_t*)a->data, 2 * a->length);
    return a->hr;
}

void bfp_ldexp_vect_ch_pair_s16(
    bfp_ch_pair_s16_t* a,
    const bfp_ch_pair_s16_t* b,
    const exponent_t exp)
{
    a->length = b->length;
    a->exp = b->exp;
    a->hr = xs3_shl_vect_s16((int16_t*) a->data, (int16_t*) b->data, 2*b->length, exp);
}

void bfp_ldexp_vect_ch_pair_s32(
    bfp_ch_pair_s32_t* a,
    const bfp_ch_pair_s32_t* b,
    const exponent_t exp)
{
    a->length = b->length;
    a->exp = b->exp;
    a->hr = xs3_shl_vect_s32((int32_t*) a->data, (int32_t*) b->data, 2*b->length, exp);
}