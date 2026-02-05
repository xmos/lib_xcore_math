// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


/*  
    void vect_complex_s32_tail_reverse(
        complex_s32_t* X,
        const unsigned N);
*/

#include "../asm_helper.h"

#define NSTACKWORDS     (8 + 0)

#define FUNCTION_NAME   vect_complex_s32_tail_reverse

#define X       a0
#define N       a1
#define X_A     a3
#define X_C     s2
#define mask_A  s3
#define mask_C  s4
#define i       s5
#define zero    s6
#define _16     s7
#define X_lo    t3

.text
.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.p2align 4

FUNCTION_NAME:
    xm.entsp (NSTACKWORDS)*4
    xm.stdsp  s3,s2,8
    xm.stdsp  s5,s4,16
    xm.stdsp  s7,s6,0
    { li t3, 0                          ; srli s7, N, 2                         }
    { srli t3, N, 2                     ; xm.vsetc t3                           }
    { srli t3, N, 3                     ; xm.brff t3, .L_finish                 }
    { nop                               ; xm.bt t3, .L_big_enough               }

    // N = 4, just reverse elements 1 and 3
    xm.lddi  a3,s2, 8(X)
    xm.lddi  s7,t3, 24(X)
    xm.stdi  a3,s2, 24(X)
    xm.stdi  s7,t3, 8(X)
    tail .L_finish

.L_big_enough:

#define X_hi    X

    la t3, vpu_vec_zero
    { srli i, N, 3                      ; mv zero, t3                           }

    { xm.mkmski mask_A, 8               ; xm.vclrdr                             }
    { addi X_lo, X, 8                   ; slli mask_A, mask_A, 8                }
    { li X_A, 32                        ; slli mask_C, mask_A, 16               }
    sh2add X_hi, N, X
    sh2add X_hi, N, X_hi
    { li _16, 16                        ; sub X_hi, X_hi, X_A                   }

.L_rev_loop:
        { add X_A, X_hi, _16            ; xm.vldc X_hi                          }
        { xm.sub X_C, X_hi, _16         ; xm.vldr X_lo                          }
        { addi i, i, -1                 ; xm.vlmaccr0 zero                      }
        { nop                           ; xm.vlmaccr0 zero                      }
        { sub X_hi, X_C, _16            ; xm.vstr X_hi                          }
        xm.vstrpv X_A, mask_A
        xm.vstrpv X_C, mask_C


        { xm.add X_A, X_lo, _16         ; xm.vstc X_lo                          }
        { xm.sub X_C, X_lo, _16         ; xm.vldr X_lo                          }
        { nop                           ; xm.vlmaccr0 zero                      }
        { nop                           ; xm.vlmaccr0 zero                      }
        { add X_lo, X_A, _16            ; xm.vstr X_lo                          }
        xm.vstrpv X_A, mask_A
        xm.vstrpv X_C, mask_C
        { nop                           ; xm.bt i, .L_rev_loop                  }

.L_finish:
    xm.lddsp  s3,s2,8
    xm.lddsp  s5,s4,16
    xm.lddsp  s7,s6,0
    xm.retsp (NSTACKWORDS)*4

.set FUNCTION_NAME.nstackwords,NSTACKWORDS;  .global FUNCTION_NAME.nstackwords; 
.set FUNCTION_NAME.maxcores,1;               .global FUNCTION_NAME.maxcores; 
.set FUNCTION_NAME.maxtimers,0;              .global FUNCTION_NAME.maxtimers; 
.set FUNCTION_NAME.maxchanends,0;            .global FUNCTION_NAME.maxchanends; 
.L_func_end:
    .size FUNCTION_NAME, .L_func_end - FUNCTION_NAME









#endif //defined(__VX4B__)
