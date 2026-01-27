// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


/*  
int32_t vect_s16_abs_sum(
    const int16_t b[],
    const unsigned length);
*/


#include "../asm_helper.h"

.text
.p2align 2

#define NSTACKVECS      (4)
#define NSTACKWORDS     (8+8*NSTACKVECS+4)

#define FUNCTION_NAME       vect_s16_abs_sum

#define STACK_VEC_TMP       (NSTACKWORDS-24-4)
#define STACK_VEC_VR        (NSTACKWORDS-32-4)
#define STACK_VEC_TMP2      (NSTACKWORDS-8-2)

#define b           x10 
#define N           x11 
#define tail        x12 
#define tmp         x13
#define neg_1       x18
#define pos_2       x19

.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.align 16; /* Translation error on this line: unexpected token at position 9. */ 

FUNCTION_NAME:
        xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
        xm.stdsp  s3,s2,0
        xm.stdsp  s5,s4,8
        li t3, 0x0100

     {   addi s4, sp, (STACK_VEC_TMP2)*4        ;   nop                                  }
         addi s5, s4, (-30)

    {   slli tail, N, SIZEOF_LOG2_S16            ;   xm.vsetc t3}/* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in    shli tail, N, SIZEOF_LOG2_S16            \nMessage: The shift amount is not 32" */
    {   xm.zexti tail, 5                            ;   xm.vclrdr                                  }
    {   srli N, N, EPV_LOG2_S16                  ;   xm.mkmsk tail, tail                        }/* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in    shri N, N, EPV_LOG2_S16                  \nMessage: The shift amount is not 32" */
        la t3, vpu_vec_0x0002
    {   mv pos_2, t3                          ; nop                                           }
        la t3, vpu_vec_neg_1
    {   mv neg_1, t3                          ;   xm.vldc t3}
    {   slli tmp, N, 5                           ; nop                                           }
    {   add t3, b, tmp                         ;   xm.brff tail, .L_tail_dealt_with             }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */

    {   addi tmp,sp, (STACK_VEC_TMP)*4             ;   xm.vldr t3}
    { nop                                           ;   xm.vstd tmp}
        xm.vstrpv tmp, tail
    {   addi t3,sp, (STACK_VEC_VR)*4              ;   xm.vclrdr                                  }
    { nop                                           ;   xm.vlmaccr0 tmp}
    xm.vlmaccr1 tmp

    {nop ; xm.vstd s4}    
    {nop ; xm.vldd s5}
    {nop ; xm.vstr s4}
    {nop ; xm.vldr s5}


    {   mv t3, tmp                            ;   xm.vstr t3}
    { nop                                           ;   xm.vldr t3}
    { nop                                           ;   xm.vpos                                    }
    {   addi t3,sp, (STACK_VEC_VR)*4              ;   xm.vstr t3}
    { nop                                           ;   xm.vldr t3}
    { nop                                           ;   xm.vldc pos_2}
    { nop                                           ;   xm.vlmaccr0 tmp}
    xm.vlmaccr1 tmp

    {nop ; xm.vstd s4}    
    {nop ; xm.vldd s5}
    {nop ; xm.vstr s4}
    {nop ; xm.vldr s5}




.L_tail_dealt_with:
    {   addi tmp,sp, (STACK_VEC_TMP)*4             ; nop                                           }
    {   addi t3,sp, (STACK_VEC_VR)*4              ;   xm.brff N, .L_loop_bot                       }/* Multiple XAT warnings: 'LDAWSP outside of known frame - offset may need correction', 'Instruction xm.brff can only branch forwards; this branch may need revising' */
.L_loop_top:
        { nop                                             ;   xm.vldc neg_1}
        { nop                                             ;   xm.vlmaccr0 b}
        xm.vlmaccr1 b

        {nop ; xm.vstd s4}    
        {nop ; xm.vldd s5}
        {nop ; xm.vstr s4}
        {nop ; xm.vldr s5}


        {   mv t3, b                                ;   xm.vstr t3}
        {   li t3, 32                               ;   xm.vldr t3}
        {   add b, b, t3                             ;   xm.vpos                                    }
        {   addi t3,sp, (STACK_VEC_VR)*4                ;   xm.vstr tmp}
        { nop                                             ;   xm.vldr t3}
        { nop                                             ;   xm.vldc pos_2}
        {   addi N, N, -1                               ;   xm.vlmaccr0 tmp}
        xm.vlmaccr1 tmp

        {nop ; xm.vstd s4}    
        {nop ; xm.vldd s5}
        {nop ; xm.vstr s4}
        {nop ; xm.vldr s5}


        { nop                                             ;   xm.bt N, .L_loop_top                       }
.L_loop_bot:


.L_finish:


   


    { addi a1,sp, (STACK_VEC_TMP)*4            ; nop/*  xm.vadddr */    }
      addi s4, a1, 32-2                           
    { nop                                      ;   xm.vstd a1}
    { nop                                      ;  lw a0, 0(s4)}
    {   slli a0, a0, 16                        ;   xm.vstr a1}
    { nop                                      ;  lw a1, 0(s4)}
        xm.lddsp  s3,s2,0                               
        xm.lddsp  s5,s4,8
    { xm.zexti a1, 16                          ;   nop}
    {   or a0, a0, a1                          ;   xm.retsp (NSTACKWORDS)*4                       }

//.cc_bottom FUNCTION_NAME.function;  /* Translation error on this line: unexpected token at position 33. */ 
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords;  /* Translation error on this line: unexpected token at position 42. */ 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores;  /* Translation error on this line: unexpected token at position 29. */ 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers;  /* Translation error on this line: unexpected token at position 30. */ 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends;  /* Translation error on this line: unexpected token at position 32. */ 
.L_end: 
    .size FUNCTION_NAME, .L_end - FUNCTION_NAME





#endif //defined(__VX4B__)
