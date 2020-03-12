
#if defined(__XS3A__)
#ifndef XS3_MATH_NO_ASM

/*  
headroom_t xs3_abs_vect_s16(
    int16_t* a,
    const int16_t* b,
    const unsigned length);

headroom_t xs3_abs_vect_s32(
    int32_t* a,
    const int32_t* b,
    const unsigned length);
*/


#include "asm_helper.h"

.text
.issue_mode dual

#if XS3_MATH_VECTOR_TAIL_SUPPORT
  #define NSTACKWORDS     (8)
#else
  #define NSTACKWORDS     (0)
#endif


#define FUNCTION_NAME   xs3_abs_vect
#define FNAME_S16       CAT(FUNCTION_NAME, _s16)
#define FNAME_S32       CAT(FUNCTION_NAME, _s32)

#if XS3_MATH_VECTOR_TAIL_SUPPORT
  #define STACK_TMP_VEC       (NSTACKWORDS-8)
#endif

#define a           r0
#define b           r1
#define len         r2
#define tail        r3

/////////////////////////////////////////////////////////////////

ASM_PREAMBLE(FNAME_S16)
FNAME_S16:
/**/    dualentsp NSTACKWORDS
        ldc r11, 0x0100

#if XS3_MATH_VECTOR_TAIL_SUPPORT
    {   shl tail, len, SIZEOF_LOG2_S16          ;   shr len, len, EPV_LOG2_S16              }
    {   zext tail, 5                            ;   vsetc r11                               }
/**/{                                           ;   bu .L_apply_op                          }
#else //XS3_MATH_VECTOR_TAIL_SUPPORT
    {   shr len, len, EPV_LOG2_S16              ;   vsetc r11                               }
    {                                           ;   bu .L_apply_op                          }
#endif //XS3_MATH_VECTOR_TAIL_SUPPORT
ASM_POSTAMBLE(FNAME_S16, NSTACKWORDS)
/////////////////////////////////////////////////////////////////
ASM_PREAMBLE(FNAME_S32)
FNAME_S32:
#if XS3_MATH_VECTOR_TAIL_SUPPORT
/**/    dualentsp NSTACKWORDS
    {   ldc r11, 0                              ;   shl tail, len, SIZEOF_LOG2_S32          }
    {   shr len, len, EPV_LOG2_S32              ;   vsetc r11                               }
    {   zext tail, 5                            ;   bu .L_apply_op                          }
#else //XS3_MATH_VECTOR_TAIL_SUPPORT
/**/{   ldc r11, 0                              ;   dualentsp NSTACKWORDS                   }
    {   shr len, len, EPV_LOG2_S32              ;   vsetc r11                               }
    {                                           ;   bu .L_apply_op                          }
#endif //XS3_MATH_VECTOR_TAIL_SUPPORT
ASM_POSTAMBLE(FNAME_S32, NSTACKWORDS)
/////////////////////////////////////////////////////////////////
#undef a
#undef b
#undef len

/*
    When branching here:
        *   a --> r0
        *   b --> r1
        *   loop_count --> r2
        *   tail --> r3
        *   VPU mode must already be set.
*/

#define a           r0
#define b           r1
#define loop_count  r2
#define tail        r3

ASM_PREAMBLE_LCL(apply_op);
.L_apply_op:

#if XS3_MATH_VECTOR_TAIL_SUPPORT
/**/{   mkmsk tail, tail                        ;                                           }
#endif //XS3_MATH_VECTOR_TAIL_SUPPORT
    {   mov r11, b                              ;   bf loop_count, .L_loop_bot              }
    {   ldc r1, 32                              ;   bu .L_loop_top                          }
.align 16
.L_loop_top:
/**/    {   sub loop_count, loop_count, 1           ;   vldr r11[0]                             }
        {                                           ;   vsign                                   }
        {                                           ;   vlmul r11[0]                            }
        {   add a, a, r1                            ;   vstr a[0]                               }
/**/    {   add r11, r11, r1                        ;   bt loop_count, .L_loop_top              }
.L_loop_bot:

#if XS3_MATH_VECTOR_TAIL_SUPPORT

    {                                           ;   bf tail, .L_finish                      }
    {                                           ;   vclrdr                                  }
    {                                           ;   vldr r11[0]                             }
/**/{                                           ;   vsign                                   }
    {   ldaw r11, sp[STACK_TMP_VEC]             ;   vlmul r11[0]                            }
    {                                           ;   vstd r11[0]                             }
    {                                           ;   vpos                                    }
/**/    vstrpv r11[0], tail
    {                                           ;   vldr r11[0]                             }
    {                                           ;   vstr r11[0]                             }
        vstrpv a[0], tail

#endif //XS3_MATH_VECTOR_TAIL_SUPPORT

.L_finish:
    {   ldc r0, 32                              ;   vgetc r11                               }
    {   shr r1, r11, 8                          ;                                           }
    {   zext r11, 5                             ;   shr r0, r0, r1                          }
    {   add r11, r11, 1                         ;                                           }
    {   sub r0, r0, r11                         ;   retsp NSTACKWORDS                       } 

ASM_POSTAMBLE_LCL(apply_op);

//////////////////////////////////////////////////////////////////////////////////////////////////


#endif //!defined(XS3_MATH_NO_ASM)
#endif //defined(__XS3A__)