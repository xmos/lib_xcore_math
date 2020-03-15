
#if defined(__XS3A__)
#ifndef XS3_MATH_NO_ASM

#include "asm_helper.h"

/*  
headroom_t xs3_mul_vect_s16(
    int16_t* a,
    const int16_t* b,
    const int16_t* c,
    const unsigned len,
    const int b_shr,
    const int c_shr);

headroom_t xs3_mul_vect_s32(
    int32_t* a,
    const int32_t* b,
    const int32_t* c,
    const unsigned len,
    const int b_shr,
    const int c_shr);
*/


#define NSTACKWORDS     (8+8)

#define FUNCTION_NAME   xs3_mul_vect
#define FNAME_S16       CAT(FUNCTION_NAME, _s16)
#define FNAME_S32       CAT(FUNCTION_NAME, _s32)

#define STACK_B_SHR     (NSTACKWORDS+1)
#define STACK_C_SHR     (NSTACKWORDS+2)
#define STACK_VEC_TMP   (NSTACKWORDS-8)
#define STACK_BYTEMASK  6

#define a           r0 
#define b           r1 
#define c           r2
#define len         r3
#define shr_b       r4
#define shr_c       r5
#define _32         r6
#define tmp_vec     r7
#define bytemask    len

.text
.issue_mode dual

ASM_PREAMBLE_LCL(apply_op);
.L_apply_op:

/**/    std r4, r5, sp[1]
        std r6, r7, sp[2]
    {   ldc _32, 32                             ;   vclrdr                              }
    {   mkmsk r11, r11                          ;   ldw shr_c, sp[STACK_C_SHR]          }
/**/{   ldaw tmp_vec, sp[NSTACKWORDS-8]         ;   ldw shr_b, sp[STACK_B_SHR]          }
    {   mkmsk r11, 32                           ;   stw r11, sp[STACK_BYTEMASK]         }
    {                                           ;   bf len, .L_loop_bot                 }
    {                                           ;   bu .L_loop_top                      }

.L_loop_top:
/**/        vlashr b[0], shr_b
            vstrpv tmp_vec[0], r11
            vlashr c[0], shr_c
        {   add b, b, _32                           ;                                       } 
/**/    {   add c, c, _32                           ;   vlmul tmp_vec[0]                    }  
        {   sub len, len, 1                         ;   vstr a[0]                           }
        {   add a, a, _32                           ;   bt len, .L_loop_top                 }
.L_loop_bot:

    {                                           ;   ldw bytemask, sp[STACK_BYTEMASK]    }
/**/{                                           ;   bf bytemask, .L_finish              }
        vlashr b[0], shr_b
        vstrpv tmp_vec[0], r11 
        vlashr c[0], shr_c
/**/{   mov r11, tmp_vec                        ;   vlmul tmp_vec[0]                    }
    {                                           ;   vstd tmp_vec[0]                     }
        vstrpv tmp_vec[0], bytemask
        vstrpv a[0], bytemask
/**/{                                           ;   vldr r11[0]                         }
    {                                           ;   vstr r11[0]                         }

.L_finish:
    ldd r4, r5, sp[1]
    ldd r6, r7, sp[2]

    // Should work for both 16 and 32 bit modes
/**/{   ldc r0, 32                              ;   vgetc r11                           }
    {   zext r11, 5                             ;   shr r1, r11, 8                      }
    {   shr r0, r0, r1                          ;   add r11, r11, 1                     }
    {   sub r0, r0, r11                         ;   retsp NSTACKWORDS                   } 

ASM_POSTAMBLE_LCL(apply_op);



ASM_PREAMBLE_V2(FNAME_S16);
FNAME_S16:
/**/    dualentsp NSTACKWORDS
        ldc r11, 0x100
    {   shl r11, len, SIZEOF_LOG2_S16           ;   vsetc r11                           }
    {   zext r11, 5                             ;   shr len, len, EPV_LOG2_S16          }
/**/{                                           ;   bu .L_apply_op                      }
ASM_POSTAMBLE(FNAME_S16, NSTACKWORDS);


ASM_PREAMBLE_V2(FNAME_S32);
FNAME_S32:
/**/    dualentsp NSTACKWORDS
    {   ldc r11, 0                              ;                                           }
    {   shl r11, len, SIZEOF_LOG2_S32           ;   vsetc r11                               }
    {   zext r11, 5                             ;   shr len, len, EPV_LOG2_S32              }
/**/{                                           ;   bu .L_apply_op                          }
ASM_POSTAMBLE(FNAME_S32, NSTACKWORDS);

#endif //!defined(XS3_MATH_NO_ASM)
#endif //defined(__XS3A__)



