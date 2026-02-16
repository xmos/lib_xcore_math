// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// XMOS Public License: Version 1

#if defined(__VX4B__)

#include "../asm_helper.h"

/*  

headroom_t vect_complex_s32_conj_nmacc(
    complex_s32_t* acc,
    const complex_s32_t* b,
    const complex_s32_t* c,
    const unsigned length,
    const right_shift_t acc_shr,
    const right_shift_t b_shr,
    const right_shift_t c_shr);

*/

.text
.p2align 2


#define NSTACKWORDS     (8+8)

#define STACK_VEC_TMP   (NSTACKWORDS-8-1)

#define STACK_BYTEMASK  1

#define acc         a0 
#define b           a1 
#define c           a2
#define len         a3
#define shr_b       s2
#define shr_c       s3
#define tmp         s4
#define tmp_vec     s5
#define shr_acc     s6

#define bytemask    len

#define FUNCTION_NAME vect_complex_s32_conj_nmacc
    


FUNCTION_NAME:
    xm.entsp (NSTACKWORDS)*4
    xm.stdsp  s3,s2,8
    xm.stdsp  s5,s4,16
    xm.stdsp  s7,s6,0

    mv shr_acc, a4
    { nop                               ; nop                                   }
    mv shr_b, a5
    { slli t3, len, 3                   ; nop                                   }
    mv shr_c, a6
    { li tmp, 32                        ; nop                                   }
    { xm.zexti t3, 5                    ; srli len, len, 2                      }
    { addi tmp_vec,sp, (STACK_VEC_TMP)*4 ; xm.mkmsk t3, t3                       }
    { li t3, 0                          ; sw t3, (STACK_BYTEMASK)*4             (sp) }
    { xm.mkmski t3, 32                  ; xm.vsetc t3                           }
    { addi len, len, -1                 ; xm.brff len, .L_loop_bot_s32          }

.L_loop_top_s32:
        xm.vlashr acc, shr_acc
        xm.vstrpv acc, t3
        xm.vlashr c, shr_c
	      xm.vstrpv tmp_vec, t3
    { nop                               ; xm.vldc tmp_vec                       }
	      xm.vlashr b, shr_b
        xm.vstrpv tmp_vec, t3
    { nop                               ; xm.vldd tmp_vec                       }
    { add b, b, tmp                     ; xm.vcmcr0                             }
    { add c, c, tmp                     ; xm.vcmci0                             }
    { nop                               ; xm.vlsub acc                          }
    { add acc, acc, tmp                 ; xm.vstr acc                           }
    { addi len, len, -1                 ; xm.bt len, .L_loop_top_s32            }

.L_loop_bot_s32:
    { nop                               ; lw bytemask, (STACK_BYTEMASK)*4        (sp) }
    { nop                               ; xm.brff len, .L_done_s32              }
    { nop                               ; xm.vclrdr                             }
    { nop                               ; xm.vstd tmp_vec                       }
        xm.vlashr acc, shr_acc
        xm.vstrpv acc, bytemask
        xm.vlashr c, shr_c
        xm.vstrpv tmp_vec, bytemask
    { nop                               ; xm.vldc tmp_vec                       }
        xm.vlashr b, shr_b
        xm.vstrpv tmp_vec, bytemask                      
    { nop                               ; xm.vldd tmp_vec                       }
    { nop                               ; xm.vcmcr0                             }
    { mv t3, tmp_vec                    ; xm.vcmci0                             }
    { nop                               ; xm.vlsub acc                          }
        xm.vstrpv tmp_vec, bytemask
    { nop                               ; xm.vldr t3                            }
    { nop                               ; xm.vstr tmp_vec                       }
        xm.vstrpv acc, bytemask

.L_done_s32:
        xm.lddsp  s3,s2,8
        xm.lddsp  s5,s4,16
        xm.lddsp  s7,s6,0

    { li a0, 31                         ; xm.vgetc t3                           }
    { xm.zexti t3, 5                    ; nop                                   }
    { sub a0, a0, t3                    ; xm.retsp (NSTACKWORDS)*4              }


.L_func_end:

.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME

#undef FUNCTION_NAME



#endif //defined(__VX4B__)



