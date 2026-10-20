// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.



#if defined(__VX4B__)

#include "../asm_helper.h"

/*

typedef struct {
    unsigned num_taps;
    right_shift_t shift;
    int16_t* coef;
    int16_t* state;
} filter_fir_s16_t;

int16_t filter_fir_s16(
    filter_fir_s16_t* filter,
    const int16_t new_sample);
*/

#define FUNCTION_NAME filter_fir_s16

#define NSTACKVECS      (3)
#define NSTACKWORDS     (8+8*NSTACKVECS)

#define FILT_N          0
#define FILT_SHIFT      1
#define FILT_COEF       2
#define FILT_STATE      3


#define STACK_VEC_TMP2  (NSTACKWORDS-8-1)
#define STACK_VEC_VR    (NSTACKWORDS-16-1)
#define STACK_VEC_TMP   (NSTACKWORDS-24-1)

#define buff        a0
#define length      a1
#define sample      a2
#define tmpA        a3
#define _32         s2
#define coef        s3

#define filter      s6

.text
.globl FUNCTION_NAME;
.type FUNCTION_NAME,@function
.p2align 4

FUNCTION_NAME:
    xm.entsp (NSTACKWORDS)*4

    xm.stdsp s2, s3, 0
    xm.stdsp s4, s5, 8

    { addi s4,sp, (STACK_VEC_TMP2)*4   ; nop                                   }
    add s5, s4, (-30)

    { li _32, 32                        ; sw s6, 16                          (sp) }
    { mv filter, a0                     ; mv sample, a1                         }
    { nop                               ; lw length,(FILT_N)*4              ( filter) }
    { nop                               ; lw buff,(FILT_STATE)*4            ( filter) }
        call filter_fir_s16_push_sample_up
    { nop                               ; lw coef,(FILT_COEF)*4             ( filter) }
    { nop                               ; lw buff,(FILT_STATE)*4            ( filter) }
    { nop                               ; lw length,(FILT_N)*4              ( filter) }
    { slli t3, _32, 3                   ; xm.vclrdr                             }
    { addi t3,sp, (STACK_VEC_TMP)*4     ; xm.vsetc t3                           }
    { mv tmpA, length                   ; xm.vstd t3                            }
    { xm.zexti tmpA, 4                  ; srli length, length, 4                }
    { slli tmpA, tmpA, 1                ; xm.brff length, .L_loop_end           }

    .L_loop_top:
        { add buff, buff, _32           ; xm.vldc buff                          }
        { addi length, length, -1       ; xm.vlmaccr0 coef                      }
        xm.vlmaccr1 coef
        { nop                           ; xm.vstd s4                            }
        { nop                           ; xm.vldd s5                            }
        { nop                           ; xm.vstr s4                            }
        { nop                           ; xm.vldr s5                            }

        { add coef, coef, _32           ; xm.bt length, .L_loop_top             }
    .L_loop_end:

    { addi a2,sp, (STACK_VEC_VR)*4      ; xm.mkmsk tmpA, tmpA                   }
    { nop                               ; xm.vstr a2                            }
    { addi t3,sp, (STACK_VEC_TMP)*4     ; xm.vldr buff                          }
        xm.vstrpv t3, tmpA
    { nop                               ; lw a2,(FILT_SHIFT)*4                  ( filter) }
    { addi t3,sp, (STACK_VEC_VR)*4      ; xm.vldc t3                            }
    { nop                               ; xm.vldr t3                            }
    { nop                               ; xm.vlmaccr0 coef                      }
    xm.vlmaccr1 coef

    sh a2, 0(t3)

    xm.vlsat t3
    { nop                               ; xm.vstr t3                            }
    lh a0, 0(t3)
.L_done:
    { nop                               ; lw s6, 16                          (sp) }

        xm.lddsp s2, s3, 0
        xm.lddsp s4, s5, 8
        xm.retsp (NSTACKWORDS)*4

.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_add FUNCTION_NAME, "callees", filter_fir_s16_push_sample_up
.resource_list_empty FUNCTION_NAME, "parallel_callees"
.L_size_end:
    .size FUNCTION_NAME, .L_size_end - FUNCTION_NAME

#undef FUNCTION_NAME



#endif //defined(__VX4B__)


