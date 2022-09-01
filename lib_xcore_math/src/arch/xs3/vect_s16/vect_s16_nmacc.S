// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// XMOS Public License: Version 1

#if defined(__XS3A__)


/*  
headroom_t vect_s16_nmacc(
    int16_t acc[],
    const int16_t b[],
    const int16_t c[],
    const unsigned len,
    const int acc_shr,
    const int bc_shr);
*/


#include "../asm_helper.h"

#define NSTACKVECTS     (2)
#define NSTACKWORDS     (8 + 8*NSTACKVECTS)

#define FUNCTION_NAME   vect_s16_nmacc

#define STACK_ACC_SHR   (NSTACKWORDS+1)
#define STACK_BC_SHR    (NSTACKWORDS+2)
#define STACK_VEC_SAT   (NSTACKWORDS-8)
#define STACK_VEC_TMP   (NSTACKWORDS-16)

#define acc         r0 
#define b           r1 
#define c           r2
#define len         r3
#define bc_shr      r4
#define _32         r5
#define tmp         r6
#define tail        r7
#define acc_shr     r8
#define mask        r9

.text
.issue_mode dual
.align 4

.cc_top FUNCTION_NAME.function,FUNCTION_NAME

FUNCTION_NAME:
        dualentsp NSTACKWORDS
        ldc r11, 0x100
        std r4, r5, sp[1]
        std r6, r7, sp[2]
        std r8, r9, sp[3]
    {   shl r11, len, SIZEOF_LOG2_S16           ;   vsetc r11                               }
    {   zext r11, 5                             ;   ldw bc_shr, sp[STACK_BC_SHR]            }
    {   shl tmp, bc_shr, 16                     ;   zext bc_shr, 16                         }
    {   or bc_shr, tmp, bc_shr                  ;   shr len, len, EPV_LOG2_S16              }
    {   ldaw tmp, sp[STACK_VEC_TMP]             ;   ldw acc_shr, sp[STACK_ACC_SHR]          }

        std bc_shr, bc_shr, sp[(STACK_VEC_SAT)/2 + 0]
        std bc_shr, bc_shr, sp[(STACK_VEC_SAT)/2 + 1]
        std bc_shr, bc_shr, sp[(STACK_VEC_SAT)/2 + 2]
        std bc_shr, bc_shr, sp[(STACK_VEC_SAT)/2 + 3]

    {   ldaw bc_shr, sp[STACK_VEC_SAT]          ;   mkmsk tail, r11                         }
    {   ldc _32, 32                             ;   bf len, .L_loop_bot                     }
    {   mkmsk mask, 32                          ;   bu .L_loop_top                          }

.align 16
.L_loop_top:
            vlashr acc[0], acc_shr
            vstrpv acc[0], mask
        {   sub len, len, 1                         ;   vclrdr                                  }
        {   add b, b, _32                           ;   vldc b[0]                               }
        {   add c, c, _32                           ;   vlmacc c[0]                             }
        {                                           ;   vlsat bc_shr[0]                         }
        {                                           ;   vlsub acc[0]                            }
        {   add acc, acc, _32                       ;   vstr acc[0]                             }
        {                                           ;   bt len, .L_loop_top                     }

.L_loop_bot:
    {                                           ;   bf tail, .L_finish                  }
      vlashr acc[0], acc_shr
      vstrpv acc[0], tail
    {                                           ;   vclrdr                              }
    {                                           ;   vldc b[0]                           }
    {                                           ;   vlmacc c[0]                         }
    {   mov r11, bc_shr                         ;   vlsat bc_shr[0]                     }
    {                                           ;   vlsub acc[0]                        }
    {                                           ;   vstd r11[0]                         }
        vstrpv r11[0], tail
        vstrpv acc[0], tail
    {                                           ;   vldr r11[0]                         }
    {                                           ;   vstr r11[0]                         }

.L_finish:
        ldd r4, r5, sp[1]
        ldd r6, r7, sp[2]
        ldd r8, r9, sp[3]
    {   ldc r0, 15                              ;   vgetc r11                           }
    {   zext r11, 5                             ;                                       }
    {   sub r0, r0, r11                         ;   retsp NSTACKWORDS                   } 

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



