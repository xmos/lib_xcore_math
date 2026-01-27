// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)



/*  
headroom_t vect_s16_headroom(
    const int16_t* v, 
    const unsigned length);
*/

#include "../asm_helper.h"

.text
.p2align 2

#define NSTACKWORDS     12

#define STACK_TMP_VEC   0

#define arg_v       x10
#define arg_len     x11


#define FUNCTION_NAME vect_s16_headroom

FUNCTION_NAME:
    {   li t3, 32                             ;   xm.entsp (NSTACKWORDS)*4               }
    {   slli t3, t3, 3                         ;   xm.vclrdr                              }
    {   slli a2, arg_len, 1                      ;   xm.vsetc t3}
    {   srli arg_len, arg_len, 4                 ;   mv t3, arg_v                      }

    {   addi a0,sp, (STACK_TMP_VEC)*4              ; nop                                       }
    {   li a3, 32                              ; nop                                       }
    {   xm.zexti a2, 5                              ;   xm.brff arg_len, .L_loop_bot_s16         }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
    { nop                                           ;   xm.bu .L_loop_top_s16                  }
     
    .p2align 4
    .L_loop_top_s16:
        {   add t3, t3, a3                        ;   xm.vldr t3}
        {   addi arg_len, arg_len, -1                 ;   xm.vstr a0}
        { nop                                           ;   xm.bt arg_len, .L_loop_top_s16         }

.L_loop_bot_s16:
    { nop                                           ;   xm.brff a2, .L_finish16                  }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
    {   xm.mkmsk a2, a2                            ;   xm.vstd a0}
    {   mv t3, a0                             ;   xm.vldr t3}
    xm.vstrpv a0, a2
    { nop                                           ;   xm.vldr t3}
    { nop                                           ;   xm.vstr t3}

.L_finish16:
    {   li a0, 15                              ;   xm.vgetc t3}
    {   xm.zexti t3, 5                             ; nop                                       }
    {   sub a0, a0, t3                         ;   xm.retsp (NSTACKWORDS)*4                   }


.L_func_end_s16:

.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords /* Translation error on this line: unexpected token at position 42. */ 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores /* Translation error on this line: unexpected token at position 29. */ 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers /* Translation error on this line: unexpected token at position 30. */ 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends /* Translation error on this line: unexpected token at position 32. */ 
.size FUNCTION_NAME, .L_func_end_s16 - FUNCTION_NAME


#undef FUNCTION_NAME







/*  
headroom_t vect_s32_headroom(
    const int32_t* v,
    const unsigned length);
*/

#define FUNCTION_NAME vect_s32_headroom

FUNCTION_NAME:
    xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */

    {   li t3, 0                              ;   slli a2, arg_len, 2                  }
    {   srli arg_len, arg_len, 3                 ;   xm.vsetc t3}
    {   addi a0,sp, (STACK_TMP_VEC)*4              ;   mv t3, arg_v                      }
    {   li a3, 32                              ;   xm.vclrdr                              }
    {   xm.zexti a2, 5                              ;   xm.brff arg_len, .L_loop_bot_s32         }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
    { nop                                           ;   xm.bu .L_loop_top_s32                  }

    .p2align 4
    .L_loop_top_s32:
        {   add t3, t3, a3                        ;   xm.vldr t3}
        {   addi arg_len, arg_len, -1                 ;   xm.vstr a0}
        { nop                                           ;   xm.bt arg_len, .L_loop_top_s32         }

    .L_loop_bot_s32:
    { nop                                           ;   xm.brff a2, .L_finish32                  }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
    {   xm.mkmsk a2, a2                            ;   xm.vstd a0}
    {   mv t3, a0                             ;   xm.vldr t3}
    xm.vstrpv a0, a2
    { nop                                           ;   xm.vldr t3}
    { nop                                           ;   xm.vstr t3}

    .L_finish32:
    {   li a0, 31                              ;   xm.vgetc t3}
    {   xm.zexti t3, 5                             ; nop                                       }
    {sub a0, a0, t3; nop}

    {      nop                      ;   xm.retsp (NSTACKWORDS)*4                   }

.L_func_end_s32:

.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords /* Translation error on this line: unexpected token at position 42. */ 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores /* Translation error on this line: unexpected token at position 29. */ 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers /* Translation error on this line: unexpected token at position 30. */ 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends /* Translation error on this line: unexpected token at position 32. */ 
.size FUNCTION_NAME, .L_func_end_s32 - FUNCTION_NAME

#undef FUNCTION_NAME





#endif //defined(__VX4B__)
