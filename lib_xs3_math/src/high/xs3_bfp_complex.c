

#include "xs3_math.h"

#include "low/xs3_math_util_low.h"
#include "low/xs3_bfp_low.h"
#include "low/xs3_bfp_complex_low.h"

#define ASSERT(COND)    assert(COND)



headroom_t bfp_headroom_vect_complex_s16(
    bfp_complex_s16_t* a)
{
    const headroom_t re_hr = xs3_headroom_vect_s16(a->real, a->length);
    const headroom_t im_hr = xs3_headroom_vect_s16(a->imag, a->length);

    a->hr = (re_hr <= im_hr)? re_hr : im_hr;

    return a->hr;
}


headroom_t bfp_headroom_vect_complex_s32(
    bfp_complex_s32_t* a)
{
    a->hr = xs3_headroom_vect_s32((int32_t*)a->data, 2 * a->length);

    return a->hr;
}





void bfp_shl_vect_complex_s16(
    bfp_complex_s16_t* a,
    const bfp_complex_s16_t* b,
    const int shl)
{
#if XS3_BFP_DEBUG_CHECK_LENGTHS
    assert(a->length == b->length);
#endif

    a->length = b->length;
    a->exp = b->exp;

    const headroom_t re_hr = xs3_shl_vect_s16(a->real, b->real, b->length, shl);
    const headroom_t im_hr = xs3_shl_vect_s16(a->imag, b->imag, b->length, shl);
    a->hr = (re_hr <= im_hr)? re_hr : im_hr;
}
    
    


void bfp_shl_vect_complex_s32(
    bfp_complex_s32_t* a,
    const bfp_complex_s32_t* b,
    const int shl)
{
#if XS3_BFP_DEBUG_CHECK_LENGTHS
    assert(a->length == b->length);
#endif

    a->length = b->length;
    a->exp = b->exp;
    a->hr = xs3_shl_vect_s32((int32_t*) a->data, (int32_t*) b->data, 2*b->length, shl);
}




void bfp_mul_vect_complex_s16(
    bfp_complex_s16_t* a, 
    const bfp_complex_s16_t* b, 
    const bfp_s16_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
    assert(b->length == c->length);
#endif

    //TODO
    const exponent_t a_exp = 0;
    const int b_shr = 0;
    const int c_shr = 0;


    a->length = b->length;
    a->exp = a_exp;
    
    const headroom_t re_hr = xs3_mul_vect_s16(a->real, b->real, c->data, b->length, b_shr, c_shr);
    const headroom_t im_hr = xs3_mul_vect_s16(a->imag, b->imag, c->data, b->length, b_shr, c_shr);
    a->hr = (re_hr <= im_hr)? re_hr : im_hr;
}



void bfp_mul_vect_complex_s32(
    bfp_complex_s32_t* a, 
    const bfp_complex_s32_t* b, 
    const bfp_s32_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
    assert(b->length == c->length);
#endif

    //TODO
    const exponent_t a_exp = 0;
    const int b_shr = 0;
    const int c_shr = 0;


    a->length = b->length;
    a->exp = a_exp;
    // a->hr = xs3_mul_vect_complex_s32(a->data, b->data, c->data, b->length, b_shr, c_shr);
}




void bfp_complex_mul_vect_complex_s16(
    bfp_complex_s16_t* a, 
    const bfp_complex_s16_t* b, 
    const bfp_complex_s16_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
    assert(b->length == c->length);
#endif

    //TODO
    const exponent_t a_exp = 0;
    const int b_shr = 0;
    const int c_shr = 0;


    a->length = b->length;
    a->exp = a_exp;
    // a->hr = xs3_complex_mul_vect_complex_s16(a->real, a->imag, 
    //                                          b->real, b->imag, 
    //                                          c->real, c->imag, 
    //                                          b->length, b_shr, c_shr);
}




void bfp_complex_mul_vect_complex_s32(
    bfp_complex_s32_t* a, 
    const bfp_complex_s32_t* b, 
    const bfp_complex_s32_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
    assert(b->length == c->length);
#endif

    //TODO
    const exponent_t a_exp = 0;
    const int b_shr = 0;
    const int c_shr = 0;


    a->length = b->length;
    a->exp = a_exp;
    a->hr = xs3_complex_mul_vect_complex_s32(a->data, b->data, c->data, b->length, b_shr, c_shr);
}





void bfp_complex_conj_mul_vect_complex_s16(
    bfp_complex_s16_t* a, 
    const bfp_complex_s16_t* b, 
    const bfp_complex_s16_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
    assert(b->length == c->length);
#endif

    //TODO
    const exponent_t a_exp = 0;
    const int b_shr = 0;
    const int c_shr = 0;


    a->length = b->length;
    a->exp = a_exp;
    // a->hr = xs3_complex_conj_mul_vect_complex_s16(a->real, a->imag, 
    //                                               b->real, b->imag, 
    //                                               c->real, c->imag, 
    //                                               b->length, b_shr, c_shr);
}




void bfp_complex_conj_mul_vect_complex_s32(
    bfp_complex_s32_t* a, 
    const bfp_complex_s32_t* b, 
    const bfp_complex_s32_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
    assert(b->length == c->length);
#endif

    //TODO
    const exponent_t a_exp = 0;
    const int b_shr = 0;
    const int c_shr = 0;


    a->length = b->length;
    a->exp = a_exp;
    a->hr = xs3_complex_conj_mul_vect_complex_s32(a->data, b->data, c->data, b->length, b_shr, c_shr);
}




void bfp_scalar_mul_vect_complex_s16(
    bfp_complex_s16_t* a, 
    const bfp_complex_s16_t* b, 
    const float c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
#endif

    int16_t c_mant;
    exponent_t c_exp;
    unpack_float_s16(&c_mant, &c_exp, c);

    //TODO
    const exponent_t a_exp = 0;
    const int b_shr = 0;
    const int c_shr = 0;

    // c_mant = ashr(c_mant, c_shr);

    a->length = b->length;
    a->exp = a_exp;
    
    const headroom_t re_hr = xs3_scalar_mul_vect_s16(a->real, b->real, c_mant, b->length, b_shr);
    const headroom_t im_hr = xs3_scalar_mul_vect_s16(a->imag, b->imag, c_mant, b->length, b_shr);
    a->hr = (re_hr <= im_hr)? re_hr : im_hr;
}




void bfp_scalar_mul_vect_complex_s32(
    bfp_complex_s32_t* a, 
    const bfp_complex_s32_t* b, 
    const float c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
    assert(b->length == c->length);
#endif

    int32_t c_mant;
    exponent_t c_exp;
    unpack_float_s32(&c_mant, &c_exp, c);

    //TODO
    const exponent_t a_exp = 0;
    const int b_shr = 0;
    const int c_shr = 0;

    // c_mant = ashr(c_mant, c_shr);

    a->length = b->length;
    a->exp = a_exp;
    a->hr = xs3_scalar_mul_vect_s32( (int32_t*) a->data, (int32_t*) b->data, c_mant, 2 * b->length, b_shr);
}




void bfp_complex_scal_mul_vect_complex_s16(
    bfp_complex_s16_t* a, 
    const bfp_complex_s16_t* b, 
    const complex_float_t c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
    assert(b->length == c->length);
#endif

    int16_t c_re_mant;
    exponent_t c_re_exp;
    unpack_float_s16(&c_re_mant, &c_re_exp, c.re);

    int16_t c_im_mant;
    exponent_t c_im_exp;
    unpack_float_s16(&c_im_mant, &c_im_exp, c.im);

    //TODO
    const exponent_t a_exp = 0;
    const int b_shr = 0;
    const int c_shr = 0;

    // c_re_mant = ashr(c_re_mant, c_shr);
    // c_im_mant = ashr(c_im_mant, c_shr);

    a->length = b->length;
    a->exp = a_exp;
    // a->hr = xs3_complex_scal_mul_vect_complex_s16(a->real, a->imag, 
    //                                               b->real, b->imag, 
    //                                               c_re_mant, c_im_mant, 
    //                                               b->length, b_shr);
}




void bfp_complex_scal_mul_vect_complex_s32(
    bfp_complex_s32_t* a, 
    const bfp_complex_s32_t* b, 
    const complex_float_t c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
    assert(b->length == c->length);
#endif

    int32_t c_re_mant;
    exponent_t c_re_exp;
    unpack_float_s32(&c_re_mant, &c_re_exp, c.re);

    int32_t c_im_mant;
    exponent_t c_im_exp;
    unpack_float_s32(&c_im_mant, &c_im_exp, c.im);

    //TODO
    const exponent_t a_exp = 0;
    const int b_shr = 0;
    const int c_shr = 0;

    // c_re_mant = ashr(c_re_mant, c_shr);
    // c_im_mant = ashr(c_im_mant, c_shr);

    a->length = b->length;
    a->exp = a_exp;
    // a->hr = xs3_complex_scal_mul_vect_complex_s32(a->data, b->data, c_re_mant, c_im_mant, b->length, b_shr);
}




void bfp_add_vect_complex_s16(
    bfp_complex_s16_t* a, 
    const bfp_complex_s16_t* b, 
    const bfp_complex_s16_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
    assert(b->length == c->length);
#endif

    //TODO
    const exponent_t a_exp = 0;
    const int b_shr = 0;
    const int c_shr = 0;



    a->length = b->length;
    a->exp = a_exp;
    const headroom_t re_hr = xs3_add_vect_s16(a->real, b->real, c->real, b->length, b_shr, c_shr);
    const headroom_t im_hr = xs3_add_vect_s16(a->imag, b->imag, c->imag, b->length, b_shr, c_shr);
    a->hr = (re_hr <= im_hr)? re_hr : im_hr;
}




void bfp_add_vect_complex_s32(
    bfp_complex_s32_t* a, 
    const bfp_complex_s32_t* b, 
    const bfp_complex_s32_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
    assert(b->length == c->length);
#endif

    //TODO
    const exponent_t a_exp = 0;
    const int b_shr = 0;
    const int c_shr = 0;



    a->length = b->length;
    a->exp = a_exp;
    a->hr = xs3_add_vect_s32((int32_t*) a->data, (int32_t*) b->data, 
                             (int32_t*) c->data, 2 * b->length, b_shr, c_shr);
}




void bfp_sub_vect_complex_s16(
    bfp_complex_s16_t* a, 
    const bfp_complex_s16_t* b, 
    const bfp_complex_s16_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
    assert(b->length == c->length);
#endif

    //TODO
    const exponent_t a_exp = 0;
    const int b_shr = 0;
    const int c_shr = 0;



    a->length = b->length;
    a->exp = a_exp;

    const headroom_t re_hr = xs3_sub_vect_s16(a->real, b->real, c->real, b->length, b_shr, c_shr);
    const headroom_t im_hr = xs3_sub_vect_s16(a->imag, b->imag, c->imag, b->length, b_shr, c_shr);
    a->hr = (re_hr <= im_hr)? re_hr : im_hr;
}




void bfp_sub_vect_complex_s32(
    bfp_complex_s32_t* a, 
    const bfp_complex_s32_t* b,
    const bfp_complex_s32_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
    assert(b->length == c->length);
#endif

    //TODO
    const exponent_t a_exp = 0;
    const int b_shr = 0;
    const int c_shr = 0;



    a->length = b->length;
    a->exp = a_exp;
    a->hr = xs3_sub_vect_s32((int32_t*) a->data, (int32_t*) b->data, 
                             (int32_t*) c->data, 2 * b->length, b_shr, c_shr);
}




void bfp_complex_s16_to_complex_s32(
    bfp_complex_s32_t* a, 
    const bfp_complex_s16_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
#endif

    //TODO - figure out how to actually do this.. if possible.
    const exponent_t a_exp = b->exp;

    a->length = b->length;
    a->exp = a_exp;
    // a->hr = xs3_complex_s16_to_complex_s32(a->data, b->real, b->imag, b->length);
}




void bfp_complex_s32_to_complex_s16(
    bfp_complex_s16_t* a, 
    const bfp_complex_s32_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
#endif

    //TODO - calculate what these should be
    const exponent_t a_exp = 0;
    const int sat = 0;


    a->length = b->length;
    a->exp = a_exp;
    // a->hr = xs3_complex_s32_to_complex_s16(a->real, a->imag, b->data, b->length, sat);
}



void bfp_squared_mag_vect_complex_s16(
    bfp_s16_t* a, 
    const bfp_complex_s16_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
#endif

    //TODO - calculate what these should be
    const exponent_t a_exp = 0;
    const int b_shr = 0;

    a->length = b->length;
    a->exp = a_exp;
    // a->hr = xs3_squared_mag_vect_complex_s16(a->data, b->real, b->imag, b->length, b_shr);
}

void bfp_squared_mag_vect_complex_s32(
    bfp_s32_t* a, 
    const bfp_complex_s32_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
#endif

    //TODO - calculate what these should be
    const exponent_t a_exp = 0;
    const int b_shr = 0;

    a->length = b->length;
    a->exp = a_exp;
    // a->hr = xs3_squared_mag_vect_complex_s32(a->data, b->data, b->length, b_shr);
}

void bfp_mag_vect_complex_s16(
    bfp_s16_t* a, 
    const bfp_complex_s16_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
#endif

    //TODO - calculate what these should be
    const exponent_t a_exp = 0;
    const int b_shr = 0;

    a->length = b->length;
    a->exp = a_exp;
    // a->hr = xs3_mag_vect_complex_s16(a->data, b->real, b->imag, b->length, b_shr);
}




void bfp_mag_vect_complex_s32(
    bfp_s32_t* a, 
    const bfp_complex_s32_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
#endif

    //TODO - calculate what these should be
    const exponent_t a_exp = 0;
    const int b_shr = 0;

    a->length = b->length;
    a->exp = a_exp;
    // a->hr = xs3_mag_vect_complex_s32(a->data, b->data, b->length, b_shr);
}




complex_float_t bfp_sum_complex_s16(
    const bfp_complex_s16_t* b)
{
    //TODO: not certain how to implement the assembly for this one.

    //TODO - calculate what these should be
    const exponent_t a_exp = 0;
    const int sat = 0;

    int16_t a_real = 0;
    int16_t a_imag = 0;

    // xs3_sum_complex_s16(&a_real, &a_imag, 
    //                     b->real, b->imag, 
    //                     b->length, sat);

    float af_real = pack_float_s16(a_real, a_exp);
    float af_imag = pack_float_s16(a_imag, a_exp);

    complex_float_t af = {af_real, af_imag};

    return af;
}




complex_float_t bfp_sum_complex_s32( 
    const bfp_complex_s32_t* b)
{
    //TODO - calculate what these should be
    const exponent_t a_exp = 0;
    const int sat = 0;

    int32_t a_real = 0;
    int32_t a_imag = 0;

    // xs3_sum_complex_s32(&a_real, &a_imag, b->data, b->length, sat);

    float af_real = pack_float_s32(a_real, a_exp);
    float af_imag = pack_float_s32(a_imag, a_exp);

    complex_float_t af = {af_real, af_imag};

    return af;
}