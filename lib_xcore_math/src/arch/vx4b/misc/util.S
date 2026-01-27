// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

.text

.align 4; /* Translation error on this line: unexpected token at position 8. */ 
/*  
void f32_unpack(
    int32_t* mantissa,
    exponent_t* exp,
    float input);
*/
#define FUNCTION_NAME   f32_unpack
#define NSTACKWORDS     (0)

#define mant_out    x10
#define exp_out     x11
#define input       x12
#define sign        x13
#define exp         x28

FUNCTION_NAME:
    xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
    xm.fsexp sign, exp, input
    xm.fmant input, input

#undef input
#define mant        x12

    // interesting way of subtracting 23 without using registers
    addi exp, exp, -24
{   addi exp, exp, 1             ;   xm.brff sign, .L_xuf_lblA        }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
{   xm.neg mant, mant              ; nop                               }
    .L_xuf_lblA:
{ nop                               ;   sw mant,0       ( mant_out)}
{ nop                               ;   sw exp,0         ( exp_out)}
    xm.retsp (NSTACKWORDS)*4/* Multiple XAT warnings: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in retsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate", 'RETSP operand may need scaling' */
.L_func_end_unpack:

.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords /* Translation error on this line: unexpected token at position 42. */ 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores /* Translation error on this line: unexpected token at position 29. */ 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers /* Translation error on this line: unexpected token at position 30. */ 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends /* Translation error on this line: unexpected token at position 32. */ 
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

#define mant        x10
#define exp         x11
#define sign        x12
#define zero        x13
#define tmp         x28

.align 4; /* Translation error on this line: unexpected token at position 8. */ 
FUNCTION_NAME:
  
    xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
    srai sign, mant, 31
  { li tmp, 23                 ; li zero, 0                 }  
  { add exp, exp, tmp           ; xm.brff sign, .L_pack_not_neg    }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
    xm.neg mant, mant
  .L_pack_not_neg:
    xm.fmake mant, sign, exp, zero, mant
  { nop                             ; xm.retsp (NSTACKWORDS)*4           }

.L_func_end_pack:

.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords /* Translation error on this line: unexpected token at position 42. */ 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores /* Translation error on this line: unexpected token at position 29. */ 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers /* Translation error on this line: unexpected token at position 30. */ 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends /* Translation error on this line: unexpected token at position 32. */ 
.size FUNCTION_NAME,.L_func_end_pack - FUNCTION_NAME

#undef NSTACKWORDS
#undef FUNCTION_NAME


#endif //defined(__VX4B__)
