
#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "vpu_helper.h"



// right_shift_t xs3_energy_s16_calc_shift(
//     const unsigned length,
//     const headroom_t b_hr)
// {
//     /*
//         b_shr needs to be calculated based on the worst-case result, given the element count and headroom of the
//         vector b[].

//         Worst case occurs when every value is INT16_MIN = -0x8000 = -(2^15).
//         However, we know that we have at least  b_hr bits of headroom on each element, so given b_hr, the worst
//         case is when each value is  INT16_MIN >> b_hr  = -(2^15) * 2^(-b_hr)

//         max_result = (-(2^15) * 2^(-b_hr) >> b_shr)^2 * len
//                    = (-(2^15) * 2^(-b_hr) * 2^(-b_shr))^2 * len
//                    = (-(2^(15-b_hr-b_shr)))^2 * len
//                    = 2^(2*(15-b_hr-b_shr)) * len
//                    = 2^(30 - 2*(b_hr+b_shr)) * len

//         If we round len up to the next greater power of 2, we can solve for b_shr without computing any actual logs

//         len' = 2^(ceil_log2(len))

//         max_result' = 2^(30 - 2*(b_hr+b_shr)) * len'
//                     = 2^(30 - 2*(b_hr+b_shr)) * 2^(ceil_log2(len))
//                     = 2^(30 - 2*(b_hr+b_shr) + ceil_log2(len))

//         The accumulator is 32 bits. The max value we can hit is 2^31

//         2^31 = 2^(30 - 2*(b_hr+b_shr) + ceil_log2(len))
//         31 = 30 - 2*(b_hr+b_shr) + ceil_log2(len)
//         2*(b_hr+b_shr) = ceil_log2(len) - 1
//         2*b_shr = ceil_log2(len) - 1 - 2*b_hr
//         b_shr = (ceil_log2(len) - 1 - 2*b_hr)/2
//         b_shr = (ceil_log2(len) - 1)/2 - b_hr

//         If (ceil_log2(len)-1) happens to be odd, b_shr will be too small. If we add 1 before dividing by 2, then
//             - If ceil_log2(len) is odd, then    ceil_log2(len)/2 == (ceil_log2(len)-1)/2
//             - If ceil_log2(len) is even, then ceil_log2(len)/2 will avoid underestimating b_shr

//     */

//     return ceil_log2(length)/2 - b_hr;
// }



void xs3_energy_s32_calc_params(
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

        len' = 2^(ceil_log2(len))

        max_result' = 2^(32 - 2*(b_hr+b_shr)) * len'
                    = 2^(32 - 2*(b_hr+b_shr)) * 2^(ceil_log2(len))
                    = 2^(32 - 2*(b_hr+b_shr) + ceil_log2(len))

        The accumulator is 40 bits. The max value we can hit is 2^39 - 1. So set the target based on whether 
        allow_saturation is true or not

        2^(38+allow_sat) = 2^(32 - 2*(b_hr+b_shr) + ceil_log2(len))
        38 + allow_sat = 32 - 2*(b_hr+b_shr) + ceil_log2(len)
        0 = 32 - 38 - allow_sat - 2*b_hr - 2*b_shr + ceil_log2(len)
        2*b_shr = -6 - allow_sat - 2*b_hr + ceil_log2(len)
        b_shr = ( ceil_log2(len) - 6 - allow_sat )/2 + b_hr


        If (ceil_log2(len)-7) happens to be odd, b_shr will be too small. If we add 1 before dividing by 2, then
            - If ceil_log2(len)-6 is odd, then    (ceil_log2(len)-6)/2 == (ceil_log2(len)-7)/2
            - If ceil_log2(len)-6 is even, then (ceil_log2(len)-6)/2 will avoid underestimating b_shr
    */


    *b_shr = (((int)ceil_log2(length))-6)/2 - b_hr;

    *b_shr = MAX(*b_shr, -((int)b_hr));

    *a_exp = 2*(b_exp + *b_shr) + 30;
}
