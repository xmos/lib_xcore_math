// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

.text
.p2align 2

/*  
float f32_sin(
    const float theta);
*/

#define FUNCTION_NAME   f32_sin
#define NSTACKWORDS     (8)



#define r           x10
#define phi         x11
#define out_mul     x12
#define tmp         x13

#define _0          x23
#define _1          x24



FUNCTION_NAME:
  xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
  xm.stdsp  s3,s2,8
  xm.stdsp  s5,s4,16
  xm.stdsp  s7,s6,24
{ li _0, 0                   ; sw s8, 4              (sp)}
  xm.flt t3, r, _0
{ nop                             ; li _1, 1                   }
{ li out_mul, 23             ; xm.bt t3, .L_neg              }

// sin(-x) = -sin(x) -- sin() has odd symmetry, so let's only deal with positive angles
.L_pos:
  xm.fmake out_mul, _0, out_mul, _0, _1 // +1.0f
  tail .L_qwer
.L_neg:
  xm.fmake out_mul, _1, out_mul, _0, _1 // -1.0f
  xm.fmul r, r, out_mul

.L_qwer:

// Normalize our angle to be 0 <= r < 4.0f
// because  sin(x) = sin(x + k*2*pi) for int k
  lw t3, two_over_pi
  xm.fmul r, r, t3

{ li t3, 21                 ; nop                             }
  xm.fmake tmp, _0, t3, _0, _1 // +0.25f
  xm.fmul tmp, r, tmp
  xm.fsexp s6, t3, tmp
  xm.fmant tmp, tmp
{ xm.neg s6, t3                 ; li s5, 23                  }
{ add s6, s6, s5              ; addi t3, t3, 2             }
  xm.shr tmp, tmp, s6
  xm.shl tmp, tmp, s6
  xm.fmake tmp, _0, t3, _0, tmp  // tmp <--  4.0*floor(r/4.0)
  
  xm.fsub r, r, tmp  // r <-- r - 4.0*floor(r/4.0)

// sin(pi + x) = -sin(x)  if   pi < x <= 2*pi
{ li t3, 24                 ; nop                             }
  xm.fmake tmp, _0, t3, _0, _1 // +2.0f
  xm.flt t3, r, tmp
{ nop                             ; xm.bt t3, .L_wert             }
  xm.fsub r, r, tmp  // r <-- r - 2
  xm.fsub out_mul, _0, out_mul   // out_mul <-- -out_mul
.L_wert:

// sin(pi/2 + x) = sin(pi - x)
  xm.fmul t3, out_mul, out_mul // x28 <-- out_mul^2 = 1.0 
  xm.flt t3, r, t3     // x28 <-- r < 1.0
{ nop                             ; xm.bt t3, .L_erty             }
  xm.fsub r, tmp, r  // r <-- 2.0 - r
.L_erty:


// Now, we have an angle r in the first quadrant
// r is a normalized angle where   0.0 <= r < 1.0

// Now apply power series for sin()

#define total         x18
#define coefs         x19

lui t3, %hi(sin_coef)
  addi t3,t3, %lo(sin_coef)
{ mv coefs, t3            ; lw tmp,0               ( t3)}
  xm.fmul phi, r, r
  xm.fmul total, r, tmp   


#define PS_TERM(N)            \
  xm.fmul r, r, phi;              \
  xm.ldw tmp, (N*4)(coefs);           \
  xm.fmacc total, total, r, tmp;

PS_TERM(1)
PS_TERM(2)
PS_TERM(3)
PS_TERM(4)
PS_TERM(5)
PS_TERM(6)
PS_TERM(7)

// Apply final output multiplier
  xm.fmul a0, total, out_mul

  xm.lddsp  s7,s6,24
  xm.lddsp  s5,s4,16
  xm.lddsp  s3,s2,8
{ nop                             ; lw s8, 4              (sp)}
{ nop                             ; xm.retsp (NSTACKWORDS)*4           }

.L_func_end:

.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords /* Translation error on this line: unexpected token at position 42. */ 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores /* Translation error on this line: unexpected token at position 29. */ 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers /* Translation error on this line: unexpected token at position 30. */ 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends /* Translation error on this line: unexpected token at position 32. */ 
.size FUNCTION_NAME,.L_func_end - FUNCTION_NAME













#endif //defined(__VX4B__)

