
#if defined(__XS3A__)


#include "../asm_helper.h"

/*  

headroom_t xs3_complex_mul_vect_complex_s16(
    int16_t* a_real,
    int16_t* a_imag,
    const int16_t* b_real,
    const int16_t* b_imag,
    const int16_t* c_real,
    const int16_t* c_imag,
    const unsigned length,
    const right_shift_t sat);

*/

#define NSTACKVECS      (2)
#define NSTACKWORDS     (12+8*(NSTACKVECS))


#define STACK_C_REAL    (NSTACKWORDS+1)
#define STACK_C_IMAG    (NSTACKWORDS+2)
#define STACK_LENGTH    (NSTACKWORDS+3)
#define STACK_SAT       (NSTACKWORDS+4)

#define STACK_VEC_TMP   (NSTACKWORDS-8)
#define STACK_VEC_SAT   (NSTACKWORDS-16)

#define STACK_BYTEMASK  8

#define FUNCTION_NAME xs3_complex_mul_vect_complex_s16
    
#define a_re        r0
#define a_im        r1
#define b_re        r2
#define b_im        r3
#define c_re        r4
#define c_im        r5
#define len         r6
#define vec_tmp     r7
#define vec_sat     r8
#define bytemask    r9



ASM_PREAMBLE(FUNCTION_NAME)


/*
    We're doing this:

    vR  <- -1
    vR  <- -1 * b.imag
    vC  <- -b.imag  
    acc <- 0
    acc <- vC * c.imag
    vC  <- b.real
    acc <- acc + vC * c.real
    vR  <- acc >> sat
    a.real <- vR

    (vC still has b.real)
    acc <- 0
    acc <- vC * c.imag
    vC  <- b.imag
    acc <- acc + vC * c.real
    vR  <- acc >> sat
    a.imag <- vR
*/

.L_neg_ones:
    .short -0x4000,-0x4000,-0x4000,-0x4000,-0x4000,-0x4000,-0x4000,-0x4000,-0x4000,-0x4000,-0x4000,-0x4000,-0x4000,-0x4000,-0x4000,-0x4000

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

    {   ldc r11, 32                             ;   ldw bytemask, sp[STACK_LENGTH]          }
    {   shr len, bytemask, 4                    ;   ldw c_re, sp[STACK_C_REAL]              }
    {   zext bytemask, 4                        ;   ldw c_im, sp[STACK_C_IMAG]              }
    {   shl bytemask, bytemask, SIZEOF_LOG2_S16 ;   shl r11, r11, 3                         }
    {   ldaw vec_tmp, sp[STACK_VEC_TMP]         ;   vsetc r11                               }
    {   ldap r11, .L_neg_ones                   ;   bf len, .L_loop_bot                     }

    .L_loop_top:
        {   sub len, len, 1                         ;   vldr r11[0]                             }
        {   mkmsk r11, 32                           ;   vlmul b_im[0]                           }
            vstrpv vec_tmp[0], r11
        {   ldaw vec_sat, sp[STACK_VEC_SAT]         ;   vldc vec_tmp[0]                         }
        {                                           ;   vclrdr                                  }
        {                                           ;   vlmacc c_im[0]                          }
        {                                           ;   vldc b_re[0]                            }
        {                                           ;   vlmacc c_re[0]                          }
        {                                           ;   vlsat vec_sat[0]                        }
        {   ldc r11, 32                             ;   vstr a_re[0]                            }
        {   add b_re, b_re, r11                     ;   vclrdr                                  }
        {   add a_re, a_re, r11                     ;   vlmacc c_im[0]                          }
        {   add c_im, c_im, r11                     ;   vldc b_im[0]                            }
        {   add b_im, b_im, r11                     ;   vlmacc c_re[0]                          }
        {   add c_re, c_re, r11                     ;   vlsat vec_sat[0]                        }
        {   add a_im, a_im, r11                     ;   vstr a_im[0]                            }
        {   ldap r11, .L_neg_ones                   ;   bt len, .L_loop_top                     }
    .L_loop_bot:

    {   mkmsk bytemask, bytemask                ;   bf bytemask, .L_done                    }   

    {                                           ;   vldr r11[0]                             }
    {   mkmsk r11, 32                           ;   vlmul b_im[0]                           }
        vstrpv vec_tmp[0], r11
    {   ldaw vec_sat, sp[STACK_VEC_SAT]         ;   vldc vec_tmp[0]                         }
    {                                           ;   vclrdr                                  }
    {                                           ;   vstd vec_tmp[0]                         }
    {                                           ;   vlmacc c_im[0]                          }
    {                                           ;   vldc b_re[0]                            }
    {   ldaw r11, sp[STACK_VEC_SAT]             ;   vlmacc c_re[0]                          }
    {                                           ;   vlsat vec_sat[0]                        }
        vstrpv a_re[0], bytemask
        vstrpv vec_tmp[0], bytemask
    {                                           ;   vldd vec_tmp[0]                         }
    {                                           ;   vstd vec_tmp[0]                         }
    {                                           ;   vclrdr                                  }
    {                                           ;   vlmacc c_im[0]                          }
    {                                           ;   vldc b_im[0]                            }
    {                                           ;   vlmacc c_re[0]                          }
    {                                           ;   vlsat vec_sat[0]                        }
        vstrpv a_im[0], bytemask   
        vstrpv vec_tmp[0], bytemask
    {                                           ;   vldd vec_tmp[0]                         }
    {                                           ;   vstd vec_tmp[0]                         }


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



