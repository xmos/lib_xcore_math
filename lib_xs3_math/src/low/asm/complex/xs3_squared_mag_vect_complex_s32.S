
#if defined(__XS3A__)

#include "../asm_helper.h"

/*  

headroom_t xs3_squared_mag_vect_complex_s32(
    int32_t* a,
    const complex_s32_t* b,
    const unsigned length,
    const right_shift_t b_shr);

*/


#define NSTACKWORDS     (10+8)

#define a               r0 
#define b               r1 
#define length          r2
#define b_shr           r3
#define vec_count       r4
#define _32             r5
#define _28             r6
#define vec_tmp         r7
#define tmpA            r8
#define tmpB            r9
#define tmpC            r10


#define STACK_VEC_TMP   (NSTACKWORDS-8)

#define STACK_A         8
#define STACK_LENGTH    9

#define FUNCTION_NAME xs3_squared_mag_vect_complex_s32
    
ASM_PREAMBLE(FUNCTION_NAME)

FUNCTION_NAME:
    dualentsp NSTACKWORDS
    std r4, r5, sp[1]
    std r6, r7, sp[2]
    std r8, r9, sp[3]
    {                                           ;   stw r10, sp[1]                          }

#define tail_mask       length

    {                                           ;   stw a, sp[STACK_A]                      }
    {                                           ;   stw length, sp[STACK_LENGTH]            }

    {   ldc _32, 32                             ;   shr vec_count, length, 2                }
    {   ldc r11, 0                              ;   shl tail_mask, length, 3                }
    {   zext tail_mask, 5                       ;   vsetc r11                               }


    {   ldaw vec_tmp, sp[STACK_VEC_TMP]         ;   ldc _28, 28                             }
    {   mkmsk r11, 32                           ;   bf vec_count, .L_loop_bot               }

    .L_loop_top:
            vlashr b[0], b_shr
            vstrpv vec_tmp[0], r11
        {   add b, b, _32                           ;   vldc vec_tmp[0]                         }
        {   sub vec_count, vec_count, 1             ;   vldd vec_tmp[0]                         }
        {   mkmsk tmpA, 4                           ;   vcmcr                                   } 
            vstrpv a[0], tmpA
        {   sub a, a, 4                             ;   shl tmpA, tmpA, 8                       }
            vstrpv a[0], tmpA
        {   sub a, a, 4                             ;   shl tmpA, tmpA, 8                       }
            vstrpv a[0], tmpA
        {   sub a, a, 4                             ;   shl tmpA, tmpA, 8                       }
            vstrpv a[0], tmpA
        {   add a, a, _28                           ;   bt vec_count, .L_loop_top               }

        {                                           ;   ldw length, sp[STACK_LENGTH]            }
        {   shr vec_count, length, 3                ;   ldw r11, sp[STACK_A]                    }

        .L_hr_loop_top:

            {   sub vec_count, vec_count, 1             ;   vldr r11[0]                             }
            {   add r11, r11, _32                       ;   vstr r11[0]                             }
            {                                           ;   bt vec_count, .L_hr_loop_top            }        

    .L_loop_bot:

    {                                           ;   bf tail_mask, .L_done                   }
    {                                           ;   vclrdr                                  }
    {                                           ;   vstd vec_tmp[0]                         }
        vlashr b[0], b_shr
        vstrpv vec_tmp[0], tail_mask
    {                                           ;   vldd vec_tmp[0]                         }
    {                                           ;   vldc vec_tmp[0]                         }
    {                                           ;   vcmcr                                   }
        vstrpv a[0], tail_mask
        vstrpv vec_tmp[0], tail_mask
    {                                           ;   vldd vec_tmp[0]                         }
    {                                           ;   vstd vec_tmp[0]                         }

.L_done:

        ldd r4, r5, sp[1]
        ldd r6, r7, sp[2]
        ldd r8, r9, sp[3]

    {   ldc r0, 31                              ;   vgetc r11                               }
    {   zext r11, 5                             ;   ldw r10, sp[1]                          }
    {   sub r0, r0, r11                         ;   retsp NSTACKWORDS                       }


ASM_POSTAMBLE(FUNCTION_NAME, NSTACKWORDS)

#undef FUNCTION_NAME



#endif //defined(__XS3A__)



