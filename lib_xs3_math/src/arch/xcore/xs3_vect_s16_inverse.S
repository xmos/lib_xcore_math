// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)
#ifndef XS3_MATH_NO_ASM

#include "asm_helper.h"

/*  

void xs3_vect_s16_inverse(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const unsigned scale);

*/

#define NSTACKWORDS     (0)

#define FUNCTION_NAME   xs3_vect_s16_inverse

#define a               r0
#define b               r1
#define length          r2
#define scale           r3

.text
.issue_mode dual
.align 4

.cc_top FUNCTION_NAME.function,FUNCTION_NAME

FUNCTION_NAME:
{   mkmsk scale, scale                      ;  dualentsp NSTACKWORDS                    }
{   add scale, scale, 1                     ;                                           }
{   sub length, length, 1                   ;   bf length, .L_finish                    }

.L_loop_top:
    {                                           ;   ld16s r11, b[length]                    }
        divs r11, scale, r11
        st16 r11, a[length]
    {   sub length, length, 1                   ;   bt length, .L_loop_top                  }
    
.L_finish:
    retsp NSTACKWORDS

.L_func_end:
.cc_bottom FUNCTION_NAME.function


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME

#endif //!defined(XS3_MATH_NO_ASM)
#endif //defined(__XS3A__)



