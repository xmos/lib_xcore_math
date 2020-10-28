
#if defined(__XS3A__)
#ifndef XS3_MATH_NO_ASM

/*  

unsigned xs3_argmax_s16(
    const int16_t b[],
    const unsigned length);
*/


#include "asm_helper.h"

.text
.issue_mode dual

#define NSTACKVECS      (3)
#define NSTACKWORDS     (6 + 8*NSTACKVECS)

#define FUNCTION_NAME       xs3_argmax_s16

#define STACK_VEC_MAX_DEX   (NSTACKWORDS-8)
#define STACK_VEC_CUR_MAX   (NSTACKWORDS-16)
#define STACK_VEC_CUR_DEX   (NSTACKWORDS-24)

#define STACK_N     4

#define b           r0      // ![0x%08X]
#define N           r1      // ![%d]
#define vec_ones    r2      // ![0x%X]
#define tmp         r3      // ![%d]
#define tmz         r4      // ![%d]
#define cur_max     r5      // ![0x%08X]



.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.align 16;
.cc_top FUNCTION_NAME.function,FUNCTION_NAME


FUNCTION_NAME:

        dualentsp NSTACKWORDS
        std r4, r5, sp[1]
    
        ldc r11, 0x100
    {                                           ;   stw N, sp[STACK_N]                      }
    {   shr N, N, 4                             ;   vsetc r11                               }

    // cur_max[i] = -0x8000
    // cur_dex[i] = 0
    // max_dex[i] = -1
        ldaw r11, cp[vpu_vec_0x8000]
    {   ldaw r11, sp[STACK_VEC_CUR_MAX]         ;   vldr r11[0]                             }
    {                                           ;   vstr r11[0]                             }
        ldaw r11, cp[vpu_vec_zero]
    {   ldaw tmp, sp[STACK_VEC_CUR_DEX]         ;   vldr r11[0]                             }
    {                                           ;   vstd tmp[0]                             }
        ldaw r11, cp[vpu_vec_neg_1]
    {   ldaw r11, sp[STACK_VEC_MAX_DEX]         ;   vldr r11[0]                             }
    {   ldaw cur_max, sp[STACK_VEC_CUR_MAX]     ;   vstr r11[0]                             }

        ldaw r11, cp[vpu_vec_0x0001]
    {   mov vec_ones, r11                       ;   vclrdr                                  }
    {   mov r11, b                              ;   bf N, .L_loop_bot                       }

    .L_loop_top:
        {   mov b, r11                              ;   vldr r11[0]                             }
        {   sub N, N, 1                             ;   vlsub cur_max[0]                        }
        {   ldaw r11, sp[0]                         ;   vdepth1                                 }
        {   mkmsk tmp, 4                            ;                                           }
            vstrpv r11[0], tmp
        {   mov r11, b                              ;   ldw tmp, sp[0]                          }
        {   mov tmz, tmp                            ;                                           }
            zip tmz, tmp, 0
        {   ldaw r11, sp[STACK_VEC_CUR_DEX]         ;   vldr r11[0]                             }
            vstrpv cur_max[0], tmp
        {   ldaw tmz, sp[STACK_VEC_MAX_DEX]         ;   vldr r11[0]                             }
            vstrpv tmz[0], tmp      
        {                                           ;   vladd vec_ones[0]                       }
        {   ldc r11, 32                             ;   vstr r11[0]                             }
        {   add r11, b, r11                         ;   bt N, .L_loop_top                       }
.L_loop_bot:

    {                                           ;   ldw N, sp[STACK_N]                      }
    {   zext N, 4                               ;                                           }
    {   mkmsk N, N                              ;   bf N, .L_no_tail                        }
    {   mov b, r11                              ;   vldr r11[0]                             }
    {   ldaw r11, sp[0]                         ;   vlsub cur_max[0]                        }
    {   mkmsk tmp, 4                            ;   vdepth1                                 }
        vstrpv r11[0], tmp
    {   mov r11, b                              ;   ldw tmp, sp[0]                          }
    {   and tmp, tmp, N                         ;   and tmz, tmp, N                         }
        zip tmz, tmp, 0
    {   ldaw r11, sp[STACK_VEC_CUR_DEX]         ;   vldr r11[0]                             }
        vstrpv cur_max[0], tmp
    {   ldaw tmz, sp[STACK_VEC_MAX_DEX]         ;   vldr r11[0]                             }
        vstrpv tmz[0], tmp      
    
.L_no_tail:

#undef cur_max
#undef vec_ones

#define cur_max     r5  // ![%d]
#define max_dex     r2  // ![0x%08X]

    {   ldaw r11, sp[STACK_VEC_CUR_MAX]         ;   ldc N, 15                               }
    {   sub N, N, 1                             ;   ld16s cur_max, r11[N]                   }
    {   ldc max_dex, 15                         ;                                           }
    .L_loop2_top:
        {                                           ;   ld16s r0, r11[N]                        }
        {   lss tmp, r0, cur_max                    ;                                           }
        {                                           ;   bru tmp                                 }
            {   mov cur_max, r0                         ;   mov max_dex, N                          }
        .L_not_it:
        {   sub N, N, 1                             ;   bt N, .L_loop2_top                      }

    {   ldaw r11, sp[STACK_VEC_MAX_DEX]         ;                                           }
    {                                           ;   ld16s r0, r11[max_dex]                  }
    {   shl r0, r0, 4                           ;                                           }
        ldd r4, r5, sp[1]
    {   add r0, r0, max_dex                     ;   retsp NSTACKWORDS                       }


.cc_bottom FUNCTION_NAME.function; 
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords; 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores; 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers; 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends; 
.L_end: 
    .size FUNCTION_NAME, .L_end - FUNCTION_NAME





#endif //!defined(XS3_MATH_NO_ASM)
#endif //defined(__XS3A__)