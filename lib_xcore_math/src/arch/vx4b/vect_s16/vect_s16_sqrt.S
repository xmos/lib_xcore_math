// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"
#define XXX 1
/*  

headroom_t vect_s16_sqrt(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const right_shift_t b_shr,
    const unsigned depth);

*/


#define NSTACKVECTS     (4)
#define NSTACKWORDS     (12+8*(NSTACKVECTS))

#define FUNCTION_NAME   vect_s16_sqrt

// Temporary vector needed because there's no instruction to do vR[] * vR[]
#define STACK_VEC_TMP       (NSTACKWORDS-8-1)
// Holds the shifted values of b[] while we're solving it.
#define STACK_VEC_TARGET    (NSTACKWORDS-16-1)
// Holds the power of 2 that is currently being worked on inside hte inner loop.
// @todo If we had an instruction that set each vR[k] to the value of a register, this wouldn't be needed.
#define STACK_VEC_POW       (NSTACKWORDS-24-1)

#define STACK_VEC_TMP2       (NSTACKWORDS-32-1)

#define STACK_DEPTH     6

#define a           a0
#define b           a1
#define length      a2
#define b_shr       a3

#define depth       s2 //s2
#define mask_vec    s3 //s3
#define _32         s4 //s4
#define pow_init    s5 //s5
#define tmp         s8 //s8
#define spare       s7 //s7

.text
.p2align 2


FUNCTION_NAME:
    xm.entsp (NSTACKWORDS)*4
    xm.stdsp  s3,s2,8
    xm.stdsp  s5,s4,16
    xm.stdsp  s7,s8,0                        
    { addi spare,sp, (STACK_VEC_TMP2)*4 ; nop                                   }

    sw a4, (STACK_DEPTH)*4(sp)
// Set VPU mode to 32-bit
// (length << 1) is the length of the vector in bytes.
{ li _32, 32                            ; nop                                   }
{ slli t3, _32, 3                       ; nop                                   }
{ slli length, length, 1                ; xm.vsetc t3                           }
    la t3, vpu_vec_0x4000
    //la t3, vpu_vec_0x7FFF
{ mv pow_init, t3                       ; nop                                   }


// Maximum supported depth is 15
{ li tmp, 15                            ; lw t3, (STACK_DEPTH)*4                (sp) }
{ xm.assert t3                          ; nop                                   }
{ xm.sltu t3, tmp, t3                   ; nop                                   }
{ li _32, 32                            ; xm.brff t3, .L_vect_loop_top          }
    { nop                               ; sw tmp, (STACK_DEPTH)*4                (sp) }


.L_vect_loop_top:

    // mask_vec is a byte mask for the elements of a[] that we're currently working on.
    // using VSTRPV with mask_vec prevents us from corrupting the headroom register.
    // depth is the number of MSBs that we're solving for
    { xm.mkmsk mask_vec, length         ; lw depth, (STACK_DEPTH)*4              (sp) }

    // First initialize the target vector using b[]
    // (Doing this first allows this function to operate in-place on b[] if desired)
    // @todo If we wanted to, we could do a VSIGN + VLMUL here to take an absolute value of each b[k],
    //       since this function will not work for any negative b[k].
        xm.vlashr b, b_shr
    { addi t3,sp, (STACK_VEC_TARGET)*4  ; add b, b, _32                         }
        xm.vstrpv t3, mask_vec

    // Initialize the result (a[]) with 0's
    { mv t3, pow_init                   ; xm.vclrdr                             }
        xm.vstrpv a, mask_vec

    // VEC_POW[] is the bit we're currently solving for. Initialize to the first non-sign bit.
    // (The VSTD is to zero out the VEC_POW[] elements that are going to be masked out, because
    //  we're going to use VEC_POW[] later to update the headroom register)
    { addi t3,sp, (STACK_VEC_POW)*4     ; xm.vldr t3                            }
    { li tmp, 1                         ; xm.vstd t3                            }
        xm.vstrpv t3, mask_vec 

    // This saves us a few cycles on the first iteration (because of loop alignment, we'd need a 
    // 'bu .L_sqrt_loop_top' here even if we didn't want to skip ahead). It's necessary because 
    // we don't want to right-shift VEC_POW[] on the first iteration (it's already 2^15), and we 
    // can't fix that by initializing VEC_POW[] to 0x8000 above because that's negative and 
    // VLASHR is an arithmetic shift.
    { addi t3,sp, (STACK_VEC_TARGET)*4  ; xm.bu .L_first_iter                   }

    // Inner loop. Iteratively solving for the square root bit-by-bit
    // 12 instructions + 1 FNOP
    .p2align 4
    .L_sqrt_loop_top:
        // Load the next power of 2 and store it back to VEC_POW[]
            xm.vlashr t3, tmp
            xm.vstrpv t3, mask_vec

        // Add the current power of 2 to each a[] to get the next value to be tested.
        // test[k] <-- a[k] + VEC_POW
        { addi t3,sp, (STACK_VEC_TMP)*4 ; xm.vladd a                            }

         

        // vR[] contains the values we're testing. Store it and square it
        // vR[k] <-- ( test[k] * test[k] ) >> 14
            xm.vstrpv t3, mask_vec
        { nop                           ; xm.vlmul0 t3                          }
        xm.vlmul1 t3
        xm.vstrpv spare, mask_vec
        { xm.vladd spare                ; nop                                   }

        { addi t3,sp, (STACK_VEC_TARGET)*4 ; nop                                   }
        .L_first_iter:

        // Subtract the squared test values from the target vector   
        // vR[k] <-- target[k] - (( test[k] * test[k] ) >> 30)
        { nop                           ; xm.vlsub t3                           }

        // If vR[k] is negative, the test value was too large, so we don't want to update those a[k]
        // for which vR[k] is negative.

        //  vR[k] = a[k] + MAX( signum( vR[k] ), 0 ) * VEC_POW[k]

        { addi depth, depth, -1         ; xm.vsign                              }
        { addi t3,sp, (STACK_VEC_POW)*4 ; xm.vpos                               }
        
        xm.vstrpv spare, mask_vec
        { xm.vladd spare                ; nop                                   }
                        
        { li tmp, 1                     ; xm.vlmul0 t3                          }
        xm.vlmul1 t3
        { nop                           ; xm.vladd a                            }

        // Store the updated results in a[]
            xm.vstrpv a, mask_vec
        { nop                           ; xm.bt depth, .L_sqrt_loop_top         }
    .L_sqrt_loop_bot:

    // a[] now contains the results, but we haven't updated the headroom register because we've only
    // been using VSTRPV. So, update the headroom register
    // @todo Do we need to update the headroom register? Aren't we more or less guaranteed there's no
    // headroom, because we got rid of the headroom of b[]? Should work out the math on this later.
    
    // We used mask_vec when initializing VEC_POW[], so we can use that here to avoid corrupting
    // the headroom register with data that comes after a[]. t3 is already pointing at VEC_POW[].
        xm.vstrpv t3, mask_vec
    { sub length, length, _32           ; xm.vldr t3                            }

    // If (length - 32) < 1 we're done.
    { xm.slt tmp, length, tmp           ; xm.vstr t3                            }
    { add a, a, _32                     ; nop                                   }
    beqz tmp, .L_vect_loop_top
.L_vect_loop_bot:

.L_finish:

    xm.lddsp  s3,s2,8
    xm.lddsp  s5,s4,16
    xm.lddsp  s7,s8,0
{ li a0, 15                             ; xm.vgetc t3                           }
{ xm.zexti t3, 5                        ; nop                                   }
{ sub a0, a0, t3                        ; xm.retsp (NSTACKWORDS)*4              } 


.L_func_end:


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME


#endif //defined(__VX4B__)



