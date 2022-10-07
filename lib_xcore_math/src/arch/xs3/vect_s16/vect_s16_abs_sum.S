// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


/*  
int32_t vect_s16_abs_sum(
    const int16_t b[],
    const unsigned length);
*/


#include "../asm_helper.h"

.text
.issue_mode dual
.align 4

#define NSTACKVECS      (2)
#define NSTACKWORDS     (8+8*NSTACKVECS)

#define FUNCTION_NAME       vect_s16_abs_sum

#define STACK_VEC_TMP       (NSTACKWORDS-8)
#define STACK_VEC_VR        (NSTACKWORDS-16)

#define b           r0 
#define N           r1 
#define tail        r2 
#define tmp         r3
#define neg_1       r4
#define pos_2       r5

.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.align 16;
.cc_top FUNCTION_NAME.function,FUNCTION_NAME

FUNCTION_NAME:
        dualentsp NSTACKWORDS
        std r4, r5, sp[1]
        ldc r11, 0x0100

    {   shl tail, N, SIZEOF_LOG2_S16            ;   vsetc r11                               }
    {   zext tail, 5                            ;   vclrdr                                  }
    {   shr N, N, EPV_LOG2_S16                  ;   mkmsk tail, tail                        }
        ldaw r11, cp[vpu_vec_0x0002]
    {   mov pos_2, r11                          ;                                           }
        ldaw r11, cp[vpu_vec_neg_1]
    {   mov neg_1, r11                          ;   vldc r11[0]                             }
    {   shl tmp, N, 5                           ;                                           }
    {   add r11, b, tmp                         ;   bf tail, .L_tail_dealt_with             }

    {   ldaw tmp, sp[STACK_VEC_TMP]             ;   vldr r11[0]                             }
    {                                           ;   vstd tmp[0]                             }
        vstrpv tmp[0], tail
    {   ldaw r11, sp[STACK_VEC_VR]              ;   vclrdr                                  }
    {                                           ;   vlmacc tmp[0]                           }
    {   mov r11, tmp                            ;   vstr r11[0]                             }
    {                                           ;   vldr r11[0]                             }
    {                                           ;   vpos                                    }
    {   ldaw r11, sp[STACK_VEC_VR]              ;   vstr r11[0]                             }
    {                                           ;   vldr r11[0]                             }
    {                                           ;   vldc pos_2[0]                           }
    {                                           ;   vlmacc tmp[0]                           }

.L_tail_dealt_with:
    {   ldaw tmp, sp[STACK_VEC_TMP]             ;                                           }
    {   ldaw r11, sp[STACK_VEC_VR]              ;   bf N, .L_loop_bot                       }
.L_loop_top:
        {                                             ;   vldc neg_1[0]                           }
        {                                             ;   vlmacc b[0]                             }
        {   mov r11, b                                ;   vstr r11[0]                             }
        {   ldc r11, 32                               ;   vldr r11[0]                             }
        {   add b, b, r11                             ;   vpos                                    }
        {   ldaw r11, sp[STACK_VEC_VR]                ;   vstr tmp[0]                             }
        {                                             ;   vldr r11[0]                             }
        {                                             ;   vldc pos_2[0]                           }
        {   sub N, N, 1                               ;   vlmacc tmp[0]                           }
        {                                             ;   bt N, .L_loop_top                       }
.L_loop_bot:


.L_finish:

        ldd r4, r5, sp[1]
    {   ldaw r1, sp[STACK_VEC_TMP]              ;   vadddr                                  }
    {                                           ;   vstd r1[0]                              }
    {                                           ;   ldw r11, sp[STACK_VEC_TMP]              }
    {   shl r11, r11, 16                        ;   vstr r1[0]                              }
    {                                           ;   ldw r1, sp[STACK_VEC_TMP]               }
    {   or r0, r11, r1                          ;   retsp NSTACKWORDS                       }

.cc_bottom FUNCTION_NAME.function; 
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords; 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores; 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers; 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends; 
.L_end: 
    .size FUNCTION_NAME, .L_end - FUNCTION_NAME





#endif //defined(__XS3A__)