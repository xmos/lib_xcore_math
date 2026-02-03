// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

.text
.align 4;

/*  
q2_30 q30_exp_small(
    const q2_30 x);
*/
#define FUNCTION_NAME   q30_exp_small
#define NSTACKWORDS     (8)

#define x       a0
#define coef    a1
#define acc_hi  a2
#define acc_lo  a3
#define tmp     s2
#define pow     s3
#define _30     s4
#define _0      s5
#define sign    s8  // 1 means positive, 0 negative

.L_ps_pos_q30: 
  .word  0x40000000,  0x20000000,  0x0AAAAAAB, 0x02AAAAAB,  0x00888889, 0x0016C16C,  0x00034034, 0x00006807,  0x00000B8F
.L_ps_neg_q30: 
  .word -0x40000000,  0x20000000, -0x0AAAAAAB, 0x02AAAAAB, -0x00888889, 0x0016C16C, -0x00034034, 0x00006807, -0x00000B8F

FUNCTION_NAME:
  xm.entsp (NSTACKWORDS)*4
  xm.stdsp  s3,s2,0
  xm.stdsp  s5,s4,8

{ xm.ldap t3, .L_ps_pos_q30             ; sw s8, 16              (sp)           }
{ mv coef, t3                           ; li _30, 30                            }
{ li _0, 0                              ; li acc_hi, 0                          }
{ xm.slt sign, _0, x                    ; nop                                   } 
{ li acc_lo, 0                          ; xm.bt sign, .L_pos_x                  }

.L_neg_x:
la t3, .L_ps_neg_q30
{ xm.neg x, x                           ; mv coef, t3                           }
.L_pos_x:

{ nop                                   ; lw tmp,0            ( coef)           }

  xm.maccs acc_hi, acc_lo, tmp, tmp
  xm.maccs acc_hi, acc_lo, tmp, x
  xm.lmul t3, pow, x, x, _0, _0
  xm.lextract pow, t3, pow, _30, 32
{ nop                                   ; lw tmp,4            ( coef)           }
  xm.maccs acc_hi, acc_lo, tmp, pow 
  xm.lmul t3, pow, pow, x, _0, _0
  xm.lextract pow, t3, pow, _30, 32
{ nop                                   ; lw tmp,8            ( coef)           }
  xm.maccs acc_hi, acc_lo, tmp, pow 
  xm.lmul t3, pow, pow, x, _0, _0
  xm.lextract pow, t3, pow, _30, 32
{ nop                                   ; lw tmp,12            ( coef)          }
  xm.maccs acc_hi, acc_lo, tmp, pow 
  xm.lmul t3, pow, pow, x, _0, _0
  xm.lextract pow, t3, pow, _30, 32
{ nop                                   ; lw tmp,16            ( coef)          }
  xm.maccs acc_hi, acc_lo, tmp, pow 
  xm.lmul t3, pow, pow, x, _0, _0
  xm.lextract pow, t3, pow, _30, 32
{ nop                                   ; lw tmp,20            ( coef)          }
  xm.maccs acc_hi, acc_lo, tmp, pow 
  xm.lmul t3, pow, pow, x, _0, _0
  xm.lextract pow, t3, pow, _30, 32
{ nop                                   ; lw tmp,24            ( coef)          }
  xm.maccs acc_hi, acc_lo, tmp, pow 
  xm.lmul t3, pow, pow, x, _0, _0
  xm.lextract pow, t3, pow, _30, 32
{ nop                                   ; lw tmp,28            ( coef)          }
  xm.maccs acc_hi, acc_lo, tmp, pow 
  xm.lmul t3, pow, pow, x, _0, _0
  xm.lextract pow, t3, pow, _30, 32
{ nop                                   ; lw tmp,32            ( coef)          }
  xm.maccs acc_hi, acc_lo, tmp, pow 
  
  // result
  xm.lextract a0, acc_hi, acc_lo, _30, 32

.L_finish:
  lw s8, 16(sp)
  xm.lddsp  s3,s2,0
  xm.lddsp  s5,s4,8
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



