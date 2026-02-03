// Copyright 2021-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// XMOS Public License: Version 1

#if defined(__VX4B__)


#include "../asm_helper.h"

/*  
    void vect_s32_unzip(
        int32_t a[],
        int32_t b[],
        const complex_s32_t c[],
        const unsigned length);
*/


#define NSTACKWORDS     (4)

#define FUNCTION_NAME   vect_s32_unzip



#define a         a0
#define b         a1
#define c         a2
#define len       a3

#define tmpA      s2
#define tmpB      s3

.text
.p2align 2


FUNCTION_NAME:
    xm.entsp (NSTACKWORDS)*4
    xm.stdsp  s3,s2,0

  { addi len, len, -1                   ; xm.brff len, .L_finish                }

  .L_loop_top:
      xm.ldd  tmpA,tmpB, len(c)
    xm.stw tmpA,len                          ( a)
    xm.stw tmpB,len                          ( b)
    { addi len, len, -1                 ; xm.bt len, .L_loop_top                }

  .L_finish:
      xm.lddsp  s3,s2,0
    { nop                               ; xm.retsp (NSTACKWORDS)*4              } 


.L_func_end:


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS; .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;              .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;             .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;           .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME


#endif //defined(__VX4B__)



