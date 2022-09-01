// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


#include "../asm_helper.h"

.text
.align 4;
.issue_mode dual

/*  
q2_30 q30_exp_small(
    const q2_30 x);
*/
#define FUNCTION_NAME   q30_exp_small
#define NSTACKWORDS     (6)

#define x       r0
#define coef    r1
#define acc_hi  r2
#define acc_lo  r3
#define tmp     r4
#define pow     r5
#define _30     r6
#define _0      r7
#define sign    r10  // 1 means positive, 0 negative

.L_ps_pos_q30: 
  .word  0x40000000,  0x20000000,  0x0AAAAAAB, 0x02AAAAAB,  0x00888889, 0x0016C16C,  0x00034034, 0x00006807,  0x00000B8F
.L_ps_neg_q30: 
  .word -0x40000000,  0x20000000, -0x0AAAAAAB, 0x02AAAAAB, -0x00888889, 0x0016C16C, -0x00034034, 0x00006807, -0x00000B8F

.cc_top FUNCTION_NAME.function,FUNCTION_NAME
FUNCTION_NAME:
  dualentsp NSTACKWORDS
  std r4, r5, sp[0]
  std r6, r7, sp[1]

{ ldap r11, .L_ps_pos_q30     ; stw r10, sp[4]              }
{ mov coef, r11               ; ldc _30, 30                 }
{ ldc _0, 0                   ; ldc acc_hi, 0               }
{ lss sign, _0, x             ;                             } 
{ ldc acc_lo, 0               ; bt sign, .L_pos_x           }

.L_neg_x:
  ldap r11, .L_ps_neg_q30
{ neg x, x                    ; mov coef, r11               }
.L_pos_x:

{                             ; ldw tmp, coef[0]            }

  maccs acc_hi, acc_lo, tmp, tmp
  maccs acc_hi, acc_lo, tmp, x
  lmul r11, pow, x, x, _0, _0
  lextract pow, r11, pow, _30, 32
{                             ; ldw tmp, coef[1]            }
  maccs acc_hi, acc_lo, tmp, pow 
  lmul r11, pow, pow, x, _0, _0
  lextract pow, r11, pow, _30, 32
{                             ; ldw tmp, coef[2]            }
  maccs acc_hi, acc_lo, tmp, pow 
  lmul r11, pow, pow, x, _0, _0
  lextract pow, r11, pow, _30, 32
{                             ; ldw tmp, coef[3]            }
  maccs acc_hi, acc_lo, tmp, pow 
  lmul r11, pow, pow, x, _0, _0
  lextract pow, r11, pow, _30, 32
{                             ; ldw tmp, coef[4]            }
  maccs acc_hi, acc_lo, tmp, pow 
  lmul r11, pow, pow, x, _0, _0
  lextract pow, r11, pow, _30, 32
{                             ; ldw tmp, coef[5]            }
  maccs acc_hi, acc_lo, tmp, pow 
  lmul r11, pow, pow, x, _0, _0
  lextract pow, r11, pow, _30, 32
{                             ; ldw tmp, coef[6]            }
  maccs acc_hi, acc_lo, tmp, pow 
  lmul r11, pow, pow, x, _0, _0
  lextract pow, r11, pow, _30, 32
{                             ; ldw tmp, coef[7]            }
  maccs acc_hi, acc_lo, tmp, pow 
  lmul r11, pow, pow, x, _0, _0
  lextract pow, r11, pow, _30, 32
{                             ; ldw tmp, coef[8]            }
  maccs acc_hi, acc_lo, tmp, pow 
  
  // result
  lextract r0, acc_hi, acc_lo, _30, 32

.L_finish:
  ldw r10, sp[4]
  ldd r4, r5, sp[0]
  ldd r6, r7, sp[1]
  retsp NSTACKWORDS

.L_func_end_unpack:
.cc_bottom FUNCTION_NAME.function

.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME,.L_func_end_unpack - FUNCTION_NAME

#undef NSTACKWORDS
#undef FUNCTION_NAME



#endif //defined(__XS3A__)



