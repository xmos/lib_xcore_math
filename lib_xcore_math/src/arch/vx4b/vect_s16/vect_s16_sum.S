// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)



/*  
    int32_t vect_s16_sum(
        const int16_t b[],
        const unsigned length);
*/


#include "../asm_helper.h"


#define FUNCTION_NAME   vect_s16_sum
#define NSTACKWORDS     (24+8+4)


#define STACK_VEC_TMP       (NSTACKWORDS-24-4)
#define STACK_VEC_TMP2       (NSTACKWORDS-8-2)

#define b           x10
#define N           x11
#define tail        x12


.text
.p2align 2


FUNCTION_NAME:
        xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
        li t3, 0x0100
        xm.stdsp  s3,s2,0
        xm.stdsp  s5,s4,8
     {   addi s4,sp, (STACK_VEC_TMP2)*4        ;   nop                                  }
         addi s5, s4, (-30)

    {   slli tail, N, SIZEOF_LOG2_S16          ;   xm.vsetc t3}/* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in    shli tail, N, SIZEOF_LOG2_S16            \nMessage: The shift amount is not 32" */
    {   xm.zexti tail, 5                       ;   xm.vclrdr                                  }
    {   srli N, N, EPV_LOG2_S16                ;   xm.brff tail, .L_tail_dealt_with         }/* Multiple XAT warnings: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in    shri N, N, EPV_LOG2_S16                  \nMessage: The shift amount is not 32", 'Instruction xm.brff can only branch forwards; this branch may need revising' */

        la t3, vpu_vec_0x0001
    {   addi s2,sp, (STACK_VEC_TMP)*4          ;   xm.vldr t3}
    { nop                                      ;   xm.vstd s2}
    {   xm.mkmsk tail, tail                    ;   slli N, N, 3                             }
        xm.vstrpv s2, tail
    sh2add s3, N, b  
    { nop                                      ;   xm.vldc s2}
    { nop                                      ;   xm.vclrdr                                  }
    {   srli N, N, 3                           ;   xm.vlmaccr0 s3}
    xm.vlmaccr1 s3    

    {nop ; xm.vstd s4}    
    {nop ; xm.vldd s5}
    {nop ; xm.vstr s4}
    {nop ; xm.vldr s5}

    {   li t3, 32                              ;   xm.vldc t3}

.L_tail_dealt_with:
    { nop                                      ;   xm.brff N, .L_loop_bot                       }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
        la t3, vpu_vec_0x0001 
    {   li t3, 32                              ;   xm.vldc t3}
  
.L_loop_top:
        {   addi N, N, -1                      ;   xm.vlmaccr0 b}
        xm.vlmaccr1 b

    {nop ; xm.vstd s4}    
    {nop ; xm.vldd s5}
    {nop ; xm.vstr s4}
    {nop ; xm.vldr s5}

        {   add b, b, t3                       ;   xm.bt N, .L_loop_top                       }
.L_loop_bot:

.L_finish:



    {   addi a1,sp, (STACK_VEC_TMP)*4          ;   nop    /* adddr*/                              }
    
    addi s4, a1, 32-2

    { nop                                      ;   xm.vstd a1}
    { nop                                      ;   lw a0, 0(s4)}
    { slli a0, a0, 16                          ;   xm.vstr a1}
    { nop                                      ;   lw a1, 0(s4)}

        xm.lddsp  s3,s2,0
        xm.lddsp  s5,s4,8
    { xm.zexti a1, 16                          ;   nop}
    {   or a0, a0, a1                          ;   xm.retsp (NSTACKWORDS)*4                       }

.L_fend: 


.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS; .global FUNCTION_NAME.nstackwords /* Translation error on this line: unexpected token at position 42. */ 
.set FUNCTION_NAME.maxcores,1;              .global FUNCTION_NAME.maxcores /* Translation error on this line: unexpected token at position 29. */ 
.set FUNCTION_NAME.maxtimers,0;             .global FUNCTION_NAME.maxtimers /* Translation error on this line: unexpected token at position 30. */ 
.set FUNCTION_NAME.maxchanends,0;           .global FUNCTION_NAME.maxchanends /* Translation error on this line: unexpected token at position 32. */ 
.size FUNCTION_NAME, .L_fend - FUNCTION_NAME




#endif //defined(__VX4B__)
