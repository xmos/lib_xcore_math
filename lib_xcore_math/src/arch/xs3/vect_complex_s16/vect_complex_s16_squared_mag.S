// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


#include "../asm_helper.h"

/*  

headroom_t vect_complex_s16_squared_mag(
    int16_t a[],
    const int16_t* b_real,
    const int16_t* b_imag,
    const unsigned length,
    const right_shift_t sat);

*/

.text
.issue_mode dual
.align 4

#define NSTACKVECS      (2)
#define NSTACKWORDS     (6+8*(NSTACKVECS))


#define STACK_SAT       (NSTACKWORDS+1)

#define STACK_VEC_TMP   (NSTACKWORDS-8)
#define STACK_VEC_SAT   (NSTACKWORDS-16)

#define FUNCTION_NAME vect_complex_s16_squared_mag
    
#define a           r0
#define b_re        r1
#define b_im        r2
#define length      r3
#define vec_tmp     r4
#define vec_sat     r5
#define bytemask    r6
#define _32         r7


.cc_top FUNCTION_NAME.function,FUNCTION_NAME
FUNCTION_NAME:
    dualentsp NSTACKWORDS
    std r4, r5, sp[1]
    std r6, r7, sp[2]

    {   ldaw r11, sp[STACK_VEC_SAT]             ;   ldw r4, sp[STACK_SAT]                   }
    {   shl r5, r4, 16                          ;   zext r4, 16                             }
    {   or r4, r4, r5                           ;   ldc _32, 32                             }
        std r4, r4, r11[0]
        std r4, r4, r11[1]
        std r4, r4, r11[2]
        std r4, r4, r11[3]

    {                                           ;   shl bytemask, length, SIZEOF_LOG2_S16   }
    {   shr length, length, 4                   ;   shl r11, _32, 3                         }
    {   zext bytemask, 5                        ;   vsetc r11                               }
    {   ldaw r11, sp[STACK_VEC_SAT]             ;   ldaw vec_tmp, sp[STACK_VEC_TMP]         }
    {   mkmsk bytemask, bytemask                ;   bf length, .L_loop_bot                  }

    .L_loop_top:

        {   sub length, length, 1                   ;   vclrdr                                  }
        {                                           ;   vldc b_re[0]                            }
        {   add b_re, b_re, _32                     ;   vlmacc b_re[0]                          }
        {                                           ;   vldc b_im[0]                            }
        {   add b_im, b_im, _32                     ;   vlmacc b_im[0]                          }
        {                                           ;   vlsat r11[0]                            }
        {   add a, a, _32                           ;   vstr a[0]                               }
        {                                           ;   bt length, .L_loop_top                  }

    .L_loop_bot: //astew: worth jumping over to single issue mode for this?

    {                                           ;   bf bytemask, .L_done                    }
    {                                           ;   vclrdr                                  }
    {                                           ;   vstd vec_tmp[0]                         }
    {                                           ;   vldc b_re[0]                            }
    {                                           ;   vlmacc b_re[0]                          }
    {                                           ;   vldc b_im[0]                            }
    {                                           ;   vlmacc b_im[0]                          }
    {                                           ;   vlsat r11[0]                            }
    {                                           ;   vstrpv a[0], bytemask                   }
    {                                           ;   vstrpv vec_tmp[0], bytemask             }
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



