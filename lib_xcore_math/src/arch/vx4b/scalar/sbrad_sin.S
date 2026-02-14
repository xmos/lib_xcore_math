// Copyright 2020-2026 XMOS LIMITED.
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

#define VEC_R           (NSTACKWORDS - 12)

#define a         a0
#define r         a1
#define out_mul   a2
#define vec_r     a3
#define tmpA      s2
#define tmpB      s3
#define _31       s4

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
  xm.entsp (NSTACKWORDS)*4
  xm.stdsp  s3,s2,8
  xm.stdsp  s5,s4,0
{ li t3, 0                              ; addi vec_r,sp, (VEC_R)*4              }
{ xm.slt out_mul, a, t3                 ; xm.vsetc t3                           }
{ li _31, 31                            ; xm.brff out_mul, .L_hgfd              }
{ xm.neg a, a                           ; nop                                   }
.L_hgfd:

  xm.lmul tmpA, tmpB, a, a, t3, t3
  xm.lextract r, tmpA, tmpB, _31, 32
  xm.lmul tmpA, tmpB, a, r, t3, t3
  xm.lextract tmpA, tmpA, tmpB, _31, 32
  xm.stdi  a,tmpA, 0 (vec_r)// a, a^3

#undef a  // no longer needed
#define tmpC     a0

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

// Now that we've filled in vec_R[], we just need to do the VPU stuff.
{ xm.ldap t3, .L_vec_b                  ; xm.vclrdr                             }
{ xm.ldap t3, .L_vec_s_hat              ; xm.vldc t3                            }
{ nop                                   ; xm.vlmacc0 vec_r                      }
 xm.vlsat t3
{ mv t3, vec_r                          ; nop                                   }
{ nop                                   ; xm.vstr t3                            }
{ xm.ldap t3, .L_weights                ; xm.vclrdr                             }
{ nop                                   ; xm.vldc t3                            }
{ mv t3, vec_r                          ; xm.vlmaccr0 vec_r                     }
{ nop                                   ; xm.vstr t3                            }
{ nop                                   ; lw a0,0            ( vec_r)           }
  xm.lddsp  s3,s2,8
  xm.lddsp  s5,s4,0
{ nop                                   ; xm.bt out_mul, .L_gpgp                }
{ nop                                   ; xm.retsp (NSTACKWORDS)*4              }
.L_gpgp:
{ xm.neg a0, a0                         ; nop                                   }
{ xm.retsp (NSTACKWORDS*4)              ; nop                                   }   

.L_func_end:

.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
.size FUNCTION_NAME,.L_func_end - FUNCTION_NAME













#endif //defined(__VX4B__)

