// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

.text
.align 4; /* Translation error on this line: unexpected token at position 8. */ 

/*  
q2_30 q30_exp_small(
    const q2_30 x);
*/
#define FUNCTION_NAME   q30_exp_small
#define NSTACKWORDS     (8)

#define x       x10
#define coef    x11
#define acc_hi  x12
#define acc_lo  x13
#define tmp     x18
#define pow     x19
#define _30     x20
#define _0      x21
#define sign    x24  // 1 means positive, 0 negative

.L_ps_pos_q30: 
  .word  0x40000000,  0x20000000,  0x0AAAAAAB, 0x02AAAAAB,  0x00888889, 0x0016C16C,  0x00034034, 0x00006807,  0x00000B8F
.L_ps_neg_q30: 
  .word -0x40000000,  0x20000000, -0x0AAAAAAB, 0x02AAAAAB, -0x00888889, 0x0016C16C, -0x00034034, 0x00006807, -0x00000B8F

FUNCTION_NAME:
  xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
  xm.stdsp  s3,s2,0
  xm.stdsp  s5,s4,8

{ xm.ldap t3, .L_ps_pos_q30     ; sw s8, 16              (sp)}
{ mv coef, t3               ; li _30, 30                 }
{ li _0, 0                   ; li acc_hi, 0               }
{ xm.slt sign, _0, x             ; nop                             } 
{ li acc_lo, 0               ; xm.bt sign, .L_pos_x           }

.L_neg_x:
lui t3, %hi(.L_ps_neg_q30)
  addi t3,t3, %lo(.L_ps_neg_q30)
{ xm.neg x, x                    ; mv coef, t3               }
.L_pos_x:

{ nop                             ; lw tmp,0            ( coef)}

  xm.maccs acc_hi, acc_lo, tmp, tmp
  xm.maccs acc_hi, acc_lo, tmp, x
  xm.lmul t3, pow, x, x, _0, _0
  xm.lextract pow, t3, pow, _30, 32
{ nop                             ; lw tmp,4            ( coef)}
  xm.maccs acc_hi, acc_lo, tmp, pow 
  xm.lmul t3, pow, pow, x, _0, _0
  xm.lextract pow, t3, pow, _30, 32
{ nop                             ; lw tmp,8            ( coef)}
  xm.maccs acc_hi, acc_lo, tmp, pow 
  xm.lmul t3, pow, pow, x, _0, _0
  xm.lextract pow, t3, pow, _30, 32
{ nop                             ; lw tmp,12            ( coef)}
  xm.maccs acc_hi, acc_lo, tmp, pow 
  xm.lmul t3, pow, pow, x, _0, _0
  xm.lextract pow, t3, pow, _30, 32
{ nop                             ; lw tmp,16            ( coef)}
  xm.maccs acc_hi, acc_lo, tmp, pow 
  xm.lmul t3, pow, pow, x, _0, _0
  xm.lextract pow, t3, pow, _30, 32
{ nop                             ; lw tmp,20            ( coef)}
  xm.maccs acc_hi, acc_lo, tmp, pow 
  xm.lmul t3, pow, pow, x, _0, _0
  xm.lextract pow, t3, pow, _30, 32
{ nop                             ; lw tmp,24            ( coef)}
  xm.maccs acc_hi, acc_lo, tmp, pow 
  xm.lmul t3, pow, pow, x, _0, _0
  xm.lextract pow, t3, pow, _30, 32
{ nop                             ; lw tmp,28            ( coef)}
  xm.maccs acc_hi, acc_lo, tmp, pow 
  xm.lmul t3, pow, pow, x, _0, _0
  xm.lextract pow, t3, pow, _30, 32
{ nop                             ; lw tmp,32            ( coef)}
  xm.maccs acc_hi, acc_lo, tmp, pow 
  
  // result
  xm.lextract a0, acc_hi, acc_lo, _30, 32

.L_finish:
  lw s8, 16(sp)
  xm.lddsp  s3,s2,0
  xm.lddsp  s5,s4,8
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



