
#if defined(__XS3A__)

#include "asm_helper.h"


#define NSTACKWORDS     (6+8)

#define a           r0 
#define b           r1 
#define len         r2
#define tmp         r3
#define tmp_vec     r4

#define bytemask    len

#define STACK_BYTEMASK  1


    // {                                           ;                                       }
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

/*  
headroom_t xs3_rect_vect_s16(
    int16_t* a,
    const int16_t* b,
    const unsigned length);
*/

#define FUNCTION_NAME xs3_rect_vect_s16
        
ASM_PREAMBLE(FUNCTION_NAME)

.align 4

.L_mode16:
.int 0x0100

FUNCTION_NAME:
    dualentsp NSTACKWORDS
    std r4, r5, sp[1]
    std r6, r7, sp[2]

    {   shl r11, len, SIZEOF_LOG2_S16           ;                                       }
    {   ldc tmp, 32                             ;                                       }
    {   zext r11, 5                             ;   shr len, len, EPV_LOG2_S16          }
    {   ldaw tmp_vec, sp[NSTACKWORDS-8]         ;   mkmsk r11, r11                      }
    {   ldap r11, .L_mode16                     ;   stw r11, sp[STACK_BYTEMASK]         }
    {                                           ;   ldw r11, r11[0]                     }
    {   mov r11, b                              ;   vsetc r11                           }
    {   sub len, len, 1                         ;   bf len, .L_loop_bot_s16             }

.L_loop_top_s16:
    {   add r11, r11, tmp                       ;   vldr r11[0]                         }
    {                                           ;   vpos                                }
    {   add a, a, tmp                           ;   vstr a[0]                           }
    {   sub len, len, 1                         ;   bt len, .L_loop_top_s16             }

.L_loop_bot_s16:
    {                                           ;   ldw bytemask, sp[STACK_BYTEMASK]    }
    {                                           ;   bf bytemask, .L_done_s16            }
    {                                           ;   vclrdr                              }
    {   mov b, r11                              ;   vldr r11[0]                         }
    {                                           ;   vpos                                }
    {   mov r11, tmp_vec                        ;   vstd tmp_vec[0]                     }
    vstrpv tmp_vec[0], bytemask
    {                                           ;   vldr r11[0]                         }
    {                                           ;   vstr r11[0]                         }
    vstrpv a[0], bytemask

.L_done_s16:
    ldd r4, r5, sp[1]
    ldd r6, r7, sp[2]

    {   ldc r0, HR_SUB_S16                      ;   vgetc r11                           }
    {   zext r11, 5                             ;                                       }
    {   sub r0, r0, r11                         ;   retsp NSTACKWORDS                   }


ASM_POSTAMBLE(FUNCTION_NAME, NSTACKWORDS)

#undef FUNCTION_NAME

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

/*  
headroom_t xs3_rect_vect_s32(
    int16_t* a,
    const int16_t* b,
    const unsigned length);
*/

#define FUNCTION_NAME xs3_rect_vect_s32
    
ASM_PREAMBLE(FUNCTION_NAME)

FUNCTION_NAME:
    dualentsp NSTACKWORDS
    std r4, r5, sp[1]
    std r6, r7, sp[2]

    {   shl r11, len, SIZEOF_LOG2_S32           ;                                       }
    {   ldc tmp, 32                             ;                                       }
    {   zext r11, 5                             ;   shr len, len, EPV_LOG2_S32          }
    {   ldaw tmp_vec, sp[NSTACKWORDS-8]         ;   mkmsk r11, r11                      }
    {   ldc r11, 0                              ;   stw r11, sp[STACK_BYTEMASK]         }
    {   mov r11, b                              ;   vsetc r11                           }
    {   sub len, len, 1                         ;   bf len, .L_loop_bot_s32             }

.L_loop_top_s32:
    {   add r11, r11, tmp                       ;   vldr r11[0]                         }
    {                                           ;   vpos                                }
    {   add a, a, tmp                           ;   vstr a[0]                           }
    {   sub len, len, 1                         ;   bt len, .L_loop_top_s32             }

.L_loop_bot_s32:
    {                                           ;   ldw bytemask, sp[STACK_BYTEMASK]    }
    {                                           ;   bf bytemask, .L_done_s32            }
    {                                           ;   vclrdr                              }
    {   mov b, r11                              ;   vldr r11[0]                         }
    {                                           ;   vpos                                }
    {   mov r11, tmp_vec                        ;   vstd tmp_vec[0]                     }
    vstrpv tmp_vec[0], bytemask
    {                                           ;   vldr r11[0]                         }
    {                                           ;   vstr r11[0]                         }
    vstrpv a[0], bytemask

.L_done_s32:
    ldd r4, r5, sp[1]
    ldd r6, r7, sp[2]

    {   ldc r0, HR_SUB_S32                      ;   vgetc r11                           }
    {   zext r11, 5                             ;                                       }
    {   sub r0, r0, r11                         ;   retsp NSTACKWORDS                   }


ASM_POSTAMBLE(FUNCTION_NAME, NSTACKWORDS)

#undef FUNCTION_NAME


//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////


#endif //defined(__XS3A__)

