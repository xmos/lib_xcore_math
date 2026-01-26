// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


/*  
headroom_t vect_s16_shl(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const int shl);

headroom_t vect_s32_shl(
    int32_t a[],,
    const int32_t b[],
    const unsigned length,
    const int shl);
*/

#include "../asm_helper.h"



#define NSTACKWORDS     (8+2+2+4)

#define FUNCTION_NAME   shl_vect
#define FNAME_S16       CAT(FUNCTION_NAME, _s16)
#define FNAME_S32       CAT(FUNCTION_NAME, _s32)

#define STACK_TMP_VEC       (NSTACKWORDS-10)

#define a           x10
#define b           x11
#define len         x12
#define b_shl       x13


.text
.p2align 2




vect_s16_shl:

        xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
        li t3, 0x0100
    {   slli s2, len, SIZEOF_LOG2_S16            ;   sw s2, 4                       (sp)}/* Multiple XAT warnings: 'STWSP outside of known frame - offset may need correction', "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in    shli s2, len, SIZEOF_LOG2_S16            \nMessage: The shift amount is not 32" */
    {   srli len, len, EPV_LOG2_S16              ;   xm.vsetc t3}/* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in    shri len, len, EPV_LOG2_S16              \nMessage: The shift amount is not 32" */
    {   xm.zexti s2, 5                              ;   xm.bu .L_apply_op                      }


.L_size_end_vect_s16_shl: 
    .size vect_s16_shl, .L_size_end_vect_s16_shl - vect_s16_shl







vect_s32_shl:

        xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
    {   li t3, 0                              ;   sw s2, 4                       (sp)}
    {   slli s2, len, SIZEOF_LOG2_S32            ;   xm.vsetc t3}/* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in    shli s2, len, SIZEOF_LOG2_S32            \nMessage: The shift amount is not 32" */
    {   srli len, len, EPV_LOG2_S32              ;   xm.zexti s2, 5                          }/* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in    shri len, len, EPV_LOG2_S32              \nMessage: The shift amount is not 32" */
    { nop                                           ;   xm.bu .L_apply_op                      }
    

.L_size_end_vect_s32_shl: 
    .size vect_s32_shl, .L_size_end_vect_s32_shl - vect_s32_shl
    
#undef a
#undef b
#undef len
#undef b_shl






/*
    When branching here:
        *   a --> x10
        *   b --> x11
        *   loop_count --> x12
        *   shl --> x13
        *   tail --> x18
        *   VPU mode must already be set.
*/

#define a           x10
#define b           x11
#define loop_count  x12
#define b_shl       x13
#define b_shr         b_shl
#define tail        x18


.type .L_apply_op,@function;  /* Translation error on this line: unexpected token at position 27. */ 

.L_apply_op:

    {   xm.neg b_shr, b_shl                        ;   xm.zexti s2, 5                          }
    {   xm.mkmsk tail, tail                        ;   xm.brff loop_count, .L_loop_bot          }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
    {   li t3, 32                             ;   xm.bu .L_loop_top                      }
    { nop                                           ;   xm.bu .L_loop_top_inplace              }

    .p2align 4
    .L_loop_top_inplace:
            xm.vlashr b, b_shr
        {   addi loop_count, loop_count, -1           ;   xm.vstr b}
        {   add b, b, t3                           ;   xm.bt loop_count, .L_loop_top_inplace  }
    {   mv a, b                                ;   xm.bu .L_loop_bot                      }

    .p2align 4
    .L_loop_top:
            xm.vlashr b, b_shr
        {   add b, b, t3                           ;   xm.vstr a}
        {   addi loop_count, loop_count, -1           ; nop                                       }
        {   add a, a, t3                           ;   xm.bt loop_count, .L_loop_top          }
    
.L_loop_bot:

    {   addi t3,sp, (STACK_TMP_VEC)*4             ;   xm.brff tail, .L_finish                  }/* Multiple XAT warnings: 'LDAWSP outside of known frame - offset may need correction', 'Instruction xm.brff can only branch forwards; this branch may need revising' */
    { nop                                           ;   xm.vclrdr                              }
        xm.vlashr b, b_shr
    { nop                                           ;   xm.vstd t3}
        xm.vstrpv t3, tail
    {   li a2, 32                              ;   xm.vldr t3}
    {   slli a2, a2, 3                           ;   xm.vstr t3}
        xm.vstrpv a, tail

.L_finish:
    {   li a0, 32                              ;   xm.vgetc t3}
    {   srli a1, t3, 8                          ;   lw s2, 4                       (sp)}
    {   xm.zexti t3, 5                             ;   xm.shr a0, a0, a1                      }
    {   addi t3, t3, 1                         ; nop                                       }
    {   sub a0, a0, t3                         ;   xm.retsp (NSTACKWORDS)*4                   } 


.L_end_apply_op: 
    .size .L_apply_op, .L_end_apply_op - .L_apply_op







.globl vect_s16_shl
.type vect_s16_shl,@function
.set vect_s16_shl.nstackwords,NSTACKWORDS;  .global vect_s16_shl.nstackwords /* Translation error on this line: unexpected token at position 41. */ 
.set vect_s16_shl.maxcores,1;               .global vect_s16_shl.maxcores /* Translation error on this line: unexpected token at position 28. */ 
.set vect_s16_shl.maxtimers,0;              .global vect_s16_shl.maxtimers /* Translation error on this line: unexpected token at position 29. */ 
.set vect_s16_shl.maxchanends,0;            .global vect_s16_shl.maxchanends /* Translation error on this line: unexpected token at position 31. */ 

.globl vect_s32_shl
.type vect_s32_shl,@function
.set vect_s32_shl.nstackwords,NSTACKWORDS;  .global vect_s32_shl.nstackwords /* Translation error on this line: unexpected token at position 41. */ 
.set vect_s32_shl.maxcores,1;               .global vect_s32_shl.maxcores /* Translation error on this line: unexpected token at position 28. */ 
.set vect_s32_shl.maxtimers,0;              .global vect_s32_shl.maxtimers /* Translation error on this line: unexpected token at position 29. */ 
.set vect_s32_shl.maxchanends,0;            .global vect_s32_shl.maxchanends /* Translation error on this line: unexpected token at position 31. */ 


#endif //defined(__VX4B__)
