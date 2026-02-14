// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

.text
.p2align 2

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


.resource_const FUNCTION_NAME, "stack_frame_bytes", ( ((NSTACKWORDS)))*4
.resource_list_add   FUNCTION_NAME, "callees", f32_normA
.resource_list_add   FUNCTION_NAME, "callees", f32_power_series
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
.size FUNCTION_NAME,.L_func_end_unpack - FUNCTION_NAME

#undef NSTACKWORDS
#undef FUNCTION_NAME



#endif //defined(__VX4B__)



