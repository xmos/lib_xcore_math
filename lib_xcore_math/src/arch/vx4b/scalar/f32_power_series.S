// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

/*  

float f32_power_series(
    const float x,
    const float coef[],
    const unsigned terms_count);
*/


#define NSTACKWORDS     (4)

#define FUNCTION_NAME   f32_power_series

#define x       a0
#define coef    a1
#define count   a2
#define acc     a3
#define tmpA    s2
#define pow     s3

// these unroll settings seem to offer the best
// tradeoff between code size and speed (3/8 should also work)
#define UNROLL_LOG2   2
#define UNROLL        4

#define CAT_(A, B)    A##B
#define CAT(A, B)     CAT_(A,B)

#define FULL_LOOP_LBL CAT(.L_loop_, UNROLL)

.text
.p2align 2


FUNCTION_NAME:
  xm.entsp (NSTACKWORDS)*4
  xm.stdsp  s3,s2,0

{ li acc, 0                             ; mv pow, x                             }
{ srli t3, count, UNROLL_LOG2           ; nop                                   }

  .L_loop_top:
  { xm.subi t3, count, UNROLL           ; xm.bt t3, .L_loop_full                }
    slli t3, t3, 2
  { add coef, coef, t3                  ; xm.bru count                          }
    xm.assert count
#if (UNROLL_LOG2 >= 1)
    tail .L_loop_1
#endif
#if (UNROLL_LOG2 >= 2)
    tail .L_loop_2
    tail .L_loop_3
#endif
#if (UNROLL_LOG2 >= 3)
    tail .L_loop_4
    tail .L_loop_5
    tail .L_loop_6
    tail .L_loop_7
#endif

    .L_loop_full:
#if (UNROLL_LOG2 >= 3)
    .L_loop_8:
    { addi count, count, -1             ; lw tmpA,(UNROLL-8)*4    ( coef)       }
      xm.fmacc acc, acc, pow, tmpA
      xm.fmul pow, pow, x
    .L_loop_7:
    { addi count, count, -1             ; lw tmpA,(UNROLL-7)*4    ( coef)       }
      xm.fmacc acc, acc, pow, tmpA
      xm.fmul pow, pow, x
    .L_loop_6:
    { addi count, count, -1             ; lw tmpA,(UNROLL-6)*4    ( coef)       }
      xm.fmacc acc, acc, pow, tmpA
      xm.fmul pow, pow, x
    .L_loop_5:
    { addi count, count, -1             ; lw tmpA,(UNROLL-5)*4    ( coef)       }
      xm.fmacc acc, acc, pow, tmpA
      xm.fmul pow, pow, x
#endif
#if (UNROLL_LOG2 >= 2)    
    .L_loop_4:
    { addi count, count, -1             ; lw tmpA,(UNROLL-4)*4    ( coef)       }
      xm.fmacc acc, acc, pow, tmpA
      xm.fmul pow, pow, x
    .L_loop_3:
    { addi count, count, -1             ; lw tmpA,(UNROLL-3)*4    ( coef)       }
      xm.fmacc acc, acc, pow, tmpA
      xm.fmul pow, pow, x
#endif
#if (UNROLL_LOG2 >= 1)
    .L_loop_2:
    { addi count, count, -1             ; lw tmpA,(UNROLL-2)*4    ( coef)       }
      xm.fmacc acc, acc, pow, tmpA
      xm.fmul pow, pow, x
#endif
    .L_loop_1:
    { addi count, count, -1             ; lw tmpA,(UNROLL-1)*4    ( coef)       }
      xm.fmacc acc, acc, pow, tmpA
      xm.fmul pow, pow, x

  li t3, UNROLL*4
  add coef, coef, t3
  { srli t3, count, UNROLL_LOG2         ; xm.bt count, .L_loop_top              }


.L_finish:
  mv a0, acc
  xm.lddsp  s3,s2,0
  xm.retsp (NSTACKWORDS)*4
.L_func_end:


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME


#endif //defined(__VX4B__)



