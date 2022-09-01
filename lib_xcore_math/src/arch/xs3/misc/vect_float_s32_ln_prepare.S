// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


#include "../asm_helper.h"

/*  

void vect_float_s32_ln_prepare(
    q2_30 a[8],
    q8_24 exp_mod[8],
    const float_s32_t b[]);
*/


#define NSTACKWORDS     (8)

#define FUNCTION_NAME   vect_float_s32_ln_prepare


#define a           r0 
#define exp_mod     r1 
#define b           r2
#define mantA       r3
#define expA        r4
#define mantB       r5
#define expB        r6
#define tmpA        r7
#define tmpB        r8
#define tmpC        r9

.text
.issue_mode dual
.align 4


.cc_top FUNCTION_NAME.function,FUNCTION_NAME

.L_32_Q24:
.word 0x20000000,0x20000000,0x20000000,0x20000000,0x20000000,0x20000000,0x20000000,0x20000000

FUNCTION_NAME:
  dualentsp NSTACKWORDS
  std r4, r5, sp[0]
  std r6, r7, sp[1]
  std r8, r9, sp[2]

{ ldc r11, 0                  ; ldc tmpC, 24                }
{ ldc r11, 1                  ; vsetc r11                   }

  ldd expA, mantA, b[0]
  ldd expB, mantB, b[1]
{ cls tmpA, mantA             ; cls tmpB, mantB             }
{ sub expA, expA, tmpA        ; sub expB, expB, tmpB        }
{ sub tmpA, tmpA, 1           ; sub tmpB, tmpB, 1           }
{ shl mantA, mantA, tmpA      ; shl mantB, mantB, tmpB      }
  std expB, expA, exp_mod[0]
  std mantB, mantA, a[0]

  ldd expA, mantA, b[2]
  ldd expB, mantB, b[3]
{ cls tmpA, mantA             ; cls tmpB, mantB             }
{ sub expA, expA, tmpA        ; sub expB, expB, tmpB        }
{ sub tmpA, tmpA, 1           ; sub tmpB, tmpB, 1           }
{ shl mantA, mantA, tmpA      ; shl mantB, mantB, tmpB      }
  std expB, expA, exp_mod[1]
  std mantB, mantA, a[1]

  ldd expA, mantA, b[4]
  ldd expB, mantB, b[5]
{ cls tmpA, mantA             ; cls tmpB, mantB             }
{ sub expA, expA, tmpA        ; sub expB, expB, tmpB        }
{ sub tmpA, tmpA, 1           ; sub tmpB, tmpB, 1           }
{ shl mantA, mantA, tmpA      ; shl mantB, mantB, tmpB      }
  std expB, expA, exp_mod[2]
  std mantB, mantA, a[2]

  ldd expA, mantA, b[6]
  ldd expB, mantB, b[7]
{ cls tmpA, mantA             ; cls tmpB, mantB             }
{ sub expA, expA, tmpA        ; sub expB, expB, tmpB        }
{ sub tmpA, tmpA, 1           ; sub tmpB, tmpB, 1           }
{ shl mantA, mantA, tmpA      ; shl mantB, mantB, tmpB      }
  std expB, expA, exp_mod[3]
  std mantB, mantA, a[3]

  vlashr a[0], r11
{ neg tmpC, tmpC              ; vstr a[0]                   }

  ldap r11, .L_32_Q24
  vlashr exp_mod[0], tmpC
{                             ; vladd r11[0]                }
{                             ; vstr exp_mod[0]             }


.L_finish:
  ldd r4, r5, sp[0]
  ldd r6, r7, sp[1]
  ldd r8, r9, sp[2]
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



