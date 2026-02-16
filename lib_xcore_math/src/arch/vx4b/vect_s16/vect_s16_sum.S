// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)



/*  
    int32_t vect_s16_sum(
        const int16_t b[],
        const unsigned length);
*/


#include "../asm_helper.h"


#define FUNCTION_NAME   vect_s16_sum
#define NSTACKWORDS     (24+8+4)


#define STACK_VEC_TMP       (NSTACKWORDS-24-4)
#define STACK_VEC_TMP2       (NSTACKWORDS-8-2)

#define b           a0
#define N           a1
#define tail        a2


.text
.p2align 2


FUNCTION_NAME:
        xm.entsp (NSTACKWORDS)*4
        li t3, 0x0100
        xm.stdsp  s3,s2,0
        xm.stdsp  s5,s4,8
     { addi s4,sp, (STACK_VEC_TMP2)*4   ; nop                                   }
         addi s5, s4, (-30)

    { slli tail, N, SIZEOF_LOG2_S16     ; xm.vsetc t3                           }
    { xm.zexti tail, 5                  ; xm.vclrdr                             }
    { srli N, N, EPV_LOG2_S16           ; xm.brff tail, .L_tail_dealt_with      }

        la t3, vpu_vec_0x0001
    { addi s2,sp, (STACK_VEC_TMP)*4     ; xm.vldr t3                            }
    { nop                               ; xm.vstd s2                            }
    { xm.mkmsk tail, tail               ; slli N, N, 3                          }
        xm.vstrpv s2, tail
    sh2add s3, N, b  
    { nop                               ; xm.vldc s2                            }
    { nop                               ; xm.vclrdr                             }
    { srli N, N, 3                      ; xm.vlmaccr0 s3                        }
    xm.vlmaccr1 s3    

    { nop                               ; xm.vstd s4                            }    
    { nop                               ; xm.vldd s5                            }
    { nop                               ; xm.vstr s4                            }
    { nop                               ; xm.vldr s5                            }

    { li t3, 32                         ; xm.vldc t3                            }

.L_tail_dealt_with:
    { nop                               ; xm.brff N, .L_loop_bot                }
        la t3, vpu_vec_0x0001 
    { li t3, 32                         ; xm.vldc t3                            }
  
.L_loop_top:
        { addi N, N, -1                 ; xm.vlmaccr0 b                         }
        xm.vlmaccr1 b

    { nop                               ; xm.vstd s4                            }    
    { nop                               ; xm.vldd s5                            }
    { nop                               ; xm.vstr s4                            }
    { nop                               ; xm.vldr s5                            }

        { add b, b, t3                  ; xm.bt N, .L_loop_top                  }
.L_loop_bot:

.L_finish:



    { addi a1,sp, (STACK_VEC_TMP)*4     ; nop    /* adddr*/                     }
    
    addi s4, a1, 32-2

    { nop                               ; xm.vstd a1                            }
    { nop                               ; lw a0, 0(s4)                          }
    { slli a0, a0, 16                   ; xm.vstr a1                            }
    { nop                               ; lw a1, 0(s4)                          }

        xm.lddsp  s3,s2,0
        xm.lddsp  s5,s4,8
    { xm.zexti a1, 16                   ; nop                                   }
    { or a0, a0, a1                     ; xm.retsp (NSTACKWORDS)*4              }

.L_fend: 


.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
.size FUNCTION_NAME, .L_fend - FUNCTION_NAME




#endif //defined(__VX4B__)
