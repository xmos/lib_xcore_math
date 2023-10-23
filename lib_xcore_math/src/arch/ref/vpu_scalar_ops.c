// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdio.h>
#include <math.h>

#include "vpu_helper.h"


#define SAT40(X)   ( ((X) > VPU_INT40_MAX)? VPU_INT40_MAX           \
                 : ( ((X) < VPU_INT40_MIN)? VPU_INT40_MIN : (X) ) )



#define ROUND_SHR8( X, SHIFT)    ((int8_t) (((SHIFT)==0)? (X) : ((((int16_t)(X))+(1<<((SHIFT)-1))) >> (SHIFT))))
#define ROUND_SHR16(X, SHIFT)    ((int16_t)(((SHIFT)==0)? (X) : ((((int32_t)(X))+(1<<((SHIFT)-1))) >> (SHIFT))))
#define ROUND_SHR32(X, SHIFT)    ((int32_t)(((SHIFT)==0)? (X) : ((((int64_t)(X))+(1<<((SHIFT)-1))) >> (SHIFT))))
#define ROUND_SHR64(X, SHIFT)    ((int64_t)(((SHIFT)==0)? (X) : ((((int64_t)(X))+(1<<((SHIFT)-1))) >> (SHIFT))))

static const int8_t  one_q6  = 0x40;
static const int16_t one_q14 = 0x4000;
static const int32_t one_q30 = 0x40000000;

static const int8_t  neg_one_q6  = -0x40;
static const int16_t neg_one_q14 = -0x4000;
static const int32_t neg_one_q30 = -0x40000000;



int8_t vladd8(
    const int8_t a, 
    const int8_t b)
{
    int16_t sum = ((int16_t)a) + b;
    return SAT(8)(sum);
}


int8_t vlsub8(
    const int8_t a, 
    const int8_t b)
{
    int16_t diff = ((int16_t)a) - b;
    return SAT(8)(diff);
}


int8_t vlashr8(
    const int8_t x,
    const right_shift_t shr)
{
    if(shr >= 8)                        return (x >= 0)? 0 : -1;
    else if((shr <= -8) && (x != 0) )   return (x >= 0)? VPU_INT8_MAX : VPU_INT8_MIN;
    else if(shr < 0)                    return SAT(8)(((int32_t)x) << (-shr));
    else                                return SAT(8)(x >> shr);
}


int8_t vpos8(
    const int8_t x)
{
    return (x >= 0)? x : 0;
}


int8_t vsign8(
    const int8_t x)
{
    return (x >= 0)? one_q6 : neg_one_q6;
}


unsigned vdepth1_8(
    const int8_t x)
{
    return (x >= 0)? 0 : 1;
}


int8_t vlmul8(
    const int8_t x,
    const int8_t y)
{
    int32_t p = ((int32_t)x)*y;
    p = ROUND_SHR32(p, 6);
    return SAT(8)(p);
}


vpu_int8_acc_t vlmacc8(
    const vpu_int8_acc_t acc,
    const int8_t x,
    const int8_t y)
{
    int64_t s = ((int64_t)acc) + (((int32_t)x)*y);
    return SAT(32)(s);
}


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


int8_t vlsat8(
    const vpu_int8_acc_t acc,
    const unsigned sat)
{
    vpu_int8_acc_t s = acc;

    if(sat >= 32) return (acc >= 0)? 0 : -1;

    if(sat > 0)
        s = ((acc >> (sat-1)) + 1) >> 1;

    return SAT(8)(s);
}





int16_t vladd16(
    const int16_t a, 
    const int16_t b)
{
    int32_t sum = ((int32_t)a) + b;
    return SAT(16)(sum);
}


int16_t vlsub16(
    const int16_t a, 
    const int16_t b)
{
    int32_t diff = ((int32_t)a) - b;
    return SAT(16)(diff);
}


int16_t vlashr16(
    const int16_t x,
    const right_shift_t shr)
{
    if(shr >= 16)                       return (x >= 0)? 0 : -1;
    else if((shr <= -16) && (x != 0) )  return (x >= 0)? VPU_INT16_MAX : VPU_INT16_MIN;
    else if(shr < 0)                    return SAT(16)(((int32_t)x) << (-shr));
    else                                return SAT(16)(x >> shr);
}


int16_t vpos16(
    const int16_t x)
{
    return (x >= 0)? x : 0;
}


int16_t vsign16(
    const int16_t x)
{
    return (x >= 0)? one_q14 : neg_one_q14;
}


unsigned vdepth1_16(
    const int16_t x)
{
    return (x >= 0)? 0 : 1;
}


int8_t vdepth8_16(
    const int16_t x)
{
    int16_t s = ROUND_SHR16(x, 8);
    return SAT(8)(s);
}


int16_t vlmul16(
    const int16_t x,
    const int16_t y)
{
    int32_t p = ((int32_t)x)*y;
    p = ROUND_SHR32(p, 14);
    return SAT(16)(p);
}


vpu_int16_acc_t vlmacc16(
    const vpu_int16_acc_t acc,
    const int16_t x,
    const int16_t y)
{
    int64_t s = ((int64_t)acc) + (((int32_t)x)*y);
    vpu_int16_acc_t p = SAT(32)(s);
    return p;
}


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


int16_t vlsat16(
    const vpu_int16_acc_t acc,
    const unsigned sat)
{
    vpu_int16_acc_t s = acc;

    if(sat >= 32) return (acc >= 0)? 0 : -1;

    if(sat > 0)
        s = ((acc >> (sat-1)) + 1) >> 1;

    return SAT(16)(s);
}

vpu_int16_acc_t vadddr16(
    const vpu_int16_acc_t acc[VPU_INT16_ACC_PERIOD])
{
    int64_t s = 0;

    for(int k = 0; k < VPU_INT16_ACC_PERIOD; k++)
        s += acc[k];

    return SAT(32)(s);
}







int32_t vladd32(
    const int32_t a, 
    const int32_t b)
{
    int64_t sum = ((int64_t)a) + b;
    return SAT(32)(sum);
}


int32_t vlsub32(
    const int32_t a, 
    const int32_t b)
{
    int64_t diff = ((int64_t)a) - b;
    return SAT(32)(diff);
}


int32_t vlashr32(
    const int32_t x,
    const right_shift_t shr)
{
    if(shr >= 32)                       return (x >= 0)? 0 : -1;
    else if((shr <= -32) && (x != 0) )  return (x >= 0)? VPU_INT32_MAX : VPU_INT32_MIN;
    else if(shr < 0)                    return SAT(32)(((int64_t)x) << (-shr));
    else                                return SAT(32)(x >> shr);
}


int32_t vpos32(
    const int32_t x)
{
    return (x >= 0)? x : 0;
}


int32_t vsign32(
    const int32_t x)
{
    return (x >= 0)? one_q30 : neg_one_q30;
}


unsigned vdepth1_32(
    const int32_t x)
{
    return (x >= 0)? 0 : 1;
}


int8_t vdepth8_32(
    const int32_t x)
{
    const int32_t p = ROUND_SHR32(x, 24);
    return SAT(8)(p);
}


int16_t vdepth16_32(
    const int32_t x)
{
    const int32_t p = ROUND_SHR32(x, 16);
    return SAT(16)(p);
}


int32_t vlmul32(
    const int32_t x,
    const int32_t y)
{
    int64_t p = ((int64_t)x)*y;
    p = ROUND_SHR64(p, 30);
    return SAT(32)(p);
}


vpu_int32_acc_t vlmacc32(
    const vpu_int32_acc_t acc,
    const int32_t x,
    const int32_t y)
{
    int64_t p = (((int64_t)x)*y);
    p = ROUND_SHR64(p, 30);
    
    vpu_int32_acc_t s = acc;
    s += p;
    
    return SAT40(s);
}


vpu_int32_acc_t vlmaccr32(
    const vpu_int32_acc_t acc,
    const int32_t x[VPU_INT32_EPV],
    const int32_t y[VPU_INT32_EPV])
{
    int64_t s = acc;
    for(int i = 0; i < VPU_INT32_EPV; i++){
        
        int64_t p = (((int64_t)x[i])*y[i]);
        p = ROUND_SHR64(p, 30);
        s += p;
    }

    return SAT40(s);
}


int32_t vlsat32(
    const vpu_int32_acc_t acc,
    const unsigned sat)
{
    vpu_int32_acc_t s = acc;

    if(sat >= 39) return (acc >= 0)? 0 : -1;

    if(sat > 0)
        s = ((acc >> (sat-1)) + 1) >> 1;

    return SAT(32)(s);
}


int32_t vcmr32(
    const complex_s32_t vD,
    const complex_s32_t vC)
{
    int64_t a = ((int64_t)vD.re) * vC.re;
    int64_t b = ((int64_t)vD.im) * vC.im;

    a = ROUND_SHR64(a, 30);
    b = ROUND_SHR64(b, 30);

    a = a - b;

    return SAT(32)(a);
}


int32_t vcmi32(
    const complex_s32_t vD,
    const complex_s32_t vC)
{
    int64_t a = ((int64_t)vD.re) * vC.im;
    int64_t b = ((int64_t)vD.im) * vC.re;

    a = ROUND_SHR64(a, 30);
    b = ROUND_SHR64(b, 30);

    a = a + b;

    return SAT(32)(a);
}


int32_t vcmcr32(
    const complex_s32_t vD,
    const complex_s32_t vC)
{
    int64_t a = ((int64_t)vD.re) * vC.re;
    int64_t b = ((int64_t)vD.im) * vC.im;

    a = ROUND_SHR64(a, 30);
    b = ROUND_SHR64(b, 30);

    a = b + a;

    return SAT(32)(a);
}


int32_t vcmci32(
    const complex_s32_t vD,
    const complex_s32_t vC)
{
    int64_t a = ((int64_t)vD.re) * vC.im;
    int64_t b = ((int64_t)vD.im) * vC.re;

    a = ROUND_SHR64(a, 30);
    b = ROUND_SHR64(b, 30);

    a = b - a;

    return SAT(32)(a);
}


float fmacc(
  const float acc,
  const float b,
  const float c)
{
  double bc = ((double)b) * ((double)c);
  double res = ((double)acc) + bc;
  float res2 = (float)res;
  return res2;
}
