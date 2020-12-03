
#include <stdio.h>

#include "vpu_helper.h"



WEAK_FUNC
int8_t vladd8(
    const int8_t a, 
    const int8_t b)
{
    int16_t sum = ((int16_t)a) + b;
    return SAT(8)(sum);
}

WEAK_FUNC
int8_t vlsub8(
    const int8_t a, 
    const int8_t b)
{
    int16_t diff = ((int16_t)a) - b;
    return SAT(8)(diff);
}

WEAK_FUNC
int8_t vlashr8(
    const int8_t x,
    const right_shift_t shr)
{
    if((shr <= -8) && (x != 0) )    return (x >= 0)? VPU_INT8_MAX : VPU_INT8_MIN;
    else if(shr < 0)                return SAT(8)(((int32_t)x) << (-shr));
    else                            return x >> shr;
}

WEAK_FUNC
int8_t vpos8(
    const int8_t x)
{
    return (x >= 0)? x : 0;
}

WEAK_FUNC
int8_t vsign8(
    const int8_t x)
{
    return (x >= 0)? 0x40 : -0x40;
}

WEAK_FUNC
unsigned vdepth1_8(
    const int8_t x)
{
    return (x >= 0)? 0 : 1;
}

WEAK_FUNC
int8_t vlmul8(
    const int8_t x,
    const int8_t y)
{
    int32_t p = ((int32_t)x)*y;
    p = ((p >> 5) + 1) >> 1;
    return SAT(8)(p);
}

WEAK_FUNC
vpu_int8_acc_t vlmacc8(
    const vpu_int8_acc_t acc,
    const int8_t x,
    const int8_t y)
{
    int64_t s = ((int64_t)acc) + (((int32_t)x)*y);
    return SAT(32)(s);
}

WEAK_FUNC
vpu_int8_acc_t vlmaccr8(
    const vpu_int8_acc_t acc,
    const int8_t x[VPU_INT8_EPV],
    const int8_t y[VPU_INT8_EPV])
{
    int64_t s = acc;
    for(int i = 0; i < VPU_INT8_EPV; i++){
        s += (((int32_t)x[i])*y[i]);
        s = SAT(32)(s);
    }
    
    return SAT(32)(s);
}

WEAK_FUNC
int8_t vlsat8(
    const vpu_int8_acc_t acc,
    const unsigned sat)
{
    vpu_int8_acc_t s = acc;

    if(sat > 0)
        s = ((acc >> (sat-1)) + 1) >> 1;

    return SAT(8)(s);
}




WEAK_FUNC
int16_t vladd16(
    const int16_t a, 
    const int16_t b)
{
    int32_t sum = ((int32_t)a) + b;
    return SAT(16)(sum);
}

WEAK_FUNC
int16_t vlsub16(
    const int16_t a, 
    const int16_t b)
{
    int32_t diff = ((int32_t)a) - b;
    return SAT(16)(diff);
}

WEAK_FUNC
int16_t vlashr16(
    const int16_t x,
    const right_shift_t shr)
{
    if((shr <= -16) && (x != 0) )   return (x >= 0)? VPU_INT16_MAX : VPU_INT16_MIN;
    else if(shr < 0)                return SAT(16)(((int32_t)x) << (-shr));
    else                            return x >> shr;
}

WEAK_FUNC
int16_t vpos16(
    const int16_t x)
{
    return (x >= 0)? x : 0;
}

WEAK_FUNC
int16_t vsign16(
    const int16_t x)
{
    return (x >= 0)? 0x4000 : -0x4000;
}

WEAK_FUNC
unsigned vdepth1_16(
    const int16_t x)
{
    return (x >= 0)? 0 : 1;
}

WEAK_FUNC
int8_t vdepth8_16(
    const int16_t x)
{
    int32_t s = (((int32_t)x) + 0x80);

    s >>= 8;

    return SAT(8)(s);
}

WEAK_FUNC
int16_t vlmul16(
    const int16_t x,
    const int16_t y)
{
    int32_t p = ((int32_t)x)*y;
    p = ((p >> 13) + 1) >> 1;
    return SAT(16)(p);
}

WEAK_FUNC
vpu_int16_acc_t vlmacc16(
    const vpu_int16_acc_t acc,
    const int16_t x,
    const int16_t y)
{
    int64_t s = ((int64_t)acc) + (((int32_t)x)*y);
    vpu_int16_acc_t p = SAT(32)(s);
    return p;
}

WEAK_FUNC
vpu_int16_acc_t vlmaccr16(
    const vpu_int16_acc_t acc,
    const int16_t x[VPU_INT16_EPV],
    const int16_t y[VPU_INT16_EPV])
{
    int64_t s = acc;
    for(int i = 0; i < VPU_INT16_EPV; i++){
        s += (((int32_t)x[i])*y[i]);
    }
    
    return SAT(32)(s);
}

WEAK_FUNC
int16_t vlsat16(
    const vpu_int16_acc_t acc,
    const unsigned sat)
{
    vpu_int16_acc_t s = acc;

    if(sat > 0)
        s = ((acc >> (sat-1)) + 1) >> 1;

    return SAT(16)(s);
}






WEAK_FUNC
int32_t vladd32(
    const int32_t a, 
    const int32_t b)
{
    int64_t sum = ((int64_t)a) + b;
    return SAT(32)(sum);
}

WEAK_FUNC
int32_t vlsub32(
    const int32_t a, 
    const int32_t b)
{
    int64_t diff = ((int64_t)a) - b;
    return SAT(32)(diff);
}

WEAK_FUNC
int32_t vlashr32(
    const int32_t x,
    const right_shift_t shr)
{
    if((shr <= -32) && (x != 0) )   return (x >= 0)? VPU_INT32_MAX : VPU_INT32_MIN;
    else if(shr < 0)                return SAT(32)(((int64_t)x) << (-shr));
    else                            return x >> shr;
}

WEAK_FUNC
int32_t vpos32(
    const int32_t x)
{
    return (x >= 0)? x : 0;
}

WEAK_FUNC
int32_t vsign32(
    const int32_t x)
{
    return (x >= 0)? 0x40000000 : -0x40000000;
}

WEAK_FUNC
unsigned vdepth1_32(
    const int32_t x)
{
    return (x >= 0)? 0 : 1;
}

WEAK_FUNC
int8_t vdepth8_32(
    const int32_t x)
{
    return SAT(8)( (((int64_t)x)+(1<<23)) >> 24 );
}

WEAK_FUNC
int16_t vdepth16_32(
    const int32_t x)
{
    return SAT(16)( (((int64_t)x)+(1<<15)) >> 16 );
}

WEAK_FUNC
int32_t vlmul32(
    const int32_t x,
    const int32_t y)
{
    int64_t p = ((int64_t)x)*y;
    p = ((p >> 29) + 1) >> 1;
    return SAT(32)(p);
}

WEAK_FUNC
vpu_int32_acc_t vlmacc32(
    const vpu_int32_acc_t acc,
    const int32_t x,
    const int32_t y)
{
    int64_t p = (((int64_t)x)*y);
    p = ((p >> 29) + 1) >> 1;
    
    vpu_int32_acc_t s = acc;
    s += p;

    if(s >= VPU_INT40_MAX)
        return VPU_INT40_MAX;
    if(s <= VPU_INT40_MIN)
        return VPU_INT40_MIN;
    
    return s;
}

WEAK_FUNC
vpu_int32_acc_t vlmaccr32(
    const vpu_int32_acc_t acc,
    const int32_t x[VPU_INT32_EPV],
    const int32_t y[VPU_INT32_EPV])
{
    int64_t s = acc;
    for(int i = 0; i < VPU_INT32_EPV; i++){
        
        int64_t p = (((int64_t)x[i])*y[i]);
        p = ((p >> 29) + 1) >> 1;
        s += p;

    }

    if(s >= VPU_INT40_MAX)
        s = VPU_INT40_MAX;
    else if(s <= VPU_INT40_MIN)
        s = VPU_INT40_MIN;
    
    return s;
}

WEAK_FUNC
int32_t vlsat32(
    const vpu_int32_acc_t acc,
    const unsigned sat)
{
    vpu_int32_acc_t s = acc;

    if(sat > 0)
        s = ((acc >> (sat-1)) + 1) >> 1;

    return SAT(32)(s);
}

WEAK_FUNC
int32_t vcmr32(
    const complex_s32_t vD,
    const complex_s32_t vC)
{
    int64_t a = ((int64_t)vD.re) * vC.re;
    int64_t b = ((int64_t)vD.im) * vC.im;

    a = ((a >> 29) + 1) >> 1;
    b = ((b >> 29) + 1) >> 1;

    a = a - b;

    return SAT(32)(a);
}

WEAK_FUNC
int32_t vcmi32(
    const complex_s32_t vD,
    const complex_s32_t vC)
{
    int64_t a = ((int64_t)vD.re) * vC.im;
    int64_t b = ((int64_t)vD.im) * vC.re;

    a = ((a >> 29) + 1) >> 1;
    b = ((b >> 29) + 1) >> 1;

    a = a + b;

    return SAT(32)(a);
}

WEAK_FUNC
int32_t vcmcr32(
    const complex_s32_t vD,
    const complex_s32_t vC)
{
    int64_t a = ((int64_t)vD.re) * vC.re;
    int64_t b = ((int64_t)vD.im) * vC.im;

    a = ((a >> 29) + 1) >> 1;
    b = ((b >> 29) + 1) >> 1;

    a = b + a;

    return SAT(32)(a);
}

WEAK_FUNC
int32_t vcmci32(
    const complex_s32_t vD,
    const complex_s32_t vC)
{
    int64_t a = ((int64_t)vD.re) * vC.im;
    int64_t b = ((int64_t)vD.im) * vC.re;

    a = ((a >> 29) + 1) >> 1;
    b = ((b >> 29) + 1) >> 1;

    a = b - a;

    return SAT(32)(a);
}