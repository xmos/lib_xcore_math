// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


/*  
void xs3_memcpy(
    void* dst,
    const void* src,
    unsigned bytes);
*/

#define NSTACKWORDS     (0)
#define FUNCTION_NAME   xs3_memcpy

#define a           a0
#define b           a1
#define len         a2
#define tmp         a3

.text
.p2align 2


FUNCTION_NAME:
    xm.entsp (NSTACKWORDS)*4
  { srli tmp, len, 5                    ; xm.zexti len, 5                       }
  { li t3, 32                           ; xm.brff tmp, .L_loop_bot              } 
  .L_loop_top:
    { add b, b, t3                      ; xm.vldd b                             }
    { addi tmp, tmp, -1                 ; xm.vstd a                             }
    { add a, a, t3                      ; xm.bt tmp, .L_loop_top                }
  .L_loop_bot:
  { xm.mkmsk len, len                   ; xm.brff len, .L_finish                }
  { mv t3, b                            ; nop                                   }
  { nop                                 ; xm.vldr t3                            }
    xm.vstrpv a, len
.L_finish:
    xm.retsp (NSTACKWORDS)*4


.L_func_end:

.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"


#endif //defined(__VX4B__)
