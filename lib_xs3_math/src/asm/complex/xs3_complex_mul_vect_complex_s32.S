
#if defined(__XS3A__)

#include "asm_helper.h"

#define NSTACKWORDS     (6+8)

#define a           r0 
#define b           r1 
#define c           r2
#define len         r3
#define shr_b       r4
#define shr_c       r5
#define tmp         r6
#define tmp_vec     r7

#define bytemask    len

#define STACK_BYTEMASK  1


    // {                                           ;                                       }
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

/*  
headroom_t xs3_complex_mul_vect_complex_s32(
    complex_s32_t* a,
    const complex_s32_t* b,
    const complex_s32_t* c,
    const unsigned length,
    const int b_shr,
    const int c_shr);
*/

#define FUNCTION_NAME xs3_complex_mul_vect_complex_s32
    
ASM_PREAMBLE(FUNCTION_NAME)

FUNCTION_NAME:
    dualentsp NSTACKWORDS
    std r4, r5, sp[1]
    std r6, r7, sp[2]

    {   shl r11, len, 3                         ;   ldw shr_b, sp[NSTACKWORDS+1]        }
    {   ldc tmp, 32                             ;   ldw shr_c, sp[NSTACKWORDS+2]        }
    {   zext r11, 5                             ;   shr len, len, 2                     }
    {   ldaw tmp_vec, sp[NSTACKWORDS-8]         ;   mkmsk r11, r11                      }
    {   ldc r11, 0                              ;   stw r11, sp[STACK_BYTEMASK]         }
    {   mkmsk r11, 32                           ;   vsetc r11                           }
    {   sub len, len, 1                         ;   bf len, .L_loop_bot_s32             }

.L_loop_top_s32:
    vlashr b[0], shr_b
	vstrpv tmp_vec[0], r11
    {                                           ;   vldc tmp_vec[0]                     }

	vlashr c[0], shr_c
    vstrpv tmp_vec[0], r11
    {                                           ;   vldd tmp_vec[0]                     }

    {   add b, b, tmp                           ;   vcmr                                }
    {   add c, c, tmp                           ;   vcmi                                }

    {   add a, a, tmp                           ;   vstr a[0]                           }
    {   sub len, len, 1                         ;   bt len, .L_loop_top_s32             }

.L_loop_bot_s32:
    {                                           ;   ldw bytemask, sp[STACK_BYTEMASK]    }
    {                                           ;   bf len, .L_done_s32                 }
    vlashr b[0], shr_b
    vstrpv tmp_vec[0], r11
    {                                           ;   vldc tmp_vec[0]                     }
    vlashr c[0], shr_c
    vstrpv tmp_vec[0], r11                      
    {                                           ;   vldd tmp_vec[0]                     }
    {                                           ;   vcmi                                }
    {   mov r11, tmp_vec                        ;   vcmr                                }
    vstrpv tmp_vec[0], bytemask
    {                                           ;   vclrdr                              }
    {                                           ;   vstr r11[0]                         }
    {                                           ;   vstd tmp_vec[0]                     }
    vstrpv tmp_vec[0], bytemask
    {                                           ;   vldr r11[0]                         }
    {                                           ;   vstr tmp_vec[0]                     }
    vstrpv a[0], bytemask

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



