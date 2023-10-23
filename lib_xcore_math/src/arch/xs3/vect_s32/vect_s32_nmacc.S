// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// XMOS Public License: Version 1

#if defined(__XS3A__)


#include "../asm_helper.h"

/*  

headroom_t vect_s32_nmacc(
    int32_t acc[],
    const int32_t b[],
    const int32_t c[],
    const unsigned len,
    const int acc_shr,
    const int b_shr,
    const int c_shr);
*/


#define NSTACKWORDS     (8+8)

#define FUNCTION_NAME   vect_s32_nmacc

#define STACK_ACC_SHR   (NSTACKWORDS+1)
#define STACK_B_SHR     (NSTACKWORDS+2)
#define STACK_C_SHR     (NSTACKWORDS+3)
#define STACK_VEC_TMP   (NSTACKWORDS-8)
#define STACK_BYTEMASK  7

#define acc         r0 
#define b           r1 
#define c           r2
#define len         r3
#define shr_b       r4
#define shr_c       r5
#define _32         r6
#define tmp_vec     r7
#define shr_acc     r8
#define bytemask    len

.text
.issue_mode dual
.align 4



.cc_top FUNCTION_NAME.function,FUNCTION_NAME

FUNCTION_NAME:
        dualentsp NSTACKWORDS
    {   ldc r11, 0                              ;   stw r8, sp[6]                           }
    {   shl r11, len, SIZEOF_LOG2_S32           ;   vsetc r11                               }
    {   zext r11, 5                             ;   shr len, len, EPV_LOG2_S32              }
    {                                           ;   ldw shr_acc, sp[STACK_ACC_SHR]          }
    
        std r4, r5, sp[1]
        std r6, r7, sp[2]
    {   ldc _32, 32                             ;   vclrdr                                  }
    {   mkmsk r11, r11                          ;   ldw shr_c, sp[STACK_C_SHR]              }
    {   ldaw tmp_vec, sp[NSTACKWORDS-8]         ;   ldw shr_b, sp[STACK_B_SHR]              }
    {   mkmsk r11, 32                           ;   stw r11, sp[STACK_BYTEMASK]             }
    {                                           ;   bf len, .L_loop_bot                     }
    {                                           ;   bu .L_loop_top                          }

.align 16
.L_loop_top:
            vlashr acc[0], shr_acc
            vstrpv acc[0], r11
            vlashr b[0], shr_b
            vstrpv tmp_vec[0], r11
            vlashr c[0], shr_c
        {   add b, b, _32                           ;                                           } 
        {   add c, c, _32                           ;   vlmul tmp_vec[0]                        }
        {                                           ;   vlsub acc[0]                            }
        {   sub len, len, 1                         ;   vstr acc[0]                             }
        {   add acc, acc, _32                       ;   bt len, .L_loop_top                     }
.L_loop_bot:

    {                                           ;   ldw bytemask, sp[STACK_BYTEMASK]        }
    {                                           ;   bf bytemask, .L_finish                  }
        vlashr acc[0], shr_acc
        vstrpv acc[0], bytemask
        vlashr b[0], shr_b
        vstrpv tmp_vec[0], r11 
        vlashr c[0], shr_c
    {   mov r11, tmp_vec                        ;   vlmul tmp_vec[0]                        }
    {                                           ;   vlsub acc[0]                            }
    {                                           ;   vstd tmp_vec[0]                         }
        vstrpv tmp_vec[0], bytemask
        vstrpv acc[0], bytemask
    {                                           ;   vldr r11[0]                             }
    {                                           ;   vstr r11[0]                             }

.L_finish:
    ldd r4, r5, sp[1]
    ldd r6, r7, sp[2]


    {   ldc r0, 31                              ;   vgetc r11                               }
    {   zext r11, 5                             ;   ldw r8, sp[6]                           }
    {   sub r0, r0, r11                         ;   retsp NSTACKWORDS                       } 


.L_func_end:
.cc_bottom FUNCTION_NAME.function


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS; .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;              .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;             .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;           .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME


#endif //defined(__XS3A__)



