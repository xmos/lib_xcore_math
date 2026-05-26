// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

/*
void s32_to_chunk_s32(
    int32_t a[8],
    int32_t b);
*/


#define FUNCTION_NAME   s32_to_chunk_s32

.text
.p2align 4


#define a   a0
#define b   a1

FUNCTION_NAME:
  xm.stdi  b,b, 0(a)
  xm.stdi  b,b, 8(a)
  xm.stdi  b,b, 16(a)
  xm.stdi  b,b, 24(a)
  ret
.L_func_end:


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.resource_const FUNCTION_NAME, "stack_frame_bytes", 0
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME


#endif //defined(__VX4B__)



