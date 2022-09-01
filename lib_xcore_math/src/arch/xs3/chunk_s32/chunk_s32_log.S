// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


#include "../asm_helper.h"

/*  

Condition:  0 < ldexp(b[k], -30) < 2


void chunk_s32_log(
    q8_24 a[],
    const int32_t b[],
    const exponent_t b_exp);
*/


#define NSTACKWORDS     (6+48)

#define FUNCTION_NAME   chunk_s32_log

#define SP_VEC_X1    ((NSTACKWORDS) - 8 )
#define SP_VEC_X2    ((NSTACKWORDS) - 16)
#define SP_VEC_X3    ((NSTACKWORDS) - 24)
#define SP_VEC_X4    ((NSTACKWORDS) - 32)
#define SP_VEC_X5    ((NSTACKWORDS) - 40)
#define SP_VEC_X6    ((NSTACKWORDS) - 48)


.text
.issue_mode dual
.align 4

.L_ps_coef1: .word -0x800000, -0x800000, -0x800000, -0x800000, -0x800000, -0x800000, -0x800000, -0x800000
.L_ps_coef2: .word  0x555555,  0x555555,  0x555555,  0x555555,  0x555555,  0x555555,  0x555555,  0x555555
.L_ps_coef3: .word -0x400000, -0x400000, -0x400000, -0x400000, -0x400000, -0x400000, -0x400000, -0x400000
.L_ps_coef4: .word  0x333333,  0x333333,  0x333333,  0x333333,  0x333333,  0x333333,  0x333333,  0x333333
.L_ps_coef5: .word -0x2aaaab, -0x2aaaab, -0x2aaaab, -0x2aaaab, -0x2aaaab, -0x2aaaab, -0x2aaaab, -0x2aaaab
 
.L_ln_2: .word 0x2c5c85fe,0x2c5c85fe,0x2c5c85fe,0x2c5c85fe,0x2c5c85fe,0x2c5c85fe,0x2c5c85fe,0x2c5c85fe

.cc_top FUNCTION_NAME.function,FUNCTION_NAME

#define a           r0 
#define b           r1
#define b_exp       r2
#define mantB       r3
#define tmpA        r4
#define tmpB        r5
#define tmpC        r6
#define vec_x       r7
#define mantA       r11

FUNCTION_NAME:
  dualentsp NSTACKWORDS
  std r4, r5, sp[0]
  std r6, r7, sp[1]

{ ldaw vec_x, sp[SP_VEC_X1]   ; ldc r11, 0                  }

  ldd mantB, mantA, b[0]
{ cls tmpA, mantA             ; cls tmpB, mantB             }
{ shl mantA, mantA, tmpA      ; shl mantB, mantB, tmpB      }
  std mantB, mantA, vec_x[0]
{ sub mantA, b_exp, tmpA      ; sub mantB, b_exp, tmpB      }
  std mantB, mantA, a[0]

  ldd mantB, mantA, b[1]
{ cls tmpA, mantA             ; cls tmpB, mantB             }
{ shl mantA, mantA, tmpA      ; shl mantB, mantB, tmpB      }
  std mantB, mantA, vec_x[1]
{ sub mantA, b_exp, tmpA      ; sub mantB, b_exp, tmpB      }
  std mantB, mantA, a[1]

  ldd mantB, mantA, b[2]
{ cls tmpA, mantA             ; cls tmpB, mantB             }
{ shl mantA, mantA, tmpA      ; shl mantB, mantB, tmpB      }
  std mantB, mantA, vec_x[2]
{ sub mantA, b_exp, tmpA      ; sub mantB, b_exp, tmpB      }
  std mantB, mantA, a[2]

  ldd mantB, mantA, b[3]
{ cls tmpA, mantA             ; cls tmpB, mantB             }
{ shl mantA, mantA, tmpA      ; shl mantB, mantB, tmpB      }
  std mantB, mantA, vec_x[3]
{ sub mantA, b_exp, tmpA      ; sub mantB, b_exp, tmpB      }
  std mantB, mantA, a[3]

{ ldc tmpA, 24                ; vsetc r11                   }

  ldaw r11, cp[vpu_vec_0x20000000]
{ neg tmpA, tmpA              ; vclrdr                      }
  vlashr a[0], tmpA
{ ldap r11, .L_ln_2           ; vladd r11[0]                }
{                             ; vlmul r11[0]                }

{ mov r11, vec_x              ; vstr a[0]                   }
{                             ; vldr r11[0]                 }
  ldaw r11, cp[vpu_vec_0x00000002]
{                             ; vlsat r11[0]                }
  ldaw r11, cp[vpu_vec_neg_0x40000000]
{ ldaw tmpB, sp[SP_VEC_X1]    ; vladd r11[0]                }

#undef mantA
#undef mantB

{ ldaw vec_x, sp[SP_VEC_X2]   ; vstr vec_x[0]               }
{                             ; vlmul tmpB[0]               } // (x-1.0)^2
{ ldaw vec_x, sp[SP_VEC_X3]   ; vstr vec_x[0]               }
{                             ; vlmul tmpB[0]               } // (x-1.0)^3
{ ldaw vec_x, sp[SP_VEC_X4]   ; vstr vec_x[0]               }
{                             ; vlmul tmpB[0]               } // (x-1.0)^4
{ ldaw vec_x, sp[SP_VEC_X5]   ; vstr vec_x[0]               }
{ ldc tmpA, 6                 ; vlmul tmpB[0]               } // (x-1.0)^5
{ ldaw vec_x, sp[SP_VEC_X6]   ; vstr vec_x[0]               }
{ ldap r11, .L_ps_coef5       ; vlmul tmpB[0]               } // (x-1.0)^6
{ ldaw tmpB, sp[SP_VEC_X1]    ; vstr vec_x[0]               }

  vlashr tmpB[0], tmpA                                         // vR[] = coef[0] * x
{ ldap r11, .L_ps_coef4       ; vldc r11[0]                 } // vC[] = coef[5]
{ ldaw vec_x, sp[SP_VEC_X5]   ; vlmacc vec_x[0]             } // vR[] += coef[5] * x^6
{ ldap r11, .L_ps_coef3       ; vldc r11[0]                 } // vC[] = coef[4]
{ ldaw vec_x, sp[SP_VEC_X4]   ; vlmacc vec_x[0]             } // vR[] += coef[4] * x^5
{ ldap r11, .L_ps_coef2       ; vldc r11[0]                 } // vC[] = coef[3]
{ ldaw vec_x, sp[SP_VEC_X3]   ; vlmacc vec_x[0]             } // vR[] += coef[3] * x^4
{ ldap r11, .L_ps_coef1       ; vldc r11[0]                 } // vC[] = coef[2]
{ ldaw vec_x, sp[SP_VEC_X2]   ; vlmacc vec_x[0]             } // vR[] += coef[2] * x^3
{                             ; vldc r11[0]                 } // vC[] = coef[1]
{ ldaw vec_x, sp[SP_VEC_X1]   ; vlmacc vec_x[0]             } // vR[] += coef[1] * x^2

{                             ; vladd a[0]                  }
{                             ; vstr a[0]                   }

// Any inputs that were 0 should become INT32_MIN
  ldaw r11, cp[vpu_vec_0x00000001]
{                             ; vldr r11[0]                 } 
{                             ; vlsub b[0]                  } 
{                             ; vdepth1                     } 
{                             ; vstr vec_x[0]               }
{                             ; ldw tmpA, vec_x[0]          }
{ mov tmpB, tmpA              ;                             }
  zip tmpB, tmpA, 0
  mov tmpB, tmpA
  zip tmpB, tmpA, 0
  ldaw r11, cp[vpu_vec_0x80000000]
{                             ; vldr r11[0]                 }
  vstrpv a[0], tmpA


.L_finish:
  ldd r4, r5, sp[0]
  ldd r6, r7, sp[1]
  retsp NSTACKWORDS


.L_func_end:
.cc_bottom FUNCTION_NAME.function


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS; .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;              .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;             .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;           .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME


#endif //defined(__XS3A__)



