// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"


/*  
Takes a normalized angle between 0.0 and 1.0 in Q31 format
and returns sin of that angle in Q30.

int32_t sbrad_sin(
    const sbrad_t theta);
*/

#define FUNCTION_NAME   sbrad_sin
#define NSTACKWORDS     (16)

#define VEC_R           (NSTACKWORDS - 8)

#define a         x10
#define r         x11
#define out_mul   x12
#define vec_r     x13
#define tmpA      x18
#define tmpB      x19
#define _31       x20

.text
.p2align 4

.L_vec_b:
  .word 0x6487ed51, -0x52aef399, 0x519af19d, -0x4cb4b33a
  .word 0x541e0d21, -0x78c1d3f8, 0x7a3d0d34, -0x5beb6e7d
.L_vec_s_hat:
  .word 1, 2, 5, 9, 14, 20, 26, 32
.L_weights:
  .word 0x40000000, 0x40000000, 0x40000000, 0x40000000
  .word 0x40000000, 0x40000000, 0x00000000, 0x00000000
  
FUNCTION_NAME:
  xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
  xm.stdsp  s3,s2,8
  xm.stdsp  s5,s4,16
{ li t3, 0                  ; addi vec_r,sp, (VEC_R)*4       }
{ xm.slt out_mul, a, t3         ; xm.vsetc t3}
{ li _31, 31                 ; xm.brff out_mul, .L_hgfd         }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
{ xm.neg a, a                    ; nop                             }
.L_hgfd:

  xm.lmul tmpA, tmpB, a, a, t3, t3
  xm.lextract r, tmpA, tmpB, _31, 32
  xm.lmul tmpA, tmpB, a, r, t3, t3
  xm.lextract tmpA, tmpA, tmpB, _31, 32
  xm.stdi  a,tmpA, 0 (vec_r)// a, a^3

#undef a  // no longer needed
#define tmpC     x10

  xm.lmul tmpA, tmpB, tmpA, r, t3, t3
  xm.lextract tmpB, tmpA, tmpB, _31, 32
  xm.lmul tmpA, tmpC, tmpB, r, t3, t3
  xm.lextract tmpA, tmpA, tmpC, _31, 32
  xm.stdi  tmpB,tmpA, 8 (vec_r)// a^5, a^7

  xm.lmul tmpA, tmpB, tmpA, r, t3, t3
  xm.lextract tmpB, tmpA, tmpB, _31, 32
  // stw tmpB, vec_r[4] // if we only wanted 5 terms
  xm.lmul tmpA, tmpC, tmpB, r, t3, t3
  xm.lextract tmpA, tmpA, tmpC, _31, 32
  xm.stdi  tmpB,tmpA, 16 (vec_r)// a^9, 1^11

  // lmul tmpA, tmpB, tmpA, r, x28, x28
  // lextract tmpB, tmpA, tmpB, _31, 32
  // lmul tmpA, tmpC, tmpB, r, x28, x28
  // lextract tmpA, tmpA, tmpC, _31, 32
  // std tmpA, tmpB, vec_r[3]

// Now that we've filled in vec_R[], we just need to do the VPU stuff.
{ xm.ldap t3, .L_vec_b          ; xm.vclrdr                      }
{ xm.ldap t3, .L_vec_s_hat      ; xm.vldc t3}
{ nop                             ; xm.vlmacc0 vec_r}
 xm.vlsat t3
{ mv t3, vec_r              ; nop}
{ nop                             ; xm.vstr t3}
{ xm.ldap t3, .L_weights        ; xm.vclrdr                      }
{ nop                             ; xm.vldc t3}
{ mv t3, vec_r              ; xm.vlmaccr0 vec_r}
{ nop                             ; xm.vstr t3}
{ nop                             ; lw a0,0            ( vec_r)}
  xm.lddsp  s3,s2,8
  xm.lddsp  s5,s4,16
{ nop                             ; xm.bt out_mul, .L_gpgp         }
{ nop                             ; xm.retsp (NSTACKWORDS)*4           }
.L_gpgp:
{ xm.neg a0, a0                  ; nop}
xm.retsp (NSTACKWORDS*4)     

.L_func_end:

.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords /* Translation error on this line: unexpected token at position 42. */ 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores /* Translation error on this line: unexpected token at position 29. */ 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers /* Translation error on this line: unexpected token at position 30. */ 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends /* Translation error on this line: unexpected token at position 32. */ 
.size FUNCTION_NAME,.L_func_end - FUNCTION_NAME













#endif //defined(__VX4B__)

