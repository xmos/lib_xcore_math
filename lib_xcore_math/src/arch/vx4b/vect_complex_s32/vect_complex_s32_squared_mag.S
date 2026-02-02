// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)

#include "../asm_helper.h"

/*  

headroom_t vect_complex_s32_squared_mag(
    int32_t a[],
    const complex_s32_t* b,
    const unsigned length,
    const right_shift_t b_shr);

*/

.text
.p2align 2


#define NSTACKWORDS     (7+8+1)

#define a               a0 
#define b               a1 
#define length          a2
#define b_shr           a3
#define vec_count       x18
#define _16             x19
#define vec_tmp         s4
#define tail_mask       s5
#define vec_ones        x22
#define tmpA            s7
#define tmpB            s8


#define STACK_VEC_TMP   (NSTACKWORDS-8-1)

#define FUNCTION_NAME vect_complex_s32_squared_mag
    


FUNCTION_NAME:
    xm.entsp (NSTACKWORDS)*4
    xm.stdsp  s3,s2,0
    xm.stdsp  s5,s4,8
    xm.stdsp  s7,s6,16
    { nop                               ; sw s8, 24                          (sp) }

    { li _16, 16                        ; srli vec_count, length, 2             }
    { li t3, 0                          ; slli tail_mask, length, 3             }
    { xm.zexti tail_mask, 5             ; xm.vsetc t3                           }

la t3, vpu_vec_complex_ones
    { addi vec_tmp,sp, (STACK_VEC_TMP)*4 ; mv vec_ones, t3                       }
    { xm.mkmski t3, 32                  ; xm.brff vec_count, .L_loop_bot        }

    .L_loop_top:
            xm.vlashr b, b_shr
            xm.vstrpv vec_tmp, t3
        { add b, b, _16                 ; xm.vldc vec_tmp                       }
        { add b, b, _16                 ; xm.vldd vec_tmp                       }
        { addi vec_count, vec_count, -1 ; xm.vcmcr0                             }
        { nop /* zero out imag part so that we don't */ ; xm.vlmul0 vec_ones                    }
        { nop /* clobber the headroom counter        */ ; xm.vstr vec_tmp                       }
        { nop                           ; lw tmpA,0                    ( vec_tmp) }
        { nop                           ; lw tmpB,8                    ( vec_tmp) }
            xm.stdi  tmpA,tmpB, 0(a)
        { nop                           ; lw tmpA,16                    ( vec_tmp) }
        { nop                           ; lw tmpB,24                    ( vec_tmp) }
            xm.stdi  tmpA,tmpB, 8(a)
        { add a, a, _16                 ; xm.bt vec_count, .L_loop_top          }

    .L_loop_bot:
    { xm.zexti length, 2                ; xm.brff tail_mask, .L_done            }
    { slli length, length,1             ; xm.vclrdr                             }
    { xm.mkmsk tail_mask, tail_mask     ; xm.vstd vec_tmp                       }
        xm.vlashr b, b_shr
        xm.vstrpv vec_tmp, tail_mask
    { li tmpA, 6                        ; xm.vldd vec_tmp                       }
    { sub length, tmpA, length          ; xm.vldc vec_tmp                       }
    { nop                               ; xm.vcmcr0                             }
    { nop                               ; xm.vlmul0 vec_ones                    }
    { xm.shli length, length, 1         ; xm.vstr vec_tmp                       }
    { nop                               ; xm.bru length                         }
    { nop                               ; lw t3,16                     ( vec_tmp) }
    { nop                               ; sw t3,8                           ( a) }
    { nop                               ; lw t3,8                     ( vec_tmp) }
    { nop                               ; sw t3,4                           ( a) }
    { nop                               ; lw t3,0                     ( vec_tmp) }
    { nop                               ; sw t3,0                           ( a) }
.L_done:
        xm.lddsp  s3,s2,0
        xm.lddsp  s5,s4,8
        xm.lddsp  s7,s6,16

    { li a0, 31                         ; xm.vgetc t3                           }
    { xm.zexti t3, 5                    ; lw s8, 24                          (sp) }
    { sub a0, a0, t3                    ; xm.retsp (NSTACKWORDS)*4              }


.L_func_end:

.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME

#undef FUNCTION_NAME



#endif //defined(__VX4B__)



