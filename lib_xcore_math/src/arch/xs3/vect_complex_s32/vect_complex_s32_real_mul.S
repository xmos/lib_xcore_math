// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)

#include "../asm_helper.h"

/*  

headroom_t vect_complex_s32_real_mul(
    complex_s32_t* a,
    const complex_s32_t* b,
    const int32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);

*/

.text
.issue_mode dual
.align 4

#define FUNCTION_NAME vect_complex_s32_real_mul

#define NSTACKVECS      (2)
#define NSTACKWORDS     (10+8*NSTACKVECS)

#define STACK_B_SHR     (NSTACKWORDS+1)
#define STACK_C_SHR     (NSTACKWORDS+2)

#define STACK_VEC_TMP   (NSTACKWORDS-8)
#define STACK_VEC_C1    (NSTACKWORDS-16)


#define STACK_TAIL_LEN  (8)
    

#define a           r0 
#define b           r1 
#define c           r2
#define length      r3
#define b_shr       r4
#define c_shr       r5
#define _32         r6
#define vec_tmp     r7
#define vec_c1      r8
#define tmpA        r9
#define tmpB        r10



.cc_top FUNCTION_NAME.function,FUNCTION_NAME
FUNCTION_NAME:
    dualentsp NSTACKWORDS
    std r4, r5, sp[1]
    std r6, r7, sp[2]
    std r8, r9, sp[3]
    {   ldc r11, 0                              ;   stw r10, sp[1]                          }
    {   mov r11, length                         ;   vsetc r11                               }
    {   shr length, length, 2                   ;   ldw b_shr, sp[STACK_B_SHR]              }
    {   zext r11, 2                             ;   ldw c_shr, sp[STACK_C_SHR]              }
    
    {   ldaw vec_tmp, sp[STACK_VEC_TMP]         ;   ldaw vec_c1, sp[STACK_VEC_C1]           }
    {   mkmsk r11, 32                           ;   stw r11, sp[STACK_TAIL_LEN]             }
    {   ldc _32, 32                             ;   bf length, .L_loop_bot                  }
    

    .L_loop_top:
    
            vlashr c[0], c_shr 
            vstrpv vec_tmp[0], r11
            ldd tmpB, tmpA, sp[(STACK_VEC_TMP/2) + 0]
            std tmpA, tmpA, sp[(STACK_VEC_C1/2)  + 0]
            std tmpB, tmpB, sp[(STACK_VEC_C1/2)  + 1]
            ldd tmpB, tmpA, sp[(STACK_VEC_TMP/2) + 1]
        {   add c, c, 8                             ;   sub length, length, 1                   }
            std tmpA, tmpA, sp[(STACK_VEC_C1/2) + 2]
            std tmpB, tmpB, sp[(STACK_VEC_C1/2) + 3]

            vlashr b[0], b_shr
        {   add b, b, _32                           ;   vlmul vec_c1[0]                         }
        {   add a, a, _32                           ;   vstr a[0]                               }
        {   add c, c, 8                             ;   bt length, .L_loop_top                  }

.L_loop_bot:

    {   ldaw r11, sp[STACK_VEC_C1]              ;   ldw length, sp[STACK_TAIL_LEN]          }
    {   shl length, length, 3                   ;   bf length, .L_done                      }
    {   mkmsk length, length                    ;   vclrdr                                  }
    {                                           ;   vstd vec_tmp[0]                         }
        vlashr c[0], c_shr
        vstrpv vec_tmp[0], length
        ldd tmpB, tmpA, sp[(STACK_VEC_TMP / 2) + 0]
        std tmpA, tmpA, sp[(STACK_VEC_C1  / 2) + 0]
        std tmpB, tmpB, sp[(STACK_VEC_C1  / 2) + 1]
        ldd tmpB, tmpA, sp[(STACK_VEC_TMP / 2) + 1]
        std tmpA, tmpA, sp[(STACK_VEC_C1  / 2) + 2]
        std tmpB, tmpB, sp[(STACK_VEC_C1  / 2) + 3]
        vlashr b[0], b_shr
    {                                           ;   vlmul r11[0]                            }
        vstrpv a[0], length
        vstrpv vec_tmp[0], length
    {                                           ;   vldd vec_tmp[0]                         }
    {                                           ;   vstd vec_tmp[0]                         }


.L_done:
        ldd r4, r5, sp[1]
        ldd r6, r7, sp[2]
        ldd r8, r9, sp[3]

    {   ldc r0, 31                              ;   vgetc r11                               }
    {   zext r11, 5                             ;   ldw r10, sp[1]                          }
    {   sub r0, r0, r11                         ;   retsp NSTACKWORDS                       }


.L_func_end:
.cc_bottom FUNCTION_NAME.function

.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME

#undef FUNCTION_NAME



#endif //defined(__XS3A__)



