// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


/*  
headroom_t vect_s16_mul(
    int16_t a[],
    const int16_t b[],
    const int16_t c[],
    const unsigned len,
    const int a_shr);
*/


#include "../asm_helper.h"

#define NSTACKWORDS     (8+8)

#define FUNCTION_NAME   vect_s16_mul

#define STACK_A_SHR     (NSTACKWORDS+1)
#define STACK_VEC_SAT   (NSTACKWORDS-8)

#define a           r0 
#define b           r1 
#define c           r2
#define len         r3
#define a_shr       r4
#define _32         r5
#define tmp         r6
#define tail        r7

.text
.issue_mode dual
.align 4

.cc_top FUNCTION_NAME.function,FUNCTION_NAME

FUNCTION_NAME:
        dualentsp NSTACKWORDS
        ldc r11, 0x100
        std r4, r5, sp[1]
        std r6, r7, sp[2]
    {   shl r11, len, SIZEOF_LOG2_S16           ;   vsetc r11                               }
    {   zext r11, 5                             ;   ldw a_shr, sp[STACK_A_SHR]              }
    {   shl tmp, a_shr, 16                      ;   zext a_shr, 16                          }
    {   or a_shr, tmp, a_shr                    ;   shr len, len, EPV_LOG2_S16              }

        std a_shr, a_shr, sp[(STACK_VEC_SAT)/2 + 0]
        std a_shr, a_shr, sp[(STACK_VEC_SAT)/2 + 1]
        std a_shr, a_shr, sp[(STACK_VEC_SAT)/2 + 2]
        std a_shr, a_shr, sp[(STACK_VEC_SAT)/2 + 3]

    {   ldaw a_shr, sp[STACK_VEC_SAT]           ;   mkmsk tail, r11                         }
    {   ldc _32, 32                             ;   bf len, .L_loop_bot                     }
    {                                           ;   bu .L_loop_top                          }

.align 16
.L_loop_top:
        {   sub len, len, 1                         ;   vclrdr                                  }
        {   add b, b, _32                           ;   vldc b[0]                               }
        {   add c, c, _32                           ;   vlmacc c[0]                             }
        {                                           ;   vlsat a_shr[0]                          }
        {   add a, a, _32                           ;   vstr a[0]                               }
        {                                           ;   bt len, .L_loop_top                     }

.L_loop_bot:
    {                                           ;   bf tail, .L_finish                  }
    {                                           ;   vclrdr                              }
    {                                           ;   vldc b[0]                           }
    {                                           ;   vlmacc c[0]                         }
    {   mov r11, a_shr                          ;   vlsat a_shr[0]                      }
    {                                           ;   vstd r11[0]                         }
        vstrpv r11[0], tail
        vstrpv a[0], tail
    {                                           ;   vldr r11[0]                         }
    {                                           ;   vstr r11[0]                         }

.L_finish:
        ldd r4, r5, sp[1]
        ldd r6, r7, sp[2]
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



