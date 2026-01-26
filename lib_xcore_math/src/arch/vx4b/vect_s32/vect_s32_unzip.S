// Copyright 2021-2022 XMOS LIMITED.
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


#define NSTACKWORDS     (4+4)

#define FUNCTION_NAME   vect_s32_unzip



#define a         x10
#define b         x11
#define c         x12
#define len       x13

#define tmpA      x18
#define tmpB      x19

.text
.p2align 2


FUNCTION_NAME:
    xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
    xm.stdsp  s3,s2,0

  { addi len, len, -1                           ; xm.brff len, .L_finish                         }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */

  .L_loop_top:
      xm.ldd  tmpA,tmpB, len(c)
    xm.stw tmpA,len                          ( a)
    xm.stw tmpB,len                          ( b)
    { addi len, len, -1                           ; xm.bt len, .L_loop_top                       }

  .L_finish:
      xm.lddsp  s3,s2,0
    { nop                                           ; xm.retsp (NSTACKWORDS)*4                         } 


.L_func_end:


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS; .global FUNCTION_NAME.nstackwords /* Translation error on this line: unexpected token at position 42. */ 
.set FUNCTION_NAME.maxcores,1;              .global FUNCTION_NAME.maxcores /* Translation error on this line: unexpected token at position 29. */ 
.set FUNCTION_NAME.maxtimers,0;             .global FUNCTION_NAME.maxtimers /* Translation error on this line: unexpected token at position 30. */ 
.set FUNCTION_NAME.maxchanends,0;           .global FUNCTION_NAME.maxchanends /* Translation error on this line: unexpected token at position 32. */ 
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME


#endif //defined(__VX4B__)



