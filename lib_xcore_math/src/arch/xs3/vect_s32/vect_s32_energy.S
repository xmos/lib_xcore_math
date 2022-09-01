// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


/*  
int64_t vect_s32_energy(
    const int32_t b[],
    const unsigned length,
    const right_shift_t b_shr);
*/


#include "../asm_helper.h"

.text
.issue_mode dual
.align 4

#define NSTACKVECS      (2)
#define NSTACKWORDS     (8 + 8*NSTACKVECS)


#define FUNCTION_NAME   vect_s32_energy

#define STACK_VEC_TMP       (NSTACKWORDS-8)
#define STACK_VEC_VR        (NSTACKWORDS-16)

#define b           r0
#define N           r1
#define b_shr       r2
#define vec_tmp     r3
#define tail        r4

.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.align 16;
.cc_top FUNCTION_NAME.function,FUNCTION_NAME

FUNCTION_NAME:

        dualentsp NSTACKWORDS
        std r4, r5, sp[0]

    {   ldc r11, 0                              ;   ldaw vec_tmp, sp[STACK_VEC_TMP]         }
    {   shl tail, N, SIZEOF_LOG2_S32            ;   vsetc r11                               }
    {   zext tail, 5                            ;   vclrdr                                  }
    {   shr N, N, EPV_LOG2_S32                  ;   bf tail, .L_tail_dealt_with_s32         }

    {                                           ;   shl N, N, 5                             }
    {   add r11, b, N                           ;   vstd vec_tmp[0]                         }
    {   mkmsk tail, tail                        ;                                           }
        vlashr r11[0], b_shr
        vstrpv vec_tmp[0], tail
#undef tail

    {                                           ;   vldc vec_tmp[0]                         }
    {                                           ;   vclrdr                                  }
    {   shr N, N, 5                             ;   vlmacc vec_tmp[0]                       }

.L_tail_dealt_with_s32:
    {   ldaw r11, sp[STACK_VEC_VR]              ;   bf N, .L_loop_bot_s32                   }


.L_loop_top_s32:
        {   ldc r11, 32                             ;   vstr r11[0]                             }
            vlashr b[0], b_shr
        {   add b, b, r11                           ;   vstr vec_tmp[0]                         }
        {   ldaw r11, sp[STACK_VEC_VR]              ;   vldc vec_tmp[0]                         }
        {                                           ;   vldr r11[0]                             }
        {   sub N, N, 1                             ;   vlmacc vec_tmp[0]                       }
        {                                           ;   bt N, .L_loop_top_s32                   }
.L_loop_bot_s32:

.L_finish_s32:


        ldaw r11, cp[vpu_vec_0x40000000]
    {   ldaw r2, sp[STACK_VEC_TMP]              ;   vldc r11[0]                             }
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


.cc_bottom FUNCTION_NAME.function; 
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords; 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores; 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers; 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends; 
.L_end: 
    .size FUNCTION_NAME, .L_end - FUNCTION_NAME






#endif //defined(__XS3A__)