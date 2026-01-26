// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

.text
.align 4; /* Translation error on this line: unexpected token at position 8. */ 

/*  
float f32_log2(
    const float x);
*/
#define FUNCTION_NAME   f32_log2
#define NSTACKWORDS     (4)

#define x     x10
#define tmp   x11
#define exp   x12
#define _0    x13

FUNCTION_NAME:
  xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */

{ mv a1, a0                  ; addi a0,sp, 4              }
lui t3, %hi(f32_normA)
  addi t3,t3, %lo(f32_normA)
  jalr t3

{ li tmp, 1                  ; li t3, 23                 }
{ li _0, 0                   ; lw exp, 4              (sp)}
  xm.fmake tmp, _0, t3, _0, tmp
  xm.fsub x, x, tmp
  xm.fmake exp, _0, t3, _0, exp
lui t3, %hi(log2_ps)       
  addi t3,t3, %lo(log2_ps)       
{ nop                             ; sw exp, 4              (sp)}
{ mv a1, t3                 ; li a2, 11                  }
lui t3, %hi(f32_power_series)
  addi t3,t3, %lo(f32_power_series)
  jalr t3
{ nop                             ; lw exp, 4              (sp)}
  xm.fadd x, x, exp
  xm.retsp (NSTACKWORDS)*4/* Multiple XAT warnings: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in retsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate", 'RETSP operand may need scaling' */

.L_func_end_unpack:

.global FUNCTION_NAME
.type FUNCTION_NAME,@function


.weak FUNCTION_NAME.callees
.add_to_set FUNCTION_NAME.callees,f32_normA.nstackwords
.add_to_set FUNCTION_NAME.callees,f32_power_series.nstackwords
.max_reduce FUNCTION_NAME.callee_maxstackwords,FUNCTION_NAME.callees,0
.set FUNCTION_NAME.nstackwords,NSTACKWORDS+FUNCTION_NAME.callee_maxstackwords
.global FUNCTION_NAME.nstackwords

.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores /* Translation error on this line: unexpected token at position 29. */ 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers /* Translation error on this line: unexpected token at position 30. */ 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends /* Translation error on this line: unexpected token at position 32. */ 
.size FUNCTION_NAME,.L_func_end_unpack - FUNCTION_NAME

#undef NSTACKWORDS
#undef FUNCTION_NAME



#endif //defined(__VX4B__)



