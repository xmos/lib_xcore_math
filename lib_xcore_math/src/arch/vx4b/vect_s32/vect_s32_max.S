// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


/*  

int32_t vect_s32_max(
    const int32_t b[],
    const unsigned length);


*/


#include "../asm_helper.h"

.text
.p2align 2

#define NSTACKVECS      (2)
#define NSTACKWORDS     (8 + 8*NSTACKVECS)

#define FUNCTION_NAME       vect_s32_max

#define STACK_VEC_TMP       (NSTACKWORDS-8-1)
#define STACK_VEC_CUR_MAX   (NSTACKWORDS-16-1)

#define b           a0      // ![0x%08X]
#define N           a1      // ![%d]
#define tail        a2      // ![0x%X]
#define tmp         a3      // ![%d]
#define tmz         s2      // ![%d]

.global FUNCTION_NAME
.type FUNCTION_NAME,@function

FUNCTION_NAME:

    xm.entsp (NSTACKWORDS)*4
    xm.stdsp  s3,s2,8
    xm.stdsp  s5,s4,16
    
    { li t3, 0                          ; slli tail, N, SIZEOF_LOG2_S32         }
    { srli N, N, EPV_LOG2_S32           ; xm.vsetc t3                           }
    { xm.zexti tail, 5                  ; slli tmp, N, 5                        }
    la t3, vpu_vec_0x80000000
    { addi t3,sp, (STACK_VEC_CUR_MAX)*4 ; xm.vldr t3                            }
    { add t3, b, tmp                    ; xm.vstr t3                            }
    { xm.mkmsk tail, tail               ; xm.vldr t3                            }
    { addi t3,sp, (STACK_VEC_CUR_MAX)*4 ; nop                                   }
        xm.vstrpv t3, tail
    
    // Tail is fully accounted for in cur_max now.

#undef tail
#define cur_max     a2      // ![0x%08X]
    
    { addi tmp,sp, (STACK_VEC_TMP)*4    ; mv cur_max, t3                        }
    { nop                               ; xm.vclrdr                             }
    { mv t3, b                          ; xm.brff N, .L_loop_bot                }

    .L_loop_top:
        // cur_max[] saved in stack

        { mv b, t3                      ; xm.vldr t3                            } //  vR[i] = b[i]
        { addi N, N, -1                 ; xm.vlsub cur_max                      } //  vR[i] = cur_max[i] - b[i]
        { addi t3,sp, 0                 ; xm.vdepth1                            } //  vR[0] = [bitmask -- 1 where vR[i] < 0]  b[i] > cur_max[i]
        { xm.mkmski tmp, 1              ; nop                                   }
            xm.vstrpv t3, tmp
        { mv t3, b                      ; lw tmp, 0                          (sp) }
        { mv tmz, tmp                   ; xm.vldr t3                            }
xm.zip tmz, tmp, 0
        { mv tmz, tmp                   ; li t3, 32                             }
xm.zip tmz, tmp, 0
            xm.vstrpv cur_max, tmp
        { add t3, b, t3                 ; xm.bt N, .L_loop_top                  }
.L_loop_bot:


    { addi t3,sp, (STACK_VEC_CUR_MAX)*4 ; lw cur_max, (STACK_VEC_CUR_MAX)*4(sp) }
    { li N, 7                           ; addi t3, t3, 4                        }
    .L_loop2_top:
        { addi N, N, -1                 ; lw a0,0                      ( t3)    }
        { xm.slt tmp, a0, cur_max       ; nop                                   }
        { xm.shli tmp, tmp, 1           ; nop                                   }
        { addi t3, t3, 4                ; xm.bru tmp                            }
            { mv cur_max, a0            ; nop                                   }
        { mv a0, cur_max                ; xm.bt N, .L_loop2_top                 }

        xm.lddsp  s5,s4,16
        xm.lddsp  s3,s2,8
        xm.retsp (NSTACKWORDS)*4


//.cc_bottom FUNCTION_NAME.function; 
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords; 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores; 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers; 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends; 
.L_end: 
    .size FUNCTION_NAME, .L_end - FUNCTION_NAME






#endif //defined(__VX4B__)
