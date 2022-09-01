// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


#include "../asm_helper.h"

.text
.align 4;
.issue_mode dual

/*  
void q30_powers(
    q2_30 a[],
    const q2_30 b,
    const unsigned length);
*/
#define FUNCTION_NAME   q30_powers
#define NSTACKWORDS     (6+(8*2))

#define ST_VEC_TMP2     (NSTACKWORDS-8)
#define ST_VEC_TMP      (NSTACKWORDS-16)

#define a             r0
#define b             r1
#define len           r2
#define tmpA          r3
#define tmpB          r4
#define pow           r5
#define _30           r6
#define vec_tmp       r7


.cc_top FUNCTION_NAME.function,FUNCTION_NAME
FUNCTION_NAME:
  dualentsp NSTACKWORDS
  std r4, r5, sp[1]
  std r6, r7, sp[2]

{ ldaw vec_tmp, sp[ST_VEC_TMP];  ldc r11, 0                 }
{ ldc r11, 0                  ;                             }
{ ldc _30, 30                 ; vsetc r11                   }
  ldw pow, cp[vpu_vec_0x40000000]
  std b, pow, vec_tmp[0]  // b^0, b^1

{ ldc tmpA, 0                 ; ldc tmpB, 0                 }
  maccs tmpA, tmpB, b, b
  lextract pow, tmpA, tmpB, _30, 32  

{ ldc tmpA, 0                 ; ldc tmpB, 0                 }
  maccs tmpA, tmpB, b, pow
  lextract r11, tmpA, tmpB, _30, 32
  std r11, pow, vec_tmp[1] // b^2, b^3
  
{ ldc tmpA, 0                 ; ldc tmpB, 0                 }
  maccs tmpA, tmpB, pow, pow
  lextract pow, tmpA, tmpB, _30, 32

  std pow, pow, vec_tmp[4] // eight b^4's
  std pow, pow, vec_tmp[5]
  std pow, pow, vec_tmp[6]
  std pow, pow, vec_tmp[7]

{ ldc tmpB, 32                ; vclrdr                      }
{ ldaw r11, sp[ST_VEC_TMP2]   ; vladd vec_tmp[0]            }
{ mkmsk tmpA, 16              ; vlmul r11[0]                }
  ldaw r11, vec_tmp[4]
  vstrpv r11[0], tmpA

{ shr tmpA, len, 3            ; ldaw r11, sp[ST_VEC_TMP]    }
{ zext len, 3                 ; vldr r11[0]                 }
{ ldaw r11, sp[ST_VEC_TMP2]   ;                             }
{ shl len, len, 2             ; bf tmpA, .L_loop_bot        }

  .L_loop_top:
  { sub tmpA, tmpA, 1           ; vstr a[0]                   }
  { add a, a, tmpB              ; vlmul r11[0]                }
  {                             ; vlmul r11[0]                }
  {                             ; bt tmpA, .L_loop_top        }
  .L_loop_bot:

{ mkmsk len, len              ; bf len, .L_finish           }
  vstrpv a[0], len
  
.L_finish:
  ldd r4, r5, sp[1]
  ldd r6, r7, sp[2]
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



