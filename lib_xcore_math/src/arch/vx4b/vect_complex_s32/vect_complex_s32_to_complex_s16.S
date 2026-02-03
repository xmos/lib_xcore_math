// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.



#if defined(__VX4B__)


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
.p2align 2

#define NSTACKWORDS     (8)

#define FUNCTION_NAME   vect_complex_s32_to_vect_complex_s16

#define a_real      a0
#define a_imag      a1
#define b           a2
#define len         a3
#define b_shr       s2
#define tail        s3
#define _28         s4
#define mask        s5


FUNCTION_NAME:
    xm.entsp (NSTACKWORDS)*4
    xm.stdsp  s3,s2,8
    xm.stdsp  s5,s4,0
    { li t3, 0                          ; li _28, 28                            }
    addi b,b,-4
    { li t3, 16                         ; xm.vsetc t3                           }
    { srli len, len, 2                  ; slli tail, len, 1                     }
    mv b_shr, a4
    { xm.mkmski mask, 8                 ; nop                                   }
    { sub b_shr, b_shr, t3              ; xm.zexti tail, 3                      }
    la t3, vpu_vec_complex_pos_j
    { xm.mkmsk tail, tail               ; xm.brff len, .L_loop_bot              }

    .L_loop_top:
        xm.vlashr b, b_shr
        { addi b, b, 4                  ; xm.vlmul0 t3                          }   
        { addi len, len, -1             ; xm.vdepth16                           }
        { nop                           ; xm.vdepth16                           }
        xm.vstrpv a_real, mask
        xm.vlashr b, b_shr         
        { add b, b, _28                 ; xm.vlmul0 t3                          }
        { addi a_real, a_real, 8        ; xm.vdepth16                           }
        { nop                           ; xm.vdepth16                           }
        xm.vstrpv a_imag, mask
        { addi a_imag, a_imag, 8        ; xm.bt len, .L_loop_top                }


.L_loop_bot:
    beqz tail, .L_finish
    xm.vlashr b, b_shr
    xm.vlmul0 t3
    xm.vdepth16
    xm.vdepth16
    xm.vstrpv a_real, tail
    addi b, b, 4
    xm.vlashr b, b_shr         
    xm.vlmul0 t3
    xm.vdepth16
    xm.vdepth16
    xm.vstrpv a_imag, tail

.L_finish:
        xm.lddsp  s3,s2,8
        xm.lddsp  s5,s4,0
        xm.retsp (NSTACKWORDS)*4

.L_func_end:

.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME


#endif //defined(__VX4B__)
