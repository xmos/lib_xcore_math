// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


/*  
int64_t vect_s32_energy(
    const int32_t b[],
    const unsigned length,
    const right_shift_t b_shr);
*/


#include "../asm_helper.h"

.text
.p2align 2

#define NSTACKVECS      (2)
#define NSTACKWORDS     (8 + 8*NSTACKVECS)


#define FUNCTION_NAME   vect_s32_energy

#define STACK_VEC_TMP       (NSTACKWORDS-8-1)
#define STACK_VEC_VR        (NSTACKWORDS-16-1)

#define b           a0
#define N           a1
#define b_shr       a2
#define vec_tmp     a3
#define tail        s2

.globl FUNCTION_NAME
.type FUNCTION_NAME,@function

FUNCTION_NAME:

        xm.entsp (NSTACKWORDS)*4
        xm.stdsp  s3,s2,0

    { li t3, 0                          ; addi vec_tmp,sp, (STACK_VEC_TMP)*4    }
    { slli tail, N, SIZEOF_LOG2_S32     ; xm.vsetc t3                           }
    { xm.zexti tail, 5                  ; xm.vclrdr                             }
    { srli N, N, EPV_LOG2_S32           ; xm.brff tail, .L_tail_dealt_with_s32  }

    { nop                               ; slli N, N, 5                          }
    { add t3, b, N                      ; xm.vstd vec_tmp                       }
    { xm.mkmsk tail, tail               ; nop                                   }
        xm.vlashr t3, b_shr
        xm.vstrpv vec_tmp, tail
#undef tail

    { nop                               ; xm.vldc vec_tmp                       }
    { nop                               ; xm.vclrdr                             }
    { srli N, N, 5                      ; xm.vlmacc0 vec_tmp                    }

.L_tail_dealt_with_s32:
    { addi t3,sp, (STACK_VEC_VR)*4      ; xm.brff N, .L_loop_bot_s32            }


.L_loop_top_s32:
        { li t3, 32                     ; xm.vstr t3                            }
            xm.vlashr b, b_shr
        { add b, b, t3                  ; xm.vstr vec_tmp                       }
        { addi t3,sp, (STACK_VEC_VR)*4  ; xm.vldc vec_tmp                       }
        { nop                           ; xm.vldr t3                            }
        { addi N, N, -1                 ; xm.vlmacc0 vec_tmp                    }
        { nop                           ; xm.bt N, .L_loop_top_s32              }
.L_loop_bot_s32:

.L_finish_s32:


la t3, vpu_vec_0x40000000
    { addi a2,sp, (STACK_VEC_TMP)*4     ; xm.vldc t3                            }
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


//.cc_bottom FUNCTION_NAME.function; 
.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
.L_end: 
    .size FUNCTION_NAME, .L_end - FUNCTION_NAME






#endif //defined(__VX4B__)
