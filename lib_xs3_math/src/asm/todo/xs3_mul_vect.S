
#if defined(__XS3A__)

#include "asm_helper.h"


#define NSTACKWORDS     (6+8)
#define STACK_TMP_VEC   (NSTACKWORDS-8)
#define STACK_BYTEMASK  (1)


#define arg_a           r0 
#define arg_b           r1 
#define arg_c           r2
#define arg_len         r3
#define arg_shr_b       r4
#define arg_shr_c       r5
#define r_tmp_vec       r6
#define tmp             r7

#define bytemask        arg_len


    // {                                           ;                                       }
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

/*  
headroom_t xs3_mul_vect_s16(
    int16_t* a,
    const int16_t* b,
    const int16_t* c,
    const unsigned length,
    const int b_shr,
    const int c_shr);
*/

#define FUNCTION_NAME xs3_mul_vect_s16
        
ASM_PREAMBLE(FUNCTION_NAME)

.align 4

.L_mode16:
.int 0x0100

FUNCTION_NAME:
    dualentsp NSTACKWORDS
    std r4, r5, sp[1]
    std r6, r7, sp[2]

    {   shl r11, arg_len, 1                     ;   ldw arg_shr_b, sp[NSTACKWORDS+1]    }
    {   ldc tmp, 32                             ;   ldw arg_shr_c, sp[NSTACKWORDS+2]    }
    {   zext r11, 5                             ;   shr arg_len, arg_len, 4             }
    {   ldaw r_tmp_vec, sp[STACK_TMP_VEC]       ;   mkmsk r11, r11                      }
    {   ldap r11, .L_mode16                     ;   stw r11, sp[STACK_BYTEMASK]         }
    {                                           ;   ldw r11, r11[0]                     }
    {   mkmsk r11, 32                           ;   vsetc r11                           }
    {   sub arg_len, arg_len, 1                 ;   bf arg_len, .L_loop_bot_s16         }

.align 4
.L_loop_top_s16:
    vlashr arg_b[0], arg_shr_b
    vstrpv r_tmp_vec[0], r11
    {   add arg_b, arg_b, tmp                   ;                                       }
    vlashr arg_c[0], arg_shr_c
    {   add arg_c, arg_c, tmp                   ;   vlmul r_tmp_vec[0]                  }
    {   add arg_a, arg_a, tmp                   ;   vstr arg_a[0]                       }
    {   sub arg_len, arg_len, 1                 ;   bt arg_len, .L_loop_top_s16         }

.L_loop_bot_s16:

    {                                           ;   ldw bytemask, sp[STACK_BYTEMASK]    }
    {                                           ;   bf bytemask, .L_done_s16            }
    {                                           ;   vclrdr                              }
    vlashr arg_b[0], arg_shr_b
    vstrpv r_tmp_vec[0], r11
    vlashr arg_c[0], arg_shr_c
    {   mov r11, r_tmp_vec                      ;   vlmul r_tmp_vec[0]                  }
    {                                           ;   vstd r_tmp_vec[0]                   }
    vstrpv r11[0], bytemask
    vstrpv arg_a[0], bytemask
    {                                           ;   vldr r11[0]                         }
    {                                           ;   vstr r11[0]                         }

.L_done_s16:

    ldd r4, r5, sp[1]
    ldd r6, r7, sp[2]

    {   ldc r0, 15                              ;   vgetc r11                           }
    {   zext r11, 5                             ;                                       }
    {   sub r0, r0, r11                         ;   retsp NSTACKWORDS                   }


ASM_POSTAMBLE(FUNCTION_NAME, NSTACKWORDS)

#undef FUNCTION_NAME

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

/*  
headroom_t xs3_mul_vect_s32(
    int32_t* a,
    const int32_t* b,
    const int32_t* c,
    const unsigned length,
    const int b_shr,
    const int c_shr);
*/

#define FUNCTION_NAME xs3_mul_vect_s32
    
ASM_PREAMBLE(FUNCTION_NAME)

FUNCTION_NAME:
    dualentsp NSTACKWORDS
    std r4, r5, sp[1]
    std r6, r7, sp[2]

    {   shl r11, arg_len, 2                     ;   ldw arg_shr_b, sp[NSTACKWORDS+1]    }
    {   ldc tmp, 32                             ;   ldw arg_shr_c, sp[NSTACKWORDS+2]    }
    {   zext r11, 5                             ;   shr arg_len, arg_len, 3             }
    {   ldaw r_tmp_vec, sp[STACK_TMP_VEC]       ;   mkmsk r11, r11                      }
    {   ldc r11, 0                              ;   stw r11, sp[STACK_BYTEMASK]         }
    {   mkmsk r11, 32                           ;   vsetc r11                           }
    {   sub arg_len, arg_len, 1                 ;   bf arg_len, .L_loop_bot_s32         }

.align 4
.L_loop_top_s32:
    vlashr arg_b[0], arg_shr_b
    vstrpv r_tmp_vec[0], r11
    {   add arg_b, arg_b, tmp                   ;                                       }
    vlashr arg_c[0], arg_shr_c
    {   add arg_c, arg_c, tmp                   ;   vlmul r_tmp_vec[0]                  }
    {   add arg_a, arg_a, tmp                   ;   vstr arg_a[0]                       }
    {   sub arg_len, arg_len, 1                 ;   bt arg_len, .L_loop_top_s32         }

.L_loop_bot_s32:

    {                                           ;   ldw bytemask, sp[STACK_BYTEMASK]    }
    {                                           ;   bf bytemask, .L_done_s32            }
    {                                           ;   vclrdr                              }
    vlashr arg_b[0], arg_shr_b
    vstrpv r_tmp_vec[0], r11
    vlashr arg_c[0], arg_shr_c
    {   mov r11, r_tmp_vec                      ;   vlmul r_tmp_vec[0]                  }
    {                                           ;   vstd r_tmp_vec[0]                   }
    vstrpv r11[0], bytemask
    vstrpv arg_a[0], bytemask
    {                                           ;   vldr r11[0]                         }
    {                                           ;   vstr r11[0]                         }

.L_done_s32:

    ldd r4, r5, sp[1]
    ldd r6, r7, sp[2]

    {   ldc r0, 31                              ;   vgetc r11                           }
    {   zext r11, 5                             ;                                       }
    {   sub r0, r0, r11                         ;   retsp NSTACKWORDS                   }


ASM_POSTAMBLE(FUNCTION_NAME, NSTACKWORDS)

#undef FUNCTION_NAME


//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////


#endif //defined(__XS3A__)



