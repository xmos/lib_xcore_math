// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


#include "../asm_helper.h"

/*  

float f32_power_series(
    const float x,
    const float coef[],
    const unsigned terms_count);
*/


#define NSTACKWORDS     (4)

#define FUNCTION_NAME   f32_power_series

#define x       r0
#define coef    r1
#define count   r2
#define acc     r3
#define tmpA    r4
#define pow     r5

// these unroll settings seem to offer the best
// tradeoff between code size and speed (3/8 should also work)
#define UNROLL_LOG2   2
#define UNROLL        4

#define CAT_(A, B)    A##B
#define CAT(A, B)     CAT_(A,B)

#define FULL_LOOP_LBL CAT(.L_loop_, UNROLL)

.text
.issue_mode dual
.align 4

.cc_top FUNCTION_NAME.function,FUNCTION_NAME

FUNCTION_NAME:
  dualentsp NSTACKWORDS
  std r4, r5, sp[1]

{ ldc acc, 0                  ; mov pow, x                  }
{ shr r11, count, UNROLL_LOG2 ;                             }

  .L_loop_top:
  { sub r11, count, UNROLL      ; bt r11, .L_loop_full        }
    shl r11, r11, 2
  { add coef, coef, r11         ; bru count                   }
    ecallf count
#if (UNROLL_LOG2 >= 1)
    bu .L_loop_1
#endif
#if (UNROLL_LOG2 >= 2)
    bu .L_loop_2
    bu .L_loop_3
#endif
#if (UNROLL_LOG2 >= 3)
    bu .L_loop_4
    bu .L_loop_5
    bu .L_loop_6
    bu .L_loop_7
#endif

    .L_loop_full:
#if (UNROLL_LOG2 >= 3)
    .L_loop_8:
    { sub count, count, 1         ; ldw tmpA, coef[UNROLL-8]    }
      fmacc acc, acc, pow, tmpA
      fmul pow, pow, x
    .L_loop_7:
    { sub count, count, 1         ; ldw tmpA, coef[UNROLL-7]    }
      fmacc acc, acc, pow, tmpA
      fmul pow, pow, x
    .L_loop_6:
    { sub count, count, 1         ; ldw tmpA, coef[UNROLL-6]    }
      fmacc acc, acc, pow, tmpA
      fmul pow, pow, x
    .L_loop_5:
    { sub count, count, 1         ; ldw tmpA, coef[UNROLL-5]    }
      fmacc acc, acc, pow, tmpA
      fmul pow, pow, x
#endif
#if (UNROLL_LOG2 >= 2)    
    .L_loop_4:
    { sub count, count, 1         ; ldw tmpA, coef[UNROLL-4]    }
      fmacc acc, acc, pow, tmpA
      fmul pow, pow, x
    .L_loop_3:
    { sub count, count, 1         ; ldw tmpA, coef[UNROLL-3]    }
      fmacc acc, acc, pow, tmpA
      fmul pow, pow, x
#endif
#if (UNROLL_LOG2 >= 1)
    .L_loop_2:
    { sub count, count, 1         ; ldw tmpA, coef[UNROLL-2]    }
      fmacc acc, acc, pow, tmpA
      fmul pow, pow, x
#endif
    .L_loop_1:
    { sub count, count, 1         ; ldw tmpA, coef[UNROLL-1]    }
      fmacc acc, acc, pow, tmpA
      fmul pow, pow, x

    ldaw coef, coef[UNROLL]
  { shr r11, count, UNROLL_LOG2   ; bt count, .L_loop_top       }


.L_finish:
  mov r0, acc
  ldd r4, r5, sp[1]
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



