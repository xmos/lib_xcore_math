// Copyright 2021 XMOS LIMITED. This Software is subject to the terms of the 
// XMOS Public License: Version 1



#if defined(__XS3A__)
#ifndef XS3_MATH_NO_ASM

/*  
void xs3_vect_s32_to_s16(
    int16_t a[],
    const int32_t b[],
    const unsigned length,
    const int v_shr);
*/

#include "asm_helper.h"

.text
.issue_mode dual
.align 4


#define NSTACKWORDS     (4)

#define FUNCTION_NAME   xs3_vect_s32_to_s16

#define a           r0
#define b           r1
#define len         r2
#define b_shr       r3
#define tail        r4
#define _16         r5


.cc_top FUNCTION_NAME.function,FUNCTION_NAME

FUNCTION_NAME:
        dualentsp NSTACKWORDS
        std r4, r5, sp[0]
    {   ldc r11, 0                              ;   ldc _16, 16                         }
    {   sub b_shr, b_shr, _16                   ;   vsetc r11                           }
    {   shr len, len, EPV_LOG2_S32              ;   shl tail, len, SIZEOF_LOG2_S16      }
#if XS3_MATH_VECTOR_TAIL_SUPPORT
    {                                           ;   zext tail, 4                        }
    {   mkmsk tail, tail                        ;   bf len, .L_loop_bot                 }
#endif //XS3_MATH_VECTOR_TAIL_SUPPORT
    {   mkmsk r11, 16                           ;   bu .L_loop_top                      }

.align 16
.L_loop_top:
        vlashr b[0], b_shr
    {   add b, b, _16                           ;   vdepth16                            }
    {   sub len, len, 1                         ;   add b, b, _16                       }
        vstrpv a[0], r11
    {   add a, a, _16                           ;   bt len, .L_loop_top                 }
.L_loop_bot:

#if XS3_MATH_VECTOR_TAIL_SUPPORT
    {                                           ;   bf tail, .L_finish                  }
    {                                           ;   vclrdr                              }
        vlashr b[0], b_shr
    {                                           ;   vdepth16                            }
        vstrpv a[0], tail
#endif //XS3_MATH_VECTOR_TAIL_SUPPORT

.L_finish:
        ldd r4, r5, sp[0]
    {                                           ;   retsp NSTACKWORDS                   } 

.L_func_end:
.cc_bottom FUNCTION_NAME.function


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS; .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;              .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;             .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;           .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME

#endif //!defined(XS3_MATH_NO_ASM)
#endif //defined(__XS3A__)