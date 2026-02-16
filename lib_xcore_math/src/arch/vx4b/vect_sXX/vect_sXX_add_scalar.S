// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


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


#define STACK_VEC_TEMP      (NSTACKWORDS-12)

#define a           a0 
#define b           a1 
#define len         a2
#define c           a3
#define b_shr       s2
#define _32         s3
#define tail        s4


.text
.p2align 2

FUNCTION_NAME:
    xm.entsp (NSTACKWORDS)*4
    xm.stdsp  s3,s2,0
    xm.stdsp  s5,s4,8

  mv t3, a4
  { li _32, 32                          ; nop                                   }
  //{ li _32, 32                               ; lw t3, (STACK_D)*4                      (sp)}
    xm.stdsp  c,t3,((STACK_VEC_TEMP/2)+0)*8
    xm.stdsp  c,t3,((STACK_VEC_TEMP/2)+1)*8
    xm.stdsp  c,t3,((STACK_VEC_TEMP/2)+2)*8
    xm.stdsp  c,t3,((STACK_VEC_TEMP/2)+3)*8

#undef  c   // no longer needed
#define vec_tmp   a3

  mv t3, a6
  { addi vec_tmp,sp, (STACK_VEC_TEMP)*4 ; nop                                   }
  //{ addi vec_tmp,sp, (STACK_VEC_TEMP)*4          ; lw t3, (STACK_MODE_BITS)*4              (sp)}
  { mv tail, len                        ; xm.vsetc t3                           }
  mv b_shr, a5
  { srli len, len, 5                    ; nop                                   }
  //{ srli len, len, 5                           ; lw b_shr, (STACK_B_SHR)*4                (sp)}
  { xm.zexti tail, 5                    ; xm.brff len, .L_loop_bot              }

  .L_loop_top:
      xm.vlashr b, b_shr
    { addi len, len, -1                 ; xm.vladd vec_tmp                      }
    { add b, b, _32                     ; xm.vstr a                             }
    { add a, a, _32                     ; xm.bt len, .L_loop_top                }
  .L_loop_bot:

  { xm.mkmsk tail, tail                 ; xm.brff tail, .L_finish               }
  { nop                                 ; xm.vclrdr                             }
    xm.vlashr b, b_shr
  { nop                                 ; xm.vladd vec_tmp                      }
  { nop                                 ; xm.vstd vec_tmp                       }
    xm.vstrpv a, tail

  // These three are because the headroom mask doesn't get updated by VSTRPV
    xm.vstrpv vec_tmp, tail
  { nop                                 ; xm.vldd vec_tmp                       }
  { nop                                 ; xm.vstd vec_tmp                       }


.L_finish:
    xm.lddsp  s3,s2,0
    xm.lddsp  s5,s4,8

  { nop                                 ; xm.vgetc t3                           }
  { xm.zexti t3, 5                      ; nop                                   }
  { mv a0, t3                           ; xm.retsp (NSTACKWORDS)*4              } 

.L_func_end:


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME



#endif //defined(__VX4B__)



