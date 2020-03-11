
#if defined(__XS3A__)
#ifndef XS3_MATH_NO_ASM


#include "asm_helper.h"


#define NSTACKWORDS     8

#define STACK_TMP_VEC   0

#define arg_v       r0
#define arg_len     r1

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

/*  
headroom_t xs3_cls_array_s16(
    const int16_t* v, 
    const unsigned length);
*/

#define FUNCTION_NAME xs3_cls_array_s16

ASM_PREAMBLE(FUNCTION_NAME)

.align 16
FUNCTION_NAME:
/**/{   ldc r11, 32                             ;   dualentsp NSTACKWORDS               }
    {   shl r11, r11, 3                         ;   vclrdr                              }
    {   shl r2, arg_len, 1                      ;   vsetc r11                           }
    {   shr arg_len, arg_len, 4                 ;   mov r11, arg_v                      }

/**/{   ldaw r0, sp[STACK_TMP_VEC]              ;                                       }
    {   ldc r3, 32                              ;                                       }
    {   zext r2, 5                              ;   bf arg_len, .L_loop_bot_s16         }
    {                                           ;   bu .L_loop_top_s16                  }
     
    .align 16
    .L_loop_top_s16:
/**/    {   add r11, r11, r3                        ;   vldr r11[0]                         }
        {   sub arg_len, arg_len, 1                 ;   vstr r0[0]                          }
        {                                           ;   bt arg_len, .L_loop_top_s16         }

.L_loop_bot_s16:
    {                                           ;   bf r2, .L_finish16                  }
/**/{   mkmsk r2, r2                            ;   vstd r0[0]                          }
    {   mov r11, r0                             ;   vldr r11[0]                         }
    vstrpv r0[0], r2
    {                                           ;   vldr r11[0]                         }
/**/{                                           ;   vstr r11[0]                         }

.L_finish16:
    {   ldc r0, 15                              ;   vgetc r11                           }
    {   zext r11, 5                             ;                                       }
    {   sub r0, r0, r11                         ;   retsp NSTACKWORDS                   }

ASM_POSTAMBLE(FUNCTION_NAME, NSTACKWORDS)

#undef FUNCTION_NAME

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

/*  
headroom_t xs3_cls_array_s32(
    const int32_t* v,
    const unsigned length);
*/

#define FUNCTION_NAME xs3_cls_array_s32

ASM_PREAMBLE(FUNCTION_NAME)

.align 16
FUNCTION_NAME:
/**/dualentsp NSTACKWORDS
    {   ldc r11, 0                              ;   shl r2, arg_len, 2                  }
    {   shr arg_len, arg_len, 3                 ;   vsetc r11                           }
    {   ldaw r0, sp[STACK_TMP_VEC]              ;   mov r11, arg_v                      }
/**/{   ldc r3, 32                              ;   vclrdr                              }
    {   zext r2, 5                              ;   bf arg_len, .L_loop_bot_s32         }
    {                                           ;   bu .L_loop_top_s32                  }

    .align 16
    .L_loop_top_s32:
/**/    {   add r11, r11, r3                        ;   vldr r11[0]                         }
        {   sub arg_len, arg_len, 1                 ;   vstr r0[0]                          }
        {                                           ;   bt arg_len, .L_loop_top_s32         }

    .L_loop_bot_s32:
    {                                           ;   bf r2, .L_finish32                  }
/**/{   mkmsk r2, r2                            ;   vstd r0[0]                          }
    {   mov r11, r0                             ;   vldr r11[0]                         }
    vstrpv r0[0], r2
    {                                           ;   vldr r11[0]                         }
/**/{                                           ;   vstr r11[0]                         }

    .L_finish32:
    {   ldc r0, 31                              ;   vgetc r11                           }
    {   zext r11, 5                             ;                                       }
    {   sub r0, r0, r11                         ;   retsp NSTACKWORDS                   }

ASM_POSTAMBLE(FUNCTION_NAME, NSTACKWORDS)

#undef FUNCTION_NAME


//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////


#endif //!defined(XS3_MATH_NO_ASM)
#endif //defined(__XS3A__)
