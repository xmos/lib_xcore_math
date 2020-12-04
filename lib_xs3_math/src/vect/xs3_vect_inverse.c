
#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "vpu_helper.h"
#include "xs3_vpu_scalar_ops.h"


int32_t xs3_inverse_s32(
    exponent_t* a_exp,
    const int32_t b)
{
    const headroom_t b_hr = HR_S32(b);

    const unsigned scale = 60 - b_hr;

    const int64_t dividend = 1LL << scale;

    *a_exp = -scale;

    return dividend / b;

}

static int16_t min_abs_s16(
    const int16_t b[], 
    const unsigned length)
{
    int16_t m = INT16_MAX;

    for(int i = 0; i < length; i++){
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

    for(int i = 0; i < length; i++){
        int32_t tmp = vlmul32(b[i], vsign32(b[i]));
        m = MIN(m, tmp);
    }

    return m;
}



void xs3_vect_s16_inverse_calc_params(
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

void xs3_vect_s32_inverse_calc_params(
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




WEAK_FUNC
void xs3_vect_s16_inverse(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const unsigned scale)
{
    const int32_t dividend = 1 << scale;
    for(int k = 0; k < length; k++){
        a[k] = (dividend / b[k]);
    }
}




WEAK_FUNC
headroom_t xs3_vect_s32_inverse(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const unsigned scale)
{

    const int64_t d = (0x1LL << scale);

    for(int k = 0; k < length; k++){
        a[k] = d / b[k];
        
        // printf("0x%016llX / %ld = %ld\n", (uint64_t) d, b[k], a[k]);
    }

    return xs3_vect_s32_headroom(a, length);
}
