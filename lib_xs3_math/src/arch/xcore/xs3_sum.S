
#if defined(__XS3A__)
#ifndef XS3_MATH_NO_ASM

/*  
int32_t xs3_sum_s16(
    const int16_t* b,
    const unsigned length);

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


.globl FNAME_S16
.type FNAME_S16,@function
.align 16;
.cc_top FNAME_S16.function,FNAME_S16

FNAME_S16:
        dualentsp NSTACKWORDS
        ldc r11, 0x0100
        std r4, r5, sp[0]

    {   shl tail, N, SIZEOF_LOG2_S16            ;   vsetc r11                               }
    {   zext tail, 5                            ;   vclrdr                                  }
    {   shr N, N, EPV_LOG2_S16                  ;   bf tail, .L_tail_dealt_with_s16         }

        ldaw r11, cp[vpu_vec_0x0001]
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
    {                                           ;   bf N, .L_loop_bot_s16                   }
        ldaw r11, cp[vpu_vec_0x0001]
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

.cc_bottom FNAME_S16.function; 
.set FNAME_S16.nstackwords,(16);    .global FNAME_S16.nstackwords; 
.set FNAME_S16.maxcores,1;          .global FNAME_S16.maxcores; 
.set FNAME_S16.maxtimers,0;         .global FNAME_S16.maxtimers; 
.set FNAME_S16.maxchanends,0;       .global FNAME_S16.maxchanends; 
.L_fend_s16: 
    .size FNAME_S16, .L_fend_s16 - FNAME_S16




.globl FNAME_S32
.type FNAME_S32,@function
.align 16;
.cc_top FNAME_S32.function,FNAME_S32

FNAME_S32:

        dualentsp NSTACKWORDS
        std r4, r5, sp[0]

    {   ldc r11, 0                              ;                                           }
    {   shl tail, N, SIZEOF_LOG2_S32            ;   vsetc r11                               }
    {   zext tail, 5                            ;   vclrdr                                  }
    {   shr N, N, EPV_LOG2_S32                  ;   bf tail, .L_tail_dealt_with_s32         }

        ldaw r11, cp[vpu_vec_0x40000000]
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
        ldaw r11, cp[vpu_vec_0x40000000]
    {   ldc r11, 32                             ;   vldc r11[0]                             }
    {   ldaw r2, sp[STACK_VEC_TMP]              ;   bf N, .L_loop_bot_s32                   }

.L_loop_top_s32:
        {   sub N, N, 1                             ;   vlmacc b[0]                             }
        {   add b, b, r11                           ;   bt N, .L_loop_top_s32                   }
.L_loop_bot_s32:

.L_finish_s32:


    // Requires vC to be filled with 0x40000000, which it already should be.
        ldaw r11, cp[vpu_vec_0x80000000]        
    {                                           ;   vstr r2[0]                              }
    {                                           ;   vlmacc r11[0]                           }
        ldaw r11, cp[vpu_vec_zero]        
    {   ldaw r11, sp[STACK_VEC_TMP]             ;   vldr r11[0]                             }
    {                                           ;   vlmaccr r11[0]                          }
    {                                           ;   vstd r11[0]                             }
    {                                           ;   vlmaccr r11[0]                          }
    {                                           ;   vstr r11[0]                             }
    {                                           ;   ldw r1, r11[0]                          }
    {   add r1, r1, 8                           ;   ldw r0, r11[1]                          }

        ldd r4, r5, sp[0]
        retsp NSTACKWORDS


.cc_bottom FNAME_S32.function; 
.set FNAME_S32.nstackwords,(16);    .global FNAME_S32.nstackwords; 
.set FNAME_S32.maxcores,1;          .global FNAME_S32.maxcores; 
.set FNAME_S32.maxtimers,0;         .global FNAME_S32.maxtimers; 
.set FNAME_S32.maxchanends,0;       .global FNAME_S32.maxchanends; 
.L_fend_s32: 
    .size FNAME_S32, .L_fend_s32 - FNAME_S32





#endif //!defined(XS3_MATH_NO_ASM)
#endif //defined(__XS3A__)