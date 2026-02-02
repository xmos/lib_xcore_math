// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

/*  
void s32_to_chunk_s32(
    int32_t a[8],
    int32_t b);
*/


#define NSTACKWORDS     (0)

#define FUNCTION_NAME   s32_to_chunk_s32

.text
.p2align 4


#define a   a0
#define b   a1

FUNCTION_NAME:
  xm.entsp (NSTACKWORDS)*4
  xm.stdi  b,b, 0(a)
  xm.stdi  b,b, 8(a)
  xm.stdi  b,b, 16(a)
  xm.stdi  b,b, 24(a)
  xm.retsp (NSTACKWORDS)*4
.L_func_end:


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS; .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;              .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;             .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;           .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME


#endif //defined(__VX4B__)



