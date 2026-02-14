// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// XMOS Public License: Version 1

#if defined(__VX4B__)


#include "../asm_helper.h"

/*  

headroom_t vect_s32_nmacc(
    int32_t acc[],
    const int32_t b[],
    const int32_t c[],
    const unsigned len,
    const int acc_shr,
    const int b_shr,
    const int c_shr);
*/


#define NSTACKWORDS     (8+8)

#define FUNCTION_NAME   vect_s32_nmacc

#define STACK_VEC_TMP   (NSTACKWORDS-8-1)
#define STACK_BYTEMASK  5

#define acc         a0 
#define b           a1 
#define c           a2
#define len         a3
#define shr_b       s2
#define shr_c       s3
#define _32         s4
#define tmp_vec     s5
#define shr_acc     s6
#define bytemask    len

.text
.p2align 2




FUNCTION_NAME:
        xm.entsp (NSTACKWORDS)*4
    { li t3, 0                          ; sw s6, 16                           (sp) }
    { slli t3, len, SIZEOF_LOG2_S32     ; xm.vsetc t3                           }
    { xm.zexti t3, 5                    ; srli len, len, EPV_LOG2_S32           }
    mv shr_acc,a4
    
        xm.stdsp  s3,s2,8
        xm.stdsp  s5,s4,0
    { li _32, 32                        ; xm.vclrdr                             }
    mv shr_c, a6
    { xm.mkmsk t3, t3                   ; nop                                   }
    mv shr_b, a5
    { addi tmp_vec,sp, (STACK_VEC_TMP)*4 ; nop                                   }
    { xm.mkmski t3, 32                  ; sw t3, (STACK_BYTEMASK)*4             (sp) }
    { nop                               ; xm.brff len, .L_loop_bot              }
    { nop                               ; xm.bu .L_loop_top                     }

.p2align 4
.L_loop_top:
            xm.vlashr acc, shr_acc
            xm.vstrpv acc, t3
            xm.vlashr b, shr_b
            xm.vstrpv tmp_vec, t3
            xm.vlashr c, shr_c
        { add b, b, _32                 ; nop                                   } 
        { add c, c, _32                 ; xm.vlmul0 tmp_vec                     }
        { nop                           ; xm.vlsub acc                          }
        { addi len, len, -1             ; xm.vstr acc                           }
        { add acc, acc, _32             ; xm.bt len, .L_loop_top                }
.L_loop_bot:

    { nop                               ; lw bytemask, (STACK_BYTEMASK)*4        (sp) }
    { nop                               ; xm.brff bytemask, .L_finish           }
        xm.vlashr acc, shr_acc
        xm.vstrpv acc, bytemask
        xm.vlashr b, shr_b
        xm.vstrpv tmp_vec, t3 
        xm.vlashr c, shr_c
    { mv t3, tmp_vec                    ; xm.vlmul0 tmp_vec                     }
    { nop                               ; xm.vlsub acc                          }
    { nop                               ; xm.vstd tmp_vec                       }
        xm.vstrpv tmp_vec, bytemask
        xm.vstrpv acc, bytemask
    { nop                               ; xm.vldr t3                            }
    { nop                               ; xm.vstr t3                            }

.L_finish:
    xm.lddsp  s3,s2,8
    xm.lddsp  s5,s4,0


    { li a0, 31                         ; xm.vgetc t3                           }
    { xm.zexti t3, 5                    ; lw s6, 16                           (sp) }
    { sub a0, a0, t3                    ; xm.retsp (NSTACKWORDS)*4              } 


.L_func_end:


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME


#endif //defined(__VX4B__)



