// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


/*  
headroom_t vect_s16_mul(
    int16_t a[],
    const int16_t b[],
    const int16_t c[],
    const unsigned len,
    const int a_shr);
*/


#include "../asm_helper.h"

#define NSTACKWORDS     (8+8)

#define FUNCTION_NAME   vect_s16_mul

#define STACK_VEC_SAT   0

#define a           x10 
#define b           x11 
#define c           x12
#define len         x13
#define a_shr       x18
#define _32         x19
#define tmp         x20
#define tail        x21

.text
.p2align 2


FUNCTION_NAME:
        xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
        li t3, 0x100
        xm.stdsp  s3,s2,32
        xm.stdsp  s5,s4,40
    {   slli t3, len, SIZEOF_LOG2_S16           ;   xm.vsetc t3}/* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in    shli t3, len, SIZEOF_LOG2_S16           \nMessage: The shift amount is not 32" */
    mv a_shr, a4
    {   xm.zexti t3, 5                             ;   nop}
    {   slli tmp, a_shr, 16                      ;   xm.zexti a_shr, 16                          }
    {   or a_shr, tmp, a_shr                    ;   srli len, len, EPV_LOG2_S16              }/* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in    shri len, len, EPV_LOG2_S16              \nMessage: The shift amount is not 32" */

        xm.stdsp  a_shr,a_shr,((STACK_VEC_SAT)/2 + 0)*8
        xm.stdsp  a_shr,a_shr,((STACK_VEC_SAT)/2 + 1)*8
        xm.stdsp  a_shr,a_shr,((STACK_VEC_SAT)/2 + 2)*8
        xm.stdsp  a_shr,a_shr,((STACK_VEC_SAT)/2 + 3)*8

    {   addi a_shr,sp, (STACK_VEC_SAT)*4           ;   xm.mkmsk tail, t3                         }
    {   li _32, 32                             ;   xm.brff len, .L_loop_bot                     }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
    { nop                                           ;   xm.bu .L_loop_top                          }

.p2align 4
.L_loop_top:
        {   addi len, len, -1                         ;   xm.vclrdr                                  }
        {   add b, b, _32                           ;   xm.vldc b}
        {   nop                         ;   xm.vlmacc0 c}
        xm.vlmacc1 c
        {add c, c, _32  ; nop}
        xm.vlsat a_shr
        {   add a, a, _32                           ;   xm.vstr a}
        { nop                                           ;   xm.bt len, .L_loop_top                     }

.L_loop_bot:
    { nop                                           ;   xm.brff tail, .L_finish                  }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
    { nop                                           ;   xm.vclrdr                              }
    { nop                                           ;   xm.vldc b}
    { nop                                           ;   xm.vlmacc0 c}
    xm.vlmacc1 c
    {   mv t3, a_shr                          ;   nop}
    xm.vlsat a_shr
    { nop                                           ;   xm.vstd t3}
        xm.vstrpv t3, tail
        xm.vstrpv a, tail
    { nop                                           ;   xm.vldr t3}
    { nop                                           ;   xm.vstr t3}

.L_finish:
        xm.lddsp  s3,s2,32
        xm.lddsp  s5,s4,40
    {   li a0, 15                              ;   xm.vgetc t3}
    {   xm.zexti t3, 5                             ; nop                                       }
    {   sub a0, a0, t3                         ;   xm.retsp (NSTACKWORDS)*4                   } 

.L_func_end:


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS; .global FUNCTION_NAME.nstackwords /* Translation error on this line: unexpected token at position 42. */ 
.set FUNCTION_NAME.maxcores,1;              .global FUNCTION_NAME.maxcores /* Translation error on this line: unexpected token at position 29. */ 
.set FUNCTION_NAME.maxtimers,0;             .global FUNCTION_NAME.maxtimers /* Translation error on this line: unexpected token at position 30. */ 
.set FUNCTION_NAME.maxchanends,0;           .global FUNCTION_NAME.maxchanends /* Translation error on this line: unexpected token at position 32. */ 
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME



#endif //defined(__VX4B__)



