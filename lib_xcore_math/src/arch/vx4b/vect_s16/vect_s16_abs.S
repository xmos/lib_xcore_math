// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

.text
.p2align 2

#define NSTACKWORDS     (20)
#define STACK_TMP_VEC       2

#define a           x10
#define b           x11
#define len         x12
#define tail        x13



/*  
headroom_t vect_s16_abs(
    int16_t a[],
    const int16_t b[],
    const unsigned length);
*/

vect_s16_abs:
        xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
        xm.stdsp  s3,s2,0
        li t3, 0x0100
    {   slli tail, len, SIZEOF_LOG2_S16          ;   srli len, len, EPV_LOG2_S16              }/* Multiple XAT warnings: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in    shli tail, len, SIZEOF_LOG2_S16          \nMessage: The shift amount is not 32", "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in    shri len, len, EPV_LOG2_S16              \nMessage: The shift amount is not 32" */
    {   xm.zexti tail, 5                            ;   xm.vsetc t3}
    { nop                                           ;   xm.bu .L_apply_op                          }

.L_func_end_s16:

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
    {   mv s3, b                              ;   xm.brff loop_count, .L_loop_bot              }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
    {   li a1, 32                              ;   xm.bu .L_loop_top                          }
.p2align 4
.L_loop_top:
        {   addi loop_count, loop_count, -1           ;   xm.vldr s3}
        { nop                                           ;   xm.vsign                                   }
        { nop                                           ;   xm.vlmul0 s3}
        xm.vlmul1 s3

        {addi a1,sp, (STACK_TMP_VEC)*4             ;  xm.vgetc t3} 
        {nop; xm.vstr a1}
        {li a1, 32; xm.vladd a1}
        {nop; xm.vsetc t3}

        {   add a, a, a1                            ;   xm.vstr a}
        {   add s3, s3, a1                        ;   xm.bt loop_count, .L_loop_top              }
.L_loop_bot:

    { nop                                           ;   xm.brff tail, .L_finish                      }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
    { nop                                           ;   xm.vclrdr                                  }
    { nop                                           ;   xm.vldr s3}
    { nop                                           ;   xm.vsign                                   }
        

    {   nop             ;   xm.vlmul0 s3} 
    xm.vlmul1 s3

    { addi s3,sp, (STACK_TMP_VEC)*4             ;   xm.vgetc t3} 
    { nop; xm.vstr s3}
    { nop; xm.vladd s3}
       
    { addi s3,sp, (STACK_TMP_VEC)*4             ;   xm.vsetc t3}
    { nop                                           ;   xm.vstd s3}
    { nop                                           ;   xm.vpos                                    }
        xm.vstrpv s3, tail
    { nop                                           ;   xm.vldr s3}
    { nop                                           ;   xm.vstr s3}
        xm.vstrpv a, tail

.L_finish:
    {   li a0, 32                              ;   xm.vgetc t3}
    {   srli a1, t3, 8                          ; nop                                           }
    {   xm.zexti t3, 5                             ;   xm.shr a0, a0, a1                          }
    {   addi t3, t3, 1                         ; nop                                           }
     xm.lddsp  s3,s2,0             
    {   sub a0, a0, t3                         ;   xm.retsp (NSTACKWORDS)*4                       } 

.L_end_apply_op: 
.size .L_apply_op, .L_end_apply_op - .L_apply_op





.global vect_s16_abs
.type vect_s16_abs,@function
.set vect_s16_abs.nstackwords,NSTACKWORDS;  .global vect_s16_abs.nstackwords /* Translation error on this line: unexpected token at position 41. */ 
.set vect_s16_abs.maxcores,1;               .global vect_s16_abs.maxcores /* Translation error on this line: unexpected token at position 28. */ 
.set vect_s16_abs.maxtimers,0;              .global vect_s16_abs.maxtimers /* Translation error on this line: unexpected token at position 29. */ 
.set vect_s16_abs.maxchanends,0;            .global vect_s16_abs.maxchanends /* Translation error on this line: unexpected token at position 31. */ 
.size vect_s16_abs, .L_func_end_s16 - vect_s16_abs

#endif //defined(__VX4B__)
