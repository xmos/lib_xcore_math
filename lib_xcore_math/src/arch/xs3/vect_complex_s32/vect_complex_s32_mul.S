// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)

#include "../asm_helper.h"

/*  

headroom_t vect_complex_s32_mul(
    complex_s32_t* a,
    const complex_s32_t* b,
    const complex_s32_t* c,
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);

*/

.text
.issue_mode dual
.align 4


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

#define STACK_SHR_B     (NSTACKWORDS+1)
#define STACK_SHR_C     (NSTACKWORDS+2)

#define STACK_VEC_TMP   (NSTACKWORDS-8)

#define STACK_BYTEMASK  1

#define FUNCTION_NAME vect_complex_s32_mul
    


.cc_top FUNCTION_NAME.function,FUNCTION_NAME
FUNCTION_NAME:
    dualentsp NSTACKWORDS
    std r4, r5, sp[1]
    std r6, r7, sp[2]

    {   shl r11, len, 3                         ;   ldw shr_b, sp[STACK_SHR_B]              }
    {   ldc tmp, 32                             ;   ldw shr_c, sp[STACK_SHR_C]              }
    {   zext r11, 5                             ;   shr len, len, 2                         }
    {   ldaw tmp_vec, sp[STACK_VEC_TMP]         ;   mkmsk r11, r11                          }
    {   ldc r11, 0                              ;   stw r11, sp[STACK_BYTEMASK]             }
    {   mkmsk r11, 32                           ;   vsetc r11                               }
    {   sub len, len, 1                         ;   bf len, .L_loop_bot_s32                 }

.L_loop_top_s32:
        vlashr b[0], shr_b
	    vstrpv tmp_vec[0], r11
    {                                           ;   vldc tmp_vec[0]                         }
	    vlashr c[0], shr_c
        vstrpv tmp_vec[0], r11
    {                                           ;   vldd tmp_vec[0]                         }
    {   add b, b, tmp                           ;   vcmr                                    }
    {   add c, c, tmp                           ;   vcmi                                    }
    {   add a, a, tmp                           ;   vstr a[0]                               }
    {   sub len, len, 1                         ;   bt len, .L_loop_top_s32                 }

.L_loop_bot_s32:
    {                                           ;   ldw bytemask, sp[STACK_BYTEMASK]        }
    {                                           ;   bf len, .L_done_s32                     }
    {                                           ;   vclrdr                                  }
    {                                           ;   vstd tmp_vec[0]                         }
        vlashr b[0], shr_b
        vstrpv tmp_vec[0], bytemask
    {                                           ;   vldc tmp_vec[0]                         }
        vlashr c[0], shr_c
        vstrpv tmp_vec[0], bytemask                      
    {                                           ;   vldd tmp_vec[0]                         }
    {                                           ;   vcmr                                    }
    {   mov r11, tmp_vec                        ;   vcmi                                    }
        vstrpv tmp_vec[0], bytemask
    {                                           ;   vldr r11[0]                             }
    {                                           ;   vstr tmp_vec[0]                         }
        vstrpv a[0], bytemask

.L_done_s32:
        ldd r4, r5, sp[1]
        ldd r6, r7, sp[2]

    {   ldc r0, 31                              ;   vgetc r11                               }
    {   zext r11, 5                             ;                                           }
    {   sub r0, r0, r11                         ;   retsp NSTACKWORDS                       }


.L_func_end:
.cc_bottom FUNCTION_NAME.function

.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME

#undef FUNCTION_NAME



#endif //defined(__XS3A__)



