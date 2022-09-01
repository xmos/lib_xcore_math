// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


#include "../asm_helper.h"

/*  

    headroom_t vect_s16_scale(
        int16_t a[],
        const int16_t b[],
        const unsigned length,
        const int16_t c,
        const right_shift_t a_shr);

*/


#define NSTACKWORDS     (8+8)

#define FUNCTION_NAME   vect_s16_scale

#define STACK_A_SHR         (NSTACKWORDS+1)

#define STACK_VEC_A_SHR     (NSTACKWORDS-8)
#define STACK_BYTEMASK      6

#define a           r0 
#define b           r1 
#define len         r2
#define c           r3
#define _32         r4
#define tail        r5


.text
.issue_mode dual
.align 4

.cc_top FUNCTION_NAME.function,FUNCTION_NAME
FUNCTION_NAME:
    {   mov r11, c                              ;   dualentsp NSTACKWORDS                   }
        std r4, r5, sp[1]

        zip r11, c, 4
        std c, c, sp[(STACK_VEC_A_SHR/2)+0]
        std c, c, sp[(STACK_VEC_A_SHR/2)+1]
        std c, c, sp[(STACK_VEC_A_SHR/2)+2]
        std c, c, sp[(STACK_VEC_A_SHR/2)+3]

#undef  c
#define tmp     r3

        ldc r11, 0x100
    {   ldaw r11, sp[STACK_VEC_A_SHR]           ;   vsetc r11                               }
    {                                           ;   ldw tmp, sp[STACK_A_SHR]                }
    {   mov r11, tmp                            ;   vldc r11[0]                             }
        zip r11, tmp, 4
        std tmp, tmp, sp[(STACK_VEC_A_SHR/2)+0]
        std tmp, tmp, sp[(STACK_VEC_A_SHR/2)+1]
        std tmp, tmp, sp[(STACK_VEC_A_SHR/2)+2]
        std tmp, tmp, sp[(STACK_VEC_A_SHR/2)+3]

#undef tmp

    {   shl tail, len, SIZEOF_LOG2_S16          ;   shr len, len, EPV_LOG2_S16              }
    {   zext tail, 5                            ;   ldc _32, 32                             }


    {   ldaw r11, sp[STACK_VEC_A_SHR]           ;   bf len, .L_loop_bot                     }
    {                                           ;   bu .L_loop_top                          }

.align 16
.L_loop_top:
        {   sub len, len, 1                         ;   vclrdr                                  }
        {                                           ;   vlmacc b[0]                             }
        {   add b, b, _32                           ;   vlsat r11[0]                            }
        {   add a, a, _32                           ;   vstr a[0]                               }
        {                                           ;   bt len, .L_loop_top                     }
.L_loop_bot:

    {   mkmsk tail, tail                        ;   bf tail, .L_finish                      }
    {   not tail, tail                          ;   vclrdr                                  }
        vstrpv r11[0], tail
    {   not tail, tail                          ;   vlmacc b[0]                             }
    {                                           ;   vlsat r11[0]                            }
        vstrpv a[0], tail
        vstrpv r11[0], tail
    {                                           ;   vldr r11[0]                             }
    {                                           ;   vstr r11[0]                             }


.L_finish:
    ldd r4, r5, sp[1]

    {   ldc r0, 15                              ;   vgetc r11                               }
    {   zext r11, 5                             ;                                           }
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



