// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

.text

.p2align 2
/*
void f32_unpack_s32(
    int32_t* mantissa,
    exponent_t* exp,
    float input);
*/
#define FUNCTION_NAME   f32_unpack_s32
#define NSTACKWORDS     (0)

#define mant_out    a0
#define exp_out     a1
#define input       a2
#define sign        a3
#define exp         t3

FUNCTION_NAME:
    xm.entsp (NSTACKWORDS)*4
    xm.fsexp sign, exp, input
    xm.fmants input, input

#undef input
#define mant        a2

    addi exp, exp, -23
{ nop                                   ; sw mant,0       ( mant_out)           }
{ nop                                   ; sw exp,0         ( exp_out)           }
    xm.retsp (NSTACKWORDS)*4
.L_func_end_unpack:

.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
.size FUNCTION_NAME,.L_func_end_unpack - FUNCTION_NAME

#undef NSTACKWORDS
#undef FUNCTION_NAME
#undef mant_out
#undef exp_out
#undef mant
#undef exp
#undef sign



/*
float s32_to_f32(
    const int32_t mantissa,
    const exponent_t exp);
*/
#define FUNCTION_NAME   s32_to_f32
#define NSTACKWORDS     (0)

#define mant        a0
#define exp         a1
#define zero        a2

.p2align 2
FUNCTION_NAME:

    xm.entsp (NSTACKWORDS)*4
  // 23 - 32 = -9
  // subtracting 32 becase we're putting mant in a high word to not extract the sign
  { add exp, exp, -9                    ; li zero, 0                            }
    xm.fmakes mant, exp, mant, zero
  { nop                                 ; xm.retsp (NSTACKWORDS)*4              }

.L_func_end_pack:

.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
.size FUNCTION_NAME,.L_func_end_pack - FUNCTION_NAME

#undef NSTACKWORDS
#undef FUNCTION_NAME


#endif //defined(__VX4B__)
