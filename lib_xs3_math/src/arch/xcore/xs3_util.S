// Copyright 2021 XMOS LIMITED. This Software is subject to the terms of the 
// XMOS Public License: Version 1

#if defined(__XS3A__)
#ifndef XS3_MATH_NO_ASM

#include "asm_helper.h"

.text

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
float xs3_pack_float(
    const int64_t mantissa,
    const exponent_t exp);
*/
#define FUNCTION_NAME   xs3_pack_float
#define NSTACKWORDS     (0)

#define a   r0
#define b   r1
#define c   r2
#define d   r3
#define e   r11

#define mant_hi     a
#define mant_lo     b
#define exp         c

.cc_top FUNCTION_NAME.function,FUNCTION_NAME
FUNCTION_NAME:
    dualentsp NSTACKWORDS

//Extract sign bit from mantissa. Add 23 to exponent

    ldc d, 0
    ecallf d
    lss d, mant_hi, d
    bf d, .L_xpf_lblA
.L_xpf_lblA:
    retsp NSTACKWORDS
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



