
#if defined(__XS3A__)
#ifndef XS3_MATH_NO_ASM

/*  

unsigned xs3_argmin_s32(
    const int32_t b[],
    const unsigned length);


*/


#include "asm_helper.h"

.text
.issue_mode dual

#define NSTACKVECS      (3)
#define NSTACKWORDS     (6 + 8*NSTACKVECS)

#define FUNCTION_NAME       xs3_argmin_s32

#define STACK_VEC_MAX_DEX   (NSTACKWORDS-8)
#define STACK_VEC_CUR_MAX   (NSTACKWORDS-16)
#define STACK_VEC_CUR_DEX   (NSTACKWORDS-24)

#define STACK_N     4

#define b           r0      // ![0x%08X]
#define N           r1      // ![%d]
#define vec_ones    r2      // ![0x%X]
#define tmp         r3      // ![%d]
#define tmz         r4      // ![%d]
#define cur_min     r5      // ![0x%08X]



.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.align 16;
.cc_top FUNCTION_NAME.function,FUNCTION_NAME


FUNCTION_NAME:

        dualentsp NSTACKWORDS
        std r4, r5, sp[1]
    
    {   ldc r11, 0                              ;   stw N, sp[STACK_N]                      }
    {   shr N, N, 3                             ;   vsetc r11                               }

    // cur_min[i] = 0x7FFFFFFF
    // cur_dex[i] = 0
    // min_dex[i] = -1
        ldaw r11, cp[vpu_vec_0x7FFFFFFF]
    {   ldaw r11, sp[STACK_VEC_CUR_MAX]         ;   vldr r11[0]                             }
    {                                           ;   vstr r11[0]                             }
        ldaw r11, cp[vpu_vec_zero]
    {   ldaw tmp, sp[STACK_VEC_CUR_DEX]         ;   vldr r11[0]                             }
    {                                           ;   vstd tmp[0]                             }
        ldaw r11, cp[vpu_vec_neg_1]
    {   ldaw r11, sp[STACK_VEC_MAX_DEX]         ;   vldr r11[0]                             }
    {   ldaw cur_min, sp[STACK_VEC_CUR_MAX]     ;   vstr r11[0]                             }

        ldaw r11, cp[vpu_vec_0x00000001]
    {   mov vec_ones, r11                       ;   vclrdr                                  }
    {   mov r11, b                              ;   bf N, .L_loop_bot                       }

    .L_loop_top:
        {   mov b, r11                              ;   vldr r11[0]                             }
        {   sub N, N, 1                             ;   vlsub cur_min[0]                        }
        {   ldaw r11, sp[0]                         ;   vdepth1                                 }
        {   mkmsk tmp, 4                            ;                                           }
            vstrpv r11[0], tmp
        {   mov r11, b                              ;   ldw tmp, sp[0]                          }
        {   not tmp, tmp                            ;   not tmz, tmp                            }
            zip tmz, tmp, 0
        {   mov tmz, tmp                            ;                                           }
            zip tmz, tmp, 0
        {   ldaw r11, sp[STACK_VEC_CUR_DEX]         ;   vldr r11[0]                             }
            vstrpv cur_min[0], tmp
        {   ldaw tmz, sp[STACK_VEC_MAX_DEX]         ;   vldr r11[0]                             }
            vstrpv tmz[0], tmp      
        {                                           ;   vladd vec_ones[0]                       }
        {   ldc r11, 32                             ;   vstr r11[0]                             }
        {   add r11, b, r11                         ;   bt N, .L_loop_top                       }
.L_loop_bot:

    {                                           ;   ldw N, sp[STACK_N]                      }
    {   zext N, 3                               ;                                           }
    {   mkmsk N, N                              ;   bf N, .L_no_tail                        }
    {   mov b, r11                              ;   vldr r11[0]                             }
    {   ldaw r11, sp[0]                         ;   vlsub cur_min[0]                        }
    {   mkmsk tmp, 4                            ;   vdepth1                                 }
        vstrpv r11[0], tmp
    {   mov r11, b                              ;   ldw tmp, sp[0]                          }
    {   not tmp, tmp                            ;                                           }
    {   and tmp, tmp, N                         ;   and tmz, tmp, N                         }
        zip tmz, tmp, 0
    {   mov tmz, tmp                            ;                                           }
        zip tmz, tmp, 0
    {   ldaw r11, sp[STACK_VEC_CUR_DEX]         ;   vldr r11[0]                             }
        vstrpv cur_min[0], tmp
    {   ldaw tmz, sp[STACK_VEC_MAX_DEX]         ;   vldr r11[0]                             }
        vstrpv tmz[0], tmp      
    
.L_no_tail:

#undef cur_min
#undef vec_ones

#define cur_min     r5  // ![%d]
#define min_dex     r2  // ![0x%08X]

    {   ldaw r11, sp[STACK_VEC_CUR_MAX]         ;   ldc N, 7                                }
    {   sub N, N, 1                             ;   ldw cur_min, r11[N]                     }
    {   ldc min_dex, 7                          ;                                           }
    .L_loop2_top:
        {                                           ;   ldw r0, r11[N]                          }
        {   lss tmp, cur_min, r0                    ;                                           }
        {                                           ;   bt tmp, .L_not_it                       }
            {   mov cur_min, r0                         ;   mov min_dex, N                          }
        .L_not_it:
        {   sub N, N, 1                             ;   bt N, .L_loop2_top                      }

    {   ldaw r11, sp[STACK_VEC_MAX_DEX]         ;                                           }
    {                                           ;   ldw r0, r11[min_dex]                    }
    {   shl r0, r0, 3                           ;                                           }
        ldd r4, r5, sp[1]
    {   add r0, r0, min_dex                     ;   retsp NSTACKWORDS                       }


.cc_bottom FUNCTION_NAME.function; 
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords; 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores; 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers; 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends; 
.L_end: 
    .size FUNCTION_NAME, .L_end - FUNCTION_NAME





#endif //!defined(XS3_MATH_NO_ASM)
#endif //defined(__XS3A__)