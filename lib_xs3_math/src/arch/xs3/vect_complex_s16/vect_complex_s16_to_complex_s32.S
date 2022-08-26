// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


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

#define VEC_TMP_REAL    (NSTACKWORDS - 8)
#define VEC_TMP_IMAG    (NSTACKWORDS - 16)

#define a               r0
#define b_real          r1
#define b_imag          r2
#define length          r3
#define tmp_real        r4
#define tmp_imag        r5

.text; 
.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.align 16
.cc_top FUNCTION_NAME.function,FUNCTION_NAME

.issue_mode single

FUNCTION_NAME:
    entsp NSTACKWORDS
    std r4, r5, sp[1]

    // Can this be done faster? Or with the VPU at all?
    .L_loop_top:  
        sub length, length, 1
        ld16s tmp_real, b_real[length]
        ld16s tmp_imag, b_imag[length]
        std tmp_imag, tmp_real, a[length]
        bt length, .L_loop_top

    ldd r4, r5, sp[1]
    retsp NSTACKWORDS       

    .cc_bottom FUNCTION_NAME.function; 
    .set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords; 
    .set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores; 
    .set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers; 
    .set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends; 
    
.L_func_end:
    .size FUNCTION_NAME, .L_func_end - FUNCTION_NAME


#endif //defined(__XS3A__)