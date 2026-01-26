// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

/*  
headroom_t vect_s32_scale(
    int32_t a[],
    const int32_t b[],
    const unsigned len,
    const int32_t c,
    const right_shift_t b_shr,
    const right_shift_t c_shr);
*/


#define NSTACKWORDS     (8+8)

#define FUNCTION_NAME   vect_s32_scale

#define STACK_VEC_TMP   0  
#define STACK_BYTEMASK  12

#define a           x10 
#define b           x11 
#define len         x12
#define c           x13
#define shr_b       x18
#define _32         x19
#define tmp_vec     x20
#define bytemask    len

.text
.p2align 2



FUNCTION_NAME:
        xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
        xm.stdsp  s3,s2,32
        xm.stdsp  s5,s4,40
    {   li t3, 0                              ; nop                                           }
    {   slli t3, len, SIZEOF_LOG2_S32           ;   xm.vsetc t3}/* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in    shli t3, len, SIZEOF_LOG2_S32           \nMessage: The shift amount is not 32" */
    {   xm.zexti t3, 5                             ;   srli len, len, EPV_LOG2_S32              }/* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in    shri len, len, EPV_LOG2_S32              \nMessage: The shift amount is not 32" */
    {   li _32, 32                             ;   xm.vclrdr                                  }
    mv shr_b, a4
    {   xm.mkmsk t3, t3                          ;   nop}
    {   xm.mkmski t3, 32                           ;   sw t3, (STACK_BYTEMASK)*4             (sp)}
        xm.stdsp  c,c,((STACK_VEC_TMP/2)+0)*8
        xm.stdsp  c,c,((STACK_VEC_TMP/2)+1)*8
        xm.stdsp  c,c,((STACK_VEC_TMP/2)+2)*8
        xm.stdsp  c,c,((STACK_VEC_TMP/2)+3)*8
    mv tmp_vec, a5
    {   addi c,sp, (STACK_VEC_TMP)*4               ;   nop}/* Multiple XAT warnings: 'LDAWSP outside of known frame - offset may need correction', 'LDWSP outside of known frame - offset may need correction' */
        xm.vlashr c, tmp_vec
        xm.vstrpv c, t3

    { nop                                           ;   xm.brff len, .L_loop_bot                     }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
    { nop                                           ;   xm.bu .L_loop_top                          }

.p2align 4
.L_loop_top:
            xm.vlashr b, shr_b
        {   add b, b, _32                           ;   xm.vlmul0 c}  
        {   addi len, len, -1                         ;   xm.vstr a}
        {   add a, a, _32                           ;   xm.bt len, .L_loop_top                     }
.L_loop_bot:

    { nop                                           ;   lw bytemask, (STACK_BYTEMASK)*4        (sp)}
    { nop                                           ;   xm.brff bytemask, .L_finish                  }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
        xm.vlashr b, shr_b
    {   mv t3, c                              ;   xm.vlmul0 c}
    { nop                                           ;   xm.vstd t3}
        xm.vstrpv t3, bytemask
        xm.vstrpv a, bytemask
    { nop                                           ;   xm.vldr t3}
    { nop                                           ;   xm.vstr t3}

.L_finish:
    xm.lddsp  s3,s2,32
    xm.lddsp  s5,s4,40

    {   li a0, 32                              ;   xm.vgetc t3}
    {   xm.zexti t3, 5                             ;   srli a1, t3, 8                          }
    {   xm.shr a0, a0, a1                          ;   addi t3, t3, 1                         }
    {   sub a0, a0, t3                         ;   xm.retsp (NSTACKWORDS)*4                       } 

.L_func_end:




.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS; .global FUNCTION_NAME.nstackwords /* Translation error on this line: unexpected token at position 42. */ 
.set FUNCTION_NAME.maxcores,1;              .global FUNCTION_NAME.maxcores /* Translation error on this line: unexpected token at position 29. */ 
.set FUNCTION_NAME.maxtimers,0;             .global FUNCTION_NAME.maxtimers /* Translation error on this line: unexpected token at position 30. */ 
.set FUNCTION_NAME.maxchanends,0;           .global FUNCTION_NAME.maxchanends /* Translation error on this line: unexpected token at position 32. */ 
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME



#endif //defined(__VX4B__)



