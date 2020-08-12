
#if defined(__XS3A__)
#ifndef XS3_MATH_NO_ASM

/*  
void xs3_complex_s16_to_complex_s32(
    complex_s32_t* a,
    const int16_t* b_real,
    const int16_t* b_imag,
    const unsigned length);
*/

#include "../asm_helper.h"

.text

#define NSTACKVECS      2
#define NSTACKWORDS     (8 + (8*NSTACKVECS))

#define FUNCTION_NAME   xs3_complex_s16_to_complex_s32

#define VEC_TMP_REAL    (NSTACKWORDS - 8)
#define VEC_TMP_IMAG    (NSTACKWORDS - 16)

#define a               r0
#define b_real          r1
#define b_imag          r2
#define length          r3
#define tmp_real        r4
#define tmp_imag        r5

ASM_PREAMBLE(FUNCTION_NAME)

.issue_mode single

FUNCTION_NAME:
        entsp NSTACKWORDS
        std r4, r5, sp[0]

    .L_loop_top:  
        sub length, length, 1
        ld16s tmp_real, b_real[length]
        ld16s tmp_imag, b_imag[length]
        std tmp_real, tmp_imag, a[length]
        bt length, .L_loop_top

    ldd r4, r5, sp[0]
    retsp NSTACKWORDS       

ASM_POSTAMBLE(FUNCTION_NAME, NSTACKWORDS)

#endif //!defined(XS3_MATH_NO_ASM)
#endif //defined(__XS3A__)