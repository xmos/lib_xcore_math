// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

.text
.p2align 2

#define NSTACKWORDS     (12)
#define STACK_TMP_VEC       0

#define a           x10
#define b           x11
#define len         x12
#define tail        x13


/*
headroom_t vect_s32_abs(
    int32_t a[],
    const int32_t b[],
    const unsigned length);
*/

vect_s32_abs:
        xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
    {   li t3, 0                              ;   slli tail, len, SIZEOF_LOG2_S32          }/* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in    shli tail, len, SIZEOF_LOG2_S32          \nMessage: The shift amount is not 32" */
    {   srli len, len, EPV_LOG2_S32              ;   xm.vsetc t3}/* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in    shri len, len, EPV_LOG2_S32              \nMessage: The shift amount is not 32" */
    {   xm.zexti tail, 5                            ;   xm.bu .L_apply_op                          }

.L_func_end_s32:




#undef a
#undef b
#undef len

/*
    When branching here:
        *   a --> x10
        *   b --> x11
        *   loop_count --> x12
        *   tail --> x13
        *   VPU mode must already be set.
*/

#define a           x10
#define b           x11
#define loop_count  x12
#define tail        x13

.type .L_apply_op,@function

.L_apply_op:

    {   xm.mkmsk tail, tail                        ; nop                                           }
    {   mv t3, b                              ;   xm.brff loop_count, .L_loop_bot              }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
    {   li a1, 32                              ;   xm.bu .L_loop_top                          }
.p2align 4
.L_loop_top:
        {   addi loop_count, loop_count, -1           ;   xm.vldr t3}
        { nop                                           ;   xm.vsign                                   }
        { nop                                           ;   xm.vlmul0 t3} //TODO this is wrong for 16 bit
        {   add a, a, a1                            ;   xm.vstr a}
        {   add t3, t3, a1                        ;   xm.bt loop_count, .L_loop_top              }
.L_loop_bot:

    { nop                                           ;   xm.brff tail, .L_finish                      }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
    { nop                                           ;   xm.vclrdr                                  }
    { nop                                           ;   xm.vldr t3}
    { nop                                           ;   xm.vsign                                   }
    {   addi t3,sp, (STACK_TMP_VEC)*4             ;   xm.vlmul0 t3} //TODO this is wrong for 16 bit
    { nop                                           ;   xm.vstd t3}
    { nop                                           ;   xm.vpos                                    }
        xm.vstrpv t3, tail
    { nop                                           ;   xm.vldr t3}
    { nop                                           ;   xm.vstr t3}
        xm.vstrpv a, tail

.L_finish:
    {   li a0, 32                              ;   xm.vgetc t3}
    {   srli a1, t3, 8                          ; nop                                           }
    {   xm.zexti t3, 5                             ;   xm.shr a0, a0, a1                          }
    {   addi t3, t3, 1                         ; nop                                           }
    {   sub a0, a0, t3                         ;   xm.retsp (NSTACKWORDS)*4                       } 

.L_end_apply_op: 
.size .L_apply_op, .L_end_apply_op - .L_apply_op

.global vect_s32_abs
.type vect_s32_abs,@function
.set vect_s32_abs.nstackwords,NSTACKWORDS;  .global vect_s32_abs.nstackwords /* Translation error on this line: unexpected token at position 41. */ 
.set vect_s32_abs.maxcores,1;               .global vect_s32_abs.maxcores /* Translation error on this line: unexpected token at position 28. */ 
.set vect_s32_abs.maxtimers,0;              .global vect_s32_abs.maxtimers /* Translation error on this line: unexpected token at position 29. */ 
.set vect_s32_abs.maxchanends,0;            .global vect_s32_abs.maxchanends /* Translation error on this line: unexpected token at position 31. */ 
.size vect_s32_abs, .L_func_end_s32 - vect_s32_abs




#endif //defined(__VX4B__)
