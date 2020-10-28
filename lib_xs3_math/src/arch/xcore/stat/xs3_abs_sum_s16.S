
#if defined(__XS3A__)
#ifndef XS3_MATH_NO_ASM

/*  
int32_t xs3_abs_sum_s16(
    const int16_t* b,
    const unsigned length);
*/


#include "asm_helper.h"

.text
.issue_mode dual

#define NSTACKVECS      (2)
#define NSTACKWORDS     (8+8*NSTACKVECS)

#define FUNCTION_NAME       xs3_abs_sum_s16

#define STACK_VEC_TMP       (NSTACKWORDS-8)
#define STACK_VEC_VR        (NSTACKWORDS-16)

#define b           r0      // ![0x%08X]
#define N           r1      // ![%d]
#define tail        r2      // ![%d]
#define tmp         r3      // ![%d]

.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.align 16;
.cc_top FUNCTION_NAME.function,FUNCTION_NAME

FUNCTION_NAME:
        dualentsp NSTACKWORDS
        std r4, r5, sp[1]
        ldc r11, 0x0100

    {   shl tail, N, SIZEOF_LOG2_S16            ;   vsetc r11                               }
    {   zext tail, 5                            ;   vclrdr                                  }
    {   shr N, N, EPV_LOG2_S16                  ;   mkmsk tail, tail                        }
        ldaw r11, cp[vpu_vec_0x0001]
    {   shl tmp, N, 5                           ;   vldc r11[0]                             }
    {   add r11, b, tmp                         ;   bf tail, .L_tail_dealt_with             }

    {   ldaw tmp, sp[STACK_VEC_TMP]             ;   vldr r11[0]                             }
    {                                           ;   vstr tmp[0]                             }
    {                                           ;   vsign                                   }
    {                                           ;   vlmul tmp[0]                            }
    {                                           ;   vstd tmp[0]                             }
        vstrpv tmp[0], tail
    {                                           ;   vclrdr                                  }
    {                                           ;   vlmacc tmp[0]                           }

.L_tail_dealt_with:
    {   ldaw tmp, sp[STACK_VEC_TMP]             ;                                           }
    {   ldaw r11, sp[STACK_VEC_VR]              ;   bf N, .L_loop_bot                       }
.L_loop_top:
        {   mov r11, b                              ;   vstr r11[0]                             }
        {                                           ;   vldr r11[0]                             }
        {   ldc r11, 32                             ;   vstr tmp[0]                             }
        {   add b, b, r11                           ;   vsign                                   }
        {                                           ;   vlmul tmp[0]                            }
        {   ldaw r11, sp[STACK_VEC_VR]              ;   vstr tmp[0]                             }
        {                                           ;   vldr r11[0]                             }
        {   sub N, N, 1                             ;   vlmacc tmp[0]                           }
        {                                           ;   bt N, .L_loop_top                       }
.L_loop_bot:


.L_finish:

        ldd r4, r5, sp[1]
    {   ldaw r1, sp[STACK_VEC_TMP]              ;   vadddr                                  }
    {                                           ;   vstd r1[0]                              }
    {                                           ;   ldw r11, sp[STACK_VEC_TMP]              }
    {   shl r11, r11, 16                        ;   vstr r1[0]                              }
    {                                           ;   ldw r1, sp[STACK_VEC_TMP]               }
    {   or r0, r11, r1                          ;   retsp NSTACKWORDS                       }

.cc_bottom FUNCTION_NAME.function; 
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords; 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores; 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers; 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends; 
.L_end: 
    .size FUNCTION_NAME, .L_end - FUNCTION_NAME




#endif //!defined(XS3_MATH_NO_ASM)
#endif //defined(__XS3A__)