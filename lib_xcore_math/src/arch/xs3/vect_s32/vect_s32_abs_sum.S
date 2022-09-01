// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


/*  

int64_t vect_s32_abs_sum(
    const int32_t b[],
    const unsigned length);
*/


#include "../asm_helper.h"

.text
.issue_mode dual
.align 4

#define NSTACKVECS      (2)
#define NSTACKWORDS     (8 + 8*NSTACKVECS)

#define FUNCTION_NAME       vect_s32_abs_sum

#define STACK_VEC_TMP       (NSTACKWORDS-8)
#define STACK_VEC_VR        (NSTACKWORDS-16)

#define b           r0      // ![0x%08X]
#define N           r1      // ![%d]
#define tail        r2      // ![0x%X]
#define _32         r3      // ![%d]
#define tmp         r4      // ![%d]
#define mask        r5      // ![0x%X]



.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.align 16;
.cc_top FUNCTION_NAME.function,FUNCTION_NAME

FUNCTION_NAME:

        dualentsp NSTACKWORDS
        std r4, r5, sp[0]

    {   ldc r11, 0                              ;   shl tail, N, SIZEOF_LOG2_S32            }
    {   ldaw r11, sp[STACK_VEC_VR]              ;   vsetc r11                               }
    {   zext tail, 5                            ;   vclrdr                                  }
    {   shr N, N, EPV_LOG2_S32                  ;   bf tail, .L_tail_dealt_with             }

    {   ldaw tmp, sp[STACK_VEC_TMP]             ;   mov r11, b                              }
    {   mkmsk mask, tail                        ;   vstd tmp[0]                             }
    {   ldaw r11, sp[STACK_VEC_VR]              ;   vldr r11[0]                             }
    {                                           ;   vsign                                   }
        vstrpv tmp[0], mask
    {                                           ;   vldc tmp[0]                             }
    {                                           ;   vclrdr                                  }
    {   add b, b, tail                          ;   vlmacc b[0]                             }
.L_tail_dealt_with:
    {   ldc _32, 32                             ;   bf N, .L_loop_bot_s32                   }

.L_loop_top_s32:
        {   mov r11, b                              ;   vstr r11[0]                             }
        {   sub N, N, 1                             ;   vldr r11[0]                             }
        {   ldaw r11, sp[STACK_VEC_TMP]             ;   vsign                                   }
        {                                           ;   vstr r11[0]                             }
        {   ldaw r11, sp[STACK_VEC_VR]              ;   vldc r11[0]                             }
        {                                           ;   vldr r11[0]                             }
        {   add b, b, _32                           ;   vlmacc b[0]                             }
        {                                           ;   bt N, .L_loop_top_s32                   }
.L_loop_bot_s32:

.L_finish_s32:

    {                                           ;   vstr r11[0]                             }
        ldaw r11, cp[vpu_vec_0x40000000]
    {                                           ;   vldc r11[0]                             }
        ldaw r11, cp[vpu_vec_0x80000000]        
    {                                           ;   vlmacc r11[0]                           }
        ldaw r11, cp[vpu_vec_zero]
    {   ldaw r11, sp[STACK_VEC_VR]              ;   vldr r11[0]                             }
    {                                           ;   vlmaccr r11[0]                          }
    {                                           ;   vstd r11[0]                             }
    {                                           ;   vlmaccr r11[0]                          }
    {                                           ;   vstr r11[0]                             }
    {                                           ;   ldw r1, r11[0]                          }
    {   add r1, r1, 8                           ;   ldw r0, r11[1]                          }

        ldd r4, r5, sp[0]
        retsp NSTACKWORDS


.cc_bottom FUNCTION_NAME.function; 
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords; 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores; 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers; 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends; 
.L_end: 
    .size FUNCTION_NAME, .L_end - FUNCTION_NAME






#endif //defined(__XS3A__)