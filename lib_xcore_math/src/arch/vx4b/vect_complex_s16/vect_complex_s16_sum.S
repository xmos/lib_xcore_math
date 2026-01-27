// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)

#include "../asm_helper.h"

/*  

complex_s16_t vect_complex_s16_sum(
    const int16_t* b_real,
    const int16_t* b_imag,
    const unsigned length,
    const right_shift_t sat);

*/

.text
.p2align 2

#define NSTACKVECS      (6)
#define NSTACKWORDS     (8+(8*NSTACKVECS)+4)

#define b_real      x10
#define b_imag      x11
#define length      x12
#define sat         x13
#define _32         x18
#define tmp         x19
#define tail_bytes  x20

#define STACK_VEC_ZEROS     (NSTACKWORDS- 8-16-4)
#define STACK_VEC_TMP       (NSTACKWORDS-16-16-4)
#define STACK_VEC_TMP2       (NSTACKWORDS-8-2)

#define STACK_LENGTH        6
#define STACK_SAT           7

#define FUNCTION_NAME vect_complex_s16_sum_OLLD //why did I think I needed to write this in assembly?
    
FUNCTION_NAME:

        xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
        xm.stdsp  s3,s2,8
        xm.stdsp  s5,s4,16
        xm.stdsp  s7,s6,24

     {   addi s6,sp, (STACK_VEC_TMP2)*4        ;   nop                                  }
         addi s7, s6, (-30)

        li t3, 0x0100
    {   addi tmp, t3, 1                         ;   xm.vsetc t3}
    {   srli length, length, 4                   ;   slli tail_bytes, length, 1               }
    { nop                                           ;   xm.zexti tail_bytes, 5                      }
    { nop                                           ;   sw length, (STACK_LENGTH)*4            (sp)}
    { nop                                           ;   sw sat, (STACK_SAT)*4                  (sp)}

    {   addi t3,sp, (STACK_VEC_ZEROS)*4           ;   xm.vclrdr                                  }
    {   addi t3,sp, (STACK_VEC_TMP)*4             ;   xm.vstd t3}

        xm.stdi  tmp,tmp, 0(t3)
        xm.stdi  tmp,tmp, 8(t3)
        xm.stdi  tmp,tmp, 16(t3)
        xm.stdi  tmp,tmp, 24(t3)

    {   li _32, 32                             ;   xm.vldc t3}
    {   addi t3,sp, (STACK_VEC_ZEROS)*4           ;   xm.brff length, .L_real_bot                  }/* Multiple XAT warnings: 'LDAWSP outside of known frame - offset may need correction', 'Instruction xm.brff can only branch forwards; this branch may need revising' */

    .L_real_top:

        {   addi length, length, -1                   ;   xm.vlmaccr0 b_real}
        xm.vlmaccr1 b_real
        {nop ; xm.vstd s6}    
        {nop ; xm.vldd s7}
        {nop ; xm.vstr s6}
        {nop ; xm.vldr s7}


        {   add b_real, b_real, _32                 ;   xm.bt length, .L_real_top                  }

    .L_real_bot:

    {   sub t3, t3, tail_bytes                ;   xm.brff tail_bytes, .L_real_end              }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
    { nop                                           ;   xm.vldc t3}
    {   xm.mkmski tmp, 2                            ;   xm.vlmaccr0 b_imag}
    xm.vlmaccr1 b_imag
        {nop ; xm.vstd s6}    
        {nop ; xm.vldd s7}
        {nop ; xm.vstr s6}
        {nop ; xm.vldr s7}

    {   addi t3,sp, (STACK_SAT)*4                 ;  nop /* xm.vadddr */                                  }
    {   addi t3,sp, 4                         ;   nop}
    xm.vlsat t3
        xm.vstrpv t3, tmp
    { nop                                           ;   lw a0, 4                           (sp)}

    .L_real_end:
    
    { nop                                           ;   lw length, (STACK_LENGTH)*4            (sp)}
    {   addi t3,sp, (STACK_VEC_ZEROS)*4           ;   xm.vclrdr                                  }


    .L_imag_top:

        {   addi length, length, -1                   ;   xm.vlmaccr0 b_imag}
        xm.vlmaccr1 b_imag
        {nop ; xm.vstd s6}    
        {nop ; xm.vldd s7}
        {nop ; xm.vstr s6}
        {nop ; xm.vldr s7}

        {   add b_imag, b_imag, _32                 ;   xm.bt length, .L_real_top                  }

    .L_imag_bot:

    {   sub t3, t3, tail_bytes                ;   xm.brff tail_bytes, .L_imag_end              }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
    { nop                                           ;   xm.vldc t3}
    {   xm.mkmski tmp, 2                            ;   xm.vlmaccr0 b_imag}
    xm.vlmaccr1 b_imag
        {nop ; xm.vstd s6}    
        {nop ; xm.vldd s7}
        {nop ; xm.vstr s6}
        {nop ; xm.vldr s7}

    {   addi t3,sp, (STACK_SAT)*4                 ;    nop /* xm.vadddr */                               }
    {   addi t3,sp, 4                         ;   nop}
    xm.vlsat t3
        xm.vstrpv t3, tmp
    { nop                                           ;   lw a0, 4                           (sp)}

    .L_imag_end:

.L_done:
        xm.lddsp  s3,s2,8
        xm.lddsp  s5,s4,16
        xm.retsp (NSTACKWORDS)*4/* Multiple XAT warnings: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in retsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate", 'RETSP operand may need scaling' */

.L_func_end:

.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords /* Translation error on this line: unexpected token at position 42. */ 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores /* Translation error on this line: unexpected token at position 29. */ 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers /* Translation error on this line: unexpected token at position 30. */ 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends /* Translation error on this line: unexpected token at position 32. */ 
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME

#undef FUNCTION_NAME



#endif //defined(__VX4B__)



