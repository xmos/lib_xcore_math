

#include "xs3_math.h"

#include "low/xs3_math_util_low.h"
#include "low/xs3_bfp_low.h"

#include <assert.h>






headroom_t xs3_cls_s16(
    const int16_t a)
{
    return CLS(a) - 16;
}


headroom_t xs3_cls_s32(
    const int32_t a)
{
    return CLS(a);
}


headroom_t xs3_cls_s64(
    const int64_t a)
{
    int32_t* tmp = (int32_t*) &a;

    if( (tmp[0] == 0) || (tmp[0] == -1)){
        return CLS(tmp[1]);
    } else {
        return CLS(tmp[0]) + 32;
    }
}

headroom_t bfp_cls_vect_s16(
    bfp_s16_t* a)
{
     a->hr = xs3_cls_array_s16(a->data, a->length);
     return a->hr;
}


headroom_t bfp_cls_vect_s32(
    bfp_s32_t* a)
{
     a->hr = xs3_cls_array_s32(a->data, a->length);
     return a->hr;
}


void bfp_ldexp_vect_s16(
    bfp_s16_t* a,
    const bfp_s16_t* b,
    const exponent_t exp)
{
    a->length = b->length;
    a->exp = b->exp;
    a->hr = xs3_shl_vect_s16(a->data, b->data, b->length, exp);
}


void bfp_ldexp_vect_s32(
    bfp_s32_t* a,
    const bfp_s32_t* b,
    const exponent_t exp)
{
    a->length = b->length;
    a->exp = b->exp;
    a->hr = xs3_shl_vect_s32(a->data, b->data, b->length, exp);
}


void bfp_mul_vect_s16(
    bfp_s16_t* a, 
    const bfp_s16_t* b, 
    const bfp_s16_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    ASSERT(b->length == c->length);
#endif

    //TODO
    const exponent_t a_exp = 0;
    const int b_shr = 0;
    const int c_shr = 0;



    a->length = b->length;
    a->exp = a_exp;
    a->hr = xs3_mul_vect_s16(a->data, b->data, c->data, b->length, b_shr, c_shr);
}


void bfp_mul_vect_s32(
    bfp_s32_t* a, 
    const bfp_s32_t* b, 
    const bfp_s32_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    ASSERT(b->length == c->length);
#endif

    //TODO
    const exponent_t a_exp = 0;
    const int b_shr = 0;
    const int c_shr = 0;



    a->length = b->length;
    a->exp = a_exp;
    a->hr = xs3_mul_vect_s32(a->data, b->data, c->data, b->length, b_shr, c_shr);
}



void bfp_scalar_mul_vect_s16(
    bfp_s16_t* a, 
    const bfp_s16_t* b, 
    const float c)
{
    int16_t c_mant;
    exponent_t c_exp;
    unpack_float_s16(&c_mant, &c_exp, c);

    //TODO
    const exponent_t a_exp = 0;
    const int b_shr = 0;
    const int c_shr = 0;

    c_mant = ashr(c_mant, c_shr);

    a->length = b->length;
    a->exp = a_exp;
    a->hr = xs3_scalar_mul_vect_s16(a->data, b->data, c_mant, b->length, b_shr);
}


void bfp_scalar_mul_vect_s32(
    bfp_s32_t* a, 
    const bfp_s32_t* b,
    const float c)
{
    int32_t c_mant;
    exponent_t c_exp;
    unpack_float_s32(&c_mant, &c_exp, c);

    //TODO
    const exponent_t a_exp = 0;
    const int b_shr = 0;
    const int c_shr = 0;

    c_mant = ashr(c_mant, c_shr);

    a->length = b->length;
    a->exp = a_exp;
    a->hr = xs3_scalar_mul_vect_s32(a->data, b->data, c_mant, b->length, b_shr);
}


void bfp_add_vect_s16(
    bfp_s16_t* a, 
    const bfp_s16_t* b, 
    const bfp_s16_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    ASSERT(b->length == c->length);
#endif

    //TODO
    const exponent_t a_exp = 0;
    const int b_shr = 0;
    const int c_shr = 0;



    a->length = b->length;
    a->exp = a_exp;
    a->hr = xs3_add_vect_s16(a->data, b->data, c->data, b->length, b_shr, c_shr);
}


void bfp_add_vect_s32(
    bfp_s32_t* a, 
    const bfp_s32_t* b, 
    const bfp_s32_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    ASSERT(b->length == c->length);
#endif

    //TODO
    const exponent_t a_exp = 0;
    const int b_shr = 0;
    const int c_shr = 0;



    a->length = b->length;
    a->exp = a_exp;
    a->hr = xs3_add_vect_s32(a->data, b->data, c->data, b->length, b_shr, c_shr);
}


void bfp_sub_vect_s16(
    bfp_s16_t* a, 
    const bfp_s16_t* b, 
    const bfp_s16_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    ASSERT(b->length == c->length);
#endif

    //TODO
    const exponent_t a_exp = 0;
    const int b_shr = 0;
    const int c_shr = 0;



    a->length = b->length;
    a->exp = a_exp;
    a->hr = xs3_sub_vect_s16(a->data, b->data, c->data, b->length, b_shr, c_shr);
}


void bfp_sub_vect_s32(
    bfp_s32_t* a, 
    const bfp_s32_t* b, 
    const bfp_s32_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    ASSERT(b->length == c->length);
#endif

    //TODO
    const exponent_t a_exp = 0;
    const int b_shr = 0;
    const int c_shr = 0;



    a->length = b->length;
    a->exp = a_exp;
    a->hr = xs3_sub_vect_s32(a->data, b->data, c->data, b->length, b_shr, c_shr);
}



void bfp_abs_vect_s16(
    bfp_s16_t* a,
    const bfp_s16_t* b)
{
    a->length = b->length;
    a->exp = b->exp;
    a->hr = xs3_abs_vect_s16(a->data, b->data, b->length);
}

    
void bfp_abs_vect_s32(
    bfp_s32_t* a,
    const bfp_s32_t* b)
{
    a->length = b->length;
    a->exp = b->exp;
    a->hr = xs3_abs_vect_s32(a->data, b->data, b->length);
}



float bfp_sum_s16(
    const bfp_s16_t* b)
{
    //TODO: not certain how to implement the assembly for this one.

    //TODO - calculate what these should be
    const exponent_t a_exp = 0;
    const int sat = 0;

    int16_t a = xs3_sum_s16(b->data, b->length, sat);

    return pack_float_s16(a, a_exp);
}

float bfp_sum_s32(
    const bfp_s32_t* b)
{
    //TODO - calculate what these should be
    const exponent_t a_exp = 0;
    const int sat = 0;

    int32_t a = xs3_sum_s32(b->data, b->length, sat);

    return pack_float_s32(a, a_exp);
}




float bfp_dot_s16(
    const bfp_s16_t* b, 
    const bfp_s16_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    ASSERT(b->length == c->length);
#endif

    //TODO - calculate what these should be
    const exponent_t a_exp = 0;
    const int b_shr = 0;
    const int c_shr = 0;
    const int sat = 0;

    int16_t a = xs3_dot_s16(b->data, c->data, b->length, b_shr, c_shr, sat);

    return pack_float_s16(a, a_exp);
}


float bfp_dot_s32(
    const bfp_s32_t* b, 
    const bfp_s32_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    ASSERT(b->length == c->length);
#endif

    //TODO - calculate what these should be
    const exponent_t a_exp = 0;
    const int b_shr = 0;
    const int c_shr = 0;
    const int sat = 0;

    int16_t a = xs3_dot_s32(b->data, c->data, b->length, b_shr, c_shr, sat);

    return pack_float_s32(a, a_exp);
}



void bfp_clip_vect_s16(
    bfp_s16_t* a, 
    const bfp_s16_t* b, 
    const int16_t lower_bound, 
    const int16_t upper_bound, 
    const int bound_exp)
{

    //TODO - calculate what these should be
    const exponent_t a_exp = 0;
    const int b_shr = 0;
    const int16_t lb = lower_bound; //modified lower bound based on a_exp
    const int16_t ub = upper_bound; //modified upper bound based on a_exp


    a->length = b->length;
    a->exp = a_exp;
    a->hr = xs3_clip_vect_s16(a->data, b->data, b->length, lb, ub, b_shr);
}


void bfp_clip_vect_s32(
    bfp_s32_t* a, 
    const bfp_s32_t* b, 
    const int32_t lower_bound, 
    const int32_t upper_bound, 
    const int bound_exp)
{

    //TODO - calculate what these should be
    const exponent_t a_exp = 0;
    const int b_shr = 0;
    const int32_t lb = lower_bound; //modified lower bound based on a_exp
    const int32_t ub = upper_bound; //modified upper bound based on a_exp


    a->length = b->length;
    a->exp = a_exp;
    a->hr = xs3_clip_vect_s32(a->data, b->data, b->length, lb, ub, b_shr);
}



void bfp_rect_vect_s16(
    bfp_s16_t* a,
    const bfp_s16_t* b)
{
    a->length = b->length;
    a->exp = b->exp;
    a->hr = xs3_rect_vect_s16(a->data, b->data, b->length);
}


void bfp_rect_vect_s32(
    bfp_s32_t* a,
    const bfp_s32_t* b)
{
    a->length = b->length;
    a->exp = b->exp;
    a->hr = xs3_rect_vect_s32(a->data, b->data, b->length);
}



void bfp_s32_to_s16(
    bfp_s16_t* a,
    const bfp_s32_t* b)
{
    //TODO - calculate what these should be
    const exponent_t a_exp = 0;
    const int b_shr = 0;

    //TODO - calculate headroom adjustment (should be related to b_shr)


    a->length = b->length;
    a->exp = a_exp;
    a->hr = b->hr - 16 + b_shr;
    xs3_s32_to_s16(a->data, b->data, b->length, b_shr);
}

    
void bfp_s16_to_s32(
    bfp_s32_t* a,
    const bfp_s16_t* b)
{    
    //TODO - figure out how to actually do this..
    const exponent_t a_exp = b->exp;

    a->length = b->length;
    a->exp = a_exp;
    a->hr = b->hr + 8;
    xs3_s16_to_s32(a->data, b->data, b->length);
}