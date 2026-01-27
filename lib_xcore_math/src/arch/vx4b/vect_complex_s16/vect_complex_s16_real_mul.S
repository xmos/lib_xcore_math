// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

/*  

headroom_t vect_complex_s16_real_mul(
    int16_t* a_real,
    int16_t* a_imag,
    const int16_t* b_real,
    const int16_t* b_imag,
    const int16_t c[],
    const unsigned length,
    const right_shift_t sat);

*/

.text
.p2align 2

#define NSTACKVECS      (4)
#define NSTACKWORDS     (16+8*(NSTACKVECS)+4)

#define STACK_VEC_TMP   (NSTACKWORDS-8-4)
#define STACK_VEC_SAT   (NSTACKWORDS-16-4)


#define FUNCTION_NAME vect_complex_s16_real_mul
    
#define a_re        x10
#define a_im        x11
#define b_re        x12
#define b_im        x13
#define c           x18
#define len         x19
#define vec_tmp     x20
#define vec_sat     x21
#define bytemask    x22


FUNCTION_NAME:
    xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
    xm.stdsp  s3,s2,8
    xm.stdsp  s5,s4,16
    xm.stdsp  s7,s6,24

    mv s2, a6
    {   addi t3,sp, (STACK_VEC_SAT)*4             ;  nop}/* Multiple XAT warnings: 'LDAWSP outside of known frame - offset may need correction', 'LDWSP outside of known frame - offset may need correction' */
    {   slli s3, s2, 16                          ;   xm.zexti s2, 16                             }
    {   or s2, s2, s3                           ;   sw s8, 4                          (sp)}
        xm.stdi  s2,s2, 0(t3)
        xm.stdi  s2,s2, 8(t3)
        xm.stdi  s2,s2, 16(t3)
        xm.stdi  s2,s2, 24(t3)

    mv len, a5 
    {   li t3, 32                             ;   mv bytemask, len}
    mv c, a4
    {   srli len, bytemask, 4                    ;   nop}
    {   xm.zexti bytemask, 4                        ; nop                                           }
    {   slli bytemask, bytemask, SIZEOF_LOG2_S16 ;   slli t3, t3, 3                         }/* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in    shli bytemask, bytemask, SIZEOF_LOG2_S16 \nMessage: The shift amount is not 32" */
    {   addi vec_tmp,sp, (STACK_VEC_TMP)*4         ;   xm.vsetc t3}
    {   addi vec_sat,sp, (STACK_VEC_SAT)*4         ; nop                                           }
    {   li t3, 32                             ;   xm.brff len, .L_loop_bot                     }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */

#define _32     x28
    .L_loop_top:
        {   addi len, len, -1                         ;   xm.vclrdr                                  }
        {   add c, c, _32                           ;   xm.vldc c}
        {  nop                    ;   xm.vlmacc0 b_re}
        xm.vlmacc1 b_re
        {   add b_re, b_re, _32                     ;   nop}
        xm.vlsat vec_sat
        {   add a_re, a_re, _32                     ;   xm.vstr a_re}
        { nop                                           ;   xm.vclrdr                                  }
        {  nop                    ;   xm.vlmacc0 b_im}
        xm.vlmacc1 b_im
        {   add b_im, b_im, _32                     ;  nop}
        xm.vlsat vec_sat
        {   add a_im, a_im, _32                     ;   xm.vstr a_im}
        { nop                                           ;   xm.bt len, .L_loop_top                     }

    .L_loop_bot:
#undef _32

    {   xm.mkmsk bytemask, bytemask                ;   xm.brff bytemask, .L_done                    }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */



    xm.vclrdr
    xm.vstd vec_tmp
    xm.vldc c
    xm.vlmacc0 b_re
    xm.vlmacc1 b_re
    xm.vlsat vec_sat
    xm.vstrpv a_re, bytemask
    xm.vstrpv vec_tmp, bytemask
    xm.vldd vec_tmp
    xm.vstd vec_tmp
    xm.vclrdr
    xm.vlmacc0 b_im
    xm.vlmacc1 b_im
    xm.vlsat vec_sat
    xm.vstrpv a_im, bytemask
    xm.vstrpv vec_tmp, bytemask
    xm.vldd vec_tmp
    xm.vstd vec_tmp
    nop

.p2align 2

.L_done:
        xm.lddsp  s3,s2,8
        xm.lddsp  s5,s4,16
        xm.lddsp  s7,s6,24

    {   li a0, 15                              ;   xm.vgetc t3}
    {   xm.zexti t3, 5                             ;   lw s8, 4                          (sp)}
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



