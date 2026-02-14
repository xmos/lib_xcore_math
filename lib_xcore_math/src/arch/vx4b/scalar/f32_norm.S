// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

.text
.p2align 2

/*  
float f32_normA(
    exponent_t* exp,
    const float x);
*/
#define FUNCTION_NAME   f32_normA
#define NSTACKWORDS     (0)


FUNCTION_NAME:
  xm.entsp (NSTACKWORDS)*4
  xm.fsexp a2, a3, a1
  xm.fmant a1, a1
{ addi a3, a3, 1                        ; li t3, 0                              }
{ addi a0, t3, -1                       ; sw a3,0               ( a0)           }
  xm.fmake a0, a2, a0, t3, a1
  xm.retsp (NSTACKWORDS)*4

.L_func_end_unpack:

.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
.size FUNCTION_NAME,.L_func_end_unpack - FUNCTION_NAME

#undef NSTACKWORDS
#undef FUNCTION_NAME



#endif //defined(__VX4B__)



