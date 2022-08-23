// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


/*  
    void vect_complex_s32_tail_reverse(
        complex_s32_t* X,
        const unsigned N);
*/

#include "../asm_helper.h"

#define NSTACKWORDS     (8 + 0)

#define FUNCTION_NAME   vect_complex_s32_tail_reverse

#define X       r0
#define N       r1
#define X_A     r3
#define X_C     r4
#define mask_A  r5
#define mask_C  r6
#define i       r7
#define zero    r8
#define _16     r9
#define X_lo    r11

.text
.issue_mode dual
.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.align 16
.cc_top FUNCTION_NAME.function,FUNCTION_NAME

FUNCTION_NAME:
        dualentsp NSTACKWORDS
        std r4, r5, sp[0]
        std r6, r7, sp[1]
        std r8, r9, sp[2]
    {   ldc r11, 0                              ;   shr r9, N, 2                            }
    {   shr r11, N, 2                           ;   vsetc r11                               }
    {   shr r11, N, 3                           ;   bf r11, .L_finish                       }
    {                                           ;   bt r11, .L_big_enough                   }

        // N = 4, just reverse elements 1 and 3
        ldd r4, r3, X[1]
        ldd r11, r9, X[3]
        std r4, r3, X[3]
        std r11, r9, X[1]
        bu .L_finish

.L_big_enough:

#define X_hi    X

        ldaw r11, cp[vpu_vec_zero]
    {   shr i, N, 3                             ;   mov zero, r11                           }

    {   mkmsk mask_A, 8                         ;   vclrdr                                  }
    {   add X_lo, X, 8                          ;   shl mask_A, mask_A, 8                   }
    {   ldc X_A, 32                             ;   shl mask_C, mask_A, 16                  }
        ldaw X_hi, X[N]
        ldaw X_hi, X_hi[N]
    {   ldc _16, 16                             ;   sub X_hi, X_hi, X_A                     }

.L_rev_loop:
        {   add X_A, X_hi, _16                      ;   vldc X_hi[0]                            }
        {   sub X_C, X_hi, _16                      ;   vldr X_lo[0]                            }
        {   sub i, i, 1                             ;   vlmaccr zero[0]                         }
        {                                           ;   vlmaccr zero[0]                         }
        {   sub X_hi, X_C, _16                      ;   vstr X_hi[0]                            }
            vstrpv X_A[0], mask_A
            vstrpv X_C[0], mask_C
        {   add X_A, X_lo, _16                      ;   vstc X_lo[0]                            }
        {   sub X_C, X_lo, _16                      ;   vldr X_lo[0]                            }
        {                                           ;   vlmaccr zero[0]                         }
        {                                           ;   vlmaccr zero[0]                         }
        {   add X_lo, X_A, _16                      ;   vstr X_lo[0]                            }
            vstrpv X_A[0], mask_A
            vstrpv X_C[0], mask_C
        {                                           ;   bt i, .L_rev_loop                       }

.L_finish:
        ldd r4, r5, sp[0]
        ldd r6, r7, sp[1]
        ldd r8, r9, sp[2]
        retsp NSTACKWORDS

.cc_bottom FUNCTION_NAME.function; 
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;  .global FUNCTION_NAME.nstackwords; 
.set FUNCTION_NAME.maxcores,1;               .global FUNCTION_NAME.maxcores; 
.set FUNCTION_NAME.maxtimers,0;              .global FUNCTION_NAME.maxtimers; 
.set FUNCTION_NAME.maxchanends,0;            .global FUNCTION_NAME.maxchanends; 
.L_func_end:
    .size FUNCTION_NAME, .L_func_end - FUNCTION_NAME









#endif //defined(__XS3A__)