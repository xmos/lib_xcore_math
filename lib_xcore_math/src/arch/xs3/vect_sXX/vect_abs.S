// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


#include "../asm_helper.h"

.text
.issue_mode dual
.align 4

#define NSTACKWORDS     (8)
#define STACK_TMP_VEC       (NSTACKWORDS-8)

#define a           r0
#define b           r1
#define len         r2
#define tail        r3



/*  
headroom_t vect_s16_abs(
    int16_t a[],
    const int16_t b[],
    const unsigned length);
*/
.cc_top vect_s16_abs.function,vect_s16_abs

vect_s16_abs:
        dualentsp NSTACKWORDS
        ldc r11, 0x0100
    {   shl tail, len, SIZEOF_LOG2_S16          ;   shr len, len, EPV_LOG2_S16              }
    {   zext tail, 5                            ;   vsetc r11                               }
    {                                           ;   bu .L_apply_op                          }

.L_func_end_s16:
.cc_bottom vect_s16_abs.function



/*
headroom_t vect_s32_abs(
    int32_t a[],
    const int32_t b[],
    const unsigned length);
*/
.cc_top vect_s32_abs.function,vect_s32_abs

vect_s32_abs:
        dualentsp NSTACKWORDS
    {   ldc r11, 0                              ;   shl tail, len, SIZEOF_LOG2_S32          }
    {   shr len, len, EPV_LOG2_S32              ;   vsetc r11                               }
    {   zext tail, 5                            ;   bu .L_apply_op                          }

.L_func_end_s32:
.cc_bottom vect_s32_abs.function




#undef a
#undef b
#undef len

/*
    When branching here:
        *   a --> r0
        *   b --> r1
        *   loop_count --> r2
        *   tail --> r3
        *   VPU mode must already be set.
*/

#define a           r0
#define b           r1
#define loop_count  r2
#define tail        r3

.type .L_apply_op,@function
.cc_top .L_apply_op.function,.L_apply_op

.L_apply_op:

    {   mkmsk tail, tail                        ;                                           }
    {   mov r11, b                              ;   bf loop_count, .L_loop_bot              }
    {   ldc r1, 32                              ;   bu .L_loop_top                          }
.align 16
.L_loop_top:
        {   sub loop_count, loop_count, 1           ;   vldr r11[0]                             }
        {                                           ;   vsign                                   }
        {                                           ;   vlmul r11[0]                            }
        {   add a, a, r1                            ;   vstr a[0]                               }
        {   add r11, r11, r1                        ;   bt loop_count, .L_loop_top              }
.L_loop_bot:

    {                                           ;   bf tail, .L_finish                      }
    {                                           ;   vclrdr                                  }
    {                                           ;   vldr r11[0]                             }
    {                                           ;   vsign                                   }
    {   ldaw r11, sp[STACK_TMP_VEC]             ;   vlmul r11[0]                            }
    {                                           ;   vstd r11[0]                             }
    {                                           ;   vpos                                    }
        vstrpv r11[0], tail
    {                                           ;   vldr r11[0]                             }
    {                                           ;   vstr r11[0]                             }
        vstrpv a[0], tail

.L_finish:
    {   ldc r0, 32                              ;   vgetc r11                               }
    {   shr r1, r11, 8                          ;                                           }
    {   zext r11, 5                             ;   shr r0, r0, r1                          }
    {   add r11, r11, 1                         ;                                           }
    {   sub r0, r0, r11                         ;   retsp NSTACKWORDS                       } 

.L_end_apply_op: 
.cc_bottom .L_apply_op.function
.size .L_apply_op, .L_end_apply_op - .L_apply_op





.global vect_s16_abs
.type vect_s16_abs,@function
.set vect_s16_abs.nstackwords,NSTACKWORDS;  .global vect_s16_abs.nstackwords
.set vect_s16_abs.maxcores,1;               .global vect_s16_abs.maxcores
.set vect_s16_abs.maxtimers,0;              .global vect_s16_abs.maxtimers
.set vect_s16_abs.maxchanends,0;            .global vect_s16_abs.maxchanends
.size vect_s16_abs, .L_func_end_s16 - vect_s16_abs

.global vect_s32_abs
.type vect_s32_abs,@function
.set vect_s32_abs.nstackwords,NSTACKWORDS;  .global vect_s32_abs.nstackwords
.set vect_s32_abs.maxcores,1;               .global vect_s32_abs.maxcores
.set vect_s32_abs.maxtimers,0;              .global vect_s32_abs.maxtimers
.set vect_s32_abs.maxchanends,0;            .global vect_s32_abs.maxchanends
.size vect_s32_abs, .L_func_end_s32 - vect_s32_abs




#endif //defined(__XS3A__)