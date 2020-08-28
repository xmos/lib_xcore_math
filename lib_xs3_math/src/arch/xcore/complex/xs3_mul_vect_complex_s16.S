
#if defined(__XS3A__)


#include "../asm_helper.h"

/*  

headroom_t xs3_mul_vect_complex_s16(
    int16_t* a_real,
    int16_t* a_imag,
    const int16_t* b_real,
    const int16_t* b_imag,
    const int16_t* c,
    const unsigned length,
    const right_shift_t sat);

*/

#define NSTACKVECS      (2)
#define NSTACKWORDS     (12+8*(NSTACKVECS))


#define STACK_C         (NSTACKWORDS+1)
#define STACK_LENGTH    (NSTACKWORDS+2)
#define STACK_SAT       (NSTACKWORDS+3)

#define STACK_VEC_TMP   (NSTACKWORDS-8)
#define STACK_VEC_SAT   (NSTACKWORDS-16)


#define FUNCTION_NAME xs3_mul_vect_complex_s16
    
#define a_re        r0
#define a_im        r1
#define b_re        r2
#define b_im        r3
#define c           r4
#define len         r5
#define vec_tmp     r6
#define vec_sat     r7
#define bytemask    r8



ASM_PREAMBLE(FUNCTION_NAME)


FUNCTION_NAME:
    dualentsp NSTACKWORDS
    std r4, r5, sp[1]
    std r6, r7, sp[2]
    std r8, r9, sp[3]

    {   ldaw r11, sp[STACK_VEC_SAT]             ;   ldw r4, sp[STACK_SAT]                   }
    {   shl r5, r4, 16                          ;   zext r4, 16                             }
    {   or r4, r4, r5                           ;   stw r10, sp[1]                          }
        std r4, r4, r11[0]
        std r4, r4, r11[1]
        std r4, r4, r11[2]
        std r4, r4, r11[3]

    {                                           ;   ldw len, sp[STACK_LENGTH]               }
    {   ldc r11, 32                             ;   ldw bytemask, sp[STACK_LENGTH]          }
    {   shr len, bytemask, 4                    ;   ldw c, sp[STACK_C]                      }
    {   zext bytemask, 4                        ;                                           }
    {   shl bytemask, bytemask, SIZEOF_LOG2_S16 ;   shl r11, r11, 3                         }
    {   ldaw vec_tmp, sp[STACK_VEC_TMP]         ;   vsetc r11                               }
    {   ldaw vec_sat, sp[STACK_VEC_SAT]         ;                                           }
    {   ldc r11, 32                             ;   bf len, .L_loop_bot                     }

#define _32     r11
    .L_loop_top:
        {   sub len, len, 1                         ;   vclrdr                                  }
        {   add c, c, _32                           ;   vldc c[0]                               }
        {   add b_re, b_re, _32                     ;   vlmacc b_re[0]                          }
        {                                           ;   vlsat vec_sat[0]                        }
        {   add a_re, a_re, _32                     ;   vstr a_re[0]                            }
        {                                           ;   vclrdr                                  }
        {   add b_im, b_im, _32                     ;   vlmacc b_im[0]                          }
        {                                           ;   vlsat vec_sat[0]                        }
        {   add a_im, a_im, _32                     ;   vstr a_im[0]                            }
        {                                           ;   bt len, .L_loop_top                     }

    .L_loop_bot:
#undef _32

    {   mkmsk bytemask, bytemask                ;   bf bytemask, .L_done                    }

    entsp 0

.issue_mode single   // Shrinks code size; removes some FNOPs

    vclrdr
    vstd vec_tmp[0]
    vldc c[0]
    vlmacc b_re[0]
    vlsat vec_sat[0]
    vstrpv a_re[0], bytemask
    vstrpv vec_tmp[0], bytemask
    vldd vec_tmp[0]
    vstd vec_tmp[0]
    vclrdr
    vlmacc b_im[0]
    vlsat vec_sat[0]
    vstrpv a_im[0], bytemask
    vstrpv vec_tmp[0], bytemask
    vldd vec_tmp[0]
    vstd vec_tmp[0]
    nop
    dualentsp 0

.issue_mode dual

.L_done:
        ldd r4, r5, sp[1]
        ldd r6, r7, sp[2]
        ldd r8, r9, sp[3]

    {   ldc r0, 15                              ;   vgetc r11                               }
    {   zext r11, 5                             ;   ldw r10, sp[1]                          }
    {   sub r0, r0, r11                         ;   retsp NSTACKWORDS                       }

ASM_POSTAMBLE(FUNCTION_NAME, NSTACKWORDS)

#undef FUNCTION_NAME



#endif //defined(__XS3A__)



