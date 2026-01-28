// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


/*  

int64_t vect_s32_abs_sum(
    const int32_t b[],
    const unsigned length);
*/


#include "../asm_helper.h"

.text
.p2align 2

#define NSTACKVECS      (2)
#define NSTACKWORDS     (8 + 8*NSTACKVECS+4)

#define FUNCTION_NAME       vect_s32_abs_sum

#define STACK_VEC_TMP       (NSTACKWORDS-8-4)
#define STACK_VEC_VR        (NSTACKWORDS-16-4)

#define b           x10      // ![0x%08X]
#define N           x11      // ![%d]
#define tail        x12      // ![0x%X]
#define _32         x13      // ![%d]
#define tmp         x18      // ![%d]
#define mask        x19      // ![0x%X]



.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.align 16; /* Translation error on this line: unexpected token at position 9. */ 

FUNCTION_NAME:

        xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
        xm.stdsp  s3,s2,0

    {   li t3, 0                              ;   slli tail, N, SIZEOF_LOG2_S32            }/* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in    shli tail, N, SIZEOF_LOG2_S32            \nMessage: The shift amount is not 32" */
    {   addi t3,sp, (STACK_VEC_VR)*4              ;   xm.vsetc t3}
    {   xm.zexti tail, 5                            ;   xm.vclrdr                                  }
    {   srli N, N, EPV_LOG2_S32                  ;   xm.brff tail, .L_tail_dealt_with             }/* Multiple XAT warnings: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in    shri N, N, EPV_LOG2_S32                  \nMessage: The shift amount is not 32", 'Instruction xm.brff can only branch forwards; this branch may need revising' */

    {   addi tmp,sp, (STACK_VEC_TMP)*4             ;   mv t3, b                              }
    {   xm.mkmsk mask, tail                        ;   xm.vstd tmp}
    {   addi t3,sp, (STACK_VEC_VR)*4              ;   xm.vldr t3}
    { nop                                           ;   xm.vsign                                   }
        xm.vstrpv tmp, mask
    { nop                                           ;   xm.vldc tmp}
    { nop                                           ;   xm.vclrdr                                  }
    {   add b, b, tail                          ;   xm.vlmacc0 b}
.L_tail_dealt_with:
    {   li _32, 32                             ;   xm.brff N, .L_loop_bot_s32                   }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */

.L_loop_top_s32:
        {   mv t3, b                              ;   xm.vstr t3}
        {   addi N, N, -1                             ;   xm.vldr t3}
        {   addi t3,sp, (STACK_VEC_TMP)*4             ;   xm.vsign                                   }
        { nop                                           ;   xm.vstr t3}
        {   addi t3,sp, (STACK_VEC_VR)*4              ;   xm.vldc t3}
        { nop                                           ;   xm.vldr t3}
        {   add b, b, _32                           ;   xm.vlmacc0 b}
        { nop                                           ;   xm.bt N, .L_loop_top_s32                   }
.L_loop_bot_s32:

.L_finish_s32:

    { nop                                           ;   xm.vstr t3}
lui t3, %hi(vpu_vec_0x40000000)
        addi t3,t3, %lo(vpu_vec_0x40000000)
    { nop                                           ;   xm.vldc t3}
lui t3, %hi(vpu_vec_0x80000000)        
        addi t3,t3, %lo(vpu_vec_0x80000000)        
    { nop                                           ;   xm.vlmacc0 t3}
lui t3, %hi(vpu_vec_zero)
        addi t3,t3, %lo(vpu_vec_zero)
    {   addi t3,sp, (STACK_VEC_VR)*4              ;   xm.vldr t3}
    { nop                                           ;   xm.vlmaccr0 t3}
    { nop                                           ;   xm.vstd t3}
    { nop                                           ;   xm.vlmaccr0 t3}
    { nop                                           ;   xm.vstr t3}
    { nop                                           ;   lw a1,0                          ( t3)}
    {   addi a1, a1, 8                           ;   lw a0,4                          ( t3)}

        xm.lddsp  s3,s2,0
        xm.retsp (NSTACKWORDS)*4/* Multiple XAT warnings: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in retsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate", 'RETSP operand may need scaling' */


//.cc_bottom FUNCTION_NAME.function;  /* Translation error on this line: unexpected token at position 33. */ 
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords;  /* Translation error on this line: unexpected token at position 42. */ 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores;  /* Translation error on this line: unexpected token at position 29. */ 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers;  /* Translation error on this line: unexpected token at position 30. */ 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends;  /* Translation error on this line: unexpected token at position 32. */ 
.L_end: 
    .size FUNCTION_NAME, .L_end - FUNCTION_NAME






#endif //defined(__VX4B__)
