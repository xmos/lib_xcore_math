// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


/*  

int64_t vect_s32_abs_sum(
    const int32_t b[],
    const unsigned length);
*/


#include "../asm_helper.h"

.text
.p2align 2

#define NSTACKVECS      (2)
#define NSTACKWORDS     (4 + 8*NSTACKVECS)

#define FUNCTION_NAME       vect_s32_abs_sum

#define STACK_VEC_TMP       (NSTACKWORDS-8-1)
#define STACK_VEC_VR        (NSTACKWORDS-16-1)

#define b           a0      // ![0x%08X]
#define N           a1      // ![%d]
#define tail        a2      // ![0x%X]
#define _32         a3      // ![%d]
#define tmp         s2      // ![%d]
#define mask        s3      // ![0x%X]



.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.align 16;

FUNCTION_NAME:

        xm.entsp (NSTACKWORDS)*4
        xm.stdsp  s3,s2,0

    { li t3, 0                          ; slli tail, N, SIZEOF_LOG2_S32         }
    { addi t3,sp, (STACK_VEC_VR)*4      ; xm.vsetc t3                           }
    { xm.zexti tail, 5                  ; xm.vclrdr                             }
    { srli N, N, EPV_LOG2_S32           ; xm.brff tail, .L_tail_dealt_with      }

    { addi tmp,sp, (STACK_VEC_TMP)*4    ; mv t3, b                              }
    { xm.mkmsk mask, tail               ; xm.vstd tmp                           }
    { addi t3,sp, (STACK_VEC_VR)*4      ; xm.vldr t3                            }
    { nop                               ; xm.vsign                              }
        xm.vstrpv tmp, mask
    { nop                               ; xm.vldc tmp                           }
    { nop                               ; xm.vclrdr                             }
    { add b, b, tail                    ; xm.vlmacc0 b                          }
.L_tail_dealt_with:
    { li _32, 32                        ; xm.brff N, .L_loop_bot_s32            }

.L_loop_top_s32:
        { mv t3, b                      ; xm.vstr t3                            }
        { addi N, N, -1                 ; xm.vldr t3                            }
        { addi t3,sp, (STACK_VEC_TMP)*4 ; xm.vsign                              }
        { nop                           ; xm.vstr t3                            }
        { addi t3,sp, (STACK_VEC_VR)*4  ; xm.vldc t3                            }
        { nop                           ; xm.vldr t3                            }
        { add b, b, _32                 ; xm.vlmacc0 b                          }
        { nop                           ; xm.bt N, .L_loop_top_s32              }
.L_loop_bot_s32:

.L_finish_s32:

    { nop                               ; xm.vstr t3                            }
la t3, vpu_vec_0x40000000
    { nop                               ; xm.vldc t3                            }
la t3, vpu_vec_0x80000000
    { nop                               ; xm.vlmacc0 t3                         }
la t3, vpu_vec_zero
    { addi t3,sp, (STACK_VEC_VR)*4      ; xm.vldr t3                            }
    { nop                               ; xm.vlmaccr0 t3                        }
    { nop                               ; xm.vstd t3                            }
    { nop                               ; xm.vlmaccr0 t3                        }
    { nop                               ; xm.vstr t3                            }
    { nop                               ; lw a1,0                          ( t3) }
    { addi a1, a1, 8                    ; lw a0,4                          ( t3) }

        xm.lddsp  s3,s2,0
        xm.retsp (NSTACKWORDS)*4


//.cc_bottom FUNCTION_NAME.function; 
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords; 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores; 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers; 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends; 
.L_end: 
    .size FUNCTION_NAME, .L_end - FUNCTION_NAME






#endif //defined(__VX4B__)
