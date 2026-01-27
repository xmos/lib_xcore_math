// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


/*  
int64_t vect_s32_energy(
    const int32_t b[],
    const unsigned length,
    const right_shift_t b_shr);
*/


#include "../asm_helper.h"

.text
.p2align 2

#define NSTACKVECS      (2)
#define NSTACKWORDS     (8 + 8*NSTACKVECS+4)


#define FUNCTION_NAME   vect_s32_energy

#define STACK_VEC_TMP       (NSTACKWORDS-8-4)
#define STACK_VEC_VR        (NSTACKWORDS-16-4)

#define b           x10
#define N           x11
#define b_shr       x12
#define vec_tmp     x13
#define tail        x18

.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.align 16; /* Translation error on this line: unexpected token at position 9. */ 

FUNCTION_NAME:

        xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
        xm.stdsp  s3,s2,0

    {   li t3, 0                              ;   addi vec_tmp,sp, (STACK_VEC_TMP)*4         }
    {   slli tail, N, SIZEOF_LOG2_S32            ;   xm.vsetc t3}/* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in    shli tail, N, SIZEOF_LOG2_S32            \nMessage: The shift amount is not 32" */
    {   xm.zexti tail, 5                            ;   xm.vclrdr                                  }
    {   srli N, N, EPV_LOG2_S32                  ;   xm.brff tail, .L_tail_dealt_with_s32         }/* Multiple XAT warnings: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in    shri N, N, EPV_LOG2_S32                  \nMessage: The shift amount is not 32", 'Instruction xm.brff can only branch forwards; this branch may need revising' */

    { nop                                           ;   slli N, N, 5                             }
    {   add t3, b, N                           ;   xm.vstd vec_tmp}
    {   xm.mkmsk tail, tail                        ; nop                                           }
        xm.vlashr t3, b_shr
        xm.vstrpv vec_tmp, tail
#undef tail

    { nop                                           ;   xm.vldc vec_tmp}
    { nop                                           ;   xm.vclrdr                                  }
    {   srli N, N, 5                             ;   xm.vlmacc0 vec_tmp}

.L_tail_dealt_with_s32:
    {   addi t3,sp, (STACK_VEC_VR)*4              ;   xm.brff N, .L_loop_bot_s32                   }/* Multiple XAT warnings: 'LDAWSP outside of known frame - offset may need correction', 'Instruction xm.brff can only branch forwards; this branch may need revising' */


.L_loop_top_s32:
        {   li t3, 32                             ;   xm.vstr t3}
            xm.vlashr b, b_shr
        {   add b, b, t3                           ;   xm.vstr vec_tmp}
        {   addi t3,sp, (STACK_VEC_VR)*4              ;   xm.vldc vec_tmp}
        { nop                                           ;   xm.vldr t3}
        {   addi N, N, -1                             ;   xm.vlmacc0 vec_tmp}
        { nop                                           ;   xm.bt N, .L_loop_top_s32                   }
.L_loop_bot_s32:

.L_finish_s32:


lui t3, %hi(vpu_vec_0x40000000)
        addi t3,t3, %lo(vpu_vec_0x40000000)
    {   addi a2,sp, (STACK_VEC_TMP)*4              ;   xm.vldc t3}
lui t3, %hi(vpu_vec_0x80000000)        
        addi t3,t3, %lo(vpu_vec_0x80000000)        
    { nop                                           ;   xm.vstr a2}
    { nop                                           ;   xm.vlmacc0 t3}
lui t3, %hi(vpu_vec_zero)        
        addi t3,t3, %lo(vpu_vec_zero)        
    {   addi t3,sp, (STACK_VEC_TMP)*4             ;   xm.vldr t3}
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
