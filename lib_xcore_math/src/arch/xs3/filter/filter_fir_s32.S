// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.



#if defined(__XS3A__)

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
#define NSTACKWORDS     (10+8*NSTACKVECS)

#define FILT_N          0
#define FILT_HEAD       1
#define FILT_SHIFT      2
#define FILT_COEF       3
#define FILT_STATE      4


#define STACK_VEC_TMP   (NSTACKWORDS-8)


#define filter      r9
#define sample      r1
#define tmp1        r4
#define tmp2        r8    
    
.text
.issue_mode dual
.globl FUNCTION_NAME;
.type FUNCTION_NAME,@function
.align 16
.cc_top FUNCTION_NAME.function,FUNCTION_NAME

FUNCTION_NAME:
        dualentsp NSTACKWORDS
        std r4, r5, sp[1]
        std r6, r7, sp[2]
        std r8, r9, sp[3]
    {   ldc r11, 0                              ;   stw r10, sp[1]                          }

    // Set VPU mode to 32-bit
    {   mov filter, r0                          ;   vsetc r11                               }


// The field filter->head points to where the newest sample will go, which is probably somewhere in the middle of the 
// state vector. This effectively splits the work to be done into two pieces -- the stuff after filter->head, and the 
// stuff before it. The stuff after filter->head I'm calling part A (corresponds to lowest coef[] indices). The stuff 
// before it I'm calling part B. 

// I'm just going to create two sets of registers, corresponding to each of the two parts. That's what this is.

#define state_A     r0
#define state_B     r5

#define N_A         r2    
#define N_B         r7

#define coef_A      r1
#define coef_B      r6

    // Get the current head position, which is also the number of taps in part B
    {                                           ;   ldw N_B, filter[FILT_HEAD]              }

    // If N_B is currently zero, then the next head is the final index. Otherwise it's just
    // the head decremented by 1.
    {   sub r11, N_B, 1                         ;   ldw N_A, filter[FILT_N]                 }
    {                                           ;   bt N_B, .L_no_reset                     }
    {   sub r11, N_A, 1                         ;                                           }

    .L_no_reset:
    {                                           ;   stw r11, filter[FILT_HEAD]              }

    // Store the newest sample in the state. And grab the rest of the state/coef/N values
    {                                           ;   ldw state_B, filter[FILT_STATE]         }
        ldaw state_A, state_B[N_B]
    {   sub N_A, N_A, N_B                       ;   stw sample, state_A[0]                  }
    {   shl tmp1, N_A, 2                        ;   ldw coef_A, filter[FILT_COEF]           }
        ldaw coef_B, coef_A[N_A]
#undef sample

    // Each part has its own tail. We'll handle both of those first (by masking the state with zeros), then we'll do the 
    // bulk of the work after

    {   ldaw r10, sp[STACK_VEC_TMP]             ;   vclrdr                                  }
    {   mov r11, state_A                        ;   vstd r10[0]                             }
    {   zext tmp1, 5                            ;   vldr r11[0]                             }
    {   mkmsk r11, tmp1                         ;   shr N_A, N_A, 3                         }
        vstrpv r10[0], r11
    {   mov r11, state_B                        ;   vldc r10[0]                             }
    {   shl tmp2, N_B, 2                        ;   vldr r11[0]                             }
    {   zext tmp2, 5                            ;   vstd r10[0]                             }
    {   mkmsk r11, tmp2                         ;   shr N_B, N_B, 3                         }
        vstrpv r10[0], r11
    {   add state_A, state_A, tmp1              ;   vclrdr                                  }
    {   add coef_A, coef_A, tmp1                ;   vlmaccr coef_A[0]                       }
    {   add state_B, state_B, tmp2              ;   vldc r10[0]                             }
    {   add coef_B, coef_B, tmp2                ;   vlmaccr coef_B[0]                       }

// Now, go back through and do full vectors.

#undef tmp2
#define _32     r8

    bu .L_part_A_start
    .align 16
    .L_part_A_start:
        {   ldc _32, 32                             ;   bf N_A, .L_part_A_end                   }
        .L_part_A_loop_top:
            {   add state_A, state_A, _32               ;   vldc state_A[0]                         }
            {   sub N_A, N_A, 1                         ;   vlmaccr coef_A[0]                       }
            {   add coef_A, coef_A, _32                 ;   bt N_A, .L_part_A_loop_top              }
    .L_part_A_end:
#undef state_A
#undef N_A
#undef coef_A

    .L_part_B_start:
        {   ldc _32, 32                             ;   bf N_B, .L_part_B_end                   }
        .L_part_B_loop_top:
            {   add state_B, state_B, _32               ;   vldc state_B[0]                         }
            {   sub N_B, N_B, 1                         ;   vlmaccr coef_B[0]                       }
            {   add coef_B, coef_B, _32                 ;   bt N_B, .L_part_B_loop_top              }
    .L_part_B_end:
    
#undef state_B
#undef N_B
#undef coef_B

// Now combine the 40-bit accumulators.
// (the logic for this is a too complicated to explain here)
        ldaw r11, cp[vpu_vec_0x40000000]
    {                                           ;   ldw r2, filter[FILT_SHIFT]              }
    {   ldaw r11, sp[STACK_VEC_TMP]             ;   vldc r11[0]                             }
    {                                           ;   vstr r11[0]                             }
        ldaw r11, cp[vpu_vec_0x80000000]
    {                                           ;   vlmacc r11[0]                           }
        ldaw r11, cp[vpu_vec_zero]
    {   ldaw r11, sp[STACK_VEC_TMP]             ;   vldr r11[0]                             }
    {   sub r4, r2, 1                           ;   vlmaccr r11[0]                          }
    {   ldc r10, 0                              ;   vstd r11[0]                             }
    {   lss r3, r10, r2                         ;   vlmaccr r11[0]                          }
    {   ldc r5, 1                               ;   vstr r11[0]                             }
    {   shl r4, r5, r4                          ;   ldw r1, r11[0]                          }
    {   add r1, r1, 8                           ;   ldw r0, r11[1]                          }

// r1 and r0 now contain a 64-bit result. Left or right-shift that as appropriate.
    {   neg r3, r2                              ;   bf r3, .L_left_shift                    }
    
    .L_right_shift:
        // (from the block above):  r5 = 1      r4 = 1<<(r2-1)
        // adding r4*r5 (=r4) to r1:r0 effectively rounds it when we extract it.
        maccs r1, r0, r4, r5
        lsats r1, r0, r2
        lextract r0, r1, r0, r2, 32
        bu .L_done

    .L_left_shift:

    // If we're left-shifting, assume there's nothing of interest in r1. Basically, assume 
    //      -(2**31) <= r0 < 2**31
    {   shl r0, r0, r3                          ;   bu .L_done                              }
.L_pre_done:
    {   ldc r0, 0                               ;                                           }

.L_done:
        ldd r8, r9, sp[3]
        ldd r6, r7, sp[2]
        ldd r4, r5, sp[1]
    {                                           ;   ldw r10, sp[1]                          }
        retsp NSTACKWORDS

.cc_bottom FUNCTION_NAME.function; 
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords; 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores; 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers; 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends; 
.L_size_end:
    .size FUNCTION_NAME, .L_size_end - FUNCTION_NAME

#undef FUNCTION_NAME



#endif //defined(__XS3A__)



