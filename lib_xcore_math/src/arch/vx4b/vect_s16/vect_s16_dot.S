// Copyright 2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


/*  
int64_t vect_s16_dot(
    const int16_t b[],
    const int16_t c[],
    const unsigned length);
*/

#include "../asm_helper.h"

.text
.p2align 2

#define NSTACKVECTS     (4)
#define NSTACKWORDS     (2 + 2 + 8*NSTACKVECTS)

#define FUNCTION_NAME   vect_s16_dot

#define STACK_VEC_TMP   (NSTACKWORDS-8-2)
#define STACK_ACC_LOW   (NSTACKWORDS-16-2)
#define STACK_ACC_MID   (NSTACKWORDS-24-2)
#define STACK_ACC_HIGH  (NSTACKWORDS-32-2)

#define b           a0
#define c           a1
#define N           a2
#define vec_twos    a3
#define tail        s2
#define _32         s3

.globl FUNCTION_NAME
.type FUNCTION_NAME,@function

FUNCTION_NAME:
  xm.entsp (NSTACKWORDS * 4)
  li t3, 0x0100
  xm.stdsp s2, s3, 0

  { xm.shl tail, N, SIZEOF_LOG2_S16 ; xm.vsetc t3 }
  { xm.zext tail, 5 ; xm.vclrdr }
  { xm.shr N, N, EPV_LOG2_S16 ; xm.ldawsp t3, STACK_ACC_LOW * 4 }
  { xm.ldawsp t3, STACK_ACC_MID * 4 ; xm.vstd t3 }
  { xm.ldawsp t3, STACK_ACC_HIGH * 4 ; xm.vstd t3 }
  { li _32, 32 ; xm.vstd t3 }
  lla t3, vpu_vec_0x0002
  mv vec_twos, t3
  { mv t3, b ; xm.brff N, .L_loop_bot }

.L_loop_top:
  { xm.ldawsp t3, STACK_ACC_LOW * 4 ; xm.vclrdr }
  xm.vldr t3
  { add b, b, _32 ; xm.vldc b }
  xm.vlmacc0 c
  xm.vlmacc1 c
  { xm.ldawsp t3, STACK_VEC_TMP * 4 ; xm.vstr t3 }
  { xm.ldawsp t3, STACK_ACC_MID * 4 ; xm.vstd t3 }
  { xm.ldawsp t3, STACK_ACC_HIGH * 4 ; xm.vldr t3 }
  { add c, c, _32 ; xm.vldd t3 }

  { xm.ldawsp t3, STACK_VEC_TMP * 4; xm.vldc vec_twos }
  xm.vlmacc0 t3
  xm.vlmacc1 t3
  xm.ldawsp t3, STACK_ACC_HIGH * 4
  { xm.ldawsp t3, STACK_ACC_MID * 4 ; xm.vstd t3 }
  { add N, N, -1 ; xm.vstr t3 }
  { mv t3, b ; xm.bt N, .L_loop_top }

.L_loop_bot:
  { xm.mkmsk tail, tail ; xm.brff tail, .L_get_res }
  { li N, 1 ; xm.vclrdr }
  { xm.ldawsp b, STACK_VEC_TMP * 4 ; xm.vldr t3 }
  xm.vstd b
  xm.vstrpv b, tail
  { li tail, 0 ; xm.bu .L_loop_top }

.L_get_res:

  { li a0, 0 ; li a1, 0 }
  { xm.ldawsp t3, STACK_ACC_HIGH * 4 ; li a2, 0x1 }
  li a3, 0x8000

  // The high:mid pair holds twice the carry, so its limbs are worth 2^31 and 2^15.
  .set OFFSET, 0
  .rept 16
  lh s2, (0 * 2) + OFFSET (t3)
  xm.shl s2, s2, 16
  xm.maccs a1, a0, s2, a3
  lhu s2, (16 * 2) + OFFSET (t3)
  xm.maccs a1, a0, s2, a3
  lhu s2, (32 * 2) + OFFSET (t3)
  xm.maccs a1, a0, s2, a2

  .set OFFSET, OFFSET+2
  .endr

.L_finish:
  xm.lddsp s2, s3, 0
  xm.retsp (NSTACKWORDS * 4)

.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
.L_end:
  .size FUNCTION_NAME, .L_end - FUNCTION_NAME

#endif //defined(__VX4B__)
