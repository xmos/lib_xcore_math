// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


#include "../asm_helper.h"

.text

.align 4;
.issue_mode dual
/*  
void f32_unpack(
    int32_t* mantissa,
    exponent_t* exp,
    float input);
*/
#define FUNCTION_NAME   f32_unpack
#define NSTACKWORDS     (0)

#define mant_out    r0
#define exp_out     r1
#define input       r2
#define sign        r3
#define exp         r11

.cc_top FUNCTION_NAME.function,FUNCTION_NAME
FUNCTION_NAME:
    dualentsp NSTACKWORDS
    fsexp sign, exp, input
    fmant input, input

#undef input
#define mant        r2

    // interesting way of subtracting 23 without using registers
    ldaw exp, exp[-6]
{   add exp, exp, 1             ;   bf sign, .L_xuf_lblA        }
{   neg mant, mant              ;                               }
    .L_xuf_lblA:
{                               ;   stw mant, mant_out[0]       }
{                               ;   stw exp, exp_out[0]         }
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


#endif //defined(__XS3A__)
