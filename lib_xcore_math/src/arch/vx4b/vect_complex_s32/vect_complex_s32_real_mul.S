// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)

#include "../asm_helper.h"

/*  

headroom_t vect_complex_s32_real_mul(
    complex_s32_t* a,
    const complex_s32_t* b,
    const int32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);

*/

.text
.p2align 2

#define FUNCTION_NAME vect_complex_s32_real_mul

#define NSTACKVECS      (2)
#define NSTACKWORDS     (12+8*NSTACKVECS+4)


#define STACK_VEC_TMP   (NSTACKWORDS-8-4)
#define STACK_VEC_C1    (NSTACKWORDS-16-4)


#define STACK_TAIL_LEN  (8)
    

#define a           a0 
#define b           a1 
#define c           a2
#define length      a3
#define b_shr       s2
#define c_shr       s3
#define _32         s4
#define vec_tmp     s5
#define vec_c1      s6
#define tmpA        s7
#define tmpB        s8



FUNCTION_NAME:
    xm.entsp (NSTACKWORDS)*4
    xm.stdsp  s3,s2,8
    xm.stdsp  s5,s4,16
    xm.stdsp  s7,s6,24
    { li t3, 0                          ; sw s8, 4                          (sp) }
    { mv t3, length                     ; xm.vsetc t3                           }
    mv b_shr, a4
    { srli length, length, 2            ; nop                                   }
    mv c_shr, a5
    { xm.zexti t3, 2                    ; nop                                   }
    
{ xm.ldawsp vec_tmp, STACK_VEC_TMP*4    ; nop                                   }
{ xm.ldawsp vec_c1, STACK_VEC_C1*4      ; nop                                   }
    { xm.mkmski t3, 32                  ; sw t3, (STACK_TAIL_LEN)*4             (sp) }
    { li _32, 32                        ; xm.brff length, .L_loop_bot           }
    

    .L_loop_top:
    
            xm.vlashr c, c_shr 
            xm.vstrpv vec_tmp, t3
            xm.lddsp  tmpA,tmpB,((STACK_VEC_TMP/2) + 0)*8
            xm.stdsp  tmpA,tmpA,((STACK_VEC_C1/2)  + 0)*8
            xm.stdsp  tmpB,tmpB,((STACK_VEC_C1/2)  + 1)*8
            xm.lddsp  tmpA,tmpB,((STACK_VEC_TMP/2) + 1)*8
        { addi c, c, 8                  ; addi length, length, -1               }
            xm.stdsp  tmpA,tmpA,((STACK_VEC_C1/2) + 2)*8
            xm.stdsp  tmpB,tmpB,((STACK_VEC_C1/2) + 3)*8

            xm.vlashr b, b_shr
        { add b, b, _32                 ; xm.vlmul0 vec_c1                      }
        { add a, a, _32                 ; xm.vstr a                             }
        { addi c, c, 8                  ; xm.bt length, .L_loop_top             }

.L_loop_bot:

    { addi t3,sp, (STACK_VEC_C1)*4      ; lw length, (STACK_TAIL_LEN)*4          (sp) }
    { slli length, length, 3            ; xm.brff length, .L_done               }
    { xm.mkmsk length, length           ; xm.vclrdr                             }
    { nop                               ; xm.vstd vec_tmp                       }
        xm.vlashr c, c_shr
        xm.vstrpv vec_tmp, length
        xm.lddsp  tmpA,tmpB,((STACK_VEC_TMP / 2) + 0)*8
        xm.stdsp  tmpA,tmpA,((STACK_VEC_C1  / 2) + 0)*8
        xm.stdsp  tmpB,tmpB,((STACK_VEC_C1  / 2) + 1)*8
        xm.lddsp  tmpA,tmpB,((STACK_VEC_TMP / 2) + 1)*8
        xm.stdsp  tmpA,tmpA,((STACK_VEC_C1  / 2) + 2)*8
        xm.stdsp  tmpB,tmpB,((STACK_VEC_C1  / 2) + 3)*8
        xm.vlashr b, b_shr
    { nop                               ; xm.vlmul0 t3                          }
        xm.vstrpv a, length
        xm.vstrpv vec_tmp, length
    { nop                               ; xm.vldd vec_tmp                       }
    { nop                               ; xm.vstd vec_tmp                       }


.L_done:
        xm.lddsp  s3,s2,8
        xm.lddsp  s5,s4,16
        xm.lddsp  s7,s6,24

    { li a0, 31                         ; xm.vgetc t3                           }
    { xm.zexti t3, 5                    ; lw s8, 4                          (sp) }
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



