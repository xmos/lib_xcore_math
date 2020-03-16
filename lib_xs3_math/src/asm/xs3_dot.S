
#if defined(__XS3A__)
#ifndef XS3_MATH_NO_ASM

/*  
int16_t xs3_dot_s16(
    const int16_t* b,
    const int16_t* c,
    const unsigned length,
    const int b_shr,
    const int c_shr,
    const int sat);


//TODO: There must be a more efficient way to get the final result for xs3_sum_s32(). The way it is now
//      uses way too much code memory.

int32_t xs3_dot_s32(
    const int32_t* b,
    const int32_t* c,
    const unsigned length,
    const int b_shr,
    const int c_shr,
    const int sat);
*/


#include "asm_helper.h"

.text
.issue_mode dual


#define NSTACKWORDS     (8 + 24)


#define FUNCTION_NAME   xs3_dot
#define FNAME_S16       CAT(FUNCTION_NAME, _s16)
#define FNAME_S32       CAT(FUNCTION_NAME, _s32)

#define STACK_C_SHR     (NSTACKWORDS+1)
#define STACK_SAT       (NSTACKWORDS+2)

#define STACK_VEC_TMP   (NSTACKWORDS-8)
#define STACK_VEC_VR    (NSTACKWORDS-16)
#define STACK_VEC_VD    (NSTACKWORDS-24)

#define b           r0
#define c           r1
#define N           r2
#define b_shr       r3
#define c_shr       r4
#define tail        r5
#define vec_vd      r6
#define vec_vr      r7
#define vec_tmp     r8
#define _32         r9



ASM_PREAMBLE(FNAME_S16)


FNAME_S16:
/**/    dualentsp NSTACKWORDS
        ldc r11, 0x0100
        std r4, r5, sp[0]
        std r6, r7, sp[1]
        std r8, r9, sp[2]

    {   shl tail, N, SIZEOF_LOG2_S16            ;   vsetc r11                               }
    {   zext tail, 5                            ;   vclrdr                                  }
    {   shr N, N, EPV_LOG2_S16                  ;   ldaw vec_tmp, sp[STACK_VEC_TMP]         }
    {                                           ;   ldw c_shr, sp[STACK_C_SHR]              }

    {   ldaw vec_vr, sp[STACK_VEC_VR]           ;   ldaw vec_vd, sp[STACK_VEC_VD]           }
    {   ldc _32, 32                             ;   bf N, .L_loop_bot_s16                   }

.L_loop_top_s16:
        {                                           ;   vstd vec_vd[0]                          }
        {                                           ;   vstr vec_vr[0]                          }
            vlashr b[0], b_shr
        {   add b, b, _32                           ;   vstr vec_tmp[0]                         }
        {                                           ;   vldc vec_tmp[0]                         }
            vlashr c[0], c_shr
        {   mov r11, vec_vr                         ;   vstr vec_tmp[0]                         }
        {                                           ;   vldr r11[0]                             }
        {   sub N, N, 1                             ;   vldd vec_vd[0]                          }  
        {                                           ;   vlmacc vec_tmp[0]                       }
        {   add c, c, _32                           ;   bt N, .L_loop_top_s16                   }
.L_loop_bot_s16:
    {   mkmsk tail, tail                        ;   bf tail, .L_finish_s16                  }
    {                                           ;   vstd vec_vd[0]                          }
    {                                           ;   vstr vec_vr[0]                          }
    {                                           ;   vclrdr                                  }
        vlashr b[0], b_shr
    {   add b, b, _32                           ;   vstd vec_tmp[0]                         }
        vstrpv vec_tmp[0], tail
    {                                           ;   vldc vec_tmp[0]                         }
        vlashr c[0], c_shr
    {   mov r11, vec_vr                         ;   vstr vec_tmp[0]                         }
    {                                           ;   vldr r11[0]                             }
    {                                           ;   vldd vec_vd[0]                          }  
    {                                           ;   vlmacc vec_tmp[0]                       }

.L_finish_s16:
        ldd r4, r5, sp[0]
        ldd r6, r7, sp[1]
        ldd r8, r9, sp[2]
    {   ldaw r11, sp[STACK_SAT]                 ;   vadddr                                  }
    {   ldaw r11, sp[STACK_VEC_TMP]             ;   vlsat r11[0]                            }
    {                                           ;   vstr r11[0]                             }
    {                                           ;   ldw r0, sp[STACK_VEC_TMP]               }
    {                                           ;   retsp NSTACKWORDS                       }
ASM_POSTAMBLE(FNAME_S16, NSTACKWORDS)





ASM_PREAMBLE(FNAME_S32)

FNAME_S32:

/**/    dualentsp NSTACKWORDS
        std r4, r5, sp[0]
        std r6, r7, sp[1]
        std r8, r9, sp[2]

    {   ldc r11, 0                              ;                                           }
    {   shl tail, N, SIZEOF_LOG2_S32            ;   vsetc r11                               }
    {   zext tail, 5                            ;   vclrdr                                  }
    {   shr N, N, EPV_LOG2_S32                  ;   ldaw vec_tmp, sp[STACK_VEC_TMP]         }
    {                                           ;   ldw c_shr, sp[STACK_C_SHR]              }

    {   ldaw vec_vr, sp[STACK_VEC_VD + 1]       ;   ldaw vec_vd, sp[STACK_VEC_VD]           }
    {   ldc _32, 32                             ;   bf N, .L_loop_bot_s32                   }

.L_loop_top_s32:
        {   add vec_vd, vec_vd, _32                 ;   vstd vec_vd[0]                          }
        {   sub vec_vd, vec_vd, _32                 ;   vstr vec_vd[0]                          }
            vlashr b[0], b_shr
        {   add b, b, _32                           ;   vstr vec_tmp[0]                         }
        {                                           ;   vldc vec_tmp[0]                         }
            vlashr c[0], c_shr
        {   mov r11, vec_vr                         ;   vstr vec_tmp[0]                         }
        {   sub vec_vr, vec_vr, _32                 ;   vldr r11[0]                             }
        {   add vec_vr, vec_vr, _32                 ;   vldd vec_vr[0]                          }  
        {   sub N, N, 1                             ;   vlmaccr vec_tmp[0]                      }
        {   add c, c, _32                           ;   bt N, .L_loop_top_s32                   }
.L_loop_bot_s32:
    {   mkmsk tail, tail                        ;   bf tail, .L_finish_s32                  }
    {   add vec_vd, vec_vd, _32                 ;   vstd vec_vd[0]                          }
    {   sub vec_vd, vec_vd, _32                 ;   vstr vec_vd[0]                          }
    {                                           ;   vclrdr                                  }
        vlashr b[0], b_shr
    {   add b, b, _32                           ;   vstd vec_tmp[0]                         }
        vstrpv vec_tmp[0], tail
    {                                           ;   vldc vec_tmp[0]                         }
        vlashr c[0], c_shr
    {   mov r11, vec_vr                         ;   vstr vec_tmp[0]                         }
    {   sub vec_vr, vec_vr, _32                 ;   vldr r11[0]                             }
    {                                           ;   vldd vec_vr[0]                          }  
    {                                           ;   vlmaccr vec_tmp[0]                      }

.L_finish_s32:

        ldd r4, r5, sp[0]
        ldd r6, r7, sp[1]
        ldd r8, r9, sp[2]
    {   ldaw r11, sp[STACK_SAT]                 ;                                           }
    {   ldaw r11, sp[STACK_VEC_TMP]             ;   vlsat r11[0]                            }
    {                                           ;   vstr r11[0]                             }
    {                                           ;   ldw r0, sp[STACK_VEC_TMP]               }
    {                                           ;   retsp NSTACKWORDS                       }

ASM_POSTAMBLE(FNAME_S32, NSTACKWORDS)





#endif //!defined(XS3_MATH_NO_ASM)
#endif //defined(__XS3A__)