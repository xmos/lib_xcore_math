

#include "xs3_math.h"

#include "low/xs3_math_util_low.h"
#include "low/xs3_bfp_low.h"

#define ASSERT(COND)    assert(COND)



headroom_t xs3_headroom_ch_pair_s16(
    const ch_pair_s16_t a)
{
    complex_s16_t* b = (complex_s16_t*) &a;
    return HR_C16( *b);
}


headroom_t xs3_headroom_ch_pair_s32(
    const ch_pair_s32_t a)
{
    complex_s32_t* b = (complex_s32_t*) &a;
    return HR_C32( *b);
}



headroom_t bfp_headroom_vect_ch_pair_s16(
    bfp_ch_pair_s16_t* a)
{
    a->hr = xs3_headroom_vect_ch_pair_s16(a->data, a->length);
    return a->hr;
}


headroom_t bfp_headroom_vect_ch_pair_s32(
    bfp_ch_pair_s32_t* a)
{
    a->hr = xs3_headroom_vect_ch_pair_s32(a->data, a->length);
    return a->hr;
}

void bfp_shl_vect_ch_pair_s16(
    bfp_ch_pair_s16_t* a,
    const bfp_ch_pair_s16_t* b,
    const left_shift_t shl)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
#endif

    a->length = b->length;
    a->exp = b->exp;
    a->hr = xs3_shl_vect_ch_pair_s16(a->data, b->data, b->length, shl);
}

void bfp_shl_vect_ch_pair_s32(
    bfp_ch_pair_s32_t* a,
    const bfp_ch_pair_s32_t* b,
    const left_shift_t shl)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
#endif

    a->length = b->length;
    a->exp = b->exp;
    a->hr = xs3_shl_vect_ch_pair_s32(a->data, b->data, b->length, shl);
}