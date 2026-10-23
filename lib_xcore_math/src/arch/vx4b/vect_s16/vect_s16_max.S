// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


/*

int32_t vect_s16_max(
    const int32_t b[],
    const unsigned length);


*/


#include "../asm_helper.h"

.text
.p2align 5

#define NSTACKVECS      (1)
#define NSTACKWORDS     (8 + 8*NSTACKVECS)

#define FUNCTION_NAME       vect_s16_max

#define STACK_VEC_CUR_MAX   (NSTACKWORDS-8-1)

#define b           a0
#define N           a1
#define tail        a2
#define tmp         a3
#define tmz         s2
#define msk         s3

.globl FUNCTION_NAME
.type FUNCTION_NAME,@function

FUNCTION_NAME:

    xm.entsp (NSTACKWORDS)*4
    xm.stdsp s2, s3, 0

    li t3, 0x100
    { slli tail, N, SIZEOF_LOG2_S16     ; xm.vsetc t3                           }
    { srli N, N, EPV_LOG2_S16           ; xm.mkmsk s3, 2                        }
    { slli tmp, N, 5                    ; nop                                   }
        la t3, vpu_vec_0x8000
    { addi t3,sp, (STACK_VEC_CUR_MAX)*4 ; xm.vldr t3                            }
    { add t3, b, tmp                    ; xm.vstr t3                            }
    { xm.zexti tail, 5                  ; xm.vldr t3                            }
    { xm.mkmsk tail, tail               ; addi t3,sp, (STACK_VEC_CUR_MAX)*4     }
        xm.vstrpv t3, tail

    // Tail is fully accounted for in cur_max now.

#undef tail
#define cur_max     a2

    { mv cur_max, t3                    ; xm.vclrdr                             }
    xm.brff N, .L_loop_bot

    .L_loop_top:
        // cur_max[] saved in stack

        xm.vldr b //  vR[i] = b[i]
        { addi N, N, -1                 ; xm.vlsub cur_max                      } //  vR[i] = cur_max[i] - b[i]
        { addi t3, sp, 2 * 4            ; xm.vdepth1                            } //  vR[0] = [bitmask -- 1 where vR[i] < 0]  b[i] > cur_max[i]
        xm.vstrpv t3, msk
        { li t3, 32                     ; lw tmp, 2 * 4 (sp)                    }
        { mv tmz, tmp                   ; xm.vldr b                             }
        xm.zip tmz, tmp, 0
        xm.vstrpv cur_max, tmp
        { add b, b, t3                 ; xm.bt N, .L_loop_top                  }
.L_loop_bot:

    { addi t3,sp, (STACK_VEC_CUR_MAX)*4 ; li N, 15                              }
    xm.ld16s cur_max, N(t3)
    { addi t3, t3, -2                   ; nop                                   }
    .L_loop2_top:
         xm.ld16s a0, N(t3)
        { addi N, N, -1                 ; nop                                   }
        { xm.slt tmp, a0, cur_max       ; nop                                   }
        { xm.shli tmp, tmp, 1           ; nop                                   }
        { nop                           ; xm.bru tmp                            }
        { mv cur_max, a0                ; nop                                   }
        { mv a0, cur_max                ; xm.bt N, .L_loop2_top                 }

        xm.lddsp s2, s3, 0
        xm.retsp (NSTACKWORDS)*4


.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
.L_end:
    .size FUNCTION_NAME, .L_end - FUNCTION_NAME






#endif //defined(__VX4B__)
