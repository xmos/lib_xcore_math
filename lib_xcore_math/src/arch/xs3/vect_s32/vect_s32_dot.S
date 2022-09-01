// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


/*  
int64_t vect_s32_dot(
    const int32_t b[],
    const int32_t c[],
    const unsigned length,
    const int b_shr,
    const int c_shr);
*/

#include "../asm_helper.h"

#define NSTACKWORDS     (8 + 24)

#define FUNCTION_NAME   vect_s32_dot

#define STACK_C_SHR     (NSTACKWORDS+1)

#define STACK_VEC_TMP   (NSTACKWORDS-8)
#define STACK_VEC_VR    (NSTACKWORDS-16)
#define STACK_VEC_VD    (NSTACKWORDS-24)

#define b           r0
#define c           r1
#define N           r2
#define b_shr       r3
#define c_shr       r4
#define tail        r5
#define vec_vd      r6
#define vec_vr      r7
#define vec_tmp     r8
#define _32         r9



.text; .issue_mode dual
.align 4

.cc_top FUNCTION_NAME.function,FUNCTION_NAME

FUNCTION_NAME:

        dualentsp NSTACKWORDS
        std r4, r5, sp[0]
        std r6, r7, sp[1]
        std r8, r9, sp[2]

    {   ldc r11, 0                              ;                                           }
    {   shl tail, N, SIZEOF_LOG2_S32            ;   vsetc r11                               }
    {   zext tail, 5                            ;   vclrdr                                  }
    {   shr N, N, EPV_LOG2_S32                  ;   ldaw vec_tmp, sp[STACK_VEC_TMP]         }
    {                                           ;   ldw c_shr, sp[STACK_C_SHR]              }

    {   ldaw vec_vr, sp[STACK_VEC_VD + 1]       ;   ldaw vec_vd, sp[STACK_VEC_VD]           }
    {   ldc _32, 32                             ;   bf N, .L_loop_bot_s32                   }

.L_loop_top_s32:
        {   add vec_vd, vec_vd, _32                 ;   vstd vec_vd[0]                          }
        {   sub vec_vd, vec_vd, _32                 ;   vstr vec_vd[0]                          }
            vlashr b[0], b_shr
        {   add b, b, _32                           ;   vstr vec_tmp[0]                         }
        {                                           ;   vldc vec_tmp[0]                         }
            vlashr c[0], c_shr
        {   mov r11, vec_vr                         ;   vstr vec_tmp[0]                         }
        {   sub vec_vr, vec_vr, _32                 ;   vldr r11[0]                             }
        {   add vec_vr, vec_vr, _32                 ;   vldd vec_vr[0]                          }  
        {   sub N, N, 1                             ;   vlmaccr vec_tmp[0]                      }
        {   add c, c, _32                           ;   bt N, .L_loop_top_s32                   }
.L_loop_bot_s32:
    {   mkmsk tail, tail                        ;   bf tail, .L_finish_s32                  }
    {   add vec_vd, vec_vd, _32                 ;   vstd vec_vd[0]                          }
    {   sub vec_vd, vec_vd, _32                 ;   vstr vec_vd[0]                          }
    {                                           ;   vclrdr                                  }
        vlashr b[0], b_shr
    {   add b, b, _32                           ;   vstd vec_tmp[0]                         }
        vstrpv vec_tmp[0], tail
    {                                           ;   vldc vec_tmp[0]                         }
        vlashr c[0], c_shr
    {   mov r11, vec_vr                         ;   vstr vec_tmp[0]                         }
    {   sub vec_vr, vec_vr, _32                 ;   vldr r11[0]                             }
    {                                           ;   vldd vec_vr[0]                          }  
    {                                           ;   vlmaccr vec_tmp[0]                      }

.L_finish_s32:

    {   ldaw r11, sp[STACK_VEC_TMP]             ;                                           }
    // (vD:vR)[k] ==  ((int32_t)vD[k])*(2^32) + ((uint32_t)vR[k])
    {                                           ;   vstd r11[0]                             }
    {                                           ;   ldw r1, r11[0]                          }
    {   sext r1, 8                              ;   vstr r11[0]                             }
    {                                           ;   ldw r0, r11[0]                          }
    
        ldd r4, r5, sp[0]
        ldd r6, r7, sp[1]
        ldd r8, r9, sp[2]
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