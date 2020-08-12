
#if defined(__XS3A__)

#include "../asm_helper.h"

/*  


complex_s32_t xs3_sum_complex_s32(
    const complex_s32_t* b,
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t sat);


*/

#define NSTACKVECS      (2)
#define NSTACKWORDS     (6+(8*NSTACKVECS))

#define b           r0
#define length      r1
#define b_shr       r2
#define sat         r3
#define _32         r4
#define tmp         r5
#define tail_bytes  r6

#define STACK_VEC_ZEROS     (NSTACKWORDS- 8)
#define STACK_VEC_TMP       (NSTACKWORDS-16)

#define FUNCTION_NAME xs3_sum_complex_s32
    
ASM_PREAMBLE(FUNCTION_NAME)

FUNCTION_NAME:

        dualentsp NSTACKWORDS
        std r4, r5, sp[1]
        std r6, r7, sp[2]

        ldc tmp, 0x40
    {   shl tmp, tmp, 24                        ;   shl tail_bytes, length, 3               }
    {   shr tmp, tmp, b_shr                     ;   zext tail_bytes, 5                      }

    {   ldaw r11, sp[STACK_VEC_ZEROS]           ;   vclrdr                                  }
    {   ldaw r11, sp[STACK_VEC_TMP]             ;   vstd r11[0]                             }

        std tmp, tmp, r11[0]
        std tmp, tmp, r11[1]
        std tmp, tmp, r11[2]
        std tmp, tmp, r11[3]

    {   ldc r11, 0                              ;   vldc r11[0]                             }
    {   ldc _32, 32                             ;   vsetc r11                               }

    {   shr length, length, 2                   ;   bf length, .L_loop_bot                  }

    .L_loop_top:

        {   sub length, length, 1               ;   vlmacc b[0]                             }
        {   add b, b, _32                       ;   bt length, .L_loop_top                  }

    .L_loop_bot:

    {   ldaw r11, sp[STACK_VEC_ZEROS]           ;                                           }
    {   sub r11, r11, tail_bytes                ;   shl tmp, sat, 16                        }
    {   zext sat, 16                            ;   vldc r11[0]                             }
    {   or sat, sat, tmp                        ;   vlmacc b[0]                             }

        std sat, sat, r11[0]
        std sat, sat, r11[1]
        std sat, sat, r11[2]
        std sat, sat, r11[3]

    {   ldaw r11, sp[STACK_VEC_TMP]             ;   vlsat r11[0]                            }

    {                                           ;   vstr r11[0]                             }
    {                                           ;   ldw tmp, sp[STACK_VEC_TMP+0]            }
    {   add tmp, tmp, r11                       ;   ldw r11, sp[STACK_VEC_TMP+2]            }
    {   add tmp, tmp, r11                       ;   ldw r11, sp[STACK_VEC_TMP+4]            }
    {   add tmp, tmp, r11                       ;   ldw r11, sp[STACK_VEC_TMP+6]            }
    {   add r0, tmp, r11                        ;   ldw tmp, sp[STACK_VEC_TMP+1]            }
    {   add tmp, tmp, r11                       ;   ldw r11, sp[STACK_VEC_TMP+3]            }
    {   add tmp, tmp, r11                       ;   ldw r11, sp[STACK_VEC_TMP+5]            }
    {   add tmp, tmp, r11                       ;   ldw r11, sp[STACK_VEC_TMP+7]            }
    {   add r1, tmp, r11                        ;                                           }

.L_done:
        ldd r4, r5, sp[1]
        ldd r6, r7, sp[2]
        retsp NSTACKWORDS

ASM_POSTAMBLE(FUNCTION_NAME, NSTACKWORDS)

#undef FUNCTION_NAME



#endif //defined(__XS3A__)



