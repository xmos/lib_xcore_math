// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// XMOS Public License: Version 1

#if defined(__VX4B__)


/*  
headroom_t vect_s16_nmacc(
    int16_t acc[],
    const int16_t b[],
    const int16_t c[],
    const unsigned len,
    const int acc_shr,
    const int bc_shr);
*/


#include "../asm_helper.h"

#define NSTACKVECTS     (2)
#define NSTACKWORDS     (8 + 8*NSTACKVECTS+4)

#define FUNCTION_NAME   vect_s16_nmacc

#define STACK_VEC_SAT   (NSTACKWORDS-8-4)
#define STACK_VEC_TMP   (NSTACKWORDS-16-4)

#define acc         a0 
#define b           a1 
#define c           a2
#define len         a3
#define bc_shr      x18
#define _32         x19
#define tmp         s4
#define tail        s5
#define acc_shr     x22
#define mask        s7

.text
.p2align 2


FUNCTION_NAME:
        xm.entsp (NSTACKWORDS)*4
        li t3, 0x100
        xm.stdsp  s3,s2,8
        xm.stdsp  s5,s4,16
        xm.stdsp  s7,s6,24
    { slli t3, len, SIZEOF_LOG2_S16     ; xm.vsetc t3                           }
    mv bc_shr, a5
    { xm.zexti t3, 5                    ; nop                                   }
    { slli tmp, bc_shr, 16              ; xm.zexti bc_shr, 16                   }
    { or bc_shr, tmp, bc_shr            ; srli len, len, EPV_LOG2_S16           }
    mv acc_shr, a4
    { addi tmp,sp, (STACK_VEC_TMP)*4    ; nop                                   }

        xm.stdsp  bc_shr,bc_shr,((STACK_VEC_SAT)/2 + 0)*8
        xm.stdsp  bc_shr,bc_shr,((STACK_VEC_SAT)/2 + 1)*8
        xm.stdsp  bc_shr,bc_shr,((STACK_VEC_SAT)/2 + 2)*8
        xm.stdsp  bc_shr,bc_shr,((STACK_VEC_SAT)/2 + 3)*8

    { addi bc_shr,sp, (STACK_VEC_SAT)*4 ; xm.mkmsk tail, t3                     }
    { li _32, 32                        ; xm.brff len, .L_loop_bot              }
    { xm.mkmski mask, 32                ; xm.bu .L_loop_top                     }

.p2align 4
.L_loop_top:
            xm.vlashr acc, acc_shr
            xm.vstrpv acc, mask
        { addi len, len, -1             ; xm.vclrdr                             }
        { add b, b, _32                 ; xm.vldc b                             }
        { nop                           ; xm.vlmacc0 c                          }
        xm.vlmacc1 c
        { add c, c, _32                 ; nop                                   }
     xm.vlsat bc_shr
        { nop                           ; xm.vlsub acc                          }
        { add acc, acc, _32             ; xm.vstr acc                           }
        { nop                           ; xm.bt len, .L_loop_top                }

.L_loop_bot:
    { nop                               ; xm.brff tail, .L_finish               }
      xm.vlashr acc, acc_shr
      xm.vstrpv acc, tail
    { nop                               ; xm.vclrdr                             }
    { nop                               ; xm.vldc b                             }
    { nop                               ; xm.vlmacc0 c                          }
    xm.vlmacc1 c
    { mv t3, bc_shr                     ; nop                                   }
     xm.vlsat bc_shr
    { nop                               ; xm.vlsub acc                          }
    { nop                               ; xm.vstd t3                            }
        xm.vstrpv t3, tail
        xm.vstrpv acc, tail
    { nop                               ; xm.vldr t3                            }
    { nop                               ; xm.vstr t3                            }

.L_finish:
        xm.lddsp  s3,s2,8
        xm.lddsp  s5,s4,16
        xm.lddsp  s7,s6,24
    { li a0, 15                         ; xm.vgetc t3                           }
    { xm.zexti t3, 5                    ; nop                                   }
    { sub a0, a0, t3                    ; xm.retsp (NSTACKWORDS)*4              } 

.L_func_end:


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS; .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;              .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;             .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;           .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME



#endif //defined(__VX4B__)



