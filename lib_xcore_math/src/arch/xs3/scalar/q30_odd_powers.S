// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


#include "../asm_helper.h"

.text
.align 4;
.issue_mode dual

/*  
void s32_odd_powers(
    int32_t a[],
    const int32_t b,
    const unsigned count,
    const right_shift_t shr);
*/
#define FUNCTION_NAME   s32_odd_powers
#define NSTACKWORDS     (4)

#define a         r0
#define b         r1
#define len       r2
#define shr       r3

#define acc_lo    r4
#define acc_hi    r5

#define b_sqr     r11



.cc_top FUNCTION_NAME.function,FUNCTION_NAME
FUNCTION_NAME:

  dualentsp NSTACKWORDS
  std r4, r5, sp[1]

{                             ; sub len, len, 1             }
{ ldc acc_lo, 0               ; ldc acc_hi, 0               }
  maccs acc_hi, acc_lo, b, b
  lextract b_sqr, acc_hi, acc_lo, shr, 32

.L_loop_top:
  { sub len, len, 1             ; stw b, a[0]                 }
  { ldc acc_lo, 0               ; ldc acc_hi, 0               }
    maccs acc_hi, acc_lo, b, b_sqr
    lextract b, acc_hi, acc_lo, shr, 32
  { add a, a, 4                 ; bt len, .L_loop_top         }

  stw b, a[0]

  ldd r4, r5, sp[1]
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



