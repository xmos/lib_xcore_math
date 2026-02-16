// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.



#if defined(__VX4B__)

#include "../asm_helper.h"

/*  

typedef struct {
    unsigned biquad_count;
    int32_t state[2][9]; // state[j][k] is the value x_k[j], i.e.  x[n-j] of the kth biquad. x[j][8] are outputs of 8th biquad
    int32_t coef[5][8];  // coefficients. coef[j][k] is for the kth biquad. j maps to b0,b1,b2,-a1,-a2.
} filter_biquad_s32_t;

int32_t filter_biquad_sat_s32(
    filter_biquad_s32_t* filter,
    const int32_t new_sample);
*/

#define FUNCTION_NAME filter_biquad_sat_s32

#define NSTACKVECS      (2)
#define NSTACKWORDS     (4+8*NSTACKVECS)

#define STACK_TMP_VR    (NSTACKWORDS - 16-1)
#define STACK_TMP_VD    (NSTACKWORDS -  8-1)

#define FILT_N          0
#define FILT_STATE      1
#define FILT_COEF       19

#define COEF_START      32
#define STATE_START     10


#define state       a0      // ![0x%08X]
#define sample      a1      // ![%d]
#define coef        a2      // ![0x%08X]
#define tmp         a3      // ![%d]
#define _32         s2      // ![%d]
#define _36         s3      // ![%d]
#define filter      s8     // ![0x%08X]
    
.text
.globl FUNCTION_NAME;
.type FUNCTION_NAME,@function
.p2align 4

FUNCTION_NAME:
        xm.entsp (NSTACKWORDS)*4
        xm.stdsp  s3,s2,0
    { li t3, 0                          ; sw s8, 8                         (sp) }
    { mv filter, a0                     ; xm.vsetc t3                           }
    { xm.ldcu tmp, FILT_STATE + STATE_START ; nop                               }
        sh2add state, tmp, filter          // state <-- &(filter->state[1][1])
    { xm.ldcu tmp, FILT_COEF + COEF_START ; xm.vclrdr                           }
        sh2add coef, tmp, filter            // coef <-- &(filter->coef[4][0])

    { li _36, 36                        ; li _32, 32                            }

// Deal with the b2 and -a2 coefficients before b1 and -a1, so we can overwrite them easily.

    { sub state, state, _36             ; xm.vldc state                         }
    { sub coef, coef, _32               ; xm.vlmacc0 coef                       }
    { add state, state, _32             ; xm.vldc state                         }
    { sub coef, coef, _32               ; xm.vlmacc0 coef                       }
    { sub state, state, _36             ; xm.vldc state                         }
    { sub coef, coef, _32               ; xm.vlmacc0 coef                       }
    { nop                               ; xm.vldc state                         }
    { sub coef, coef, _32               ; xm.vlmacc0 coef                       }

    // Now acc[k] =  b1[k] * x[n-1][k] + b2[k] * x[n-2][k] - a1[k] * y[n-1][k] - a2[k] * y[n-2][k]
    // state = &(filter->state[0][0])
    // coef = &(filter->coef[0][0])

#undef _36
#define N       s3      // ![%d]

    // Move filter->state[0][:] to filter->state[1][:]

    { add t3, state, s3                 ; xm.vldc state                         }
    { add t3, t3, _32                   ; xm.vstc t3                            }
    { add tmp, state, _32               ; lw s2,(FILT_N)*4            ( filter) }
    { li s3, 6                          ; lw tmp,0                       ( tmp) }
    { li tmp, 6*8                       ; sw tmp,0                        ( t3) }
        mul N, s2, s3

    // Place the newest input sample in state[0][0]
    { sub N, tmp, N                     ; sw sample,0                  ( state) }

#undef sample
#define zeros a1

    // Overwrite state[0][1:9] with 0's
    la t3, vpu_vec_zero
    { addi zeros, t3, 0                 ; li _32, 32                            }
    { addi t3, state, 4                 ; xm.vldc t3                            }
    { nop                               ; xm.vstc t3                            }

    // vC[:] <-- coef[b0][:]
    { nop                               ; xm.vldc coef                          }

#undef coef
#define state_p1 a2

    { addi state_p1, t3, 0              ; addi t3,sp, (STACK_TMP_VR)*4          }

    // Every element in t3[0:8] except for t3[0] is zero, so a VLMACC shouldn't affect them.
    // Subsequent VLMACCs will corrupt the accumulators, but The Mask will stop that from being a
    // problem. Smokin'!

    // Let's make this more clear. We still haven't MACCed in the terms corresponding to b0,
    // but we can't do all of those simultaneously as we did with the others because the x[n-0]
    // for one section IS the output of the previous section, which we haven't finished calculating
    // yet. So we need to go up the chain of filter sections, computing the output of each to get
    // the input to the next. Because we've set the state[0][1:0] to zeros, when we're working on
    // the k'th filter section, MACCing against that will not affect accumulators > k. Then we write
    // out the output of section k. We do the MACC again, **which will corrupt the accumulators 
    // which are LESS THAN k.... but that's FINE because we're not going to write them out again.

    // instead of using _32 could just use that register as a second pointer for the vD temp stack,
    // wouldn't need to recalculate the stack pointer every time, doesn't            _       _
    // matter here as we're not using most of the resourse line instructions anyway   \(`~`)/

    { xm.mkmski tmp, 4                  ; xm.vlmacc0 state                      }
    { add t3, t3, _32                   ; xm.vstr t3                            }
    { nop                               ; xm.vstd t3                            }
        xm.vlsat zeros
        xm.vstrpv state_p1, tmp
li N, (0)
    { nop                               ; xm.bru N /* Do N-1 remaining biquads */ }

    { sub t3, t3, _32                   ; xm.vldd t3                            }
    { nop                               ; xm.vldr t3                            }
    { slli tmp, tmp, 4                  ; xm.vlmacc0 state                      }
    { add t3, t3, _32                   ; xm.vstr t3                            }
    { nop                               ; xm.vstd t3                            }
        xm.vlsat zeros
        xm.vstrpv state_p1, tmp


    { sub t3, t3, _32                   ; xm.vldd t3                            }
    { nop                               ; xm.vldr t3                            }
    { slli tmp, tmp, 4                  ; xm.vlmacc0 state                      }
    { add t3, t3, _32                   ; xm.vstr t3                            }
    { nop                               ; xm.vstd t3                            }
        xm.vlsat zeros
        xm.vstrpv state_p1, tmp


    { sub t3, t3, _32                   ; xm.vldd t3                            }
    { nop                               ; xm.vldr t3                            }
    { slli tmp, tmp, 4                  ; xm.vlmacc0 state                      }
    { add t3, t3, _32                   ; xm.vstr t3                            }
    { nop                               ; xm.vstd t3                            }
        xm.vlsat zeros
        xm.vstrpv state_p1, tmp


    { sub t3, t3, _32                   ; xm.vldd t3                            }
    { nop                               ; xm.vldr t3                            }
    { slli tmp, tmp, 4                  ; xm.vlmacc0 state                      }
    { add t3, t3, _32                   ; xm.vstr t3                            }
    { nop                               ; xm.vstd t3                            }
        xm.vlsat zeros
        xm.vstrpv state_p1, tmp


    { sub t3, t3, _32                   ; xm.vldd t3                            }
    { nop                               ; xm.vldr t3                            }
    { slli tmp, tmp, 4                  ; xm.vlmacc0 state                      }
    { add t3, t3, _32                   ; xm.vstr t3                            }
    { nop                               ; xm.vstd t3                            }
        xm.vlsat zeros
        xm.vstrpv state_p1, tmp


    { sub t3, t3, _32                   ; xm.vldd t3                            }
    { nop                               ; xm.vldr t3                            }
    { slli tmp, tmp, 4                  ; xm.vlmacc0 state                      }
    { add t3, t3, _32                   ; xm.vstr t3                            }
    { nop                               ; xm.vstd t3                            }
        xm.vlsat zeros
        xm.vstrpv state_p1, tmp


    { sub t3, t3, _32                   ; xm.vldd t3                            }
    { nop                               ; xm.vldr t3                            }
    { slli tmp, tmp, 4                  ; xm.vlmacc0 state                      }
    { add t3, t3, _32                   ; xm.vstr t3                            }
    { nop                               ; xm.vstd t3                            }
        xm.vlsat zeros
        xm.vstrpv state_p1, tmp

    // Final vstrpv should have written the output to filt->state[0][N]. filt->state should
    // still be pointing at filt->state[0][0]

    { nop                               ; lw N,(FILT_N)*4              ( filter) }
    xm.ldw a0,N                        ( state)
    { nop                               ; lw s8, 8                          (sp) }
        xm.lddsp  s3,s2,0
        xm.retsp (NSTACKWORDS)*4

//.cc_bottom FUNCTION_NAME.function; 
.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
.L_size_end:
    .size FUNCTION_NAME, .L_size_end - FUNCTION_NAME

#undef FUNCTION_NAME



#endif //defined(__VX4B__)
