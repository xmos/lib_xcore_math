// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)



/*
    int64_t vect_s32_sum(
        const int32_t b[],
        const unsigned length);
*/


#include "../asm_helper.h"


#define FUNCTION_NAME   vect_s32_sum
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
        std r4, r5, sp[0]

    {   ldc r11, 0                              ;                                           }
    {   shl tail, N, SIZEOF_LOG2_S32            ;   vsetc r11                               }
    {   zext tail, 5                            ;   vclrdr                                  }
    {   shr N, N, EPV_LOG2_S32                  ;   bf tail, .L_tail_dealt_with             }

        ldaw r11, cp[vpu_vec_0x40000000]
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
        ldaw r11, cp[vpu_vec_0x40000000]
    {   ldc r11, 32                             ;   vldc r11[0]                             }
    {   ldaw r2, sp[STACK_VEC_TMP]              ;   bf N, .L_loop_bot                       }

.L_loop_top:
        {   sub N, N, 1                             ;   vlmacc b[0]                             }
        {   add b, b, r11                           ;   bt N, .L_loop_top                       }
.L_loop_bot:

.L_finish:

    // Requires vC to be filled with 0x40000000, which it already should be.
        ldaw r11, cp[vpu_vec_0x80000000]        
    {                                           ;   vstr r2[0]                              }
    {                                           ;   vlmacc r11[0]                           }
        ldaw r11, cp[vpu_vec_zero]        
    {   ldaw r11, sp[STACK_VEC_TMP]             ;   vldr r11[0]                             }
    {                                           ;   vlmaccr r11[0]                          }
    {                                           ;   vstd r11[0]                             }
    {                                           ;   vlmaccr r11[0]                          }
    {                                           ;   vstr r11[0]                             }
    {                                           ;   ldw r1, r11[0]                          }
    {   add r1, r1, 8                           ;   ldw r0, r11[1]                          }

        ldd r4, r5, sp[0]
        retsp NSTACKWORDS

.L_fend: 
.cc_bottom FUNCTION_NAME.function

.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords; 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores; 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers; 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends; 
.size FUNCTION_NAME, .L_fend - FUNCTION_NAME






#endif //defined(__XS3A__)