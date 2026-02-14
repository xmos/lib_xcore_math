// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

/*  

    headroom_t vect_s16_scale(
        int16_t a[],
        const int16_t b[],
        const unsigned length,
        const int16_t c,
        const right_shift_t a_shr);

*/


#define NSTACKWORDS     (8+8)

#define FUNCTION_NAME   vect_s16_scale

#define STACK_VEC_A_SHR     0
#define STACK_BYTEMASK      10

#define a           a0 
#define b           a1 
#define len         a2
#define c           a3
#define _32         s2
#define tail        s3


.text
.p2align 2

FUNCTION_NAME:
    { mv t3, c                          ; xm.entsp (NSTACKWORDS)*4              }
        xm.stdsp  s3,s2,32

xm.zip t3, c, 4
        xm.stdsp  c,c,((STACK_VEC_A_SHR/2)+0)*8
        xm.stdsp  c,c,((STACK_VEC_A_SHR/2)+1)*8
        xm.stdsp  c,c,((STACK_VEC_A_SHR/2)+2)*8
        xm.stdsp  c,c,((STACK_VEC_A_SHR/2)+3)*8

#undef  c
#define tmp     a3

        li t3, 0x100
    { addi t3,sp, (STACK_VEC_A_SHR)*4   ; xm.vsetc t3                           }
    mv tmp, a4
    { mv t3, tmp                        ; xm.vldc t3                            }
xm.zip t3, tmp, 4
        xm.stdsp  tmp,tmp,((STACK_VEC_A_SHR/2)+0)*8
        xm.stdsp  tmp,tmp,((STACK_VEC_A_SHR/2)+1)*8
        xm.stdsp  tmp,tmp,((STACK_VEC_A_SHR/2)+2)*8
        xm.stdsp  tmp,tmp,((STACK_VEC_A_SHR/2)+3)*8

#undef tmp

    { slli tail, len, SIZEOF_LOG2_S16   ; srli len, len, EPV_LOG2_S16           }
    { xm.zexti tail, 5                  ; li _32, 32                            }


    { addi t3,sp, (STACK_VEC_A_SHR)*4   ; xm.brff len, .L_loop_bot              }
    { nop                               ; xm.bu .L_loop_top                     }

.p2align 4
.L_loop_top:
        { addi len, len, -1             ; xm.vclrdr                             }
        { nop                           ; xm.vlmacc0 b                          }
        xm.vlmacc1 b
        { add b, b, _32                 ; nop                                   }
        xm.vlsat t3
        { add a, a, _32                 ; xm.vstr a                             }
        { nop                           ; xm.bt len, .L_loop_top                }
.L_loop_bot:

    { xm.mkmsk tail, tail               ; xm.brff tail, .L_finish               }
    { xm.not tail, tail                 ; xm.vclrdr                             }
        xm.vstrpv t3, tail
    { xm.not tail, tail                 ; xm.vlmacc0 b                          }
    xm.vlmacc1 b
    xm.vlsat t3
        xm.vstrpv a, tail
        xm.vstrpv t3, tail
    { nop                               ; xm.vldr t3                            }
    { nop                               ; xm.vstr t3                            }


.L_finish:
    xm.lddsp  s3,s2,32

    { li a0, 15                         ; xm.vgetc t3                           }
    { xm.zexti t3, 5                    ; nop                                   }
    { sub a0, a0, t3                    ; xm.retsp (NSTACKWORDS)*4              } 

.L_func_end:


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME






#endif //defined(__VX4B__)



