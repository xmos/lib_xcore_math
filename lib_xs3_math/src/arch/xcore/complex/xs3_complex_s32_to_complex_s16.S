


#if defined(__XS3A__)
#ifndef XS3_MATH_NO_ASM

/*  

headroom_t xs3_complex_s32_to_complex_s16(
    int16_t* a_real,
    int16_t* a_imag,
    const complex_s32_t* b,
    const unsigned length,
    const right_shift_t b_shr);

*/

#include "../asm_helper.h"

.text
.issue_mode dual

#define NSTACKVECS      (2)
#define NSTACKWORDS     (6 + (8*NSTACKVECS))

#define FUNCTION_NAME   xs3_complex_s32_to_complex_s16

#define STACK_VEC_TMP  (NSTACKWORDS-16)

#define STACK_A_REAL    (

#define a_real      r0
#define a_imag      r1
#define b           r2
#define len         r3
#define b_shr       r4
#define tail        r5
#define _28         r6
#define mask        r7


ASM_PREAMBLE(FUNCTION_NAME)



FUNCTION_NAME:
        dualentsp NSTACKWORDS
        std r4, r5, sp[0]
        std r6, r7, sp[1]
    {   ldc r11, 0                              ;   ldc _28, 28                             }

    {   ldc r11, 16                             ;   vsetc r11                               }
    {   shr len, len, 2                         ;   shl tail, len, 1                        }
    {   sub b_shr, b_shr, r11                   ;                                           }
        ldaw r11, cp[vpu_vec_complex_ones]
    {   zext tail, 3                            ;   mkmsk mask, 8                           }
    {   mkmsk tail, tail                        ;   bf len, .L_loop_bot                     }

    .L_loop_top:
            vlashr b[0], b_shr
        {   add b, b, 4                             ;   vlmul r11[0]                            }   
        {   sub len, len, 1                         ;   vdepth16                                }
        {                                           ;   vdepth16                                }
            vstrpv a_real[0], mask
            vlashr b[0], b_shr         
        {   add b, b, _28                           ;   vlmul r11[0]                            }
        {   add a_real, a_real, 8                   ;   vdepth16                                }
        {                                           ;   vdepth16                                }
            vstrpv a_imag[0], mask
        {   add a_imag, a_imag, 8                   ;   bt len, .L_loop_top                     }


.L_loop_bot:

    entsp 0

.issue_mode single

    bf tail, .L_finish
    vlashr b[0], b_shr
    vlmul r11[0]
    vdepth16
    vdepth16
    vstrpv a_real[0], tail
    vlashr b[0], b_shr         
    vlmul r11[0]
    vdepth16
    vdepth16
    vstrpv a_imag[0], tail


.L_finish:
        ldd r4, r5, sp[0]
        ldd r6, r7, sp[1]
        retsp NSTACKWORDS

ASM_POSTAMBLE(FUNCTION_NAME, NSTACKWORDS)

#endif //!defined(XS3_MATH_NO_ASM)
#endif //defined(__XS3A__)