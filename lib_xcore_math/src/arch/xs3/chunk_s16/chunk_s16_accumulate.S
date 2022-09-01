// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)

#include "../asm_helper.h"

.text
.align 16;
.issue_mode dual

/*  

The first time this is called on a vector, `vpu_init` should be set
to 0x0100  (16-bit mode with no headroom mask).  This function will
vsetc with that value, and the result of vgetc will be returned at
the end of this function. This way the caller need not repeatedly
compare headroom for each chunk with the minimum found so far.

Instead, after all chunks have been processed, the headroom can be
computed from the final value returned.

unsigned chunk_s16_accumulate(
    split_acc_s32_t* acc,
    const int16_t b[VPU_INT16_EPV],
    const right_shift_t b_shr,
    const unsigned vpu_ctrl);
*/
#define FUNCTION_NAME   chunk_s16_accumulate
#define NSTACKWORDS     (2 + 8)

#define STACK_VEC_C     (NSTACKWORDS - 8)

#define acc       r0
#define b         r1
#define b_shr     r2
#define vec_c     r3



.cc_top FUNCTION_NAME.function,FUNCTION_NAME
FUNCTION_NAME:
  dualentsp NSTACKWORDS

{ mov r11, r3                 ; vldd acc[0]                 }
{ clz r11, b_shr              ; vsetc r11                   }
{ ldaw vec_c, sp[STACK_VEC_C] ; bf r11, .L_b_shr_neg        }

.L_b_shr_pos:
    // non-neg b_shr means we want vlashr
    ldaw r11, cp[vpu_vec_0x0001]
    vlashr b[0], b_shr
  { mkmsk b_shr, 32             ; vldc r11[0]                 }
    vstrpv vec_c[0], b_shr
    ldaw r11, acc[8]
  {                             ; vldd acc[0]                 }
  {                             ; vldr r11[0]                 }
  {                             ; vlmacc vec_c[0]             }
  {                             ; vstd acc[0]                 }
    vstrpv r11[0], b_shr
  {                             ; vgetc r11                   }
  { mov r0, r11                 ; retsp NSTACKWORDS           }

.L_b_shr_neg:
    // neg b_shr means we want to set c[] to a power of 2
    ldaw r11, cp[vpu_vec_0x0001]
    vlashr r11[0], b_shr
  { mkmsk b_shr, 32             ; vldd acc[0]                 }
    vstrpv vec_c[0], b_shr
  {                             ; vldc vec_c[0]               }
    ldaw r11, acc[8]
  {                             ; vldr r11[0]                 }
  {                             ; vlmacc b[0]                 }
  {                             ; vstd acc[0]                 }
    vstrpv r11[0], b_shr
  {                             ; vgetc r11                   }
  { mov r0, r11                 ; retsp NSTACKWORDS           }

.L_func_end_unpack:
.cc_bottom FUNCTION_NAME.function

.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME,.L_func_end_unpack - FUNCTION_NAME

#undef NSTACKWORDS
#undef FUNCTION_NAME


#endif //defined(__XS3A__)



