

#include "xs3_math.h"

#include "low/xs3_math_util_low.h"
#include "low/xs3_bfp_low.h"

#include <assert.h>
#include <stdio.h>


    
/* ******************
 *
 * 
 * ******************/
headroom_t xs3_headroom_s64(
    const int64_t a)
{
    int32_t* tmp = (int32_t*) &a;

    if( (tmp[0] == 0) || (tmp[0] == -1)){
        return cls(tmp[1]);
    } else {
        return cls(tmp[0]) + 32;
    }
}



    
/* ******************
 *
 * 
 * ******************/
headroom_t bfp_vect_s16_headroom(
    bfp_s16_t* a)
{
     a->hr = xs3_vect_s16_headroom(a->data, a->length);

     return a->hr;
}



    
/* ******************
 *
 * 
 * ******************/
headroom_t bfp_vect_s32_headroom(
    bfp_s32_t* a)
{
     a->hr = xs3_vect_s32_headroom(a->data, a->length);

     return a->hr;
}


    
/* ******************
 *
 * 
 * ******************/
void bfp_vect_s16_shl(
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


    
/* ******************
 *
 * 
 * ******************/
void bfp_vect_s32_shl(
    bfp_s32_t* a,
    const bfp_s32_t* b,
    const left_shift_t shl)
{
#if XS3_BFP_DEBUG_CHECK_LENGTHS
    assert(a->length == b->length);
#endif
    a->length = b->length;
    a->exp = b->exp;
    a->hr = xs3_vect_s32_shl(a->data, b->data, b->length, shl);
}




/* ******************
 *
 * 
 * ******************/
void bfp_vect_s16_add(
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

    xs3_add_sub_vect_calc_params(&a->exp, &b_shr, &c_shr,
            b->exp, c->exp, b->hr, c->hr, XS3_BFP_ALLOW_SATURATION);

    a->hr = xs3_vect_s16_add(a->data, b->data, c->data, b->length, b_shr, c_shr);
}


    
/* ******************
 *
 * 
 * ******************/
void bfp_vect_s32_add(
    bfp_s32_t* a, 
    const bfp_s32_t* b, 
    const bfp_s32_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == c->length);
    assert(b->length == a->length);
#else
    a->length = b->length;
#endif

    right_shift_t b_shr, c_shr;

    xs3_add_sub_vect_calc_params(&a->exp, &b_shr, &c_shr,
            b->exp, c->exp, b->hr, c->hr, XS3_BFP_ALLOW_SATURATION);

    a->hr = xs3_vect_s32_add(a->data, b->data, c->data, b->length, b_shr, c_shr);
}



    
/* ******************
 *
 * 
 * ******************/
void bfp_vect_s16_sub(
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

    xs3_add_sub_vect_calc_params(&a->exp, &b_shr, &c_shr, b->exp, c->exp, 
            b->hr, c->hr, XS3_BFP_ALLOW_SATURATION);

    a->hr = xs3_vect_s16_sub(a->data, b->data, c->data, b->length, b_shr, c_shr);
}



    
/* ******************
 *
 * 
 * ******************/
void bfp_vect_s32_sub(
    bfp_s32_t* a, 
    const bfp_s32_t* b, 
    const bfp_s32_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == c->length);
    assert(b->length == a->length);
#else
    a->length = b->length;
#endif

    right_shift_t b_shr, c_shr;

    xs3_add_sub_vect_calc_params(&a->exp, &b_shr, &c_shr, b->exp, c->exp, b
            ->hr, c->hr, XS3_BFP_ALLOW_SATURATION);

    a->hr = xs3_vect_s32_sub(a->data, b->data, c->data, b->length, b_shr, c_shr);
}




    
/* ******************
 *
 * 
 * ******************/
void bfp_vect_s16_mul(
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



    



    
/* ******************
 *
 * 
 * ******************/
void bfp_vect_s32_mul(
    bfp_s32_t* a, 
    const bfp_s32_t* b, 
    const bfp_s32_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == c->length);
    assert(b->length == a->length);
#else
    a->length = b->length;
#endif

    right_shift_t b_shr, c_shr;
    xs3_vect_s32_mul_calc_params(&a->exp, &b_shr, &c_shr, b->exp, c->exp, b->hr, c->hr); 

    a->hr = xs3_vect_s32_mul(a->data, b->data, c->data, b->length, b_shr, c_shr);
}



    
/* ******************
 *
 * 
 * ******************/
void bfp_vect_s16_scalar_mul(
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

    xs3_vect_s16_scalar_mul_calc_params(&a->exp, &sat, b->exp, alpha_exp, b->hr, s_hr,
                                        XS3_BFP_ALLOW_SATURATION);

    a->hr = xs3_vect_s16_scalar_mul(a->data, b->data, b->length, alpha_mant, sat);
}



    
/* ******************
 *
 * 
 * ******************/
void bfp_vect_s32_scalar_mul(
    bfp_s32_t* a, 
    const bfp_s32_t* b,
    const int32_t alpha_mant,
    const exponent_t alpha_exp)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
#else
    a->length = b->length;
#endif

    right_shift_t b_shr, s_shr;

    headroom_t s_hr = HR_S32(alpha_mant);

    xs3_vect_s32_mul_calc_params(&a->exp, &b_shr, &s_shr, b->exp, alpha_exp, b->hr, s_hr);

    int32_t alpha = alpha_mant >> s_shr;

    a->hr = xs3_vect_s32_scalar_mul(a->data, b->data, b->length, alpha, b_shr);
}



    
/* ******************
 *
 * 
 * ******************/
void bfp_vect_s16_abs(
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



    
/* ******************
 *
 * 
 * ******************/
void bfp_vect_s32_abs(
    bfp_s32_t* a,
    const bfp_s32_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
#else
    a->length = b->length;
#endif

#if !XS3_BFP_ALLOW_SATURATION
    if(b->hr == 0){
        //shift down by one bit first
        xs3_vect_s32_shl(a->data, b->data, b->length, -1);
        a->exp = b->exp+1;
        a->hr = b->hr+1;
        b = a;
    }
#endif

    a->exp = b->exp;
    a->hr = xs3_vect_s32_abs(a->data, b->data, b->length);
}




    
/* ******************
 *
 * 
 * ******************/
int32_t bfp_vect_s16_sum(
    const bfp_s16_t* b)
{
    return xs3_vect_s16_sum(b->data, b->length);
}


    
/* ******************
 *
 * 
 * ******************/
int64_t bfp_vect_s32_sum(
    const bfp_s32_t* b)
{
    return xs3_vect_s32_sum(b->data, b->length);
}


    
/* ******************
 *
 * 
 * ******************/
int64_t bfp_vect_s16_dot(
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


    
/* ******************
 *
 * 
 * ******************/
int64_t bfp_vect_s32_dot(
    exponent_t* a_exp,
    const bfp_s32_t* b, 
    const bfp_s32_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == c->length);
#endif

    right_shift_t b_shr, c_shr;

    xs3_vect_s32_dot_calc_params(a_exp, &b_shr, &c_shr, b->exp, c->exp, 
                            b->hr, c->hr, b->length,
                            XS3_BFP_ALLOW_SATURATION);

    int64_t res = xs3_vect_s32_dot(b->data, c->data, b->length, b_shr, c_shr);

    return res;
}



/* ******************
 *
 * 
 * ******************/
void bfp_vect_s16_clip(
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


    
/* ******************
 *
 * 
 * ******************/
void bfp_vect_s32_clip(
    bfp_s32_t* a, 
    const bfp_s32_t* b, 
    const int32_t lower_bound, 
    const int32_t upper_bound, 
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

    int32_t lb;
    int32_t ub;

    if(bound_shr < 0){
        int64_t ub64 = ((int64_t)upper_bound) << (-bound_shr);
        int64_t lb64 = ((int64_t)lower_bound) << (-bound_shr);

        ub = (ub64 >= VPU_INT32_MAX)? VPU_INT32_MAX : (ub64 <= VPU_INT32_MIN)? VPU_INT32_MIN : ub64;
        lb = (lb64 >= VPU_INT32_MAX)? VPU_INT32_MAX : (lb64 <= VPU_INT32_MIN)? VPU_INT32_MIN : lb64;
    } else {
        // TODO: Should force upper_bound to round downwards to enforce the guarantee that no output can be larger than 
        // upper bound?
        ub = upper_bound >> bound_shr;
        // And lower bound upwards?
        lb = (lower_bound + ((1<<bound_shr)-1)) >> bound_shr;
    }

    if(ub == VPU_INT32_MIN){
        /* upper bound must be smaller than any element of b, so set everything to that */
        a->exp = bound_exp;
        a->hr = HR_S32(upper_bound);
        xs3_vect_s32_set(a->data, upper_bound, b->length);
    } else if(lb == VPU_INT32_MAX){
        /* lower bound must be larger than any element of b, so set everything to that */
        a->exp = bound_exp;
        a->hr = HR_S32(lower_bound);
        xs3_vect_s32_set(a->data, lower_bound, b->length);
    } else if(ub == lb){
        /* upper and lower bounds are indistinguishable */
        a->exp = a_exp;
        a->hr = HR_S32(ub);
        xs3_vect_s32_set(a->data, ub, b->length);

    } else {
        /* some elements may be between bounds */
        a->exp = a_exp;
        a->hr = xs3_vect_s32_clip(a->data, b->data, b->length, lb, ub, b_shr);
    }
}

    
/* ******************
 *
 * 
 * ******************/
void bfp_vect_s16_rect(
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


    
/* ******************
 *
 * 
 * ******************/
void bfp_vect_s32_rect(
    bfp_s32_t* a,
    const bfp_s32_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
#else 
    a->length = b->length;
#endif

    a->exp = b->exp;
    a->hr = xs3_vect_s32_rect(a->data, b->data, b->length);
}


    
/* ******************
 *
 * 
 * ******************/
void bfp_s32_to_s16(
    bfp_s16_t* a,
    const bfp_s32_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
#else
    a->length = b->length;
#endif
    
    right_shift_t b_shr = 16 - b->hr;

    const unsigned mod = !XS3_BFP_ALLOW_SATURATION;
    b_shr += mod;

    a->exp = b->exp + mod;
    a->hr = mod;
    // printf("%d\t%d\t%d\n", b->hr, b_shr, a->hr);
    xs3_vect_s32_to_s16(a->data, b->data, b->length, b_shr);
    // printf("0x%08X\t0x%08X\n", b->data[0], a->data[0]);
}

    
/* ******************
 *
 * 
 * ******************/
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

    
/* ******************
 *
 * 
 * ******************/
void bfp_vect_s16_sqrt(
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

    
/* ******************
 *
 * 
 * ******************/
void bfp_vect_s32_sqrt(
    bfp_s32_t* a,
    const bfp_s32_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
#else 
    a->length = b->length;
#endif

    right_shift_t b_shr;

    xs3_vect_s32_sqrt_calc_params(&a->exp, &b_shr, b->exp, b->hr);

    a->hr = xs3_vect_s32_sqrt(a->data, b->data, b->length, b_shr, XS3_BFP_SQRT_DEPTH_S32);
}

    
/* ******************
 *
 * 
 * ******************/
void bfp_vect_s16_inverse(
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
    
    bfp_vect_s16_headroom(a);
}

    
/* ******************
 *
 * 
 * ******************/
void bfp_vect_s32_inverse(
    bfp_s32_t* a,
    const bfp_s32_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
#else 
    a->length = b->length;
#endif

    unsigned scale;

    xs3_vect_s32_inverse_calc_params(&a->exp, &scale, b->data, b->exp, b->length);

    a->hr = xs3_vect_s32_inverse(a->data, b->data, b->length, scale);
}