// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

/*  

headroom_t vect_complex_s16_mul(
    int16_t* a_real,
    int16_t* a_imag,
    const int16_t* b_real,
    const int16_t* b_imag,
    const int16_t* c_real,
    const int16_t* c_imag,
    const unsigned length,
    const right_shift_t sat);

*/

.text
.p2align 2

#define NSTACKVECS      (4)
#define NSTACKWORDS     (12+8*(NSTACKVECS)+4)


#define STACK_VEC_TMP   (NSTACKWORDS-8-4)
#define STACK_VEC_SAT   (NSTACKWORDS-16-4)

#define FUNCTION_NAME vect_complex_s16_mul
    
#define a_re        a0
#define a_im        a1
#define b_re        a2
#define b_im        a3
#define c_re        x18
#define c_im        x19
#define len         s4
#define vec_tmp     s5
#define vec_sat     x22
#define bytemask    s7



/*
    We're doing this:

    vR  <- -1
    vR  <- -1 * b.imag
    vC  <- -b.imag  
    acc <- 0
    acc <- vC * c.imag
    vC  <- b.real
    acc <- acc + vC * c.real
    vR  <- acc >> sat
    a.real <- vR

    (vC still has b.real)
    acc <- 0
    acc <- vC * c.imag
    vC  <- b.imag
    acc <- acc + vC * c.real
    vR  <- acc >> sat
    a.imag <- vR
*/


//.L_neg_ones:
  //  .short -0x4000,-0x4000,-0x4000,-0x4000,-0x4000,-0x4000,-0x4000,-0x4000,-0x4000,-0x4000,-0x4000,-0x4000,-0x4000,-0x4000,-0x4000,-0x4000

FUNCTION_NAME:
    xm.entsp (NSTACKWORDS)*4
    xm.stdsp  s3,s2,8
    xm.stdsp  s5,s4,16
    xm.stdsp  s7,s6,24

    mv s2, a7
    { addi t3,sp, (STACK_VEC_SAT)*4     ; nop                                   }
    { slli s3, s2, 16                   ; xm.zexti s2, 16                       }
    { or s2, s2, s3                     ; sw s8, 4                          (sp) }
        xm.stdi  s2,s2, 0(t3)
        xm.stdi  s2,s2, 8(t3)
        xm.stdi  s2,s2, 16(t3)
        xm.stdi  s2,s2, 24(t3)
    mv bytemask, a6
    { li t3, 32                         ; nop                                   }
    mv c_re, a4
    { srli len, bytemask, 4             ; nop                                   }
    mv c_im, a5
    { xm.zexti bytemask, 4              ; nop                                   }
    { slli bytemask, bytemask, SIZEOF_LOG2_S16 ; slli t3, t3, 3                        }
    { addi vec_tmp,sp, (STACK_VEC_TMP)*4 ; xm.vsetc t3                           }
    la t3, vpu_vec_0x8000    
    { mv s8, t3                         ; xm.brff len, .L_loop_bot              }

    .L_loop_top:
        { addi len, len, -1             ; xm.vldr t3                            }
        { xm.mkmski t3, 32              ; xm.vlmul0 b_im                        }
         xm.vlmul1 b_im
            xm.vstrpv vec_tmp, t3
        { addi vec_sat,sp, (STACK_VEC_SAT)*4 ; xm.vldc vec_tmp                       }
        { nop                           ; xm.vclrdr                             }
        { nop                           ; xm.vlmacc0 c_im                       }
        xm.vlmacc1 c_im
        { nop                           ; xm.vldc b_re                          }
        { nop                           ; xm.vlmacc0 c_re                       }
        xm.vlmacc1 c_re
        xm.vlsat vec_sat
        { li t3, 32                     ; xm.vstr a_re                          }

        
        { add b_re, b_re, t3            ; xm.vclrdr                             }
        { add a_re, a_re, t3            ; xm.vlmacc0 c_im                       }
        xm.vlmacc1 c_im
        { add c_im, c_im, t3            ; xm.vldc b_im                          }
        { add b_im, b_im, t3            ; xm.vlmacc0 c_re                       }
        xm.vlmacc1 c_re
        { add c_re, c_re, t3            ; nop                                   }
        xm.vlsat vec_sat
        { add a_im, a_im, t3            ; xm.vstr a_im                          }
        { mv t3, s8                     ; xm.bt len, .L_loop_top                }
    .L_loop_bot:

    { xm.mkmsk bytemask, bytemask       ; xm.brff bytemask, .L_done             }   

    { nop                               ; xm.vldr t3                            }
    { xm.mkmski t3, 32                  ; xm.vlmul0 b_im                        }
    xm.vlmul1 b_im
        xm.vstrpv vec_tmp, t3
    { addi vec_sat,sp, (STACK_VEC_SAT)*4 ; xm.vldc vec_tmp                       }
    { nop                               ; xm.vclrdr                             }
    { nop                               ; xm.vstd vec_tmp                       }
    { nop                               ; xm.vlmacc0 c_im                       }
    xm.vlmacc1 c_im
    { nop                               ; xm.vldc b_re                          }
    { addi t3,sp, (STACK_VEC_SAT)*4     ; xm.vlmacc0 c_re                       }
    xm.vlmacc1 c_re
    xm.vlsat vec_sat
        xm.vstrpv a_re, bytemask
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
        xm.vstrpv a_im, bytemask   
        xm.vstrpv vec_tmp, bytemask
    { nop                               ; xm.vldd vec_tmp                       }
    { nop                               ; xm.vstd vec_tmp                       }


.L_done:
        xm.lddsp  s3,s2,8
        xm.lddsp  s5,s4,16
        xm.lddsp  s7,s6,24

    { li a0, 15                         ; xm.vgetc t3                           }
    { xm.zexti t3, 5                    ; lw s8, 4                          (sp) }
    { sub a0, a0, t3                    ; xm.retsp (NSTACKWORDS)*4              }


.L_func_end:

.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME

#undef FUNCTION_NAME



#endif //defined(__VX4B__)



