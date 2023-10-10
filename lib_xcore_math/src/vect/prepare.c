// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>

#include "xmath/xmath.h"
#include "xmath/xs3/vpu_scalar_ops.h"


////////////////////////////////////////
//      Params for 16-bit             //
////////////////////////////////////////


void vect_s16_macc_prepare(
    exponent_t* new_acc_exp,
    right_shift_t* acc_shr,
    right_shift_t* bc_shr,
    const exponent_t acc_exp,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t acc_hr,
    const headroom_t b_hr,
    const headroom_t c_hr)
{

  /*
    Most extreme result is when  B = -2^(15-b_hr) and C = -2^(15-c_hr). We want their product, right-shifted
    by bc_shr, to have 1 bit of headroom in an int16_t, which means 2^14.

      2^14 = -2^(15-b_hr) * -2^(15-c_hr) * 2^-bc_shr
           = 2^(15 - b_hr + 15 - c_hr - bc_shr)
           = 2^(30 - (b_hr+c_hr) - bc_shr)

      14 = 30 - (b_hr+c_hr) - bc_shr

      bc_shr = 16 - (b_hr + c_hr)
  */
  const headroom_t bc_hr = b_hr + c_hr;

  *bc_shr = 16 - bc_hr;

  // The exponent associated with the right-shifted product of B and C
  const exponent_t bc_exp = b_exp + c_exp - bc_hr + 16;

  // The exponent that acc[] would have if it had exactly one bit of headroom.
  // This way
  const exponent_t tmp_exp = acc_exp - acc_hr + 1;

  // The new exponent should be whichever of those two exponents is greater.
  *new_acc_exp = (bc_exp > tmp_exp)? bc_exp : tmp_exp;
  
  // Can compute the proper shifts now
  *acc_shr = *new_acc_exp - acc_exp;
  *bc_shr += *new_acc_exp - bc_exp;
}

    
/* ******************
 *
 *
 * ******************/
void vect_s16_mul_prepare(
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
    */
    headroom_t total_hr = b_hr+c_hr;
    *a_shr = 16-total_hr;
    *a_shr = (*a_shr < 0)? 0 : *a_shr;

    *a_exp = (b_exp+c_exp)+*a_shr;
}

void vect_s16_clip_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    int16_t* lower_bound,
    int16_t* upper_bound,
    const exponent_t b_exp,
    const exponent_t bound_exp,
    const headroom_t b_hr)
{
    (void) b_hr;
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
    
    *a_exp = b_exp; //minimum b exponent
    
    right_shift_t bound_shr = *a_exp - bound_exp;
    *b_shr = *a_exp - b_exp;

    int16_t lb;
    int16_t ub;

    if(bound_shr < 0){
        int32_t ub32 = ((int32_t)*upper_bound) << (-bound_shr);
        int32_t lb32 = ((int32_t)*lower_bound) << (-bound_shr);

        ub = (ub32 >= VPU_INT16_MAX)? VPU_INT16_MAX : (ub32 <= VPU_INT16_MIN)? VPU_INT16_MIN : (int16_t) ub32;
        lb = (lb32 >= VPU_INT16_MAX)? VPU_INT16_MAX : (lb32 <= VPU_INT16_MIN)? VPU_INT16_MIN : (int16_t) lb32;
    } else {
        // TODO: Should force upper_bound to round downwards to enforce the guarantee that no output can be larger than
        // upper bound?
        ub = *upper_bound >> bound_shr;
        // And lower bound upwards?
        lb = (*lower_bound + ((1<<bound_shr)-1)) >> bound_shr;
    }

    if(ub == VPU_INT16_MIN){
        /* upper bound must be smaller than any element of b, so set everything to that */
        *a_exp = bound_exp;
    } else if(lb == VPU_INT16_MAX){
        /* lower bound must be larger than any element of b, so set everything to that */
        *a_exp = bound_exp;
    }

    *lower_bound = lb;
    *upper_bound = ub;
}


////////////////////////////////////////
//      Params for 32-bit             //
////////////////////////////////////////

void vect_s32_macc_prepare(
    exponent_t* new_acc_exp,
    right_shift_t* acc_shr,
    right_shift_t* b_shr,
    right_shift_t* c_shr,
    const exponent_t acc_exp,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t acc_hr,
    const headroom_t b_hr,
    const headroom_t c_hr)
{

  /*
    We want the product of B[] and C[] to end up with 1 bit of headroom in an int32_t. 1 bit of headroom in
    an int32_t means a maximum of +/- 2^30.  Most extreme case is if B[] and C[] are -2^(31-b_hr) and -2^(31-c_hr)
    respectively. So, let's solve for the b_shr+c_shr that makes this work. Also there's an implicit 30-bit
    right-shift applied by the VPU when multiplying.
                  
      2^30 = -2^(31-b_hr-b_shr) * -2^(31-c_hr-c_shr) * 2^-30
           = 2^(31-b_hr-b_shr) * 2^(31-c_hr-c_shr) * 2^-30
           = 2^(62 - b_hr - c_hr - b_shr - c_shr - 30)
      30 = 32 - b_hr - c_hr - b_shr - c_shr

      --->  (b_shr+c_shr) = 2 - (b_hr + c_hr)
  */

  *b_shr = 1-b_hr;
  *c_shr = 1-c_hr;

  // exponent_t p_exp = b_exp + c_exp + *b_shr + *c_shr + 30;
  exponent_t p_exp = b_exp + c_exp - b_hr - c_hr + 32;
  // d_exp is the exponent acc[] would have if it were shifted to have 1 bit of headroom
  exponent_t d_exp = acc_exp - acc_hr + 1;

  *new_acc_exp = (d_exp > p_exp)? d_exp : p_exp;

  *acc_shr = *new_acc_exp - acc_exp;

  right_shift_t p_shr = *new_acc_exp - p_exp;

  // p_shr needs to be split between b_shr and c_shr. (it can't be negative)
  *b_shr += (p_shr>>1);
  *c_shr += p_shr - (p_shr>>1);
}


/* ******************
 *  Note: this is the same for both 16 and 32 bits. Might warrant a renaming?
 *        Used by at least 3 different functions. although the output a_exp needs
 *        to be adjusted for 16-bit... so maybe just create a second function?
 *
 * ******************/
void vect_s32_mul_prepare(
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



void vect_s16_scale_prepare(
    exponent_t* a_exp,
    right_shift_t* a_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr)
{

    headroom_t total_hr = b_hr + c_hr;

    /*
        Operation is:
            A = ( B * C ) / 2^a_shr

            max(A) = (-2^15 * 2^-b_hr) * (-2^15 * 2^-c_hr) * 2^-a_shr
                      = 2^30 * 2^(-b_hr-c_hr-a_shr)
                      = 2^(30 - total_hr - a_shr)

        We want that case to yield (2^15) is saturation is allowed, and (2^14) otherwise.

            2^15 = 2^(30 - total_hr - a_shr)
            15 = 30 - total_hr - a_shr
            a_shr = 30 - 15 - total_hr

            a_shr = 15 - total_hr
    */

    *a_shr = MAX(0, (int) (15 - total_hr) );

    *a_exp = b_exp + c_exp + *a_shr;
}



void vect_s16_sqrt_prepare(
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


static int16_t min_abs_s16(
    const int16_t b[],
    const unsigned length)
{
    int16_t m = INT16_MAX;

    for(unsigned i = 0; i < length; i++){
        int16_t tmp = vlmul16(b[i], vsign16(b[i]));
        m = MIN(m, tmp);
    }

    return m;
}

static int32_t min_abs_s32(
    const int32_t b[],
    const unsigned length)
{
    int32_t m = INT32_MAX;

    for(unsigned i = 0; i < length; i++){
        int32_t tmp = vlmul32(b[i], vsign32(b[i]));
        m = MIN(m, tmp);
    }

    return m;
}

void vect_s16_inverse_prepare(
    exponent_t* a_exp,
    unsigned* scale,
    const int16_t b[],
    const exponent_t b_exp,
    const unsigned length)
{

    // Performing a signed division

    //   0x40000000 / b[k]
    // = 2^30 / b[k]
    // = 2^30 * (1/b[k])

    // We'll need to shift down the result based on the smallest magnitude element of b[]

    int16_t a = min_abs_s16( b, length );

    headroom_t hr = HR_S16(a);

    //  2^(14-hr) <= abs(a)
    //  with equality:

    // 2^30 / 2^(14-hr)
    // = 2^(30-14+hr) = 2^(16+hr)

    // 2^(16+hr) >= 2^30 / abs(a) > 2^(15+hr)

    // We want the result for a to be between 2^13 and 2^14..

    // 2^(16+hr) >> shr = 2^14
    // 2^(16+hr-shr) = 2^14
    // 16+hr-shr = 14
    // shr = 2+hr

    right_shift_t shr = 2+hr;

    *scale = 28-hr;

    // As for the result exponent..

    //   1 / (x * 2^b_exp)
    // = (1 / x) * 2^-b_exp
    // = ( 2^(30-shr) / 2^(30-shr) ) * ( 1 / x ) * 2^(-b_exp)
    // = (2^(30-shr)/x)  * 2^(shr-30-b_exp)

    *a_exp = shr - b_exp - 30;
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
void vect_s32_add_prepare(
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

    *a_exp = MAX(b_min_exp, c_min_exp) + 1;

    *b_shr = *a_exp - b_exp;
    *c_shr = *a_exp - c_exp;
}


    
/* ******************
 *
 *
 * ******************/
void vect_s32_dot_prepare(
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

    In this case, we want the result to be  2^39 (which saturates to ((2^39)-1))

        2^39 = 2^( 32 + L - total_hr - total_shr )
        2^39 = 2^( 32 + L - total_hr - total_shr )
        39 = 32 + L - total_hr - total_shr
        total_shr = L - total_hr - 7

    If length isn't an exact power of 2, then round up
        K = u32_ceil_log2(length)
        total_shr = K - total_hr - 7

    */

    const int K = u32_ceil_log2(length);
    const headroom_t total_hr = b_hr + c_hr;
    right_shift_t total_shr = K - 7;

    *b_shr = -(int)b_hr;
    *c_shr = -(int)c_hr;

    if(total_shr < 0){
        // Do nothing. We've already eliminated all our headroom.
        // we know that total_shr is more than zero, so safely recasting to unsigned
    } else if((unsigned)total_shr >= total_hr){

        *b_shr += b_hr;
        *c_shr += c_hr;
        total_shr -= total_hr;
        *b_shr += (total_shr >> 1);
        *c_shr += total_shr - (total_shr >> 1);

    } else {

        *b_shr += MIN(b_hr, (unsigned)total_shr);
        *c_shr += total_shr - MIN(b_hr, (unsigned)total_shr);

    }

    *a_exp = b_exp + c_exp + *b_shr + *c_shr + 30;
}



void vect_s32_sqrt_prepare(
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


void vect_s32_inverse_prepare(
    exponent_t* a_exp,
    unsigned* scale,
    const int32_t b[],
    const exponent_t b_exp,
    const unsigned length)
{

    int32_t a = min_abs_s32( b, length );

    headroom_t hr = HR_S32(a);
    //      2^(30-hr)  <=  a  <  2^(31-hr)


    //  max{ 2^K / a }   -->  2^K / min{a}
    // minimum value a could be is 2^(30-hr)
    //   = 2^K / 2^(30-hr)
    //  max{ 2^K / a } = 2^(K - 30 + hr)


    //  min{ 2^K / a }  -->  2^K / max{a}
    // maximum value a could be is 2^(31-hr) (if negative)
    //   =  2^K / 2^(31-hr) = 2^(K - 31 + hr)
    //  min{ 2^K / 2^(31-hr) } = 2^(K - 31 + hr)


    // So,
    //  2^(K-31+hr)  <=  (2^K/a)  <=  2^(K-30-hr)
    
    // To get the most precision, we would ideally want the result to be between  2^30 and 2^31,
    // but unfortunately we can't represent 2^31 and so we have to give up 1 bit of precision, just
    // in case the headroom is dominated by a positive power of 2. (Testing for this condition is
    // too expensive).

    // K-31+hr = 29  -> K+hr = 60
    // K = 60-hr

    int K = 60 - hr;

    // This will be what each element should be divided into.
    //    uint64_t d = (0x1ULL << K);

    // So, instead of computing  1/v  we ended up computing  2^K/v, which is
    // 2^K times larger than 1/v.

    *a_exp = -b_exp - K;

    *scale = K;
}


void vect_s32_energy_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    const unsigned length,
    const exponent_t b_exp,
    const headroom_t b_hr)
{
    /*
        b_shr needs to be calculated based on the worst-case result, given the element count and headroom of the
        vector b[].

        Worst case occurs when every value is INT32_MIN = -0x8000 = -(2^31).
        However, we know that we have at least  b_hr bits of headroom on each element, so given b_hr, the worst
        case is when each value is  INT32_MIN >> b_hr  = -(2^31) * 2^(-b_hr) = -(2^(31-b_hr))

        And there's an extra 30-bit right-shift when we square the 32-bit numbers.

        max_result = (-(2^31) * 2^(-b_hr) >> b_shr)^2 * 2^(-30) * len
                   = (-(2^31) * 2^(-b_hr) * 2^(-b_shr))^2 * 2^(-30) * len
                   = (-(2^(31-b_hr-b_shr)))^2 * 2^(-30) * len
                   = 2^(2*(31-b_hr-b_shr)-30) * len
                   = 2^(62 - 2*(b_hr+b_shr)-30) * len
                   = 2^(32 - 2*(b_hr+b_shr)) * len

        If we round len up to the next greater power of 2, we can solve for b_shr without computing any actual logs

        len' = 2^(u32_ceil_log2(len))

        max_result' = 2^(32 - 2*(b_hr+b_shr)) * len'
                    = 2^(32 - 2*(b_hr+b_shr)) * 2^(u32_ceil_log2(len))
                    = 2^(32 - 2*(b_hr+b_shr) + u32_ceil_log2(len))

        The accumulator is 40 bits. The max value we can hit is 2^39 - 1. So set the target based on whether
        allow_saturation is true or not

        2^(38+allow_sat) = 2^(32 - 2*(b_hr+b_shr) + u32_ceil_log2(len))
        38 + allow_sat = 32 - 2*(b_hr+b_shr) + u32_ceil_log2(len)
        0 = 32 - 38 - allow_sat - 2*b_hr - 2*b_shr + u32_ceil_log2(len)
        2*b_shr = -6 - allow_sat - 2*b_hr + u32_ceil_log2(len)
        b_shr = ( u32_ceil_log2(len) - 6 - allow_sat )/2 + b_hr


        If (u32_ceil_log2(len)-7) happens to be odd, b_shr will be too small. If we add 1 before dividing by 2, then
            - If u32_ceil_log2(len)-6 is odd, then    (u32_ceil_log2(len)-6)/2 == (u32_ceil_log2(len)-7)/2
            - If u32_ceil_log2(len)-6 is even, then (u32_ceil_log2(len)-6)/2 will avoid underestimating b_shr
    */


    *b_shr = (((int)u32_ceil_log2(length))-6)/2 - b_hr;

    *b_shr = MAX(*b_shr, -((int)b_hr));

    // b_shr == -14 is annoying because  ( ((X[k]<<14)^2) & (1<<29) )  is always be 0, which means
    // that when the 30-bit right-shift happens, it will always round down (square is guaranteed to
    // be positive), which in turn means the error is not zero mean, which in turn means that the
    // expected error grows with length.

    // if(*b_shr == -14)
    //   *b_shr = -13;

    *a_exp = 2*(b_exp + *b_shr) + 30;
}


void vect_s32_clip_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    int32_t* lower_bound,
    int32_t* upper_bound,
    const exponent_t b_exp,
    const exponent_t bound_exp,
    const headroom_t b_hr)
{
    (void) b_hr;
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
    
    *a_exp = b_exp;//minimum b exponent
    
    right_shift_t bound_shr = *a_exp - bound_exp;
    *b_shr = *a_exp - b_exp;

    int32_t lb;
    int32_t ub;

    if(bound_shr < 0){
        int64_t ub64 = ((int64_t)*upper_bound) << (-bound_shr);
        int64_t lb64 = ((int64_t)*lower_bound) << (-bound_shr);

        ub = (ub64 >= VPU_INT32_MAX)? VPU_INT32_MAX : (ub64 <= VPU_INT32_MIN)? VPU_INT32_MIN : (int32_t) ub64;
        lb = (lb64 >= VPU_INT32_MAX)? VPU_INT32_MAX : (lb64 <= VPU_INT32_MIN)? VPU_INT32_MIN : (int32_t) lb64;
    } else {
        // TODO: Should force upper_bound to round downwards to enforce the guarantee that no output can be larger than
        // upper bound?
        ub = *upper_bound >> bound_shr;
        // And lower bound upwards?
        lb = (*lower_bound + ((1<<bound_shr)-1)) >> bound_shr;
    }

    if(ub == VPU_INT32_MIN){
        /* upper bound must be smaller than any element of b, so set everything to that */
        *a_exp = bound_exp;
    } else if(lb == VPU_INT32_MAX){
        /* lower bound must be larger than any element of b, so set everything to that */
        *a_exp = bound_exp;
    }

    *upper_bound = ub;
    *lower_bound = lb;
}


void vect_2vec_prepare(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    right_shift_t* c_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr,
    const headroom_t extra_operand_hr)
{
  const exponent_t b_min_exp = b_exp - b_hr + extra_operand_hr;
  const exponent_t c_min_exp = c_exp - c_hr + extra_operand_hr;
  *a_exp = (b_min_exp > c_min_exp)? b_min_exp : c_min_exp;
  *b_shr = *a_exp - b_exp;
  *c_shr = *a_exp - c_exp;
}
