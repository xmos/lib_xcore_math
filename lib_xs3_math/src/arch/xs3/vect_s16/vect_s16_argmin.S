// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


/*  

unsigned vect_s16_argmin(
    const int16_t b[],
    const unsigned length);
*/


#include "../asm_helper.h"

.text
.issue_mode dual
.align 4

#define NSTACKVECS      (3)
#define NSTACKWORDS     (8 + 8*NSTACKVECS)

#define FUNCTION_NAME       vect_s16_argmin

#define STACK_VEC_MAX_DEX   (NSTACKWORDS-8)
#define STACK_VEC_CUR_MAX   (NSTACKWORDS-16)
#define STACK_VEC_CUR_DEX   (NSTACKWORDS-24)

#define STACK_N     6

#define b           r0      // ![0x%08X]
#define N           r1      // ![%d]
#define vec_16s     r2      // ![0x%X]
#define tmp         r3      // ![%d]
#define tmz         r4      // ![%d]
#define cur_min     r5      // ![0x%08X]
#define mask_0xF    r6      // ![0x%04X]
#define vec_ones    r7      // ![0x%08X]


.cc_top FUNCTION_NAME.function,FUNCTION_NAME
FUNCTION_NAME:

    dualentsp NSTACKWORDS
    std r4, r5, sp[1]
    std r6, r7, sp[2]

    ldc r11, 0x100
{   mkmsk mask_0xF, 4                       ;   stw N, sp[STACK_N]                      }
{   shr N, N, 4                             ;   vsetc r11                               }

// cur_min[i] = 0x7FFF
    ldaw r11, cp[vpu_vec_0x7FFF]
{   ldaw r11, sp[STACK_VEC_CUR_MAX]         ;   vldr r11[0]                             }
{                                           ;   vstr r11[0]                             }

// cur_dex[i] = i
{   ldaw tmp, sp[STACK_VEC_CUR_DEX]         ;   ldc r11, 15                             }
.L_setup_cur_dex:
        st16 r11, tmp[r11]
    {   sub r11, r11, 1                     ;   bt r11, .L_setup_cur_dex                    }

// min_dex[i] = -1
    ldaw r11, cp[vpu_vec_neg_1]
{   ldaw r11, sp[STACK_VEC_MAX_DEX]         ;   vldr r11[0]                             }
{   ldaw cur_min, sp[STACK_VEC_CUR_MAX]     ;   vstr r11[0]                             }

    ldaw r11, cp[vpu_vec_0x0010]
{   mov vec_16s, r11                        ;   vclrdr                                  }
    ldaw r11, cp[vpu_vec_0x0001]
{   mov vec_ones, r11                       ;                                           }
{   mov r11, b                              ;   bf N, .L_loop_bot                       }


/*

    vR[k] = b[k]
    vR[k] = cur_min[k] - b[k]
    vR[k] = (cur_min[k] - b[k]) < 0 = cur_min[k] < b[k]

    vR[k] = !vR[k] = !(cur_min[k] < b[k]) = (cur_min[k] >= b[k])

    So, this will replace the indexes if b[k] is equal to cur_min[k]

    Instead, we want:
    
    !vR[k] = (cur_min[k] > b[k])
           = cur_min[k] >= b[k] + 1

    vR[k] = !(cur_min[k] >= b[k] + 1)
    vR[k] = cur_min[k] < b[k] + 1
    vR[k] = cur_min[k] - (b[k]+1) < 0
    vR[k] = cur_min[k] - (b[k] + 1)



*/


.L_loop_top:
    {   mov b, r11                              ;   vldr r11[0]                             }
    {                                           ;   vladd vec_ones[0]                       }
    {   sub N, N, 1                             ;   vlsub cur_min[0]                        }
    {   ldaw r11, sp[0]                         ;   vdepth1                                 }
        vstrpv r11[0], mask_0xF
    {   mov r11, b                              ;   ldw tmp, sp[0]                          }
    {   not tmz, tmp                            ;   not tmp, tmp                            }
        zip tmz, tmp, 0                                                                      
    {   ldaw r11, sp[STACK_VEC_CUR_DEX]         ;   vldr r11[0]                             }
        vstrpv cur_min[0], tmp                                                                
    {   ldaw tmz, sp[STACK_VEC_MAX_DEX]         ;   vldr r11[0]                             } 
        vstrpv tmz[0], tmp                                                                    
    {                                           ;   vladd vec_16s[0]                        }
    {   ldc r11, 32                             ;   vstr r11[0]                             } 
    {   add r11, b, r11                         ;   bt N, .L_loop_top                       }
.L_loop_bot:

{                                           ;   ldw N, sp[STACK_N]                      }
{   zext N, 4                               ;                                           }
{   mkmsk N, N                              ;   bf N, .L_no_tail                        }
{   mov b, r11                              ;   vldr r11[0]                             }
{                                           ;   vladd vec_ones[0]                       }
{   ldaw r11, sp[0]                         ;   vlsub cur_min[0]                        }
{                                           ;   vdepth1                                 }
    vstrpv r11[0], mask_0xF
{   mov r11, b                              ;   ldw tmp, sp[0]                          }
{   not tmp, tmp                            ;                                           }
{   and tmp, tmp, N                         ;   and tmz, tmp, N                         }
    zip tmz, tmp, 0
{   ldaw r11, sp[STACK_VEC_CUR_DEX]         ;   vldr r11[0]                             }
    vstrpv cur_min[0], tmp
{   ldaw tmz, sp[STACK_VEC_MAX_DEX]         ;   vldr r11[0]                             }
    vstrpv tmz[0], tmp      

.L_no_tail:

#undef cur_min
#undef vec_16s
#undef mask_0xF

#define cur_min     r5  // ![%d]
#define min_dex     r2  // ![0x%08X]


{   ldaw r11, sp[STACK_VEC_CUR_MAX]         ;   ldc N, 15                               }
{   ldaw r6, sp[STACK_VEC_MAX_DEX]          ;   ld16s cur_min, r11[N]                   }
{   sub N, N, 1                             ;   ld16s min_dex, r6[N]                    }
.L_loop2_top:
    {                                           ;   ld16s r0, r11[N]                        }
    {   lss tmp, cur_min, r0                    ;   ld16s r7, r6[N]                         }
    {   eq tmz, cur_min, r0                     ;   bt tmp, .L_greater_than                 }
    .L_less_or_equal:
        {    lss tmp, r7, min_dex                   ;   bf tmz, .L_less                         }
        .L_equal:
            {                                           ;   bf tmp, .L_greater_than                 }
        .L_less:
            {   mov cur_min, r0                         ;   mov min_dex, r7                         }

    .L_greater_than:
    {   sub N, N, 1                             ;   bt N, .L_loop2_top                      }

    ldd r6, r7, sp[2]
    ldd r4, r5, sp[1]
{   mov r0, min_dex                         ;   retsp NSTACKWORDS                       }

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