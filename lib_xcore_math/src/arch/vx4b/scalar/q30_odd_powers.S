// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

.text
.align 4; /* Translation error on this line: unexpected token at position 8. */ 

/*  
void s32_odd_powers(
    int32_t a[],
    const int32_t b,
    const unsigned count,
    const right_shift_t shr);
*/
#define FUNCTION_NAME   s32_odd_powers
#define NSTACKWORDS     (4)

#define a         x10
#define b         x11
#define len       x12
#define shr       x13

#define acc_lo    x18
#define acc_hi    x19

#define b_sqr     x28



FUNCTION_NAME:

  xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
  xm.stdsp  s3,s2,8

{ nop                             ; addi len, len, -1             }
{ li acc_lo, 0               ; li acc_hi, 0               }
  xm.maccs acc_hi, acc_lo, b, b
  xm.lextract b_sqr, acc_hi, acc_lo, shr, 32

.L_loop_top:
  { addi len, len, -1             ; sw b,0                 ( a)}
  { li acc_lo, 0               ; li acc_hi, 0               }
    xm.maccs acc_hi, acc_lo, b, b_sqr
    xm.lextract b, acc_hi, acc_lo, shr, 32
  { addi a, a, 4                 ; xm.bt len, .L_loop_top         }

  sw b,0( a)

  xm.lddsp  s3,s2,8
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



#endif //defined(__VX4B__)



