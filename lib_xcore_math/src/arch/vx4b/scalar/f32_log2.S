// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

.text
.align 4;

/*  
float f32_log2(
    const float x);
*/
#define FUNCTION_NAME   f32_log2
#define NSTACKWORDS     (4)

#define x     a0
#define tmp   a1
#define exp   a2
#define _0    a3

FUNCTION_NAME:
  xm.entsp (NSTACKWORDS)*4
{ mv a1, a0                             ; addi a0,sp, 4                         }
  la t3, f32_normA
  jalr t3

{ li tmp, 1                             ; li t3, 23                             }
{ li _0, 0                              ; lw exp, 4              (sp)           }
  xm.fmake tmp, _0, t3, _0, tmp
  xm.fsub x, x, tmp
  xm.fmake exp, _0, t3, _0, exp
la t3, log2_ps       
{ nop                                   ; sw exp, 4              (sp)           }
{ mv a1, t3                             ; li a2, 11                             }
la t3, f32_power_series
  jalr t3
{ nop                                   ; lw exp, 4              (sp)           }
  xm.fadd x, x, exp
  xm.retsp (NSTACKWORDS)*4
.L_func_end_unpack:

.global FUNCTION_NAME
.type FUNCTION_NAME,@function


.weak FUNCTION_NAME.callees
.add_to_set FUNCTION_NAME.callees,f32_normA.nstackwords
.add_to_set FUNCTION_NAME.callees,f32_power_series.nstackwords
.max_reduce FUNCTION_NAME.callee_maxstackwords,FUNCTION_NAME.callees,0
.set FUNCTION_NAME.nstackwords,NSTACKWORDS+FUNCTION_NAME.callee_maxstackwords
.global FUNCTION_NAME.nstackwords

.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME,.L_func_end_unpack - FUNCTION_NAME

#undef NSTACKWORDS
#undef FUNCTION_NAME



#endif //defined(__VX4B__)



