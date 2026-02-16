// Copyright 2021-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


/*  
void vect_s8_is_negative(
    int8_t a[],
    const int8_t b[],
    const unsigned len);
*/


#include "../asm_helper.h"

#define NSTACKWORDS     (8)

#define FUNCTION_NAME   vect_s8_is_negative

#define a           a0 
#define b           a1 
#define len         a2
#define _32         a3
#define vec_0xC1    s2
#define tail        s3

.text
.p2align 2


FUNCTION_NAME:
    xm.entsp (NSTACKWORDS)*4
    li t3, 0x200
    xm.stdsp  s3,s2,0
  { mv t3, len                          ; xm.vsetc t3                           }
  { xm.zexti t3, 5                      ; srli len, len, EPV_LOG2_S8            }
  { li _32, 32                          ; xm.mkmsk tail, t3                     }
la t3, vpu_vec_0xC1
  { mv vec_0xC1, t3                     ; mv t3, b                              }
  { nop                                 ; xm.brff len, .L_loop_bot              }

  .L_loop_top:
    { add t3, t3, _32                   ; xm.vldr t3                            }
    { nop                               ; xm.vsign                              }
    { nop                               ; xm.vlsub vec_0xC1                     }
    { nop                               ; xm.vpos                               }
    { addi len, len, -1                 ; xm.vstr a                             }
    { add a, a, _32                     ; xm.bt len, .L_loop_top                }
  .L_loop_bot:

  { nop                                 ; xm.brff tail, .L_finish               }
  { nop                                 ; xm.vldr t3                            }
  { nop                                 ; xm.vsign                              }
  { nop                                 ; xm.vlsub vec_0xC1                     }
  { nop                                 ; xm.vpos                               }
    xm.vstrpv a, tail

    
.L_finish:
        xm.lddsp  s3,s2,0
    { nop                               ; xm.retsp (NSTACKWORDS)*4              } 

.L_func_end:


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME



#endif //defined(__VX4B__)



