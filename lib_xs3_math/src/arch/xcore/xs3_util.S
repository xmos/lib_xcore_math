// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)
#ifndef XS3_MATH_NO_ASM

#include "asm_helper.h"

.text

.align 4;
.issue_mode dual
/*  
void xs3_unpack_float(
    int32_t* mantissa,
    exponent_t* exp,
    float input);
*/
#define FUNCTION_NAME   xs3_unpack_float
#define NSTACKWORDS     (0)

.cc_top FUNCTION_NAME.function,FUNCTION_NAME
FUNCTION_NAME:
    dualentsp NSTACKWORDS
    fsexp r3, r11, r2
    fmant r2, r2
    ldaw r11, r11[-6]
{   add r11, r11, 1             ;   bf r3, .L_xuf_lblA          }
{   neg r2, r2                  ;                               }
    .L_xuf_lblA:
{                               ;   stw r2, r0[0]               }
{                               ;   stw r11, r1[0]              }
    retsp NSTACKWORDS
.L_func_end_unpack:
.cc_bottom FUNCTION_NAME.function

.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME,.L_func_end_unpack - FUNCTION_NAME

#undef NSTACKWORDS
#undef FUNCTION_NAME



/*  
float xs3_float_s32_to_f32(
    const int32_t mantissa,
    const exponent_t exp);
*/
#define FUNCTION_NAME   xs3_float_s32_to_f32
#define NSTACKWORDS     (0)

#define mant        r0
#define exp         r1
#define sign        r2
#define zero        r3
#define tmp         r11

.align 4;
.cc_top FUNCTION_NAME.function,FUNCTION_NAME
FUNCTION_NAME:
  
    dualentsp NSTACKWORDS
    ashr sign, mant, 32
  { ldc tmp, 23                 ; ldc zero, 0                 }  
  { add exp, exp, tmp           ; bf sign, .L_pack_not_neg    }
    neg mant, mant
  .L_pack_not_neg:
    fmake mant, sign, exp, zero, mant
  {                             ; retsp NSTACKWORDS           }

.L_func_end_pack:
.cc_bottom FUNCTION_NAME.function

.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME,.L_func_end_pack - FUNCTION_NAME

#undef NSTACKWORDS
#undef FUNCTION_NAME




#endif //!defined(XS3_MATH_NO_ASM)
#endif //defined(__XS3A__)



