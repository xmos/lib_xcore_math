// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)

#include "../asm_helper.h"

/*  

complex_s16_t vect_complex_s16_sum(
    const int16_t* b_real,
    const int16_t* b_imag,
    const unsigned length,
    const right_shift_t sat);

*/

.text
.issue_mode dual
.align 4

#define NSTACKVECS      (2)
#define NSTACKWORDS     (6+(8*NSTACKVECS))

#define b_real      r0
#define b_imag      r1
#define length      r2
#define sat         r3
#define _32         r4
#define tmp         r5
#define tail_bytes  r6

#define STACK_VEC_ZEROS     (NSTACKWORDS- 8)
#define STACK_VEC_TMP       (NSTACKWORDS-16)

#define STACK_LENGTH        6
#define STACK_SAT           7

#define FUNCTION_NAME vect_complex_s16_sum_OLLD //why did I think I needed to write this in assembly?
    
.cc_top FUNCTION_NAME.function,FUNCTION_NAME
FUNCTION_NAME:

        dualentsp NSTACKWORDS
        std r4, r5, sp[1]
        std r6, r7, sp[2]

        ldc r11, 0x0100
    {   add tmp, r11, 1                         ;   vsetc r11                               }
    {   shr length, length, 4                   ;   shl tail_bytes, length, 1               }
    {                                           ;   zext tail_bytes, 5                      }
    {                                           ;   stw length, sp[STACK_LENGTH]            }
    {                                           ;   stw sat, sp[STACK_SAT]                  }

    {   ldaw r11, sp[STACK_VEC_ZEROS]           ;   vclrdr                                  }
    {   ldaw r11, sp[STACK_VEC_TMP]             ;   vstd r11[0]                             }

        std tmp, tmp, r11[0]
        std tmp, tmp, r11[1]
        std tmp, tmp, r11[2]
        std tmp, tmp, r11[3]

    {   ldc _32, 32                             ;   vldc r11[0]                             }
    {   ldaw r11, sp[STACK_VEC_ZEROS]           ;   bf length, .L_real_bot                  }

    .L_real_top:

        {   sub length, length, 1                   ;   vlmacc b_real[0]                        }
        {   add b_real, b_real, _32                 ;   bt length, .L_real_top                  }

    .L_real_bot:

    {   sub r11, r11, tail_bytes                ;   bf tail_bytes, .L_real_end              }
    {                                           ;   vldc r11[0]                             }
    {   mkmsk tmp, 2                            ;   vlmacc b_imag[0]                        }
    {   ldaw r11, sp[STACK_SAT]                 ;   vadddr                                  }
    {   ldaw r11, sp[1]                         ;   vlsat r11[0]                            }
        vstrpv r11[0], tmp
    {                                           ;   ldw r0, sp[1]                           }

    .L_real_end:
    
    {                                           ;   ldw length, sp[STACK_LENGTH]            }
    {   ldaw r11, sp[STACK_VEC_ZEROS]           ;   vclrdr                                  }


    .L_imag_top:

        {   sub length, length, 1                   ;   vlmacc b_imag[0]                        }
        {   add b_imag, b_imag, _32                 ;   bt length, .L_real_top                  }

    .L_imag_bot:

    {   sub r11, r11, tail_bytes                ;   bf tail_bytes, .L_imag_end              }
    {                                           ;   vldc r11[0]                             }
    {   mkmsk tmp, 2                            ;   vlmacc b_imag[0]                        }
    {   ldaw r11, sp[STACK_SAT]                 ;   vadddr                                  }
    {   ldaw r11, sp[1]                         ;   vlsat r11[0]                            }
        vstrpv r11[0], tmp
    {                                           ;   ldw r0, sp[1]                           }

    .L_imag_end:

.L_done:
        ldd r4, r5, sp[1]
        ldd r6, r7, sp[2]
        retsp NSTACKWORDS

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



