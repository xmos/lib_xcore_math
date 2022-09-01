// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


#include "../asm_helper.h"

.text
.align 4;
.issue_mode dual

/*  
float f32_log2(
    const float x);
*/
#define FUNCTION_NAME   f32_log2
#define NSTACKWORDS     (4)

#define x     r0
#define tmp   r1
#define exp   r2
#define _0    r3

.cc_top FUNCTION_NAME.function,FUNCTION_NAME
FUNCTION_NAME:
  dualentsp NSTACKWORDS

{ mov r1, r0                  ; ldaw r0, sp[1]              }
  ldap r11, f32_normA
  bla r11

{ ldc tmp, 1                  ; ldc r11, 23                 }
{ ldc _0, 0                   ; ldw exp, sp[1]              }
  fmake tmp, _0, r11, _0, tmp
  fsub x, x, tmp
  fmake exp, _0, r11, _0, exp
  ldaw r11, cp[log2_ps]       
{                             ; stw exp, sp[1]              }
{ mov r1, r11                 ; ldc r2, 11                  }
  ldap r11, f32_power_series
  bla r11
{                             ; ldw exp, sp[1]              }
  fadd x, x, exp
  retsp NSTACKWORDS

.L_func_end_unpack:
.cc_bottom FUNCTION_NAME.function

.global FUNCTION_NAME
.type FUNCTION_NAME,@function


.weak FUNCTION_NAME.callees
.add_to_set FUNCTION_NAME.callees,f32_normA.nstackwords
.add_to_set FUNCTION_NAME.callees,f32_power_series.nstackwords
.max_reduce FUNCTION_NAME.callee_maxstackwords,FUNCTION_NAME.callees,0
.set FUNCTION_NAME.nstackwords,NSTACKWORDS+FUNCTION_NAME.callee_maxstackwords
.global FUNCTION_NAME.nstackwords

.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME,.L_func_end_unpack - FUNCTION_NAME

#undef NSTACKWORDS
#undef FUNCTION_NAME



#endif //defined(__XS3A__)



