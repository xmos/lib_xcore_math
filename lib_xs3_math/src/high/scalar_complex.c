

#include "xs3_math.h"

#include "low/xs3_bfp_low.h"
#include "low/xs3_bfp_complex_low.h"

#define ASSERT(COND)    assert(COND)




headroom_t xs3_headroom_complex_s16(
    const complex_s16_t a)
{
    return HR_C16(a);
}


headroom_t xs3_headroom_complex_s32(
    const complex_s32_t a)
{
    return HR_C32(a);
}




void bfp_add_complex_s16(
    complex_s16_t* a, 
    exponent_t* a_exp, 
    const complex_s16_t b, 
    const exponent_t b_exp)
{
    //TODO
}

void bfp_add_complex_s32(
    complex_s32_t* a, 
    exponent_t* a_exp, 
    const complex_s32_t b, 
    const exponent_t b_exp)
{
    //TODO
}

void bfp_sub_complex_s16(
    complex_s16_t* a, 
    exponent_t* a_exp, 
    const complex_s16_t b, 
    const exponent_t b_exp)
{
    //TODO
}


void bfp_sub_complex_s32(
    complex_s32_t* a, 
    exponent_t* a_exp, 
    const complex_s32_t b, 
    const exponent_t b_exp)
{
    //TODO
}

    
void bfp_mul_complex_s16(
    complex_s16_t* a, 
    exponent_t* a_exp, 
    const int16_t b, 
    const exponent_t b_exp)
{
    //TODO
}

    
void bfp_mul_complex_s32(
    complex_s32_t* a, 
    exponent_t* a_exp, 
    const int32_t b, 
    const exponent_t b_exp)
{
    //TODO
}

    
void bfp_complex_mul_complex_s16(
    complex_s16_t* a, 
    exponent_t* a_exp, 
    const complex_s16_t b, 
    const exponent_t b_exp)
{
    //TODO
}

    
void bfp_complex_mul_complex_s32(
    complex_s32_t* a, 
    exponent_t* a_exp, 
    const complex_s32_t b, 
    const exponent_t b_exp)
{
    //TODO
}

    
void bfp_complex_conj_mul_complex_s16(
    complex_s16_t* a, 
    exponent_t* a_exp, 
    const complex_s16_t b, 
    const exponent_t b_exp)
{
    //TODO
}

    
void bfp_clpx_conj_mul_complex_s32(
    complex_s32_t* a, 
    exponent_t* a_exp, 
    const complex_s32_t b, 
    const exponent_t b_exp)
{
    //TODO
}