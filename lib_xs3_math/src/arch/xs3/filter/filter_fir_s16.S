// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.



#if defined(__XS3A__)

#include "../asm_helper.h"

/*  

typedef struct {
    unsigned num_taps;
    right_shift_t shift;
    int16_t* coef;
    int16_t* state;
} filter_fir_s16_t;

int16_t filter_fir_s16(
    filter_fir_s16_t* filter,
    const int16_t new_sample);
*/

#define FUNCTION_NAME filter_fir_s16

#define NSTACKVECS      (2)
#define NSTACKWORDS     (10+8*NSTACKVECS)

#define FILT_N          0
#define FILT_SHIFT      1
#define FILT_COEF       2
#define FILT_STATE      3


#define STACK_VEC_TMP   (NSTACKWORDS-8)
#define STACK_VEC_VR    (NSTACKWORDS-16)

#define STACK_FILTER    (8)

#define buff        r0
#define length      r1
#define sample      r2
#define tmpA        r3
#define _32         r4
#define coef        r5
#define filter      r10
    
.text
.issue_mode dual
.globl FUNCTION_NAME;
.type FUNCTION_NAME,@function
.align 16
.cc_top FUNCTION_NAME.function,FUNCTION_NAME

FUNCTION_NAME:
        dualentsp NSTACKWORDS

        std r4, r5, sp[1]
        std r6, r7, sp[2]
        std r8, r9, sp[3]
    {   ldc _32, 32                             ;   stw r10, sp[1]                          }
    {   mov filter, r0                          ;   mov sample, r1                          }
    {                                           ;   ldw length, filter[FILT_N]              }
    {                                           ;   ldw buff, filter[FILT_STATE]            }
        bl filter_fir_s16_push_sample_up
    {                                           ;   ldw coef, filter[FILT_COEF]             }
    {                                           ;   ldw buff, filter[FILT_STATE]            }
    {                                           ;   ldw length, filter[FILT_N]              }
    {   shl r11, _32, 3                         ;   vclrdr                                  }
    {   ldaw r11, sp[STACK_VEC_TMP]             ;   vsetc r11                               }
    {   mov tmpA, length                        ;   vstd r11[0]                             }
    {   zext tmpA, 4                            ;   shr length, length, 4                   }
    {   shl tmpA, tmpA, 1                       ;   bf length, .L_loop_end                  }
    
    .L_loop_top:
        {   add buff, buff, _32                     ;   vldc buff[0]                            }
        {   sub length, length, 1                   ;   vlmaccr coef[0]                         }
        {   add coef, coef, _32                     ;   bt length, .L_loop_top                  }
    .L_loop_end:

    {   ldaw r2, sp[STACK_VEC_VR]               ;   mkmsk tmpA, tmpA                            }
    {   mov r11, buff                           ;   vstr r2[0]                                  }
    {   ldaw r11, sp[STACK_VEC_TMP]             ;   vldr r11[0]                                 }
        vstrpv r11[0], tmpA
    {   ldc tmpA, 0                             ;   ldw r2, filter[FILT_SHIFT]                  }
    {   ldaw r11, sp[STACK_VEC_VR]              ;   vldc r11[0]                                 }
    {                                           ;   vldr r11[0]                                 }
    {                                           ;   vlmaccr coef[0]                             }
    {                                           ;   vadddr                                      }
        st16 r2, r11[tmpA]
    {                                           ;   vlsat r11[0]                                }
    {                                           ;   vstr r11[0]                                 }
    {                                           ;   ld16s r0, r11[tmpA]                         }

.L_done:
    {                                           ;   ldw r10, sp[1]                          }
        ldd r8, r9, sp[3]
        ldd r6, r7, sp[2]
        ldd r4, r5, sp[1]
        retsp NSTACKWORDS

.cc_bottom FUNCTION_NAME.function; 
.set FUNCTION_NAME.nstackwords,NSTACKWORDS + filter_fir_s16_push_sample_up.nstackwords;     
.global FUNCTION_NAME.nstackwords; 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores; 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers; 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends; 
.L_size_end:
    .size FUNCTION_NAME, .L_size_end - FUNCTION_NAME

#undef FUNCTION_NAME



#endif //defined(__XS3A__)



