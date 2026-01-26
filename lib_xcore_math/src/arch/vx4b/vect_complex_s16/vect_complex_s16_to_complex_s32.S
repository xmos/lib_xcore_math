// Copyright 2020-2022 XMOS LIMITED.
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
#define NSTACKWORDS     (4 + (8*NSTACKVECS)+4)

#define FUNCTION_NAME   vect_complex_s16_to_vect_complex_s32

#define a               x10
#define b_real          x11
#define b_imag          x12
#define length          x13
#define tmp_real        x18
#define tmp_imag        x19

.text;  /* Translation error on this line: unexpected token at position 5. */ 
.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.p2align 4


FUNCTION_NAME:
    xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in entsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
    xm.stdsp  s3,s2,8

    // Can this be done faster? Or with the VPU at all?
    .L_loop_top:  
        addi length, length, -1
        xm.ld16s tmp_real, length(b_real)
        xm.ld16s tmp_imag, length(b_imag)
        xm.std  tmp_real,tmp_imag, length(a)
        bnez length, .L_loop_top

    xm.lddsp  s3,s2,8
    xm.retsp (NSTACKWORDS)*4       /* Multiple XAT warnings: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in retsp NSTACKWORDS       \nMessage: 0th operand fits in 6 bit unsigned immediate", 'RETSP operand may need scaling' */

    //.cc_bottom FUNCTION_NAME.function;  /* Translation error on this line: unexpected token at position 37. */ 
    .set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords;  /* Translation error on this line: unexpected token at position 46. */ 
    .set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores;  /* Translation error on this line: unexpected token at position 33. */ 
    .set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers;  /* Translation error on this line: unexpected token at position 34. */ 
    .set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends;  /* Translation error on this line: unexpected token at position 36. */ 
    
.L_func_end:
    .size FUNCTION_NAME, .L_func_end - FUNCTION_NAME


#endif //defined(__VX4B__)
