// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

#define NSTACKWORDS     (8+8+4)

#define len         a3

.text
.p2align 4


/*  
headroom_t vect_s32_max_elementwise(
    int32_t a[],
    const int32_t b[],
    const int32_t c[],
    const unsigned len,
    const right_shift_t b_shr,
    const right_shift_t c_shr);
*/
#define FUNC_NAME   vect_s32_max_elementwise
FUNC_NAME:
  { li t3, 0                            ; xm.entsp (NSTACKWORDS)*4              }
  { slli t3, len, SIZEOF_LOG2_S32       ; xm.vsetc t3                           }
  { xm.zexti t3, 5                      ; srli len, len, EPV_LOG2_S32           }
    call vect_sXX_max_elementwise
  { li t3, 31                           ; nop                                   }
  { sub a0, t3, a0                      ; xm.retsp (NSTACKWORDS)*4              }
.L_end_s32:

.global FUNC_NAME
.type FUNC_NAME,@function
.resource_const FUNC_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNC_NAME, "tail_callees"
.resource_list_add   FUNC_NAME, "callees", vect_sXX_max_elementwise
.resource_list_empty FUNC_NAME, "parallel_callees"
.size FUNC_NAME, .L_end_s32 - FUNC_NAME
#undef FUNC_NAME
  


/*  
headroom_t vect_s16_max_elementwise(
    int16_t a[],
    const int16_t b[],
    const int16_t c[],
    const unsigned len,
    const right_shift_t b_shr,
    const right_shift_t c_shr);
*/
#define FUNC_NAME   vect_s16_max_elementwise
FUNC_NAME:
    xm.entsp (NSTACKWORDS)*4
    li t3, 0x0100
  { slli t3, len, SIZEOF_LOG2_S16       ; xm.vsetc t3                           }
  { xm.zexti t3, 5                      ; srli len, len, EPV_LOG2_S16           }
    call vect_sXX_max_elementwise
  { li t3, 15                           ; nop                                   }
  { sub a0, t3, a0                      ; xm.retsp (NSTACKWORDS)*4              }
.L_end_s16:

.global FUNC_NAME
.type FUNC_NAME,@function
.resource_const FUNC_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNC_NAME, "tail_callees"
.resource_list_add   FUNC_NAME, "callees", vect_sXX_max_elementwise
.resource_list_empty FUNC_NAME, "parallel_callees"
.size FUNC_NAME, .L_end_s16 - FUNC_NAME
#undef FUNC_NAME
  


/*  
headroom_t vect_s8_max_elementwise(
    int8_t a[],
    const int8_t b[],
    const int8_t c[],
    const unsigned len,
    const right_shift_t b_shr,
    const right_shift_t c_shr);
*/
#define FUNC_NAME   vect_s8_max_elementwise
FUNC_NAME:
    xm.entsp (NSTACKWORDS)*4
    li t3, 0x0200
  { mv t3, len                          ; xm.vsetc t3                           }
  { xm.zexti t3, 5                      ; srli len, len, EPV_LOG2_S8            }
    call vect_sXX_max_elementwise
  { li t3, 7                            ; nop                                   }
  { sub a0, t3, a0                      ; xm.retsp (NSTACKWORDS)*4              }
.L_end_s8:

.global FUNC_NAME
.type FUNC_NAME,@function
.resource_const FUNC_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNC_NAME, "tail_callees"
.resource_list_add   FUNC_NAME, "callees", vect_sXX_max_elementwise
.resource_list_empty FUNC_NAME, "parallel_callees"
.size FUNC_NAME, .L_end_s8 - FUNC_NAME
#undef FUNC_NAME



#undef len

#define STACK_VEC_TMP   (NSTACKWORDS-8-4)
#define STACK_BYTEMASK  6

#define a           a0 
#define b           a1 
#define c           a2
#define len         a3
#define shr_b       s2
#define shr_c       s3
#define _32         s4
#define tmp_vec     s5
#define bytemask    len



/**
 * WARNING: This does _NOT_ use the standard ABI. It assumes t3 will contain
 *          the length of the tail in bytes.
 */

vect_sXX_max_elementwise:
    xm.stdsp  s3,s2,8
    xm.stdsp  s5,s4,16

  { li _32, 32                          ; xm.vclrdr                             }
  mv shr_c, a5
  { xm.mkmsk t3, t3                     ; nop                                   }
  mv shr_b, a4
  { addi tmp_vec,sp, (STACK_VEC_TMP)*4  ; nop                                   }
  { xm.mkmski t3, 32                    ; sw t3, (STACK_BYTEMASK)*4             (sp) }
  { nop                                 ; xm.brff len, .L_loop_bot              }
  { nop                                 ; xm.bu .L_loop_top                     }

  // Deal with main vector body
.p2align 4
.L_loop_top:
    // Here we need to assume shr_b and shr_c have been chosen to guarantee 1 
    // bit of headroom in each so that c[k] - b[k] can't saturate. That means 
    // this should be perfectly accurate if there's already at least 1 bit of 
    // headroom in each input.
      xm.vlashr c, shr_c
      xm.vstrpv tmp_vec, t3
      xm.vlashr b, shr_b
      xm.vstrpv a, t3 
    { addi len, len, -1                 ; xm.vlsub tmp_vec                      }
    { add c, c, _32                     ; xm.vpos                               }
    { add b, b, _32                     ; xm.vladd a                            }
    { add a, a, _32                     ; xm.vstr a                             }
    { nop                               ; xm.bt len, .L_loop_top                }
.L_loop_bot:

    lw bytemask, (STACK_BYTEMASK)*4(sp)
    beqz bytemask, .L_finish
    xm.vlashr c, shr_c
    xm.vstrpv tmp_vec, bytemask
    xm.vlashr b, shr_b
    xm.vstrpv a, bytemask
    xm.vlsub tmp_vec
    xm.vpos
    mv t3, tmp_vec
    xm.vladd a
    xm.vstd tmp_vec
    xm.vstrpv tmp_vec, bytemask
    xm.vldr t3
    xm.vstr tmp_vec
    xm.vstrpv a, bytemask

.L_finish:
    xm.lddsp  s3,s2,8
    xm.lddsp  s5,s4,16
    xm.vgetc t3
    xm.zexti t3, 5
    mv a0, t3
    ret 

.L_end_sXX:

.resource_const vect_sXX_max_elementwise, "stack_frame_bytes", 0
.resource_list_empty vect_sXX_max_elementwise, "tail_callees"
.resource_list_empty vect_sXX_max_elementwise, "callees"
.resource_list_empty vect_sXX_max_elementwise, "parallel_callees"
.size vect_sXX_max_elementwise, . - vect_sXX_max_elementwise


#endif //defined(__VX4B__)



