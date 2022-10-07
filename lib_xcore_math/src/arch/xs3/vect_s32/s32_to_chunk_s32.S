// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


#include "../asm_helper.h"

/*  
void s32_to_chunk_s32(
    int32_t a[8],
    int32_t b);
*/


#define NSTACKWORDS     (0)

#define FUNCTION_NAME   s32_to_chunk_s32

.text
.issue_mode single
.align 16

.cc_top FUNCTION_NAME.function,FUNCTION_NAME

#define a   r0
#define b   r1

FUNCTION_NAME:
  entsp NSTACKWORDS
  std b, b, a[0]
  std b, b, a[1]
  std b, b, a[2]
  std b, b, a[3]
  retsp NSTACKWORDS
.L_func_end:
.cc_bottom FUNCTION_NAME.function


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS; .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;              .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;             .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;           .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME


#endif //defined(__XS3A__)



