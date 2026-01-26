// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

/*  

headroom_t vect_complex_s16_scale(
    int16_t* a_real,
    int16_t* a_imag,
    const int16_t* b_real,
    const int16_t* b_imag,
    const int16_t c_real,
    const int16_t c_imag,
    const unsigned length,
    const right_shift_t sat);

*/

#define NSTACKVECS      (4)
#define NSTACKWORDS     (8+8*(NSTACKVECS)+4)


#define STACK_VEC_SAT       (NSTACKWORDS-8-4)
#define STACK_VEC_C_REAL    (NSTACKWORDS-16-4)
#define STACK_VEC_C_IMAG    (NSTACKWORDS-24-4)
#define STACK_VEC_C_IMAG_N  (NSTACKWORDS-32-4)

#define FUNCTION_NAME vect_complex_s16_scale
    
#define a_real      x10
#define a_imag      x11
#define b_real      x12
#define b_imag      x13
#define length      x18
#define _32         x19
#define bytemask    x20


.text
.p2align 2

/*
    We're doing this:

    vR  <- -1
    vR  <- -1 * b.imag
    vC  <- -b.imag  
    acc <- 0
    acc <- vC * c.imag
    vC  <- b.real
    acc <- acc + vC * c.real
    vR  <- acc >> sat
    a.real <- vR

    (vC still has b.real)
    acc <- 0
    acc <- vC * c.imag
    vC  <- b.imag
    acc <- acc + vC * c.real
    vR  <- acc >> sat
    a.imag <- vR
*/


FUNCTION_NAME:
    

    xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in entsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
    xm.stdsp  s3,s2,8
    xm.stdsp  s5,s4,16

    addi t3,sp, (STACK_VEC_C_IMAG_N)*4/* Multiple XAT warnings: 'LDAWSP outside of known frame - offset may need correction', "Falling back on assumption: the int < 64 for the integer value of the item at position 1 in the instruction's operands in ldawsp t3, STACK_VEC_C_IMAG_N\nMessage: the word-scale offset fits in a 6b unsigned immediate" */
    mv s4, a5
    xm.neg s2, s4
    xm.neg s3, s4
xm.zip s3, s2, 4

    xm.stdi  s2,s2, 0(t3)
    xm.stdi  s2,s2, 8(t3)
    xm.stdi  s2,s2, 16(t3)
    xm.stdi  s2,s2, 24(t3)

    addi t3,sp, (STACK_VEC_C_IMAG)*4/* Multiple XAT warnings: 'LDAWSP outside of known frame - offset may need correction', "Falling back on assumption: the int < 64 for the integer value of the item at position 1 in the instruction's operands in ldawsp t3, STACK_VEC_C_IMAG\nMessage: the word-scale offset fits in a 6b unsigned immediate" */
    mv s2, s4

    xm.zip s4, s2, 4

    xm.stdi  s2,s2, 0(t3)
    xm.stdi  s2,s2, 8(t3)
    xm.stdi  s2,s2, 16(t3)
    xm.stdi  s2,s2, 24(t3)
    mv s2, a4
    mv s3, s2
xm.zip s3, s2, 4
    
    xm.stdi  s2,s2, 32(t3)
    xm.stdi  s2,s2, 40(t3)
    xm.stdi  s2,s2, 48(t3)
    xm.stdi  s2,s2, 56(t3)

    mv s2, a7
    mv s3, s2
xm.zip s3, s2, 4
    
    xm.stdi  s2,s2, 64(t3)
    xm.stdi  s2,s2, 72(t3)
    xm.stdi  s2,s2, 80(t3)
    xm.stdi  s2,s2, 88(t3)


.p2align 2
    mv length, a6
    {   li _32, 32                             ;   nop}/* XAT Warning: 'LDWSP has unknown offset - this may need correction' */
#define vect_count  length
    {   srli vect_count, length, 4               ;   slli bytemask, length, SIZEOF_LOG2_S16   }/* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in    shli bytemask, length, SIZEOF_LOG2_S16   \nMessage: The shift amount is not 32" */
    {   xm.zexti bytemask, 5                        ;   slli t3, _32, 3                         }
    {   addi t3,sp, (STACK_VEC_C_IMAG_N)*4        ;   xm.vsetc t3}/* XAT Warning: 'LDAWSP has unknown offset - this may need correction' */
    { nop                                           ;   xm.brff vect_count, .L_loop_bot              }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */

    .L_loop_top:
        { nop                                           ;   xm.vldc t3}
        {   addi vect_count, vect_count, -1           ;   xm.vclrdr                                  }
        {   addi t3,sp, (STACK_VEC_C_REAL)*4          ;   xm.vlmacc0 b_imag}
         xm.vlmacc1 b_imag
        { nop                                           ;   xm.vldc b_real}
        { xm.vlmacc0 t3; nop}
          xm.vlmacc1 t3
        {   add b_real, b_real, _32                 ;   addi t3,sp, (STACK_VEC_SAT)*4}
        xm.vlsat t3
        {   add a_real, a_real, _32                 ;   xm.vstr a_real}
        {   addi t3,sp, (STACK_VEC_C_IMAG)*4          ;   xm.vclrdr                                  }/* XAT Warning: 'LDAWSP has unknown offset - this may need correction' */
        { nop                                           ;   xm.vlmacc0 t3}                        /* XAT Warning: 'LDAWSP has unknown offset - this may need correction' */
        xm.vlmacc1 t3
        {   addi t3,sp, (STACK_VEC_C_REAL)*4          ;   xm.vldc b_imag}/* XAT Warning: 'LDAWSP has unknown offset - this may need correction' */
        {    nop            ;   xm.vlmacc0 t3}
        xm.vlmacc1 t3
        {   add b_imag, b_imag, _32                 ;   addi t3,sp, (STACK_VEC_SAT)*4}
        xm.vlsat t3
        {   add a_imag, a_imag, _32                 ;   xm.vstr a_imag}
        {   addi t3,sp, (STACK_VEC_C_IMAG_N)*4        ;   xm.bt vect_count, .L_loop_top              }/* XAT Warning: 'LDAWSP has unknown offset - this may need correction' */
    .L_loop_bot:

    {   xm.mkmsk bytemask, bytemask                ;   xm.brff bytemask, .L_done                    }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
    { nop                                           ;   xm.vldc t3}
    { nop                                           ;   xm.vclrdr                                  }
    { nop                                           ;   xm.vstd t3}
    {   addi t3,sp, (STACK_VEC_C_REAL)*4          ;   xm.vlmacc0 b_imag}
    xm.vlmacc1 b_imag
#define vec_tmp     b_real
    {   addi vec_tmp,sp, (STACK_VEC_C_IMAG_N)*4    ;   xm.vldc b_real}/* XAT Warning: 'LDAWSP has unknown offset - this may need correction' */
    {  nop              ;   xm.vlmacc0 t3}/* XAT Warning: 'LDAWSP has unknown offset - this may need correction' */
    xm.vlmacc1 t3
    addi t3,sp, (STACK_VEC_SAT)*4
    xm.vlsat t3
        xm.vstrpv vec_tmp, bytemask
        xm.vstrpv a_real, bytemask        
    { nop                                           ;   xm.vldd vec_tmp}
    { nop                                           ;   xm.vstd vec_tmp}
    {   addi t3,sp, (STACK_VEC_C_IMAG)*4          ;   xm.vclrdr                                  }/* XAT Warning: 'LDAWSP has unknown offset - this may need correction' */
    { nop                                           ;   xm.vlmacc0 t3}                          /* XAT Warning: 'LDAWSP has unknown offset - this may need correction' */
    xm.vlmacc1 t3
    {   addi t3,sp, (STACK_VEC_C_REAL)*4          ;   xm.vldc b_imag}/* XAT Warning: 'LDAWSP has unknown offset - this may need correction' */
    {   nop            ;   xm.vlmacc0 t3}
    xm.vlmacc1 t3
     {   addi t3,sp, (STACK_VEC_SAT)*4             ;  nop}
    xm.vlsat t3
        xm.vstrpv vec_tmp, bytemask
        xm.vstrpv a_imag, bytemask
    { nop                                           ;   xm.vldd vec_tmp}
    { nop                                           ;   xm.vstd vec_tmp}



.L_done:
        xm.lddsp  s3,s2,8/* XAT Warning: "Not correcting LDDSP offset because it's not in the local frame range" */
        xm.lddsp  s5,s4,16/* XAT Warning: "Not correcting LDDSP offset because it's not in the local frame range" */

    {   li a0, 15                              ;   xm.vgetc t3}
    {   xm.zexti t3, 5                             ; nop                                           }
    {   sub a0, a0, t3                         ;   xm.retsp (NSTACKWORDS)*4                       }


.L_func_end:

.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords /* Translation error on this line: unexpected token at position 42. */ 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores /* Translation error on this line: unexpected token at position 29. */ 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers /* Translation error on this line: unexpected token at position 30. */ 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends /* Translation error on this line: unexpected token at position 32. */ 
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME


#undef FUNCTION_NAME



#endif //defined(__VX4B__)



