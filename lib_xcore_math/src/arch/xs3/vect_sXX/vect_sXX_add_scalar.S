// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


#include "../asm_helper.h"

/*  

unsigned vect_sXX_add_scalar(
    int32_t a[],
    const int32_t b[],
    const unsigned length_bytes,
    const int32_t c,
    const int32_t d,
    const right_shift_t b_shr,
    const unsigned mode_bits);

*/


#define NSTACKVECTS     (1)
#define NSTACKWORDS     (8+8*(NSTACKVECTS))

#define FUNCTION_NAME   vect_sXX_add_scalar

#define STACK_D             (NSTACKWORDS+1)
#define STACK_B_SHR         (NSTACKWORDS+2)
#define STACK_MODE_BITS     (NSTACKWORDS+3)

#define STACK_VEC_TEMP      (NSTACKWORDS-8)

#define a           r0 
#define b           r1 
#define len         r2
#define c           r3
#define b_shr       r4
#define _32         r5
#define tail        r6


.text
.issue_mode dual
.align 4

.cc_top FUNCTION_NAME.function,FUNCTION_NAME
FUNCTION_NAME:
    dualentsp NSTACKWORDS
    std r4, r5, sp[0]
    std r6, r7, sp[1]

  { ldc _32, 32                               ; ldw r11, sp[STACK_D]                      }
    std r11, c, sp[(STACK_VEC_TEMP/2)+0]
    std r11, c, sp[(STACK_VEC_TEMP/2)+1]
    std r11, c, sp[(STACK_VEC_TEMP/2)+2]
    std r11, c, sp[(STACK_VEC_TEMP/2)+3]

#undef  c   // no longer needed
#define vec_tmp   r3

  { ldaw vec_tmp, sp[STACK_VEC_TEMP]          ; ldw r11, sp[STACK_MODE_BITS]              }
  { mov tail, len                             ; vsetc r11                                 }
  { shr len, len, 5                           ; ldw b_shr, sp[STACK_B_SHR]                }
  { zext tail, 5                              ; bf len, .L_loop_bot                       }

  .L_loop_top:
      vlashr b[0], b_shr
    { sub len, len, 1                           ; vladd vec_tmp[0]                          }
    { add b, b, _32                             ; vstr a[0]                                 }
    { add a, a, _32                             ; bt len, .L_loop_top                       }
  .L_loop_bot:

  { mkmsk tail, tail                          ; bf tail, .L_finish                        }
  {                                           ; vclrdr                                    }
    vlashr b[0], b_shr
  {                                           ; vladd vec_tmp[0]                          }
  {                                           ; vstd vec_tmp[0]                           }
    vstrpv a[0], tail

  // These three are because the headroom mask doesn't get updated by VSTRPV
    vstrpv vec_tmp[0], tail
  {                                           ; vldd vec_tmp[0]                           }
  {                                           ; vstd vec_tmp[0]                           }


.L_finish:
    ldd r4, r5, sp[0]
    ldd r6, r7, sp[1]

  {                                           ;   vgetc r11                               }
  {   zext r11, 5                             ;                                           }
  {   mov r0, r11                             ;   retsp NSTACKWORDS                       } 

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



