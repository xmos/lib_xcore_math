// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

/*  
headroom_t vect_s32_scale(
    int32_t a[],
    const int32_t b[],
    const unsigned len,
    const int32_t c,
    const right_shift_t b_shr,
    const right_shift_t c_shr);
*/


#define NSTACKWORDS     (8+8)

#define FUNCTION_NAME   vect_s32_scale

#define STACK_VEC_TMP   0  
#define STACK_BYTEMASK  12

#define a           a0 
#define b           a1 
#define len         a2
#define c           a3
#define shr_b       s2
#define _32         s3
#define tmp_vec     s4
#define bytemask    len

.text
.p2align 2



FUNCTION_NAME:
        xm.entsp (NSTACKWORDS)*4
        xm.stdsp  s3,s2,32
        xm.stdsp  s5,s4,40
    { li t3, 0                          ; nop                                   }
    { slli t3, len, SIZEOF_LOG2_S32     ; xm.vsetc t3                           }
    { xm.zexti t3, 5                    ; srli len, len, EPV_LOG2_S32           }
    { li _32, 32                        ; xm.vclrdr                             }
    mv shr_b, a4
    { xm.mkmsk t3, t3                   ; nop                                   }
    { xm.mkmski t3, 32                  ; sw t3, (STACK_BYTEMASK)*4             (sp) }
        xm.stdsp  c,c,((STACK_VEC_TMP/2)+0)*8
        xm.stdsp  c,c,((STACK_VEC_TMP/2)+1)*8
        xm.stdsp  c,c,((STACK_VEC_TMP/2)+2)*8
        xm.stdsp  c,c,((STACK_VEC_TMP/2)+3)*8
    mv tmp_vec, a5
    { addi c,sp, (STACK_VEC_TMP)*4      ; nop                                   }
        xm.vlashr c, tmp_vec
        xm.vstrpv c, t3

    { nop                               ; xm.brff len, .L_loop_bot              }
    { nop                               ; xm.bu .L_loop_top                     }

.p2align 4
.L_loop_top:
            xm.vlashr b, shr_b
        { add b, b, _32                 ; xm.vlmul0 c                           }  
        { addi len, len, -1             ; xm.vstr a                             }
        { add a, a, _32                 ; xm.bt len, .L_loop_top                }
.L_loop_bot:

    { nop                               ; lw bytemask, (STACK_BYTEMASK)*4        (sp) }
    { nop                               ; xm.brff bytemask, .L_finish           }
        xm.vlashr b, shr_b
    { mv t3, c                          ; xm.vlmul0 c                           }
    { nop                               ; xm.vstd t3                            }
        xm.vstrpv t3, bytemask
        xm.vstrpv a, bytemask
    { nop                               ; xm.vldr t3                            }
    { nop                               ; xm.vstr t3                            }

.L_finish:
    xm.lddsp  s3,s2,32
    xm.lddsp  s5,s4,40

    { li a0, 32                         ; xm.vgetc t3                           }
    { xm.zexti t3, 5                    ; srli a1, t3, 8                        }
    { xm.shr a0, a0, a1                 ; addi t3, t3, 1                        }
    { sub a0, a0, t3                    ; xm.retsp (NSTACKWORDS)*4              } 

.L_func_end:




.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS; .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;              .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;             .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;           .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME



#endif //defined(__VX4B__)



