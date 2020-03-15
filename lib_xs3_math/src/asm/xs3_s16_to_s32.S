
#if defined(__XS3A__)
#ifndef XS3_MATH_NO_ASM

/*  
headroom_t xs3_s16_to_s32(
    int16_t* a,
    const int32_t* b,
    const unsigned length);
*/

#include "asm_helper.h"

.text
.issue_mode dual

#if XS3_MATH_VECTOR_TAIL_SUPPORT
  #define NSTACKWORDS     (2)
#else
  #define NSTACKWORDS     (0)
#endif

#define FUNCTION_NAME   xs3_s16_to_s32

#define a               r0
#define b               r1
#define len             r2
#define _16             r3
#define tail            r4
#define constsA         r11
#define constsB         r11

ASM_PREAMBLE(FUNCTION_NAME)

FUNCTION_NAME:
/**/    dualentsp NSTACKWORDS
        ldc r11, 0x0200
    {   ldap r11, .L_vlmacc_consts_A            ;   vsetc r11                               }

#if XS3_MATH_VECTOR_TAIL_SUPPORT
        std r4, r5, sp[0]
/**/{   shl tail, len, SIZEOF_LOG2_S32          ;                                           }
    {   shr len, len, EPV_LOG2_S32              ;   zext tail, 5                            }
    {   mkmsk tail, tail                        ;   bf len, .L_loop_bot                     }
    {                                           ;   bu .L_loop_top                          }
#else // XS3_MATH_VECTOR_TAIL_SUPPORT
    {   shr len, len, EPV_LOG2_S32              ;   bu .L_loop_top                          }
#endif // XS3_MATH_VECTOR_TAIL_SUPPORT

.align 16

.L_vlmacc_consts_A:
.byte 0x7F, 0x00, 0x7F, 0x00, 0x7F, 0x00, 0x7F, 0x00, 0x7F, 0x00, 0x7F, 0x00, 0x7F, 0x00, 0x7F, 0x00
.L_vlmacc_consts_B:
.byte 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01

.L_loop_top: //All in 8-bit mode
/**/    {   sub len, len, 1                         ;   vclrdr                                  } 
        {   ldc _16, 16                             ;   vldc b[0]                               }
        {   add b, b, _16                           ;   vlmacc constsA[0]                       }
        {   add r11, r11, _16                       ;   vlmacc constsA[0]                       }
/**/    {   sub r11, r11, _16                       ;   vlmacc constsB[0]                       }
        {   add a, a, _16                           ;   vstr a[0]                               }
        {   add a, a, _16                           ;   bt len, .L_loop_top                     }
.L_loop_bot:

#if XS3_MATH_VECTOR_TAIL_SUPPORT
    {   ldc _16, 16                             ;   bf tail, .L_finish                      }
/**/{                                           ;   vclrdr                                  }
    {                                           ;   vldc b[0]                               }
    {                                           ;   vlmacc r11[0]                           }
    {   add r11, r11, _16                       ;   vlmacc r11[0]                           }
/**/{                                           ;   vlmacc r11[0]                           }
    vstrpv a[0], tail
#endif // XS3_MATH_VECTOR_TAIL_SUPPORT

.L_finish:
        ldd r4, r5, sp[0]
        retsp NSTACKWORDS       

ASM_POSTAMBLE(FUNCTION_NAME, NSTACKWORDS)

#endif //!defined(XS3_MATH_NO_ASM)
#endif //defined(__XS3A__)