// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

.text
.align 4;

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
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME,.L_func_end_unpack - FUNCTION_NAME

#undef NSTACKWORDS
#undef FUNCTION_NAME



#endif //defined(__VX4B__)



