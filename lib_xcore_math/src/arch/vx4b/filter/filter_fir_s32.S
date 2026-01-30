// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.



#if defined(__VX4B__)

#include "../asm_helper.h"

/*  

typedef struct {
    unsigned num_taps;
    unsigned head;
    right_shift_t shift;
    int32_t* coef;
    int32_t* state;
} filter_fir_s32_t;

int32_t filter_fir_s32(
    filter_fir_s32_t* filter,
    const int32_t new_sample);
*/

#define FUNCTION_NAME filter_fir_s32

#define NSTACKVECS      (1)
#define NSTACKWORDS     (8+8*NSTACKVECS)

#define FILT_N          0
#define FILT_HEAD       1
#define FILT_SHIFT      2
#define FILT_COEF       3
#define FILT_STATE      4


#define STACK_VEC_TMP   (NSTACKWORDS-8-1)


#define filter      x23
#define sample      x11
#define tmp1        x18
#define tmp2        x22    
    
.text
.globl FUNCTION_NAME;
.type FUNCTION_NAME,@function
.p2align 4

FUNCTION_NAME:
    xm.entsp (NSTACKWORDS)*4
    xm.stdsp  s3,s2,8
    xm.stdsp  s5,s4,16
    xm.stdsp  s7,s6,0
    { li t3, 0                          ; sw s8, 24                          (sp) }

    // Set VPU mode to 32-bit
    { mv filter, a0                     ; xm.vsetc t3                           }


// The field filter->head points to where the newest sample will go, which is probably somewhere in the middle of the 
// state vector. This effectively splits the work to be done into two pieces -- the stuff after filter->head, and the 
// stuff before it. The stuff after filter->head I'm calling part A (corresponds to lowest coef[] indices). The stuff 
// before it I'm calling part B. 

// I'm just going to create two sets of registers, corresponding to each of the two parts. That's what this is.

#define state_A     x10
#define state_B     x19

#define N_A         x12    
#define N_B         x21

#define coef_A      x11
#define coef_B      x20

    // Get the current head position, which is also the number of taps in part B
    { nop                               ; lw N_B,(FILT_HEAD)*4        ( filter) }

    // If N_B is currently zero, then the next head is the final index. Otherwise it's just
    // the head decremented by 1.
    { addi t3, N_B, -1                  ; lw N_A,(FILT_N)*4           ( filter) }
    { nop                               ; xm.bt N_B, .L_no_reset                }
    { addi t3, N_A, -1                  ; nop                                   }

    .L_no_reset:
    { nop                               ; sw t3,(FILT_HEAD)*4         ( filter) }

    // Store the newest sample in the state. And grab the rest of the state/coef/N values
    { nop                               ; lw state_B,(FILT_STATE)*4   ( filter) }
    sh2add state_A, N_B, state_B
    { sub N_A, N_A, N_B                 ; sw sample,0                ( state_A) }
    { slli tmp1, N_A, 2                 ; lw coef_A,(FILT_COEF)*4     ( filter) }
    sh2add coef_B, N_A, coef_A
#undef sample

    // Each part has its own tail. We'll handle both of those first (by masking the state with zeros), then we'll do the 
    // bulk of the work after

    { addi s8,sp, (STACK_VEC_TMP)*4     ; xm.vclrdr                             }
    { mv t3, state_A                    ; xm.vstd s8                            }
    { xm.zexti tmp1, 5                  ; xm.vldr t3                            }
    { xm.mkmsk t3, tmp1                 ; srli N_A, N_A, 3                      }
     xm.vstrpv s8, t3
    { mv t3, state_B                    ; xm.vldc s8                            }
    { slli tmp2, N_B, 2                 ; xm.vldr t3                            }
    { xm.zexti tmp2, 5                  ; xm.vstd s8                            }
    { xm.mkmsk t3, tmp2                 ; srli N_B, N_B, 3                      }
    xm.vstrpv s8, t3
    { add state_A, state_A, tmp1        ; xm.vclrdr                             }
    { add coef_A, coef_A, tmp1          ; xm.vlmaccr0 coef_A                    }
    { add state_B, state_B, tmp2        ; xm.vldc s8                            }
    { add coef_B, coef_B, tmp2          ; xm.vlmaccr0 coef_B                    }

// Now, go back through and do full vectors.

#undef tmp2
#define _32     x22

    .p2align 4
    .L_part_A_start:
        { li _32, 32                    ; xm.brff N_A, .L_part_A_end            }
        .L_part_A_loop_top:
            { add state_A, state_A, _32 ; xm.vldc state_A                       }
            { addi N_A, N_A, -1         ; xm.vlmaccr0 coef_A                    }
            { add coef_A, coef_A, _32   ; xm.bt N_A, .L_part_A_loop_top         }
    .L_part_A_end:
#undef state_A
#undef N_A
#undef coef_A

    .L_part_B_start:
        { li _32, 32                    ; xm.brff N_B, .L_part_B_end            }
        .L_part_B_loop_top:
            { add state_B, state_B, _32 ; xm.vldc state_B                       }
            { addi N_B, N_B, -1         ; xm.vlmaccr0 coef_B                    }
            { add coef_B, coef_B, _32   ; xm.bt N_B, .L_part_B_loop_top         }
    .L_part_B_end:
    
#undef state_B
#undef N_B
#undef coef_B

// Now combine the 40-bit accumulators, assumes that x24 points to the stack.
// (the logic for this is a too complicated to explain here)
    la t3, vpu_vec_0x40000000
    { nop                               ; lw a2,(FILT_SHIFT)*4        ( filter) }
    { addi s2, a2, -1                   ; xm.vldc t3                            }
    { li s3, 1                          ; xm.vstr s8                            }
    la t3, vpu_vec_0x80000000
    { xm.shl s2, s3, s2                 ; xm.vlmacc0 t3                         }
    la t3, vpu_vec_zero
    { li t3, 0                          ; xm.vldr t3                            }
    { xm.slt a3, t3, a2                 ; xm.vlmaccr0 s8                        }
    { nop                               ; xm.vstd x24                           }
    { xm.neg x20, x12                   ; nop                                   }
    { addi s4, s4, 1                    ; xm.vlmaccr0 s8                        }
    { nop                               ; xm.vstr s8                            }

// x11 and x10 will contain a 64-bit result. Left or right-shift that as appropriate.

    xm.lddi  a1,a0, 0(s8)
    { addi a1, a1, 8                    ; xm.brff a3, .L_left_shift             }

    .L_right_shift:
        // (from the block above):  x19 = 1, x18 = 1<<(x12 - 1)
        // adding x18*x19 (=x18) to x11:x10 effectively rounds it when we extract it.
        xm.maccs a1, a0, s2, s3
        xm.lsats a1, a0, a2
        xm.lextract a0, a1, a0, a2, 32
    { nop                               ; xm.bu .L_done                         }

    .L_left_shift:
        // (from the block above):  x19 = 1, x20 = -x12 + 1, t3 = 0
        // If we're left-shifting (or zero-shifting), we still need to saturate to q31.
        // lsats has a bug which doesn't allow to use it with 0, so we'll have to 
        // add 1 to our shift, left-shift, saturate and extract with 1, no need to round here.
    { xm.shl a1, a1, s4                 ; nop                                   }
    xm.linsert a1, t3, a0, s4, 32
    xm.lsats a1, t3, s3
    xm.lextract a0, a1, t3, s3, 32

.L_done:
        xm.lddsp  s7,s6,0
        xm.lddsp  s5,s4,16
        xm.lddsp  s3,s2,8
    { nop                               ; lw s8, 24                          (sp) }
        xm.retsp (NSTACKWORDS)*4

//.cc_bottom FUNCTION_NAME.function; 
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords; 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores; 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers; 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends; 
.L_size_end:
    .size FUNCTION_NAME, .L_size_end - FUNCTION_NAME

#undef FUNCTION_NAME

#endif //defined(__VX4B__)
