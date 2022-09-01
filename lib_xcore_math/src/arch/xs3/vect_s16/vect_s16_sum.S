// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)



/*  
    int32_t vect_s16_sum(
        const int16_t b[],
        const unsigned length);
*/


#include "../asm_helper.h"


#define FUNCTION_NAME   vect_s16_sum
#define NSTACKWORDS     (16)


#define STACK_VEC_TMP       (NSTACKWORDS-8)

#define b           r0
#define N           r1
#define tail        r2


.text
.issue_mode dual
.align 4

.cc_top FUNCTION_NAME.function,FUNCTION_NAME

FUNCTION_NAME:
        dualentsp NSTACKWORDS
        ldc r11, 0x0100
        std r4, r5, sp[0]

    {   shl tail, N, SIZEOF_LOG2_S16            ;   vsetc r11                               }
    {   zext tail, 5                            ;   vclrdr                                  }
    {   shr N, N, EPV_LOG2_S16                  ;   bf tail, .L_tail_dealt_with         }

        ldaw r11, cp[vpu_vec_0x0001]
    {   ldaw r4, sp[STACK_VEC_TMP]              ;   vldr r11[0]                             }
    {                                           ;   vstd r4[0]                              }
    {   mkmsk tail, tail                        ;   shl N, N, 3                             }
        vstrpv r4[0], tail
        ldaw r5, b[N]                
    {                                           ;   vldc r4[0]                              }
    {                                           ;   vclrdr                                  }
    {   shr N, N, 3                             ;   vlmacc r5[0]                            }
    {   ldc r11, 32                             ;   vldc r11[0]                             }

.L_tail_dealt_with:
    {                                           ;   bf N, .L_loop_bot                       }
        ldaw r11, cp[vpu_vec_0x0001]
    {   ldc r11, 32                             ;   vldc r11[0]                             }

.L_loop_top:
        {   sub N, N, 1                             ;   vlmacc b[0]                             }
        {   add b, b, r11                           ;   bt N, .L_loop_top                       }
.L_loop_bot:

.L_finish:

        ldd r4, r5, sp[0]
    {   ldaw r1, sp[STACK_VEC_TMP]              ;   vadddr                                  }
    {                                           ;   vstd r1[0]                              }
    {                                           ;   ldw r11, sp[STACK_VEC_TMP]              }
    {   shl r11, r11, 16                        ;   vstr r1[0]                              }
    {                                           ;   ldw r1, sp[STACK_VEC_TMP]               }
    {   or r0, r11, r1                          ;   retsp NSTACKWORDS                       }

.L_fend: 
.cc_bottom FUNCTION_NAME.function


.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS; .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;              .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;             .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;           .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME, .L_fend - FUNCTION_NAME




#endif //defined(__XS3A__)