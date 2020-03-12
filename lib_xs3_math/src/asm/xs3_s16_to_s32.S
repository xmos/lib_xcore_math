
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
  #define NSTACKWORDS     (2+8)
#else
  #define NSTACKWORDS     (2)
#endif

#define FUNCTION_NAME   xs3_s16_to_s32

#if XS3_MATH_VECTOR_TAIL_SUPPORT
  #define STACK_VEC_TMP   (NSTACKWORDS-8)
#endif

#define a               r0
#define b               r1
#define len             r2
#define tail            r3
#define _16             r4
#define write_mask      r5

ASM_PREAMBLE(FUNCTION_NAME)

.L_vlmacc_const1:
.short 0x7F01, 0x7F01, 0x7F01, 0x7F01, 0x7F01, 0x7F01, 0x7F01, 0x7F01
.L_vlmacc_const2:
.short 0x02FF, 0x02FF, 0x02FF, 0x02FF, 0x02FF, 0x02FF, 0x02FF, 0x02FF 
.word 0x44444444
FUNCTION_NAME:
/**/    dualentsp NSTACKWORDS
        std r4, r5, sp[0]
        ldc r11, 0x0200
    {   shl tail, len, SIZEOF_LOG2_S32          ;   ldc _16, 16                             }
/**/{   shr len, len, EPV_LOG2_S32              ;   vsetc r11                               }
    {   ldap r11, .L_vlmacc_const2              ;   zext tail, 5                            }
    {   mkmsk tail, tail                        ;   ldw write_mask, r11[4]                  }
    {   sub a, a, _16                           ;   bf len, .L_loop_bot                     }

.align 16
.L_loop_top:
/**/    {   sub len, len, 1                         ;   vclrdr                                  } 
        {   add a, a, _16                           ;   vldc b[0]                               }
        {   sub r11, r11, _16                       ;   vstd a[0]                               }
        {   add b, b, _16                           ;   vlmacc r11[0]                           }
/**/    {   add r11, r11, _16                       ;   vlmacc r11[0]                           }
        {   shl write_mask, write_mask, 1           ;   vlmacc r11[0]                           }
            vstrpv a[0], write_mask
        {   shr write_mask, write_mask, 1           ;   vlmaccr r11[0] /*can be any address!*/  }
        /****** FNOP ******/
/**/        vstrpv a[0], write_mask
        {   add a, a, _16                           ;   bt len, .L_loop_top                     }
.L_loop_bot:

#if XS3_MATH_VECTOR_TAIL_SUPPORT
    {   add a, a, _16                           ;   bf tail, .L_finish                      }
    {   sub r11, r11, _16                       ;   vclrdr                                  }
/**/{   ldaw r1, sp[STACK_VEC_TMP]              ;   vldc b[0]                               }
    {                                           ;   vstd r1[0]                              }
    {   add r1, r11, _16                        ;   vlmacc r11[0]                           }
    {   ldaw r11, sp[STACK_VEC_TMP]             ;   vlmacc r11[0]                           }
/**/{   shl write_mask, write_mask, 1           ;   vlmacc r1[0]                            }
        vstrpv r11[0], write_mask
    {   shr write_mask, write_mask, 1           ;   vlmaccr r11[0]                          }
        vstrpv r11[0], write_mask
/**/{                                           ;   vldr r11[0]                             }
        vstrpv a[0], tail
#endif // XS3_MATH_VECTOR_TAIL_SUPPORT

.L_finish:
        ldd r4, r5, sp[0]
    {                                           ;   retsp NSTACKWORDS                       } 

ASM_POSTAMBLE(FUNCTION_NAME, NSTACKWORDS)

#endif //!defined(XS3_MATH_NO_ASM)
#endif //defined(__XS3A__)