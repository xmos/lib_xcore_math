// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

.text
.p2align 2

#define NSTACKWORDS     (12)
#define STACK_TMP_VEC       0

#define a           a0
#define b           a1
#define len         a2
#define tail        a3


/*
headroom_t vect_s32_abs(
    int32_t a[],
    const int32_t b[],
    const unsigned length);
*/

vect_s32_abs:
        xm.entsp (NSTACKWORDS)*4
    { li t3, 0                          ; slli tail, len, SIZEOF_LOG2_S32       }
    { srli len, len, EPV_LOG2_S32       ; xm.vsetc t3                           }
    { xm.zexti tail, 5                  ; xm.bu .L_apply_op                     }

.L_func_end_s32:




#undef a
#undef b
#undef len

/*
    When branching here:
        *   a --> a0
        *   b --> a1
        *   loop_count --> a2
        *   tail --> a3
        *   VPU mode must already be set.
*/

#define a           a0
#define b           a1
#define loop_count  a2
#define tail        a3

.type .L_apply_op,@function

.L_apply_op:

    { xm.mkmsk tail, tail               ; nop                                   }
    { mv t3, b                          ; xm.brff loop_count, .L_loop_bot       }
    { li a1, 32                         ; xm.bu .L_loop_top                     }
.p2align 4
.L_loop_top:
        { addi loop_count, loop_count, -1 ; xm.vldr t3                            }
        { nop                           ; xm.vsign                              }
        { nop                           ; xm.vlmul0 t3                          } //TODO this is wrong for 16 bit
        { add a, a, a1                  ; xm.vstr a                             }
        { add t3, t3, a1                ; xm.bt loop_count, .L_loop_top         }
.L_loop_bot:

    { nop                               ; xm.brff tail, .L_finish               }
    { nop                               ; xm.vclrdr                             }
    { nop                               ; xm.vldr t3                            }
    { nop                               ; xm.vsign                              }
    { addi t3,sp, (STACK_TMP_VEC)*4     ; xm.vlmul0 t3                          } //TODO this is wrong for 16 bit
    { nop                               ; xm.vstd t3                            }
    { nop                               ; xm.vpos                               }
        xm.vstrpv t3, tail
    { nop                               ; xm.vldr t3                            }
    { nop                               ; xm.vstr t3                            }
        xm.vstrpv a, tail

.L_finish:
    { li a0, 32                         ; xm.vgetc t3                           }
    { srli a1, t3, 8                    ; nop                                   }
    { xm.zexti t3, 5                    ; xm.shr a0, a0, a1                     }
    { addi t3, t3, 1                    ; nop                                   }
    { sub a0, a0, t3                    ; xm.retsp (NSTACKWORDS)*4              } 

.L_end_apply_op: 
.size .L_apply_op, .L_end_apply_op - .L_apply_op

.global vect_s32_abs
.type vect_s32_abs,@function
.resource_const vect_s32_abs, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty vect_s32_abs, "tail_callees"
.resource_list_empty vect_s32_abs, "callees"
.resource_list_empty vect_s32_abs, "parallel_callees"
.size vect_s32_abs, .L_func_end_s32 - vect_s32_abs




#endif //defined(__VX4B__)
