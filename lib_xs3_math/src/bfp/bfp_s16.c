

#include "bfp_math.h"

#include "vect/xs3_vect_s32.h"
#include "vect/xs3_vect_s16.h"

#include <assert.h>
#include <stdio.h>



    
headroom_t bfp_s16_headroom(
    bfp_s16_t* a)
{
     a->hr = xs3_vect_s16_headroom(a->data, a->length);

     return a->hr;
}


void bfp_s16_shl(
    bfp_s16_t* a,
    const bfp_s16_t* b,
    const left_shift_t shl)
{
#if XS3_BFP_DEBUG_CHECK_LENGTHS
    assert(a->length == b->length);
#endif

    a->length = b->length;
    a->exp = b->exp;
    a->hr = xs3_vect_s16_shl(a->data, b->data, b->length, shl);
}


void bfp_s16_add(
    bfp_s16_t* a, 
    const bfp_s16_t* b, 
    const bfp_s16_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == c->length);
    assert(b->length == a->length);
#else
    a->length = b->length;
#endif

    right_shift_t b_shr, c_shr;

    xs3_vect_add_sub_calc_params(&a->exp, &b_shr, &c_shr,
            b->exp, c->exp, b->hr, c->hr, XS3_BFP_ALLOW_SATURATION);

    a->hr = xs3_vect_s16_add(a->data, b->data, c->data, b->length, b_shr, c_shr);
}


void bfp_s16_sub(
    bfp_s16_t* a, 
    const bfp_s16_t* b, 
    const bfp_s16_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == c->length);
    assert(b->length == a->length);
#else
    a->length = b->length;
#endif

    right_shift_t b_shr, c_shr;

    xs3_vect_add_sub_calc_params(&a->exp, &b_shr, &c_shr, b->exp, c->exp, 
            b->hr, c->hr, XS3_BFP_ALLOW_SATURATION);

    a->hr = xs3_vect_s16_sub(a->data, b->data, c->data, b->length, b_shr, c_shr);
}


void bfp_s16_mul(
    bfp_s16_t* a, 
    const bfp_s16_t* b, 
    const bfp_s16_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == c->length);
    assert(b->length == a->length);
#else
    a->length = b->length;
#endif

    right_shift_t a_shr;

    xs3_vect_s16_mul_calc_params(&a->exp, &a_shr,
            b->exp, c->exp, b->hr, c->hr);

    a->hr = xs3_vect_s16_mul(a->data, b->data, c->data, b->length, a_shr);
}


void bfp_s16_scale(
    bfp_s16_t* a, 
    const bfp_s16_t* b, 
    const int16_t alpha_mant,
    const exponent_t alpha_exp)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
#else
    a->length = b->length;
#endif

    right_shift_t sat;
    headroom_t s_hr = HR_S16(alpha_mant);

    xs3_vect_s16_scale_calc_params(&a->exp, &sat, b->exp, alpha_exp, b->hr, s_hr,
                                        XS3_BFP_ALLOW_SATURATION);

    a->hr = xs3_vect_s16_scale(a->data, b->data, b->length, alpha_mant, sat);
}


void bfp_s16_abs(
    bfp_s16_t* a,
    const bfp_s16_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
#else
    a->length = b->length;
#endif

#if !XS3_BFP_ALLOW_SATURATION
    if(b->hr == 0){
        //shift down by one bit first
        xs3_vect_s16_shl(a->data, b->data, b->length, -1);
        a->exp = b->exp+1;
        a->hr = b->hr+1;
        b = a;
    }
#endif

    a->exp = b->exp;
    a->hr = xs3_vect_s16_abs(a->data, b->data, b->length);
}


int32_t bfp_s16_sum(
    const bfp_s16_t* b)
{
    return xs3_vect_s16_sum(b->data, b->length);
}


int64_t bfp_s16_dot(
    exponent_t* a_exp,
    const bfp_s16_t* b, 
    const bfp_s16_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == c->length);
#endif

    headroom_t bc_hr = b->hr + c->hr;

    *a_exp = b->exp + c->exp;

    return xs3_vect_s16_dot(b->data, c->data, b->length, bc_hr);
}


void bfp_s16_clip(
    bfp_s16_t* a, 
    const bfp_s16_t* b, 
    const int16_t lower_bound, 
    const int16_t upper_bound, 
    const int bound_exp)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
#else 
    a->length = b->length;
#endif

    assert(lower_bound <= upper_bound);

    // Suppose we say a->exp = b->exp. Then, we have to shift the bounds so that
    //  they match b->exp. So, bound_shr = b->exp - bound_exp. Two possibilities:
    //  A) bound_shr is negative (gets larger)
    //  B) bound_shr is non-negative (gets smaller (or stays same)

    // In case A, we shift the bound left. If upper_bound is positive and saturates, then of course all elements of b
    //  were already less than the upper bound. Likewise, if lower_bound is negative and saturates, then all elements of
    //  b were greater than the lower bound. If upper is negative and saturates or lower is positive and saturates, then
    //  we just set all of the elements of the output to upper or lower (accordingly), since nothing could possible be
    //  within the range specified.

    // In case B, we shift the bounds right, and we lose some precision on them, but that's it.
    
    exponent_t a_exp = b->exp;// - b->hr + XS3_BFP_ALLOW_SATURATION; //minimum b exponent
    
    right_shift_t bound_shr = a_exp - bound_exp;
    right_shift_t b_shr = a_exp - b->exp;

    int16_t lb;
    int16_t ub;

    if(bound_shr < 0){
        int32_t ub32 = ((int32_t)upper_bound) << (-bound_shr);
        int32_t lb32 = ((int32_t)lower_bound) << (-bound_shr);

        ub = (ub32 >= VPU_INT16_MAX)? VPU_INT16_MAX : (ub32 <= VPU_INT16_MIN)? VPU_INT16_MIN : ub32;
        lb = (lb32 >= VPU_INT16_MAX)? VPU_INT16_MAX : (lb32 <= VPU_INT16_MIN)? VPU_INT16_MIN : lb32;
    } else {
        // TODO: Should force upper_bound to round downwards to enforce the guarantee that no output can be larger than 
        // upper bound?
        ub = upper_bound >> bound_shr;
        // And lower bound upwards?
        lb = (lower_bound + ((1<<bound_shr)-1)) >> bound_shr;
    }

    if(ub == VPU_INT16_MIN){
        /* upper bound must be smaller than any element of b, so set everything to that */
        a->exp = bound_exp;
        a->hr = HR_S16(upper_bound);
        xs3_vect_s16_set(a->data, upper_bound, b->length);
    } else if(lb == VPU_INT16_MAX){
        /* lower bound must be larger than any element of b, so set everything to that */
        a->exp = bound_exp;
        a->hr = HR_S16(lower_bound);
        xs3_vect_s16_set(a->data, lower_bound, b->length);
    } else if(ub == lb){
        /* upper and lower bounds are indistinguishable */
        a->exp = a_exp;
        a->hr = HR_S16(ub);
        xs3_vect_s16_set(a->data, ub, b->length);

    } else {
        /* some elements may be between bounds */
        a->exp = a_exp;
        a->hr = xs3_vect_s16_clip(a->data, b->data, b->length, lb, ub, b_shr);
    }
}


void bfp_s16_rect(
    bfp_s16_t* a,
    const bfp_s16_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
#else 
    a->length = b->length;
#endif

    a->exp = b->exp;
    a->hr = xs3_vect_s16_rect(a->data, b->data, b->length);
}


void bfp_s16_sqrt(
    bfp_s16_t* a,
    const bfp_s16_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
#else 
    a->length = b->length;
#endif

    right_shift_t b_shr;

    xs3_vect_s16_sqrt_calc_params(&a->exp, &b_shr, b->exp, b->hr);

    a->hr = xs3_vect_s16_sqrt(a->data, b->data, b->length, b_shr, XS3_BFP_SQRT_DEPTH_S16);
}


void bfp_s16_inverse(
    bfp_s16_t* a,
    const bfp_s16_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
#else 
    a->length = b->length;
#endif

    unsigned scale;

    xs3_vect_s16_inverse_calc_params(&a->exp, &scale, b->data, b->exp, b->length);
    
    xs3_vect_s16_inverse(a->data, b->data, b->length, scale);
    
    bfp_s16_headroom(a);
}


int32_t bfp_s16_abs_sum(
    const bfp_s16_t* b)
{
    return xs3_vect_s16_abs_sum(b->data, b->length);
}


int16_t bfp_s16_mean(
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


int64_t bfp_s16_energy(
    exponent_t* a_exp,
    const bfp_s16_t* b)
{
    *a_exp = 2*b->exp;
    return xs3_vect_s16_dot(b->data, b->data, b->length, 2*b->hr);
}


int32_t bfp_s16_rms(
    exponent_t* a_exp,
    const bfp_s16_t* b)
{
    exponent_t exp, len_inv_exp;
    const int64_t energy64 = bfp_s16_energy(&exp, b);
    const int32_t energy32 = xs3_scalar_s64_to_s32(&exp, energy64, exp);
    const int32_t len_inv = xs3_inverse_s32(&len_inv_exp, b->length);
    const int32_t mean_energy = xs3_mul_s32(&exp, energy32, len_inv, exp, len_inv_exp);

    return xs3_sqrt_s32(a_exp, mean_energy, exp, XS3_BFP_SQRT_DEPTH_S32);
}


int16_t bfp_s16_max(
    const bfp_s16_t* b)
{
    return xs3_vect_s16_max(b->data, b->length);
}


int16_t bfp_s16_min(
    const bfp_s16_t* b)
{
    return xs3_vect_s16_min(b->data, b->length);
}


unsigned bfp_s16_argmax(
    const bfp_s16_t* b)
{
    return xs3_vect_s16_argmax(b->data, b->length);
}


unsigned bfp_s16_argmin(
    const bfp_s16_t* b)
{
    return xs3_vect_s16_argmin(b->data, b->length);
}


void bfp_s16_to_s32(
    bfp_s32_t* a,
    const bfp_s16_t* b)
{    
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
#else 
    a->length = b->length;
#endif

    //TODO - figure out how to actually do this..
    const exponent_t a_exp = b->exp;

    a->exp = a_exp - 8;
    a->hr = b->hr + 8;
    xs3_vect_s16_to_s32(a->data, b->data, b->length);
}
