
#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "../../vect/vpu_helper.h"

static int64_t mul_s32(int32_t b, int32_t c, int b_shr, int c_shr)
{
    int64_t bp = b;
    int64_t cp = c;

    bp = ASHR(32)(bp, b_shr);
    cp = ASHR(32)(cp, c_shr);

    // bp = (b_shr >= 0)? bp >> b_shr : bp << (-b_shr);
    // cp = (c_shr >= 0)? cp >> c_shr : cp << (-c_shr);

    // bp = (bp >= VPU_INT32_MAX)? VPU_INT32_MAX : (bp <= VPU_INT32_MIN)? VPU_INT32_MIN : bp;
    // bp = ( (b_shr > 0) && (b < 0) && (-(1<<b_shr)) < b)? -1 : bp;
    // assert(b >= 0 || bp < 0);
    
    // cp = (cp >= VPU_INT32_MAX)? VPU_INT32_MAX : (cp <= VPU_INT32_MIN)? VPU_INT32_MIN : cp;
    // cp = ( (c_shr > 0) && (c < 0) && (-(1<<c_shr)) < c)? -1 : cp;
    // assert(c >= 0 || cp < 0);

    // int64_t a = (bp * cp + (1<<29)) >> (30);
    
    int64_t a = ROUND_SHR(bp * cp, 30);

    // a = (a >= VPU_INT32_MAX)? VPU_INT32_MAX : (a <= VPU_INT32_MIN)? VPU_INT32_MIN : a;

    return (int32_t) a;
}




int64_t xs3_vect_s16_dot(
    const int16_t b[],
    const int16_t c[],
    const unsigned length)
{
    int64_t acc = 0;
    for(int k = 0; k < length; k++){
        acc += ((int32_t)b[k]) * c[k];
    }

    return SAT(32)(acc);
}



int64_t xs3_vect_s32_dot(
    const int32_t b[],
    const int32_t c[],
    const unsigned length,
    const int b_shr,
    const int c_shr)
{
    int64_t acc = 0;
    for(int k = 0; k < length; k++){
        acc += mul_s32(b[k], c[k], b_shr, c_shr);
    }

    return acc;
}