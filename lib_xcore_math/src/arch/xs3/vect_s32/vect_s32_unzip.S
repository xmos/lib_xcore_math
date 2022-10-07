// Copyright 2021-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// XMOS Public License: Version 1

#if defined(__XS3A__)


#include "../asm_helper.h"

/*  
    void vect_s32_unzip(
        int32_t a[],
        int32_t b[],
        const complex_s32_t c[],
        const unsigned length);
*/


#define NSTACKWORDS     (2)

#define FUNCTION_NAME   vect_s32_unzip



#define a         r0
#define b         r1
#define c         r2
#define len       r3

#define tmpA      r4
#define tmpB      r5

.text
.issue_mode dual
.align 4

.cc_top FUNCTION_NAME.function,FUNCTION_NAME

FUNCTION_NAME:
    dualentsp NSTACKWORDS
    std r4, r5, sp[0]

  { sub len, len, 1                           ; bf len, .L_finish                         }

  .L_loop_top:
      ldd tmpB, tmpA, c[len]
    {                                           ; stw tmpA, a[len]                          }
    {                                           ; stw tmpB, b[len]                          }
    { sub len, len, 1                           ; bt len, .L_loop_top                       }

  .L_finish:
      ldd r4, r5, sp[0]
    {                                           ; retsp NSTACKWORDS                         } 


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



