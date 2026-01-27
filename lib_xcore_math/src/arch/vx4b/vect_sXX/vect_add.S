// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"



.text
.p2align 2


#define NSTACKWORDS     (32)
#define STACK_VEC_TMP   (NSTACKWORDS-8-1)
#define STACK_BYTEMASK  8

#define a           x10 
#define b           x11 
#define c           x12
#define len         x13

#define shr_b       x18
#define shr_c       x19
#define _32         x20
#define tmp_vec     x21
#define bytemask    len





/*  
headroom_t vect_s16_add(
    int16_t a[],
    const int16_t b[],
    const int16_t c[],
    const unsigned len,
    const int b_shr,
    const int c_shr);
*/
vect_s16_add:
FNAME_S16:
        xm.entsp (NSTACKWORDS)*4
        li t3, 0x100
    {   slli t3, len, SIZEOF_LOG2_S16           ;   xm.vsetc t3}
    {   xm.zexti t3, 5                             ;   srli len, len, EPV_LOG2_S16          }
        { nop                                           ;   xm.bu .L_apply_op                          }
.L_func_end_s16: 




/*
headroom_t vect_s32_add(
    int32_t a[],
    const int32_t b[],
    const int32_t c[],
    const unsigned len,
    const int b_shr,
    const int c_shr);
*/
vect_s32_add:
FNAME_S32:
        xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
    {   li t3, 0                              ; nop                                           }
    {   slli t3, len, SIZEOF_LOG2_S32           ;   xm.vsetc t3}/* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in    shli t3, len, SIZEOF_LOG2_S32           \nMessage: The shift amount is not 32" */
    {   xm.zexti t3, 5                             ;   srli len, len, EPV_LOG2_S32              }/* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in    shri len, len, EPV_LOG2_S32              \nMessage: The shift amount is not 32" */
    { nop                                           ;   xm.bu .L_apply_op                          }
.L_func_end_s32: 






/*
    Code shared by all functions above
*/
.type .L_apply_op,@function
.L_apply_op:

        xm.stdsp  s3,s2,0
        xm.stdsp  s5,s4,8
    {   li _32, 32                             ;   xm.vclrdr                              }
    mv shr_c, a5 
    mv shr_b, a4 
    {   xm.mkmsk t3, t3                          ;   nop   }
    {   addi tmp_vec,sp, (NSTACKWORDS-8-1)*4         ;   nop}
    {   xm.mkmski t3, 32                           ;   sw t3, (STACK_BYTEMASK)*4         (sp)}
    { nop                                           ;   xm.brff len, .L_loop_bot                 }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
    { nop                                           ;   xm.bu .L_loop_top                      }

.p2align 4
.L_loop_top:
            xm.vlashr b, shr_b
            xm.vstrpv tmp_vec, t3
            xm.vlashr c, shr_c
        {   add b, b, _32                           ; nop                                       } 
        {   add c, c, _32                           ;   xm.vladd tmp_vec}  
        {   addi len, len, -1                         ;   xm.vstr a}
        {   add a, a, _32                           ;   xm.bt len, .L_loop_top                 }
.L_loop_bot:

    { nop                                           ;   lw bytemask, (STACK_BYTEMASK)*4    (sp)}
    { nop                                           ;   xm.brff bytemask, .L_finish              }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
        xm.vlashr b, shr_b
        xm.vstrpv tmp_vec, t3 
        xm.vlashr c, shr_c
    {   mv t3, tmp_vec                        ;   xm.vladd tmp_vec}
    { nop                                           ;   xm.vstd tmp_vec}
        xm.vstrpv tmp_vec, bytemask
        xm.vstrpv a, bytemask
    { nop                                           ;   xm.vldr t3}
    { nop                                           ;   xm.vstr t3}

.L_finish:
    xm.lddsp  s3,s2,0
    xm.lddsp  s5,s4,8

    // Should work for both 16 and 32 bit modes
    {   li a0, 32                              ;   xm.vgetc t3}
    {   xm.zexti t3, 5                             ;   srli a1, t3, 8                      }
    {   xm.shr a0, a0, a1                          ;   addi t3, t3, 1                     }
    {   sub a0, a0, t3                         ;   xm.retsp (NSTACKWORDS)*4                   } 

.L_end_apply_op: 
    .size .L_apply_op, .L_end_apply_op - .L_apply_op



.global vect_s16_add
.type vect_s16_add,@function
.set vect_s16_add.nstackwords,NSTACKWORDS;  .global vect_s16_add.nstackwords /* Translation error on this line: unexpected token at position 41. */ 
.set vect_s16_add.maxcores,1;               .global vect_s16_add.maxcores /* Translation error on this line: unexpected token at position 28. */ 
.set vect_s16_add.maxtimers,0;              .global vect_s16_add.maxtimers /* Translation error on this line: unexpected token at position 29. */ 
.set vect_s16_add.maxchanends,0;            .global vect_s16_add.maxchanends /* Translation error on this line: unexpected token at position 31. */ 
.size vect_s16_add, .L_func_end_s16 - vect_s16_add

.global vect_s32_add
.type vect_s32_add,@function
.set vect_s32_add.nstackwords,NSTACKWORDS;  .global vect_s32_add.nstackwords /* Translation error on this line: unexpected token at position 41. */ 
.set vect_s32_add.maxcores,1;               .global vect_s32_add.maxcores /* Translation error on this line: unexpected token at position 28. */ 
.set vect_s32_add.maxtimers,0;              .global vect_s32_add.maxtimers /* Translation error on this line: unexpected token at position 29. */ 
.set vect_s32_add.maxchanends,0;            .global vect_s32_add.maxchanends /* Translation error on this line: unexpected token at position 31. */ 
.size vect_s32_add, .L_func_end_s32 - vect_s32_add





#endif //defined(__VX4B__)



