// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)



/*
    int64_t vect_s32_sum(
        const int32_t b[],
        const unsigned length);
*/


#include "../asm_helper.h"


#define FUNCTION_NAME   vect_s32_sum
#define NSTACKWORDS     (16+4)


#define STACK_VEC_TMP       (NSTACKWORDS-8-4)

#define b           x10
#define N           x11
#define tail        x12


.text
.p2align 2


FUNCTION_NAME:
        xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
        xm.stdsp  s3,s2,0

    {   li t3, 0                              ; nop                                           }
    {   slli tail, N, SIZEOF_LOG2_S32            ;   xm.vsetc t3}/* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in    shli tail, N, SIZEOF_LOG2_S32            \nMessage: The shift amount is not 32" */
    {   xm.zexti tail, 5                            ;   xm.vclrdr                                  }
    {   srli N, N, EPV_LOG2_S32                  ;   xm.brff tail, .L_tail_dealt_with             }/* Multiple XAT warnings: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in    shri N, N, EPV_LOG2_S32                  \nMessage: The shift amount is not 32", 'Instruction xm.brff can only branch forwards; this branch may need revising' */

    la t3, vpu_vec_0x40000000 
    {   addi s2,sp, (STACK_VEC_TMP)*4              ;   xm.vldr t3}
    { nop                                           ;   xm.vstd s2}
    {   xm.mkmsk tail, tail                        ;   slli N, N, 3                             }
        xm.vstrpv s2, tail
        sh2add s3, N, b              
    { nop                                           ;   xm.vldc s2}
    { nop                                           ;   xm.vclrdr                                  }
    {   srli N, N, 3                             ;   xm.vlmacc0 s3}
    {   li t3, 32                             ;   xm.vldc t3}

.L_tail_dealt_with:
    la t3, vpu_vec_0x40000000
    {   li t3, 32                             ;   xm.vldc t3}
    {   addi a2,sp, (STACK_VEC_TMP)*4              ;   xm.brff N, .L_loop_bot                       }/* Multiple XAT warnings: 'LDAWSP outside of known frame - offset may need correction', 'Instruction xm.brff can only branch forwards; this branch may need revising' */

.L_loop_top:
        {   addi N, N, -1                             ;   xm.vlmacc0 b}
        {   add b, b, t3                           ;   xm.bt N, .L_loop_top                       }
.L_loop_bot:

.L_finish:

    // Requires vC to be filled with 0x40000000, which it already should be.
  
    la t3, vpu_vec_0x80000000   
    { nop                                           ;   xm.vstr a2}
    { nop                                           ;   xm.vlmacc0 t3}
    
    la t3, vpu_vec_zero  
    {   addi t3,sp, (STACK_VEC_TMP)*4             ;   xm.vldr t3}
    { nop                                           ;   xm.vlmaccr0 t3}
    { nop                                           ;   xm.vstd t3}
    { nop                                           ;   xm.vlmaccr0 t3}
    { nop                                           ;   xm.vstr t3}
    { nop                                           ;   lw a1,0                          ( t3)}
    {   addi a1, a1, 8                           ;   lw a0,4                          ( t3)}

        xm.lddsp  s3,s2,0
        xm.retsp (NSTACKWORDS)*4/* Multiple XAT warnings: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in retsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate", 'RETSP operand may need scaling' */

.L_fend: 

.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords;  /* Translation error on this line: unexpected token at position 42. */ 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores;  /* Translation error on this line: unexpected token at position 29. */ 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers;  /* Translation error on this line: unexpected token at position 30. */ 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends;  /* Translation error on this line: unexpected token at position 32. */ 
.size FUNCTION_NAME, .L_fend - FUNCTION_NAME






#endif //defined(__VX4B__)
