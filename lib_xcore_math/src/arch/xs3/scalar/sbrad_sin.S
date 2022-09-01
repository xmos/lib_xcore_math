// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


#include "../asm_helper.h"


/*  
Takes a normalized angle between 0.0 and 1.0 in Q31 format
and returns sin of that angle in Q30.

int32_t sbrad_sin(
    const sbrad_t theta);
*/

#define FUNCTION_NAME   sbrad_sin
#define NSTACKWORDS     (14)

#define VEC_R           (NSTACKWORDS - 8)

#define a         r0
#define r         r1
#define out_mul   r2
#define vec_r     r3
#define tmpA      r4
#define tmpB      r5
#define _31       r6

.text
.issue_mode dual
.align 16

.L_vec_b:
  .word 0x6487ed51, -0x52aef399, 0x519af19d, -0x4cb4b33a
  .word 0x541e0d21, -0x78c1d3f8, 0x7a3d0d34, -0x5beb6e7d
.L_vec_s_hat:
  .word 1, 2, 5, 9, 14, 20, 26, 32
.L_weights:
  .word 0x40000000, 0x40000000, 0x40000000, 0x40000000
  .word 0x40000000, 0x40000000, 0x00000000, 0x00000000
  
.cc_top FUNCTION_NAME.function,FUNCTION_NAME
FUNCTION_NAME:
  dualentsp NSTACKWORDS
  std r4, r5, sp[1]
  std r6, r7, sp[2]
{ ldc r11, 0                  ; ldaw vec_r, sp[VEC_R]       }
{ lss out_mul, a, r11         ; vsetc r11                   }
{ ldc _31, 31                 ; bf out_mul, .L_hgfd         }
{ neg a, a                    ;                             }
.L_hgfd:

  lmul tmpA, tmpB, a, a, r11, r11
  lextract r, tmpA, tmpB, _31, 32
  lmul tmpA, tmpB, a, r, r11, r11
  lextract tmpA, tmpA, tmpB, _31, 32
  std tmpA, a, vec_r[0] // a, a^3

#undef a  // no longer needed
#define tmpC     r0

  lmul tmpA, tmpB, tmpA, r, r11, r11
  lextract tmpB, tmpA, tmpB, _31, 32
  lmul tmpA, tmpC, tmpB, r, r11, r11
  lextract tmpA, tmpA, tmpC, _31, 32
  std tmpA, tmpB, vec_r[1] // a^5, a^7

  lmul tmpA, tmpB, tmpA, r, r11, r11
  lextract tmpB, tmpA, tmpB, _31, 32
  // stw tmpB, vec_r[4] // if we only wanted 5 terms
  lmul tmpA, tmpC, tmpB, r, r11, r11
  lextract tmpA, tmpA, tmpC, _31, 32
  std tmpA, tmpB, vec_r[2] // a^9, 1^11

  // lmul tmpA, tmpB, tmpA, r, r11, r11
  // lextract tmpB, tmpA, tmpB, _31, 32
  // lmul tmpA, tmpC, tmpB, r, r11, r11
  // lextract tmpA, tmpA, tmpC, _31, 32
  // std tmpA, tmpB, vec_r[3]

// Now that we've filled in vec_R[], we just need to do the VPU stuff.
{ ldap r11, .L_vec_b          ; vclrdr                      }
{ ldap r11, .L_vec_s_hat      ; vldc r11[0]                 }
{                             ; vlmacc vec_r[0]             }
{ mov r11, vec_r              ; vlsat r11[0]                }
{                             ; vstr r11[0]                 }
{ ldap r11, .L_weights        ; vclrdr                      }
{                             ; vldc r11[0]                 }
{ mov r11, vec_r              ; vlmaccr vec_r[0]            }
{                             ; vstr r11[0]                 }
{                             ; ldw r0, vec_r[0]            }
  ldd r4, r5, sp[1]
  ldd r6, r7, sp[2]
{                             ; bt out_mul, .L_gpgp         }
{                             ; retsp NSTACKWORDS           }
.L_gpgp:
{ neg r0, r0                  ; retsp NSTACKWORDS           }

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

