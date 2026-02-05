// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)

#include "../asm_helper.h"

/*  

headroom_t vect_complex_s32_mag(
    int32_t a[],
    const complex_s32_t* b,
    const unsigned length,
    const right_shift_t b_shr,
    const complex_s32_t* rot_table
    const unsigned table_rows);

*/

.text
.p2align 2

#define NSTACKVECS      (2)
#define NSTACKWORDS     (8+(8*NSTACKVECS))

#define STACK_VEC_TMP   (NSTACKWORDS-8-1)
#define STACK_VEC_TMP2  (NSTACKWORDS-16-1)

#define FUNCTION_NAME vect_complex_s32_mag

#define Q(R)    R

#define a           a0 
#define b           a1 
#define length      a2
#define b_shr       a3
#define _32         s2
#define vec_tmp     s3
#define mask32      s4
#define tmp         s5
#define iter        s6
#define tail_bytes  s7


FUNCTION_NAME:
    xm.entsp (NSTACKWORDS)*4
    xm.stdsp  s3,s2,8
    xm.stdsp  s5,s4,16
    xm.stdsp  s7,s6,0
    { li s8, 0                          ; sw s8, 24                          (sp) }

    { li _32, 32                        ; li t3, 0                              }
    { addi vec_tmp,sp, (STACK_VEC_TMP)*4 ; xm.vsetc t3                           }
    
    { srli length, length, 2            ; mv tail_bytes, length                 }
    { xm.mkmski mask32, 32              ; xm.zexti tail_bytes, 2                }
    { slli tail_bytes, tail_bytes, 2    ; xm.brff length, .L_outer_loop_bot     }

    .L_outer_loop_top:

            xm.vlashr b, b_shr
        { nop                           ; xm.vsign                              }
        mv t3, a4 
            xm.vstrpv vec_tmp, mask32
            xm.vlashr b, b_shr
        { add b, b, _32                 ; xm.vlmul0 vec_tmp                     }
        mv iter, a5
            xm.vstrpv vec_tmp, mask32

        .L_inner_loop_top:
            { addi iter, iter, -1       ; xm.vldd vec_tmp                       }
            { add t3, t3, _32           ; xm.vldc t3                            }
            { nop                       ; xm.vcmr0                              }
            { nop                       ; xm.vcmi0                              }
                xm.vstrpv vec_tmp, mask32
            { nop                       ; xm.vsign                              }
            { nop                       ; xm.vlmul0 vec_tmp                     }
                xm.vstrpv vec_tmp, mask32
            { nop                       ; xm.bt iter, .L_inner_loop_top         }
        
        { nop                           ; xm.vstr vec_tmp                       }
        { addi length, length, -1       ; lw t3,0                     ( vec_tmp) }
        { nop                           ; sw t3,0                           ( a) }
        { nop                           ; lw t3,8                     ( vec_tmp) }
        { nop                           ; sw t3,4                           ( a) }
        { addi a, a, 8                  ; lw t3,16                     ( vec_tmp) }
        { nop                           ; sw t3,0                           ( a) }
        { nop                           ; lw t3,24                     ( vec_tmp) }
        { addi a, a, 8                  ; sw t3,4                           ( a) }
        { nop                           ; xm.bt length, .L_outer_loop_top       }

    .L_outer_loop_bot:  

    { xm.mkmsk tail_bytes, tail_bytes   ; xm.brff tail_bytes, .L_done           }
        xm.vlashr b, b_shr

    mv t3, a4
    { nop                               ; xm.vsign                              }
        xm.vstrpv vec_tmp, mask32
        xm.vlashr b, b_shr
    { nop                               ; xm.vlmul0 vec_tmp                     }
    mv iter, a5
        xm.vstrpv vec_tmp, mask32
              
    .L_inner_loop2_top:
        { addi iter, iter, -1           ; xm.vldd vec_tmp                       }
        { add t3, t3, _32               ; xm.vldc t3                            }
        { nop                           ; xm.vcmr0                              }
        { nop                           ; xm.vcmi0                              }
            xm.vstrpv vec_tmp, mask32
        { nop                           ; xm.vsign                              }
        { nop                           ; xm.vlmul0 vec_tmp                     }
            xm.vstrpv vec_tmp, mask32
        { nop                           ; xm.bt iter, .L_inner_loop2_top        }
        
    { nop                               ; lw t3,0                     ( vec_tmp) }
    { nop                               ; sw t3,0                     ( vec_tmp) }
    { nop                               ; lw t3,8                     ( vec_tmp) }
    { nop                               ; sw t3,4                     ( vec_tmp) }
    { nop                               ; lw t3,16                     ( vec_tmp) }
    { nop                               ; sw t3,8                     ( vec_tmp) }
    { nop                               ; lw t3,24                     ( vec_tmp) }
    { addi t3,sp, (STACK_VEC_TMP)*4     ; sw t3,12                     ( vec_tmp) }
    { nop                               ; xm.vclrdr                             }
    { nop                               ; xm.vldr t3                            }
    { nop                               ; xm.vstd t3                            }
        xm.vstrpv t3, tail_bytes
        xm.vstrpv a, tail_bytes
    { nop                               ; xm.vldd t3                            }
    { nop                               ; xm.vstd t3                            }

.L_done:
        xm.lddsp  s3,s2,8
        xm.lddsp  s5,s4,16
        xm.lddsp  s7,s6,0

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



