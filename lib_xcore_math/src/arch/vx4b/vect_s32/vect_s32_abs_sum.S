// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


/*

int64_t vect_s32_abs_sum(
    const int32_t b[],
    const unsigned length);
*/


#include "../asm_helper.h"

.text
.p2align 5

#define NSTACKVECS      (2)
#define NSTACKWORDS     (4 + 8*NSTACKVECS)

#define FUNCTION_NAME       vect_s32_abs_sum

#define STACK_VEC_TMP       (NSTACKWORDS-8-1)
#define STACK_VEC_VR        (NSTACKWORDS-16-1)

#define b           a0
#define N           a1
#define tail        a2
#define _32         a3
#define tmp         s2
#define mask        s3

.global FUNCTION_NAME
.type FUNCTION_NAME, @function

FUNCTION_NAME:

        xm.entsp (NSTACKWORDS)*4
        xm.stdsp s2, s3, 0

    addi tmp,sp, (STACK_VEC_TMP)*4
    { li t3, 0                          ; slli tail, N, SIZEOF_LOG2_S32         }
    { addi t3,sp, (STACK_VEC_VR)*4      ; xm.vsetc t3                           }
    { xm.zexti tail, 5                  ; xm.vclrdr                             }
    { srli N, N, EPV_LOG2_S32           ; xm.brff tail, .L_tail_dealt_with      }

    { xm.mkmsk mask, tail               ; xm.vstd tmp                           }
    xm.vldr b
    { nop                               ; xm.vsign                              }
        xm.vstrpv tmp, mask
    { nop                               ; xm.vldc tmp                           }
    { nop                               ; xm.vclrdr                             }
    { add b, b, tail                    ; xm.vlmacc0 b                          }
.L_tail_dealt_with:
    { li _32, 32                        ; xm.brff N, .L_loop_bot_s32            }

.L_loop_top_s32:
        xm.vstr t3
        { addi N, N, -1                 ; xm.vldr b                             }
        xm.vsign
        xm.vstr tmp
        xm.vldc tmp
        xm.vldr t3
        { add b, b, _32                 ; xm.vlmacc0 b                          }
        { nop                           ; xm.bt N, .L_loop_top_s32              }
.L_loop_bot_s32:

    { nop                               ; xm.vstr t3                            }
la t3, vpu_vec_0x40000000
    { nop                               ; xm.vldc t3                            }
la t3, vpu_vec_0x80000000
    { nop                               ; xm.vlmacc0 t3                         }
la t3, vpu_vec_zero
    { addi t3,sp, (STACK_VEC_VR)*4      ; xm.vldr t3                            }
    { nop                               ; xm.vlmaccr0 t3                        }
    { nop                               ; xm.vstd t3                            }
    { nop                               ; xm.vlmaccr0 t3                        }
    { nop                               ; xm.vstr t3                            }
    xm.ldd a1, a0, 0 (t3)

    xm.lddsp s2, s3, 0
    { add a1, a1, 8                     ; xm.retsp (NSTACKWORDS)*4              }


.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
.L_end:
    .size FUNCTION_NAME, .L_end - FUNCTION_NAME






#endif //defined(__VX4B__)
