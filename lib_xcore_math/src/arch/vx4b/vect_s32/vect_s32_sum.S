// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)



/*
    int64_t vect_s32_sum(
        const int32_t b[],
        const unsigned length);
*/


#include "../asm_helper.h"


#define FUNCTION_NAME   vect_s32_sum
#define NSTACKWORDS     (16)


#define STACK_VEC_TMP       (NSTACKWORDS-8-1)

#define b           a0
#define N           a1
#define tail        a2


.text
.p2align 2


FUNCTION_NAME:
        xm.entsp (NSTACKWORDS)*4
        xm.stdsp  s3,s2,0

    { li t3, 0                          ; nop                                   }
    { slli tail, N, SIZEOF_LOG2_S32     ; xm.vsetc t3                           }
    { xm.zexti tail, 5                  ; xm.vclrdr                             }
    { srli N, N, EPV_LOG2_S32           ; xm.brff tail, .L_tail_dealt_with      }

    la t3, vpu_vec_0x40000000 
    { addi s2,sp, (STACK_VEC_TMP)*4     ; xm.vldr t3                            }
    { nop                               ; xm.vstd s2                            }
    { xm.mkmsk tail, tail               ; slli N, N, 3                          }
        xm.vstrpv s2, tail
        sh2add s3, N, b              
    { nop                               ; xm.vldc s2                            }
    { nop                               ; xm.vclrdr                             }
    { srli N, N, 3                      ; xm.vlmacc0 s3                         }
    { li t3, 32                         ; xm.vldc t3                            }

.L_tail_dealt_with:
    la t3, vpu_vec_0x40000000
    { li t3, 32                         ; xm.vldc t3                            }
    { addi a2,sp, (STACK_VEC_TMP)*4     ; xm.brff N, .L_loop_bot                }

.L_loop_top:
        { addi N, N, -1                 ; xm.vlmacc0 b                          }
        { add b, b, t3                  ; xm.bt N, .L_loop_top                  }
.L_loop_bot:

.L_finish:

    // Requires vC to be filled with 0x40000000, which it already should be.
  
    la t3, vpu_vec_0x80000000   
    { nop                               ; xm.vstr a2                            }
    { nop                               ; xm.vlmacc0 t3                         }
    
    la t3, vpu_vec_zero  
    { addi t3,sp, (STACK_VEC_TMP)*4     ; xm.vldr t3                            }
    { nop                               ; xm.vlmaccr0 t3                        }
    { nop                               ; xm.vstd t3                            }
    { nop                               ; xm.vlmaccr0 t3                        }
    { nop                               ; xm.vstr t3                            }
    { nop                               ; lw a1,0                          ( t3) }
    { addi a1, a1, 8                    ; lw a0,4                          ( t3) }

        xm.lddsp  s3,s2,0
        xm.retsp (NSTACKWORDS)*4

.L_fend: 

.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords; 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores; 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers; 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends; 
.size FUNCTION_NAME, .L_fend - FUNCTION_NAME






#endif //defined(__VX4B__)
