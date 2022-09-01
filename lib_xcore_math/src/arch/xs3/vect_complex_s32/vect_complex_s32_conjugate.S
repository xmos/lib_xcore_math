// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)

#include "../asm_helper.h"

/*  
headroom_t vect_complex_s32_conjugate(
    complex_s32_t* a,
    const complex_s32_t* b,
    const unsigned length);
*/

.text
.issue_mode dual
.align 4

#define NSTACKVECTS     (1)
#define NSTACKWORDS     (2 + 8*(NSTACKVECTS))

#define STACK_VEC_TMP   (NSTACKWORDS - 8)

#define a           r0 
#define b           r1 
#define len         r2

#define _32         r3
#define tail        r4

#define FUNCTION_NAME vect_complex_s32_conjugate
    
.cc_top FUNCTION_NAME.function,FUNCTION_NAME
FUNCTION_NAME:
  dualentsp NSTACKWORDS
  std r4, r5, sp[0]
    
  { ldc _32, 32                 ; ldc r11, 0                  }
  { shl tail, len, 3            ; vsetc r11                   }
    ldaw r11, cp[vpu_vec_complex_conj_op]

  { zext tail, 5                ; shr len, len, 2             }
  { mkmsk tail, tail            ; bf len, .L_loop_bot         }
    
  .L_loop_top:
    { sub len, len, 1             ; vldr r11[0]                 }
    { add b, b, _32               ; vlmul b[0]                  }
    { add a, a, _32               ; vstr a[0]                   }
    {                             ; bt len, .L_loop_top         }
  .L_loop_bot:
  {                             ; bf tail, .L_done            }
  {                             ; vclrdr                      }
  {                             ; vldr r11[0]                 }
  { ldaw r11, sp[STACK_VEC_TMP] ; vlmul b[0]                  }
    vstrpv a[0], tail

  // To make sure the tail is captured in the headroom..
  {                             ; vstd r11[0]                 }
    vstrpv r11[0], tail
  {                             ; vldd r11[0]                 }
  {                             ; vstd r11[0]                 }

.L_done:
    ldd r4, r5, sp[0]
  {   ldc r0, 31                ;   vgetc r11                 }
  {   zext r11, 5               ;                             }
  {   sub r0, r0, r11           ;   retsp NSTACKWORDS         }
  
.L_func_end:
.cc_bottom FUNCTION_NAME.function

.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME

#undef FUNCTION_NAME

#endif //defined(__XS3A__)



