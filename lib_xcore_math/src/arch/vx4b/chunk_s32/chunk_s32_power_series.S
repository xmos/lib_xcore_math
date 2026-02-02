// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

.text
.align 4;

/*  
    void chunk_q30_power_series(
        int32_t a[VPU_INT32_EPV],
        const q2_30 b[VPU_INT32_EPV],
        const int32_t coef[],
        const unsigned term_count);
*/
#define FUNCTION_NAME   chunk_q30_power_series
#define NSTACKWORDS     (8 + 2 * 8 + 4)

#define VEC_POW   (NSTACKWORDS - 8-4)
#define VEC_ACC   (NSTACKWORDS - 16-4)

#define a           a0
#define b           a1
#define coef        a2
#define len         a3
#define vec_pow     x18
#define vec_acc     x19
#define tmp         s4
#define _32         s5

FUNCTION_NAME:
xm.entsp (NSTACKWORDS)*4
xm.stdsp  s3,s2,0
xm.stdsp  s5,s4,8
{ addi len, len, -1                     ; nop                                   }
xm.stdsp  s7,s6,16

{ li t3, 0                              ; xm.vldc coef                          }
{ nop                                   ; xm.vsetc t3                           }
la t3, vpu_vec_0x40000000
{ addi vec_pow,sp, (VEC_POW)*4          ; xm.vldr t3                            }
{ addi vec_acc,sp, (VEC_ACC)*4          ; xm.vstr vec_pow                       }
{ li _32, 32                            ; xm.vclrdr                             }
{ add coef, coef, _32                   ; xm.vlmacc0 vec_pow                    }
{ mv t3, vec_pow                        ; xm.vstr vec_acc                       }

  .L_loop_top:
  { nop                                 ; xm.vldr t3                            }
  { nop                                 ; xm.vlmul0 b                           }
  { mv t3, vec_acc                      ; xm.vstr vec_pow                       }
  { nop                                 ; xm.vldr t3                            }
  { add coef, coef, _32                 ; xm.vldc coef                          }
  { addi len, len, -1                   ; xm.vlmacc0 vec_pow                    }
  { nop                                 ; xm.vstr vec_acc                       }
  { mv t3, vec_pow                      ; xm.bt len, .L_loop_top                }

{ nop                                   ; xm.vstr a                             }

.L_finish:
xm.lddsp  s3,s2,0
xm.lddsp  s5,s4,8
xm.lddsp  s7,s6,16
xm.retsp (NSTACKWORDS)*4

.L_func_end_unpack:

.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME,.L_func_end_unpack - FUNCTION_NAME

#undef NSTACKWORDS
#undef FUNCTION_NAME

#endif //defined(__VX4B__)



