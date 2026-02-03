// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

.text
.align 4;

/*  
  The difference between this and chunk_q30_power_series() is that this one doesn't require
  the coefficient vector to contain redundant elements for each vector index. Instead, this version
  will broadcast the coefficient to a chunk. For this reason it is significantly slower, but it is
  also less wasteful of memory.

  NOTE: This hasn't (yet) been officially added to the API

    void chunk_q30_power_series_v2(
        int32_t a[],
        const q2_30 b[],
        const int32_t coef[],
        const unsigned term_count);
*/
#define FUNCTION_NAME   chunk_q20_power_series
#define NSTACKWORDS     (8 + 4 * 8)

#define VEC_POW   (NSTACKWORDS - 8-1)
#define VEC_ACC   (NSTACKWORDS - 16-1)
#define VEC_TMP   (NSTACKWORDS - 24-1) // -->  [coef, coef, 0, 0, 0, 0, 0, 0]
                                     // (last six elements must stay zeros)
#define VEC_COEF  (NSTACKWORDS - 30-1) // -->  [coef, coef, coef, coef, coef, coef, coef, coef]
                                     // (overlaps VEC_TMP and that's fine)

#define a           a0
#define b           a1
#define coef        a2
#define len         a3
#define vec_pow     s2
#define vec_acc     s3
#define vec_coef    s4
#define tmp         s5

FUNCTION_NAME:
xm.entsp (NSTACKWORDS)*4
xm.stdsp  s3,s2,0
xm.stdsp  s5,s4,8
{ addi tmp,sp, (VEC_TMP)*4              ; xm.vclrdr                             }
xm.stdsp  s7,s6,16
{ li t3, 0                              ; xm.vstd tmp                           }
{ addi len, len, -1                     ; xm.vsetc t3                           }
{ addi vec_pow,sp, (VEC_POW)*4          ; lw t3,0            ( coef)            }
xm.stdi  t3,t3, 0(tmp)
{ addi vec_acc,sp, (VEC_ACC)*4          ; xm.vldd tmp                           }
{ addi tmp,sp, (VEC_COEF)*4             ; xm.vfttf                              }
{ addi coef, coef, 4                    ; xm.vstd vec_coef                      }
{ nop                                   ; xm.vldc vec_coef                      }
la t3, vpu_vec_0x40000000
{ nop                                   ; xm.vldr t3                            }
{ nop                                   ; xm.vstr vec_pow                       }
{ nop                                   ; xm.vclrdr                             }
{ nop                                   ; xm.vlmacc0 vec_pow                    }
{ mv t3, vec_pow                        ; xm.vstr vec_acc                       }

  .L_loop_top:
  { nop                                 ; xm.vldr t3                            }
  { nop                                 ; xm.vlmul0 b                           }
  { mv t3, vec_acc                      ; xm.vstr vec_pow                       }
  { nop                                 ; xm.vldr t3                            }
  { addi coef, coef, 4                  ; lw t3,0            ( coef)            }
    xm.stdi  t3,t3, 0(tmp)
  { nop                                 ; xm.vldd tmp                           }
  { nop                                 ; xm.vfttf                              }
  { nop                                 ; xm.vstd vec_coef                      }
  { nop                                 ; xm.vldc vec_coef                      }
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



