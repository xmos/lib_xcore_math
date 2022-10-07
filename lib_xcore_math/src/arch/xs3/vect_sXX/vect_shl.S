// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


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



#define NSTACKWORDS     (8+2)

#define FUNCTION_NAME   shl_vect
#define FNAME_S16       CAT(FUNCTION_NAME, _s16)
#define FNAME_S32       CAT(FUNCTION_NAME, _s32)

#define STACK_TMP_VEC       (NSTACKWORDS-8)

#define a           r0
#define b           r1
#define len         r2
#define b_shl       r3


.text
.issue_mode dual
.align 4



.cc_top vect_s16_shl.function,vect_s16_shl

vect_s16_shl:

        dualentsp NSTACKWORDS
        ldc r11, 0x0100
    {   shl r4, len, SIZEOF_LOG2_S16            ;   stw r4, sp[1]                       }
    {   shr len, len, EPV_LOG2_S16              ;   vsetc r11                           }
    {   zext r4, 5                              ;   bu .L_apply_op                      }

.cc_bottom vect_s16_shl.function

.L_size_end_vect_s16_shl: 
    .size vect_s16_shl, .L_size_end_vect_s16_shl - vect_s16_shl






.cc_top vect_s32_shl.function,vect_s32_shl

vect_s32_shl:

        dualentsp NSTACKWORDS
    {   ldc r11, 0                              ;   stw r4, sp[1]                       }
    {   shl r4, len, SIZEOF_LOG2_S32            ;   vsetc r11                           }
    {   shr len, len, EPV_LOG2_S32              ;   zext r4, 5                          }
    {                                           ;   bu .L_apply_op                      }
    
.cc_bottom vect_s32_shl.function

.L_size_end_vect_s32_shl: 
    .size vect_s32_shl, .L_size_end_vect_s32_shl - vect_s32_shl
    
#undef a
#undef b
#undef len
#undef b_shl






/*
    When branching here:
        *   a --> r0
        *   b --> r1
        *   loop_count --> r2
        *   shl --> r3
        *   tail --> r4
        *   VPU mode must already be set.
*/

#define a           r0
#define b           r1
#define loop_count  r2
#define b_shl       r3
#define b_shr         b_shl
#define tail        r4


.type .L_apply_op,@function; 
.cc_top .L_apply_op.function,.L_apply_op

.L_apply_op:

    {   neg b_shr, b_shl                        ;   zext r4, 5                          }
    {   mkmsk tail, tail                        ;   bf loop_count, .L_loop_bot          }
    {   ldc r11, 32                             ;   bu .L_loop_top                      }
    {                                           ;   bu .L_loop_top_inplace              }

    .align 16
    .L_loop_top_inplace:
            vlashr b[0], b_shr
        {   sub loop_count, loop_count, 1           ;   vstr b[0]                           }
        {   add b, b, r11                           ;   bt loop_count, .L_loop_top_inplace  }
    {   mov a, b                                ;   bu .L_loop_bot                      }

    .align 16
    .L_loop_top:
            vlashr b[0], b_shr
        {   add b, b, r11                           ;   vstr a[0]                           }
        {   sub loop_count, loop_count, 1           ;                                       }
        {   add a, a, r11                           ;   bt loop_count, .L_loop_top          }
    
.L_loop_bot:

    {   ldaw r11, sp[STACK_TMP_VEC]             ;   bf tail, .L_finish                  }
    {                                           ;   vclrdr                              }
        vlashr b[0], b_shr
    {                                           ;   vstd r11[0]                         }
        vstrpv r11[0], tail
    {   ldc r2, 32                              ;   vldr r11[0]                         }
    {   shl r2, r2, 3                           ;   vstr r11[0]                         }
        vstrpv a[0], tail

.L_finish:
    {   ldc r0, 32                              ;   vgetc r11                           }
    {   shr r1, r11, 8                          ;   ldw r4, sp[1]                       }
    {   zext r11, 5                             ;   shr r0, r0, r1                      }
    {   add r11, r11, 1                         ;                                       }
    {   sub r0, r0, r11                         ;   retsp NSTACKWORDS                   } 

.cc_bottom .L_apply_op.function

.L_end_apply_op: 
    .size .L_apply_op, .L_end_apply_op - .L_apply_op







.globl vect_s16_shl
.type vect_s16_shl,@function
.set vect_s16_shl.nstackwords,NSTACKWORDS;  .global vect_s16_shl.nstackwords
.set vect_s16_shl.maxcores,1;               .global vect_s16_shl.maxcores
.set vect_s16_shl.maxtimers,0;              .global vect_s16_shl.maxtimers
.set vect_s16_shl.maxchanends,0;            .global vect_s16_shl.maxchanends

.globl vect_s32_shl
.type vect_s32_shl,@function
.set vect_s32_shl.nstackwords,NSTACKWORDS;  .global vect_s32_shl.nstackwords
.set vect_s32_shl.maxcores,1;               .global vect_s32_shl.maxcores
.set vect_s32_shl.maxtimers,0;              .global vect_s32_shl.maxtimers
.set vect_s32_shl.maxchanends,0;            .global vect_s32_shl.maxchanends


#endif //defined(__XS3A__)