

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
headroom_t bfp_headroom_vect_s16(
    bfp_s16_t* a)
{
     a->hr = xs3_headroom_vect_s16(a->data, a->length);

     return a->hr;
}



    
/* ******************
 *
 * 
 * ******************/
headroom_t bfp_headroom_vect_s32(
    bfp_s32_t* a)
{
     a->hr = xs3_headroom_vect_s32(a->data, a->length);

     return a->hr;
}


    
/* ******************
 *
 * 
 * ******************/
void bfp_shl_vect_s16(
    bfp_s16_t* a,
    const bfp_s16_t* b,
    const left_shift_t shl)
{
#if XS3_BFP_DEBUG_CHECK_LENGTHS
    assert(a->length == b->length);
#endif

    a->length = b->length;
    a->exp = b->exp;
    a->hr = xs3_shl_vect_s16(a->data, b->data, b->length, shl);
}


    
/* ******************
 *
 * 
 * ******************/
void bfp_shl_vect_s32(
    bfp_s32_t* a,
    const bfp_s32_t* b,
    const left_shift_t shl)
{
#if XS3_BFP_DEBUG_CHECK_LENGTHS
    assert(a->length == b->length);
#endif
    a->length = b->length;
    a->exp = b->exp;
    a->hr = xs3_shl_vect_s32(a->data, b->data, b->length, shl);
}




/*            
    A = B + C

    Bf[] = B[] * 2^(B.exp)
    Cf[] = C[] * 2^(C.exp)

    Worst case scenario is: (B.exp - B.hr) == (C.exp - C.hr), and where B.hr and C.hr
    are due to a negative power of 2, and where corresponding elements in the two vectors
    cause that. In other words:

    min(Bf[]) = min(Cf[]) = -(2^(16-X.hr+1))
    Then min(Bf[] + Cf[]) = 2*min(Bf[]) = -(2^(16-X.hr+2)), which is also a negative
    power of 2, and which will saturate if we remove all the headroom from B[] and C[]

    So, if worst case result is -(2^(16-X.hr+2)), which has to be -0x4000, because the
    value -0x8000 would saturate to -0x8001, then the output exponent will have to be
    A.exp = (B.exp - B.hr) + 2

    However, if (B.exp-B.hr) != (C.exp-C.hr), then min(Bf[]+Cf[]) is strictly greater
    than  -(2^(16-X.hr+2)), and A.exp can be  (B.exp - B.hr) + 1

*/


    
/* ******************
 *
 * 
 * ******************/
void bfp_add_vect_calc_params(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    right_shift_t* c_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr,
    const unsigned allow_saturation)
{
    const exponent_t b_min_exp = b_exp - b_hr;
    const exponent_t c_min_exp = c_exp - c_hr;

    a_exp[0] = XS3_MAX(b_min_exp, c_min_exp) + 1;

    if((b_min_exp == c_min_exp) && !(allow_saturation))
        a_exp[0] = a_exp[0] + 1;

    b_shr[0] = a_exp[0] - b_exp;
    c_shr[0] = a_exp[0] - c_exp;
}



/* ******************
 *
 * 
 * ******************/
void bfp_add_vect_s16(
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

    bfp_add_vect_calc_params(&a->exp, &b_shr, &c_shr,
            b->exp, c->exp, b->hr, c->hr, XS3_BFP_ALLOW_SATURATION);

    a->hr = xs3_add_vect_s16(a->data, b->data, c->data, b->length, b_shr, c_shr);
}


    
/* ******************
 *
 * 
 * ******************/
void bfp_add_vect_s32(
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

    bfp_add_vect_calc_params(&a->exp, &b_shr, &c_shr,
            b->exp, c->exp, b->hr, c->hr, XS3_BFP_ALLOW_SATURATION);

    a->hr = xs3_add_vect_s32(a->data, b->data, c->data, b->length, b_shr, c_shr);
}


    
/* ******************
 *
 * 
 * ******************/
void bfp_sub_vect_calc_params(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    right_shift_t* c_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr)
{
    const exponent_t b_min_exp = b_exp - b_hr;
    const exponent_t c_min_exp = c_exp - c_hr;

    a_exp[0] = XS3_MAX(b_min_exp, c_min_exp) + 1;

    b_shr[0] = a_exp[0] - b_exp;
    c_shr[0] = a_exp[0] - c_exp;
}



    
/* ******************
 *
 * 
 * ******************/
void bfp_sub_vect_s16(
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

    bfp_sub_vect_calc_params(&a->exp, &b_shr, &c_shr, b->exp, c->exp, b->hr, c->hr);

    a->hr = xs3_sub_vect_s16(a->data, b->data, c->data, b->length, b_shr, c_shr);
}



    
/* ******************
 *
 * 
 * ******************/
void bfp_sub_vect_s32(
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

    bfp_sub_vect_calc_params(&a->exp, &b_shr, &c_shr, b->exp, c->exp, b->hr, c->hr);

    a->hr = xs3_sub_vect_s32(a->data, b->data, c->data, b->length, b_shr, c_shr);
}



    
/* ******************
 *
 * 
 * ******************/
void bfp_mul_vect_s16_calc_params(
    exponent_t* a_exp,
    right_shift_t* a_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr)
{
    /*
        (-0x8000 >> b_hr) * (-0x8000 >> c_hr)
        -2^15 * 2^(-b_hr) * -2^15 * 2^(-c_hr)
        2^30 * 2^(-b_hr-c_hr)
        2^(30-total_hr)
        2^14 * 2^(16-total_hr)
        0x4000 * 2^(16-total_hr)

        Don't need to worry about XS3_BFP_ALLOW_SATURATION, because
        max negative value is -0x8000 * 0x7FFF
    */
    headroom_t total_hr = b_hr+c_hr;
    *a_shr = 16-total_hr;
    *a_shr = (*a_shr < 0)? 0 : *a_shr;

    *a_exp = (b_exp+c_exp)+*a_shr;
}


    
/* ******************
 *
 * 
 * ******************/
void bfp_mul_vect_s16(
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

    bfp_mul_vect_s16_calc_params(&a->exp, &a_shr,
            b->exp, c->exp, b->hr, c->hr);

    a->hr = xs3_mul_vect_s16(a->data, b->data, c->data, b->length, a_shr);
}



    
/* ******************
 *
 * 
 * ******************/
void bfp_mul_vect_s32_calc_params(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    right_shift_t* c_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr)
{

    /*
        (-0x80000000 >> b_hr) * (-0x80000000 >> c_hr) >> 30
        -2^31 * 2^-b_hr * -2^31 * 2^-c_hr * 2^-30
        2^(62 - b_hr - c_hr - 30)
        2^(32 - total_hr)
        2^30 * 2^(2 - total_hr)
        0x40000000 * 2^(2-total_hr)
    */
    headroom_t total_hr = b_hr + c_hr;

    if(total_hr == 0){
        *b_shr = 1;
        *c_shr = 1;
    } else if(total_hr == 1){
        *b_shr = (b_hr == 0)? 1 : 0;
        *c_shr = (c_hr == 0)? 1 : 0;
    } else if(b_hr == 0){
        *b_shr = 0;
        *c_shr = 2-total_hr;
    } else if(c_hr == 0){
        *b_shr = 2-total_hr;
        *c_shr = 0;
    } else {
        *b_shr = 1-b_hr;
        *c_shr = 1-c_hr;
    }

    *a_exp = b_exp + c_exp + *b_shr + *c_shr + 30;
}


    
/* ******************
 *
 * 
 * ******************/
void bfp_mul_vect_s32(
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
    bfp_mul_vect_s32_calc_params(&a->exp, &b_shr, &c_shr, b->exp, c->exp, b->hr, c->hr); 

    a->hr = xs3_mul_vect_s32(a->data, b->data, c->data, b->length, b_shr, c_shr);
}




    
/* ******************
 *
 * 
 * ******************/
void bfp_scalar_mul_vect_s16(
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

    right_shift_t b_shr, s_shr;

    headroom_t s_hr = HR_S16(alpha_mant);

    bfp_mul_vect_s32_calc_params(&a->exp, &b_shr, &s_shr, b->exp, alpha_exp, b->hr, s_hr);
    a->exp = a->exp - 30 + 14;

    int16_t alpha = alpha_mant >> s_shr;

    a->hr = xs3_scalar_mul_vect_s16(a->data, b->data, b->length, alpha, b_shr);
}



    
/* ******************
 *
 * 
 * ******************/
void bfp_scalar_mul_vect_s32(
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

    bfp_mul_vect_s32_calc_params(&a->exp, &b_shr, &s_shr, b->exp, alpha_exp, b->hr, s_hr);

    int32_t alpha = alpha_mant >> s_shr;

    a->hr = xs3_scalar_mul_vect_s32(a->data, b->data, b->length, alpha, b_shr);
}



    
/* ******************
 *
 * 
 * ******************/
void bfp_abs_vect_s16(
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
        xs3_shl_vect_s16(a->data, b->data, b->length, -1);
        a->exp = b->exp+1;
        a->hr = b->hr+1;
        b = a;
    }
#endif

    a->exp = b->exp;
    a->hr = xs3_abs_vect_s16(a->data, b->data, b->length);
}



    
/* ******************
 *
 * 
 * ******************/
void bfp_abs_vect_s32(
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
        xs3_shl_vect_s32(a->data, b->data, b->length, -1);
        a->exp = b->exp+1;
        a->hr = b->hr+1;
        b = a;
    }
#endif

    a->exp = b->exp;
    a->hr = xs3_abs_vect_s32(a->data, b->data, b->length);
}




    
/* ******************
 *
 * 
 * ******************/
int32_t bfp_sum_s16(
    const bfp_s16_t* b)
{
    return xs3_sum_s16(b->data, b->length);
}


    
/* ******************
 *
 * 
 * ******************/
int64_t bfp_sum_s32(
    const bfp_s32_t* b)
{
    return xs3_sum_s32(b->data, b->length);
}


    
/* ******************
 *
 * 
 * ******************/
void bfp_dot_s16_calc_params(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    right_shift_t* c_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr,
    const unsigned length)
{
    /*
            (-0x8000 >> b_hr) * (-0x8000 >> c_hr)
            -2^15 * 2^(-b_hr) * -2^15 * 2^(-c_hr)
            2^30 * 2^(-b_hr-c_hr)
            0x40000000 * 2^(-total_hr)

        Then, we're adding together up to `length` of these, which means the total is potentially:
            2^30 * 2^(-total_hr) * 2^(ceil(log2(length))
            2^30 * 2^(ceil_log2(length) - total_hr)

        So, if we get two vectors where all elements are -0x8000, and we have exactly 2^K of them:
            2^30 * 2^(-total_hr) * 2^K
            2^(30) * 2^(K - total_hr)

        The 2^(30) avoids saturation, so we need to shift down by a total of:
            (b_shr + c_shr) = (K - total_hr) bits.

    */
    // headroom_t total_hr = b_hr+c_hr;
    unsigned K = ceil_log2(length);

    right_shift_t total_shr = K - (b_hr + c_hr);

    if(total_shr <= 0){
        *b_shr = 0;
        *c_shr = 0;
    } else {
        int hr_delta = (b_hr - c_hr);
        *c_shr = (total_shr <= hr_delta)? total_shr : hr_delta;
        total_shr -= *c_shr;
        *b_shr = total_shr / 2;
        *c_shr += (total_shr - *b_shr);
    }

    *a_exp = b_exp + c_exp + *b_shr + *c_shr;

}
    
/* ******************
 *
 * 
 * ******************/
int32_t bfp_dot_s16(
    exponent_t* a_exp,
    const bfp_s16_t* b, 
    const bfp_s16_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == c->length);
#endif

    right_shift_t b_shr, c_shr;

    bfp_dot_s16_calc_params(a_exp, &b_shr, &c_shr, b->exp, c->exp, b->hr, c->hr, b->length);

    int32_t res = xs3_dot_s16(b->data, c->data, b->length, b_shr, c_shr);

    return res;
}


    
/* ******************
 *
 * 
 * ******************/
void bfp_dot_s32_calc_params(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    right_shift_t* c_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr,
    const unsigned length)
{
    /*
            (-0x80000000 >> b_hr) * (-0x80000000 >> c_hr)
            -2^31 * 2^(-b_hr) * -2^31 * 2^(-c_hr) * 2^(-30)
            2^62 * 2^(-b_hr-c_hr) * 2^(-30)
            2^32 * 2^(-total_hr)

        Then, we're adding together up to `length` of these, which means the total is potentially:
            2^32 * 2^(-total_hr) * 2^(ceil(log2(length))
            2^32 * 2^(K - total_hr)
            
        We have 40 bits of accumulator. The maximum value we can be left with (to avoid saturation) is 2^38:

            2^38 = 2^32 * 2^(K - total_hr) * 2^(- total_shr)
            2^(total_shr) = 2^32 * 2^(K - total_hr) * 2^(-38)
                          = 2^(32 - 38 + K - total_hr)
            total_shr = K - total_hr - 6

    */

    unsigned K = ceil_log2(length);

    right_shift_t total_shr = K - 6;

    *b_shr = - b_hr;
    *c_shr = - c_hr;

    if(total_shr <= 0){
        total_shr = 0;
    } else {
        *b_shr += (total_shr/2);
        *c_shr += (total_shr - (total_shr/2));
    }

    *a_exp = b_exp + c_exp - b_hr - c_hr + total_shr + 30;

}

    
/* ******************
 *
 * 
 * ******************/
int64_t bfp_dot_s32(
    exponent_t* a_exp,
    const bfp_s32_t* b, 
    const bfp_s32_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == c->length);
#endif

    right_shift_t b_shr, c_shr;

    bfp_dot_s32_calc_params(a_exp, &b_shr, &c_shr, b->exp, c->exp, b->hr, c->hr, b->length);

    // printf("shr: [%d, %d]\t\texp: %d\n", b_shr, c_shr, *a_exp);

    int64_t res = xs3_dot_s32(b->data, c->data, b->length, b_shr, c_shr);

    return res;
}



/* ******************
 *
 * 
 * ******************/
void bfp_clip_vect_s16(
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
        xs3_set_vect_s16(a->data, upper_bound, b->length);
    } else if(lb == VPU_INT16_MAX){
        /* lower bound must be larger than any element of b, so set everything to that */
        a->exp = bound_exp;
        a->hr = HR_S16(lower_bound);
        xs3_set_vect_s16(a->data, lower_bound, b->length);
    } else if(ub == lb){
        /* upper and lower bounds are indistinguishable */
        a->exp = a_exp;
        a->hr = HR_S16(ub);
        xs3_set_vect_s16(a->data, ub, b->length);

    } else {
        /* some elements may be between bounds */
        a->exp = a_exp;
        a->hr = xs3_clip_vect_s16(a->data, b->data, b->length, lb, ub, b_shr);
    }
}


    
/* ******************
 *
 * 
 * ******************/
void bfp_clip_vect_s32(
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
        xs3_set_vect_s32(a->data, upper_bound, b->length);
    } else if(lb == VPU_INT32_MAX){
        /* lower bound must be larger than any element of b, so set everything to that */
        a->exp = bound_exp;
        a->hr = HR_S32(lower_bound);
        xs3_set_vect_s32(a->data, lower_bound, b->length);
    } else if(ub == lb){
        /* upper and lower bounds are indistinguishable */
        a->exp = a_exp;
        a->hr = HR_S32(ub);
        xs3_set_vect_s32(a->data, ub, b->length);

    } else {
        /* some elements may be between bounds */
        a->exp = a_exp;
        a->hr = xs3_clip_vect_s32(a->data, b->data, b->length, lb, ub, b_shr);
    }
}

    
/* ******************
 *
 * 
 * ******************/
void bfp_rect_vect_s16(
    bfp_s16_t* a,
    const bfp_s16_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
#else 
    a->length = b->length;
#endif

    a->exp = b->exp;
    a->hr = xs3_rect_vect_s16(a->data, b->data, b->length);
}


    
/* ******************
 *
 * 
 * ******************/
void bfp_rect_vect_s32(
    bfp_s32_t* a,
    const bfp_s32_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
#else 
    a->length = b->length;
#endif

    a->exp = b->exp;
    a->hr = xs3_rect_vect_s32(a->data, b->data, b->length);
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
    xs3_s32_to_s16(a->data, b->data, b->length, b_shr);
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
    xs3_s16_to_s32(a->data, b->data, b->length);
}