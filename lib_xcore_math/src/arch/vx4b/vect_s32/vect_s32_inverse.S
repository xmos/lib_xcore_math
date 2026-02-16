// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

/*  

headroom_t vect_s32_inverse(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const unsigned scale);

*/


#define NSTACKVECTS     (1)
#define NSTACKWORDS     (8+8*(NSTACKVECTS))

#define FUNCTION_NAME   vect_s32_inverse


#define STACK_VEC_TMP       (NSTACKWORDS-8-1)


#define a               a0
#define b               a1
#define length          a2
#define scale           a3
#define div_hi          a3
#define div_lo          s2
#define v_mask          s3
#define _32             s4
#define val1            s5
#define val2            s6
#define vec_tmp         s7

.text
.p2align 2


FUNCTION_NAME:
    xm.entsp (NSTACKWORDS)*4
    xm.stdsp  s3,s2,0
    xm.stdsp  s5,s4,8
    xm.stdsp  s7,s6,16

{ li t3, 0                              ; sw s8, 24                          (sp) }
{ slli length, length, 2                ; xm.vsetc t3                           }

{ li _32, 32                            ; nop                                   }
{ sub val2, scale, _32                  ; li val1, 1                            }
{ xm.shl div_hi, val1, val2             ; xm.shl div_lo, val1, scale            }
{ xm.vclrdr                             ; nop                                   }
{ addi vec_tmp,sp, (STACK_VEC_TMP)*4    ; xm.brff length, .L_loop_bot           }
{ xm.vldr b                             ; nop                                   }
.p2align 4
.L_loop_top:
    // The masked out elements will  
    { xm.mkmsk v_mask, length           ; xm.vstd vec_tmp                       }
        xm.vlashr b, v_mask
    { sub length, length, _32           ; xm.vsign                              }

    { nop                               ; xm.vlmul0 b                           }
        xm.vstrpv vec_tmp, v_mask
        xm.vlashr b, v_mask
    { add b, b, _32                     ; xm.vsign                              }
    { mv val2, v_mask                   ; nop                                   }
    .L_div_loop_top:

        { srli val2, val2, 4            ; lw val1,0                    ( vec_tmp) }
            xm.ldivu val1, s8, div_hi, div_lo, val1
        { addi vec_tmp, vec_tmp, 4      ; sw val1,0                    ( vec_tmp) }
        { nop                           ; xm.bt val2, .L_div_loop_top           }
    .L_div_loop_bot:
    { addi vec_tmp,sp, (STACK_VEC_TMP)*4 ; nop                                   }


    { li val1, 1                        ; xm.vlmul0 vec_tmp                     }
        xm.vstrpv a, v_mask
    { xm.slt val1, length, val1         ; xm.vstr vec_tmp                       } // Headroom update
    { add a, a, _32                     ; nop                                   }
    beqz val1, .L_loop_top 
.L_loop_bot:

.L_finish:
    xm.lddsp  s3,s2,0
    xm.lddsp  s5,s4,8
    xm.lddsp  s7,s6,16
{ li a0, 31                             ; xm.vgetc t3                           }
{ xm.zexti t3, 5                        ; lw s8, 24                      (sp)   }
{ sub a0, a0, t3                        ; xm.retsp (NSTACKWORDS)*4              } 


.L_func_end:


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME


#endif //defined(__VX4B__)



