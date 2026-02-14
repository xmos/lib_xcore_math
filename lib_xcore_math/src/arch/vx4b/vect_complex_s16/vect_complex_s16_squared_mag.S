// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

/*  

headroom_t vect_complex_s16_squared_mag(
    int16_t a[],
    const int16_t* b_real,
    const int16_t* b_imag,
    const unsigned length,
    const right_shift_t sat);

*/

.text
.p2align 2

#define NSTACKVECS      (4)
#define NSTACKWORDS     (8+8*(NSTACKVECS))


#define STACK_VEC_TMP   (NSTACKWORDS-8-1)
#define STACK_VEC_SAT   (NSTACKWORDS-16-1)

#define FUNCTION_NAME vect_complex_s16_squared_mag
    
#define a           a0
#define b_re        a1
#define b_im        a2
#define length      a3
#define vec_tmp     s2
#define vec_sat     s3
#define bytemask    s4
#define _32         s5


FUNCTION_NAME:
    xm.entsp (NSTACKWORDS)*4
    xm.stdsp  s3,s2,8
    xm.stdsp  s5,s4,0

    mv s2, a4
    { addi t3,sp, (STACK_VEC_SAT)*4     ; nop                                   }
    { slli s3, s2, 16                   ; xm.zexti s2, 16                       }
    { or s2, s2, s3                     ; li _32, 32                            }
        xm.stdi  s2,s2, 0(t3)
        xm.stdi  s2,s2, 8(t3)
        xm.stdi  s2,s2, 16(t3)
        xm.stdi  s2,s2, 24(t3)

    { nop                               ; slli bytemask, length, SIZEOF_LOG2_S16 }
    { srli length, length, 4            ; slli t3, _32, 3                       }
    { xm.zexti bytemask, 5              ; xm.vsetc t3                           }
{ xm.ldawsp t3, STACK_VEC_SAT      *4   ; nop                                   }
{ nop                                   ; xm.ldawsp vec_tmp, STACK_VEC_TMP*4    }
    { xm.mkmsk bytemask, bytemask       ; xm.brff length, .L_loop_bot           }

    .L_loop_top:

        { addi length, length, -1       ; xm.vclrdr                             }
        { nop                           ; xm.vldc b_re                          }
        { nop                           ; xm.vlmacc0 b_re                       }
        xm.vlmacc1 b_re
        { add b_re, b_re, _32           ; nop                                   }
        { nop                           ; xm.vldc b_im                          }
        { nop                           ; xm.vlmacc0 b_im                       }
        xm.vlmacc1 b_im
        { add b_im, b_im, _32           ; nop                                   }
        xm.vlsat t3
        { add a, a, _32                 ; xm.vstr a                             }
        { nop                           ; xm.bt length, .L_loop_top             }

    .L_loop_bot: //astew: worth jumping over to single issue mode for this?

    { nop                               ; xm.brff bytemask, .L_done             }
    { nop                               ; xm.vclrdr                             }
    { nop                               ; xm.vstd vec_tmp                       }
    { nop                               ; xm.vldc b_re                          }
    { nop                               ; xm.vlmacc0 b_re                       }
    xm.vlmacc1 b_re
    { nop                               ; xm.vldc b_im                          }
    { nop                               ; xm.vlmacc0 b_im                       }
    xm.vlmacc1 b_im
    xm.vlsat t3
    { nop                               ; xm.vstrpv a, bytemask                 }
    { nop                               ; xm.vstrpv vec_tmp, bytemask           }
    { nop                               ; xm.vldd vec_tmp                       }
    { nop                               ; xm.vstd vec_tmp                       }

.L_done:
    xm.lddsp  s3,s2,8
    xm.lddsp  s5,s4,0
    { li a0, 15                         ; xm.vgetc t3                           }
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



