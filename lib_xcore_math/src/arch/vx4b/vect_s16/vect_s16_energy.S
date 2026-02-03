// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


/*  
int32_t vect_s16_energy(
    const int16_t b[],
    const unsigned length,
    const right_shift_t b_shr);
*/


#include "../asm_helper.h"

.text
.p2align 2

#define NSTACKVECS      (4)
#define NSTACKWORDS     (8 + 8*NSTACKVECS+4)


#define FUNCTION_NAME   vect_s16_energy

#define STACK_VEC_TMP       (NSTACKWORDS-8-16-4)
#define STACK_VEC_VR        (NSTACKWORDS-16-16-4)
#define STACK_VEC_TMP2       (NSTACKWORDS-8-2)

#define b           a0
#define N           a1
#define b_shr       a2
#define vec_tmp     a3
#define tail        s2

.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.align 16;

FUNCTION_NAME:

        xm.entsp (NSTACKWORDS)*4
        xm.stdsp  s3,s2,0
        xm.stdsp  s5,s4,8

     { addi s4,sp, (STACK_VEC_TMP2)*4   ; nop                                   }
         addi s5, s4, (-30)
        li t3, 0x100
    { nop                               ; addi vec_tmp,sp, (STACK_VEC_TMP)*4    }
    { slli tail, N, SIZEOF_LOG2_S16     ; xm.vsetc t3                           }
    { xm.zexti tail, 5                  ; xm.vclrdr                             }
    { srli N, N, EPV_LOG2_S16           ; xm.brff tail, .L_tail_dealt_with_s16  }

    { nop                               ; slli N, N, 5                          }
    { add t3, b, N                      ; xm.vstd vec_tmp                       }
    { xm.mkmsk tail, tail               ; nop                                   }
        xm.vlashr t3, b_shr
        xm.vstrpv vec_tmp, tail
#undef tail

    { nop                               ; xm.vldc vec_tmp                       }
    { nop                               ; xm.vclrdr                             }
    { srli N, N, 5                      ; xm.vlmaccr0 vec_tmp                   }
    xm.vlmaccr1 vec_tmp
        { nop                           ; xm.vstd s4                            }    
        { nop                           ; xm.vldd s5                            }
        { nop                           ; xm.vstr s4                            }
        { nop                           ; xm.vldr s5                            }

.L_tail_dealt_with_s16:
    { addi t3,sp, (STACK_VEC_VR)*4      ; xm.brff N, .L_loop_bot_s16            }


.L_loop_top_s16:
        { li t3, 32                     ; xm.vstr t3                            }
            xm.vlashr b, b_shr
        { add b, b, t3                  ; xm.vstr vec_tmp                       }
        { addi t3,sp, (STACK_VEC_VR)*4  ; xm.vldc vec_tmp                       }
        { nop                           ; xm.vldr t3                            }
        { addi N, N, -1                 ; xm.vlmaccr0 vec_tmp                   }
        xm.vlmaccr1 vec_tmp
        { nop                           ; xm.vstd s4                            }    
        { nop                           ; xm.vldd s5                            }
        { nop                           ; xm.vstr s4                            }
        { nop                           ; xm.vldr s5                            }
        { nop                           ; xm.bt N, .L_loop_top_s16              }
.L_loop_bot_s16:

.L_finish_s16:

    //{ nop                                           ;   xm.vadddr                                  }
    { nop                               ; xm.vstd vec_tmp                       }
       addi s4, vec_tmp, 32-2
    { nop                               ; lw a1, 0(s4)                          }
    { slli a1, a1, 16                   ; xm.vstr vec_tmp                       }
    { nop                               ; lw a0, 0(s4)                          }
        xm.lddsp  s3,s2,0
         xm.lddsp  s5,s4,8
    { xm.zexti a0, 16                   ; nop                                   }
    { or a0, a0, a1                     ; xm.retsp (NSTACKWORDS)*4              }


//.cc_bottom FUNCTION_NAME.function; 
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords; 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores; 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers; 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends; 
.L_end: 
    .size FUNCTION_NAME, .L_end - FUNCTION_NAME






#endif //defined(__VX4B__)
