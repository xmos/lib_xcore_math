// Copyright 2021-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// XMOS Public License: Version 1

#if defined(__VX4B__)


#include "../asm_helper.h"

/*  
    void vect_s32_zip(
        complex_s32_t a[],
        const int32_t b[],
        const int32_t c[],
        const unsigned length,
        const right_shift_t b_shr,
        const right_shift_t c_shr);
*/


#define NSTACKWORDS     (8+2*8+4)

#define FUNCTION_NAME   vect_s32_zip

#define STACK_VEC_C     (NSTACKWORDS-8-4)
#define STACK_VEC_B     (NSTACKWORDS-16-4)


#define a         a0
#define b         a1
#define c         a2
#define len       a3
#define b_shr     s2
#define c_shr     s3

#define vec_B     s4
#define vec_C     s5
#define _28       s6
#define _32       s7


.text
.p2align 2


FUNCTION_NAME:
    xm.entsp (NSTACKWORDS)*4
    xm.stdsp  s3,s2,0
    xm.stdsp  s5,s4,8
    xm.stdsp  s7,s6,16
  { li t3, 0                            ; sw s8, 24                            (sp) }
  { slli t3, len, SIZEOF_LOG2_S32       ; xm.vsetc t3                           }
  { xm.zexti t3, 5                      ; srli len, len, EPV_LOG2_S32           }
  { addi vec_B,sp, (STACK_VEC_B)*4      ; sw t3, 28                            (sp) }
  { li t3, 2                            ; xm.vclrdr                             }
{ xm.bitrev t3, t3                      ; nop                                   }
xm.vstd vec_B
  { addi vec_C,sp, (STACK_VEC_C)*4      ; sw t3,0                         ( vec_B) }
  { li _32, 32                          ; xm.vldc vec_B                         }
  mv b_shr, a4
  { li _28, 28                          ; nop                                   }

  mv c_shr,a5
  { nop                                 ; xm.brff len, .L_loop_bot              }
//  { nop                                           ; xm.bu .L_loop_top                            }

  .p2align 4
  .L_loop_top:
      xm.vlashr b, b_shr
    { add b, b, _32                     ; xm.vstr vec_B                         }
      xm.vlashr c, c_shr
    { add a, a, _32                     ; addi len, len, -1                     }

    { add vec_C, vec_C, _28             ; xm.vstr vec_C                         }
    { add vec_B, vec_B, _28             ; xm.vclrdr                             }
    { addi vec_C, vec_C, -4             ; xm.vlmaccr0 vec_C                     }
    { addi vec_B, vec_B, -4             ; xm.vlmaccr0 vec_B                     }

    { addi vec_C, vec_C, -4             ; xm.vlmaccr0 vec_C                     }
    { addi vec_B, vec_B, -4             ; xm.vlmaccr0 vec_B                     }
    { addi vec_C, vec_C, -4             ; xm.vlmaccr0 vec_C                     }
    { addi vec_B, vec_B, -4             ; xm.vlmaccr0 vec_B                     }
    //FNOP
    { addi vec_C, vec_C, -4             ; xm.vlmaccr0 vec_C                     }
    { addi vec_B, vec_B, -4             ; xm.vlmaccr0 vec_B                     }
    { sub t3, a, _32                    ; xm.vstr a                             }
    { add a, a, _32                     ; xm.vclrdr                             }

    { addi vec_C, vec_C, -4             ; xm.vlmaccr0 vec_C                     }
    { addi vec_B, vec_B, -4             ; xm.vlmaccr0 vec_B                     }
    { addi vec_C, vec_C, -4             ; xm.vlmaccr0 vec_C                     }
    { addi vec_B, vec_B, -4             ; xm.vlmaccr0 vec_B                     }
    //FNOP
    { addi vec_C, vec_C, -4             ; xm.vlmaccr0 vec_C                     }
    { addi vec_B, vec_B, -4             ; xm.vlmaccr0 vec_B                     }
    { addi vec_C, vec_C, -4             ; xm.vlmaccr0 vec_C                     }
    { addi vec_B, vec_B, -4             ; xm.vlmaccr0 vec_B                     }
    
    { add c, c, _32                     ; nop                                   }
    { addi vec_B,sp, (STACK_VEC_B)*4    ; xm.vstr t3                            }
    { addi vec_C,sp, (STACK_VEC_C)*4    ; xm.bt len, .L_loop_top                }
  .L_loop_bot:
  
  { nop                                 ; lw len, 28                              (sp) }
  { srli len, len, SIZEOF_LOG2_S32      ; xm.brff len, .L_finish                }
    xm.vlashr b, b_shr
  { nop                                 ; xm.vstr vec_B                         }
    xm.vlashr c, c_shr
  { addi len, len, -1                   ; xm.vstr vec_C                         }

#define tmpB  s6
#define tmpC  s7
  .L_tail_loop_top:
    { nop                               ; xm.ldw tmpB,len                        ( vec_B) }
    { nop                               ; xm.ldw tmpC,len                        ( vec_C) }
      xm.std  tmpB,tmpC, len(a)
    { addi len, len, -1                 ; xm.bt len, .L_tail_loop_top           }
  .L_tail_loop_bot:

  .L_finish:
      xm.lddsp  s3,s2,0
      xm.lddsp  s5,s4,8
      xm.lddsp  s7,s6,16
    { nop                               ; lw s8, 24                            (sp) }
    { nop                               ; xm.retsp (NSTACKWORDS)*4              } 


.L_func_end:


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME


#endif //defined(__VX4B__)



