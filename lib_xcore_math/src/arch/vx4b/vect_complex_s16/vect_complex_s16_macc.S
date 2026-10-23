// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// XMOS Public License: Version 1

#if defined(__VX4B__)


#include "../asm_helper.h"

/*  

headroom_t vect_complex_s16_macc(
    int16_t* acc_real,
    int16_t* acc_imag,
    const int16_t* b_real,
    const int16_t* b_imag,
    const int16_t* c_real,
    const int16_t* c_imag,
    const unsigned length,
    const right_shift_t acc_shr,
    const right_shift_t sat);

*/

.text
.p2align 5

#define NSTACKVECS      (2)
#define NSTACKWORDS     (8+8*(NSTACKVECS))

#define STACK_SAT       (NSTACKWORDS)

#define STACK_VEC_TMP   (NSTACKWORDS-8-1)
#define STACK_VEC_SAT   (NSTACKWORDS-16-1)

#define FUNCTION_NAME vect_complex_s16_macc
    
#define acc_re      a0
#define acc_im      a1
#define b_re        a2
#define b_im        a3
#define c_re        s2
#define c_im        s3
#define len         s4
#define vec_tmp     s5

#define vec_sat     a6
#define acc_shr     a7


/*
    We're doing this:

    vR  <- -1
    vR  <- -1 * b.imag
    vC  <- -b.imag  
    tmp <- 0
    tmp <- vC * c.imag
    vC  <- b.real
    tmp <- tmp + vC * c.real
    vR  <- tmp >> sat
    vR  <- acc.real + vR
    acc.real <- vR

    (vC still has b.real)
    tmp <- 0
    tmp <- vC * c.imag
    vC  <- b.imag
    tmp <- tmp + vC * c.real
    vR  <- tmp >> sat
    vR  <- acc.imag + vR
    acc.imag <- vR
*/


FUNCTION_NAME:
    xm.entsp (NSTACKWORDS)*4
    xm.stdsp s2, s3, 0
    xm.stdsp s4, s5, 8

    // move bytemask to compute it later
    mv s3, a6

    { addi t3,sp, (STACK_VEC_SAT)*4     ; xm.ldwsp s2, (STACK_SAT)*4            }
    mv vec_sat, t3
    { slli s4, s2, 16                   ; xm.zexti s2, 16                       }
    { or s2, s2, s4                     ; xm.stwsp s6, 16                       }
    xm.stdi  s2,s2, 0(t3)
    xm.stdi  s2,s2, 8(t3)
    xm.stdi  s2,s2, 16(t3)
    xm.stdi  s2,s2, 24(t3)

    la t3, vpu_vec_0x8000
    { li t3, 32                         ; mv s6, t3                             }
    { srl len, s3, 4                    ; xm.zext s3, 4                         }
    { sll s3, s3, SIZEOF_LOG2_S16       ; sll t3, t3, 3                         }
    { add vec_tmp,sp, (STACK_VEC_TMP)*4 ; xm.vsetc t3                           }

    mv c_re, a4
    mv a4, s3
    mv c_im, a5

    { xm.mkmsk t3, 32                   ; xm.brff len, .L_loop_bot              }

    .L_loop_top:
            xm.vlashr acc_re, acc_shr
            xm.vstrpv acc_re, t3
            xm.vlashr acc_im, acc_shr
            xm.vstrpv acc_im, t3
        { addi len, len, -1             ; xm.vldr s6                            }
        xm.vlmul0 b_im
        xm.vlmul1 b_im
        xm.vstrpv vec_tmp, t3
        xm.vldc vec_tmp
        { nop                           ; xm.vclrdr                             }
        { nop                           ; xm.vlmacc0 c_im                       }
        xm.vlmacc1 c_im
        { nop                           ; xm.vldc b_re                          }
        { nop                           ; xm.vlmacc0 c_re                       }
        xm.vlmacc1 c_re
        xm.vlsat vec_sat
        { nop                           ; xm.vladd acc_re                       }
        { li t3, 32                     ; xm.vstr acc_re                        }
        { add b_re, b_re, t3            ; xm.vclrdr                             }
        { add acc_re, acc_re, t3        ; xm.vlmacc0 c_im                       }
        xm.vlmacc1 c_im
        { add c_im, c_im, t3            ; xm.vldc b_im                          }
        { add b_im, b_im, t3            ; xm.vlmacc0 c_re                       }
        xm.vlmacc1 c_re
        { add c_re, c_re, t3            ; nop                                   }
        xm.vlsat vec_sat
        { nop                           ; xm.vladd acc_im                       }
        { add acc_im, acc_im, t3        ; xm.vstr acc_im                        }
        { xm.mkmsk t3, 32               ; xm.bt len, .L_loop_top                }
    .L_loop_bot:

#define bytemask len

    mv bytemask, a4
    { xm.mkmsk bytemask, bytemask       ; xm.brff bytemask, .L_done             }   

        xm.vlashr acc_re, acc_shr
        xm.vstrpv acc_re, bytemask
        xm.vlashr acc_im, acc_shr
        xm.vstrpv acc_im, bytemask
    xm.vldr s6
    xm.vlmul0 b_im
    xm.vlmul1 b_im
    xm.vstrpv vec_tmp, t3
    xm.vldc vec_tmp
    { nop                               ; xm.vclrdr                             }
    { nop                               ; xm.vstd vec_tmp                       }
    { nop                               ; xm.vlmacc0 c_im                       }
    xm.vlmacc1 c_im
    { nop                               ; xm.vldc b_re                          }
    xm.vlmacc0 c_re
    xm.vlmacc1 c_re
    xm.vlsat vec_sat
    { nop                               ; xm.vladd acc_re                       }
        xm.vstrpv acc_re, bytemask
        xm.vstrpv vec_tmp, bytemask
    { nop                               ; xm.vldd vec_tmp                       }
    { nop                               ; xm.vstd vec_tmp                       }
    { nop                               ; xm.vclrdr                             }
    { nop                               ; xm.vlmacc0 c_im                       }
     xm.vlmacc1 c_im
    { nop                               ; xm.vldc b_im                          }
    { nop                               ; xm.vlmacc0 c_re                       }
    xm.vlmacc1 c_re
    xm.vlsat vec_sat
    { nop                               ; xm.vladd acc_im                       }
        xm.vstrpv acc_im, bytemask   
        xm.vstrpv vec_tmp, bytemask
    { nop                               ; xm.vldd vec_tmp                       }
    { nop                               ; xm.vstd vec_tmp                       }


.L_done:
        xm.lddsp s2, s3, 0
        xm.lddsp s4, s5, 8

    { li a0, 15                         ; xm.vgetc t3                           }
    { xm.zexti t3, 5                    ; xm.ldwsp s6, 16                       }
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

