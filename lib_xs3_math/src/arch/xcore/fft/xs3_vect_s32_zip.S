// Copyright 2021 XMOS LIMITED. This Software is subject to the terms of the 
// XMOS Public License: Version 1

#if defined(__XS3A__)
#ifndef XS3_MATH_NO_ASM

#include "../asm_helper.h"

/*  
    void xs3_vect_s32_zip(
        complex_s32_t a[],
        const int32_t b[],
        const int32_t c[],
        const unsigned length,
        const right_shift_t b_shr,
        const right_shift_t c_shr);
*/


#define NSTACKWORDS     (8+2*8)

#define FUNCTION_NAME   xs3_vect_s32_zip

#define STACK_B_SHR     (NSTACKWORDS+1)
#define STACK_C_SHR     (NSTACKWORDS+2)
#define STACK_VEC_C     (NSTACKWORDS-8)
#define STACK_VEC_B     (NSTACKWORDS-16)


#define a         r0
#define b         r1
#define c         r2
#define len       r3
#define b_shr     r4
#define c_shr     r5

#define vec_B     r6
#define vec_C     r7
#define tmpB      r8
#define tmpC      r9
#define _64       r10

.text
.issue_mode dual
.align 4

.cc_top FUNCTION_NAME.function,FUNCTION_NAME

FUNCTION_NAME:
    dualentsp NSTACKWORDS
    std r4, r5, sp[0]
    std r6, r7, sp[1]
    std r8, r9, sp[2]
  { ldc r11, 0                                ; stw r10, sp[6]                            }
  { shl r11, len, SIZEOF_LOG2_S32             ; vsetc r11                                 }
  { zext r11, 5                               ; shr len, len, EPV_LOG2_S32                }
  { ldaw vec_B, sp[STACK_VEC_B]               ; ldw b_shr, sp[STACK_B_SHR]                }
  { ldaw vec_C, sp[STACK_VEC_C]               ; ldw c_shr, sp[STACK_C_SHR]                }
  { ldc _64, 32                               ; stw r11, sp[7]                            }
  { shl _64, _64, 1                           ; bf len, .L_loop_bot                       }

  .L_loop_top:
      vlashr b[0], b_shr
    { sub len, len, 1                           ; vstr vec_B[0]                             }
      vlashr c[0], c_shr
    { ldc r11, 7                                ; vstr vec_C[0]                             }

    // Loop can be unrolled up to 4x for modest speed-up
    .L_inner_loop_top:
      { add b, b, 4                               ; ldw tmpB, vec_B[r11]                      }
      { add c, c, 4                               ; ldw tmpC, vec_C[r11]                      }
        std tmpC, tmpB, a[r11]
      { sub r11, r11, 1                           ; bt r11, .L_inner_loop_top                 }
    .L_inner_loop_bot: 
    { add a, a, _64                             ; bt len, .L_loop_top                       }
  .L_loop_bot:
  
  {                                           ; ldw len, sp[7]                              }
  { shr len, len, SIZEOF_LOG2_S32             ; bf len, .L_finish                           }
    vlashr b[0], b_shr
  {                                           ; vstr vec_B[0]                               }
    vlashr c[0], c_shr                    
  { sub len, len, 1                           ; vstr vec_C[0]                               }

  .L_tail_loop_top:
    {                                           ; ldw tmpB, vec_B[len]                        }
    {                                           ; ldw tmpC, vec_C[len]                        }
      std tmpC, tmpB, a[len]
    { sub len, len, 1                           ; bt len, .L_tail_loop_top                    }
  .L_tail_loop_bot:

  .L_finish:
      ldd r4, r5, sp[0]
      ldd r6, r7, sp[1]
      ldd r8, r9, sp[2]
    {                                           ; ldw r10, sp[6]                            }
    {                                           ; retsp NSTACKWORDS                         } 


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



