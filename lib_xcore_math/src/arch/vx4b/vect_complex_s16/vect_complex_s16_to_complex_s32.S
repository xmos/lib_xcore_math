// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


/*  
void vect_complex_s16_to_vect_complex_s32(
    complex_s32_t* a,
    const int16_t* b_real,
    const int16_t* b_imag,
    const unsigned length);
*/

#include "../asm_helper.h"

#define NSTACKVECS      0
#define NSTACKWORDS     (4 + (8*NSTACKVECS))

#define FUNCTION_NAME   vect_complex_s16_to_vect_complex_s32

#define a               a0
#define b_real          a1
#define b_imag          a2
#define length          a3
#define tmp_real        s2
#define tmp_imag        s3

.text; 
.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.p2align 4


FUNCTION_NAME:
    xm.entsp (NSTACKWORDS)*4
    xm.stdsp  s3,s2,0

    // Can this be done faster? Or with the VPU at all?
    .L_loop_top:  
        addi length, length, -1
        xm.ld16s tmp_real, length(b_real)
        xm.ld16s tmp_imag, length(b_imag)
        xm.std  tmp_real,tmp_imag, length(a)
        bnez length, .L_loop_top

    xm.lddsp  s3,s2,0
    xm.retsp (NSTACKWORDS)*4       

    //.cc_bottom FUNCTION_NAME.function; 
.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
    
.L_func_end:
    .size FUNCTION_NAME, .L_func_end - FUNCTION_NAME


#endif //defined(__VX4B__)
