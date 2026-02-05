// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

.text
.p2align 2

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


#define y_exp   a0
#define X       a1
#define x_exp   a2
#define depth   a3
#define tmp     s3




FUNCTION_NAME:
    xm.entsp (NSTACKWORDS)*4
    xm.stdsp  s3,s2,8
    xm.stdsp  s5,s4,16
    xm.stdsp  s7,s6,0


{ xm.cls tmp, X                         ; nop                                   }
 xm.stwsp s8, 24      
{ addi tmp, tmp, -1                     ; li t3, 31                             }
{ xm.shl X, X, tmp                      ; sub tmp, x_exp, tmp                   }
{ sub tmp, tmp, t3                      ; sub x_exp, tmp, t3                    }
{ xm.zexti tmp, 1                       ; nop                                   }
la t3, vpu_vec_0x80000000
{ nop                                   ; lw t3,0             ( t3)             }
{ nop                                   ; xm.brff tmp, .L_is_even               }
        srai t3, t3, 1
    { addi x_exp, x_exp, 1              ; nop                                   }

.L_is_even:
    srai x_exp, x_exp, 1
{ nop                                   ; sw x_exp,0         ( y_exp)           }

#undef x_exp
#undef y_exp

#define targ_hi     a0
#define targ_lo     a1
#define result      a2
#define guess       s2
#define base        s4
#define acc_hi      s5
#define acc_lo      s6
#define a_exp       s7


{ mv tmp, t3                            ; mv t3, X                              }
{ li targ_hi, 0                         ; li targ_lo, 0                         }
    xm.maccs targ_hi, targ_lo, tmp, t3

#undef X

// Subtract just one more from targ_hi:targ_lo, so that we're doing <= instead of just <
{ li tmp, 1                             ; xm.mkmski t3, 32                      }
    xm.maccs targ_hi, targ_lo, tmp, t3

    li base, 0x40
{ li result, 0                          ; slli base, base, 24                   }

// @todo can potentially save a little bit of time by doing a clz on targ_hi. Might be able to skip the first iteration

.L_loop_top:
    { mv acc_hi, targ_hi                ; mv acc_lo, targ_lo                    }
    { add tmp, result, base             ; addi depth, depth, -1                 }
        xm.maccs acc_hi, acc_lo, tmp, tmp
    { xm.clz acc_hi, acc_hi             ; nop                                   }
    { srli base, base, 1                ; xm.bt acc_hi, .L_too_large            }
        { mv result, tmp                ; nop                                   }
    .L_too_large:
    { nop                               ; xm.bt depth, .L_loop_top              }
.L_loop_end:

    xm.lddsp  s7,s6,0
    xm.lddsp  s5,s4,16
    xm.lddsp  s3,s2,8
{ nop                                   ; lw s8, 24              (sp)           }
{ mv a0, result                         ; xm.retsp (NSTACKWORDS)*4              }

.L_func_end:

.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME,.L_func_end - FUNCTION_NAME













#endif //defined(__VX4B__)

