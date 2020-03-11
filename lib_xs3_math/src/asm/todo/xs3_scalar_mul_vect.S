
#if defined(__XS3A__)

#include "asm_helper.h"


#define NSTACKWORDS         (4+8)
#define STACK_TMP_VEC       (4)
#define STACK_TMP_VEC_DBL   (2)
#define STACK_BYTEMASK      (1)


#define a           r0 
#define b           r1 
#define c           r2
#define len         r3
#define shr_b       r4
#define tmp_vec     r5

#define tmp         c
#define bytemask    len


    // {                                           ;                                       }
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

/*  
headroom_t xs3_scalar_mul_vect_s16(
    int16_t* a,
    const int16_t* b,
    const int16_t c,
    const unsigned length,
    const int b_shr);
*/

#define FUNCTION_NAME xs3_scalar_mul_vect_s16
        
ASM_PREAMBLE(FUNCTION_NAME)

.align 4

.L_mode16:
.int 0x0100

FUNCTION_NAME:
    dualentsp NSTACKWORDS
    std r4, r5, sp[1]
    {   mov r11, c                              ;                                       }
    zip r11, c, 4

    std c, c, sp[STACK_TMP_VEC_DBL+0]
    std c, c, sp[STACK_TMP_VEC_DBL+1]
    std c, c, sp[STACK_TMP_VEC_DBL+2]
    std c, c, sp[STACK_TMP_VEC_DBL+3]

    {   shl r11, len, 1                         ;   ldw shr_b, sp[NSTACKWORDS+1]    }
    {   ldc tmp, 32                             ;                                       }
    {   zext r11, 5                             ;                                       }
    {   shr len, len, 4                         ;                                       }
    {   ldaw tmp_vec, sp[STACK_TMP_VEC]         ;   mkmsk r11, r11                      }
    {   ldap r11, .L_mode16                     ;   stw r11, sp[STACK_BYTEMASK]         }
    {                                           ;   ldw r11, r11[0]                     }
    {   mkmsk r11, 32                           ;   vsetc r11                           }
    {   sub len, len, 1                         ;   bf len, .L_loop_bot_s16             }

.align 4
.L_loop_top_s16:
    vlashr b[0], shr_b
    {   add b, b, tmp                           ;   vlmul tmp_vec[0]                    }
    {   add c, c, tmp                           ;                                       }
    {   add a, a, tmp                           ;   vstr a[0]                           }
    {   sub len, len, 1                         ;   bt len, .L_loop_top_s16             }

.L_loop_bot_s16:

    {                                           ;   ldw bytemask, sp[STACK_BYTEMASK]    }
    {                                           ;   bf bytemask, .L_done_s16            }
    {                                           ;   vclrdr                              }
    vlashr b[0], shr_b
    {                                           ;   vlmul tmp_vec[0]                    }
    {   mov r11, tmp_vec                        ;   vstd tmp_vec[0]                     }
    vstrpv tmp_vec[0], bytemask
    vstrpv a[0], bytemask
    {                                           ;   vldr r11[0]                         }
    {                                           ;   vstr r11[0]                         }

.L_done_s16:

    ldd r4, r5, sp[1]

    {   ldc r0, 15                              ;   vgetc r11                           }
    {   zext r11, 5                             ;                                       }
    {   sub r0, r0, r11                         ;   retsp NSTACKWORDS                   }

ASM_POSTAMBLE(FUNCTION_NAME, NSTACKWORDS)

#undef FUNCTION_NAME

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

/*  
headroom_t xs3_scalar_mul_vect_s32(
    int32_t* a,
    const int32_t* b,
    const int32_t c,
    const unsigned length,
    const int b_shr);
*/

#define FUNCTION_NAME xs3_scalar_mul_vect_s32
    
ASM_PREAMBLE(FUNCTION_NAME)

FUNCTION_NAME:
    dualentsp NSTACKWORDS
    std r4, r5, sp[1]
    {   mov r11, c                              ;                                       }
    zip r11, c, 4

    std c, c, sp[STACK_TMP_VEC_DBL+0]
    std c, c, sp[STACK_TMP_VEC_DBL+1]
    std c, c, sp[STACK_TMP_VEC_DBL+2]
    std c, c, sp[STACK_TMP_VEC_DBL+3]

    {   shl r11, len, 2                         ;   ldw shr_b, sp[NSTACKWORDS+1]    }
    {   ldc tmp, 32                             ;                                       }
    {   zext r11, 5                             ;                                       }
    {   shr len, len, 3                         ;                                       }
    {   ldaw tmp_vec, sp[STACK_TMP_VEC]         ;   mkmsk r11, r11                      }
    {   ldc r11, 0                              ;   stw r11, sp[STACK_BYTEMASK]         }
    {   mkmsk r11, 32                           ;   vsetc r11                           }
    {   sub len, len, 1                         ;   bf len, .L_loop_bot_s32             }

.align 4
.L_loop_top_s32:
    vlashr b[0], shr_b
    {   add b, b, tmp                           ;   vlmul tmp_vec[0]                    }
    {   add c, c, tmp                           ;                                       }
    {   add a, a, tmp                           ;   vstr a[0]                           }
    {   sub len, len, 1                         ;   bt len, .L_loop_top_s32             }

.L_loop_bot_s32:

    {                                           ;   ldw bytemask, sp[STACK_BYTEMASK]    }
    {                                           ;   bf bytemask, .L_done_s32            }
    {                                           ;   vclrdr                              }
    vlashr b[0], shr_b
    {                                           ;   vlmul tmp_vec[0]                    }
    {   mov r11, tmp_vec                        ;   vstd tmp_vec[0]                     }
    vstrpv tmp_vec[0], bytemask
    vstrpv a[0], bytemask
    {                                           ;   vldr r11[0]                         }
    {                                           ;   vstr r11[0]                         }

.L_done_s32:

    ldd r4, r5, sp[1]

    {   ldc r0, 31                              ;   vgetc r11                           }
    {   zext r11, 5                             ;                                       }
    {   sub r0, r0, r11                         ;   retsp NSTACKWORDS                   }

ASM_POSTAMBLE(FUNCTION_NAME, NSTACKWORDS)

#undef FUNCTION_NAME


//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////


#endif //defined(__XS3A__)



