// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.



#if defined(__XS3A__)


/*  

headroom_t vect_complex_s32_to_vect_complex_s16(
    int16_t* a_real,
    int16_t* a_imag,
    const complex_s32_t* b,
    const unsigned length,
    const right_shift_t b_shr);

*/

#include "../asm_helper.h"

.text
.issue_mode dual
.align 4

#define NSTACKVECS      (2)
#define NSTACKWORDS     (6 + (8*NSTACKVECS))

#define FUNCTION_NAME   vect_complex_s32_to_vect_complex_s16

#define STACK_VEC_TMP  (NSTACKWORDS-16)

#define STACK_B_SHR     (NSTACKWORDS+1)

#define a_real      r0
#define a_imag      r1
#define b           r2
#define len         r3
#define b_shr       r4
#define tail        r5
#define _28         r6
#define mask        r7


.cc_top FUNCTION_NAME.function,FUNCTION_NAME
FUNCTION_NAME:
        dualentsp NSTACKWORDS
        std r4, r5, sp[1]
        std r6, r7, sp[2]
    {   ldc r11, 0                              ;   ldc _28, 28                             }
    sub b,b,4
    {   ldc r11, 16                             ;   vsetc r11                               }
    {   shr len, len, 2                         ;   shl tail, len, 1                        }
    {   mkmsk mask, 8                           ;   ldw b_shr, sp[STACK_B_SHR]              }
    {   sub b_shr, b_shr, r11                   ;   zext tail, 3                            }
        ldaw r11, cp[vpu_vec_complex_pos_j]
    {   mkmsk tail, tail                        ;   bf len, .L_loop_bot                     }

    .L_loop_top:
            vlashr b[0], b_shr
        {   add b, b, 4                             ;   vlmul r11[0]                            }   
        {   sub len, len, 1                         ;   vdepth16                                }
        {                                           ;   vdepth16                                }
            vstrpv a_real[0], mask
            vlashr b[0], b_shr         
        {   add b, b, _28                           ;   vlmul r11[0]                            }
        {   add a_real, a_real, 8                   ;   vdepth16                                }
        {                                           ;   vdepth16                                }
            vstrpv a_imag[0], mask
        {   add a_imag, a_imag, 8                   ;   bt len, .L_loop_top                     }


.L_loop_bot:

    entsp 0

.issue_mode single

    bf tail, .L_finish
    vlashr b[0], b_shr
    vlmul r11[0]
    vdepth16
    vdepth16
    vstrpv a_real[0], tail
    add b, b, 4
    vlashr b[0], b_shr         
    vlmul r11[0]
    vdepth16
    vdepth16
    vstrpv a_imag[0], tail


.L_finish:
        ldd r4, r5, sp[1]
        ldd r6, r7, sp[2]
        retsp NSTACKWORDS

.L_func_end:
.cc_bottom FUNCTION_NAME.function

.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME


#endif //defined(__XS3A__)