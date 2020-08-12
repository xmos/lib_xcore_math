
#if defined(__XS3A__)
#ifndef XS3_MATH_NO_ASM

/*  
void xs3_reverse_complex_s32(
    const complex_s32_t* X,
    const unsigned N);
*/

#include "../asm_helper.h"

.text
.issue_mode dual

#define NSTACKWORDS     (8 + 24)


#define FUNCTION_NAME   xs3_reverse_complex_s32

#define STACK_VEC_TMP   (8)

#define X       r0
#define N       r1
#define i       r2
#define S_re    r3
#define S_im    r4
#define T_re    r5
#define T_im    r6
#define X_lo    r7
#define X_hi    r8


ASM_PREAMBLE(FUNCTION_NAME)
FUNCTION_NAME:
        dualentsp NSTACKWORDS
        std r4, r5, sp[0]
        std r6, r7, sp[1]
        std r8, r9, sp[2]

    {                                           ;   shl i, N, 1                             }
        ldaw X_hi, X[i]
    {   sub X_hi, X_hi, 8                       ;   shr i, N, 2                             }
    {                                           ;   bu .L_rev_loop                          }
    
.align 16
.L_rev_loop:
            ldd S_im, S_re, X_hi[0]
            ldd T_im, T_re, X_lo[0]
            std S_im, S_re, X_lo[0]
        {   add X_lo, X_lo, 8                       ;   sub i, i, 1                             }
            std T_im, T_re, X_hi[0]
        {   sub X_hi, X_hi, 8                       ;   bt i, .L_rev_loop                       }

    {   ldd r4, r5, sp[0]                                                                   }
    {   ldd r6, r7, sp[1]                                                                   }
    {   ldd r8, r9, sp[2]                                                                   }
    {   retsp NSTACKWORDS                                                                   }

ASM_POSTAMBLE(FUNCTION_NAME, NSTACKWORDS)







#endif //!defined(XS3_MATH_NO_ASM)
#endif //defined(__XS3A__)