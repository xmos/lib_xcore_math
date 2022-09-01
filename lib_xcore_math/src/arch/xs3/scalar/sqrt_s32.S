// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


#include "../asm_helper.h"

.text
.issue_mode dual
.align 4

/*  
int32_t s32_sqrt( 
    exponent_t* y_exp,
    const int32_t X,
    const exponent_t x_exp,
    const unsigned depth);

    @todo This can probably be sped up ~25% by using the VPU to compute 3 bits at a time. 
          (The speedup would be more significant if there was a quick way to create an element mask (vdepth1 creates a
           byte mask) and a way to load each vR[k] from a single register.
*/

#define FUNCTION_NAME   s32_sqrt
#define NSTACKWORDS     (8)


#define y_exp   r0
#define X       r1
#define x_exp   r2
#define depth   r3
#define tmp     r5




.cc_top FUNCTION_NAME.function,FUNCTION_NAME
FUNCTION_NAME:
    dualentsp NSTACKWORDS
    std r4, r5, sp[1]
    std r6, r7, sp[2]
    std r8, r9, sp[3]


{   cls tmp, X                  ;   stw r10, sp[1]              }
{   sub tmp, tmp, 1             ;   ldc r11, 31                 }
{   shl X, X, tmp               ;   sub tmp, x_exp, tmp         }
{   sub tmp, tmp, r11           ;   sub x_exp, tmp, r11         }
{   zext tmp, 1                 ;                               }
ldaw r11, cp[vpu_vec_0x80000000]
{                               ;   ldw r11, r11[0]             }
{                               ;   bf tmp, .L_is_even          }
        ashr r11, r11, 1
    {   add x_exp, x_exp, 1         ;                               }

.L_is_even:
    ashr x_exp, x_exp, 1
{                               ;   stw x_exp, y_exp[0]         }

#undef x_exp
#undef y_exp

#define targ_hi     r0
#define targ_lo     r1
#define result      r2
#define guess       r4
#define base        r6
#define acc_hi      r7
#define acc_lo      r8
#define a_exp       r9


{   mov tmp, r11                ;   mov r11, X                  }
{   ldc targ_hi, 0              ;   ldc targ_lo, 0              }
    maccs targ_hi, targ_lo, tmp, r11

#undef X

// Subtract just one more from targ_hi:targ_lo, so that we're doing <= instead of just <
{   ldc tmp, 1                  ;   mkmsk r11, 32               }
    maccs targ_hi, targ_lo, tmp, r11

    ldc base, 0x40
{   ldc result, 0               ;   shl base, base, 24          }

// @todo can potentially save a little bit of time by doing a clz on targ_hi. Might be able to skip the first iteration

.L_loop_top:
    {   mov acc_hi, targ_hi         ;   mov acc_lo, targ_lo         }
    {   add tmp, result, base       ;   sub depth, depth, 1         }
        maccs acc_hi, acc_lo, tmp, tmp
    {   clz acc_hi, acc_hi          ;                               }
    {   shr base, base, 1           ;   bt acc_hi, .L_too_large     }
        {   mov result, tmp             ;                               }
    .L_too_large:
    {                               ;   bt depth, .L_loop_top       }
.L_loop_end:

    ldd r8, r9, sp[3]
    ldd r6, r7, sp[2]
    ldd r4, r5, sp[1]
{                               ;   ldw r10, sp[1]              }
{   mov r0, result              ;   retsp NSTACKWORDS           }

.L_func_end:
.cc_bottom FUNCTION_NAME.function

.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME,.L_func_end - FUNCTION_NAME













#endif //defined(__XS3A__)

