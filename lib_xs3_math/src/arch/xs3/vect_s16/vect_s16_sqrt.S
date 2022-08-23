// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


#include "../asm_helper.h"

/*  

headroom_t vect_s16_sqrt(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const right_shift_t b_shr,
    const unsigned depth);

    @todo There's no reason the 16- and 32-bit versions can't be combined into one function. They're almost identical.

*/


#define NSTACKVECTS     (3)
#define NSTACKWORDS     (10+8*(NSTACKVECTS))

#define FUNCTION_NAME   vect_s16_sqrt

// Temporary vector needed because there's no instruction to do vR[] * vR[]
#define STACK_VEC_TMP       (NSTACKWORDS-8)
// Holds the shifted values of b[] while we're solving it.
#define STACK_VEC_TARGET    (NSTACKWORDS-16)
// Holds the power of 2 that is currently being worked on inside hte inner loop.
// @todo If we had an instruction that set each vR[k] to the value of a register, this wouldn't be needed.
#define STACK_VEC_POW       (NSTACKWORDS-24)

#define STACK_DEPTH     NSTACKWORDS+1

#define a           r0
#define b           r1
#define length      r2
#define b_shr       r3

#define depth       r4
#define mask_vec    r5
#define _32         r6
#define pow_init    r7
#define tmp         r10

.text
.issue_mode dual
.align 4

.cc_top FUNCTION_NAME.function,FUNCTION_NAME

FUNCTION_NAME:
    dualentsp NSTACKWORDS
    std r4, r5, sp[1]
    std r6, r7, sp[2]
    std r8, r9, sp[3]

// Set VPU mode to 32-bit
// (length << 1) is the length of the vector in bytes.
{   ldc _32, 32                             ;                                           }
{   shl r11, _32, 3                         ;   stw r10, sp[1]                          }
{   shl length, length, 1                   ;   vsetc r11                               }
    ldaw r11, cp[vpu_vec_0x4000]
{   mov pow_init, r11                       ;                                           }


// Maximum supported depth is 15
{   ldc tmp, 15                             ;   ldw r11, sp[STACK_DEPTH]                }
{   ecallf r11                              ;                                           }
{   lsu r11, tmp, r11                       ;                                           }
{   ldc _32, 32                             ;   bf r11, .L_vect_loop_top                }
    {                                           ;   stw tmp, sp[STACK_DEPTH]                }


.L_vect_loop_top:

    // mask_vec is a byte mask for the elements of a[] that we're currently working on.
    // using VSTRPV with mask_vec prevents us from corrupting the headroom register.
    // depth is the number of MSBs that we're solving for
    {   mkmsk mask_vec, length                  ;   ldw depth, sp[STACK_DEPTH]              }

    // First initialize the target vector using b[]
    // (Doing this first allows this function to operate in-place on b[] if desired)
    // @todo If we wanted to, we could do a VSIGN + VLMUL here to take an absolute value of each b[k],
    //       since this function will not work for any negative b[k].
        vlashr b[0], b_shr
    {   ldaw r11, sp[STACK_VEC_TARGET]          ;   add b, b, _32                           }
        vstrpv r11[0], mask_vec

    // Initialize the result (a[]) with 0's
    {   mov r11, pow_init                       ;   vclrdr                                  }
        vstrpv a[0], mask_vec

    // VEC_POW[] is the bit we're currently solving for. Initialize to the first non-sign bit.
    // (The VSTD is to zero out the VEC_POW[] elements that are going to be masked out, because
    //  we're going to use VEC_POW[] later to update the headroom register)
    {   ldaw r11, sp[STACK_VEC_POW]             ;   vldr r11[0]                             }
    {   ldc tmp, 1                              ;   vstd r11[0]                             }
        vstrpv r11[0], mask_vec 

    // This saves us a few cycles on the first iteration (because of loop alignment, we'd need a 
    // 'bu .L_sqrt_loop_top' here even if we didn't want to skip ahead). It's necessary because 
    // we don't want to right-shift VEC_POW[] on the first iteration (it's already 2^15), and we 
    // can't fix that by initializing VEC_POW[] to 0x8000 above because that's negative and 
    // VLASHR is an arithmetic shift.
    {   ldaw r11, sp[STACK_VEC_TARGET]          ;   bu .L_first_iter                        }

    // Inner loop. Iteratively solving for the square root bit-by-bit
    // 12 instructions + 1 FNOP
    .align 16
    .L_sqrt_loop_top:

        // Load the next power of 2 and store it back to VEC_POW[]
            vlashr r11[0], tmp
            vstrpv r11[0], mask_vec

        // Add the current power of 2 to each a[] to get the next value to be tested.
        // test[k] <-- a[k] + VEC_POW
        {   ldaw r11, sp[STACK_VEC_TMP]             ;   vladd a[0]                              }


        // vR[] contains the values we're testing. Store it and square it
        // vR[k] <-- ( test[k] * test[k] ) >> 14
            vstrpv r11[0], mask_vec
        {   ldaw r11, sp[STACK_VEC_TARGET]          ;   vlmul r11[0]                            }

        .L_first_iter:

        // Subtract the squared test values from the target vector   
        // vR[k] <-- target[k] - (( test[k] * test[k] ) >> 30)
        {                                           ;   vlsub r11[0]                            }

        // If vR[k] is negative, the test value was too large, so we don't want to update those a[k]
        // for which vR[k] is negative.

        //  vR[k] = a[k] + MAX( signum( vR[k] ), 0 ) * VEC_POW[k]

        {   sub depth, depth, 1                     ;   vsign                                   }
        {   ldaw r11, sp[STACK_VEC_POW]             ;   vpos                                    }
        {   ldc tmp, 1                              ;   vlmul r11[0]                            }
        {                                           ;   vladd a[0]                              }

        // Store the updated results in a[]
            vstrpv a[0], mask_vec
        {                                           ;   bt depth, .L_sqrt_loop_top              }
    .L_sqrt_loop_bot:

    // a[] now contains the results, but we haven't updated the headroom register because we've only
    // been using VSTRPV. So, update the headroom register
    // @todo Do we need to update the headroom register? Aren't we more or less guaranteed there's no
    // headroom, because we got rid of the headroom of b[]? Should work out the math on this later.
    
    // We used mask_vec when initializing VEC_POW[], so we can use that here to avoid corrupting
    // the headroom register with data that comes after a[]. r11 is already pointing at VEC_POW[].
        vstrpv r11[0], mask_vec
    {   sub length, length, _32                 ;   vldr r11[0]                             }

    // If (length - 32) < 1 we're done.
    {   lss tmp, length, tmp                    ;   vstr r11[0]                             }
    {   add a, a, _32                           ;   bf tmp, .L_vect_loop_top                }

.L_vect_loop_bot:

.L_finish:

    ldd r4, r5, sp[1]
    ldd r6, r7, sp[2]
    ldd r8, r9, sp[3]
{   ldc r0, 15                              ;   vgetc r11                           }
{   zext r11, 5                             ;   ldw r10, sp[1]                      }
{   sub r0, r0, r11                         ;   retsp NSTACKWORDS                   } 


.L_func_end:
.cc_bottom FUNCTION_NAME.function


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME


#endif //defined(__XS3A__)



