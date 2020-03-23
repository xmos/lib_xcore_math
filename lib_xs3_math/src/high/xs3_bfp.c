

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
float bfp_dot_s16(
    const bfp_s16_t* b, 
    const bfp_s16_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == c->length);
#endif

    //TODO - calculate what these should be
    const exponent_t a_exp = 0;
    const int b_shr = 0;
    const int c_shr = 0;
    const int sat = 0;

    int16_t a = xs3_dot_s16(b->data, c->data, b->length, b_shr, c_shr, sat);

    return pack_float_s16(a, a_exp);
}

    
/* ******************
 *
 * 
 * ******************/
float bfp_dot_s32(
    const bfp_s32_t* b, 
    const bfp_s32_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == c->length);
#endif

    //TODO - calculate what these should be
    const exponent_t a_exp = 0;
    const int b_shr = 0;
    const int c_shr = 0;
    const int sat = 0;

    int16_t a = xs3_dot_s32(b->data, c->data, b->length, b_shr, c_shr, sat);

    return pack_float_s32(a, a_exp);
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

    // always base a_exp on b, because either they will work in that exponent, 
    //  or they will saturate. But because we we're choosing a_exp precisely so
    //  that b doesn't saturate, if the bound saturates, so, too would relevant
    //  b values.

    exponent_t a_exp = b->exp - b->hr + XS3_BFP_ALLOW_SATURATION; //minimum b exponent
    
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
        // Should force upper_bound to round downwards to enforce the guarantee that no output
        // can be larger than upper bound?
        ub = upper_bound >> bound_shr;
        // And lower bound upwards?
        lb = (lower_bound + ((1<<bound_shr)-1)) >> bound_shr;
    }

    a->exp = a_exp;    
    if(ub == lb){
        //If modified upper and lower bounds are identical, then just set the elements to that.
        a->hr = HR_S16(ub);
        xs3_set_vect_s16(a->data, ub, b->length);
    } else {
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

    // always base a_exp on b, because either they will work in that exponent, 
    //  or they will saturate. But because we we're choosing a_exp precisely so
    //  that b doesn't saturate, if the bound saturates, so, too would relevant
    //  b values.
    // I guess the trade off is that we might lose resolution on the clipping boundaries
    //  if they have to be down-shifted.

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
        // Should force upper_bound to round downwards to enforce the guarantee that no output
        // can be larger than upper bound?
        ub = upper_bound >> bound_shr;
        // And lower bound upwards?
        lb = (lower_bound + ((1<<bound_shr)-1)) >> bound_shr;
    }

    a->exp = a_exp;    
    if(ub == lb){
        //If modified upper and lower bounds are identical, then just set the elements to that.
        a->hr = HR_S32(ub);
        xs3_set_vect_s32(a->data, ub, b->length);
    } else {
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