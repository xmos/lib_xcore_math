
#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "vpu_helper.h"




////////////////////////////////////////
//  Shared params (16- and 32-bit)    //
////////////////////////////////////////



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
void xs3_add_sub_vect_calc_params(
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

    *a_exp = XS3_MAX(b_min_exp, c_min_exp) + 1;

    if((b_min_exp == c_min_exp) && !(allow_saturation))
        *a_exp = *a_exp + 1;

    *b_shr = *a_exp - b_exp;
    *c_shr = *a_exp - c_exp;
}

    



////////////////////////////////////////
//      Params for 16-bit             //
////////////////////////////////////////


    
/* ******************
 *
 * 
 * ******************/
void xs3_vect_s16_mul_calc_params(
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
void xs3_vect_s16_dot_calc_params(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    right_shift_t* c_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr,
    const unsigned length,
    const unsigned allow_saturation)
{
    /*
    
    Operation is
        A = sum[k]{ (B[k] >> b_shr) * (C[k] >> c_shr) }

    Worst case result is when each B[k] = (-0x8000 >> b_hr) and each C[k] = (-0x8000 >> c_hr)
    Then the result is

        A = length * ( (-0x8000 >> b_hr) >> b_shr) * ((-0x8000 >> c_hr) >> c_shr )
          = length * ( -2^15 * 2^-b_hr * 2^-b_shr * -2^15 * 2^-c_hr * 2^-c_shr )
          = length * 2^(30-b_hr-b_shr-c_hr-c_shr )
            total_hr = b_hr+c_hr
            total_shr = b_shr + c_shr
        A = length * 2^(30-total_hr-total_shr)

    Suppose length happens to be a power of 2:  length = 2^(L)

        A = 2^L * 2^(30-total_hr-total_shr)
        A = 2^(30+L-total_hr-total_shr)

    In this case, we want the result to be  2^31 (which saturates to INT32_MAX) if saturation is allowed, and 2^30 
    otherwise.

        2^30 << allow_sat = 2^( 30 + L - total_hr - total_shr )
        2^( 30 + allow_sat ) = 2^( 30 + L - total_hr - total_shr )
        30 + allow_sat = 30 + L - total_hr - total_shr
        total_shr = L - allow_sat - total_hr

    If length isn't an exact power of 2, then round up, and ignore allow_saturation (assume it's 1)
        K = ceil_log2(length)
        T = allow_sat || ((1<<K) == length)
        total_shr = K - T - total_hr
    
    Note: if total_shr is negative, we don't need to do anything -- the only advantage to left-shifting the
          inputs is that the result has less headroom.

    */
   
   const int K = ceil_log2(length);
   const int T = (allow_saturation || (length != (1 << K)))? 1 : 0;
   const headroom_t total_hr = b_hr + c_hr;
   const right_shift_t total_shr = K - T - total_hr;

    if(total_shr <= 0){
        *c_shr = *b_shr = 0;
    } else {
        *b_shr = total_shr >> 1;
        *c_shr = total_shr - *b_shr;
    }

    *a_exp = b_exp + c_exp + *b_shr + *c_shr;
}



////////////////////////////////////////
//      Params for 32-bit             //
////////////////////////////////////////


/* ******************
 *  Note: this is the same for both 16 and 32 bits. Might warrant a renaming?
 *        Used by at least 3 different functions. although the output a_exp needs
 *        to be adjusted for 16-bit... so maybe just create a second function?
 * 
 * ******************/
void xs3_vect_s32_mul_calc_params(
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



void xs3_vect_s16_scalar_mul_calc_params(
    exponent_t* a_exp,
    right_shift_t* sat,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr,
    const unsigned allow_saturation)
{

    headroom_t total_hr = b_hr + c_hr;

    /*
        Operation is:
            A = ( B * C ) / 2^sat

            max(A) = (-2^15 * 2^-b_hr) * (-2^15 * 2^-c_hr) * 2^-sat
                      = 2^30 * 2^(-b_hr-c_hr-sat)
                      = 2^(30 - total_hr - sat)

        We want that case to yield (2^15) is saturation is allowed, and (2^14) otherwise.

            2^(14+allow_sat) = 2^(30 - total_hr - sat)
            14 + allow_sat = 30 - total_hr - sat
            sat = 30 - 14 - total_hr - allow_sat
            sat = 16 - allow_sat - total_hr

            sat = (allow_sat? 15 : 16) - total_hr
    */

    *sat = MAX(0, (int) ((allow_saturation? 15 : 16) - total_hr) );

    *a_exp = b_exp + c_exp + *sat;
}



void xs3_vect_s16_sqrt_calc_params(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    const exponent_t b_exp,
    const right_shift_t b_hr)
{

    *b_shr = -((int)b_hr);

    // sqrt(X * 2^P) = sqrt(X) * sqrt(2^P) 
    //               = sqrt(X) * 2^(P/2)
    // But we can't have fractional exponents, so leave one bit of headroom if b_shr would cause 
    // the exponent to be odd.
    if( ((unsigned)(b_exp + *b_shr)) % 2 == 1){
        *b_shr += 1;
    }


    *a_exp = (b_exp + *b_shr - 14) >> 1;


}



    
/* ******************
 *
 * 
 * ******************/
void xs3_vect_s32_dot_calc_params(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    right_shift_t* c_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr,
    const unsigned length,
    const unsigned allow_saturation)
{
    /*
    
    Operation is
        A = sum[k]{ (B[k] >> b_shr) * (C[k] >> c_shr) >> 30 }

    Worst case result is when each B[k] = (-0x80000000 >> b_hr) = -2^(31-b_hr) 
    and each C[k] = (-0x80000000 >> c_hr) = -2^(31-c_hr), then the result is

        A = length * (  (-2^(31-b_hr) >> b_shr)  *  (-2^(31-b_hr) >> c_shr)  ) >> 30
          = length * ( -2^(31-b_hr-b_shr) * -2^(31-c_hr-c_shr) ) * 2^-30
          = length * 2^(62-b_hr-b_shr-c_hr-c_shr-30)
          = length * 2^(32-b_hr-b_shr-c_hr-c_shr)
            total_hr = b_hr+c_hr
            total_shr = b_shr + c_shr
        A = length * 2^(32-total_hr-total_shr)

    Suppose length happens to be a power of 2:  length = 2^(L)

        A = 2^L * 2^(32-total_hr-total_shr)
        A = 2^(32+L-total_hr-total_shr)

    In this case, we want the result to be  2^39 (which saturates to ((2^39)-1)) if saturation is allowed, and 2^38 
    otherwise.

        2^38 << allow_sat = 2^( 32 + L - total_hr - total_shr )
        2^( 38 + allow_sat ) = 2^( 32 + L - total_hr - total_shr )
        38 + allow_sat = 32 + L - total_hr - total_shr
        total_shr = L - allow_sat - total_hr - 6

    If length isn't an exact power of 2, then round up, and ignore allow_saturation (assume it's 1)
        K = ceil_log2(length)
        T = allow_sat || ((1<<K) == length)
        total_shr = K - T - total_hr - 6

    */

    const int K = ceil_log2(length);
    const int T = (allow_saturation || (length != (1 << K)))? 1 : 0;
    const headroom_t total_hr = b_hr + c_hr;
    right_shift_t total_shr = K - T - 6;

    *b_shr = (int) -b_hr;
    *c_shr = (int) -c_hr;

    if(total_shr < 0){
        // Do nothing. We've already eliminated all our headroom.
    } else if(total_shr >= total_hr){

        *b_shr += b_hr;
        *c_shr += c_hr;
        total_shr -= total_hr;
        *b_shr += (total_shr >> 1);
        *c_shr += total_shr - (total_shr >> 1);

    } else {

        *b_shr += MIN(b_hr, total_shr);
        *c_shr += total_shr - MIN(b_hr, total_shr);

    }

    *a_exp = b_exp + c_exp + *b_shr + *c_shr + 30;
}



void xs3_vect_s32_sqrt_calc_params(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    const exponent_t b_exp,
    const right_shift_t b_hr)
{

    *b_shr = -((int)b_hr);

    // sqrt(X * 2^P) = sqrt(X) * sqrt(2^P) 
    //               = sqrt(X) * 2^(P/2)
    // But we can't have fractional exponents, so leave one bit of headroom if b_shr would cause 
    // the exponent to be odd.
    if( ((unsigned)(b_exp + *b_shr)) % 2 == 1){
        *b_shr += 1;
    }


    *a_exp = (b_exp + *b_shr - 30) >> 1;


}
