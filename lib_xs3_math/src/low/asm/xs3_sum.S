
#if defined(__XS3A__)
#ifndef XS3_MATH_NO_ASM

/*  
int32_t xs3_sum_s16(
    const int16_t* b,
    const unsigned length);

//TODO: There must be a more efficient way to get the final result for xs3_sum_s32(). The way it is now
//      uses way too much code memory.

int64_t xs3_sum_s32(
    const int32_t* b,
    const unsigned length);
*/


#include "asm_helper.h"

.text
.issue_mode dual

#define NSTACKWORDS     (16)


#define FUNCTION_NAME   xs3_sum
#define FNAME_S16       CAT(FUNCTION_NAME, _s16)
#define FNAME_S32       CAT(FUNCTION_NAME, _s32)

#define STACK_VEC_TMP       (NSTACKWORDS-8)

#define b           r0
#define N           r1
#define tail        r2



ASM_PREAMBLE(FNAME_S16)

.L_const_s16:
.word 0x00010001, 0x00010001, 0x00010001, 0x00010001, 0x00010001, 0x00010001, 0x00010001, 0x00010001

FNAME_S16:
/**/    dualentsp NSTACKWORDS
        ldc r11, 0x0100
        std r4, r5, sp[0]

    {   shl tail, N, SIZEOF_LOG2_S16            ;   vsetc r11                               }
    {   zext tail, 5                            ;   vclrdr                                  }
    {   shr N, N, EPV_LOG2_S16                  ;   bf tail, .L_tail_dealt_with_s16         }

    {   ldap r11, .L_const_s16                  ;                                           }
    {   ldaw r4, sp[STACK_VEC_TMP]              ;   vldr r11[0]                             }
    {                                           ;   vstd r4[0]                              }
    {   mkmsk tail, tail                        ;   shl N, N, 3                             }
        vstrpv r4[0], tail
        ldaw r5, b[N]                
    {                                           ;   vldc r4[0]                              }
    {                                           ;   vclrdr                                  }
    {   shr N, N, 3                             ;   vlmacc r5[0]                            }
    {   ldc r11, 32                             ;   vldc r11[0]                             }

.L_tail_dealt_with_s16:
    {   ldap r11, .L_const_s16                  ;   bf N, .L_loop_bot_s16                   }
    {   ldc r11, 32                             ;   vldc r11[0]                             }

.L_loop_top_s16:
        {   sub N, N, 1                             ;   vlmacc b[0]                             }
        {   add b, b, r11                           ;   bt N, .L_loop_top_s16                   }
.L_loop_bot_s16:

.L_finish_s16:

        ldd r4, r5, sp[0]
    {   ldaw r1, sp[STACK_VEC_TMP]              ;   vadddr                                  }
    {                                           ;   vstd r1[0]                              }
    {                                           ;   ldw r11, sp[STACK_VEC_TMP]              }
    {   shl r11, r11, 16                        ;   vstr r1[0]                              }
    {                                           ;   ldw r1, sp[STACK_VEC_TMP]               }
    {   or r0, r11, r1                          ;   retsp NSTACKWORDS                       }
ASM_POSTAMBLE(FNAME_S16, NSTACKWORDS)





ASM_PREAMBLE(FNAME_S32)

.L_const_s32:
.word 0x40000000, 0x40000000, 0x40000000, 0x40000000, 0x40000000, 0x40000000, 0x40000000, 0x40000000
FNAME_S32:

/**/    dualentsp NSTACKWORDS
        std r4, r5, sp[0]

    {   ldc r11, 0                              ;                                           }
    {   shl tail, N, SIZEOF_LOG2_S32            ;   vsetc r11                               }
    {   zext tail, 5                            ;   vclrdr                                  }
    {   shr N, N, EPV_LOG2_S32                  ;   bf tail, .L_tail_dealt_with_s32         }

    {   ldap r11, .L_const_s32                  ;                                           }
    {   ldaw r4, sp[STACK_VEC_TMP]              ;   vldr r11[0]                             }
    {                                           ;   vstd r4[0]                              }
    {   mkmsk tail, tail                        ;   shl N, N, 3                             }
        vstrpv r4[0], tail
        ldaw r5, b[N]                
    {                                           ;   vldc r4[0]                              }
    {                                           ;   vclrdr                                  }
    {   shr N, N, 3                             ;   vlmacc r5[0]                            }
    {   ldc r11, 32                             ;   vldc r11[0]                             }

.L_tail_dealt_with_s32:
    {   ldap r11, .L_const_s32                  ;   bf N, .L_loop_bot_s32                   }
    {   ldc r11, 32                             ;   vldc r11[0]                             }

.L_loop_top_s32:
        {   sub N, N, 1                             ;   vlmacc b[0]                             }
        {   add b, b, r11                           ;   bt N, .L_loop_top_s32                   }
.L_loop_bot_s32:

.L_finish_s32:

    //VADDDR instruction doesn't work for 32-bit values. We'll have to do this without the VPU
    
    // there MUST be a more efficient way to do this.... (could make the VPU handle this by using
    //  VLMACCR instead of VLMACC and storing and re-loading (with an offset) vD and vR between
    //  each VLMACCR so that it's always using the same accumulator... but then the loop has 3
    //  times as many instructions... plus an FNOP


    {   ldaw r11, sp[STACK_VEC_TMP]             ;                                           }
    
    // (vD:vR)[k] ==  ((int32_t)vD[k])*(2^32) + ((uint32_t)vR[k])
    // So start with the vR values; treat them as unsigned, and seed the accumulator
    // with that value.
    {   ldc r2, 1                               ;   vstr r11[0]                             }
    {   ldc r0, 0                               ;   ldc r1, 0                               }

    {                                           ;   ldw r3, sp[STACK_VEC_TMP+0]             }
    maccu r1, r0, r2, r3
    {                                           ;   ldw r3, sp[STACK_VEC_TMP+1]             }
    maccu r1, r0, r2, r3
    {                                           ;   ldw r3, sp[STACK_VEC_TMP+2]             }
    maccu r1, r0, r2, r3
    {                                           ;   ldw r3, sp[STACK_VEC_TMP+3]             }
    maccu r1, r0, r2, r3
    {                                           ;   ldw r3, sp[STACK_VEC_TMP+4]             }
    maccu r1, r0, r2, r3
    {                                           ;   ldw r3, sp[STACK_VEC_TMP+5]             }
    maccu r1, r0, r2, r3
    {                                           ;   ldw r3, sp[STACK_VEC_TMP+6]             }
    maccu r1, r0, r2, r3
    {                                           ;   ldw r3, sp[STACK_VEC_TMP+7]             }
    maccu r1, r0, r2, r3

    //Now deal with the stuff from vD. Ideally we'd be multiplying these directly by 2^32, but
    //we can't. We could multiply by (2^31)-1, but that would take 3 maccs (2^31-1, 2^31-1, 2)
    // to get the full value accumulated. Instead, we can use (-2^31 = 0x80000000) and only
    //  need to do two maccs per value. But then we need to also do a NEG on each value.

    {   ldc r2, 31                              ;   vstd r11[0]                             }
    {   mkmsk r2, r2                            ;                                           }
    {   add r2, r2, 1                           ;   ldw r3, sp[STACK_VEC_TMP+0]             }
    {   neg r3, r3                              ;   ldw r4, sp[STACK_VEC_TMP+1]             }
        maccs r1, r0, r2, r3
        maccs r1, r0, r2, r3
    {   neg r4, r4                              ;   ldw r3, sp[STACK_VEC_TMP+2]             }
        maccs r1, r0, r2, r4
        maccs r1, r0, r2, r4
    {   neg r3, r3                              ;   ldw r4, sp[STACK_VEC_TMP+3]             }
        maccs r1, r0, r2, r3
        maccs r1, r0, r2, r3
    {   neg r4, r4                              ;   ldw r3, sp[STACK_VEC_TMP+4]             }
        maccs r1, r0, r2, r4
        maccs r1, r0, r2, r4
    {   neg r3, r3                              ;   ldw r4, sp[STACK_VEC_TMP+5]             }
        maccs r1, r0, r2, r3
        maccs r1, r0, r2, r3
    {   neg r4, r4                              ;   ldw r3, sp[STACK_VEC_TMP+6]             }
        maccs r1, r0, r2, r4
        maccs r1, r0, r2, r4
    {   neg r3, r3                              ;   ldw r4, sp[STACK_VEC_TMP+7]             }
        maccs r1, r0, r2, r3
        maccs r1, r0, r2, r3
    {   neg r4, r4                              ;                                           }
        maccs r1, r0, r2, r4
        maccs r1, r0, r2, r4

        ldd r4, r5, sp[0]
        retsp NSTACKWORDS

ASM_POSTAMBLE(FNAME_S32, NSTACKWORDS)





#endif //!defined(XS3_MATH_NO_ASM)
#endif //defined(__XS3A__)