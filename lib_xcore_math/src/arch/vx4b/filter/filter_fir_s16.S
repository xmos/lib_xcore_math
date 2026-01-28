// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.



#if defined(__VX4B__)

#include "../asm_helper.h"

/*  

typedef struct {
    unsigned num_taps;
    right_shift_t shift;
    int16_t* coef;
    int16_t* state;
} filter_fir_s16_t;

int16_t filter_fir_s16(
    filter_fir_s16_t* filter,
    const int16_t new_sample);
*/

#define FUNCTION_NAME filter_fir_s16

#define NSTACKVECS      (2)
#define NSTACKWORDS     (12+8*NSTACKVECS)

#define FILT_N          0
#define FILT_SHIFT      1
#define FILT_COEF       2
#define FILT_STATE      3


#define STACK_VEC_TMP   (NSTACKWORDS-8)
#define STACK_VEC_VR    (NSTACKWORDS-16)

#define STACK_FILTER    (8)

#define buff        x10
#define length      x11
#define sample      x12
#define tmpA        x13
#define _32         x18
#define coef        x19
#define filter      x24
    
.text
.globl FUNCTION_NAME; /* Translation error on this line: unexpected token at position 20. */ 
.type FUNCTION_NAME,@function
.p2align 4

FUNCTION_NAME:
        xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */

        xm.stdsp  s3,s2,8
        xm.stdsp  s5,s4,16
        xm.stdsp  s7,s6,24
    {   li _32, 32                             ;   sw s8, 4                          (sp)}
    {   mv filter, a0                          ;   mv sample, a1                          }
    { nop                                           ;   lw length,(FILT_N)*4              ( filter)}
    { nop                                           ;   lw buff,(FILT_STATE)*4            ( filter)}
        call filter_fir_s16_push_sample_up
    { nop                                           ;   lw coef,(FILT_COEF)*4             ( filter)}
    { nop                                           ;   lw buff,(FILT_STATE)*4            ( filter)}
    { nop                                           ;   lw length,(FILT_N)*4              ( filter)}
    {   slli t3, _32, 3                         ;   xm.vclrdr                                  }
    {   addi t3,sp, (STACK_VEC_TMP)*4             ;   xm.vsetc t3}
    {   mv tmpA, length                        ;   xm.vstd t3}
    {   xm.zexti tmpA, 4                            ;   srli length, length, 4                   }
    {   slli tmpA, tmpA, 1                       ;   xm.brff length, .L_loop_end                  }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
    
    .L_loop_top:
        {   add buff, buff, _32                     ;   xm.vldc buff}
        {   addi length, length, -1                   ;   xm.vlmaccr0 coef}
        xm.vlmaccr1 coef
        {   add coef, coef, _32                     ;   xm.bt length, .L_loop_top                  }
    .L_loop_end:

    {   addi a2,sp, (STACK_VEC_VR)*4               ;   xm.mkmsk tmpA, tmpA                            }
    {   mv t3, buff                           ;   xm.vstr a2}
    {   addi t3,sp, (STACK_VEC_TMP)*4             ;   xm.vldr t3}
        xm.vstrpv t3, tmpA
    {   li tmpA, 0                             ;   lw a2,(FILT_SHIFT)*4                  ( filter)}
    {   addi t3,sp, (STACK_VEC_VR)*4              ;   xm.vldc t3}
    { nop                                           ;   xm.vldr t3}
    { nop                                           ;   xm.vlmaccr0 coef}
    xm.vlmaccr1 coef
    //{ nop                                           ;   xm.vadddr                                      }
    xm.st16 x12, tmpA(x28)

    xm.vlsat t3
    { nop                                           ;   xm.vstr t3}
    xm.ld16s x10, tmpA(x28) 
.L_done:
    { nop                                           ;   lw s8, 4                          (sp)}
        xm.lddsp  s7,s6,24
        xm.lddsp  s5,s4,16
        xm.lddsp  s3,s2,8
        xm.retsp (NSTACKWORDS)*4/* Multiple XAT warnings: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in retsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate", 'RETSP operand may need scaling' */

//.cc_bottom FUNCTION_NAME.function;  /* Translation error on this line: unexpected token at position 33. */ 
.set FUNCTION_NAME.nstackwords,NSTACKWORDS + filter_fir_s16_push_sample_up.nstackwords;      /* Translation error on this line: unexpected token at position 86. */ 
.global FUNCTION_NAME.nstackwords;  /* Translation error on this line: unexpected token at position 33. */ 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores;  /* Translation error on this line: unexpected token at position 29. */ 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers;  /* Translation error on this line: unexpected token at position 30. */ 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends;  /* Translation error on this line: unexpected token at position 32. */ 
.L_size_end:
    .size FUNCTION_NAME, .L_size_end - FUNCTION_NAME

#undef FUNCTION_NAME



#endif //defined(__VX4B__)



