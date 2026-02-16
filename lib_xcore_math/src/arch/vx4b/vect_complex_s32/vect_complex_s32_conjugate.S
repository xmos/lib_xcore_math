// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)

#include "../asm_helper.h"

/*  
headroom_t vect_complex_s32_conjugate(
    complex_s32_t* a,
    const complex_s32_t* b,
    const unsigned length);
*/

.text
.p2align 2

#define NSTACKVECTS     (1)
#define NSTACKWORDS     (4 + 8*(NSTACKVECTS))

#define STACK_VEC_TMP   (NSTACKWORDS - 8-1)

#define a           a0 
#define b           a1 
#define len         a2

#define _32         a3
#define tail        s2

#define FUNCTION_NAME vect_complex_s32_conjugate
    
FUNCTION_NAME:
  xm.entsp (NSTACKWORDS)*4
  xm.stdsp  s3,s2,0
    
  { li _32, 32                          ; li t3, 0                              }
  { slli tail, len, 3                   ; xm.vsetc t3                           }
la t3, vpu_vec_complex_conj_op

  { xm.zexti tail, 5                    ; srli len, len, 2                      }
  { xm.mkmsk tail, tail                 ; xm.brff len, .L_loop_bot              }
    
  .L_loop_top:
    { addi len, len, -1                 ; xm.vldr t3                            }
    { add b, b, _32                     ; xm.vlmul0 b                           }
    { add a, a, _32                     ; xm.vstr a                             }
    { nop                               ; xm.bt len, .L_loop_top                }
  .L_loop_bot:
  { nop                                 ; xm.brff tail, .L_done                 }
  { nop                                 ; xm.vclrdr                             }
  { nop                                 ; xm.vldr t3                            }
  { addi t3,sp, (STACK_VEC_TMP)*4       ; xm.vlmul0 b                           }
    xm.vstrpv a, tail

  // To make sure the tail is captured in the headroom..
  { nop                                 ; xm.vstd t3                            }
    xm.vstrpv t3, tail
  { nop                                 ; xm.vldd t3                            }
  { nop                                 ; xm.vstd t3                            }

.L_done:
    xm.lddsp  s3,s2,0
  { li a0, 31                           ; xm.vgetc t3                           }
  { xm.zexti t3, 5                      ; nop                                   }
  { sub a0, a0, t3                      ; xm.retsp (NSTACKWORDS)*4              }
  
.L_func_end:

.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME

#undef FUNCTION_NAME

#endif //defined(__VX4B__)



