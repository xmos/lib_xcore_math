// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// XMOS Public License: Version 1

#if defined(__XS3A__)

#include "../asm_helper.h"

/*  

headroom_t vect_complex_s32_conj_macc(
    complex_s32_t* acc,
    const complex_s32_t* b,
    const complex_s32_t* c,
    const unsigned length,
    const right_shift_t acc_shr,
    const right_shift_t b_shr,
    const right_shift_t c_shr);

*/

.text
.issue_mode dual
.align 4


#define NSTACKWORDS     (8+8)

#define STACK_SHR_ACC   (NSTACKWORDS+1)
#define STACK_SHR_B     (NSTACKWORDS+2)
#define STACK_SHR_C     (NSTACKWORDS+3)

#define STACK_VEC_TMP   (NSTACKWORDS-8)

#define STACK_BYTEMASK  1

#define acc         r0 
#define b           r1 
#define c           r2
#define len         r3
#define shr_b       r4
#define shr_c       r5
#define tmp         r6
#define tmp_vec     r7
#define shr_acc     r8

#define bytemask    len

#define FUNCTION_NAME vect_complex_s32_conj_macc
    


.cc_top FUNCTION_NAME.function,FUNCTION_NAME
FUNCTION_NAME:
    dualentsp NSTACKWORDS
    std r4, r5, sp[1]
    std r6, r7, sp[2]
    std r8, r9, sp[3]

    {                                           ;   ldw shr_acc, sp[STACK_SHR_ACC]          }
    {   shl r11, len, 3                         ;   ldw shr_b, sp[STACK_SHR_B]              }
    {   ldc tmp, 32                             ;   ldw shr_c, sp[STACK_SHR_C]              }
    {   zext r11, 5                             ;   shr len, len, 2                         }
    {   ldaw tmp_vec, sp[STACK_VEC_TMP]         ;   mkmsk r11, r11                          }
    {   ldc r11, 0                              ;   stw r11, sp[STACK_BYTEMASK]             }
    {   mkmsk r11, 32                           ;   vsetc r11                               }
    {   sub len, len, 1                         ;   bf len, .L_loop_bot_s32                 }

.L_loop_top_s32:
        vlashr acc[0], shr_acc
        vstrpv acc[0], r11
        vlashr c[0], shr_c
	      vstrpv tmp_vec[0], r11
    {                                           ;   vldc tmp_vec[0]                         }
	      vlashr b[0], shr_b
        vstrpv tmp_vec[0], r11
    {                                           ;   vldd tmp_vec[0]                         }
    {   add b, b, tmp                           ;   vcmcr                                   }
    {   add c, c, tmp                           ;   vcmci                                   }
    {                                           ;   vladd acc[0]                            }
    {   add acc, acc, tmp                       ;   vstr acc[0]                             }
    {   sub len, len, 1                         ;   bt len, .L_loop_top_s32                 }

.L_loop_bot_s32:
    {                                           ;   ldw bytemask, sp[STACK_BYTEMASK]        }
    {                                           ;   bf len, .L_done_s32                     }
    {                                           ;   vclrdr                                  }
    {                                           ;   vstd tmp_vec[0]                         }
        vlashr acc[0], shr_acc
        vstrpv acc[0], bytemask
        vlashr c[0], shr_c
        vstrpv tmp_vec[0], bytemask
    {                                           ;   vldc tmp_vec[0]                         }
        vlashr b[0], shr_b
        vstrpv tmp_vec[0], bytemask                      
    {                                           ;   vldd tmp_vec[0]                         }
    {                                           ;   vcmcr                                   }
    {   mov r11, tmp_vec                        ;   vcmci                                   }
    {                                           ;   vladd acc[0]                            }
        vstrpv tmp_vec[0], bytemask
    {                                           ;   vldr r11[0]                             }
    {                                           ;   vstr tmp_vec[0]                         }
        vstrpv acc[0], bytemask

.L_done_s32:
        ldd r4, r5, sp[1]
        ldd r6, r7, sp[2]
        ldd r8, r9, sp[3]

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



