

#include "xs3_math.h"

#include "low/xs3_math_util_low.h"
#include "low/xs3_bfp_low.h"

#define ASSERT(COND)    assert(COND)





headroom_t bfp_vect_ch_pair_s16_headroom(
    bfp_ch_pair_s16_t* a)
{
    a->hr = xs3_vect_ch_pair_s16_headroom(a->data, a->length);
    return a->hr;
}


headroom_t bfp_vect_ch_pair_s32_headroom(
    bfp_ch_pair_s32_t* a)
{
    a->hr = xs3_vect_ch_pair_s32_headroom(a->data, a->length);
    return a->hr;
}

void bfp_vect_ch_pair_s16_shl(
    bfp_ch_pair_s16_t* a,
    const bfp_ch_pair_s16_t* b,
    const left_shift_t shl)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
#endif

    a->length = b->length;
    a->exp = b->exp;
    a->hr = xs3_vect_ch_pair_s16_shl(a->data, b->data, b->length, shl);
}

void bfp_vect_ch_pair_s32_shl(
    bfp_ch_pair_s32_t* a,
    const bfp_ch_pair_s32_t* b,
    const left_shift_t shl)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
#endif

    a->length = b->length;
    a->exp = b->exp;
    a->hr = xs3_vect_ch_pair_s32_shl(a->data, b->data, b->length, shl);
}