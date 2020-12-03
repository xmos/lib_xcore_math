

#include "xs3_math.h"

#include "low/xs3_math_util_low.h"
#include "low/xs3_bfp_low.h"
#include "low/xs3_bfp_complex_low.h"
#include "../low/vpu_helper.h"

#define ASSERT(COND)    assert(COND)


const extern unsigned rot_table32_rows;
const extern complex_s32_t rot_table32[30][4];

const extern unsigned rot_table16_rows;
const extern int16_t rot_table16[14][2][16];

headroom_t bfp_headroom_vect_complex_s16(
    bfp_complex_s16_t* a)
{
    a->hr = xs3_headroom_vect_complex_s16(a->real, a->imag, a->length);
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
    const left_shift_t shl)
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
    const left_shift_t shl)
{
#if XS3_BFP_DEBUG_CHECK_LENGTHS
    assert(a->length == b->length);
#endif

    a->length = b->length;
    a->exp = b->exp;
    a->hr = xs3_shl_vect_s32((int32_t*) a->data, (int32_t*) b->data, 2*b->length, shl);
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

    right_shift_t b_shr, c_shr;

    xs3_add_sub_vect_calc_params(&a->exp, &b_shr, &c_shr, b->exp, c->exp, 
                             b->hr, c->hr, XS3_BFP_ALLOW_SATURATION);

    a->length = b->length;
    
    a->hr = xs3_add_vect_complex_s16(a->real, a->imag, b->real, b->imag, c->real, c->imag, 
                                     b->length, b_shr, c_shr);
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

    right_shift_t b_shr, c_shr;

    xs3_add_sub_vect_calc_params(&a->exp, &b_shr, &c_shr, b->exp, c->exp, 
                             b->hr, c->hr, XS3_BFP_ALLOW_SATURATION);

    a->length = b->length;
    a->hr = xs3_add_vect_complex_s32(a->data, b->data, c->data, b->length, b_shr, c_shr);
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

    right_shift_t b_shr, c_shr;

    xs3_add_sub_vect_calc_params(&a->exp, &b_shr, &c_shr, b->exp, c->exp, 
                             b->hr, c->hr, XS3_BFP_ALLOW_SATURATION);

    a->length = b->length;
    
    a->hr = xs3_sub_vect_complex_s16(a->real, a->imag, b->real, b->imag, c->real, c->imag,
                                     b->length, b_shr, c_shr);
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

    right_shift_t b_shr, c_shr;

    xs3_add_sub_vect_calc_params(&a->exp, &b_shr, &c_shr, b->exp, c->exp, 
                             b->hr, c->hr, XS3_BFP_ALLOW_SATURATION);

    a->length = b->length;
    a->hr = xs3_sub_vect_complex_s32(a->data, b->data, c->data, b->length, b_shr, c_shr);
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

    exponent_t a_exp;
    right_shift_t sat;

    xs3_mul_vect_complex_s16_calc_params(&a_exp, &sat, b->exp, c->exp, b->hr, c->hr, XS3_BFP_ALLOW_SATURATION);

    a->length = b->length;
    a->exp = a_exp;
    
    const headroom_t re_hr = xs3_mul_vect_s16(a->real, b->real, c->data, b->length, sat);
    const headroom_t im_hr = xs3_mul_vect_s16(a->imag, b->imag, c->data, b->length, sat);
    a->hr = (re_hr <= im_hr)? re_hr : im_hr;
}



/* ******************
 *
 * 
 * ******************/


void bfp_complex_mul_vect_complex_s16(
    bfp_complex_s16_t* a, 
    const bfp_complex_s16_t* b, 
    const bfp_complex_s16_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
    assert(b->length == c->length);
#endif

    exponent_t a_exp;
    right_shift_t sat;

    xs3_complex_mul_vect_complex_s16_calc_params(&a_exp, &sat, b->exp, c->exp, b->hr, c->hr, XS3_BFP_ALLOW_SATURATION);

    a->length = b->length;
    a->exp = a_exp;
    a->hr = xs3_complex_mul_vect_complex_s16(a->real, a->imag, b->real, b->imag, c->real, c->imag, b->length, sat);
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

    exponent_t a_exp;
    right_shift_t b_shr, c_shr;

    xs3_mul_vect_complex_s32_calc_params(&a_exp, &b_shr, &c_shr,    
                                         b->exp, c->exp, b->hr, c->hr, 
                                         XS3_BFP_ALLOW_SATURATION);

    a->length = b->length;
    a->exp = a_exp;

    a->hr = xs3_mul_vect_complex_s32(a->data, b->data, c->data, b->length, b_shr, c_shr);
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

    exponent_t a_exp;
    right_shift_t b_shr, c_shr;

    xs3_complex_mul_vect_complex_s32_calc_params(&a_exp, &b_shr, &c_shr, b->exp, c->exp, 
                                                 b->hr, c->hr, XS3_BFP_ALLOW_SATURATION);

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

    exponent_t a_exp;
    right_shift_t sat;

    xs3_complex_mul_vect_complex_s16_calc_params(&a_exp, &sat, b->exp, c->exp, b->hr, c->hr, XS3_BFP_ALLOW_SATURATION);

    a->length = b->length;
    a->exp = a_exp;
    a->hr = xs3_complex_conj_mul_vect_complex_s16(a->real, a->imag, b->real, b->imag, c->real, c->imag, b->length, sat);
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

    exponent_t a_exp;
    right_shift_t b_shr, c_shr;

    xs3_complex_mul_vect_complex_s32_calc_params(&a_exp, &b_shr, &c_shr, 
                                                 b->exp, c->exp, b->hr, c->hr,
                                                 XS3_BFP_ALLOW_SATURATION);

    a->length = b->length;
    a->exp = a_exp;
    a->hr = xs3_complex_conj_mul_vect_complex_s32(a->data, b->data, c->data, 
                                                  b->length, b_shr, c_shr);
}




void bfp_scalar_mul_vect_complex_s16(
    bfp_complex_s16_t* a, 
    const bfp_complex_s16_t* b, 
    const int16_t alpha_mant,
    const exponent_t alpha_exp)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
#endif

    right_shift_t sat;
    headroom_t s_hr = HR_S16(alpha_mant);

    xs3_scalar_mul_vect_s16_calc_params(&a->exp, &sat, b->exp, alpha_exp, 
                                                b->hr, s_hr, XS3_BFP_ALLOW_SATURATION);
    a->length = b->length;
    a->hr = xs3_scalar_mul_vect_complex_s16(a->real, a->imag, b->real, b->imag, alpha_mant, b->length, sat);
}




void bfp_scalar_mul_vect_complex_s32(
    bfp_complex_s32_t* a, 
    const bfp_complex_s32_t* b, 
    const int32_t alpha_mant,
    const exponent_t alpha_exp)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
#endif

    right_shift_t b_shr, s_shr;

    headroom_t s_hr = HR_S32(alpha_mant);

    // @todo This calc_params() func needs an 'allow_saturation' argument.
    //Uses same param logic as mul_vect_s32
    xs3_mul_vect_s32_calc_params(&a->exp, &b_shr, &s_shr, b->exp, alpha_exp, b->hr, s_hr);
    int32_t alpha = SIGNED_ASHR(alpha_mant, s_shr);

    a->length = b->length;
    a->hr = xs3_scalar_mul_vect_complex_s32( a->data, b->data, alpha, b->length, b_shr);
}


void bfp_complex_scal_mul_vect_complex_s16(
    bfp_complex_s16_t* a, 
    const bfp_complex_s16_t* b,
    const complex_s16_t alpha_mant,
    const exponent_t alpha_exp)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
#endif

    right_shift_t sat;

    headroom_t alpha_hr = HR_C16(alpha_mant);

    xs3_complex_mul_vect_complex_s16_calc_params(&a->exp, &sat, b->exp, alpha_exp, b->hr, alpha_hr, XS3_BFP_ALLOW_SATURATION);

    a->length = b->length;

    a->hr = xs3_complex_scal_mul_vect_complex_s16(a->real, a->imag, b->real, b->imag, 
                                                  alpha_mant.re, alpha_mant.im, 
                                                  b->length, sat);
}

void bfp_complex_scal_mul_vect_complex_s32(
    bfp_complex_s32_t* a, 
    const bfp_complex_s32_t* b, 
    const complex_s32_t alpha_mant,
    const exponent_t alpha_exp)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
#endif

    right_shift_t b_shr, alpha_shr;
    const headroom_t alpha_hr = HR_C32(alpha_mant);

    xs3_complex_scal_mul_vect_complex_s32_calc_params(&a->exp, &b_shr, &alpha_shr, b->exp, alpha_exp,
                                                      b->hr, alpha_hr, XS3_BFP_ALLOW_SATURATION);

    const complex_s32_t c = { ASHR32(alpha_mant.re, alpha_shr), ASHR32(alpha_mant.im, alpha_shr) };
    
    a->length = b->length;
    a->hr = xs3_complex_scal_mul_vect_complex_s32(a->data, b->data, c.re, c.im, b->length, b_shr);
}
void bfp_complex_s16_to_complex_s32(
    bfp_complex_s32_t* a, 
    const bfp_complex_s16_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
#endif

    xs3_complex_s16_to_complex_s32(a->data, b->real, b->imag, b->length);

    a->length = b->length;
    a->exp = b->exp;
    a->hr = b->hr + 16;
}





void bfp_complex_s32_to_complex_s16(
    bfp_complex_s16_t* a, 
    const bfp_complex_s32_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
#endif

    const right_shift_t b_shr = 16 - b->hr;

    xs3_complex_s32_to_complex_s16(a->real, a->imag, b->data, b->length, b_shr);

    a->length = b->length;
    a->exp = b->exp + b_shr;
    a->hr = 0;
}





void bfp_squared_mag_vect_complex_s16(
    bfp_s16_t* a, 
    const bfp_complex_s16_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
#endif

    right_shift_t sat;

    xs3_squared_mag_vect_complex_s16_calc_params(&a->exp, &sat, b->exp, b->hr, XS3_BFP_ALLOW_SATURATION);

    a->length = b->length;
    a->hr = xs3_squared_mag_vect_complex_s16(a->data, b->real, b->imag, b->length, sat);
}





void bfp_squared_mag_vect_complex_s32(
    bfp_s32_t* a, 
    const bfp_complex_s32_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
#endif

    right_shift_t b_shr;

    xs3_squared_mag_vect_complex_s32_calc_params(&a->exp, &b_shr, b->exp, b->hr, XS3_BFP_ALLOW_SATURATION);

    a->length = b->length;
    a->hr = xs3_squared_mag_vect_complex_s32(a->data, b->data, b->length, b_shr);
}



void bfp_mag_vect_complex_s16(
    bfp_s16_t* a, 
    const bfp_complex_s16_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
#endif

    right_shift_t b_shr;

    xs3_mag_vect_complex_calc_params(&a->exp, &b_shr, b->exp, b->hr, XS3_BFP_ALLOW_SATURATION);

    a->length = b->length;

    a->hr = xs3_mag_vect_complex_s16(a->data, b->real, b->imag, b->length, 
                                     b_shr, (int16_t*) rot_table16, rot_table16_rows);
}




void bfp_mag_vect_complex_s32(
    bfp_s32_t* a, 
    const bfp_complex_s32_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
#endif

    right_shift_t b_shr;

    xs3_mag_vect_complex_calc_params(&a->exp, &b_shr, b->exp, b->hr, XS3_BFP_ALLOW_SATURATION);

    a->length = b->length;
    a->hr = xs3_mag_vect_complex_s32(a->data, b->data, b->length, 
                                     b_shr, (complex_s32_t*) rot_table32, rot_table32_rows);
}




complex_s32_t bfp_sum_complex_s16(
    const bfp_complex_s16_t* b)
{
    return xs3_sum_complex_s16(b->real, b->imag, b->length);
}




complex_s64_t bfp_sum_complex_s32( 
    exponent_t* a_exp,
    const bfp_complex_s32_t* b)
{
    right_shift_t b_shr;
    complex_s64_t res;

    xs3_sum_complex_s32_calc_params(a_exp, &b_shr, b->exp, b->hr, b->length, XS3_BFP_ALLOW_SATURATION);
    xs3_sum_complex_s32(&res, b->data, b->length, b_shr);
    return res;
}