// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

.text
.p2align 2

/*  
void s32_odd_powers(
    int32_t a[],
    const int32_t b,
    const unsigned count,
    const right_shift_t shr);
*/
#define FUNCTION_NAME   s32_odd_powers
#define NSTACKWORDS     (4)

#define a         a0
#define b         a1
#define len       a2
#define shr       a3

#define acc_lo    s2
#define acc_hi    s3

#define b_sqr     t3



FUNCTION_NAME:

  xm.entsp (NSTACKWORDS)*4
  xm.stdsp  s3,s2,0

{ nop                                   ; addi len, len, -1                     }
{ li acc_lo, 0                          ; li acc_hi, 0                          }
  xm.maccs acc_hi, acc_lo, b, b
  xm.lextract b_sqr, acc_hi, acc_lo, shr, 32

.L_loop_top:
  { addi len, len, -1                   ; sw b,0                 ( a)           }
  { li acc_lo, 0                        ; li acc_hi, 0                          }
    xm.maccs acc_hi, acc_lo, b, b_sqr
    xm.lextract b, acc_hi, acc_lo, shr, 32
  { addi a, a, 4                        ; xm.bt len, .L_loop_top                }

  sw b,0( a)

  xm.lddsp  s3,s2,0
  xm.retsp (NSTACKWORDS)*4

.L_func_end_unpack:

.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
.size FUNCTION_NAME,.L_func_end_unpack - FUNCTION_NAME

#undef NSTACKWORDS
#undef FUNCTION_NAME



#endif //defined(__VX4B__)



