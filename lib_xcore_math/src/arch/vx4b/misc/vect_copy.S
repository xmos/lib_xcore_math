// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


/*  
headroom_t vect_s32_copy(
    int32_t a[],
    const int32_t b[],
    unsigned length);
*/

#define NSTACKWORDS     (0)
#define FUNCTION_NAME   vect_s32_copy

#define a           a0
#define b           a1
#define len         a2
#define tmp         a3

.text
.p2align 2


FUNCTION_NAME:
    xm.entsp (NSTACKWORDS)*4
  { li t3, 0                            ; srli tmp, len, 3                      }
  { xm.zexti tmp, 1                     ; srli len, tmp, 1                      }
  { add len, len, tmp                   ; xm.vsetc t3                           }
  { li t3, 32                           ; xm.bt tmp, .L_loop_mid                }
  { nop                                 ; xm.bu .L_loop_top                     }
  
  .p2align 4
  .L_loop_top:
    { add b, b, t3                      ; xm.vldd b                             }
    { add a, a, t3                      ; xm.vstd a                             }
    .L_loop_mid:
    { add b, b, t3                      ; xm.vldd b                             }
    { addi len, len, -1                 ; xm.vstd a                             }
    { add a, a, t3                      ; xm.bt len, .L_loop_top                }
  .L_loop_bot:

.L_finish:
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


#endif //defined(__VX4B__)
