// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


/*  
int64_t vect_s16_dot(
    const int16_t b[],
    const int16_t c[],
    const unsigned length);
*/

#include "../asm_helper.h"

.text
.issue_mode dual
.align 4

#define NSTACKVECTS     (4)
#define NSTACKWORDS     (2 + 8*NSTACKVECTS)

#define FUNCTION_NAME   vect_s16_dot

#define STACK_VEC_TMP   (NSTACKWORDS-8)
#define STACK_ACC_LOW   (NSTACKWORDS-16)
#define STACK_ACC_MID   (NSTACKWORDS-24)
#define STACK_ACC_HIGH  (NSTACKWORDS-32)

#define b           r0
#define c           r1
#define N           r2
#define vec_ones    r3
#define tail        r4
#define _32         r5

.issue_mode dual
.align 4

.cc_top FUNCTION_NAME.function,FUNCTION_NAME

FUNCTION_NAME:
        dualentsp NSTACKWORDS
        ldc r11, 0x0100
        std r4, r5, sp[0]
        // std r6, r7, sp[1]
        // std r8, r9, sp[2]

    {   shl tail, N, SIZEOF_LOG2_S16            ;   vsetc r11                               }
    {   zext tail, 5                            ;   vclrdr                                  }
    {   shr N, N, EPV_LOG2_S16                  ;   ldaw r11, sp[STACK_ACC_LOW]             }
    {   ldaw r11, sp[STACK_ACC_MID]             ;   vstd r11[0]                             }
    {   ldaw r11, sp[STACK_ACC_HIGH]            ;   vstd r11[0]                             }
    {   ldc _32, 32                             ;   vstd r11[0]                             }
        ldaw r11, cp[vpu_vec_0x0001]
    {   mov vec_ones, r11                       ;                                           }
    {   mov r11, b                              ;   bf N, .L_loop_bot                       }

    .L_loop_top:
        {   ldaw r11, sp[STACK_ACC_LOW]             ;   vclrdr                                  }
        {                                           ;   vldr r11[0]                             }
        {   add b, b, _32                           ;   vldc b[0]                               }
        {   add c, c, _32                           ;   vlmacc c[0]                             }
        {   ldaw r11, sp[STACK_VEC_TMP]             ;   vstr r11[0]                             }
        {   ldaw r11, sp[STACK_ACC_MID]             ;   vstd r11[0]                             }
        {   ldaw r11, sp[STACK_ACC_HIGH]            ;   vldr r11[0]                             }
        {                                           ;   vldd r11[0]                             }
        {   ldaw r11, sp[STACK_VEC_TMP]             ;   vldc vec_ones[0]                        }
        {   ldaw r11, sp[STACK_ACC_HIGH]            ;   vlmacc r11[0]                           }
        {   ldaw r11, sp[STACK_ACC_MID]             ;   vstd r11[0]                             }
        {   sub N, N, 1                             ;   vstr r11[0]                             }
        {   mov r11, b                              ;   bt N, .L_loop_top                       }
    .L_loop_bot:

    {   mkmsk tail, tail                        ;   bf tail, .L_get_res                     }
    {   ldc N, 1                                ;   vclrdr                                  }
    {   ldaw b, sp[STACK_VEC_TMP]               ;   vldr r11[0]                             }
    {                                           ;   vstd b[0]                               }
        vstrpv b[0], tail
    {   ldc tail, 0                             ;   bu .L_loop_top                          }

    .L_get_res:

    {   ldaw r11, sp[STACK_ACC_LOW]             ;   vclrdr                                  }
    {                                           ;   vldr r11[0]                             }
    {   ldaw r11, sp[STACK_VEC_TMP]             ;   vadddr                                  }
    {                                           ;   vstr r11[0]                             }
    {                                           ;   ldw r2, r11[0]                          }
    {   ldaw r11, sp[STACK_ACC_MID]             ;   vstd r11[0]                             }
    {   ldaw r11, sp[STACK_ACC_HIGH]            ;   vldr r11[0]                             }
    {   ldaw r11, sp[STACK_VEC_TMP]             ;   vldd r11[0]                             }
    {                                           ;   vlmacc r11[0]                           }
    {                                           ;   vadddr                                  }
    {                                           ;   vstr r11[0]                             }
    {   zext r2, 16                             ;   ldw r0, r11[0]                          }
    {   shl r0, r0, 16                          ;   vstd r11[0]                             }
    {   add r0, r0, r2                          ;   ldw r1, r11[0]                          }
    {   sext r1, 16                             ;                                           }

.L_finish:
        ldd r4, r5, sp[0]
        // ldd r6, r7, sp[1]
        // ldd r8, r9, sp[2]
        retsp NSTACKWORDS

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