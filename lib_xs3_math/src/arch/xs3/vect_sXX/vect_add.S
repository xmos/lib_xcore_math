// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


#include "../asm_helper.h"



.text
.issue_mode dual
.align 4


#define NSTACKWORDS     (8+8)

#define STACK_B_SHR     (NSTACKWORDS+1)
#define STACK_C_SHR     (NSTACKWORDS+2)
#define STACK_VEC_TMP   (NSTACKWORDS-8)
#define STACK_BYTEMASK  6

#define a           r0 
#define b           r1 
#define c           r2
#define len         r3
#define shr_b       r4
#define shr_c       r5
#define _32         r6
#define tmp_vec     r7
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
.cc_top vect_s16_add.function,vect_s16_add
vect_s16_add:
FNAME_S16:
        dualentsp NSTACKWORDS
        ldc r11, 0x100
    {   shl r11, len, SIZEOF_LOG2_S16           ;   vsetc r11                           }
    {   zext r11, 5                             ;   shr len, len, EPV_LOG2_S16          }
    {                                           ;   bu .L_apply_op                      }
.L_func_end_s16: 
.cc_bottom vect_s16_add.function




/*
headroom_t vect_s32_add(
    int32_t a[],
    const int32_t b[],
    const int32_t c[],
    const unsigned len,
    const int b_shr,
    const int c_shr);
*/
.cc_top vect_s32_add.function,vect_s32_add
vect_s32_add:
FNAME_S32:
        dualentsp NSTACKWORDS
    {   ldc r11, 0                              ;                                           }
    {   shl r11, len, SIZEOF_LOG2_S32           ;   vsetc r11                               }
    {   zext r11, 5                             ;   shr len, len, EPV_LOG2_S32              }
    {                                           ;   bu .L_apply_op                          }
.L_func_end_s32: 
.cc_bottom vect_s32_add.function






/*
    Code shared by all functions above
*/
.type .L_apply_op,@function
.cc_top .L_apply_op.function,.L_apply_op
.L_apply_op:

        std r4, r5, sp[1]
        std r6, r7, sp[2]
    {   ldc _32, 32                             ;   vclrdr                              }
    {   mkmsk r11, r11                          ;   ldw shr_c, sp[STACK_C_SHR]          }
    {   ldaw tmp_vec, sp[NSTACKWORDS-8]         ;   ldw shr_b, sp[STACK_B_SHR]          }
    {   mkmsk r11, 32                           ;   stw r11, sp[STACK_BYTEMASK]         }
    {                                           ;   bf len, .L_loop_bot                 }
    {                                           ;   bu .L_loop_top                      }


// If we just initialize vC with 1's (0x40000000 for s32), couldn't this work?
// .L_loop_top:
//         {   sub len, len, 1                         ;   vclrdr                              }
//         {   add b, b, _32                           ;   vlmacc b[0]                         } 
//         {   add c, c, _32                           ;   vlmacc c[0]                         }
//         {                                           ;   vlsat sat[0]                        }
//         {   add a, a, _32                           ;   vstr a[0]                           }
//         {                                           ;   bt len, .L_loop_top                 }
// .L_loop_bot:

.align 16
.L_loop_top:
            vlashr b[0], shr_b
            vstrpv tmp_vec[0], r11
            vlashr c[0], shr_c
        {   add b, b, _32                           ;                                       } 
        {   add c, c, _32                           ;   vladd tmp_vec[0]                    }  
        {   sub len, len, 1                         ;   vstr a[0]                           }
        {   add a, a, _32                           ;   bt len, .L_loop_top                 }
.L_loop_bot:

    {                                           ;   ldw bytemask, sp[STACK_BYTEMASK]    }
    {                                           ;   bf bytemask, .L_finish              }
        vlashr b[0], shr_b
        vstrpv tmp_vec[0], r11 
        vlashr c[0], shr_c
    {   mov r11, tmp_vec                        ;   vladd tmp_vec[0]                    }
    {                                           ;   vstd tmp_vec[0]                     }
        vstrpv tmp_vec[0], bytemask
        vstrpv a[0], bytemask
    {                                           ;   vldr r11[0]                         }
    {                                           ;   vstr r11[0]                         }

.L_finish:
    ldd r4, r5, sp[1]
    ldd r6, r7, sp[2]

    // Should work for both 16 and 32 bit modes
    {   ldc r0, 32                              ;   vgetc r11                           }
    {   zext r11, 5                             ;   shr r1, r11, 8                      }
    {   shr r0, r0, r1                          ;   add r11, r11, 1                     }
    {   sub r0, r0, r11                         ;   retsp NSTACKWORDS                   } 

.cc_bottom .L_apply_op.function
.L_end_apply_op: 
    .size .L_apply_op, .L_end_apply_op - .L_apply_op



.global vect_s16_add
.type vect_s16_add,@function
.set vect_s16_add.nstackwords,NSTACKWORDS;  .global vect_s16_add.nstackwords
.set vect_s16_add.maxcores,1;               .global vect_s16_add.maxcores
.set vect_s16_add.maxtimers,0;              .global vect_s16_add.maxtimers
.set vect_s16_add.maxchanends,0;            .global vect_s16_add.maxchanends
.size vect_s16_add, .L_func_end_s16 - vect_s16_add

.global vect_s32_add
.type vect_s32_add,@function
.set vect_s32_add.nstackwords,NSTACKWORDS;  .global vect_s32_add.nstackwords
.set vect_s32_add.maxcores,1;               .global vect_s32_add.maxcores
.set vect_s32_add.maxtimers,0;              .global vect_s32_add.maxtimers
.set vect_s32_add.maxchanends,0;            .global vect_s32_add.maxchanends
.size vect_s32_add, .L_func_end_s32 - vect_s32_add





#endif //defined(__XS3A__)



