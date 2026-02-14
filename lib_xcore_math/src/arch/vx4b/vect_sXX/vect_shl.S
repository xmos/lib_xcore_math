// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


/*  
headroom_t vect_s16_shl(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const int shl);

headroom_t vect_s32_shl(
    int32_t a[],,
    const int32_t b[],
    const unsigned length,
    const int shl);
*/

#include "../asm_helper.h"



#define NSTACKWORDS     (8+2+2+4)

#define FUNCTION_NAME   shl_vect
#define FNAME_S16       CAT(FUNCTION_NAME, _s16)
#define FNAME_S32       CAT(FUNCTION_NAME, _s32)

#define STACK_TMP_VEC       (NSTACKWORDS-10)

#define a           a0
#define b           a1
#define len         a2
#define b_shl       a3


.text
.p2align 2




vect_s16_shl:

        xm.entsp (NSTACKWORDS)*4
        li t3, 0x0100
    { slli s2, len, SIZEOF_LOG2_S16     ; sw s2, 4                       (sp)   }
    { srli len, len, EPV_LOG2_S16       ; xm.vsetc t3                           }
    { xm.zexti s2, 5                    ; xm.bu .L_apply_op                     }


.L_size_end_vect_s16_shl: 
    .size vect_s16_shl, .L_size_end_vect_s16_shl - vect_s16_shl







vect_s32_shl:

        xm.entsp (NSTACKWORDS)*4
    { li t3, 0                          ; sw s2, 4                       (sp)   }
    { slli s2, len, SIZEOF_LOG2_S32     ; xm.vsetc t3                           }
    { srli len, len, EPV_LOG2_S32       ; xm.zexti s2, 5                        }
    { nop                               ; xm.bu .L_apply_op                     }
    

.L_size_end_vect_s32_shl: 
    .size vect_s32_shl, .L_size_end_vect_s32_shl - vect_s32_shl
    
#undef a
#undef b
#undef len
#undef b_shl






/*
    When branching here:
        *   a --> a0
        *   b --> a1
        *   loop_count --> a2
        *   shl --> a3
        *   tail --> s2
        *   VPU mode must already be set.
*/

#define a           a0
#define b           a1
#define loop_count  a2
#define b_shl       a3
#define b_shr         b_shl
#define tail        s2


.type .L_apply_op,@function; 

.L_apply_op:

    { xm.neg b_shr, b_shl               ; xm.zexti s2, 5                        }
    { xm.mkmsk tail, tail               ; xm.brff loop_count, .L_loop_bot       }
    { li t3, 32                         ; xm.bu .L_loop_top                     }
    { nop                               ; xm.bu .L_loop_top_inplace             }

    .p2align 4
    .L_loop_top_inplace:
            xm.vlashr b, b_shr
        { addi loop_count, loop_count, -1 ; xm.vstr b                             }
        { add b, b, t3                  ; xm.bt loop_count, .L_loop_top_inplace }
    { mv a, b                           ; xm.bu .L_loop_bot                     }

    .p2align 4
    .L_loop_top:
            xm.vlashr b, b_shr
        { add b, b, t3                  ; xm.vstr a                             }
        { addi loop_count, loop_count, -1 ; nop                                   }
        { add a, a, t3                  ; xm.bt loop_count, .L_loop_top         }
    
.L_loop_bot:

    { addi t3,sp, (STACK_TMP_VEC)*4     ; xm.brff tail, .L_finish               }
    { nop                               ; xm.vclrdr                             }
        xm.vlashr b, b_shr
    { nop                               ; xm.vstd t3                            }
        xm.vstrpv t3, tail
    { li a2, 32                         ; xm.vldr t3                            }
    { slli a2, a2, 3                    ; xm.vstr t3                            }
        xm.vstrpv a, tail

.L_finish:
    { li a0, 32                         ; xm.vgetc t3                           }
    { srli a1, t3, 8                    ; lw s2, 4                       (sp)   }
    { xm.zexti t3, 5                    ; xm.shr a0, a0, a1                     }
    { addi t3, t3, 1                    ; nop                                   }
    { sub a0, a0, t3                    ; xm.retsp (NSTACKWORDS)*4              } 


.L_end_apply_op: 
    .size .L_apply_op, .L_end_apply_op - .L_apply_op







.globl vect_s16_shl
.type vect_s16_shl,@function
.resource_const vect_s16_shl, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty vect_s16_shl, "tail_callees"
.resource_list_empty vect_s16_shl, "callees"
.resource_list_empty vect_s16_shl, "parallel_callees"

.globl vect_s32_shl
.type vect_s32_shl,@function
.resource_const vect_s32_shl, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty vect_s32_shl, "tail_callees"
.resource_list_empty vect_s32_shl, "callees"
.resource_list_empty vect_s32_shl, "parallel_callees"


#endif //defined(__VX4B__)
