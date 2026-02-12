// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

.text

.p2align 2
/*  
void f32_unpack(
    int32_t* mantissa,
    exponent_t* exp,
    float input);
*/
#define FUNCTION_NAME   f32_unpack
#define NSTACKWORDS     (0)

#define mant_out    a0
#define exp_out     a1
#define input       a2
#define sign        a3
#define exp         t3

FUNCTION_NAME:
    xm.entsp (NSTACKWORDS)*4
    xm.fsexp sign, exp, input
    xm.fmant input, input

#undef input
#define mant        a2

    // interesting way of subtracting 23 without using registers
    addi exp, exp, -24
{ addi exp, exp, 1                      ; xm.brff sign, .L_xuf_lblA             }
{ xm.neg mant, mant                     ; nop                                   }
    .L_xuf_lblA:
{ nop                                   ; sw mant,0       ( mant_out)           }
{ nop                                   ; sw exp,0         ( exp_out)           }
    xm.retsp (NSTACKWORDS)*4
.L_func_end_unpack:

.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME,.L_func_end_unpack - FUNCTION_NAME

#undef NSTACKWORDS
#undef FUNCTION_NAME
#undef mant_out
#undef exp_out
#undef mant
#undef exp
#undef sign



/*  
float s32_to_f32(
    const int32_t mantissa,
    const exponent_t exp);
*/
#define FUNCTION_NAME   s32_to_f32
#define NSTACKWORDS     (0)

#define mant        a0
#define exp         a1
#define sign        a2
#define zero        a3
#define tmp         t3

.p2align 2
FUNCTION_NAME:
  
    xm.entsp (NSTACKWORDS)*4
    srai sign, mant, 31
  { li tmp, 23                          ; li zero, 0                            }  
  { add exp, exp, tmp                   ; xm.brff sign, .L_pack_not_neg         }
    xm.neg mant, mant
  .L_pack_not_neg:
    xm.fmake mant, sign, exp, zero, mant
  { nop                                 ; xm.retsp (NSTACKWORDS)*4              }

.L_func_end_pack:

.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME,.L_func_end_pack - FUNCTION_NAME

#undef NSTACKWORDS
#undef FUNCTION_NAME


#endif //defined(__VX4B__)
