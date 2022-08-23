// Copyright 2021-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// XMOS Public License: Version 1

#if defined(__XS3A__)


#include "../asm_helper.h"

/*  
    void vect_s32_zip(
        complex_s32_t a[],
        const int32_t b[],
        const int32_t c[],
        const unsigned length,
        const right_shift_t b_shr,
        const right_shift_t c_shr);
*/


#define NSTACKWORDS     (8+2*8)

#define FUNCTION_NAME   vect_s32_zip

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
#define _28       r8
#define _32       r9


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
  { ldaw vec_B, sp[STACK_VEC_B]               ; stw r11, sp[7]                            }
  { ldc r11, 2                                ; vclrdr                                    }
  { bitrev r11, r11                           ; vstd vec_B[0]                             }
  { ldaw vec_C, sp[STACK_VEC_C]               ; stw r11, vec_B[0]                         }
  { ldc _32, 32                               ; vldc vec_B[0]                             }
  { ldc _28, 28                               ; ldw b_shr, sp[STACK_B_SHR]                }
  {                                           ; ldw c_shr, sp[STACK_C_SHR]                }
  {                                           ; bf len, .L_loop_bot                       }
  {                                           ; bu .L_loop_top                            }

  .align 16
  .L_loop_top:
      vlashr b[0], b_shr
    { add b, b, _32                             ; vstr vec_B[0]                             }
      vlashr c[0], c_shr
    { add a, a, _32                             ; sub len, len, 1                           }

    { add vec_C, vec_C, _28                     ; vstr vec_C[0]                             }
    { add vec_B, vec_B, _28                     ; vclrdr                                    }
    { sub vec_C, vec_C, 4                       ; vlmaccr vec_C[0]                          }
    { sub vec_B, vec_B, 4                       ; vlmaccr vec_B[0]                          }

    { sub vec_C, vec_C, 4                       ; vlmaccr vec_C[0]                          }
    { sub vec_B, vec_B, 4                       ; vlmaccr vec_B[0]                          }
    { sub vec_C, vec_C, 4                       ; vlmaccr vec_C[0]                          }
    { sub vec_B, vec_B, 4                       ; vlmaccr vec_B[0]                          }
    //FNOP
    { sub vec_C, vec_C, 4                       ; vlmaccr vec_C[0]                          }
    { sub vec_B, vec_B, 4                       ; vlmaccr vec_B[0]                          }
    { sub r11, a, _32                           ; vstr a[0]                                 }
    { add a, a, _32                             ; vclrdr                                    }

    { sub vec_C, vec_C, 4                       ; vlmaccr vec_C[0]                          }
    { sub vec_B, vec_B, 4                       ; vlmaccr vec_B[0]                          }
    { sub vec_C, vec_C, 4                       ; vlmaccr vec_C[0]                          }
    { sub vec_B, vec_B, 4                       ; vlmaccr vec_B[0]                          }
    //FNOP
    { sub vec_C, vec_C, 4                       ; vlmaccr vec_C[0]                          }
    { sub vec_B, vec_B, 4                       ; vlmaccr vec_B[0]                          }
    { sub vec_C, vec_C, 4                       ; vlmaccr vec_C[0]                          }
    { sub vec_B, vec_B, 4                       ; vlmaccr vec_B[0]                          }
    
    { add c, c, _32                             ;                                           }
    { ldaw vec_B, sp[STACK_VEC_B]               ; vstr r11[0]                               }
    { ldaw vec_C, sp[STACK_VEC_C]               ; bt len, .L_loop_top                       }
  .L_loop_bot:
  
  {                                           ; ldw len, sp[7]                              }
  { shr len, len, SIZEOF_LOG2_S32             ; bf len, .L_finish                           }
    vlashr b[0], b_shr
  {                                           ; vstr vec_B[0]                               }
    vlashr c[0], c_shr
  { sub len, len, 1                           ; vstr vec_C[0]                               }

#define tmpB  r8
#define tmpC  r9
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


#endif //defined(__XS3A__)



