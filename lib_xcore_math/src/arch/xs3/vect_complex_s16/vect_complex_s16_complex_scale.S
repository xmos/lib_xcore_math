// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


#include "../asm_helper.h"

/*  

headroom_t vect_complex_s16_scale(
    int16_t* a_real,
    int16_t* a_imag,
    const int16_t* b_real,
    const int16_t* b_imag,
    const int16_t c_real,
    const int16_t c_imag,
    const unsigned length,
    const right_shift_t sat);

*/

#define NSTACKVECS      (4)
#define NSTACKWORDS     (6+8*(NSTACKVECS))


#define STACK_C_REAL    (NSTACKWORDS+1)
#define STACK_C_IMAG    (NSTACKWORDS+2)
#define STACK_LENGTH    (NSTACKWORDS+3)
#define STACK_SAT       (NSTACKWORDS+4)

#define STACK_VEC_SAT       (NSTACKWORDS-8)
#define STACK_VEC_C_REAL    (NSTACKWORDS-16)
#define STACK_VEC_C_IMAG    (NSTACKWORDS-24)
#define STACK_VEC_C_IMAG_N  (NSTACKWORDS-32)

#define FUNCTION_NAME vect_complex_s16_scale
    
#define a_real      r0
#define a_imag      r1
#define b_real      r2
#define b_imag      r3
#define length      r4
#define _32         r5
#define bytemask    r6


.text
.align 4

/*
    We're doing this:

    vR  <- -1
    vR  <- -1 * b.imag
    vC  <- -b.imag  
    acc <- 0
    acc <- vC * c.imag
    vC  <- b.real
    acc <- acc + vC * c.real
    vR  <- acc >> sat
    a.real <- vR

    (vC still has b.real)
    acc <- 0
    acc <- vC * c.imag
    vC  <- b.imag
    acc <- acc + vC * c.real
    vR  <- acc >> sat
    a.imag <- vR
*/

.cc_top FUNCTION_NAME.function,FUNCTION_NAME

FUNCTION_NAME:
    
.issue_mode single

    entsp NSTACKWORDS
    std r4, r5, sp[1]
    std r6, r7, sp[2]

    ldaw r11, sp[STACK_VEC_C_IMAG_N]
    ldw r6, sp[STACK_C_IMAG]
    neg r4, r6
    neg r5, r6
    zip r5, r4, 4

    std r4, r4, r11[0]
    std r4, r4, r11[1]
    std r4, r4, r11[2]
    std r4, r4, r11[3]

    ldaw r11, sp[STACK_VEC_C_IMAG]
    mov r4, r6

    zip r6, r4, 4

    std r4, r4, r11[0]
    std r4, r4, r11[1]
    std r4, r4, r11[2]
    std r4, r4, r11[3]

    ldw r4, sp[STACK_C_REAL]
    mov r5, r4
    zip r5, r4, 4
    
    std r4, r4, r11[4]
    std r4, r4, r11[5]
    std r4, r4, r11[6]
    std r4, r4, r11[7]

    ldw r4, sp[STACK_SAT]
    mov r5, r4
    zip r5, r4, 4
    
    std r4, r4, r11[8]
    std r4, r4, r11[9]
    std r4, r4, r11[10]
    std r4, r4, r11[11]

    dualentsp 0

.issue_mode dual
.align 4

    {   ldc _32, 32                             ;   ldw length, sp[STACK_LENGTH]            }
#define vect_count  length
    {   shr vect_count, length, 4               ;   shl bytemask, length, SIZEOF_LOG2_S16   }
    {   zext bytemask, 5                        ;   shl r11, _32, 3                         }
    {   ldaw r11, sp[STACK_VEC_C_IMAG_N]        ;   vsetc r11                               }
    {                                           ;   bf vect_count, .L_loop_bot              }

    .L_loop_top:
        {                                           ;   vldc r11[0]      /* -c_imag          */ }
        {   sub vect_count, vect_count, 1           ;   vclrdr                                  }
        {   ldaw r11, sp[STACK_VEC_C_REAL]          ;   vlmacc b_imag[0] /* -c_imag * b_imag */ }
        {                                           ;   vldc b_real[0]                          }
        {   ldaw r11, sp[STACK_VEC_SAT]             ;   vlmacc r11[0]    /*  b_real * c_real */ }
        {   add b_real, b_real, _32                 ;   vlsat r11[0]                            }
        {   add a_real, a_real, _32                 ;   vstr a_real[0]                          }
        {   ldaw r11, sp[STACK_VEC_C_IMAG]          ;   vclrdr                                  }
        {                                           ;   vlmacc r11[0]    /* b_real * c_imag */  }
        {   ldaw r11, sp[STACK_VEC_C_REAL]          ;   vldc b_imag[0]                          }
        {   ldaw r11, sp[STACK_VEC_SAT]             ;   vlmacc r11[0]    /* b_imag * c_real */  }
        {   add b_imag, b_imag, _32                 ;   vlsat r11[0]                            }
        {   add a_imag, a_imag, _32                 ;   vstr a_imag[0]                          }
        {   ldaw r11, sp[STACK_VEC_C_IMAG_N]        ;   bt vect_count, .L_loop_top              }
    .L_loop_bot:

    {   mkmsk bytemask, bytemask                ;   bf bytemask, .L_done                    }
    {                                           ;   vldc r11[0]      /*    -c_imag       */ }
    {                                           ;   vclrdr                                  }
    {                                           ;   vstd r11[0]      /* clear tmp vec    */ }
    {   ldaw r11, sp[STACK_VEC_C_REAL]          ;   vlmacc b_imag[0] /* -c_imag * b_imag */ }
#define vec_tmp     b_real
    {   ldaw vec_tmp, sp[STACK_VEC_C_IMAG_N]    ;   vldc b_real[0]                          }
    {   ldaw r11, sp[STACK_VEC_SAT]             ;   vlmacc r11[0]    /*  b_real * c_real */ }
    {                                           ;   vlsat r11[0]                            }
        vstrpv vec_tmp[0], bytemask
        vstrpv a_real[0], bytemask        
    {                                           ;   vldd vec_tmp[0]                         }
    {                                           ;   vstd vec_tmp[0]                         }
    {   ldaw r11, sp[STACK_VEC_C_IMAG]          ;   vclrdr                                  }
    {                                           ;   vlmacc r11[0]    /* b_real * c_imag  */ }
    {   ldaw r11, sp[STACK_VEC_C_REAL]          ;   vldc b_imag[0]                          }
    {   ldaw r11, sp[STACK_VEC_SAT]             ;   vlmacc r11[0]    /* b_imag * c_real  */ }
    {                                           ;   vlsat r11[0]                            }
        vstrpv vec_tmp[0], bytemask
        vstrpv a_imag[0], bytemask
    {                                           ;   vldd vec_tmp[0]                         }
    {                                           ;   vstd vec_tmp[0]                         }



.L_done:
        ldd r4, r5, sp[1]
        ldd r6, r7, sp[2]

    {   ldc r0, 15                              ;   vgetc r11                               }
    {   zext r11, 5                             ;                                           }
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



