// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


/*  
int64_t vect_s32_dot(
    const int32_t b[],
    const int32_t c[],
    const unsigned length,
    const int b_shr,
    const int c_shr);
*/

#include "../asm_helper.h"

#define NSTACKWORDS     (8 + 3*8)

#define FUNCTION_NAME   vect_s32_dot

#define STACK_VEC_TMP   (NSTACKWORDS-8-1)
#define STACK_VEC_VR    (NSTACKWORDS-16-1)
#define STACK_VEC_VD    (NSTACKWORDS-24-1)

#define b           a0
#define c           a1
#define N           a2
#define b_shr       a3
#define c_shr       s2
#define tail        s3
#define vec_vd      s4
#define vec_vr      s5
#define vec_tmp     s6
#define _32         s7



.text
.p2align 2


FUNCTION_NAME:

        xm.entsp (NSTACKWORDS)*4
        xm.stdsp  s3,s2,0
        xm.stdsp  s5,s4,8
        xm.stdsp  s7,s6,16

    { li t3, 0                          ; nop                                   }
    { slli tail, N, SIZEOF_LOG2_S32     ; xm.vsetc t3                           }
    { xm.zexti tail, 5                  ; xm.vclrdr                             }
    { srli N, N, EPV_LOG2_S32           ; addi vec_tmp,sp, (STACK_VEC_TMP)*4    }
    mv c_shr, a4

{ xm.ldawsp vec_vr, (STACK_VEC_VD + 1)*4 ; nop                                   }
{ xm.ldawsp vec_vd, (STACK_VEC_VD    )*4 ; nop                                   }
    { li _32, 32                        ; xm.brff N, .L_loop_bot_s32            }

.L_loop_top_s32:
        { add vec_vd, vec_vd, _32       ; xm.vstd vec_vd                        }
        { sub vec_vd, vec_vd, _32       ; xm.vstr vec_vd                        }
            xm.vlashr b, b_shr
        { add b, b, _32                 ; xm.vstr vec_tmp                       }
        { nop                           ; xm.vldc vec_tmp                       }
            xm.vlashr c, c_shr
        { mv t3, vec_vr                 ; xm.vstr vec_tmp                       }
        { sub vec_vr, vec_vr, _32       ; xm.vldr t3                            }
        { add vec_vr, vec_vr, _32       ; xm.vldd vec_vr                        }  
        { addi N, N, -1                 ; xm.vlmaccr0 vec_tmp                   }
        { add c, c, _32                 ; xm.bt N, .L_loop_top_s32              }
.L_loop_bot_s32:
    { xm.mkmsk tail, tail               ; xm.brff tail, .L_finish_s32           }
    { add vec_vd, vec_vd, _32           ; xm.vstd vec_vd                        }
    { sub vec_vd, vec_vd, _32           ; xm.vstr vec_vd                        }
    { nop                               ; xm.vclrdr                             }
        xm.vlashr b, b_shr
    { add b, b, _32                     ; xm.vstd vec_tmp                       }
        xm.vstrpv vec_tmp, tail
    { nop                               ; xm.vldc vec_tmp                       }
        xm.vlashr c, c_shr
    { mv t3, vec_vr                     ; xm.vstr vec_tmp                       }
    { sub vec_vr, vec_vr, _32           ; xm.vldr t3                            }
    { nop                               ; xm.vldd vec_vr                        }  
    { nop                               ; xm.vlmaccr0 vec_tmp                   }

.L_finish_s32:

    { addi t3,sp, (STACK_VEC_TMP)*4     ; nop                                   }
    // (vD:vR)[k] ==  ((int32_t)vD[k])*(2^32) + ((uint32_t)vR[k])
    { nop                               ; xm.vstd t3                            }
    { nop                               ; lw a1,0                          ( t3) }
{ xm.sext a1, 8                         ; nop                                   }
    { nop                               ; xm.vstr t3                            }
    { nop                               ; lw a0,0                          ( t3) }
    
        xm.lddsp  s3,s2,0
        xm.lddsp  s5,s4,8
        xm.lddsp  s7,s6,16
        xm.retsp (NSTACKWORDS)*4

.L_func_end:


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS; .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;              .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;             .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;           .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME






#endif //defined(__VX4B__)
