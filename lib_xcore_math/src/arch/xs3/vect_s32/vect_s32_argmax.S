// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


/*  

unsigned vect_s32_argmax(
    const int32_t b[],
    const unsigned length);
*/


#include "../asm_helper.h"

.text
.issue_mode dual
.align 4

#define NSTACKVECS      (3)
#define NSTACKWORDS     (8 + 8*NSTACKVECS)

#define FUNCTION_NAME       vect_s32_argmax

#define STACK_VEC_MAX_DEX   (NSTACKWORDS-8)
#define STACK_VEC_CUR_MAX   (NSTACKWORDS-16)
#define STACK_VEC_CUR_DEX   (NSTACKWORDS-24)

#define STACK_N     6

#define b           r0      // ![0x%08X]
#define N           r1      // ![%d]
#define vec_8s      r2      // ![0x%X]
#define tmp         r3      // ![%d]
#define tmz         r4      // ![%d]
#define cur_max     r5      // ![0x%08X]
#define mask_0xF    r6      // ![0x%04X]



.cc_top FUNCTION_NAME.function,FUNCTION_NAME
FUNCTION_NAME:

    dualentsp NSTACKWORDS
    std r4, r5, sp[1]
    std r6, r7, sp[2]

{   ldc r11, 0                              ;   stw N, sp[STACK_N]                      }
{   shr N, N, 3                             ;   vsetc r11                               }

// cur_max[i] = -0x80000000
    ldaw r11, cp[vpu_vec_0x80000000]
{   ldaw r11, sp[STACK_VEC_CUR_MAX]         ;   vldr r11[0]                             }
{   mkmsk mask_0xF, 4                       ;   vstr r11[0]                             }

// cur_dex[i] = i
{   ldaw tmp, sp[STACK_VEC_CUR_DEX]         ;   ldc r11, 7                             }
.L_setup_cur_dex:
    {                                       ;   stw r11, tmp[r11]                           }
    {   sub r11, r11, 1                     ;   bt r11, .L_setup_cur_dex                    }

// max_dex[i] = -1
    ldaw r11, cp[vpu_vec_neg_1]
{   ldaw r11, sp[STACK_VEC_MAX_DEX]         ;   vldr r11[0]                             }
{   ldaw cur_max, sp[STACK_VEC_CUR_MAX]     ;   vstr r11[0]                             }

    ldaw r11, cp[vpu_vec_0x00000008]
{   mov vec_8s, r11                         ;   vclrdr                                  }
{   mov r11, b                              ;   bf N, .L_loop_bot                       }

.L_loop_top:
    {   mov b, r11                              ;   vldr r11[0]                             }
    {   sub N, N, 1                             ;   vlsub cur_max[0]                        }
    {   ldaw r11, sp[0]                         ;   vdepth1                                 }
        vstrpv r11[0], mask_0xF
    {   mov r11, b                              ;   ldw tmp, sp[0]                          }
    {   mov tmz, tmp                            ;                                           }
        zip tmz, tmp, 0
    {   mov tmz, tmp                            ;                                           }
        zip tmz, tmp, 0
    {   ldaw r11, sp[STACK_VEC_CUR_DEX]         ;   vldr r11[0]                             }
        vstrpv cur_max[0], tmp
    {   ldaw tmz, sp[STACK_VEC_MAX_DEX]         ;   vldr r11[0]                             }
        vstrpv tmz[0], tmp      
    {                                           ;   vladd vec_8s[0]                         }
    {   ldc r11, 32                             ;   vstr r11[0]                             }
    {   add r11, b, r11                         ;   bt N, .L_loop_top                       }
.L_loop_bot:

{                                           ;   ldw N, sp[STACK_N]                      }
{   zext N, 3                               ;                                           }
{   mkmsk N, N                              ;   bf N, .L_no_tail                        }
{   mov b, r11                              ;   vldr r11[0]                             }
{   ldaw r11, sp[0]                         ;   vlsub cur_max[0]                        }
{                                           ;   vdepth1                                 }
    vstrpv r11[0], mask_0xF
{   mov r11, b                              ;   ldw tmp, sp[0]                          }
{   and tmp, tmp, N                         ;   and tmz, tmp, N                         }
    zip tmz, tmp, 0
{   mov tmz, tmp                            ;                                           }
    zip tmz, tmp, 0
{   ldaw r11, sp[STACK_VEC_CUR_DEX]         ;   vldr r11[0]                             }
    vstrpv cur_max[0], tmp
{   ldaw tmz, sp[STACK_VEC_MAX_DEX]         ;   vldr r11[0]                             }
    vstrpv tmz[0], tmp      

.L_no_tail:

#undef cur_max
#undef vec_8s
#undef mask_0xF

#define cur_max     r5  // ![%d]
#define max_dex     r2  // ![0x%08X]

{   ldaw r11, sp[STACK_VEC_CUR_MAX]         ;   ldc N, 7                                }
{   ldaw r6, sp[STACK_VEC_MAX_DEX]          ;   ldw cur_max, r11[N]                     }
{   sub N, N, 1                             ;   ldw max_dex, r6[N]                      }
.L_loop2_top:
    {                                           ;   ldw r0, r11[N]                          }
    {   lss tmp, r0, cur_max                    ;   ldw r7, r6[N]                           }
    {   eq tmz, r0, cur_max                     ;   bt tmp, .L_less_than                    }
    .L_greater_or_equal:
        {    lss tmp, r7, max_dex                   ;   bf tmz, .L_greater                      }
        .L_equal:
            {                                           ;   bf tmp, .L_less_than                    }
        .L_greater:
            {   mov cur_max, r0                         ;   mov max_dex, r7                         }

    .L_less_than:
    {   sub N, N, 1                             ;   bt N, .L_loop2_top                      }

    ldd r6, r7, sp[2]
    ldd r4, r5, sp[1]
{   mov r0, max_dex                         ;   retsp NSTACKWORDS                       }


.L_end: 
.cc_bottom FUNCTION_NAME.function


.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME, .L_end - FUNCTION_NAME






#endif //defined(__XS3A__)