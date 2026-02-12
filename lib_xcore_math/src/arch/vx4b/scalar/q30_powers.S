// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

.text
.p2align 2

/*  
void q30_powers(
    q2_30 a[],
    const q2_30 b,
    const unsigned length);
*/
#define FUNCTION_NAME   q30_powers
#define NSTACKWORDS     (8+(8*2))

#define ST_VEC_TMP2     (NSTACKWORDS-8-2)
#define ST_VEC_TMP      (NSTACKWORDS-16-2)

#define a             a0
#define b             a1
#define len           a2
#define tmpA          a3
#define tmpB          s2
#define pow           s3
#define _30           s4
#define vec_tmp       s5


FUNCTION_NAME:
  xm.entsp (NSTACKWORDS)*4
  xm.stdsp  s3,s2,8
  xm.stdsp  s5,s4,0
  xm.stdsp  s7,s6,16 

{ addi vec_tmp,sp, (ST_VEC_TMP)*4       ; li t3, 0                              }
{ li t3, 0                              ; nop                                   }
{ li _30, 30                            ; xm.vsetc t3                           }
  lw pow, vpu_vec_0x40000000
  xm.stdi  pow,b, 0  (vec_tmp)// b^0, b^1

{ li tmpA, 0                            ; li tmpB, 0                            }
  xm.maccs tmpA, tmpB, b, b
  xm.lextract pow, tmpA, tmpB, _30, 32  

{ li tmpA, 0                            ; li tmpB, 0                            }
  xm.maccs tmpA, tmpB, b, pow
  xm.lextract t3, tmpA, tmpB, _30, 32
  xm.stdi  pow,t3, 8 (vec_tmp)// b^2, b^3
  
{ li tmpA, 0                            ; li tmpB, 0                            }
  xm.maccs tmpA, tmpB, pow, pow
  xm.lextract pow, tmpA, tmpB, _30, 32

  xm.stdi  pow,pow, 32 (vec_tmp)// eight b^4's
  xm.stdi  pow,pow, 40(vec_tmp)
  xm.stdi  pow,pow, 48(vec_tmp)
  xm.stdi  pow,pow, 56(vec_tmp)

{ li tmpB, 32                           ; xm.vclrdr                             }
{ addi t3,sp, (ST_VEC_TMP2)*4           ; xm.vladd vec_tmp                      }
{ xm.mkmski tmpA, 16                    ; xm.vlmul0 t3                          }

  addi t3, vec_tmp, 16
  xm.vstrpv t3, tmpA

{ srli tmpA, len, 3                     ; addi t3,sp, (ST_VEC_TMP)*4            }
{ xm.zexti len, 3                       ; xm.vldr t3                            }
{ addi t3,sp, (ST_VEC_TMP2)*4           ; nop                                   }
{ slli len, len, 2                      ; xm.brff tmpA, .L_loop_bot             }

  .L_loop_top:
  { addi tmpA, tmpA, -1                 ; xm.vstr a                             }
  { add a, a, tmpB                      ; xm.vlmul0 t3                          }
  { nop                                 ; xm.vlmul0 t3                          }
  { nop                                 ; xm.bt tmpA, .L_loop_top               }
  .L_loop_bot:

{ xm.mkmsk len, len                     ; xm.brff len, .L_finish                }
  xm.vstrpv a, len
  
.L_finish:
  xm.lddsp  s3,s2,8
  xm.lddsp  s5,s4,0
  xm.lddsp  s7,s6,16 
  xm.retsp (NSTACKWORDS)*4

.L_func_end_unpack:

.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME,.L_func_end_unpack - FUNCTION_NAME

#undef NSTACKWORDS
#undef FUNCTION_NAME



#endif //defined(__VX4B__)



