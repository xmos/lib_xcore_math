

#include "bfp_math.h"

#include "vect/xs3_vect_s32.h"
#include "vect/xs3_vect_s16.h"
#include "../vect/vpu_helper.h"

#define ASSERT(COND)    assert(COND)


const extern unsigned rot_table32_rows;
const extern complex_s32_t rot_table32[30][4];


headroom_t bfp_complex_s32_headroom(
    bfp_complex_s32_t* a)
{
    a->hr = xs3_vect_s32_headroom((int32_t*)a->data, 2 * a->length);
    return a->hr;
}


void bfp_complex_s32_shl(
    bfp_complex_s32_t* a,
    const bfp_complex_s32_t* b,
    const left_shift_t shl)
{
#if XS3_BFP_DEBUG_CHECK_LENGTHS
    assert(a->length == b->length);
#endif

    a->length = b->length;
    a->exp = b->exp;
    a->hr = xs3_vect_s32_shl((int32_t*) a->data, (int32_t*) b->data, 2*b->length, shl);
}


void bfp_complex_s32_add(
    bfp_complex_s32_t* a, 
    const bfp_complex_s32_t* b, 
    const bfp_complex_s32_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
    assert(b->length == c->length);
#endif

    right_shift_t b_shr, c_shr;

    xs3_vect_add_sub_calc_params(&a->exp, &b_shr, &c_shr, b->exp, c->exp, 
                             b->hr, c->hr, XS3_BFP_ALLOW_SATURATION);

    a->length = b->length;
    a->hr = xs3_vect_complex_s32_add(a->data, b->data, c->data, b->length, b_shr, c_shr);
}


void bfp_complex_s32_sub(
    bfp_complex_s32_t* a, 
    const bfp_complex_s32_t* b,
    const bfp_complex_s32_t* c)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
    assert(b->length == c->length);
#endif

    right_shift_t b_shr, c_shr;

    xs3_vect_add_sub_calc_params(&a->exp, &b_shr, &c_shr, b->exp, c->exp, 
                             b->hr, c->hr, XS3_BFP_ALLOW_SATURATION);

    a->length = b->length;
    a->hr = xs3_vect_complex_s32_sub(a->data, b->data, c->data, b->length, b_shr, c_shr);
}


void bfp_complex_s32_real_mul(
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

    xs3_vect_complex_s32_real_mul_calc_params(&a_exp, &b_shr, &c_shr,    
                                         b->exp, c->exp, b->hr, c->hr, 
                                         XS3_BFP_ALLOW_SATURATION);

    a->length = b->length;
    a->exp = a_exp;

    a->hr = xs3_vect_complex_s32_real_mul(a->data, b->data, c->data, b->length, b_shr, c_shr);
}


void bfp_complex_s32_mul(
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

    xs3_vect_complex_s32_complex_mul_calc_params(&a_exp, &b_shr, &c_shr, b->exp, c->exp, 
                                                 b->hr, c->hr, XS3_BFP_ALLOW_SATURATION);

    a->length = b->length;
    a->exp = a_exp;
    a->hr = xs3_vect_complex_s32_complex_mul(a->data, b->data, c->data, b->length, b_shr, c_shr);
}


void bfp_complex_s32_conj_mul(
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

    xs3_vect_complex_s32_complex_mul_calc_params(&a_exp, &b_shr, &c_shr, 
                                                 b->exp, c->exp, b->hr, c->hr,
                                                 XS3_BFP_ALLOW_SATURATION);

    a->length = b->length;
    a->exp = a_exp;
    a->hr = xs3_vect_complex_s32_conj_mul(a->data, b->data, c->data, 
                                                  b->length, b_shr, c_shr);
}


void bfp_complex_s32_real_scale(
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
    xs3_vect_s32_mul_calc_params(&a->exp, &b_shr, &s_shr, b->exp, alpha_exp, b->hr, s_hr);
    int32_t alpha = SIGNED_ASHR(alpha_mant, s_shr);

    a->length = b->length;
    a->hr = xs3_vect_complex_s32_real_scale( a->data, b->data, alpha, b->length, b_shr);
}


void bfp_complex_s32_scale(
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

    xs3_vect_complex_s32_scale_calc_params(&a->exp, &b_shr, &alpha_shr, b->exp, alpha_exp,
                                                      b->hr, alpha_hr, XS3_BFP_ALLOW_SATURATION);

    const complex_s32_t c = { ASHR32(alpha_mant.re, alpha_shr), ASHR32(alpha_mant.im, alpha_shr) };
    
    a->length = b->length;
    a->hr = xs3_vect_complex_s32_scale(a->data, b->data, c.re, c.im, b->length, b_shr);
}


void bfp_complex_s32_squared_mag(
    bfp_s32_t* a, 
    const bfp_complex_s32_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
#endif

    right_shift_t b_shr;

    xs3_vect_complex_s32_squared_mag_calc_params(&a->exp, &b_shr, b->exp, b->hr, XS3_BFP_ALLOW_SATURATION);

    a->length = b->length;
    a->hr = xs3_vect_complex_s32_squared_mag(a->data, b->data, b->length, b_shr);
}


void bfp_complex_s32_mag(
    bfp_s32_t* a, 
    const bfp_complex_s32_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
#endif

    right_shift_t b_shr;

    xs3_vect_complex_mag_calc_params(&a->exp, &b_shr, b->exp, b->hr, XS3_BFP_ALLOW_SATURATION);

    a->length = b->length;
    a->hr = xs3_vect_complex_s32_mag(a->data, b->data, b->length, 
                                     b_shr, (complex_s32_t*) rot_table32, rot_table32_rows);
}


complex_s64_t bfp_complex_s32_sum( 
    exponent_t* a_exp,
    const bfp_complex_s32_t* b)
{
    right_shift_t b_shr;
    complex_s64_t res;

    xs3_vect_complex_s32_sum_calc_params(a_exp, &b_shr, b->exp, b->hr, b->length, XS3_BFP_ALLOW_SATURATION);
    xs3_vect_complex_s32_sum(&res, b->data, b->length, b_shr);
    return res;
}


void bfp_complex_s32_to_complex_s16(
    bfp_complex_s16_t* a, 
    const bfp_complex_s32_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(b->length == a->length);
#endif

    const right_shift_t b_shr = 16 - b->hr;

    xs3_vect_complex_s32_to_complex_s16(a->real, a->imag, b->data, b->length, b_shr);

    a->length = b->length;
    a->exp = b->exp + b_shr;
    a->hr = 0;
}