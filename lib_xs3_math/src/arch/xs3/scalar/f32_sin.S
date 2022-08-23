// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


#include "../asm_helper.h"

.text
.issue_mode dual
.align 4

/*  
float f32_sin(
    const float theta);
*/

#define FUNCTION_NAME   f32_sin
#define NSTACKWORDS     (8)



#define r           r0
#define phi         r1
#define out_mul     r2
#define tmp         r3

#define _0          r9
#define _1          r10


.cc_top FUNCTION_NAME.function,FUNCTION_NAME

FUNCTION_NAME:
  dualentsp NSTACKWORDS
  std r4, r5, sp[1]
  std r6, r7, sp[2]
  std r8, r9, sp[3]
{ ldc _0, 0                   ; stw r10, sp[1]              }
  flt r11, r, _0
{                             ; ldc _1, 1                   }
{ ldc out_mul, 23             ; bt r11, .L_neg              }

// sin(-x) = -sin(x) -- sin() has odd symmetry, so let's only deal with positive angles
.L_pos:
  fmake out_mul, _0, out_mul, _0, _1 // +1.0f
  bu .L_qwer
.L_neg:
  fmake out_mul, _1, out_mul, _0, _1 // -1.0f
  fmul r, r, out_mul

.L_qwer:

// Normalize our angle to be 0 <= r < 4.0f
// because  sin(x) = sin(x + k*2*pi) for int k
  ldw r11, cp[two_over_pi]
  fmul r, r, r11

{ ldc r11, 21                 ;                             }
  fmake tmp, _0, r11, _0, _1 // +0.25f
  fmul tmp, r, tmp
  fsexp r8, r11, tmp
  fmant tmp, tmp
{ neg r8, r11                 ; ldc r7, 23                  }
{ add r8, r8, r7              ; add r11, r11, 2             }
  shr tmp, tmp, r8
  shl tmp, tmp, r8
  fmake tmp, _0, r11, _0, tmp  // tmp <--  4.0*floor(r/4.0)
  
  fsub r, r, tmp  // r <-- r - 4.0*floor(r/4.0)

// sin(pi + x) = -sin(x)  if   pi < x <= 2*pi
{ ldc r11, 24                 ;                             }
  fmake tmp, _0, r11, _0, _1 // +2.0f
  flt r11, r, tmp
{                             ; bt r11, .L_wert             }
  fsub r, r, tmp  // r <-- r - 2
  fsub out_mul, _0, out_mul   // out_mul <-- -out_mul
.L_wert:

// sin(pi/2 + x) = sin(pi - x)
  fmul r11, out_mul, out_mul // r11 <-- out_mul^2 = 1.0 
  flt r11, r, r11     // r11 <-- r < 1.0
{                             ; bt r11, .L_erty             }
  fsub r, tmp, r  // r <-- 2.0 - r
.L_erty:


// Now, we have an angle r in the first quadrant
// r is a normalized angle where   0.0 <= r < 1.0

// Now apply power series for sin()

#define total         r4
#define coefs         r5

  ldaw r11, cp[sin_coef]
{ mov coefs, r11            ; ldw tmp, r11[0]               }
  fmul phi, r, r
  fmul total, r, tmp   


#define PS_TERM(N)            \
  fmul r, r, phi;              \
  ldw tmp, coefs[N];           \
  fmacc total, total, r, tmp;

PS_TERM(1)
PS_TERM(2)
PS_TERM(3)
PS_TERM(4)
PS_TERM(5)
PS_TERM(6)
PS_TERM(7)

// Apply final output multiplier
  fmul r0, total, out_mul

  ldd r8, r9, sp[3]
  ldd r6, r7, sp[2]
  ldd r4, r5, sp[1]
{                             ; ldw r10, sp[1]              }
{                             ; retsp NSTACKWORDS           }

.L_func_end:
.cc_bottom FUNCTION_NAME.function

.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME,.L_func_end - FUNCTION_NAME













#endif //defined(__XS3A__)

