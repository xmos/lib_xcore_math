
#if defined(__XS3A__)

#include "asm_helper.h"


#define NSTACKWORDS     (6+8+8)

#define a           r0 
#define len         r1
#define tmp_vec     r2
#define cur_max_vec r3
#define incr        r4
#define tmpA        r5
#define tmpB        r6

#define bytemask    len
#define scal_max    tmpA

#define STACK_BYTEMASK  1

    // {                                           ;                                       }
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

/*  
int16_t xs3_max_s16(
    const int16_t a[], 
    const unsigned length);
*/

#define FUNCTION_NAME xs3_max_s16
        
ASM_PREAMBLE(FUNCTION_NAME)

.align 4

.L_mode16:
.int 0x0100

FUNCTION_NAME:
    dualentsp NSTACKWORDS
    std r4, r5, sp[1]
    std r6, r7, sp[2]

    {   mov r7, len                             ;                                       }
    {   shl r11, len, SIZEOF_LOG2_S16           ;   vclrdr                              }
    {   zext r11, 5                             ;   shr len, len, EPV_LOG2_S16          }
    {   ldaw tmp_vec, sp[NSTACKWORDS- 8]        ;   mkmsk r11, r11                      }
    {   ldaw cur_max_vec, sp[NSTACKWORDS-16]    ;   vclrdr                              }
    {   ldap r11, .L_mode16                     ;   stw r11, sp[STACK_BYTEMASK]         }
    {   ldc incr, 32                            ;   ldw r11, r11[0]                     }
    {   mov r11, a                              ;   vsetc r11                           }
    {   sub len, len, 1                         ;   bf len, .L_loop_bot_s16             }

    {   add a, a, incr                          ;   vldr r11[0]                         }
    {   mov r11, a                              ;   vstr cur_max_vec[0]                 }
    {                                           ;   bf len, .L_loop_bot_s16             }

.L_loop_top_s16:
    {   sub len, len, 1                         ;   vldr r11[0]                         }
    {                                           ;   vlsub cur_max_vec[0]                }
    {                                           ;   vdepth1                             }
    {                                           ;   vstr tmp_vec[0]                     }
    {                                           ;   ldw tmpA, tmp_vec[0]                }
    {   mov tmpB, tmpA                          ;   vldr r11[0]                         }
    zip tmpB, tmpA, 0
    vstrpv cur_max_vec[0], tmpA
    {   add r11, r11, incr                      ;   bt len, .L_loop_top_s16             }

.L_loop_bot_s16:
    
    {                                           ;   ldw bytemask, sp[STACK_BYTEMASK]    }
    {                                           ;   vldr r11[0]                         }
    {                                           ;   vlsub cur_max_vec[0]                }
    {                                           ;   vdepth1                             }
    {                                           ;   vstr tmp_vec[0]                     }
    {                                           ;   ldw tmpA, tmp_vec[0]                }
    {   mov tmpB, tmpA                          ;   vldr r11[0]                         }
    zip tmpB, tmpA, 0
    {   and tmpA, tmpA, bytemask                ;                                       }
    vstrpv cur_max_vec[0], tmpA
    {   add r11, r11, incr                      ;   bt len, .L_loop_top_s16             }

//Loop through the cur_max_vec to find the max

    {   ldc len, HR_SUB_S16                     ;   sub r7, r7, 1                       }
    {   lss r11, r7, len                        ;                                       }
    {                                           ;   bf r11, .L_loop2_pre_s16            }
    {   mov r7, len                             ;                                       }
.L_loop2_pre_s16:
    {   sub len, len, 1                         ;   ld16s scal_max, cur_max_vec[len]    }
.L_loop2_top_s16:
    {                                           ;   ld16s tmpB, cur_max_vec[len]        }
    {   lss r11, scal_max, tmpB                 ;                                       }
    {                                           ;   bf r11, .L_loop2_partb_s16          } 
    {   mov scal_max, tmpB                      ;                                       }
.L_loop2_partb_s16:
    {   sub len, len, 1                         ;   bt len, .L_loop2_top_s16            }
.L_loop2_bot_s16:
    
.L_done_s16:
    mov r0, scal_max
    ldd r4, r5, sp[1]
    ldd r6, r7, sp[2]
    retsp NSTACKWORDS


ASM_POSTAMBLE(FUNCTION_NAME, NSTACKWORDS)

#undef FUNCTION_NAME

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

/*  
int32_t xs3_max_s32(
    const int32_t a[],
    const unsigned length);
*/

#define FUNCTION_NAME xs3_max_s32
    
ASM_PREAMBLE(FUNCTION_NAME)

FUNCTION_NAME:
    dualentsp NSTACKWORDS
    std r4, r5, sp[1]
    std r6, r7, sp[2]

    {   mov r7, len                             ;                                       }
    {   shl r11, len, SIZEOF_LOG2_S32           ;   vclrdr                              }
    {   zext r11, 5                             ;   shr len, len, EPV_LOG2_S32          }
    {   ldaw tmp_vec, sp[NSTACKWORDS- 8]        ;   mkmsk r11, r11                      }
    {   ldaw cur_max_vec, sp[NSTACKWORDS-16]    ;   vclrdr                              }
    {   ldc r11, 0                              ;   stw r11, sp[STACK_BYTEMASK]         }
    {   ldc incr, 32                            ;                                       }
    {   mov r11, a                              ;   vsetc r11                           }
    {   sub len, len, 1                         ;   bf len, .L_loop_bot_s32             }

    {   add a, a, incr                          ;   vldr r11[0]                         }
    {   mov r11, a                              ;   vstr cur_max_vec[0]                 }
    {                                           ;   bf len, .L_loop_bot_s32             }

.L_loop_top_s32:
    {   sub len, len, 1                         ;   vldr r11[0]                         }
    {                                           ;   vlsub cur_max_vec[0]                }
    {                                           ;   vdepth1                             }
    {                                           ;   vstr tmp_vec[0]                     }
    {                                           ;   ldw tmpA, tmp_vec[0]                }
    {   mov tmpB, tmpA                          ;   vldr r11[0]                         }
    zip tmpB, tmpA, 0
    vstrpv cur_max_vec[0], tmpA
    {   add r11, r11, incr                      ;   bt len, .L_loop_top_s32             }

.L_loop_bot_s32:
    
    {                                           ;   ldw bytemask, sp[STACK_BYTEMASK]    }
    {                                           ;   vldr r11[0]                         }
    {                                           ;   vlsub cur_max_vec[0]                }
    {                                           ;   vdepth1                             }
    {                                           ;   vstr tmp_vec[0]                     }
    {                                           ;   ldw tmpA, tmp_vec[0]                }
    {   mov tmpB, tmpA                          ;   vldr r11[0]                         }
    zip tmpB, tmpA, 0
    {   and tmpA, tmpA, bytemask                ;                                       }
    vstrpv cur_max_vec[0], tmpA
    {   add r11, r11, incr                      ;   bt len, .L_loop_top_s32             }

//Loop through the cur_max_vec to find the max

    {   ldc len, HR_SUB_S32                     ;   sub r7, r7, 1                       }
    {   lss r11, r7, len                        ;                                       }
    {                                           ;   bf r11, .L_loop2_pre_s32            }
    {   mov r7, len                             ;                                       }
.L_loop2_pre_s32:
    {   sub len, len, 1                         ;   ldw scal_max, cur_max_vec[len]      }
.L_loop2_top_s32:
    {                                           ;   ldw tmpB, cur_max_vec[len]          }
    {   lss r11, scal_max, tmpB                 ;                                       }
    {                                           ;   bf r11, .L_loop2_partb_s32          } 
    {   mov scal_max, tmpB                      ;                                       }
.L_loop2_partb_s32:
    {   sub len, len, 1                         ;   bt len, .L_loop2_top_s32            }
.L_loop2_bot_s32:
    
.L_done_s32:
    mov r0, scal_max
    ldd r4, r5, sp[1]
    ldd r6, r7, sp[2]
    retsp NSTACKWORDS


ASM_POSTAMBLE(FUNCTION_NAME, NSTACKWORDS)

#undef FUNCTION_NAME

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////


#endif //defined(__XS3A__)

